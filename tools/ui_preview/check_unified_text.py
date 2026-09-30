"""Check received/waiting data and internal layout using actual renderer logs."""
from collections import defaultdict
import json
from pathlib import Path
import re
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]


def check(output, home_only=False):
    header = (ROOT / "CH584_V1_0_1/APP/include/ui_menu_assets.h").read_text(encoding="utf-8")
    tables = {}
    for size in (11, 14, 16, 18):
        body = re.search(rf"ui_menu_glyphs_{size}\[\] = \{{(.*?)\}};", header, re.S).group(1)
        tables[size] = {int(code, 16): int(width) for code, width in
                        re.findall(r"\{0x([0-9a-f]+), (\d+)u,", body)}
    records = defaultdict(list)
    inks = defaultdict(list)
    logos = defaultdict(list)
    for line in (output / "after/draws.tsv").read_text(encoding="utf-8").splitlines():
        parts = line.split("\t")
        if parts[0] == "CUSTOM_TEXT" and parts[2] == "primary":
            x, y, size, missing = map(int, parts[3:7])
            records[parts[1]].append((x, y, size, parts[7]))
            assert not missing, line
        elif parts[0] == "CUSTOM_INK" and parts[2] == "primary":
            inks[parts[1]].append(tuple(map(int, parts[3:10])) + (parts[10],))
        elif parts[0] == "XBMP" and parts[2] == "primary":
            x, y, w, h = map(int, parts[3:7])
            if w >= 60 and h >= 30 and x == 9 and y == 5:
                logos[parts[1]].append((x, y, w, h))
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
            require(logos[case] == [(9, 5, 88, 44)], case + ": shared enlarged logo position and size")
            states = [row for row in rows if row[3].startswith("状态：")]
            for state in states:
                machine = next(row for row in rows if row[3].startswith("本机号："))
                station = next(row for row in rows if row[3].startswith("分站："))
                require(machine[1:3] == (44, 11) and machine[0] + width(machine) == 372 and
                        station[1:3] == (44, 11) and state[1:3] == (44, 11),
                        case + ": compact header shares baseline and right anchor")
                require(station[0] - state[0] - width(state) == 8 and
                        machine[0] - station[0] - width(station) == 8,
                        case + ": both header text gaps are eight pixels")
                require(state[0] >= 98 and state[0] + width(state) < station[0] and
                        station[0] + width(station) < machine[0] and machine[0] + width(machine) <= 372,
                        case + ": longest address/state/station fit without overlap")
            stats = [row for row in rows if row[3].startswith(("已绑定:", "已用通道:", "报警:"))]
            require(len(states) == 1 and not stats or not states and len(stats) == 3,
                    case + ": exactly one page header variant")
            require(all(row[1:3] == (44, 11) and row[0] >= 98 and row[0] + width(row) <= 372
                        for row in stats), case + ": statistics above data beside logo")
            data_rows = [row for row in rows if row[1] >= 52]
            for row in data_rows:
                is_badge = row[3].isdigit() and (row[0] < 40 or 203 <= row[0] < 225)
                require(row[1] in (70, 92, 114, 136, 158) and row[2] == (14 if is_badge else 18),
                        case + ": data baseline/font " + str(row))
            for baseline in (70, 92, 114, 136, 158):
                for left in (14, 203):
                    fields = [row for row in data_rows if row[1] == baseline and left <= row[0] < left + 166]
                    require(len(fields) >= 3 and any(row[0] == left + 24 for row in fields),
                            case + f": complete data row at {left},{baseline}")
            case_inks = [row for row in inks[case] if row[3] >= 0]
            require(len(case_inks) == len(rows), case + ": actual ink available for every drawn text")
            for row in case_inks:
                x, baseline, size, x0, y0, x1, y1, text = row
                if baseline >= 52:
                    row_index = (baseline - 70) // 22
                    left, right = (8, 186) if x < 190 else (197, 375)
                    require(left <= x0 <= x1 <= right and 53 + 22 * row_index <= y0 <= y1 <= 73 + 22 * row_index,
                            case + ": ink inside its data cell " + text)
                elif text.startswith(("本机号：", "分站：", "状态：", "已绑定:", "已用通道:", "报警:")):
                    require(98 <= x0 <= x1 <= 372 and 33 <= y0 <= y1 <= 46,
                            case + ": header ink beside logo " + text)
            for index, a in enumerate(case_inks):
                for b in case_inks[index + 1:]:
                    require(a[5] < b[3] or b[5] < a[3] or a[6] < b[4] or b[6] < a[4],
                            case + ": separate ink " + a[7] + " / " + b[7])
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
    for kind, zero in (() if home_only else (("signal", "0 dBm"), ("voltage", "0.0 V"))):
        require(count(f"menu_{kind}_waiting", "--") == 10,
                kind + ": ten unread values wait")
        require(count(f"menu_{kind}_received_zero", zero) == 10,
                kind + ": ten actual zeros visible")
        require(count(f"menu_{kind}_partial_received", zero) == 9 and
                count(f"menu_{kind}_partial_received", "--") == 1,
                kind + ": partial receipt remains independent")
    if not home_only:
        require(sum(row[3].startswith("SW_") for row in records["menu_names_waiting"]) == 10,
                "binding names available before telemetry")
        lower_names = [row for case in ("menu_names_descenders", "menu_names_descenders_page2")
                       for row in records[case] if "g_jpqy" in row[3]]
        require(len(lower_names) == 2 and all(row[1] == 158 and row[2] == 18 for row in lower_names),
                "last-row descenders leave space above bottom frame")
    result = {"checks": len(checks), "failures": failures}
    (output / "text_contract.json").write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n",
                                               encoding="utf-8")
    print(f"Text/layout contracts: {len(checks)} checks, {len(failures)} failures")
    return result


if __name__ == "__main__":
    sys.exit(bool(check(HERE / "unified_text_output")["failures"]))
