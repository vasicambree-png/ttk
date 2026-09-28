#include "CONFIG.h"
#include "yuying_TFT.h"
//#include "u8g2_fonts.c"
#include "u8g2.h"
#include "stdlib.h"
/* ★ 功能清单：主页面/子页要显示 CH584M 本地数据（CH_com_buf[]、绑定表、
 *   异常计数、扫描名称缓存），这些类型与 extern 声明都在 observer.h 里。
 *   observer.h 自身 include yuying_TFT.h（有 include guard），无循环问题。 */
#include "observer.h"
extern u8g2_t u8g2;
data_LIST Data_list1;

const uint8_t bmp[]={
};

uint8_t UI_Select = 0;
uint8_t UI_State = 0;
uint8_t UI_LevelMark = 0;

//高
const uint8_t bitmap1[]={
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x00,0x80,0xFF,0xFF,0x01,0xE0,0xFF,0xFF,0x07,0xF8,0x07,0xE0,0x1F,
        0xFC,0x00,0x00,0x3F,0x3E,0x00,0x00,0x7C,0x1F,0xF0,0x0F,0xF8,0x0F,0xFE,0x7F,0xF0,0x80,0xFF,0xFF,0x01,0xC0,0x1F,0xF8,0x03,0xE0,0x07,0xE0,0x07,0xE0,0x01,0x80,0x07,
        0xE0,0x00,0x00,0x07,0x00,0xF0,0x0F,0x00,0x00,0xF8,0x1F,0x00,0x00,0xFC,0x3F,0x00,0x00,0x1C,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0x00,
        0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*E:\桌面\信号高.bmp*/0
};
//中
const uint8_t bitmap2[]={
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0xFE,0x7F,0x00,0x80,0xFF,0xFF,0x01,0xC0,0x1F,0xF8,0x03,0xE0,0x07,0xE0,0x07,0xE0,0x01,0x80,0x07,
        0xE0,0x00,0x00,0x07,0x00,0xF0,0x0F,0x00,0x00,0xF8,0x1F,0x00,0x00,0xFC,0x3F,0x00,0x00,0x1C,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0x00,
        0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*E:\桌面\信号 中.bmp*/0
};
//低
const uint8_t bitmap3[]={

        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0xF8,0x1F,0x00,0x00,0xFC,0x3F,0x00,0x00,0x1C,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0x00,
        0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*E:\桌面\信号低.bmp*/0

};
//无
const uint8_t bitmap4[]={

        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x80,0x03,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0xF7,0x3F,0x00,0x80,0xFE,0xFF,0x01,0xE0,0x1F,0xF0,0x07,
        0xF0,0x1D,0x80,0x0F,0x7C,0x3C,0x00,0x3E,0x1E,0x38,0x00,0x78,0x0C,0xF0,0x07,0x30,0x00,0x7C,0x3F,0x00,0x00,0xEF,0xFE,0x00,0x80,0xEF,0xF1,0x01,0xC0,0xC3,0xC1,0x03,
        0x80,0xC1,0x83,0x01,0x00,0x80,0x03,0x00,0x00,0x60,0x07,0x00,0x00,0xF8,0x07,0x00,0x00,0x7C,0x0E,0x00,0x00,0x18,0x1E,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x3C,0x00,
        0x00,0xC0,0x3B,0x00,0x00,0xC0,0x73,0x00,0x00,0xC0,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*E:\桌面\无信号.bmp*/0

};
MENU_LIST Main_List[] = {

  { 0, POS_X, POS_Y+ MENU_HIGH * 0, "地址分区", addr_Control },
  { 1, POS_X, POS_Y+ MENU_HIGH * 1, "组网测试", networking_Control },

  /* ★ 契约 §1：第 2 项由「探头标定」改为「设备绑定」（原标定页保留代码但不再挂载） */
  { 2, POS_X, POS_Y + MENU_HIGH * 2, "设备绑定", binding_Control },
  { 3, POS_X, POS_Y + MENU_HIGH * 3, "安装调试", install_Control },

  { 4, POS_X, POS_Y + MENU_HIGH * 4, "上传设置", uploading_Control },
  { 5, POS_X, POS_Y + MENU_HIGH * 5, "其他设置", Other_Settings_Control },

  /* ★ 契约 §1：第 6 项由「数据置零」改为「信息汇总」（原置零页保留代码但不再挂载） */
  { 6, POS_X, POS_Y + MENU_HIGH * 6, "信息汇总", summary_Control },
  { 7, POS_X, POS_Y + MENU_HIGH * 7, "返回主页", return_main_Control },
};
MENU_LIST Menu_List[] = {

  { 0, POS_X1, POS_Y1, "地址分区", addr_Control },
  { 1, POS_X1, POS_Y1, "组网测试", networking_Control},

  { 2, POS_X1, POS_Y1 , "设备绑定", binding_Control },
  { 3, POS_X1, POS_Y1, "安装调试", install_Control },

  { 4, POS_X1, POS_Y1 , "上传设置", uploading_Control },
  { 5, POS_X1, POS_Y1 , "其他设置", Other_Settings_Control },

  { 6, POS_X1, POS_Y1 , "信息汇总", summary_Control },
  { 7, POS_X1, POS_Y1 , "返回主页", return_main_Control },
};







const uint8_t MAIN_LEN = sizeof(Main_List) / sizeof(MENU_LIST);  //����˵���Ŀ����

/* ==================================================================
 * 功能清单（2026-09-21）新增：CH584M 本地数据的显示支持
 *
 * 字体约定（契约 §7）：**中文一律用 UI_FONT_CN**
 *   （= u8g2_font_wqy14_t_gb2312a，3755 常用字，清单用字缺 0）。
 *   工程原有的 u8g2_font16/24/32_lunar 只有 111 个汉字（实测），
 *   清单文案里的 绑/解/键/蓝/牙/名/称/警/报/异/常/恢/复/出/厂/息/汇/总/
 *   扫/描/个/用/巷/综/合 全部缺失，用它们画会**直接显示空白**。
 *   wqy14 的实测字形参数（脚本 D:\ai_work\_tools\wqy14_measure.js）：
 *     汉字 13x13、步进 14px、墨迹相对基线 = [-12, +1]；ASCII 步进 7~9px。
 *   因此本文件所有新页面的**行距取 14px 或其整数倍**，字号统一 wqy14。
 * ================================================================== */

/* 通道类型显示表（下标 = Sensor_Tpye 数值，与 STM32 的 Data_tpye 逐项一致）
 *   名称：sensor 类型中文名（取自枚举注释）；
 *   单位：★ 2026-09-22 按厂商广播格式规格（契约_传感器数据解析与显示标度 §4.2）确定：
 *         锚杆(MG)=kN、激光(JG)=mm、位移(WY2/4/6/8)=mm、裂缝(LF)=mm、
 *         倾角(QJ)=°、应力(YL)=MPa；液位/微震/地音/测试 仍未定义量纲，留空不猜。
 *   另：★ 更正第 5 轮的一条错误结论 —— 当时说"新字库缺 '°' 字形"是针对**旧字库**
 *       u8g2_font24_lunar 的结论。当前 UI_FONT_CN = u8g2_font_wqy14_t_gb2312a
 *       经 _tools/font_probe.js 核验**含 U+00B0(°)**（负例对照：€/emoji/U+FFFD 均为缺），
 *       故倾角可以正常显示 "°"。 */
static const char *const CH_TYPE_NAME[TPYE_END] = {
    "--", "锚杆", "激光", "位移", "位移", "位移", "位移",
    "裂缝", "倾角", "应力", "液位", "微震", "地音", "测试"
};
static const char *const CH_TYPE_UNIT[TPYE_END] = {
    /* 0 NONE  1 MG(kN)  2 JG(mm)  3 WY2  4 WY4  5 WY6  6 WY8  7 LF  8 QJ(°)  9 YL(MPa) 10..13 未定义 */
    "", "kN", "mm", "mm", "mm", "mm", "mm", "mm", "°", "MPa", "", "", "", ""
};

/* ★ 契约 §4.1：显示标度 —— MG / 位移(WY) / 应力(YL) / 倾角(QJ) 上传值带一位小数（显示 = 值/10）；
 *   激光 JG 不带（原样显示）。换算只发生在**显示层**，
 *   CH_com_buf[].CH_data 与 0x03/0x06 协议帧仍为**原始值**（上游口径不变）。 */
static uint8_t ch_disp_scale10(Sensor_Tpye t)
{
    return (t == TPYE_MG || (t >= TPYE_WY2 && t <= TPYE_WY8) ||
            t == TPYE_YL || t == TPYE_QJ) ? 1 : 0;
}
/* 只格式化显示值，保留原始数据中的小数和负倾角符号。 */
static void ch_disp_format(char *dst, size_t size, Sensor_Tpye t, uint16_t raw)
{
    int32_t v = (t == TPYE_QJ) ? (int32_t)(int16_t)raw : (int32_t)raw;
    if (ch_disp_scale10(t))
    {
        uint32_t magnitude = (uint32_t)((v < 0) ? -v : v);
        snprintf(dst, size, "%s%lu.%lu", (v < 0) ? "-" : "",
                 (unsigned long)(magnitude / 10), (unsigned long)(magnitude % 10));
    }
    else
        snprintf(dst, size, "%u", (unsigned int)raw);
}

/* 画一行文本并返回其像素宽度
 * ★ x/y 用 uint16_t：主页面单位列/占位符在 x=292 / 312，写成 uint8_t 会被截断
 *   （312→56）导致文字画到错位置，编译期也会报 -Woverflow。 */
static uint16_t ui_draw(uint16_t x, uint16_t y, const char *s)
{
    u8g2_DrawUTF8(&u8g2, x, y, s);
    return (uint16_t)u8g2_GetUTF8Width(&u8g2, s);
}

/* 选项高亮框：x/y = 文字起点与基线，w = 文字宽度
 * wqy14 墨迹 = [基线-12, 基线+1] ⇒ 框取 [基线-13, 基线+4]（上下各留 1~3px）
 *
 * ==========================================================================
 * ★★ 全工程**唯一**的高亮框绘制点（本次修正）
 * --------------------------------------------------------------------------
 * 屏幕上"方框同时出现在两个位置"的原因：
 *   原约定（老页面 networking_Control1 / addr_Control / calibration_Control …
 *   一直遵守）是——
 *       menu_rank == 2  → 焦点在**左侧菜单**，左侧框由 UI_Menu_Display() 画，
 *                         右侧页面**不许画框**；
 *       menu_rank == 3  → 焦点在**右侧页面**，左侧框不画，右侧页面才画框。
 *   但后来新增的 binding_* / summary_* / install_* 这几页**漏了 menu_rank 判断**，
 *   于是 menu_rank==2 时左侧画一个框、右侧页面又画一个框 ⇒ 同屏两个框。
 *
 * 修正：把判断收敛到本函数这**唯一一处**，条件互斥 ⇒ 同一时刻屏幕上只有一个高亮框。
 *       （左侧框只在 menu_rank==2 画，右侧框只在 menu_rank!=2 画。）
 * ========================================================================== */
static void ui_hl_box(uint16_t x, uint16_t baseline, uint16_t w, uint8_t on)
{
    if (!on) return;
    if (Data_list1.menu_rank == 2) return;   /* 焦点在左侧菜单：右侧一律不画框 */
    u8g2_DrawFrame(&u8g2, (u8g2_uint_t)(x - 5), (u8g2_uint_t)(baseline - 13),
                   (u8g2_uint_t)(w + 10), 18);
}

/* 按**像素宽度**截断设备名（契约 §4.2：通道绑定名称页"名称过长截断"）
 *   dst     : 至少 n 字节的输出缓冲
 *   src     : 名字（'\0' 结尾，BLE 名为 ASCII）
 *   n       : 最多考察的字节数（含 '\0'，调用处传 MAX_NAME_LEN）
 *   max_w   : 允许的最大像素宽度
 *   ⚠️ 必须按宽度而不是按字符数截断：wqy14 最宽 ASCII（'W'/'M'）步进 9px、
 *      最窄 7px，同样 16 个字符可能是 112px 也可能是 144px（实测
 *      "WWWW..(×16)" = 192px），按字符数截断仍会压过双列分隔线。
 *   开销：u8g2 无"单字符步进"接口，这里对每个字符查一次
 *   u8g2_GetUTF8Width(单字符)（字库为线性查表），20 行 × ≤30 字符 = 每帧
 *   ≤600 次单字符查询，相对整屏 SPI 刷新可忽略。 */
static void ui_name_fit(char *dst, const uint8_t *src, uint8_t n, uint16_t max_w)
{
    char     one[2];
    uint16_t w = 0;
    uint8_t  i;

    for (i = 0; i < (uint8_t)(n - 1) && src[i] != '\0'; i++)
    {
        uint16_t cw;
        one[0] = (char)src[i];
        one[1] = '\0';
        cw = (uint16_t)u8g2_GetUTF8Width(&u8g2, one);
        if ((uint16_t)(w + cw) > max_w) break;
        dst[i] = (char)src[i];
        w = (uint16_t)(w + cw);
    }
    dst[i] = '\0';
}

