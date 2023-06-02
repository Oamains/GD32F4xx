//
// Created by Oamains on 2023/5/30.
//

#ifndef _TFTLCD_H
#define _TFTLCD_H
#include "tftlcd_init.h"
#include "gd32f4xx.h"
#include "systick.h"
#include "spi.h"
#include "src/misc/lv_color.h"


void LCD_Fill_GAM(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, lv_color_t *color);
/**
 * 指定区域填充颜色
 * @param xsta
 * @param ysta
 * @param xend
 * @param yend
 * @param color
 */
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);

/**
 * 在指定位置画一个点
 * @param x
 * @param y
 * @param color
 */
void LCD_DrawPoint(u16 x, u16 y, u16 color);

/**
 * 在指定位置画一条线
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 */
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);

/**
 * 在指定位置画一个矩形
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 */
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);

/**
 * 在指定位置画一个圆
 * @param x0
 * @param y0
 * @param r
 * @param color
 */
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color);

/**
 * 显示汉字字符
 * @param x
 * @param y
 * @param s
 * @param fc
 * @param bc
 * @param sizey
 * @param mode
 */
void LCD_ShowChinese(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);

/**
 * 显示单个12x12汉字
 * @param x
 * @param y
 * @param s
 * @param fc
 * @param bc
 * @param sizey
 * @param mode
 */
void LCD_ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);

/**
 * 显示单个16x16汉字
 * @param x
 * @param y
 * @param s
 * @param fc
 * @param bc
 * @param sizey
 * @param mode
 */
void LCD_ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);

/**
 * 显示单个24x24汉字
 * @param x
 * @param y
 * @param s
 * @param fc
 * @param bc
 * @param sizey
 * @param mode
 */
void LCD_ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);

/**
 * 显示单个32x32汉字
 * @param x
 * @param y
 * @param s
 * @param fc
 * @param bc
 * @param sizey
 * @param mode
 */
void LCD_ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);

/**
 * 显示一个字符
 * @param x
 * @param y
 * @param num
 * @param fc
 * @param bc
 * @param sizey
 * @param mode
 */
void LCD_ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode);

/**
 * 显示字符串
 * @param x
 * @param y
 * @param p
 * @param fc
 * @param bc
 * @param sizey
 * @param mode
 */
void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode);
/**
 * 求幂
 * @param m
 * @param n
 * @return
 */
u32 mypow(u8 m, u8 n);

/**
 * 显示整数变量
 * @param x
 * @param y
 * @param num
 * @param len
 * @param fc
 * @param bc
 * @param sizey
 */
void LCD_ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey);

/**
 * x显示两位小数变量
 * @param x
 * @param y
 * @param num
 * @param len
 * @param fc
 * @param bc
 * @param sizey
 */
void LCD_ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey);

/**
 * 显示图片
 * @param x
 * @param y
 * @param length
 * @param width
 * @param pic
 */
void LCD_ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[]);


//画笔颜色
#define WHITE             0xFFFF
#define BLACK             0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED                   0XFFE0
#define GBLUE                   0X07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define GREEN             0x07E0
#define CYAN             0x7FFF
#define YELLOW             0xFFE0
#define BROWN                 0XBC40 //棕色
#define BRRED                 0XFC07 //棕红色
#define GRAY                 0X8430 //灰色
#define DARKBLUE         0X01CF    //深蓝色
#define LIGHTBLUE         0X7D7C    //浅蓝色
#define GRAYBLUE         0X5458 //灰蓝色
#define LIGHTGREEN         0X841F //浅绿色
#define LGRAY                 0XC618 //浅灰色（PANNEL）窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色（中间层颜色）
#define LBBLUE           0X2B12 //浅棕蓝色（选择条码的反色）
#endif //_TFTLCD_H
