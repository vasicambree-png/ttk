#include "CONFIG.h"
#include "yuying_TFT.h"
//#include "u8g2_fonts.c"
#include "u8g2.h"
#include "stdlib.h"
/* ★ 功能清单：主页面/子页要显示 CH584M 本地数据（CH_com_buf[]、绑定表、
 *   异常计数、扫描名称缓存），这些类型与 extern 声明都在 observer.h 里。
 *   observer.h 自身 include yuying_TFT.h（有 include guard），无循环问题。 */
#include "observer.h"
#include "ui_home_assets.h"
#include "ui_menu_assets.h"
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
 * 主页面（menu_rank == 1）：按单色 384×168 屏幕适配工业仪表版式
 *
 * 版式：双层切角外框；品牌、标题与电池值同排；状态和地址占一行；
 *       左右切角数据栏各 5 行；底部三段统计栏。
 *
 * 网格：**5 行 × 2 列 = 10 格，每格 1 个通道**，右列紧接左列：
 *   第 1 页 = 通道 1..10（左列 1..5，右列 6..10）
 *   第 2 页 = 通道 11..20（左列 11..15，右列 16..20）
 *   分页由 0x01 帧的 chu_num1 决定（== 2 为第 2 页，其余按第 1 页）。
 * 每格一行式：`切角编号 类型名 数值(右对齐) 单位`
 *   数值：MG/位移/应力/倾角 = 原始值/10，保留一位小数（倾角按有符号），激光原样
 *   单位：kN（锚杆）/ mm（位移·激光·裂缝）/ °（倾角）/ MPa（应力）（契约 §4.2）
 *
 * 数据分工：STM32 仍提供全部元数据；主页按参考图显示状态/本机号/分站/电池，
 *           版本和信号字段暂不占用主页版面；
 *           CH584M 本地提供 已绑定数、已用通道数、告警数、20 通道类型与数值。
 * ================================================================== */
/* MAIN_* 几何常量仍供其他20通道子页复用，数值保持原样。 */
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

/* 主页专用坐标，避免影响信号、电压和名称子页。 */
#define HOME_GRID_TOP     46
#define HOME_ROW_H        19
#define HOME_ROWS         5
#define HOME_CELL_L_X     14
#define HOME_CELL_R_X     203
#define HOME_NUM_W        24
#define HOME_DATA_RIGHT   166
#define HOME_ROW_BASE(i)  ((uint8_t)(HOME_GRID_TOP + 14 + HOME_ROW_H * (i)))
#define HOME_HEADER_BATTERY_RIGHT 372u
#define HOME_HEADER_BATTERY_BASE  20u
#define HOME_STATUS_BASELINE      39u
#define HOME_STATUS_LEFT          14u
#define HOME_STATUS_RIGHT         374u
#define HOME_STATUS_MIN_GAP       12u
#define HOME_STATUS_DIVIDER_X     8u
#define HOME_STATUS_DIVIDER_W     369u
#define HOME_STATUS_DIVIDER_Y     44u
#define HOME_STATS_X              6u
#define HOME_STATS_Y              147u
#define HOME_STATS_W              372u
#define HOME_STATS_H              17u
#define HOME_STATS_DIVIDER_Y      150u
#define HOME_STATS_DIVIDER_H      11u
#define HOME_STATS_BASELINE       161u
#define HOME_STATS_DIVIDER_1      130u
#define HOME_STATS_DIVIDER_2      254u
#define HOME_STATS_COL1_X         7u
#define HOME_STATS_COL1_W         123u
#define HOME_STATS_COL2_X         131u
#define HOME_STATS_COL2_W         123u
#define HOME_STATS_COL3_X         255u
#define HOME_STATS_COL3_W         122u

/* 切角横纵跨度相等，保持 45 度直线，供主页各类边框共用。 */
static void ui_main_cut_frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                              uint8_t cut)
{
    uint16_t right = (uint16_t)(x + w - 1u);
    uint16_t bottom = (uint16_t)(y + h - 1u);
    uint8_t cut_y = cut;

    u8g2_DrawLine(&u8g2, (u8g2_uint_t)(x + cut), (u8g2_uint_t)y,
                  (u8g2_uint_t)(right - cut), (u8g2_uint_t)y);
    u8g2_DrawLine(&u8g2, (u8g2_uint_t)(right - cut), (u8g2_uint_t)y,
                  (u8g2_uint_t)right, (u8g2_uint_t)(y + cut_y));
    u8g2_DrawLine(&u8g2, (u8g2_uint_t)right, (u8g2_uint_t)(y + cut_y),
                  (u8g2_uint_t)right, (u8g2_uint_t)(bottom - cut_y));
    u8g2_DrawLine(&u8g2, (u8g2_uint_t)right, (u8g2_uint_t)(bottom - cut_y),
                  (u8g2_uint_t)(right - cut), (u8g2_uint_t)bottom);
    u8g2_DrawLine(&u8g2, (u8g2_uint_t)(right - cut), (u8g2_uint_t)bottom,
                  (u8g2_uint_t)(x + cut), (u8g2_uint_t)bottom);
    u8g2_DrawLine(&u8g2, (u8g2_uint_t)(x + cut), (u8g2_uint_t)bottom,
                  (u8g2_uint_t)x, (u8g2_uint_t)(bottom - cut_y));
    u8g2_DrawLine(&u8g2, (u8g2_uint_t)x, (u8g2_uint_t)(bottom - cut_y),
                  (u8g2_uint_t)x, (u8g2_uint_t)(y + cut_y));
    u8g2_DrawLine(&u8g2, (u8g2_uint_t)x, (u8g2_uint_t)(y + cut_y),
                  (u8g2_uint_t)(x + cut), (u8g2_uint_t)y);
}