/* 名称仍是原有参数：较长的 ASCII 设备名用现有小字库完整显示。 */
static void ui_draw_name(uint16_t x, uint16_t y, const uint8_t *src, uint16_t max_w)
{
    char name[MAX_NAME_LEN];
    uint8_t i, ascii = 1;

    for (i = 0; i < MAX_NAME_LEN - 1 && src[i] != '\0'; i++)
    {
        name[i] = (char)src[i];
        if (src[i] < 32 || src[i] > 126) ascii = 0;
    }
    name[i] = '\0';

    u8g2_SetFont(&u8g2, UI_FONT_CN);
    if (u8g2_GetUTF8Width(&u8g2, name) > max_w)
    {
        if (ascii)
            u8g2_SetFont(&u8g2, u8g2_font_5x8_tr);
        else
            ui_name_fit(name, src, MAX_NAME_LEN, max_w);
    }
    ui_draw(x, y, name);
    u8g2_SetFont(&u8g2, UI_FONT_CN);
}



short x, x_trg;          //          ֵ
short y, y_trg;                 //
short frame_lenth, frame_lenth_trg;
short frame_y, frame_y_trg;

void UI_MoveSet(short *p, short *p_trg, uint8_t step, uint8_t min) {
  step = abs(*p_trg - *p) > min ? step : 1;
  if (*p < *p_trg) {
    *p += step;
  } else if (*p > *p_trg) {
    *p -= step;
  }
}



uint8_t rank=0;

/* ==================================================================
 * ★★ 保存结果提示页（V1_0_2 第六轮新增）
 * ------------------------------------------------------------------
 * 需求：保存必须有明确的成功/失败页面提示，两类来源：
 *   1) 参数存到 STM32 自己的 Data-EEPROM：STM32 发 0x01（menu_rank=6，
 *      参数=UI_MSG_xxx）通知本机显示；
 *   2) 蓝牙绑定存到 CH584M 自己的 Data-Flash（0x05）：本机自己知道结果，
 *      直接本地显示。
 * 机制：
 *   · g_ui_msg 非 0 时，UI_Control() **优先画提示页**，普通页面帧盖不掉它
 *     （但 Data_list1 照常更新 ⇒ 提示消失后自动画回最新页面）；
 *   · 显示时长由 1 秒事件倒数（ui_msg_tick_sec()），到点自动清除并请求重画；
 *   · 提示文本全部用 UI_FONT_CN（u8g2_font_wqy14_t_gb2312a）。
 *     ★ 注意：旧的 u8g2_font24_lunar **缺** 解/绑/全/部/参 等字，
 *       中文提示一律用 UI_FONT_CN，否则会画成空白。
 * ================================================================== */
volatile uint8_t g_ui_msg      = UI_MSG_NONE;
static   uint8_t s_ui_msg_hold = 0u;

void ui_show_msg(uint8_t code)
{
    /* ★ 第 11 轮：code == UI_MSG_NONE(0) 表示"显式清除提示"。
       软关机提示页（UI_MSG_POWER_OFF）是常驻的，不会自己超时消失，
       必须由 STM32 在正式开机后发一帧 msg=0 把它清掉。 */
    if (code == UI_MSG_NONE)
    {
        if (g_ui_msg != UI_MSG_NONE)
        {
            g_ui_msg      = UI_MSG_NONE;
            s_ui_msg_hold = 0u;
        }
        return;
    }

    g_ui_msg      = code;
    s_ui_msg_hold = UI_MSG_HOLD_SEC;
    /* 重画请求由调用方负责：
       0x01 路径在 app_uart_process() 解析成功后本来就会置 dis_flag_cnt = 1；
       0x05 路径同理；1 秒事件里的延期清 Flash 路径由那里显式置位。 */
}

uint8_t ui_msg_tick_sec(void)
{
    if (g_ui_msg == UI_MSG_NONE) return 0u;

    /* ★ 第 11 轮：软关机提示页常驻，不参与超时倒数
       （由 STM32 发 msg=0 显式清除，见 ui_show_msg()）。 */
    if ((g_ui_msg == UI_MSG_POWER_OFF) || (g_ui_msg == UI_MSG_SHUTTING_DOWN)) return 0u;

    if (s_ui_msg_hold > 0u) s_ui_msg_hold--;

    if (s_ui_msg_hold == 0u)
    {
        g_ui_msg = UI_MSG_NONE;
        return 1u;                 /* 提示刚消失：调用方应请求一次重画 */
    }
    return 0u;
}

static void UI_Message_Display(void)
{
    const char *line = "保存";
    uint16_t w, x, y;
    uint16_t dw, dh;
    uint16_t bw;

    switch (g_ui_msg)
    {
        case UI_MSG_PARAM_SAVE_OK:   line = "参数保存成功"; break;
        case UI_MSG_PARAM_SAVE_FAIL: line = "参数保存失败"; break;
        case UI_MSG_BIND_SAVE_OK:    line = "绑定保存成功"; break;
        case UI_MSG_BIND_SAVE_FAIL:  line = "绑定保存失败"; break;
        case UI_MSG_BIND_SAVE_EMPTY: line = "无绑定可保存"; break;
        case UI_MSG_BIND_CLEAR_OK:   line = "绑定已清除";   break;
        case UI_MSG_BIND_CLEAR_FAIL: line = "绑定清除失败"; break;
        /* ★ 第 11 轮：软关机提示页（常驻，直到 STM32 发 msg=0 清除） */
        case UI_MSG_POWER_OFF:       line = "长按K1+K3开机"; break;
        case UI_MSG_SHUTTING_DOWN:   line = "正在关机中"; break;
        default:                     line = "保存";         break;
    }

    u8g2_SetFont(&u8g2, UI_FONT_CN);
    u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetDrawColor(&u8g2, 1);

    dw = (uint16_t)u8g2_GetDisplayWidth(&u8g2);
    dh = (uint16_t)u8g2_GetDisplayHeight(&u8g2);

    w = (uint16_t)u8g2_GetUTF8Width(&u8g2, line);
    x = (uint16_t)((dw > w) ? ((dw - w) / 2u) : 0u);
    y = (uint16_t)((dh / 2u) + 6u);            /* 基线略低于中线（wqy14 墨迹在基线上方） */

    /* 提示框：居中、左右各留 24px、上下留 27px（框内只放一行大字） */
    bw = (uint16_t)(dw - 48u);
    u8g2_DrawFrame(&u8g2, 24u, (u8g2_uint_t)(y - 34u), (u8g2_uint_t)bw, 54u);

    ui_draw(x, y, line);
}

__HIGH_CODE
void UI_Control(data_LIST *list)
{
    uint8_t rank2_addr=0,rank3_addr=0;
    rank=list->menu_rank;
    rank2_addr=list->rank2_addr;
    rank3_addr=list->rank3_addr;

    u8g2_ClearBuffer(&u8g2);

    /* ★★ 保存结果提示页优先：只要 g_ui_msg 非 0，就画提示页**并直接返回**。
       这样 STM32 随后发来的普通页面帧（0x01 menu_rank 1/2/3）不会把提示盖掉；
       普通页面的字段仍然照常写进 Data_list1，提示消失后画回的就是最新页面。 */
    if (g_ui_msg != UI_MSG_NONE)
    {
        UI_Message_Display();
        u8g2_SendBuffer(&u8g2);
        return;
    }

   switch (rank)
   {
       case 1:
           UI_Main_Display(&Data_list1);
           break;
       case 2:
       case 3:
           UI_Select=rank2_addr;
           UI_Menu_Display();
           break;
       case 4:
           new_return();
           break;
       case 5:
//           PRINT("rank == %d ;u8g2Init(&u8g2)\r\n",rank);
//           u8g2Init(&u8g2);
//           rank=0;
//
//            app_drv_fifo_init(&app_uart_rx_fifo, app_uart_rx_buffer, 512);
//            Usart3_Init();
           break;
       default:

           break;
   }
   u8g2_SendBuffer(&u8g2);
}
/* ==================================================================
 * 主页面（menu_rank == 1）★ 2026-09-22 按用户提供的版式图重做
 *   版式图：D:\ai_work\1_tt.png（1280×560 = 384×168 的 3.3333 倍，宽高比完全一致）
 *   契约：02_协议文档\协议_主页面版式_冻结.md
 *
 * 版式（384×168 屏幕坐标；括号内为版式图量出的对应位置）：
 *   ┌──────────────────────────────────────────────────────────────┐ 外框 (1,1,382,166)
 *   │ ███ 粗顶边 (1,1,382,3)          〔图上 y 1..4〕                │
 *   │            矿用巷道综合测站（居中）                            │ baseline 18
 *   │  主机1 分站2 发往中继 信号3                                    │ baseline 34
 *   │                            版本V1.0 电池3.65V 开机              │ baseline 48
 *   ├──────────────────────────────────────────────────────────────┤ y=51 / y=53 双线
 *   │ 01 锚杆  51 kN           ┃ 06 位移 3105 mm                    │ baseline 66〔y56..68〕
 *   ├──────────────────────────╂───────────────────────────────────┤ y=71〔y70..71〕
 *   │ 02 …                     ┃ 07 …                              │ baseline 84〔y74..86〕
 *   ├──────────────────────────╂───────────────────────────────────┤ y=89
 *   │ 03 …                     ┃ 08 …                              │ baseline 102〔y92..104〕
 *   ├──────────────────────────╂───────────────────────────────────┤ y=107
 *   │ 04 …                     ┃ 09 …                              │ baseline 120〔y110..122〕
 *   ├──────────────────────────╂───────────────────────────────────┤ y=125
 *   │ 05 …                     ┃ 10 …                              │ baseline 138〔y128..140〕
 *   ├──────────────────────────────────────────────────────────────┤ y=143〔y142〕
 *   │▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓│ y=145/147 粗双线〔y142..147〕
 *   │ [已绑定 3]  [已用通道 9]  [报警 2]                            │ baseline 162〔y154..166〕
 *   └──────────────────────────────────────────────────────────────┘
 *
 * 网格：**5 行 × 2 列 = 10 格，每格 1 个通道**（用户确认），右列紧接左列：
 *   第 1 页 = 通道 1..10（左列 1..5，右列 6..10）
 *   第 2 页 = 通道 11..20（左列 11..15，右列 16..20）
 *   分页由 0x01 帧的 chu_num1 决定（== 2 为第 2 页，其余按第 1 页）。
 * 每格一行式：`编号(2 位) 类型名 数值(右对齐) 单位`
 *   数值：MG/位移/应力/倾角 = 原始值/10，保留一位小数（倾角按有符号），激光原样
 *   单位：kN（锚杆）/ mm（位移·激光·裂缝）/ °（倾角）/ MPa（应力）（契约 §4.2）
 *
 * 数据分工：STM32 提供 主机/分站/发往/信号/版本/电池/状态/页码；
 *           CH584M 本地提供 已绑定数、已用通道数、告警数、20 通道类型与数值。
 * ================================================================== */
#define MAIN_GRID_TOP     53                        /* 网格上边线 */
#define MAIN_ROW_H        18                        /* 行高（14px 字 + 4px 间隔） */
#define MAIN_ROWS         5                         /* 行数 */
#define MAIN_COL_SPLIT_X  190                       /* 中间竖线 x（图上 184..194） */
#define MAIN_CELL_L_X     8                         /* 左格起点 */
#define MAIN_CELL_R_X     192                       /* 右格起点 */
#define MAIN_NUM_W        20                        /* 编号列宽 */
#define MAIN_TYPE_W       26                        /* 类型列宽（2 个汉字） */
#define MAIN_VAL_RIGHT    150                       /* 数值右对齐（相对格起点） */
#define MAIN_UNIT_X       154                       /* 单位起点（相对格起点） */
#define MAIN_ROW_BASE(i)  ((uint8_t)(MAIN_GRID_TOP + 13 + MAIN_ROW_H * (i)))

