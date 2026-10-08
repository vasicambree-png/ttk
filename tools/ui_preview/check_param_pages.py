"""Check all twenty channels and cell ink from actual firmware drawing logs."""
from collections import defaultdict
import csv
import json
from pathlib import Path
import re


def check(output):
    root = Path(__file__).resolve().parents[2]
    header = (root / "CH584_V1_0_1/APP/include/ui_menu_assets.h").read_text(encoding="utf-8")
    glyphs = re.search(r"ui_menu_glyphs_11\[\] = \{(.*?)\};", header, re.S).group(1)
    widths = {int(code, 16): int(width) for code, width in
              re.findall(r"\{0x([0-9a-f]+), (\d+)u,", glyphs)}

    def width(text):
        return sum(widths.get(ord(char), widths[ord('?')]) for char in text)

    records, inks = defaultdict(list), defaultdict(list)
    for line in (output / "after/draws.tsv").read_text(encoding="utf-8").splitlines():
        parts = line.split("\t")
        if parts[0] == "CUSTOM_TEXT" and parts[2] == "primary":
            records[parts[1]].append(tuple(map(int, parts[3:6])) + (parts[7],))
        elif parts[0] == "CUSTOM_INK" and parts[2] == "primary":
            inks[parts[1]].append(tuple(map(int, parts[3:10])) + (parts[10],))
    with (output / "after/cases.tsv").open(encoding="utf-8", newline="") as stream:
        cases = list(csv.DictReader(stream, delimiter="\t"))
    failures, checks = [], []

    def require(condition, message):
        checks.append(message)
        if not condition:
            failures.append(message)

    case_names = {case["case"] for case in cases}
    for label in ("signal", "voltage", "names"):
        require(f"menu_{label}_endpoints" in case_names,
                label + ": independent column-endpoint fixture present")
        require(f"menu_20_{label}_waiting" in case_names,
                label + ": twenty-channel waiting fixture present")
    for label in ("signal", "voltage"):
        for mode in ("received_zero", "partial_received"):
            require(f"menu_20_{label}_{mode}" in case_names,
                    label + ": twenty-channel " + mode + " fixture present")

    for case in cases:
        kind = int(case["subpage"])
        menu = int(case["menu"])
        if int(case["rank"]) != 3 or not (menu == 3 and kind in (3, 4) or menu == 6 and kind == 5):
            continue
        name = case["case"]
        rows = records[name]
        data = [row for row in rows if 5 <= row[1] < 137]
        numbers = [row for row in data if row[3].isdigit() and row[2] == 11 and
                   (14 <= row[0] < 34 or 203 <= row[0] < 223)]
        require([int(row[3]) for row in numbers] == list(range(1, 21)),
                name + ": all twenty ordered channel numbers")
        for i, row in enumerate(numbers):
            require(row[1] == 16 + 13 * (i % 10) and
                    ((14 <= row[0] < 34) if i < 10 else (203 <= row[0] < 223)),
                    name + ": ten rows per column " + row[3])
            left = 14 if i < 10 else 203
            require(row[0] == left + (16 - width(row[3])) // 2,
                    name + ": number centered in its gutter " + row[3])
        label = {3: "信号:", 4: "电压:", 5: "名称:"}[int(case["subpage"])]
        require(sum(row[3] == label for row in data) == 0,
                name + ": no repeated row label")
        require(len(data) == 40 and all(row[1] in range(16, 134, 13) and row[2] == 11 for row in data),
                name + ": forty data fields at eleven pixels and thirteen-pixel pitch")
        require(len(rows) == 41 and not any(row[3] in
                ("信息汇总", "安装调试", "设备信号", "设备电压", "名称", "信号", "电压") for row in rows),
                name + ": no top title strip")
        for row in data:
            if row in numbers:
                continue
            left = 14 if row[0] < 190 else 203
            require(row[0] == left + 24 + (142 - width(row[3])) // 2,
                    name + ": data centered in its slot " + row[3])
        require(sum(row[3] == "返回" for row in rows) == 1 and
                sum(row[3] == "下一页" for row in rows) == 0,
                name + ": one return action and no next-page action")
        require(all("1/2" not in row[3] and "2/2" not in row[3] for row in rows),
                name + ": no page indicator")
        data_ink = [row for row in inks[name] if 5 <= row[1] < 137 and row[3] >= 0]
        require(len(data_ink) == len(data), name + ": visible ink for every data field")
        for row in data_ink:
            x, baseline, size, x0, y0, x1, y1, text = row
            left, right = (8, 186) if x < 190 else (197, 375)
            top = 6 + 13 * ((baseline - 16) // 13)
            require(left <= x0 <= x1 <= right and top <= y0 <= y1 <= top + 12,
                    name + ": ink inside cell " + text)
        for i, a in enumerate(data_ink):
            for b in data_ink[i + 1:]:
                require(a[5] < b[3] or b[5] < a[3] or a[6] < b[4] or b[6] < a[4],
                        name + ": separate ink " + a[7] + " / " + b[7])
        if "waiting" in name:
            values = [row[3] for row in data if not row[3].isdigit()]
            require(sum(text.startswith("SW_") for text in values) == 20 if int(case["subpage"]) == 5
                    else values.count("--") == 20, name + ": binding names or unread placeholders")
        if "received_zero" in name:
            zero = "0 dBm" if int(case["subpage"]) == 3 else "0.0 V"
            require(sum(row[3] == zero for row in data) == 20 and
                    sum(row[3] == "--" for row in data) == 0,
                    name + ": received zero is distinct from waiting")
        elif "partial_received" in name:
            zero = "0 dBm" if int(case["subpage"]) == 3 else "0.0 V"
            unread = 1 if name.startswith("menu_20_") else 2
            require(sum(row[3] == zero for row in data) == 20 - unread and
                    sum(row[3] == "--" for row in data) == unread,
                    name + ": unread channel remains independent")
        if name.endswith("_endpoints"):
            expected = {3: {1: "-11 dBm", 10: "-42 dBm", 11: "-73 dBm", 20: "-104 dBm"},
                        4: {1: "1.1 V", 10: "4.2 V", 11: "7.3 V", 20: "10.4 V"},
                        5: {1: "SW_CH01", 10: "SW_CH10", 11: "SW_CH11", 20: "SW_CH20"}}[int(case["subpage"])]
            for channel, text in expected.items():
                baseline = 16 + 13 * ((channel - 1) % 10)
                left = 14 if channel <= 10 else 203
                require(any(row[3] == text and row[1] == baseline and
                            left + 24 <= row[0] < left + 166 for row in data),
                        name + ": channel " + str(channel) + " value in its own row")
    for label in ("signal", "voltage", "names"):
        images = [(output / "after" / f"menu_{label}_page_{page}.pgm").read_bytes()
                  for page in (0, 1, 2, 65535)]
        require(all(image == images[0] for image in images),
                label + ": legacy page codes cannot change twenty-channel content")
    for kind in (3, 4, 5):
        prefix = f"pagination_{kind}_"
        images = [(output / "after" / (prefix + step + ".pgm")).read_bytes()
                  for step in ("enter", "next", "exit", "reenter")]
        require(images[0] == images[1] == images[3] and images[2] != images[0],
                prefix + "legacy next frame preserves all channels, exit and reenter are clean")
        left = (output / "after" / f"route_return_left_stale_{kind}.pgm").read_bytes()
        require(left == (output / "after/menu_7.pgm").read_bytes(),
                f"return menu: left selection ignores stale subpage {kind}")
        for menu in range(8):
            if menu == 3 and kind in (3, 4) or menu == 6 and kind == 5:
                continue
            actual = output / "after" / f"route_menu_{menu}_stale_{kind}.pgm"
            require(actual.read_bytes() == (output / "after" / f"menu_all_{menu}_focus_0.pgm").read_bytes(),
                    f"menu {menu}: stale subpage {kind} cannot steal rendering")
    for case in cases:
        if int(case["rank"]) == 3 and not case["case"].startswith("message_"):
            full_data = int(case["menu"]) == 3 and int(case["subpage"]) in (3, 4) or \
                        int(case["menu"]) == 6 and int(case["subpage"]) == 5
            if not full_data:
                require(not any(113 <= row[0] and row[1] <= 42 for row in records[case["case"]]),
                        case["case"] + ": third-level detail title removed")
    result = {"checks": len(checks), "failures": failures}
    (output / "param_page_contract.json").write_text(
        json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"Parameter page contracts: {len(checks)} checks, {len(failures)} failures")
    return result