/* 品牌只包含参考图的标识；数值与状态不进入静态资源。 */
static void ui_main_draw_brand(void)
{
    u8g2_DrawXBMP(&u8g2, 9u, 5u, UI_HOME_BRAND_WIDTH, UI_HOME_BRAND_HEIGHT,
                  ui_home_brand);
}

/* 斜牌与双斜纹均为 45 度平行直线，按屏幕中心镜像。 */
static void ui_main_draw_title(void)
{
    uint8_t row, stripe, offset;
    uint16_t left;

    for (row = 0u; row < 27u; row++)
    {
        left = (uint16_t)(72u + row);
        u8g2_DrawHLine(&u8g2, left, (uint16_t)(2u + row), 17u);
        u8g2_DrawHLine(&u8g2, (uint16_t)(383u - left - 16u),
                       (uint16_t)(2u + row), 17u);
    }
    u8g2_SetDrawColor(&u8g2, 0);
    for (stripe = 0u; stripe < 2u; stripe++)
        for (offset = 0u; offset < 2u; offset++)
        {
            left = (uint16_t)(79u + stripe * 6u + offset);
            u8g2_DrawLine(&u8g2, left, 4u, (uint16_t)(left + 21u), 25u);
            u8g2_DrawLine(&u8g2, (uint16_t)(383u - left), 4u,
                          (uint16_t)(383u - left - 21u), 25u);
        }
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawHLine(&u8g2, 98u, 28u, 188u);
    u8g2_DrawXBMP(&u8g2, 112u, 6u, UI_HOME_TITLE_WIDTH, UI_HOME_TITLE_HEIGHT,
                  ui_home_title);
}

/* 状态行独立小字库，保持混排基线；不改变菜单共用字体。 */
static uint16_t ui_main_meta_code(const uint8_t **cursor)
{
    const uint8_t *p = *cursor;
    uint16_t code = *p++;
    if ((code & 0xe0u) == 0xc0u && (p[0] & 0xc0u) == 0x80u)
    {
        code = (uint16_t)(((code & 0x1fu) << 6u) | (p[0] & 0x3fu));
        p++;
    }
    else if ((code & 0xf0u) == 0xe0u && (p[0] & 0xc0u) == 0x80u &&
             (p[1] & 0xc0u) == 0x80u)
    {
        code = (uint16_t)(((code & 0x0fu) << 12u) |
                          ((p[0] & 0x3fu) << 6u) | (p[1] & 0x3fu));
        p += 2;
    }
    *cursor = p;
    return code;
}

static const ui_home_glyph_t *ui_main_meta_glyph(uint16_t code)
{
    uint8_t i;
    for (i = 0u; i < sizeof(ui_home_meta_glyphs) / sizeof(ui_home_meta_glyphs[0]); i++)
        if (ui_home_meta_glyphs[i].code == code) return &ui_home_meta_glyphs[i];
    return NULL;
}

static uint16_t ui_main_meta_width(const char *text)
{
    const uint8_t *cursor = (const uint8_t *)text;
    uint16_t width = 0u;
    while (*cursor != '\0')
    {
        const ui_home_glyph_t *glyph = ui_main_meta_glyph(ui_main_meta_code(&cursor));
        width = (uint16_t)(width + (glyph ? glyph->width : 6u));
    }
    return width;
}

static void ui_main_meta_draw(uint16_t x, uint16_t baseline, const char *text)
{
    const uint8_t *cursor = (const uint8_t *)text;
    while (*cursor != '\0')
    {
        const ui_home_glyph_t *glyph = ui_main_meta_glyph(ui_main_meta_code(&cursor));
        if (glyph)
        {
            u8g2_DrawXBMP(&u8g2, x, (uint16_t)(baseline - UI_HOME_META_BASELINE),
                          glyph->width, UI_HOME_META_HEIGHT, glyph->bits);
            x = (uint16_t)(x + glyph->width);
        }
        else
        {
            u8g2_SetFont(&u8g2, u8g2_font_5x8_tr);
            ui_draw(x, baseline, "?");
            u8g2_SetFont(&u8g2, UI_FONT_CN);
            x = (uint16_t)(x + 6u);
        }
    }
}

/* 只增强主页类型标签，名称和类型映射仍使用现有 CH_TYPE_NAME。 */
static void ui_main_draw_type(uint16_t x, uint16_t baseline, const char *text)
{
    uint8_t i;
    for (i = 0u; i < sizeof(ui_home_labels) / sizeof(ui_home_labels[0]); i++)
        if (strcmp(text, ui_home_labels[i].text) == 0)
        {
            u8g2_DrawXBMP(&u8g2, x, (uint16_t)(baseline - 12u),
                          28u, 15u, ui_home_labels[i].bits);
            return;
        }
    ui_draw(x, baseline, text);
}