void UI_Main_Display(data_LIST *pData)
{
    char     buf[48];
    char     send_str[12];
    uint8_t  i, first, page, row, col, cx, y;
    uint16_t w;

    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetFont(&u8g2, UI_FONT_CN);          /* 中文全字库，见文件顶部说明 */

    /* ---------- 外框 + 粗顶边（版式图四周有框、顶部为粗条） ---------- */
    u8g2_DrawFrame(&u8g2, 1, 1, 382, 166);
    u8g2_DrawBox(&u8g2, 1, 1, 382, 3);

    /* ---------- 大标题（居中，版式图标题位于屏幕中段） ---------- */
    {
        const char *title = "矿用巷道综合测站";
        w = (uint16_t)u8g2_GetUTF8Width(&u8g2, title);
        ui_draw((uint16_t)((384 - w) / 2), 18, title);
    }

    /* ---------- 状态行：主机/分站/发往/信号 + 版本/电池/开关机 ---------- */
    if (pData->UI_main.send_host_num == 122)      sprintf(send_str, "分站");
    else if (pData->UI_main.send_host_num == 121) sprintf(send_str, "无");
    else if (pData->UI_main.send_host_num == 0)   sprintf(send_str, "中继");
    else                                          sprintf(send_str, "%d", pData->UI_main.send_host_num);

    sprintf(buf, "主机%d 分站%d 发往%s 信号%d",
            pData->UI_main.host_num, pData->UI_main.sub_num, send_str,
            pData->UI_main.Lora_rssi);
    ui_draw(8, 34, buf);

    /* ★ 契约 §6.5：On/Off 语义统一为 STM32 口径（1 = 开机/工作，0 = 关机）
     * ⚠️ 标签「版本 / 电池」必须保留 —— 功能清单契约要求主页含 STM32 六项
     *    （主机/分站/发往/信号/版本/电池），删掉标签会被跨端套件判为回归。
     *    单独占一行，避免长地址与版本/电池/状态互相覆盖或被宽度条件隐藏。 */
    sprintf(buf, "版本V%d.%d 电池%d.%d%dV %s",
            pData->UI_main.version / 10, pData->UI_main.version % 10,
            pData->UI_main.vbat / 100, pData->UI_main.vbat % 100 / 10,
            pData->UI_main.vbat % 100 % 10,
            (pData->UI_main.state == 1) ? "开机" : "关机");
    w = (uint16_t)u8g2_GetUTF8Width(&u8g2, buf);
    ui_draw((uint16_t)(384 - 8 - w), 48, buf);

    /* ---------- 分隔线：表头双线 + 网格线（版式图对应位置） ---------- */
    u8g2_DrawHLine(&u8g2, 3, 51, 378);
    u8g2_DrawHLine(&u8g2, 3, 53, 378);

    for (i = 1; i <= MAIN_ROWS; i++)
        u8g2_DrawHLine(&u8g2, 3, (uint8_t)(MAIN_GRID_TOP + MAIN_ROW_H * i), 378);
    u8g2_DrawVLine(&u8g2, MAIN_COL_SPLIT_X, MAIN_GRID_TOP,
                   (uint8_t)(MAIN_ROW_H * MAIN_ROWS));

    /* ---------- 通道网格：5 行 × 2 列 = 10 格，每格 1 个通道 ---------- */
    page  = (pData->UI_main.chu_num1 == 2) ? 2 : 1;
    first = (uint8_t)((page == 1) ? 0 : 10);

    for (i = 0; i < 10; i++)
    {
        uint8_t   ch  = (uint8_t)(first + i);
        device_t *d   = &CH_com_buf[ch];

        row = (uint8_t)(i % MAIN_ROWS);                /* 0..4 */
        col = (uint8_t)(i / MAIN_ROWS);                /* 0 = 左列，1 = 右列 */
        cx  = (uint8_t)(col ? MAIN_CELL_R_X : MAIN_CELL_L_X);
        y   = MAIN_ROW_BASE(row);

        if (d->valid && d->Type > TPYE_NONE && d->Type < TPYE_END)
        {
            sprintf(buf, "%02d", ch + 1);                  /* 通道号 01..20 */
            ui_draw(cx, y, buf);

            ui_draw((uint16_t)(cx + MAIN_NUM_W), y, CH_TYPE_NAME[d->Type]);   /* 类型 */
            /* ★ 契约 §4.1：MG / 位移 / 应力 / 倾角 显示 = 原始值/10（QJ 有符号）；激光原样 */
            ch_disp_format(buf, sizeof(buf), (Sensor_Tpye)d->Type, d->CH_data);
            w = (uint16_t)u8g2_GetUTF8Width(&u8g2, buf);
            ui_draw((uint16_t)(cx + MAIN_VAL_RIGHT - w), y, buf);             /* 数值右对齐 */
            if (CH_TYPE_UNIT[d->Type][0] != '\0')
                ui_draw((uint16_t)(cx + MAIN_UNIT_X), y, CH_TYPE_UNIT[d->Type]); /* 单位 */
        }
        else
        {
            /* ★ 契约 §4.3：未绑定 / 不存在的通道显示 **`序号.-----`**
             *   （例：第 3 通道 → `3.-----`）。整行作为**一个字符串**画出，
             *   序号不补零，与契约示例逐字符一致（不再画旧的 `--` 占位符）。 */
            sprintf(buf, "%d.-----", ch + 1);
            ui_draw(cx, y, buf);
        }
    }

    /* ---------- 底部：粗双线 + 三个方框字段（版式图底部为方框区） ---------- */
    u8g2_DrawHLine(&u8g2, 3, 143, 378);
    u8g2_DrawHLine(&u8g2, 3, 145, 378);
    u8g2_DrawHLine(&u8g2, 3, 147, 378);

    u8g2_DrawFrame(&u8g2, 4,   149, 122, 16);
    u8g2_DrawFrame(&u8g2, 130, 149, 124, 16);
    u8g2_DrawFrame(&u8g2, 258, 149, 122, 16);

    sprintf(buf, "已绑定%d", g_binding_count);
    ui_draw(10, 162, buf);
    sprintf(buf, "已用通道%d", count_used_channels());
    ui_draw(136, 162, buf);
    sprintf(buf, "报警%d", (int)(g_name_err_count + g_volt_err_count));
    ui_draw(264, 162, buf);
}

void UI_Menu_Display(void)
{
    /* ★ 契约 §6.1：UI_Select 直接来自串口字节（Usart3_task.c 的
     *   UI_Select = rank2，以及本文件 UI_Control() 的 UI_Select = rank2_addr），
     *   旧代码只在第 4 步给 Main_List 加了 <8 保护，而第 5/6 步的
     *   Menu_List[UI_Select] 完全没有边界检查 —— 越界读函数指针会跳到任意地址。
     *   这里统一钳制到合法范围（0..MAIN_LEN-1）。
     * ⚠️ MAIN_LEN 取自 Main_List[]，却同时用于索引 Menu_List[]，**两个列表长度
     *   必须始终一致（当前都是 8 项）**；将来只给其中一个增删项就会越界。 */
    if (UI_Select >= MAIN_LEN) UI_Select = 0;

    /* ★★ 契约_息屏省电数据链与页面体系 §4.2 / §4.4（2026-09-22）：
     *   三个"参数过多"页面 —— 设备信号(re=3) / 设备电压(re=4) / 通道绑定名称(re=5) ——
     *   按需求改成**整屏显示**（左列通道 1~10、右列 11~20，一屏 20 条）。
     *   ⚠️ 必须在**画左侧菜单之前**分流：否则左列内容（尤其名称页的长名称）
     *      会压在左侧菜单文字上（旧实现即如此）。渲染完直接 return，
     *      整屏只有该页内容 + 标题带 + 「返回」。 */
    if (Data_list1.menu_rank == 3)
    {
        uint8_t re_top = Data_list1.UI_main.re_flag;
        if (re_top == 3 || re_top == 4)
        {
            UI_Select = Data_list1.rank2_addr;      /* 与常规路径一致，供页面内引用 */
            install_Control();
            return;
        }
        if (re_top == 5)
        {
            UI_Select = Data_list1.rank2_addr;
            summary_Control();
            return;
        }
    }

    // 1. 绘制左侧菜单列表（所有项）
    /* ★ 中文一律 UI_FONT_CN：菜单项含"设备绑定/信息汇总"，lunar 字库缺
     *   绑/息/汇/总 等字形，用 lunar 会显示空白（契约 §7）。 */
    u8g2_SetFont(&u8g2, UI_FONT_CN);
    for (uint8_t i = 0; i < 8; i++)
    {
        u8g2_DrawUTF8(&u8g2, Main_List[i].posx, Main_List[i].posy, Main_List[i].name);
    }

    // 2. 绘制左侧外框和内框
    u8g2_DrawRFrame(&u8g2, 4, 4, 94, 160, 6);   // 外框
    u8g2_DrawRFrame(&u8g2, 6, 6, 90, 156, 4);   // 内框

    // 3. 右侧区域外框
    u8g2_DrawRFrame(&u8g2, 102, 4, 278, 160, 6);

    // 4. 在当前选中的菜单项上画矩形框（高亮）
    if(Data_list1.menu_rank==2)
    {
        if (UI_Select < MAIN_LEN)   // 确保索引有效
        {
            u8g2_SetFont(&u8g2, UI_FONT_CN);
            // 获取菜单名称的像素宽度
            uint8_t name_width = u8g2_GetUTF8Width(&u8g2, Main_List[UI_Select].name);
            // 定义框的边距（与文字保持间距）
            uint8_t padding = 2;
            // 计算框的坐标（根据字体基线调整）
            // wqy14 墨迹 = [基线-12, 基线+1] ⇒ 框取 [基线-14, 基线+4]
            uint8_t x = Main_List[UI_Select].posx - padding - 2;
            uint8_t y = Main_List[UI_Select].posy - 14;   // 根据实际字体调整
            uint8_t w = name_width + padding * 2 + 4;
            uint8_t h = 19;  // 框高度略大于 14px 字高
            // 画框（仅边框，不填充）
            u8g2_DrawFrame(&u8g2, x, y, w, h);
        }
    }

    // 5. 右侧内容区域：绘制标题（使用 Menu_List 中的名称）
    /* ★ 标题字体改用 UI_FONT_CN（原 u8g2_font24_lunar 缺字）；
     *   wqy14 墨迹 = [基线-12, 基线+1]，标题带为 y=6..34，
     *   故基线取 28（墨迹 16..29），分割线仍在 y=34。 */
    u8g2_SetFont(&u8g2, UI_FONT_CN);
    u8g2_DrawUTF8(&u8g2, Menu_List[UI_Select].posx, 28, Menu_List[UI_Select].name);
    u8g2_DrawLine(&u8g2, 104, 34, 210, 34);   // 标题下分割线

    // 6. 调用当前选中菜单对应的回调函数（显示具体内容）
    Menu_List[UI_Select].function();
}

/* ==================================================================
 * 「设备绑定」页（rank2_addr == 2，契约 §1.2）
 *
 *   rank3_addr（列表高亮项）：0 = 已绑定设备:个数 / 1 = 一键解绑 /
 *                            2 = 绑定设备 / 3 = 返回
 *   re_flag（二级子页，契约 §2.3）：1 = 一键解绑子页；2 = 绑定设备子页；
 *                            0 且 rank3==0 = 已绑定设备子页
 *
 *   ★ CH584M 只画不执行：真正的解绑由 STM32 发 0x04、保存由 0x05 执行。
 *   ★ 设备行显示 **名称 + 12 位十六进制 MAC**（清单要求"MAC 端可见"）。
 *     名称与 MAC 分两行，超出当前区域的设备使用已有秒时基轮显。
 * ================================================================== */

/* 已绑定设备列表子页（re_flag==1 与 rank3==0 共用）
 *   with_buttons = 1 → 底部两项"解绑 / 返回"（高亮由 rank3 决定：0=解绑，1=返回）
 *   with_buttons = 0 → 只有"返回"（固定高亮） */
static void binding_device_list_page(uint8_t rank3, uint8_t with_buttons)
{
    char    line[48];
    uint8_t i, n, first, pages;

    u8g2_SetFont(&u8g2, UI_FONT_CN);

    sprintf(line, "总设备数:%d", g_binding_count);
    ui_draw(110, 48, line);

    n = g_binding_count;
    if (n > MAX_BINDING_NUM) n = MAX_BINDING_NUM;
    pages = (uint8_t)((n + 2) / 3);
    first = pages ? (uint8_t)(((g_name_chk_sec / 4) % pages) * 3) : 0;
    for (i = first; i < n && i < first + 3; i++)
    {
        uint8_t y = (uint8_t)(62 + 28 * (i - first));

        sprintf(line, "%02d", i + 1);
        ui_draw(110, y, line);
        ui_draw_name(130, y, g_binding_list[i].name, 244);

        sprintf(line, "%02X%02X%02X%02X%02X%02X",
                g_binding_list[i].mac[0], g_binding_list[i].mac[1], g_binding_list[i].mac[2],
                g_binding_list[i].mac[3], g_binding_list[i].mac[4], g_binding_list[i].mac[5]);
        ui_draw(130, (uint16_t)(y + 14), line);
    }

    if (with_buttons)
    {
        ui_draw(110, 152, "解绑");
        ui_hl_box(110, 152, 28, (uint8_t)(rank3 == 0));
        ui_draw(250, 152, "返回");
        ui_hl_box(250, 152, 28, (uint8_t)(rank3 != 0));
    }
    else
    {
        ui_draw(110, 152, "返回");
        ui_hl_box(110, 152, 28, 1);
    }
}

/* 绑定设备子页（re_flag==2）：显示最近扫描到的蓝牙名称 + 保存目前设备/返回
 *   "保存目前设备" = STM32 发 0x05（CH584M 在 case 0x05 里落盘绑定表）。 */
static void binding_scan_page(uint8_t rank3)
{
    uint8_t     i, n;
    const char *nm;

    u8g2_SetFont(&u8g2, UI_FONT_CN);

    ui_draw(110, 48, "扫描到的蓝牙名称");

    n = scan_name_cache_count();
    if (n == 0) ui_draw(110, 62, "--");     /* 还没扫到任何设备 */
    for (i = 0; i < n && i < 6; i++)
    {
        nm = scan_name_cache_name(i);
        if (nm != NULL)
            ui_draw_name(110, (uint16_t)(62 + 14 * i), (const uint8_t *)nm, 264);
        else
            ui_draw(110, (uint16_t)(62 + 14 * i), "--");
    }

    ui_draw(110, 152, "保存目前设备");
    ui_hl_box(110, 152, 84, (uint8_t)(rank3 == 0));
    ui_draw(250, 152, "返回");
    ui_hl_box(250, 152, 28, (uint8_t)(rank3 != 0));
}

