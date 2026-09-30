"""Check pagination and cell ink from the actual firmware drawing logs."""
from collections import defaultdict
import csv
import json


def check(output):
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

    for case in cases:
        if int(case["rank"]) != 3 or int(case["subpage"]) not in (3, 4, 5):
            continue
        name = case["case"]
        page2 = int(case["page"]) == 2
        rows = records[name]
        data = [row for row in rows if row[1] >= 52]
        numbers = [row for row in data if row[3].isdigit() and row[2] == 14 and
                   (14 <= row[0] < 36 or 203 <= row[0] < 225)]
        first = 11 if page2 else 1
        require([int(row[3]) for row in numbers] == list(range(first, first + 10)),
                name + ": ten ordered channel numbers from correct page")
        for i, row in enumerate(numbers):
            require(row[1] == 70 + 22 * (i % 5) and
                    ((14 <= row[0] < 36) if i < 5 else (203 <= row[0] < 225)),
                    name + ": five rows per column " + row[3])
        label = {3: "信号:", 4: "电压:", 5: "名称:"}[int(case["subpage"])]
        require(sum(row[3] == label for row in data) == 0,
                name + ": no repeated row label")
        require(all(row[1] in (70, 92, 114, 136, 158) for row in data),
                name + ": shared home data baselines")
        require(sum(row[3] == ("返回" if page2 else "下一页") for row in rows) == 1 and
                sum(row[3] == ("下一页" if page2 else "返回") for row in rows) == 0,
                name + ": one correct confirm action")
        require(sum(row[3].endswith("2/2" if page2 else "1/2") for row in rows) == 1,
                name + ": correct visible page indicator")
        data_ink = [row for row in inks[name] if row[1] >= 52 and row[3] >= 0]
        for row in data_ink:
            x, baseline, size, x0, y0, x1, y1, text = row
            left, right = (8, 186) if x < 190 else (197, 375)
            top = 53 + 22 * ((baseline - 70) // 22)
            require(left <= x0 <= x1 <= right and top <= y0 <= y1 <= top + 20,
                    name + ": ink inside cell " + text)
        for i, a in enumerate(data_ink):
            for b in data_ink[i + 1:]:
                require(a[5] < b[3] or b[5] < a[3] or a[6] < b[4] or b[6] < a[4],
                        name + ": separate ink " + a[7] + " / " + b[7])
        if "waiting" in name:
            values = [row[3] for row in data if not row[3].isdigit()]
            require(sum(text.startswith("SW_") for text in values) == 10 if int(case["subpage"]) == 5
                    else values.count("--") == 10, name + ": binding names or unread placeholders")
        if "received_zero" in name:
            zero = "0 dBm" if int(case["subpage"]) == 3 else "0.0 V"
            require(sum(row[3] == zero for row in data) == 10 and
                    sum(row[3] == "--" for row in data) == 0,
                    name + ": received zero is distinct from waiting")
        elif "partial_received" in name:
            zero = "0 dBm" if int(case["subpage"]) == 3 else "0.0 V"
            require(sum(row[3] == zero for row in data) == 9 and
                    sum(row[3] == "--" for row in data) == 1,
                    name + ": unread channel remains independent")
    for kind in (3, 4, 5):
        prefix = f"pagination_{kind}_"
        images = [(output / "after" / (prefix + step + ".pgm")).read_bytes()
                  for step in ("enter", "next", "exit", "reenter")]
        require(images[0] == images[3] and images[0] != images[1] and
                images[2] not in (images[0], images[1]),
                prefix + "controller frame sequence returns and reenters cleanly")
    result = {"checks": len(checks), "failures": failures}
    (output / "param_page_contract.json").write_text(
        json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"Parameter page contracts: {len(checks)} checks, {len(failures)} failures")
    return result