/* 通道号在徽标中居中，并为两位数与框线保留一像素空白。 */
static void ui_main_channel_badge(uint16_t x, uint16_t baseline, uint8_t channel)
{
    char label[4];
    uint16_t label_width;
    uint16_t top = (uint16_t)(baseline - 12u);

    ui_main_cut_frame(x, top, 20u, 15u, 2u);
    sprintf(label, "%u", (unsigned int)channel);
    u8g2_SetFont(&u8g2, u8g2_font_helvB10_tr);
    label_width = (uint16_t)u8g2_GetUTF8Width(&u8g2, label);
    ui_draw((uint16_t)(x + (20u - label_width) / 2u),
            (uint16_t)(baseline + 1u), label);
    u8g2_SetFont(&u8g2, UI_FONT_CN);
}

void UI_Main_Display(data_LIST *pData)
{
    char     buf[48];
    char     send_str[12];
    uint8_t  i, first, page, row, col, cx, y;
    uint8_t  bitmap_mode = u8g2.bitmap_transparency;
    uint16_t w;

    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetFont(&u8g2, UI_FONT_CN);          /* 中文全字库，见文件顶部说明 */

    /* 分页在状态行与底部统计栏之前确定：第 1 页显示状态/本机号/分站，
     * 第 2 页显示底部统计栏；分页依据仍是 0x01 帧的 chu_num1。 */
    page = (pData->UI_main.chu_num1 == 2u) ? 2u : 1u;

    /* ---------- 双层切角外框、左侧品牌区与中部标题牌 ---------- */
    u8g2_SetBitmapMode(&u8g2, 1);
    ui_main_cut_frame(1u, 1u, 382u, 166u, 10u);
    /* 保持外侧两像素、内侧一像素，切角端点与直边对齐。 */
    ui_main_cut_frame(2u, 2u, 380u, 164u, 9u);
    ui_main_cut_frame(4u, 4u, 376u, 160u, 7u);
    ui_main_draw_brand();
    ui_main_draw_title();

    /* 电池电压仍取主控字段，排版与参考图一致地置于右上角。 */
    sprintf(buf, "%d.%d%dV",
            pData->UI_main.vbat / 100, pData->UI_main.vbat % 100 / 10,
            pData->UI_main.vbat % 100 % 10);
    u8g2_SetFont(&u8g2, u8g2_font_helvB10_tr);
    w = (uint16_t)u8g2_GetUTF8Width(&u8g2, buf);
    ui_draw((uint16_t)(HOME_HEADER_BATTERY_RIGHT - w), HOME_HEADER_BATTERY_BASE, buf);
    u8g2_SetFont(&u8g2, UI_FONT_CN);

    /* ---------- 第 1 页专属：状态、本机号和分站号：只重排显示，不改字段来源 ---------- */
    if (page == 1u)
    {
        if (pData->UI_main.send_host_num == 122u)      sprintf(send_str, "分站");
        else if (pData->UI_main.send_host_num == 121u) sprintf(send_str, "无");
        else if (pData->UI_main.send_host_num == 0u)   sprintf(send_str, "中继");
        else                                           sprintf(send_str, "%d", pData->UI_main.send_host_num);

        {
            uint16_t state_w, machine_w, station_w, state_x, machine_x, station_x;
            uint16_t machine_left, machine_right;
            const char *state = (pData->UI_main.state == 1) ? "开机" : "关机";

            sprintf(buf, "状态：%s", state);
            state_w = ui_main_meta_width(buf);
            sprintf(buf, "本机号：%d-->%s", pData->UI_main.host_num, send_str);
            machine_w = ui_main_meta_width(buf);
            sprintf(buf, "分站：%d", pData->UI_main.sub_num);
            station_w = ui_main_meta_width(buf);
            state_x = HOME_STATUS_LEFT;
            station_x = (uint16_t)(HOME_STATUS_RIGHT - station_w);
            machine_left = (uint16_t)(state_x + state_w + HOME_STATUS_MIN_GAP);
            machine_right = (uint16_t)(station_x - HOME_STATUS_MIN_GAP);
            machine_x = (uint16_t)(machine_left + (machine_right - machine_left - machine_w) / 2u);

            sprintf(buf, "状态：%s", state);
            ui_main_meta_draw(state_x, HOME_STATUS_BASELINE, buf);
            sprintf(buf, "本机号：%d-->%s", pData->UI_main.host_num, send_str);
            ui_main_meta_draw(machine_x, HOME_STATUS_BASELINE, buf);
            sprintf(buf, "分站：%d", pData->UI_main.sub_num);
            ui_main_meta_draw(station_x, HOME_STATUS_BASELINE, buf);
        }

        /* 仅保留数据区上沿的一条分隔线；左右双框本身已清楚区分两列。 */
        u8g2_DrawHLine(&u8g2, HOME_STATUS_DIVIDER_X, HOME_STATUS_DIVIDER_Y,
                       HOME_STATUS_DIVIDER_W);
    }

    /* ---------- 两页共用同一位置的左右数据框 ---------- */
    ui_main_cut_frame(7u, HOME_GRID_TOP, 181u, 98u, 6u);
    ui_main_cut_frame(196u, HOME_GRID_TOP, 181u, 98u, 6u);

    for (i = 1; i < HOME_ROWS; i++)
    {
        uint8_t line_y = (uint8_t)(HOME_GRID_TOP + HOME_ROW_H * i);
        u8g2_DrawHLine(&u8g2, 7u, line_y, 181u);
        u8g2_DrawHLine(&u8g2, 196u, line_y, 181u);
    }
    /* ---------- 通道网格：分页、类型、值和单位处理保持原样 ---------- */
    first = (uint8_t)((page == 1u) ? 0u : 10u);

    for (i = 0; i < 10; i++)
    {
        uint8_t   ch  = (uint8_t)(first + i);
        device_t *d   = &CH_com_buf[ch];

        row = (uint8_t)(i % HOME_ROWS);                /* 0..4 */
        col = (uint8_t)(i / HOME_ROWS);                /* 0 = 左列，1 = 右列 */
        cx  = (uint8_t)(col ? HOME_CELL_R_X : HOME_CELL_L_X);
        y   = HOME_ROW_BASE(row);

        ui_main_channel_badge(cx, y, (uint8_t)(ch + 1u));
        if (d->valid && d->Type > TPYE_NONE && d->Type < TPYE_END)
        {
            uint16_t value_w, unit_w, unit_x;
            const char *unit = CH_TYPE_UNIT[d->Type];
            const uint8_t *unit_font = (d->Type == TPYE_QJ)
                                       ? UI_FONT_CN : u8g2_font_helvB10_tr;
            ui_main_draw_type((uint16_t)(cx + HOME_NUM_W), y, CH_TYPE_NAME[d->Type]);
            /* ★ 契约 §4.1：MG / 位移 / 应力 / 倾角 显示 = 原始值/10（QJ 有符号）；激光原样 */
            ch_disp_format(buf, sizeof(buf), (Sensor_Tpye)d->Type, d->CH_data);
            u8g2_SetFont(&u8g2, unit_font);
            unit_w = (uint16_t)u8g2_GetUTF8Width(&u8g2, unit);
            unit_x = (uint16_t)(cx + HOME_DATA_RIGHT - unit_w);
            u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);
            value_w = (uint16_t)u8g2_GetUTF8Width(&u8g2, buf);
            ui_draw((uint16_t)(unit_x - (unit_w ? 4u : 0u) - value_w), y, buf);
            if (unit_w)
            {
                u8g2_SetFont(&u8g2, unit_font);
                ui_draw(unit_x, y, unit);
            }
            u8g2_SetFont(&u8g2, UI_FONT_CN);
        }
        else
        {
            /* 序号已在切角徽标中显示；横线表示该通道尚无有效数据。 */
            ui_draw((uint16_t)(cx + HOME_NUM_W), y, "-----");
        }
    }

    /* ---------- 第 2 页专属：底部三段统计栏：与参考图一致用冒号和竖线分隔 ---------- */
    if (page == 2u)
    {
        ui_main_cut_frame(HOME_STATS_X, HOME_STATS_Y, HOME_STATS_W, HOME_STATS_H, 4u);
        u8g2_DrawVLine(&u8g2, HOME_STATS_DIVIDER_1, HOME_STATS_DIVIDER_Y,
                       HOME_STATS_DIVIDER_H);
        u8g2_DrawVLine(&u8g2, HOME_STATS_DIVIDER_2, HOME_STATS_DIVIDER_Y,
                       HOME_STATS_DIVIDER_H);

        sprintf(buf, "已绑定: %d", g_binding_count);
        w = (uint16_t)u8g2_GetUTF8Width(&u8g2, buf);
        ui_draw((uint16_t)(HOME_STATS_COL1_X + (HOME_STATS_COL1_W - w) / 2u),
                HOME_STATS_BASELINE, buf);
        sprintf(buf, "已用通道: %d", count_used_channels());
        w = (uint16_t)u8g2_GetUTF8Width(&u8g2, buf);
        ui_draw((uint16_t)(HOME_STATS_COL2_X + (HOME_STATS_COL2_W - w) / 2u),
                HOME_STATS_BASELINE, buf);
        sprintf(buf, "报警: %d", (int)(g_name_err_count + g_volt_err_count));
        w = (uint16_t)u8g2_GetUTF8Width(&u8g2, buf);
        ui_draw((uint16_t)(HOME_STATS_COL3_X + (HOME_STATS_COL3_W - w) / 2u),
                HOME_STATS_BASELINE, buf);
    }
    u8g2_SetBitmapMode(&u8g2, bitmap_mode);
}