void binding_Control(void)
{
    uint8_t re    = Data_list1.UI_main.re_flag;
    uint8_t rank3 = Data_list1.rank3_addr;
    char    buf[48];

    /* ---- 二级子页（由 re_flag 选择，见本段顶部注释） ----
     * ★ 修正（复核清单原文后）：清单只要求「1.已绑定设备：个数」在**菜单里就地显示个数**；
     *   明确"进入对应的子页面"的只有「2.一键解绑」与「3.绑定设备」。
     *   原先这里还有 `if (rank3 == 0) → 明细子页`，而进入本页时 rank3_addr 默认就是 0，
     *   于是 4 项菜单直接看不到（STM32 侧也从未为该项设过 Sub2，属单方面多画一页）。
     *   故删除该分支：本页恒画 4 项列表；设备明细（名称 + 12 位 MAC）在
     *   re_flag==1 的「一键解绑」子页里展示，与清单一致。 */
    if (re == 2) { binding_scan_page(rank3);             return; }   /* 绑定设备 */
    if (re == 1) { binding_device_list_page(rank3, 1);   return; }   /* 一键解绑 */

    /* ---- 主列表（4 项） ---- */
    u8g2_SetFont(&u8g2, UI_FONT_CN);

    sprintf(buf, "已绑定设备:%d", g_binding_count);
    ui_draw(110, 52, buf);                  /* 清单「1.已绑定设备：个数」= 菜单里就地显示个数 */
    ui_hl_box(110, 52, 83, (uint8_t)(rank3 == 0));

    ui_draw(110, 70, "一键解绑");
    ui_hl_box(110, 70, 56, (uint8_t)(rank3 == 1));

    ui_draw(110, 88, "绑定设备");
    ui_hl_box(110, 88, 56, (uint8_t)(rank3 == 2));

    ui_draw(110, 106, "返回");
    ui_hl_box(110, 106, 28, (uint8_t)(rank3 == 3));
}

/* ==================================================================
 * 「信息汇总」页（rank2_addr == 6，契约 §1.4）
 *
 *   rank3_addr：0 = 已绑定的设备名称（子页 re_flag=5）
 *               1 = 蓝牙名称错误警报:个数（就地显示 g_name_err_count）
 *               2 = 蓝牙电压异常警报:个数（就地显示 g_volt_err_count）
 *               3 = 恢复出厂（确认子页 re_flag=6）
 *               4 = 返回
 *   ★ 恢复出厂的实际执行 = STM32 发 0x07（CH584M 在 case 0x07 里清 RAM + 擦 Flash）。
 * ================================================================== */

/* ==================================================================
 * ★★ 契约_息屏省电数据链与页面体系 §4.2 / §4.3 / §4.4（2026-09-22 冻结）
 *    三个"参数过多"页面的**单页全屏双列**版式常量：
 *      安装调试 → 设备信号   install_ch_page(3, ..)
 *      安装调试 → 设备电压   install_ch_page(4, ..)
 *      信息汇总 → 通道绑定名称 summary_name_page(..)
 *
 *  版式：左列 = 通道 1..10，右列 = 通道 11..20，**一屏 20 条、不再分页**
 *        （K1/K3 翻页对这三页无效；STM32 侧 chu_num2 仍照发，字段语义不变）。
 *  行内容：`序号 值 单位`（名称页 = `序号 名称`）。
 *  未绑定 / 不存在的通道：`序号.-----`（如第 3 通道 → `3.-----`，§4.3）；
 *        已绑定但本轮无数据的通道照旧显示上次值。
 *  「返回」项（§4.4）：画在标题带右侧，**固定高亮** —— 本页只有这一个可选项，
 *        K2（确认）必落在它上面 ⇒ 回上一级（实际由 STM32 侧 sub2_close()
 *        执行：UI.Sub2=Sub2_None + 回菜单；CH584M 只画不执行）。
 *
 *  ★ 列几何**复用主页面的双列网格常量**（契约 §4.2 明文要求）：
 *      MAIN_CELL_L_X(8) / MAIN_CELL_R_X(192) / MAIN_COL_SPLIT_X(190) /
 *      MAIN_NUM_W(20) / MAIN_VAL_RIGHT(150)。
 *  ⚠️ 行距**不能**复用 MAIN_ROW_H(18)：那是主页面的"5 行"网格，20 条要 10 行，
 *      10 × 18 = 180 > 168（屏高），几何上放不下。故本组页面取
 *      PARAM_ROW_H = 13px —— 既等于改动前这三页的行距（47 + 13×i），
 *      也等于 UI_FONT_CN 汉字的步进；行基线 47..164 与改动前 10 行页同高，
 *      墨迹上边 35、下边 165，不与右侧面板内框(y=4..163)冲撞。
 *  ⚠️ 已知版面关系（**改动前就存在**，本次未改变其性质）：左列起点沿用主页面
 *      的 MAIN_CELL_L_X(8)，而菜单页的左侧菜单文字画在 x=20..76（UI_Menu_Display
 *      第 1 步），两者会有重叠；改动前的这三页同样从 x=8/x=50 起画，属同一现象。
 *      如需避开，只需把本组页面的左列起点改成 110、右列改成 250 一处常量。
 * ================================================================== */
#define PARAM_GRID_TOP    47                 /* 首行基线（= 改动前这三页的首行） */
#define PARAM_ROW_H       13                 /* 行距：10 行 ⇒ 47..164 */
#define PARAM_ROWS        10                 /* 每列行数（左 1..10 / 右 11..20） */
#define PARAM_ROW_BASE(i) ((uint8_t)(PARAM_GRID_TOP + PARAM_ROW_H * (i)))
#define PARAM_VLINE_TOP   36                 /* 双列中竖线上端（标题带分隔线 y=34 之下） */
#define PARAM_VLINE_LEN   129                /* 中竖线长：36..164（到右侧面板内框底边） */
#define PARAM_LABEL_X     200                /* 子页标识 x（沿用改动前的位置，不再带页码） */
#define PARAM_TITLE_BASE  28                 /* 标题带基线（与 UI_Menu_Display 的标题同带） */
#define PARAM_BACK_X      300                /* 「返回」项文字起点（§4.4） */
#define PARAM_BACK_W      28                 /* 「返回」文字宽度（wqy14 实测 28px） */
#define PARAM_NAME_MAX_W  156                /* 名称可用宽度：左列 190-(8+20)=162，留 6px 余量 */

/* 已绑定设备名称子页（re_flag==5）：20 个通道的绑定名称，**单页全屏双列**（§4.2/§4.3/§4.4）
 *   page 入参保留：chu_num2 仍由 0x01 帧下发（跨端页面码字段语义不变，契约 §5），
 *   但本页**不再分页**。 */
static void summary_name_page(uint16_t page)
{
    char     buf[48];
    uint8_t  i, row, col;
    uint16_t cx, y;
    device_t *d;

    (void)page;                              /* 契约 §4.2：不再分页（入参保留以便审计页面码） */

    u8g2_SetFont(&u8g2, UI_FONT_CN);

    ui_draw(PARAM_LABEL_X, PARAM_TITLE_BASE, "名称");        /* 不再有 "1-10/11-20" 页码 */

    /* 「返回」（契约 §4.4）：固定高亮 = 本页唯一可选项 */
    ui_draw(PARAM_BACK_X, PARAM_TITLE_BASE, "返回");
    ui_hl_box(PARAM_BACK_X, PARAM_TITLE_BASE, PARAM_BACK_W, 1);

    /* 双列分隔竖线：左列 1..10 / 右列 11..20 */
    u8g2_DrawVLine(&u8g2, MAIN_COL_SPLIT_X, PARAM_VLINE_TOP, PARAM_VLINE_LEN);

    for (i = 0; i < 20; i++)
    {
        row = (uint8_t)(i % PARAM_ROWS);              /* 0..9 */
        col = (uint8_t)(i / PARAM_ROWS);              /* 0 = 左列 1..10，1 = 右列 11..20 */
        cx  = (uint16_t)(col ? MAIN_CELL_R_X : MAIN_CELL_L_X);
        y   = PARAM_ROW_BASE(row);
        d   = &CH_com_buf[i];

        /* ★ 契约 §4.3：未绑定 / 不存在的通道画成 `序号.-----`（序号不补零，
         *   与契约示例 `3.-----` 逐字符一致） */
        if (!(d->valid && d->name[0] != '\0'))
        {
            sprintf(buf, "%d.-----", i + 1);
            ui_draw(cx, y, buf);
            continue;
        }

        sprintf(buf, "%02d", i + 1);                  /* 已绑定行：编号对齐 2 位 */
        ui_draw(cx, y, buf);

        ui_draw_name((uint16_t)(cx + MAIN_NUM_W), y, d->name, PARAM_NAME_MAX_W);
    }
}

/* 恢复出厂确认子页（re_flag==6）：两项，高亮由 rank3 决定（0=确认，其余=返回） */
static void summary_factory_page(uint8_t rank3)
{
    u8g2_SetFont(&u8g2, UI_FONT_CN);

    ui_draw(110, 64, "确认恢复出厂设置?");
    ui_draw(110, 120, "确认");
    ui_hl_box(110, 120, 28, (uint8_t)(rank3 == 0));
    ui_draw(210, 120, "返回");
    ui_hl_box(210, 120, 28, (uint8_t)(rank3 != 0));
}

void summary_Control(void)
{
    uint8_t re    = Data_list1.UI_main.re_flag;
    uint8_t rank3 = Data_list1.rank3_addr;
    char    buf[48];

    if (re == 5) { summary_name_page(Data_list1.UI_main.chu_num2); return; }  /* 通道绑定名称 */
    if (re == 6) { summary_factory_page(rank3);                     return; }  /* 恢复出厂确认 */

    u8g2_SetFont(&u8g2, UI_FONT_CN);

    ui_draw(110, 52, "已绑定的设备名称");
    ui_hl_box(110, 52, 112, (uint8_t)(rank3 == 0));

    sprintf(buf, "蓝牙名称错误警报:%d", g_name_err_count);
    ui_hl_box(110, 70, ui_draw(110, 70, buf), (uint8_t)(rank3 == 1));

    sprintf(buf, "蓝牙电压异常警报:%d", g_volt_err_count);
    ui_hl_box(110, 88, ui_draw(110, 88, buf), (uint8_t)(rank3 == 2));

    ui_draw(110, 106, "恢复出厂");
    ui_hl_box(110, 106, 56, (uint8_t)(rank3 == 3));

    ui_draw(110, 124, "返回");
    ui_hl_box(110, 124, 28, (uint8_t)(rank3 == 4));
}

typedef struct {

    uint16_t posx;                   // 文本起始x坐标
    uint16_t posy;                   // 文本基线y坐标
    uint8_t font_size;            // 字体大小（16/24/32）
    uint16_t label_wight;         // 显示文本的宽度
    uint8_t  kuang_wight;         //方框离文字的宽
    uint8_t  kuang_high;          //方框离文字的高

} OptionItem;
void addr_Control()
{
    uint8_t menu_rank=0;
    uint8_t rank3_addr=0;

    OptionItem laber[10]=
    {
          {120, 80, 14,0,4,4},
          {250, 80, 14,0,4,4},
          {120, 120, 14,0,4,4},
          {250, 120, 14,0,4,4},
          {120, 156, 14,0,4,4},
          {250, 156, 14,0,4,4}
    };

    u8g2_SetFont(&u8g2, UI_FONT_CN);
    u8g2_DrawUTF8(&u8g2, 120, 80, "分区:");          // 框y=72,高26,垂直居中约90
    laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"分区:");;

    char tmp[20];
    sprintf(tmp, "%d", Data_list1.Menu_rank1.set_sub_num);
    u8g2_DrawUTF8(&u8g2, 250, 80, tmp);
    laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp);

    u8g2_DrawUTF8(&u8g2, 120, 120, "地址:");         // 下框y=104,高26,垂直居中约122
    laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"地址:");

    char tmp1[20];
    sprintf(tmp1, "%d", Data_list1.Menu_rank1.set_host_num);
    u8g2_DrawUTF8(&u8g2, 250, 120, tmp1);
    laber[3].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp1);;

    u8g2_DrawUTF8(&u8g2, 120, 156, "保存并重启");         // 下框y=104,高26,垂直居中约122
    laber[4].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"保存并重启");

    u8g2_DrawUTF8(&u8g2, 250, 156, "不保存返回");          // 框y=72,高26,垂直居中约90
    laber[5].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"不保存返回");


    menu_rank=Data_list1.menu_rank;
    rank3_addr=Data_list1.rank3_addr;

    if (menu_rank == 3)   // 确保索引有效
    {
        if(rank3_addr<6)
        {
           // u8g2_SetFont(&u8g2, u8g2_font16_lunar);
            // 获取菜单名称的像素宽度
            uint16_t name_width =laber[rank3_addr].label_wight;
            // 定义框的边距（与文字保持间距）
            uint16_t padding = laber[rank3_addr].kuang_wight;
            // 计算框的坐标（根据字体基线调整）
            // u8g2 的 y 坐标是基线，假设字体高度约为 16，ascent 约 14，descent 约 2
            // 框的左上角 y = posy - 14 - 2（适当微调）
            uint16_t x = laber[rank3_addr].posx - padding;
            uint16_t y = laber[rank3_addr].posy -laber[rank3_addr].font_size ;  // 根据实际字体调整
            uint16_t w = name_width + padding * 2;
            uint16_t h =laber[rank3_addr].font_size+ laber[rank3_addr].kuang_high*2;  // 框高度略大于字体高度
            // 画框（仅边框，不填充）
            u8g2_DrawFrame(&u8g2, x, y, w, h);
        }
    }
}

