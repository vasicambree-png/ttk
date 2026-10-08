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
    for count in (0, 1, 6, 7, 10, 11, 20, 32):
        for cache in (0, 1):
            for button in (0, 1):
                for cycle in range(max(1, (count + 9) // 10) + 1):
                    fixture_name = f"subpage_scan_bound_{count}_cache_{cache}_cycle_{cycle}_button_{button}"
                    require(fixture_name in case_names,
                            fixture_name + ": all-binding count/cache/timer/action fixture present")
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
        data = [row for row in rows if 22 <= row[1] < 164]
        numbers = [row for row in data if row[3].isdigit() and row[2] == 11 and
                   (13 <= row[0] < 33 or 202 <= row[0] < 222)]
        require([int(row[3]) for row in numbers] == list(range(1, 21)),
                name + ": all twenty ordered channel numbers")
        for i, row in enumerate(numbers):
            require(row[1] == 33 + 14 * (i % 10) and
                    ((13 <= row[0] < 33) if i < 10 else (202 <= row[0] < 222)),
                    name + ": ten rows per column " + row[3])
            left = 13 if i < 10 else 202
            require(row[0] == left + (16 - width(row[3])) // 2,
                    name + ": number centered in its gutter " + row[3])
        label = {3: "信号:", 4: "电压:", 5: "名称:"}[int(case["subpage"])]
        require(sum(row[3] == label for row in data) == 0,
                name + ": no repeated row label")
        require(len(data) == 40 and all(row[1] in range(33, 160, 14) and row[2] == 11 for row in data),
                name + ": forty data fields at eleven pixels and fourteen-pixel pitch")
        title = "信息汇总" if kind == 5 else "安装调试"
        subtitle = {3: "信号", 4: "电压", 5: "名称"}[kind]
        require(len(rows) == 42 and (29, 16, 14, title) in rows and
                not any(row[3] == subtitle for row in rows),
                name + ": compact menu title retained without data subtitle")
        require(sum(row[3] == "返回" and row[0] >= 298 and row[1:3] == (16, 14)
                    for row in rows) == 1,
                name + ": compact return at upper right")
        for row in data:
            if row in numbers:
                continue
            left = 13 if row[0] < 190 else 202
            require(row[0] == left + 24 + (144 - width(row[3])) // 2,
                    name + ": data centered in its slot " + row[3])
        require(sum(row[3] == "返回" for row in rows) == 1 and
                sum(row[3] == "下一页" for row in rows) == 0,
                name + ": one return action and no next-page action")
        require(all("1/2" not in row[3] and "2/2" not in row[3] for row in rows),
                name + ": no page indicator")
        data_ink = [row for row in inks[name] if 22 <= row[1] < 164 and row[3] >= 0]
        require(len(data_ink) == len(data), name + ": visible ink for every data field")
        header_ink = [row for row in inks[name] if row[1] == 16 and row[3] >= 0]
        require(len(header_ink) == 2 and all(7 <= row[3] <= row[5] <= 376 and
                2 <= row[4] <= row[6] <= 20 for row in header_ink),
                name + ": header ink fits compressed band")
        for i, a in enumerate(header_ink):
            for b in header_ink[i + 1:]:
                require(a[5] < b[3] or b[5] < a[3],
                        name + ": header title and return remain separate")
        for row in data_ink:
            x, baseline, size, x0, y0, x1, y1, text = row
            left, right = (7, 187) if x < 190 else (196, 376)
            top = 23 + 14 * ((baseline - 33) // 14)
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
                baseline = 33 + 14 * ((channel - 1) % 10)
                left = 13 if channel <= 10 else 202
                require(any(row[3] == text and row[1] == baseline and
                            left + 24 <= row[0] < left + 168 for row in data),
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
                        int(case["menu"]) == 6 and int(case["subpage"]) == 5 or \
                        int(case["menu"]) == 2 and int(case["subpage"]) in (1, 2)
            if not full_data:
                titles = ("地址分区", "组网测试", "设备绑定", "安装调试", "上传设置", "其他设置", "信息汇总", "返回主页")
                require((141, 20, 14, titles[int(case["menu"])]) in records[case["case"]],
                        case["case"] + ": compact third-level detail title restored")
    for menu in range(8):
        if menu == 2:
            continue
        require((output / "after" / f"route_menu_{menu}_stale_1.pgm").read_bytes() ==
                (output / "after" / f"menu_all_{menu}_focus_0.pgm").read_bytes(),
                f"menu {menu}: unbind flag cannot steal another menu")
    require((output / "after/route_return_left_stale_1.pgm").read_bytes() ==
            (output / "after/menu_7.pgm").read_bytes(),
            "return menu: left selection ignores unbind flag")
    require((output / "after/binding_left_stale_1.pgm").read_bytes() ==
            (output / "after/menu_2.pgm").read_bytes(),
            "binding menu: second-level selection ignores stale unbind flag")
    for case in cases:
        name = case["case"]
        rows = records[name]
        if int(case["menu"]) == 7 and int(case["rank"]) in (2, 3):
            versions = [row for row in rows if re.fullmatch(r"V\d+\.\d", row[3])]
            require(len(versions) == 1 and versions[0][1:3] == (153, 11) and
                    versions[0][0] + width(versions[0][3]) == 368,
                    name + ": controller version at lower-right corner")
            version_inks = [row for row in inks[name] if re.fullmatch(r"V\d+\.\d", row[7])]
            require(len(version_inks) == 1 and 118 <= version_inks[0][3] <= version_inks[0][5] <= 372 and
                    140 <= version_inks[0][4] <= version_inks[0][6] <= 157,
                    name + ": version ink inside footer band")
            slogans = [row for row in inks[name] if row[7] == "精确 · 稳定 · 可靠"]
            require(len(slogans) == 1 and len(version_inks) == 1 and
                    slogans[0][5] + 4 < version_inks[0][3],
                    name + ": slogan and longest version remain separate")
            if name.startswith("return_version_"):
                raw = int(name.removeprefix("return_version_"))
                require(versions and versions[0][3] == f"V{raw // 10}.{raw % 10}",
                        name + ": version follows controller tenths format")
        if int(case["rank"]) != 3 or int(case["menu"]) != 2 or int(case["subpage"]) not in (1, 2):
            continue
        if int(case["subpage"]) == 2:
            fixture = re.fullmatch(r"subpage_scan_bound_(\d+)_cache_(\d+)_cycle_(\d+)_button_(\d+)", name)
            longest = re.fullmatch(r"subpage_scan_longest_cycle_(\d+)_button_(\d+)", name)
            expected_count = int(fixture[1]) if fixture else 32 if longest else 0 if "empty" in name else 3
            expected_numbers = list(range(1, expected_count + 1))
            column_rows = max(1, (expected_count + 1) // 2) if expected_count <= 20 else 8
            column_step = 189 if expected_count <= 20 else 92
            row_step = 11 if expected_count <= 20 else 13
            name_width = 147 if expected_count <= 20 else 64
            require((29, 16, 14, "设备绑定") in rows and
                    (108, 16, 14, f"已绑定设备:{expected_count}台") in rows and
                    sum(row[3] == "保存目前设备" for row in rows) == 1 and
                    sum(row[3] == "返回" for row in rows) == 1 and
                    not any(row[3] == "地址分区" for row in rows),
                    name + ": fullscreen scan page retains title, binding count and actions")
            page_rows = [row for row in rows if re.fullmatch(r"\d+/\d+", row[3])]
            require(not page_rows, name + ": every binding is shown without pagination")
            if fixture:
                reference = f"subpage_scan_bound_{expected_count}_cache_0_cycle_0_button_{fixture[4]}"
                require((output / f"after/{name}.pgm").read_bytes() ==
                        (output / f"after/{reference}.pgm").read_bytes(),
                        name + ": time and independent scan cache do not change binding page")
            if longest:
                reference = f"subpage_scan_longest_cycle_0_button_{longest[2]}"
                require((output / f"after/{name}.pgm").read_bytes() ==
                        (output / f"after/{reference}.pgm").read_bytes(),
                        name + ": long-name binding page is independent of timer")
            data = [row for row in rows if 22 <= row[1] < 140]
            labels = [row for row in data if re.fullmatch(r"[1-9]\d*:", row[3])]
            names = [row for row in data if row not in labels]
            require(data == [(14, 33, 11, "--")] if expected_count == 0 else
                    len(labels) == len(names) == len(expected_numbers) and
                    [row[3] for row in labels] == [f"{i}:" for i in expected_numbers],
                    name + ": ordered devices have number prefixes or empty placeholder")
            for i, (label, entry) in enumerate(zip(labels, names)):
                x, y = 14 + column_step * (i // column_rows), 33 + row_step * (i % column_rows)
                require(label[:3] == (x, y, 11) and entry[:3] == (x + 24, y, 11) and
                        width(label[3]) < 24 and width(entry[3]) <= name_width,
                        name + ": all numbered names fit their fixed column cells")
            if longest:
                require(len(names) == len(expected_numbers) and all(row[3].startswith("W") and
                        set(row[3]) <= {"W", "."} and width(row[3]) <= name_width for row in names),
                        name + ": longest names fit their column using existing clipping")
            elif expected_count:
                def clipped(text):
                    while width(text) > name_width:
                        text = text[:-1]
                    return text
                require([row[3] for row in names] == [clipped(f"SW_{i:02d}_WY_01-04") for i in expected_numbers],
                        name + ": binding-list order is preserved independently of scan cache")
            scan_ink = [row for row in inks[name] if 22 <= row[1] < 140 and row[3] >= 0]
            require(all(14 <= row[3] <= row[5] < 378 and 22 <= row[4] <= row[6] < 140
                    for row in scan_ink), name + ": names fit above action buttons")
            for row in scan_ink:
                left = 14 + column_step * ((row[0] - 14) // column_step)
                right = left + 24 + name_width
                require(left <= row[3] <= row[5] <= right,
                        name + ": number and name ink stay inside their column")
            for i, a in enumerate(scan_ink):
                for b in scan_ink[i + 1:]:
                    require(a[5] < b[3] or b[5] < a[3] or a[6] < b[4] or b[6] < a[4],
                            name + ": device and prefix ink do not overlap")
            continue
        require((29, 16, 14, "设备绑定") in rows and
                sum(row[3] == "解绑" for row in rows) == 1 and
                sum(row[3] == "返回" for row in rows) == 1 and
                not any(row[3] == "地址分区" for row in rows),
                name + ": fullscreen binding page replaces sidebar and retains actions")
        counts = [row for row in rows if re.fullmatch(r"总设备数:\d+台", row[3])]
        require(len(counts) == 1 and counts[0][:3] == (108, 16, 14),
                name + ": device count uses title size and device unit")
        header_ink = [row for row in inks[name] if row[1] < 22 and row[3] >= 0]
        require(len(header_ink) == 4 and all(2 <= row[4] <= row[6] <= 20 for row in header_ink),
                name + ": binding header ink fits top band")
        for i, a in enumerate(header_ink):
            for b in header_ink[i + 1:]:
                require(a[5] < b[3] or b[5] < a[3],
                        name + ": count, title and actions do not overlap")
        data = [row for row in rows if row[1] >= 22]
        labels = [row for row in data if re.fullmatch(r"[1-9]\d*:", row[3])]
        require(len(data) == len(labels) * 3 and len(labels) <= 10,
                name + ": device rows retain number, name and full MAC")
        numbers = [int(row[3][:-1]) for row in labels]
        require(not numbers or numbers == list(range(numbers[0], numbers[0] + len(numbers))),
                name + ": device sequence has no leading zero or skipped entries")
        for i, row in enumerate(labels):
            left = 13 if i < 5 else 202
            baseline = 33 + 28 * (i % 5)
            require(row[:3] == (left, baseline, 11) and
                    sum(r[1] == baseline + 13 and left + 24 <= r[0] < left + 168 and
                        re.fullmatch(r"[0-9A-F]{12}", r[3]) is not None for r in data) == 1,
                    name + ": name/MAC row belongs to device " + row[3])
        data_ink = [row for row in inks[name] if row[1] >= 22 and row[3] >= 0]
        for row in data_ink:
            left, right = (7, 187) if row[0] < 190 else (196, 376)
            top = 23 + 28 * ((row[1] - 33) // 28)
            require(left <= row[3] <= row[5] <= right and top <= row[4] <= row[6] <= top + 26,
                    name + ": device ink inside its cell " + row[7])
        if name.startswith("subpage_binding_full_cycle_"):
            cycle = int(name.split("_cycle_")[1].split("_")[0])
            first = cycle * 10 + 1
            require(numbers == list(range(first, min(first + 10, 33))),
                    name + ": full binding table cycles through all 32 devices")
        if name.startswith("binding_count_"):
            count, cycle = map(int, re.fullmatch(r"binding_count_(\d+)_cycle_(\d+)", name).groups())
            first = 1 if count == 10 or cycle == 0 else 11
            require(numbers == list(range(first, min(first + 10, count + 1))),
                    name + ": exact-page and partial-page boundaries")
    result = {"checks": len(checks), "failures": failures}
    (output / "param_page_contract.json").write_text(
        json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"Parameter page contracts: {len(checks)} checks, {len(failures)} failures")
    return result