/* Menu-only artwork and layout. Runtime fields and selection IDs stay intact. */
static const ui_menu_glyph_t *ui_menu_glyph(uint16_t code, uint8_t size)
{
    const ui_menu_glyph_t *table = (size == 18u) ? ui_menu_glyphs_18 : ui_menu_glyphs_16;
    uint16_t count = (uint16_t)(sizeof(ui_menu_glyphs_16) / sizeof(ui_menu_glyphs_16[0]));
    uint16_t i;
    for (i = 0u; i < count; i++)
        if (table[i].code == code) return &table[i];
    return NULL;
}

static uint16_t ui_menu_text_width(const char *text, uint8_t size)
{
    const uint8_t *cursor = (const uint8_t *)text;
    uint16_t width = 0u;
    while (*cursor != '\0')
    {
        const ui_menu_glyph_t *glyph = ui_menu_glyph(ui_main_meta_code(&cursor), size);
        width = (uint16_t)(width + (glyph ? glyph->width : size));
    }
    return width;
}

static void ui_menu_text(uint16_t x, uint16_t baseline, const char *text, uint8_t size)
{
    const uint8_t *cursor = (const uint8_t *)text;
    while (*cursor != '\0')
    {
        const ui_menu_glyph_t *glyph = ui_menu_glyph(ui_main_meta_code(&cursor), size);
        if (glyph == NULL) glyph = ui_menu_glyph('?', size);
        if (glyph != NULL)
        {
            u8g2_DrawXBMP(&u8g2, x, (uint16_t)(baseline - size + 1u),
                          glyph->width, (uint16_t)(size + 2u), glyph->bits);
            x = (uint16_t)(x + glyph->width);
        }
    }
}