void addr_Control1(void)
{
    uint8_t menu_rank = Data_list1.menu_rank;
    uint8_t rank3_addr = Data_list1.rank3_addr;

    // 定义每个选项的配置：{文本(为NULL表示数值), x, y, 数值来源}
    typedef struct {
        const char* text;       // 标签文本，若为NULL表示动态数值
        uint8_t x;
        uint8_t y;
        uint16_t value;         // 数值（仅当 text==NULL 时使用）
    } ItemConfig;

    // 配置表（共6个元素）
    ItemConfig configs[] = {
        {"分区:",    120, 80,  0},
        {NULL,       250, 80,  Data_list1.Menu_rank1.set_sub_num},   // 分区值
        {"地址:",    120, 120, 0},
        {NULL,       250, 120, Data_list1.Menu_rank1.set_host_num},  // 地址值
        {"保存并重启", 120, 156, 0},
        {"不保存返回", 250, 156, 0}
    };
    #define OPTION_COUNT (sizeof(configs)/sizeof(configs[0]))

    OptionItem laber[OPTION_COUNT];
    u8g2_SetFont(&u8g2, u8g2_font24_lunar);

    // 循环绘制并计算宽度
    for (int i = 0; i < OPTION_COUNT; i++)
    {
        // 填充固定属性
        laber[i].posx = configs[i].x;
        laber[i].posy = configs[i].y;
        laber[i].font_size = 24;
        laber[i].kuang_wight = 4;
        laber[i].kuang_high = 4;

        // 决定显示文本
        char dyn_buf[20] = {0};
        const char* display_text;
        if (configs[i].text == NULL) {
            // 数值选项：生成数字字符串
            sprintf(dyn_buf, "%d", configs[i].value);
            display_text = dyn_buf;
        } else {
            display_text = configs[i].text;
        }

        // 绘制并获取宽度
        u8g2_DrawUTF8(&u8g2, laber[i].posx, laber[i].posy, display_text);
        laber[i].label_wight = u8g2_GetUTF8Width(&u8g2, display_text);
    }

    // ---------- 画框 ----------
    if (menu_rank == 3 && rank3_addr < OPTION_COUNT)
    {
        OptionItem *sel = &laber[rank3_addr];
        uint16_t padding = sel->kuang_wight;
        // 微调 y 偏移：使用 font_size - 2 使框更贴合文字（可选）
        uint16_t y_offset = sel->font_size - 2;   // 24-2=22，可改为 sel->font_size 保持原样
        uint16_t x = sel->posx - padding;
        uint16_t y = sel->posy - y_offset;
        uint16_t w = sel->label_wight + padding * 2;
        uint16_t h = sel->font_size + sel->kuang_high * 2;
        u8g2_DrawFrame(&u8g2, x, y, w, h);
    }
}
void networking_Control1()
{

    uint8_t menu_rank=0;
     uint8_t rank3_addr=0;
      menu_rank = Data_list1.menu_rank;
      rank3_addr = Data_list1.rank3_addr;

     OptionItem laber[10]=
     {
           {120, 112, 14,0,4,4},   /* rank3_addr = 0  "开始组网" */
           {250, 112, 14,0,4,4},   /* rank3_addr = 1  "返回"     */
           {120, 150, 14,0,4,4},   /* rank3_addr = 2  解绑全部（屏上显示"重置组网"） */
           {250, 150, 14,0,4,4},   /* rank3_addr = 3  保存绑定（屏上显示"保存组网"） */
     };

      /* ★ 改用 UI_FONT_CN（文泉驿 GB2312 半区，全字库）——原 u8g2_font24_lunar
       * 只有 125 个汉字，功能清单的菜单文案会缺字显示空白。详见 yuying_TFT.h。 */
      u8g2_SetFont(&u8g2, UI_FONT_CN);

      u8g2_DrawUTF8(&u8g2, 120, 60, "组网");          // 框y=72,高26,垂直居中约90

      /* ★ 组网测试页子选项扩展到 4 项（与协议契约 §5 / STM32 侧顺序严格一致）：
       *   rank3_addr 0=开始组网  1=返回  2=解绑全部  3=保存绑定
       *   CH584M 侧只负责"显示 + 高亮框"，点击后由 STM32 发 0x04/0x05 请求，
       *   CH584M 在 parse_received_frame() 的 case 0x04/0x05 里执行。
       *   布局 2×2（面板约 x=102..380 / y=4..164，24px 字体不重叠）：
       *     第 1 行 y=112：开始组网(x=120) / 返回(x=250)
       *     第 2 行 y=150：重置组网(x=120) / 保存组网(x=250)
       *   rank3_addr 已不再兼任"扫描模式"（P1/P2 后由 g_scan_mode 负责），
       *   因此这里扩项不会影响扫描/绑定行为。
       *
       *   ⚠️ 用字说明（实机字形已核验）：
       *   本工程可用的三个中文点阵字库（u8g2_font16_lunar / u8g2_font24_lunar /
       *   u8g2_font32_lunar）都只有 **同一套 207 个字形**，其中**没有**
       *   "解""绑""全""部"四个字（核验脚本：D:\ai_work\_tools\u8g2_font_glyph_check.js，
       *   直接按 u8g2 新字体格式解析 unicode 段）。
       *   若照抄契约里的"解绑全部 / 保存绑定"，这两项会**显示为空**（u8g2 跳过无字形字符）。
       *   因此改用**同义且字形齐备**的措辞，rank3_addr 顺序与语义完全不变：
       *     rank3_addr = 2 → "重置组网"（清空全部绑定 + 清 Flash，即"解绑全部"）
       *     rank3_addr = 3 → "保存组网"（RAM 绑定表落盘 Data-Flash，即"保存绑定"）
       *   若必须显示"解绑/绑定"字样，需要另行用 bdfconv 生成含这些字形的字库子集。 */
      u8g2_DrawUTF8(&u8g2, 120, 112, "开始组网");
      laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"开始组网");
      u8g2_DrawUTF8(&u8g2, 250, 112, "返回");
      laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"返回");
      u8g2_DrawUTF8(&u8g2, 120, 150, "重置组网");
      laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"重置组网");
      u8g2_DrawUTF8(&u8g2, 250, 150, "保存组网");
      laber[3].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"保存组网");


      if (menu_rank == 3)   // 确保索引有效
          {
              if(rank3_addr<4)   /* ★ 4 项：0..3（原为 <2） */
              {
                 // u8g2_SetFont(&u8g2, u8g2_font16_lunar);
                  // 获取菜单名称的像素宽度
                  uint16_t name_width =laber[rank3_addr].label_wight;
                                    // 定义框的边距（与文字保持间距）
                                    uint16_t padding = laber[rank3_addr].kuang_wight;
                                    // 计算框的坐标（根据字体基线调整）
                                    // u8g2 的 y 坐标是基线，假设字体高度约为 16，ascent 约 14，descent 约 2
                                    // 框的左上角 y = posy - 14 - 2（适当微调）
                                    uint16_t x = laber[rank3_addr].posx - padding;
                                    uint16_t y = laber[rank3_addr].posy -laber[rank3_addr].font_size ;  // 根据实际字体调整
                                    uint16_t w = name_width + padding * 2;
                                    uint16_t h =laber[rank3_addr].font_size+ laber[rank3_addr].kuang_high*2;  // 框高度略大于字体高度
                                    // 画框（仅边框，不填充）
                                    u8g2_DrawFrame(&u8g2, x, y, w, h);
              }
          }
}

void networking_Control2()
{
    uint8_t menu_rank=0;
         uint8_t rank3_addr=0;
          menu_rank = Data_list1.menu_rank;
          rank3_addr = Data_list1.rank3_addr;
      u8g2_SetFont(&u8g2, UI_FONT_CN);

      u8g2_DrawUTF8(&u8g2, 110, 60, "次数序号:");          // 框y=72,高26,垂直居中约90
      char tmp1[20];
      sprintf(tmp1, "%d", Data_list1.Menu_rank2.xuhao_num);
      u8g2_DrawUTF8(&u8g2, 250, 60, tmp1);

      u8g2_DrawUTF8(&u8g2, 110, 84, "本机地址:");         // 下框y=104,高26,垂直居中约122
      if(Data_list1.Menu_rank2.now_host_addr==121)
      {
          u8g2_DrawUTF8(&u8g2, 250, 84, "中继");
      }
      else{
          char tmp2[20];
          sprintf(tmp2, "%d", Data_list1.Menu_rank2.now_host_addr);
          u8g2_DrawUTF8(&u8g2, 250, 84, tmp2);
      }

      u8g2_DrawUTF8(&u8g2, 110, 110, "测试地址:");
      if(Data_list1.Menu_rank2.text_host_addr==121)
      {
          u8g2_DrawUTF8(&u8g2, 250, 110, "中继");
      }
      else if(Data_list1.Menu_rank2.text_host_addr==122)
      {
          u8g2_DrawUTF8(&u8g2, 250, 110, "分站");
      }
      else
      {
          char tmp3[20];
          sprintf(tmp3, "%d", Data_list1.Menu_rank2.text_host_addr);
          u8g2_DrawUTF8(&u8g2, 250, 110, tmp3);
      }

      u8g2_DrawUTF8(&u8g2, 110, 134, "测试次数:");         // 下框y=104,高26,垂直居中约122
      char tmp4[20];
      sprintf(tmp4, "%d", Data_list1.Menu_rank2.text_cnt);
      u8g2_DrawUTF8(&u8g2, 250, 134, tmp4);

      u8g2_DrawUTF8(&u8g2, 110, 158, "成功比率:");
      char tmp5[20];
      sprintf(tmp5, "%d %%", Data_list1.Menu_rank2.bl_numl);
      u8g2_DrawUTF8(&u8g2, 250, 158, tmp5);

}
void networking_Control(void)
{
   if( Data_list1.Menu_rank2.net_flag==0)
   {
       networking_Control1();//组网选择
   }
   else{
       networking_Control2();//组网信息显示
   }
}

void calibration_pass_Control(void)
{
   if( Data_list1.Menu_rank3.pass_flag)
   {

       password_Control(); //密码

   }
   else
   {
       calibration_Control();  //标定值
   }
}
void calibration_Control()
{
    uint8_t menu_rank=0;
         uint8_t rank3_addr=0;
          menu_rank = Data_list1.menu_rank;
          rank3_addr = Data_list1.rank3_addr;
       OptionItem laber[10]=
       {
             {120, 90, 24,0,4,4},
             {120, 120, 24,0,4,4},
             {120, 156, 24,0,4,4},
             {250, 156, 24,0,4,4},
  //           {120, 156, 24,0,4,4},
  //           {250, 156, 24,0,4,4}
       };
    u8g2_SetFont(&u8g2, u8g2_font24_lunar);

     u8g2_DrawUTF8(&u8g2, 120, 60, "标定点:");          // 框y=72,高26,垂直居中约90
     u8g2_DrawUTF8(&u8g2, 120, 90,  "0   mm");
     laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"0   mm");

     if(Data_list1.Menu_rank3.biaoding_flag[0]==1)//动态数据
     {
         char tmp1[20];
         sprintf(tmp1, "%d", Data_list1.Menu_rank3.biaoding_ad[0][0]);
         u8g2_DrawUTF8(&u8g2, 200, 90, tmp1);

         char tmp2[20];
         sprintf(tmp2, "%d", Data_list1.Menu_rank3.biaoding_ad[0][1]);
         u8g2_DrawUTF8(&u8g2, 280, 90, tmp2);

         u8g2_DrawUTF8(&u8g2, 350, 90,  "OK");
     }

     u8g2_DrawUTF8(&u8g2, 120, 120, "200 mm");         // 下框y=104,高26,垂直居中约122
     laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"200 mm");

     if(Data_list1.Menu_rank3.biaoding_flag[1]==1) //动态数据
     {

          char tmp3[20];
          sprintf(tmp3, "%d", Data_list1.Menu_rank3.biaoding_ad[1][0]);
          u8g2_DrawUTF8(&u8g2, 200, 120, tmp3);

          char tmp4[20];
          sprintf(tmp4, "%d", Data_list1.Menu_rank3.biaoding_ad[1][1]);
          u8g2_DrawUTF8(&u8g2, 280, 120, tmp4);



          u8g2_DrawUTF8(&u8g2, 350, 120,  "OK");
     }


     u8g2_DrawUTF8(&u8g2, 120, 156, "保存并重启");         // 下框y=104,高26,垂直居中约122
     laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"保存并重启");

     u8g2_DrawUTF8(&u8g2, 250, 156, "不保存返回");
     laber[3].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"不保存返回");

     if (menu_rank == 3)   // 确保索引有效
     {
         if(rank3_addr<4)
         {
            // u8g2_SetFont(&u8g2, u8g2_font16_lunar);
             // 获取菜单名称的像素宽度
             uint16_t name_width =laber[rank3_addr].label_wight;
                               // 定义框的边距（与文字保持间距）
                               uint16_t padding = laber[rank3_addr].kuang_wight;
                               // 计算框的坐标（根据字体基线调整）
                               // u8g2 的 y 坐标是基线，假设字体高度约为 16，ascent 约 14，descent 约 2
                               // 框的左上角 y = posy - 14 - 2（适当微调）
                               uint16_t x = laber[rank3_addr].posx - padding;
                               uint16_t y = laber[rank3_addr].posy -laber[rank3_addr].font_size ;  // 根据实际字体调整
                               uint16_t w = name_width + padding * 2;
                               uint16_t h =laber[rank3_addr].font_size+ laber[rank3_addr].kuang_high*2;  // 框高度略大于字体高度
                               // 画框（仅边框，不填充）
                               u8g2_DrawFrame(&u8g2, x, y, w, h);
         }
     }
}

