"""Check received/waiting data and internal layout using actual renderer logs."""
from collections import defaultdict
import json
from pathlib import Path
import re
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]


def check(output):
    header = (ROOT / "CH584_V1_0_1/APP/include/ui_menu_assets.h").read_text(encoding="utf-8")
    tables = {}
    for size in (11, 14, 16, 18):
        body = re.search(rf"ui_menu_glyphs_{size}\[\] = \{{(.*?)\}};", header, re.S).group(1)
        tables[size] = {int(code, 16): int(width) for code, width in
                        re.findall(r"\{0x([0-9a-f]+), (\d+)u,", body)}
    records = defaultdict(list)
    for line in (output / "after/draws.tsv").read_text(encoding="utf-8").splitlines():
        parts = line.split("\t")
        if parts[0] == "CUSTOM_TEXT" and parts[2] == "primary":
            x, y, size, missing = map(int, parts[3:7])
            records[parts[1]].append((x, y, size, parts[7]))
            assert not missing, line
    failures, checks = [], []

    def require(condition, description):
        checks.append(description)
        if not condition:
            failures.append(description)

    def width(record):
        return sum(tables[record[2]][ord(char)] for char in record[3])

    def count(case, text):
        return sum(row[3] == text for row in records[case])

    for case, rows in records.items():
        if case.startswith("home_") or case == "save_message_tick2_home":
            states = [row for row in rows if row[3].startswith("状态：")]
            for state in states:
                machine = next(row for row in rows if row[3].startswith("本机号："))
                station = next(row for row in rows if row[3].startswith("分站："))
                require(abs(state[0] * 2 + width(state) - 384) <= 1,
                        case + ": state horizontally centered")
                require(state[1] - state[2] + 1 >= 46,
                        case + ": status below logo bottom at y=40")
                require(machine[0] + width(machine) + 8 <= state[0] and
                        state[0] + width(state) + 8 <= station[0],
                        case + ": address/state/station separated")
        for label in ("名称:", "信号:", "电压:"):
            if any(row[3] == label for row in rows):
                require(count(case, label) == 20, case + ": all twenty fixed " + label)
                numbered = [row for row in rows if row[3].isdigit() and row[1] >= 45 and
                            row[0] in (8, 192)]
                require(sorted(int(row[3]) for row in numbered) == list(range(1, 21)),
                        case + ": all twenty channel numbers")
    for case in ("home_empty", "home_page2_empty"):
        require(count(case, "通道") == 10 and count(case, "--") == 10,
                case + ": fixed labels before binding")
    for case in ("home_bound_waiting", "home_page2_bound_waiting"):
        require(count(case, "--") == 10 and count(case, "0.0") == 0 and count(case, "0") == 0,
                case + ": bound channels await actual data")
    for case in ("home_received_zero", "home_page2_received_zero"):
        require(count(case, "--") == 0 and count(case, "0.0") == 5 and count(case, "0") == 5,
                case + ": actual zero readings are visible")
    require(count("home_partial_received", "--") == 1,
            "home_partial_received: only unread channel waits")
    for kind, zero in (("signal", "0 dBm"), ("voltage", "0.0 V")):
        require(count(f"menu_{kind}_waiting", "--") == 20,
                kind + ": twenty unread values wait")
        require(count(f"menu_{kind}_received_zero", zero) == 20,
                kind + ": twenty actual zeros visible")
        require(count(f"menu_{kind}_partial_received", zero) == 19 and
                count(f"menu_{kind}_partial_received", "--") == 1,
                kind + ": partial receipt remains independent")
    require(sum(row[3].startswith("SW_") for row in records["menu_names_waiting"]) == 20,
            "binding names available before telemetry")
    lower_names = [row for row in records["menu_names_descenders"] if "g_jpqy" in row[3]]
    require(len(lower_names) == 2 and all(row[1] == 162 and row[2] == 11 for row in lower_names),
            "last-row descenders leave space above bottom frame")
    result = {"checks": len(checks), "failures": failures}
    (output / "text_contract.json").write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n",
                                               encoding="utf-8")
    print(f"Text/layout contracts: {len(checks)} checks, {len(failures)} failures")
    return result


if __name__ == "__main__":
    sys.exit(bool(check(HERE / "unified_text_output")["failures"]))
