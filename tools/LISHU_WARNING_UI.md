# 返回主页隶书与警字清晰度

2026-10-08，基于main的67258d7。

返回主页“安全相伴”16px静态背景与“精确 稳定 可靠”18px运行时标语改用Windows LiSu（SIMLI.TTF）。底部采用专用XBMP，保留原文本advance、基线、居中及长版本号防碰撞逻辑；版本号仍使用原18px字体。生成器将两种资源一并生成，避免重新生成覆盖修改。

原微软雅黑粗体“警”在14/16px抗锯齿缩小后二值化，上半部笔画粘连。仅这两个字号的U+8B66改用宋体（simsun.ttc）原尺寸单色hinting，保留字宽、字模画布及基线；名称/电压警报共享这个字模，普通和选中态均覆盖。

修改文件：`tools/generate_menu_assets.py`、生成的`CH584_V1_0_1/APP/include/ui_menu_assets.h`、`CH584_V1_0_1/APP/yuying_TFT.c`。原有数组中仅`ui_menu_14_8b66`、`ui_menu_16_8b66`、`ui_menu_return_page`变化，新增`ui_menu_return_slogan`。其它字体、图标、数据及协议逻辑不变。

生成环境为已有Pillow12.1.1的Python3.13.13；最新安装Python3.14.8没有Pillow，因此不为本次任务安装额外依赖。连续生成SHA256均为`f30bb3fa159da2c2e67dcc7591012811936efc8d67f87cb97c558a949f7aa9bc`。

真实C预览521个用例：缺字/越界/数据改写/重绘差异/位图模式变化均0；214612项布局检查0失败。检查返回页不同版本号和信息汇总普通/选中态的点阵预览。报告位于忽略目录`tools/ui_preview/output/lishu_warning_20261008/`。宿主原有36条C4828编码警告。

当前工程实际WCH构建规则重新编译、链接、生成HEX成功，无固件警告/错误；FLASH212368/458752字节，RAM66064/98304字节。HEX：`tools/screen_power_build/output/CH584M_TFT_HB.hex`。

烧录及实屏清晰度尚未验证。手动检查：烧录后进入返回主页查看两句隶书及版本间距，进入信息汇总查看电压警报普通/选中态“警”字。此修改不执行恢复出厂或其它主控动作。