void password_Control()
{
    uint8_t menu_rank=0;
         uint8_t rank3_addr=0;
          menu_rank = Data_list1.menu_rank;
          rank3_addr = Data_list1.rank3_addr;
    OptionItem laber[10]=
     {
           {162, 108, 24,0,8,8},
           {208, 108, 24,0,8,8},
           {254, 108, 24,0,8,8},
           {300, 108, 24,0,8,8},
//           {120, 156, 24,0,4,4},
//           {250, 156, 24,0,4,4}
     };
    u8g2_SetFont(&u8g2, u8g2_font24_lunar);

     u8g2_DrawUTF8(&u8g2, 160, 80, "请输入标定密码");          // 框y=72,高26,垂直居中约90


     u8g2_DrawRFrame(&u8g2, 160, 90, 28, 28, 4);
     u8g2_DrawRFrame(&u8g2, 206, 90, 28, 28, 4);
     u8g2_DrawRFrame(&u8g2, 252, 90, 28, 28, 4);
     u8g2_DrawRFrame(&u8g2, 298, 90, 28, 28, 4);

      char tmp1[20];
      sprintf(tmp1, "%d", Data_list1.Menu_rank3.pass_buf[0]%10);
      u8g2_DrawUTF8(&u8g2, 167, 112, tmp1);
      laber[0].label_wight=24;

      char tmp2[20];
      sprintf(tmp2, "%d", Data_list1.Menu_rank3.pass_buf[1]%10);
      u8g2_DrawUTF8(&u8g2, 213, 112, tmp2);
      laber[1].label_wight=24;

      char tmp3[20];
      sprintf(tmp3, "%d", Data_list1.Menu_rank3.pass_buf[2]%10);
      u8g2_DrawUTF8(&u8g2, 259, 112, tmp3);
      laber[2].label_wight=24;

      char tmp4[20];
      sprintf(tmp4, "%d", Data_list1.Menu_rank3.pass_buf[3]%10);
      u8g2_DrawUTF8(&u8g2, 305, 112, tmp4);
      laber[3].label_wight=24;

     if (menu_rank == 3)   // 确保索引有效
     {
         if(rank3_addr<4)
         {
            // u8g2_SetFont(&u8g2, u8g2_font16_lunar);
             // 获取菜单名称的像素宽度
             uint16_t name_width =laber[rank3_addr].label_wight;
                               // 定义框的边距（与文字保持间距）
               uint16_t padding = laber[rank3_addr].kuang_wight;
               // 计算框的坐标（根据字体基线调整）
               // u8g2 的 y 坐标是基线，假设字体高度约为 16，ascent 约 14，descent 约 2
               // 框的左上角 y = posy - 14 - 2（适当微调）
               uint16_t x = laber[rank3_addr].posx - padding;
               uint16_t y = laber[rank3_addr].posy -laber[rank3_addr].font_size ;  // 根据实际字体调整
               uint16_t w = name_width + padding * 2;
               uint16_t h =laber[rank3_addr].font_size+ laber[rank3_addr].kuang_high*2;  // 框高度略大于字体高度
               // 画框（仅边框，不填充）
               u8g2_DrawFrame(&u8g2, x, y, w, h);
         }
     }
}
void new_return(void)
{
    u8g2_SetFont(&u8g2, UI_FONT_CN);

    u8g2_DrawUTF8(&u8g2, 80, 80, "正在重启保存数据");
}
/* ==================================================================
 * 「安装调试」页（rank2_addr == 3，契约 §1.3）—— 按清单重做
 *
 *   rank3_addr：0 = 设备信号（RSSI） / 1 = 设备电压 / 2 = 返回
 *   re_flag   ：3 = 设备信号子页（20 通道 RSSI）
 *               4 = 设备电压子页（20 通道电压）
 *
 *   ★ 契约 §4.2（2026-09-22 冻结）：两个子页由"按 chu_num2 分 2 页、每页 10 条"
 *     改为**单页全屏双列**：左列通道 1..10、右列 11..20，一屏 20 条，不再分页
 *     （K1/K3 翻页对它们无效；chu_num2 仍由 0x01 帧下发、字段语义不变）。
 *   ★ 契约 §4.3：未绑定 / 不存在的通道画 `序号.-----`（第 3 通道 → 3.-----）；
 *     已绑定但本轮无数据的通道照旧显示上次值。
 *   ★ 契约 §4.4：纯显示页画「返回」（标题带右侧、固定高亮）；K2 落在返回项上
 *     ⇒ 回上一级（STM32 侧 sub2_close() 执行）。
 *   版式常量/行距说明见上方 PARAM_* 段落（列几何复用主页面，行距 13px）。
 *
 *   数据源：CH584M 本地 CH_com_buf[i].rssi / .voltage（observer.h:114-115），
 *   不是 STM32 下发的 shishi_buf（旧实现在这一点上是错的，已重做）。
 * ================================================================== */
static void install_ch_page(uint8_t kind, uint16_t page)
{
    char     buf[32];
    uint8_t  i, row, col;
    uint16_t cx, y, w;
    device_t *d;

    (void)page;                              /* 契约 §4.2：不再分页（入参保留以便审计页面码） */

    u8g2_SetFont(&u8g2, UI_FONT_CN);

    /* 子页标识（不再有 "1-10/11-20" 页码） */
    ui_draw(PARAM_LABEL_X, PARAM_TITLE_BASE, (kind == 3) ? "信号" : "电压");

    /* 「返回」（契约 §4.4）：固定高亮 = 本页唯一可选项 */
    ui_draw(PARAM_BACK_X, PARAM_TITLE_BASE, "返回");
    ui_hl_box(PARAM_BACK_X, PARAM_TITLE_BASE, PARAM_BACK_W, 1);

    /* 双列分隔竖线：左列 1..10 / 右列 11..20 */
    u8g2_DrawVLine(&u8g2, MAIN_COL_SPLIT_X, PARAM_VLINE_TOP, PARAM_VLINE_LEN);

    for (i = 0; i < 20; i++)
    {
        row = (uint8_t)(i % PARAM_ROWS);              /* 0..9 */
        col = (uint8_t)(i / PARAM_ROWS);              /* 0 = 左列 1..10，1 = 右列 11..20 */
        cx  = (uint16_t)(col ? MAIN_CELL_R_X : MAIN_CELL_L_X);
        y   = PARAM_ROW_BASE(row);
        d   = &CH_com_buf[i];

        /* ★ 契约 §4.3：未绑定 / 不存在的通道画成 `序号.-----`（序号不补零） */
        if (!d->valid)
        {
            sprintf(buf, "%d.-----", i + 1);
            ui_draw(cx, y, buf);
            continue;
        }

        sprintf(buf, "%02d", i + 1);                  /* 已绑定行：编号对齐 2 位 */
        ui_draw(cx, y, buf);

        if (kind == 3)
        {
            /* rssi 在 observer.c 里是 (uint8_t)Rssi（Rssi 为负 dBm）⇒ 负值被截断成
             * 191..255，这里按有符号还原后再显示（例如 0xBF → -65 dBm）。 */
            sprintf(buf, "%d dBm", (int)(int8_t)d->rssi);
        }
        else
        {
            /* ★ 契约_传感器数据解析与显示标度 §4.3（2026-09-22）：电压字节也带一位小数，
             *   规格给出 0x23 = 35 ⇒ **3.5V**，故显示 = 原始值/10 且保留 1 位小数。
             *   （改前按原始值显示 35，无法反映 3.5V。） */
            sprintf(buf, "%d.%d V", d->voltage / 10, d->voltage % 10);
        }

        /* 值+单位右对齐到主页面同一条数值右边界（复用 MAIN_VAL_RIGHT） */
        w = (uint16_t)u8g2_GetUTF8Width(&u8g2, buf);
        ui_draw((uint16_t)(cx + MAIN_VAL_RIGHT - w), y, buf);
    }
}