static uint8_t ui_menu_selected(uint8_t option)
{
    return (uint8_t)(Data_list1.menu_rank == 3u && Data_list1.rank3_addr == option);
}

/* A single selected control is inverted; passive field outlines stay thin. */
static void ui_menu_control(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                            const char *text, uint8_t selected, uint8_t centered)
{
    uint16_t tw = ui_menu_text_width(text, 16u);
    uint16_t tx;
    u8g2_SetDrawColor(&u8g2, 0);
    if (selected) u8g2_DrawBox(&u8g2, x, y, w, h);
    else u8g2_DrawFrame(&u8g2, x, y, w, h);
    u8g2_SetDrawColor(&u8g2, selected ? 1u : 0u);
    if (tw <= w - 8u)
    {
        tx = centered ? (uint16_t)(x + (w - tw) / 2u) : (uint16_t)(x + 5u);
        ui_menu_text(tx, (uint16_t)(y + (h - 18u) / 2u + 15u), text, 16u);
    }
    else
    {
        /* Long counters use the existing complete Chinese font, not clipping. */
        u8g2_SetFont(&u8g2, UI_FONT_CN);
        tw = (uint16_t)u8g2_GetUTF8Width(&u8g2, text);
        tx = centered && tw <= w - 8u ? (uint16_t)(x + (w - tw) / 2u) : (uint16_t)(x + 5u);
        ui_draw(tx, (uint16_t)(y + (h - 14u) / 2u + 12u), text);
    }
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_SetFont(&u8g2, UI_FONT_CN);
}

static void ui_menu_button(uint8_t right, const char *text, uint8_t selected)
{
    ui_menu_control(right ? 247u : 117u, 137u, 128u, 25u, text, selected, 1u);
}

static void ui_menu_field(uint16_t y, const char *label, const char *value,
                          uint8_t label_selected, uint8_t value_selected)
{
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawFrame(&u8g2, 117u, y, 207u, 27u);
    if (label_selected) u8g2_DrawBox(&u8g2, 119u, (uint16_t)(y + 2u), 117u, 23u);
    if (value_selected) u8g2_DrawBox(&u8g2, 238u, (uint16_t)(y + 2u), 84u, 23u);
    u8g2_SetDrawColor(&u8g2, label_selected ? 1u : 0u);
    ui_menu_text(122u, (uint16_t)(y + 20u), label, 16u);
    u8g2_SetDrawColor(&u8g2, value_selected ? 1u : 0u);
    ui_menu_text(250u, (uint16_t)(y + 20u), value, 16u);
    u8g2_SetDrawColor(&u8g2, 0);
}

static void ui_menu_header(uint8_t menu, uint8_t fullscreen, const char *subtitle)
{
    uint16_t left = fullscreen ? 1u : 113u;
    uint16_t width = fullscreen ? 382u : 266u;
    if (menu >= MAIN_LEN) menu = 0u;
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawBox(&u8g2, left, 1u, width, 166u);
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawRFrame(&u8g2, (uint16_t)(left + 1u), fullscreen ? 1u : 5u,
                    (uint16_t)(width - 2u), fullscreen ? 166u : 159u, 4u);
    u8g2_DrawXBMP(&u8g2, (uint16_t)(left + 5u), fullscreen ? 5u : 10u,
                  29u, 27u, ui_menu_title_icons[menu]);
    ui_menu_text((uint16_t)(left + 43u), 31u, Menu_List[menu].name, 18u);
    u8g2_DrawHLine(&u8g2, (uint16_t)(left + 6u), fullscreen ? 34u : 42u,
                   (uint16_t)(width - 12u));
    if (fullscreen)
    {
        ui_menu_text(200u, 28u, subtitle, 16u);
        ui_menu_control(298u, 9u, 75u, 24u, "返回", 1u, 1u);
    }
    u8g2_SetFont(&u8g2, UI_FONT_CN);
}

