# CH584 384×168 UI 离线预览

在工程根目录运行：

```powershell
python tools/ui_preview/run_preview.py
```

工具使用完整 `CH584_V1_0_1/APP/yuying_TFT.c` 和实际工程头文件，直接编译工程的字体解码、UTF-8 解码、位图、矩形、圆角和线条绘制算法。只将显示端点接到本地 384×168 单色像素数组，并把 GPIO/SPI/延时端点设为空操作。源码仅在生成的宿主副本中将 MSVC 不接受的未使用空数组调整为一个零字节。

第一次运行会保留 `output/yuying_TFT.session_before.c`，后续运行不会覆盖此快照。`before` 从该快照编译，`after` 从当前 UI 源码编译。历史 `obj/display_verify` 不会被修改。

MSVC 默认环境脚本是当前机器已有的 `D:\visual studio\visual studio2026\VC\Auxiliary\Build\vcvars64.bat`。其他机器可在进程环境设置 `UI_PREVIEW_VCVARS` 指向其 `vcvars64.bat`。工具不修改系统 PATH，也不安装软件。Python 工具仅使用标准库。

结果统一写到忽略的 `output/`：

- `after/home_example.png`、`after/home_example_4x.png`：参考图示例数据和整数倍放大。
- `before/`、`after/`：各用例图片、实际绘制日志、逐用例检查、编译日志和中间文件。
- `report.md`、`report.json`：缺字、屏幕边界、输入/通道/绑定数据不改写、连续刷新/独立重画一致性，以及保留菜单和提示页的前后逐像素对比。

`python tools/ui_preview/run_preview.py --phase after` 只更新当前渲染；`--report-only` 使用已有结果生成报告和 PNG。

二三级页面修改使用独立范围和输出目录：

```powershell
python -B tools/ui_preview/run_preview.py --scope menus --output menu_output --phase both
```

`--scope` 默认是 `home`，保留上述主页工作流和 `output/` 默认路径。`menus` 默认输出到 `menu_output/`；显式 `--output` 相对于 `tools/ui_preview/`，必须是该目录内的独立子目录。两个范围使用各自的会话快照，已有快照永不覆盖，菜单运行不会写入历史 `output/`。

本轮菜单基线已保存为 `menu_output/yuying_TFT.session_before.c`。需要固定来源时，加上 `--baseline-sha256 <完整SHA256>`；快照缺失或摘要不一致会立即停止，避免将修改后的源码当成基线。`prepare_host.py` 只在对应源码引用资源时复制 `ui_home_assets.h` 或 `ui_menu_assets.h`，before 使用该范围内冻结的资源副本，after 使用当前资源。

菜单范围覆盖全部合法焦点、子页按钮、空/满绑定、绑定列表轮显全部页、29 字节最长名称、上传地址 0/121/122/65535、地址/组网/功率/亮屏时长/告警数量极值、RSSI 和电压极值、20 通道空/稀疏/完整数据及交替页面。每次绘图都检查输入数据及消息码不改写、位图透明模式恢复，并对比连续刷新和独立重绘；每个 PNG 另生成 `*_3x.png`。

边界统计同时检查黑字和 `draw_color=0` 的反白文字。宿主启动时用真实字库绘制两个故意越界的反白字符，分别验证右侧越界与完全位于屏幕左侧的情况；探针只验证检查器，不计入固件页面的越界统计。`summary.json` 单独报告探针失败数量。

`menus` 允许菜单页面视觉变化，要求所有 `home_*`、`message_*`、`save_message_tick*`、`power_off_message_tick10` 和 `restart_confirmation` 在 before/after 间逐像素一致。名称、信号和电压全屏页以 `chu_num2=0/1/2/65535` 绘制，另检查各组像素一致，验证当前不分页的显示语义。

分阶段运行可使用 `--phase before` 或 `--phase after`，然后通过下面命令合并比较报告：

```powershell
python -B tools/ui_preview/run_preview.py --scope menus --output menu_output --phase both --report-only
```

before 的页面问题会保留在报告中供诊断；after 的缺字、越界、数据改写、重绘差异、位图状态变化、检查器探针失败或保留页面不一致会使命令返回非零。图片转换只处理本次 `cases.tsv` 记录的用例，目录中的旧图片不会混入比较。

报告另外列出宿主编译警告和错误，不把桌面 MSVC 编译与固件 WCH GCC 构建混为一项。本机现有 `u8g2.h` 字库声明前的 GBK 注释在 `/utf-8` 下产生 C4828 编码警告；工具不修改该固件头文件。

离线预览不能证明固件完整编译、真实 UART/BLE/Flash/刷新调度、烧录或屏幕实物表现。检查只覆盖 UI 函数在给定输入下的绘制结果，不会操作串口或硬件。