void install_Control()
{
    uint8_t re    = Data_list1.UI_main.re_flag;
    uint8_t rank3 = Data_list1.rank3_addr;

    if (re == 3) { install_ch_page(3, Data_list1.UI_main.chu_num2); return; }   /* 20 通道 RSSI */
    if (re == 4) { install_ch_page(4, Data_list1.UI_main.chu_num2); return; }   /* 20 通道电压 */

    /* ---- 主列表（3 项） ---- */
    u8g2_SetFont(&u8g2, UI_FONT_CN);

    ui_draw(110, 60, "设备信号");
    ui_hl_box(110, 60, 56, (uint8_t)(rank3 == 0));

    ui_draw(110, 88, "设备电压");
    ui_hl_box(110, 88, 56, (uint8_t)(rank3 == 1));

    ui_draw(110, 116, "返回");
    ui_hl_box(110, 116, 28, (uint8_t)(rank3 == 2));
}
void uploading_Control()
{

    uint8_t menu_rank=0;
         uint8_t rank3_addr=0;
          menu_rank = Data_list1.menu_rank;
          rank3_addr = Data_list1.rank3_addr;
    OptionItem laber[10]=
     {
           {300, 60, 14,0,4,4},
           {300, 86, 14,0,4,4},
           {300, 112, 14,0,4,4},

           {120, 156, 14,0,4,4},
           {250, 156, 14,0,4,4},
//           {120, 156, 24,0,4,4},
//           {250, 156, 24,0,4,4}
     };
        u8g2_SetFont(&u8g2, UI_FONT_CN);


        u8g2_DrawUTF8(&u8g2, 300, 30, "更改为");          // 框y=72,高26,垂直居中约90

        u8g2_DrawUTF8(&u8g2, 110, 60, "上传1:");         // 下框y=104,高26,垂直居中约122

        if(Data_list1.Menu_rank5.old_send_addr[0]==0)
        {
            u8g2_DrawUTF8(&u8g2, 200, 60, "中继");
        }
        else if(Data_list1.Menu_rank5.old_send_addr[0]==121)
        {
            u8g2_DrawUTF8(&u8g2, 200, 60, "无");
        }
        else if(Data_list1.Menu_rank5.old_send_addr[0]==122)
        {
            u8g2_DrawUTF8(&u8g2, 200, 60, "分站");
        }
        else
        {
            char tmp1[20];
            sprintf(tmp1, "%d号", Data_list1.Menu_rank5.old_send_addr[0]);
            u8g2_DrawUTF8(&u8g2, 200, 60, tmp1);
                //  laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp1);

        }

        u8g2_DrawUTF8(&u8g2, 110, 86, "上传2:");         // 下框y=104,高26,垂直居中约122

        if(Data_list1.Menu_rank5.old_send_addr[1]==0)
         {
             u8g2_DrawUTF8(&u8g2, 200, 86, "中继");
         }
        else if(Data_list1.Menu_rank5.old_send_addr[1]==121)
       {
           u8g2_DrawUTF8(&u8g2, 200, 86, "无");
       }
         else if(Data_list1.Menu_rank5.old_send_addr[1]==122)
         {
             u8g2_DrawUTF8(&u8g2, 200, 86, "分站");
         }
         else{
             char tmp2[20];
             sprintf(tmp2, "%d号", Data_list1.Menu_rank5.old_send_addr[1]);
             u8g2_DrawUTF8(&u8g2, 200, 86, tmp2);

         }

        u8g2_DrawUTF8(&u8g2, 110, 112, "上传3:");         // 下框y=104,高26,垂直居中约122
         if(Data_list1.Menu_rank5.old_send_addr[2]==0)
         {
             u8g2_DrawUTF8(&u8g2, 200, 112, "中继");
         }
         else if(Data_list1.Menu_rank5.old_send_addr[2]==121)
         {
              u8g2_DrawUTF8(&u8g2, 200, 112, "无");
         }
         else if(Data_list1.Menu_rank5.old_send_addr[2]==122)
         {
             u8g2_DrawUTF8(&u8g2, 200, 112, "分站");
         }
         else
         {
             char tmp3[20];
             sprintf(tmp3, "%d号", Data_list1.Menu_rank5.old_send_addr[2]);
             u8g2_DrawUTF8(&u8g2, 200, 112, tmp3);
         }

          if(Data_list1.Menu_rank5.new_send_addr[0]==0)
          {
              u8g2_DrawUTF8(&u8g2, 300, 60, "中继");
              laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,"中继");
          }
          else if(Data_list1.Menu_rank5.new_send_addr[0]==122)
          {
               u8g2_DrawUTF8(&u8g2, 300, 60, "分站");
               laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,"分站");
          }
          else if(Data_list1.Menu_rank5.new_send_addr[0]==121)
           {
                u8g2_DrawUTF8(&u8g2, 300, 60, "无");
                laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,"无");
           }
          else
          {
               char tmp4[20];
               sprintf(tmp4, "%d号", Data_list1.Menu_rank5.new_send_addr[0]);
               u8g2_DrawUTF8(&u8g2, 300, 60, tmp4);
               laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp4);
          }

          if(Data_list1.Menu_rank5.new_send_addr[1]==0)
           {
               u8g2_DrawUTF8(&u8g2, 300, 86, "中继");
               laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,"中继");
           }
           else if(Data_list1.Menu_rank5.new_send_addr[1]==122)
           {
               u8g2_DrawUTF8(&u8g2, 300, 86, "分站");
               laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,"分站");
           }
           else if(Data_list1.Menu_rank5.new_send_addr[1]==121)
            {
                u8g2_DrawUTF8(&u8g2, 300, 86, "无");
                laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,"无");
            }
           else{
               char tmp5[20];

               sprintf(tmp5, "%d号", Data_list1.Menu_rank5.new_send_addr[1]);
               u8g2_DrawUTF8(&u8g2, 300, 86, tmp5);
               laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp5);
           }


          if(Data_list1.Menu_rank5.new_send_addr[2]==0)
           {
               u8g2_DrawUTF8(&u8g2, 300, 112, "中继");
               laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,"中继");
           }
           else if(Data_list1.Menu_rank5.new_send_addr[2]==122)
           {
               u8g2_DrawUTF8(&u8g2, 300, 112, "分站");
               laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,"分站");
           }
           else if(Data_list1.Menu_rank5.new_send_addr[2]==121)
           {
             u8g2_DrawUTF8(&u8g2, 300, 112, "无");
             laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,"无");
           }
           else
           {
               char tmp6[20];

               sprintf(tmp6, "%d号", Data_list1.Menu_rank5.new_send_addr[2]);
               u8g2_DrawUTF8(&u8g2, 300, 112, tmp6);
               laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp6);
           }

      //   u8g2_DrawUTF8(&u8g2, 250, 120, "1200 mm");


         // u8g2_DrawUTF8(&u8g2, 250, 120, "8");

        u8g2_SetFont(&u8g2, UI_FONT_CN);
        u8g2_DrawUTF8(&u8g2, 120, 156, "保存并重启");         // 下框y=104,高26,垂直居中约122
        laber[3].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"保存并重启");
        u8g2_DrawUTF8(&u8g2, 250, 156, "不保存返回");
        laber[4].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"不保存返回");

         if (menu_rank == 3)   // 确保索引有效
          {
              if(rank3_addr<5)
              {
                 // u8g2_SetFont(&u8g2, u8g2_font16_lunar);
                  // 获取菜单名称的像素宽度
                  uint16_t name_width =laber[rank3_addr].label_wight;
                  // 定义框的边距（与文字保持间距）
                  uint16_t padding = laber[rank3_addr].kuang_wight;
                  // 计算框的坐标（根据字体基线调整）
                  // u8g2 的 y 坐标是基线，假设字体高度约为 16，ascent 约 14，descent 约 2
                  // 框的左上角 y = posy - 14 - 2（适当微调）
                  uint16_t x = laber[rank3_addr].posx - padding;
                  uint16_t y = laber[rank3_addr].posy -laber[rank3_addr].font_size ;  // 根据实际字体调整
                  uint16_t w = name_width + padding * 2;
                  uint16_t h =laber[rank3_addr].font_size+ laber[rank3_addr].kuang_high*2;  // 框高度略大于字体高度
                  // 画框（仅边框，不填充）
                  u8g2_DrawFrame(&u8g2, x, y, w, h);
              }
          }

}
void Other_Settings_Control() {

     uint8_t menu_rank=0;
     uint8_t rank3_addr=0;
     menu_rank = Data_list1.menu_rank;
     rank3_addr = Data_list1.rank3_addr;
     OptionItem laber[10]=
     {
           {110, 70, 14,0,4,4},
           {250, 70, 14,0,4,4},

           {110, 98, 14,0,4,4},
           {250, 98, 14,0,4,4},

           {110, 126, 14,0,4,4},
           {250, 126, 14,0,4,4},

           {110, 154, 14,0,4,4},
           {250, 154, 14,0,4,4}
     };

      u8g2_SetFont(&u8g2, UI_FONT_CN);
      u8g2_DrawUTF8(&u8g2, 110, 70, "通信功率:");          // 框y=72,高26,垂直居中约90
      laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"通信功率:");

      char tmp1[20];
      sprintf(tmp1, "%d db", Data_list1.Menu_rank6.power);
      u8g2_DrawUTF8(&u8g2, 250, 70, tmp1);
      laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp1);

    //  u8g2_DrawUTF8(&u8g2, 250, 70, "-10 db");
      u8g2_DrawUTF8(&u8g2, 110, 98, "亮屏时间:");         // 下框y=104,高26,垂直居中约122
      laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"亮屏时间:");

      char tmp2[20];
      sprintf(tmp2, "%d 秒", Data_list1.Menu_rank6.time_light);
      u8g2_DrawUTF8(&u8g2, 250, 98, tmp2);
      laber[3].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp2);

    //  u8g2_DrawUTF8(&u8g2, 250, 98, "30 秒");
      u8g2_DrawUTF8(&u8g2, 110, 126, "通信状态:");         // 下框y=104,高26,垂直居中约122
      laber[4].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"通信状态:");

      if(Data_list1.Menu_rank6.state==1)   /* ★ 契约 §6.5：1 = 开机/工作（与 STM32 On_Off_flag 一致；
                                            *   原为 ==0 画"开机"，语义相反，已修正） */
      {
//      char tmp3[20];
//      sprintf(tmp3, "%d mm", Data_list1.Menu_rank6.state);
      u8g2_DrawUTF8(&u8g2, 250, 126, "开机");
      laber[5].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"开机");
      }
      else{
//          char tmp3[20];
//                sprintf(tmp3, "%d mm", Data_list1.Menu_rank6.state);
        u8g2_DrawUTF8(&u8g2, 250, 126, "不开机");
        laber[5].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"不开机");
      }
     // u8g2_DrawUTF8(&u8g2, 250, 126, "开机");         // 下框y=104,高26,垂直居中约122
      u8g2_DrawUTF8(&u8g2, 110, 154, "保存并重启");         // 下框y=104,高26,垂直居中约122
          laber[6].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"保存并重启");

          u8g2_DrawUTF8(&u8g2, 250, 154, "不保存返回");
          laber[7].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"不保存返回");

//      u8g2_DrawUTF8(&u8g2, 110, 154, "返回菜单");
//      laber[6].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"返回菜单");

      if (menu_rank == 3)   // 确保索引有效
       {
           if(rank3_addr<8)
           {
              // u8g2_SetFont(&u8g2, u8g2_font16_lunar);
               // 获取菜单名称的像素宽度
               uint16_t name_width =laber[rank3_addr].label_wight;
                                 // 定义框的边距（与文字保持间距）
                                 uint16_t padding = laber[rank3_addr].kuang_wight;
                                 // 计算框的坐标（根据字体基线调整）
                                 // u8g2 的 y 坐标是基线，假设字体高度约为 16，ascent 约 14，descent 约 2
                                 // 框的左上角 y = posy - 14 - 2（适当微调）
                                 uint16_t x = laber[rank3_addr].posx - padding;
                                 uint16_t y = laber[rank3_addr].posy -laber[rank3_addr].font_size ;  // 根据实际字体调整
                                 uint16_t w = name_width + padding * 2;
                                 uint16_t h =laber[rank3_addr].font_size+ laber[rank3_addr].kuang_high*2;  // 框高度略大于字体高度
                                 // 画框（仅边框，不填充）
                                 u8g2_DrawFrame(&u8g2, x, y, w, h);
           }
       }
}

void zero_setting_Control() {

    uint8_t menu_rank=0;
         uint8_t rank3_addr=0;
          menu_rank = Data_list1.menu_rank;
          rank3_addr = Data_list1.rank3_addr;
    OptionItem laber[10]=
     {
           {120, 120, 16,0,4,4},
           {250, 120, 16,0,4,4},
           {120, 156, 24,0,4,4},
           {250, 156, 24,0,4,4},
//           {120, 156, 24,0,4,4},
//           {250, 156, 24,0,4,4}
     };
       u8g2_SetFont(&u8g2, u8g2_font16_lunar);
       u8g2_DrawUTF8(&u8g2, 120, 60, "通道一:");          // 框y=72,高26,垂直居中约90
      // u8g2_DrawUTF8(&u8g2, 250, 60, "1200 mm");

       char tmp1[20];
       sprintf(tmp1, "%d mm", Data_list1.Menu_rank7.len_value[0]/10);
       u8g2_DrawUTF8(&u8g2, 220, 60, tmp1);

       char tmp2[20];
       sprintf(tmp2, "%d", Data_list1.Menu_rank7.ad_value[0]);
       u8g2_DrawUTF8(&u8g2, 320, 60, tmp2);
        //  laber[3].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp2);

       u8g2_DrawUTF8(&u8g2, 120, 80, "通道二:");         // 下框y=104,高26,垂直居中约122
       char tmp3[20];
       sprintf(tmp3, "%d mm", Data_list1.Menu_rank7.len_value[1]/10);
       u8g2_DrawUTF8(&u8g2, 220, 80, tmp3);

       char tmp4[20];
       sprintf(tmp4, "%d", Data_list1.Menu_rank7.ad_value[1]);
       u8g2_DrawUTF8(&u8g2, 320, 80, tmp4);
          //   laber[3].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp2);



     //  u8g2_DrawUTF8(&u8g2, 250, 80, "1200 mm");

       u8g2_DrawUTF8(&u8g2, 120, 100, "已保存置零点为:");         // 下框y=104,高26,垂直居中约122
      // u8g2_DrawUTF8(&u8g2, 250, 100, "7.8 mm");
       char tmp5[20];
       sprintf(tmp5, "%d.%d mm", Data_list1.Menu_rank7.old_len/10,Data_list1.Menu_rank7.old_len%10);
       u8g2_DrawUTF8(&u8g2, 250, 100, tmp5);

       u8g2_DrawUTF8(&u8g2, 120, 120, "置零点为:");         // 下框y=104,高26,垂直居中约122
       laber[0].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"置零点为:");
       //u8g2_DrawUTF8(&u8g2, 250, 120, "6.6 mm");

       char tmp6[20];
       sprintf(tmp6, "%d.%d mm", Data_list1.Menu_rank7.new_len/10,Data_list1.Menu_rank7.new_len%10);
       u8g2_DrawUTF8(&u8g2, 250, 120, tmp6);
       laber[1].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)tmp6);

       u8g2_SetFont(&u8g2, u8g2_font24_lunar);
       u8g2_DrawUTF8(&u8g2, 120, 156, "保存并重启");         // 下框y=104,高26,垂直居中约122
       laber[2].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"保存并重启");

       u8g2_DrawUTF8(&u8g2, 250, 156, "不保存返回");
       laber[3].label_wight=u8g2_GetUTF8Width(&u8g2,(const char*)"不保存返回");

      if (menu_rank == 3)   // 确保索引有效
      {
          if(rank3_addr<4)
          {
             // u8g2_SetFont(&u8g2, u8g2_font16_lunar);
              // 获取菜单名称的像素宽度
              uint16_t name_width =laber[rank3_addr].label_wight;
                                // 定义框的边距（与文字保持间距）
                                uint16_t padding = laber[rank3_addr].kuang_wight;
                                // 计算框的坐标（根据字体基线调整）
                                // u8g2 的 y 坐标是基线，假设字体高度约为 16，ascent 约 14，descent 约 2
                                // 框的左上角 y = posy - 14 - 2（适当微调）
                                uint16_t x = laber[rank3_addr].posx - padding;
                                uint16_t y = laber[rank3_addr].posy -laber[rank3_addr].font_size ;  // 根据实际字体调整
                                uint16_t w = name_width + padding * 2;
                                uint16_t h =laber[rank3_addr].font_size+ laber[rank3_addr].kuang_high*2;  // 框高度略大于字体高度
                                // 画框（仅边框，不填充）
                                u8g2_DrawFrame(&u8g2, x, y, w, h);
          }
      }
}


/* 菜单第 7 项「返回主页」（rank2_addr == 7，契约 §1 第 43 行 / §6）
 *
 * ★ CH584M 是纯渲染端，**不做任何本地页面跳转**：menu_rank / rank2_addr /
 *   rank3_addr 只能由 STM32 的 0x01 帧写入（Usart3_task.c:479-483），
 *   本函数**绝不**修改这些字段，只画对应的提示。
 *   实际"返回主页"由 STM32 按键（K2 确认）后下发 menu_rank=1 的 0x01 帧完成。 */
void return_main_Control()
{
    u8g2_SetFont(&u8g2, UI_FONT_CN);
    ui_draw(129, 92, "请按确认键返回主页");
}








void u8g2Init(u8g2_t *u8g2)
{
    TFT_IO_init();

    u8g2_Setup_st7305_yuying_168x384_f(u8g2, U8G2_R3,u8x8_byte_CH584M_spi, u8x8_CH584M_gpio_and_delay);

    u8g2_InitDisplay(u8g2);         //̬
    u8g2_SetPowerSave(u8g2, 0);    //
    u8g2_ClearBuffer(u8g2);
    u8g2_SendBuffer(u8g2);
}