void UI_Menu_Display(void)
{
    uint8_t i;
    uint8_t bitmap_mode = u8g2.bitmap_transparency;
    uint8_t font_mode = u8g2.font_decode.is_transparent;
    uint8_t draw_color = u8g2.draw_color;
    if (UI_Select >= MAIN_LEN) UI_Select = 0;
    u8g2_SetBitmapMode(&u8g2, 1);
    u8g2_SetFontMode(&u8g2, 1);

    /* Preserve the three existing full-screen, twenty-channel branches. */
    if (Data_list1.menu_rank == 3)
    {
        uint8_t re_top = Data_list1.UI_main.re_flag;
        if (re_top == 3 || re_top == 4)
        {
            UI_Select = Data_list1.rank2_addr;
            install_Control();
            goto restore_style;
        }
        if (re_top == 5)
        {
            UI_Select = Data_list1.rank2_addr;
            summary_Control();
            goto restore_style;
        }
    }

    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawFrame(&u8g2, 1u, 1u, 382u, 166u);
    u8g2_DrawRFrame(&u8g2, 6u, 5u, 103u, 159u, 4u);
    u8g2_DrawFrame(&u8g2, 8u, 7u, 99u, 155u);
    for (i = 0u; i < MAIN_LEN; i++)
    {
        uint16_t top = (uint16_t)(9u + 19u * i);
        uint8_t selected = (uint8_t)(Data_list1.menu_rank == 2u && UI_Select == i);
        u8g2_SetDrawColor(&u8g2, 1);
        if (selected) u8g2_DrawBox(&u8g2, 10u, top, 95u, 19u);
        else if (UI_Select == i) u8g2_DrawVLine(&u8g2, 10u, top, 19u);
        u8g2_SetDrawColor(&u8g2, selected ? 0u : 1u);
        u8g2_DrawXBMP(&u8g2, 14u, (uint16_t)(top + 3u), 13u, 13u, ui_menu_nav_icons[i]);
        ui_menu_text(32u, (uint16_t)(top + 15u), Main_List[i].name, 16u);
    }
    ui_menu_header(UI_Select, 0u, "");
    Menu_List[UI_Select].function();

restore_style:
    u8g2_SetBitmapMode(&u8g2, bitmap_mode);
    u8g2_SetFontMode(&u8g2, font_mode);
    u8g2_SetDrawColor(&u8g2, draw_color);
    u8g2_SetFont(&u8g2, UI_FONT_CN);
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
    char line[48];
    uint8_t i, n, first, pages;
    sprintf(line, "总设备数:%d", g_binding_count);
    u8g2_SetFont(&u8g2, UI_FONT_CN);
    ui_draw(122u, 56u, line);
    n = g_binding_count;
    if (n > MAX_BINDING_NUM) n = MAX_BINDING_NUM;
    pages = (uint8_t)((n + 2) / 3);
    first = pages ? (uint8_t)(((g_name_chk_sec / 4) % pages) * 3) : 0;
    u8g2_SetFont(&u8g2, UI_FONT_CN);
    for (i = first; i < n && i < first + 3; i++)
    {
        uint16_t y = (uint16_t)(71u + 26u * (i - first));
        sprintf(line, "%02d", i + 1);
        ui_draw(122u, y, line);
        ui_draw_name(144u, y, g_binding_list[i].name, 224u);
        sprintf(line, "%02X%02X%02X%02X%02X%02X",
                g_binding_list[i].mac[0], g_binding_list[i].mac[1], g_binding_list[i].mac[2],
                g_binding_list[i].mac[3], g_binding_list[i].mac[4], g_binding_list[i].mac[5]);
        ui_draw(144u, (uint16_t)(y + 13u), line);
    }
    if (with_buttons)
    {
        ui_menu_control(117u, 140u, 128u, 22u, "解绑",
                         (uint8_t)(Data_list1.menu_rank == 3u && rank3 == 0u), 1u);
        ui_menu_control(247u, 140u, 128u, 22u, "返回",
                         (uint8_t)(Data_list1.menu_rank == 3u && rank3 != 0u), 1u);
    }
    else ui_menu_control(117u, 140u, 128u, 22u, "返回",
                          (uint8_t)(Data_list1.menu_rank == 3u), 1u);
}

/* 绑定设备子页（re_flag==2）：显示最近扫描到的蓝牙名称 + 保存目前设备/返回
 *   "保存目前设备" = STM32 发 0x05（CH584M 在 case 0x05 里落盘绑定表）。 */
