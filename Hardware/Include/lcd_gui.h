//
// Created by Oamains on 2023/5/13.
//

#ifndef LCD_GUI_H
#define LCD_GUI_H

#include <stdint-gcc.h>
#include "lcdfont.h"
#include "math.h"
#include "st7789_lcd.h"

#define COLOR_GOLDEN   (0XFEA0)//金色
#define COLOR_SILVER   (0XC618)//银色
#define COLOR_BRONZE   (0XBB86)//古铜
#define COLOR_WHITE   (0xFFFF)
#define COLOR_BLACK   (0x0000)
#define COLOR_BLUE    (0x001F)
#define COLOR_BRED    (0XF81F)
#define COLOR_GRED    (0XFFE0)
#define COLOR_GBLUE   (0X07FF)
#define COLOR_RED     (0xF800)
#define COLOR_MAGENTA (0xF81F)
#define COLOR_GREEN   (0x07E0)
#define COLOR_CYAN    (0x7FFF)
#define COLOR_YELLOW  (0xFFE0)
#define COLOR_BROWN   (0XBC40)      //棕色
#define COLOR_BRRED   (0XFC07)      //棕红色
#define COLOR_GRAY    (0X8430)       //灰色
#define COLOR_DARKBLUE (0X01CF)   //深蓝色
#define COLOR_LIGHTBLUE (0X7D7C)  //浅蓝色
#define COLOR_GRAYBLUE (0X5458)   //灰蓝色
#define COLOR_LIGHTGREEN (0X841F) //浅绿色
#define COLOR_LGRAY (0XC618)      //浅灰色(PANNEL),窗体背景色
#define COLOR_LGRAYBLUE (0XA651)  //浅灰蓝色(中间层颜色)
#define COLOR_LBBLUE (0X2B12)     //浅棕蓝色(选择条目的反色)

//指定区域填充颜色
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);

//在指定位置画一个点
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);

//在指定位置画一条线
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

//在指定位置画一个矩形
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

//在指定位置画一个圆
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

//显示汉字串
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);

void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                          uint8_t mode);//显示单个12x12汉字
//显示单个16x16汉字
void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                          uint8_t mode);

//显示单个24x24汉字
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                          uint8_t mode);

//显示单个32x32汉字
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                          uint8_t mode);

//显示一个字符
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);

//显示字符串
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);

//求幂
uint32_t mypow(uint8_t m, uint8_t n);

//显示整数变量
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);

//显示两位小数变量
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
//显示图片
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[]);

void DrawEclipsePoint(uint16_t x0, uint16_t y0, uint16_t x, uint16_t y, uint16_t color);

void MiddlePiontEclipse(uint16_t x0, uint16_t y0, uint16_t a, uint16_t b, uint16_t color);

void Draw_Solid_Circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);

#endif //LCD_GUI_H