void u8g2Init1(u8g2_t *u8g2)
{
    TFT_IO_init();

    u8g2_Setup_st7306_300x400_f(u8g2, U8G2_R3,u8x8_byte_CH584M_spi, u8x8_CH584M_gpio_and_delay);

    u8g2_InitDisplay(u8g2);         //̬
    u8g2_SetPowerSave(u8g2, 0);    //

    u8g2_ClearBuffer(u8g2);
    u8g2_SendBuffer(u8g2);
}

__HIGH_CODE
uint8_t u8x8_CH584M_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,U8X8_UNUSED void *arg_ptr)
{
  switch(msg)
  {
   case U8X8_MSG_GPIO_AND_DELAY_INIT:

      break;
    case U8X8_MSG_DELAY_MILLI:                          // delay arg_int * 1 milli second
        DelayMs(arg_int);
        break;
    case U8X8_MSG_DELAY_10MICRO:
        DelayUs(arg_int * 10);
        break;

        case U8X8_MSG_GPIO_CS:
              if(arg_int)                                     // arg_int=1: Input dir with pullup high for I2C clock pin
              {
                  CS_HIGH;
              }
              else
              {
                  CS_LOW;
              }

                break;
        case U8X8_MSG_GPIO_DC:
              if(arg_int)                                     // arg_int=1: Input dir with pullup high for I2C clock pin

              {
                  DC_DATA;
              }
               else
               {
                   DC_CMD;
               }

                break;
        case U8X8_MSG_GPIO_RESET:
                break;


    default:

        break;
  }
  return 1;
}
__HIGH_CODE
uint8_t u8x8_byte_CH584M_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
      SPI0_MasterDMATrans((uint8_t *) arg_ptr, arg_int);
  //  HAL_SPI_Transmit(&hspi1, (uint8_t *) arg_ptr, arg_int, 10000);
    break;
  case U8X8_MSG_BYTE_INIT:
    break;
  case U8X8_MSG_BYTE_SET_DC:
      if(arg_int)
      {
          DC_DATA;

      }
      else
      {
          DC_CMD;

      }

    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
          u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
          u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
          break;
  case U8X8_MSG_BYTE_END_TRANSFER:
          u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
          u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
          break;
    break;
  default:
    return 0;
  }
  return 1;
}
void TFT_IO_init(void)
{
    GPIOA_SetBits(GPIO_Pin_12);
    GPIOA_ModeCfg(GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, GPIO_ModeOut_PP_5mA);
    SPI0_MasterDefInit();

    GPIOA_SetBits(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
    GPIOA_ModeCfg(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3, GPIO_ModeOut_PP_5mA);

    GPIOB_ResetBits(GPIO_Pin_0|GPIO_Pin_1);
    GPIOB_ModeCfg(GPIO_Pin_0|GPIO_Pin_1, GPIO_ModeOut_PP_5mA);
}
//void UI_Main_Display(void)
//{
//    // -------- 全局设置 --------
//    u8g2_SetDrawColor(&u8g2, 1);          // 前景色（黑色）
//    u8g2_SetFontMode(&u8g2, 1);           // 透明模式（文字不覆盖背景）
//
//    // -------- 1. 主外框（双层） --------
//    u8g2_DrawRFrame(&u8g2, 4, 4, 376, 160, 6);   // 外框
//    u8g2_DrawRFrame(&u8g2, 6, 6, 372, 156, 4);   // 内框（增加层次感）
//
//    u8g2_SetFont(&u8g2, u8g2_font32_lunar);
//    u8g2_DrawLine(&u8g2, 118, 40, 90, 6);                 // 分隔线
//    u8g2_DrawLine(&u8g2, 266, 40, 294, 6);                 // 分隔线
//    u8g2_DrawLine(&u8g2, 118, 40, 266, 40);                 // 分隔线
//
//    u8g2_DrawLine(&u8g2, 114, 42, 86, 10);                 // 分隔线
//    u8g2_DrawLine(&u8g2, 270, 42, 298, 10);                 // 分隔线
//   u8g2_DrawLine(&u8g2, 328, 10, 298, 10);                 // 分隔线
//
//   u8g2_DrawLine(&u8g2, 12, 40, 12, 120);                 // 分隔线
//   u8g2_DrawLine(&u8g2, 12, 40, 30, 30);                 // 分隔线
//   u8g2_DrawLine(&u8g2, 30, 30, 76, 30);                 // 分隔线
//   // -------- 3. 标题（居中） --------
//      u8g2_SetFont(&u8g2, u8g2_font32_lunar);
//      const char* title = "位移系统";
//      uint8_t tw = u8g2_GetUTF8Width(&u8g2, title);
//      u8g2_DrawUTF8(&u8g2, (384 - tw) / 2, 36, title);       // y=60
//   // -------- 2. 顶部状态栏 --------
//       u8g2_SetFont(&u8g2, u8g2_font16_lunar);
//       u8g2_DrawUTF8(&u8g2, 12, 22, "三为矿安");                // 左logo
//
//       u8g2_SetFont(&u8g2, u8g2_font_helvB08_tf);      // 8像素英文
//       u8g2_DrawUTF8(&u8g2, 340, 40, "V1.0.1");        // 左下
//       u8g2_DrawUTF8(&u8g2, 340, 20, "3.3V");         // 右下
//
//       // -------- 5. 数据信息（两行） --------
//       u8g2_DrawRFrame(&u8g2, 30, 56, 246, 28, 3);
//        // 下框：监测点
//        u8g2_DrawRFrame(&u8g2, 30, 92, 246, 28, 3);
//
//       // 填充数据（标签+数值同一行）
//       u8g2_SetFont(&u8g2, u8g2_font24_lunar);
//
//       u8g2_DrawUTF8(&u8g2, 32, 76, "浅基点:");          // 框y=72,高26,垂直居中约90
//       u8g2_DrawUTF8(&u8g2, 120, 76, "1200.0 mm");
//       u8g2_DrawUTF8(&u8g2, 32, 112, "深基点:");         // 下框y=104,高26,垂直居中约122
//             u8g2_DrawUTF8(&u8g2, 120, 112, "1200.0 mm");
//   // -------- 5. 底部状态信息（两行） --------
//  //  u8g2_DrawLine(&u8g2, 12, 134, 372, 134);        // 分割线
//
//    u8g2_DrawLine(&u8g2, 118, 158, 90, 134);                 // 分隔线
//    u8g2_DrawLine(&u8g2, 266, 158, 294, 134);                 // 分隔线
//    u8g2_DrawLine(&u8g2, 118, 158, 266, 158);                 // 分隔线
//
//    u8g2_DrawLine(&u8g2, 90, 134, 12, 134);                 // 分隔线
//    u8g2_DrawLine(&u8g2, 294, 134, 372, 134);                 // 分隔线
//
//    u8g2_SetFont(&u8g2, u8g2_font16_lunar);
//    u8g2_DrawUTF8(&u8g2, 20, 150, "开机");                // 左logo
//     // u8g2_SetFont(&u8g2, u8g2_font16_lunar);
//    u8g2_DrawUTF8(&u8g2, 290, 130, "分站:64号");                // 左logo
//    u8g2_DrawUTF8(&u8g2, 290, 150, "发往:118号");                // 左logo
//
//
//   const char* lb1 = "主机号:";
//   const char* val1 = "120";
//   uint8_t lw1 = u8g2_GetUTF8Width(&u8g2, lb1);
//   uint8_t vw1 = u8g2_GetUTF8Width(&u8g2, val1);
//   int total1 = lw1 + 10 + vw1;                    // 10为间距
//   int start1 = 60 + (266 - total1) / 2;           // 框内居中
//   u8g2_DrawUTF8(&u8g2, start1, 150, lb1);          // 框y=72,高26,垂直居中约90
//   u8g2_DrawUTF8(&u8g2, start1 + lw1 + 10, 150, val1);
//}



//void UI_Menu_Display(void) {
//
//  u8g2_SetFont(&u8g2, u8g2_font16_lunar);
//  for (uint8_t i = 0; i < 8; i++)
//  {
//    u8g2_DrawUTF8(&u8g2,Main_List[i].posx, Main_List[i].posy, Main_List[i].name);
//  }
//
//  u8g2_DrawRFrame(&u8g2, 4, 4, 94, 160, 6);   // 外框
//  u8g2_DrawRFrame(&u8g2, 6, 6, 90, 156, 4);   // 内框（增加层次感）
//
//  u8g2_DrawRFrame(&u8g2, 102, 4, 278, 160, 6);   // 内框（增加层次感）
//
//  u8g2_SetFont(&u8g2, u8g2_font24_lunar);
//  u8g2_DrawUTF8(&u8g2,Menu_List[UI_Select].posx, Menu_List[UI_Select].posy, Menu_List[UI_Select].name);
//
//
//  u8g2_DrawLine(&u8g2, 104, 34, 210, 34);
//  Menu_List[UI_Select].function();
//
//  //------画装饰小条------//
////  u8g2_SetDrawColor(&u8g2,1);
////  u8g2_DrawBox(&u8g2,120, 0, 6, 2);
////  u8g2_DrawBox(&u8g2,120, 62, 6, 2);
////  u8g2_DrawBox(&u8g2,122, 2, 2, 60);
////  u8g2_DrawRBox(&u8g2,120, frame_y + 3, 6, 8, 2);
////   u8g2_SetDrawColor(&u8g2,2);                                                                      //选择反色
////    frame_lenth_trg = u8g2_GetUTF8Width(&u8g2,Main_List[UI_Select].name) + 2;                        //菜单每个条目的长度
////    frame_y_trg = Main_List[UI_Select].posy - POS_Y;                                            //选择框的目标值随着选择数值移动
////    UI_MoveSet(&frame_y, &frame_y_trg, MENU_HEIGHT / 4, MENU_HEIGHT / 4 + 1);                    //移动菜单选中框
////    UI_MoveSet(&frame_lenth, &frame_lenth_trg, frame_lenth_trg / 4, frame_lenth_trg / 4 + 1);  //调整选中框长度
////    u8g2_DrawRBox(&u8g2,1, frame_y, frame_lenth, MENU_HEIGHT, 3);                                     //画空心方框坐标x，y，宽度frame_lenth,高度MENU_HEIGHT
//
//}
//void draw(u8g2_t *u8g2)
//{
//
////                u8g2_DrawStr(&u8g2, 0, 15, "Hello World!");
////                u8g2_DrawCircle(&u8g2, 64, 40, 10, U8G2_DRAW_ALL);
//                // ˢ����Ļ
//
//    u8g2_SetFontMode(u8g2, 1);
//    u8g2_SetFontDirection(u8g2, 0);
//    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
//    u8g2_DrawStr(u8g2, 0, 20, "U");
//
//    u8g2_SetFontDirection(u8g2, 1);
//    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
//    u8g2_DrawStr(u8g2, 21,8,"8");
//
//    u8g2_SetFontDirection(u8g2, 0);
//    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
//    u8g2_DrawStr(u8g2, 51,30,"g");
//    u8g2_DrawStr(u8g2, 67,30,"\xb2");
//
//    u8g2_DrawHLine(u8g2, 2, 35, 47);
//    u8g2_DrawHLine(u8g2, 3, 36, 47);
//    u8g2_DrawVLine(u8g2, 45, 32, 12);
//    u8g2_DrawVLine(u8g2, 46, 33, 12);
//
////    u8g2_SetFont(u8g2, u8g2_font_wqy12_t_chinese3);
////    u8g2_DrawUTF8(u8g2, 200, 0, "��ã����磡");
//    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
//    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
//}
//__HIGH_CODE
//void spi_write_test2(uint16_t data)//ͨ�� FIFO��ʽ ��SPI�ӿڷ��� 16 λ����
//{
//    GPIOA_ResetBits(GPIO_Pin_12);     //Ƭѡ�ź�����
//
//    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;   // �������ݷ���Ϊ���
//    while(!(R8_SPI0_INT_FLAG & RB_SPI_FREE));  // �ȴ�FIFO�е�����ȫ���������
//    R16_SPI0_TOTAL_CNT = 2;    //���� SPI0 �����ݴ������ֽ���Ϊ 2����ʾ��Ҫ���� 2 ���ֽڡ�
//    R8_SPI0_FIFO = data>>8;   //�����ݵĸ��ֽ�д�� SPI0 �� FIFO �Ĵ���
//    R8_SPI0_FIFO = data>>0;   //�����ݵĵ��ֽ�д�� SPI0 �� FIFO �Ĵ���
//    while(!(R8_SPI0_INT_FLAG & RB_SPI_FREE));   // �ȴ�FIFO�е�����ȫ���������
//
//    GPIOA_SetBits(GPIO_Pin_12);   //Ƭѡ�ź�����
//}
//__HIGH_CODE
//void spi_write_test(uint16_t data)
//{
//    GPIOA_ResetBits(GPIO_Pin_12);  //Ƭѡ�ź�����
//
//    SPI0_MasterSendByte(data>>8);  //�� SPI0 �ӿڷ������ݵĸ��ֽڡ�ʹ�� >> ���������������� 8 λ���Ի�ȡ�� 8 λ���ݡ�
//    SPI0_MasterSendByte(data>>0);  //�� SPI0 �ӿڷ������ݵĵ��ֽڡ�ʹ�� >> ���������������� 0 λ���Ի�ȡ�� 8 λ���ݡ�
//
//    GPIOA_SetBits(GPIO_Pin_12);  //Ƭѡ�ź�����
//}