static void binding_scan_page(uint8_t rank3)
{
    uint8_t i, n;
    const char *nm;
    u8g2_SetFont(&u8g2, UI_FONT_CN);
    ui_draw(122u, 56u, "扫描到的蓝牙名称");
    n = scan_name_cache_count();
    if (n == 0u) ui_draw(122u, 71u, "--");
    for (i = 0u; i < n && i < 6u; i++)
    {
        nm = scan_name_cache_name(i);
        if (nm != NULL)
            ui_draw_name(122u, (uint16_t)(71u + 13u * i), (const uint8_t *)nm, 246u);
        else ui_draw(122u, (uint16_t)(71u + 13u * i), "--");
    }
    ui_menu_control(117u, 140u, 128u, 22u, "保存目前设备",
                     (uint8_t)(Data_list1.menu_rank == 3u && rank3 == 0u), 1u);
    ui_menu_control(247u, 140u, 128u, 22u, "返回",
                     (uint8_t)(Data_list1.menu_rank == 3u && rank3 != 0u), 1u);
}

void binding_Control(void)
{
    uint8_t re = Data_list1.UI_main.re_flag;
    uint8_t rank3 = Data_list1.rank3_addr;
    char buf[48];
    if (re == 2) { binding_scan_page(rank3); return; }
    if (re == 1) { binding_device_list_page(rank3, 1); return; }
    sprintf(buf, "已绑定设备:%d", g_binding_count);
    ui_menu_control(117u, 50u, 258u, 25u, buf, ui_menu_selected(0u), 0u);
    ui_menu_control(117u, 79u, 258u, 25u, "一键解绑", ui_menu_selected(1u), 0u);
    ui_menu_control(117u, 108u, 258u, 25u, "绑定设备", ui_menu_selected(2u), 0u);
    ui_menu_button(1u, "返回", ui_menu_selected(3u));
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

    ui_menu_header(UI_Select, 1u, "名称");

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
    ui_menu_text(126u, 83u, "确认恢复出厂设置?", 16u);
    ui_menu_button(0u, "确认", (uint8_t)(Data_list1.menu_rank == 3u && rank3 == 0u));
    ui_menu_button(1u, "返回", (uint8_t)(Data_list1.menu_rank == 3u && rank3 != 0u));
}

void summary_Control(void)
{
    uint8_t re = Data_list1.UI_main.re_flag;
    uint8_t rank3 = Data_list1.rank3_addr;
    char buf[48];
    if (re == 5) { summary_name_page(Data_list1.UI_main.chu_num2); return; }
    if (re == 6) { summary_factory_page(rank3); return; }
    ui_menu_control(117u, 48u, 258u, 20u, "已绑定的设备名称", ui_menu_selected(0u), 0u);
    sprintf(buf, "蓝牙名称错误警报:%d", g_name_err_count);
    ui_menu_control(117u, 70u, 258u, 20u, buf, ui_menu_selected(1u), 0u);
    sprintf(buf, "蓝牙电压异常警报:%d", g_volt_err_count);
    ui_menu_control(117u, 92u, 258u, 20u, buf, ui_menu_selected(2u), 0u);
    ui_menu_control(117u, 114u, 258u, 20u, "恢复出厂", ui_menu_selected(3u), 0u);
    ui_menu_button(1u, "返回", ui_menu_selected(4u));
}

typedef struct {

    uint16_t posx;                   // 文本起始x坐标
    uint16_t posy;                   // 文本基线y坐标
    uint8_t font_size;            // 字体大小（16/24/32）
    uint16_t label_wight;         // 显示文本的宽度
    uint8_t  kuang_wight;         //方框离文字的宽
    uint8_t  kuang_high;          //方框离文字的高

} OptionItem;
void addr_Control(void)
{
    char value[20];
    sprintf(value, "%d", Data_list1.Menu_rank1.set_sub_num);
    ui_menu_field(62u, "分站号:", value, ui_menu_selected(0u), ui_menu_selected(1u));
    sprintf(value, "%d", Data_list1.Menu_rank1.set_host_num);
    ui_menu_field(99u, "本机地址:", value, ui_menu_selected(2u), ui_menu_selected(3u));
    u8g2_DrawXBMP(&u8g2, 325u, 73u, 47u, 52u, ui_menu_detail_address);
    ui_menu_button(0u, "保存并重启", ui_menu_selected(4u));
    ui_menu_button(1u, "不保存返回", ui_menu_selected(5u));
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
void networking_Control1(void)
{
    /* Same four STM32-controlled actions, in their original index order. */
    ui_menu_text(123u, 63u, "组网", 16u);
    u8g2_DrawFrame(&u8g2, 130u, 74u, 25u, 18u);
    u8g2_DrawFrame(&u8g2, 133u, 77u, 19u, 12u);
    u8g2_DrawVLine(&u8g2, 142u, 92u, 5u);
    u8g2_DrawHLine(&u8g2, 136u, 97u, 13u);
    u8g2_DrawHLine(&u8g2, 161u, 84u, 63u);
    u8g2_DrawXBMP(&u8g2, 226u, 71u, 29u, 27u, ui_menu_title_icons[1]);
    u8g2_DrawHLine(&u8g2, 258u, 84u, 63u);
    u8g2_DrawFrame(&u8g2, 327u, 73u, 21u, 25u);
    ui_menu_control(117u, 107u, 128u, 25u, "开始组网", ui_menu_selected(0u), 1u);
    ui_menu_control(247u, 107u, 128u, 25u, "返回", ui_menu_selected(1u), 1u);
    ui_menu_button(0u, "重置组网", ui_menu_selected(2u));
    ui_menu_button(1u, "保存组网", ui_menu_selected(3u));
}

void networking_Control2(void)
{
    char value[20];
    const char *labels[] = {"次数序号:", "本机地址:", "测试地址:", "测试次数:", "成功比率:"};
    uint8_t i;
    for (i = 0u; i < 5u; i++)
    {
        uint16_t y = (uint16_t)(47u + 23u * i);
        if (i == 0u) sprintf(value, "%d", Data_list1.Menu_rank2.xuhao_num);
        else if (i == 1u)
        {
            if (Data_list1.Menu_rank2.now_host_addr == 121u) sprintf(value, "中继");
            else sprintf(value, "%d", Data_list1.Menu_rank2.now_host_addr);
        }
        else if (i == 2u)
        {
            if (Data_list1.Menu_rank2.text_host_addr == 121u) sprintf(value, "中继");
            else if (Data_list1.Menu_rank2.text_host_addr == 122u) sprintf(value, "分站");
            else sprintf(value, "%d", Data_list1.Menu_rank2.text_host_addr);
        }
        else if (i == 3u) sprintf(value, "%d", Data_list1.Menu_rank2.text_cnt);
        else sprintf(value, "%d %%", Data_list1.Menu_rank2.bl_numl);
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_DrawFrame(&u8g2, 117u, y, 258u, 22u);
        ui_menu_text(122u, (uint16_t)(y + 17u), labels[i], 16u);
        ui_menu_text(250u, (uint16_t)(y + 17u), value, 16u);
    }
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

    ui_menu_header(UI_Select, 1u, (kind == 3) ? "信号" : "电压");

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

void install_Control(void)
{
    uint8_t re = Data_list1.UI_main.re_flag;
    if (re == 3) { install_ch_page(3, Data_list1.UI_main.chu_num2); return; }
    if (re == 4) { install_ch_page(4, Data_list1.UI_main.chu_num2); return; }
    ui_menu_control(117u, 62u, 258u, 27u, "设备信号", ui_menu_selected(0u), 0u);
    ui_menu_control(117u, 99u, 258u, 27u, "设备电压", ui_menu_selected(1u), 0u);
    ui_menu_button(1u, "返回", ui_menu_selected(2u));
}
void uploading_Control(void)
{
    uint8_t i;
    char label[16], old_value[20], new_value[20];
    ui_menu_text(190u, 61u, "已存地址", 16u);
    ui_menu_text(290u, 61u, "更改为", 16u);
    for (i = 0u; i < 3u; i++)
    {
        uint16_t y = (uint16_t)(65u + 23u * i);
        uint16_t old = Data_list1.Menu_rank5.old_send_addr[i];
        uint16_t next = Data_list1.Menu_rank5.new_send_addr[i];
        sprintf(label, "上传%d:", i + 1);
        if (old == 0u) sprintf(old_value, "中继");
        else if (old == 121u) sprintf(old_value, "无");
        else if (old == 122u) sprintf(old_value, "分站");
        else sprintf(old_value, "%d号", old);
        if (next == 0u) sprintf(new_value, "中继");
        else if (next == 121u) sprintf(new_value, "无");
        else if (next == 122u) sprintf(new_value, "分站");
        else sprintf(new_value, "%d号", next);
        ui_menu_text(122u, (uint16_t)(y + 17u), label, 16u);
        ui_menu_control(189u, y, 86u, 22u, old_value, 0u, 1u);
        ui_menu_control(279u, y, 96u, 22u, new_value, ui_menu_selected(i), 1u);
    }
    ui_menu_button(0u, "保存并重启", ui_menu_selected(3u));
    ui_menu_button(1u, "不保存返回", ui_menu_selected(4u));
}
void Other_Settings_Control(void)
{
    char value[20];
    sprintf(value, "%d db", Data_list1.Menu_rank6.power);
    ui_menu_field(49u, "通信功率:", value, ui_menu_selected(0u), ui_menu_selected(1u));
    sprintf(value, "%d 秒", Data_list1.Menu_rank6.time_light);
    ui_menu_field(78u, "亮屏时间:", value, ui_menu_selected(2u), ui_menu_selected(3u));
    ui_menu_field(107u, "通信状态:", Data_list1.Menu_rank6.state == 1 ? "开机" : "不开机",
                  ui_menu_selected(4u), ui_menu_selected(5u));
    u8g2_DrawXBMP(&u8g2, 326u, 68u, 46u, 58u, ui_menu_detail_other);
    ui_menu_button(0u, "保存并重启", ui_menu_selected(6u));
    ui_menu_button(1u, "不保存返回", ui_menu_selected(7u));
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
void return_main_Control(void)
{
    /* Static reference artwork only; the STM32 still owns the return action. */
    u8g2_SetDrawColor(&u8g2, 1u);
    u8g2_DrawBox(&u8g2, 113u, 1u, 266u, 166u);
    u8g2_SetDrawColor(&u8g2, 0u);
    u8g2_DrawXBMP(&u8g2, 113u, 1u, 266u, 166u, ui_menu_return_page);
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

