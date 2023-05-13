#include "lcd_gui.h"
//
// Created by Oamains on 2023/5/13.
//
/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color) {
    uint16_t i, j;
    Lcd_PushStart(xsta, ysta, xend - 1, yend - 1);//设置显示范围
    for (i = ysta; i < yend; i++) {
        for (j = xsta; j < xend; j++) {
            Lcd_WriteData16(color);
        }
    }
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
    Lcd_PushStart(x, y, x, y);//设置光标位置
    Lcd_WriteData16(color);
}


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;//画线起点坐标
    uCol = y1;
    if (delta_x > 0)incx = 1; //设置单步方向
    else if (delta_x == 0)incx = 0;//垂直线
    else {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0;//水平线
    else {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
    else distance = delta_y;
    for (t = 0; t < distance + 1; t++) {
        LCD_DrawPoint(uRow, uCol, color);//画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}


void DrawEclipsePoint(uint16_t x0, uint16_t y0, uint16_t x, uint16_t y, uint16_t color) {
    LCD_DrawPoint(x0 + x, y0 + y, color);
    LCD_DrawPoint(x0 - x, y0 + y, color);
    LCD_DrawPoint(x0 + x, y0 - y, color);
    LCD_DrawPoint(x0 - x, y0 - y, color);
}


void MiddlePiontEclipse(uint16_t x0, uint16_t y0, uint16_t a, uint16_t b, uint16_t color) {
    //alert("dre eclipse");
    uint16_t a2 = a * a, b2 = b * b;
    uint16_t d2, d1 = b2 + a2 * (0.25 - b);
    uint16_t x = 0, y = b;
    DrawEclipsePoint(x0, y0, x, y, color);

    //在上面的部分，|k|<1
    while (b2 * (x + 1) < a2 * (y - 0.5)) {
        if (d1 < 0)
            d1 += b2 * (2 * x + 3);
        else {
            d1 += b2 * (2 * x + 3) + a2 * (2 - 2 * y);
            y--;
        }
        x++;
        DrawEclipsePoint(x0, y0, x, y, color);
    }

    //画在下面的部分，|k|>1
    d2 = sqrt(b * (x + 0.5) + a * (y - 1)) - a * b;
    while (y > 0) {
        if (d2 < 0) {
            d2 += b2 * (2 * x + 2) + a2 * (3 - 2 * y);
            x++;
        } else {
            d2 += a2 * (3 - 2 * y);
        }
        y--;
        DrawEclipsePoint(x0, y0, x, y, color);
    }
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color) {
    int a, b;
    a = 0;
    b = r;
    while (a <= b) {
        LCD_DrawPoint(x0 - b, y0 - a, color);             //3
        LCD_DrawPoint(x0 + b, y0 - a, color);             //0
        LCD_DrawPoint(x0 - a, y0 + b, color);             //1
        LCD_DrawPoint(x0 - a, y0 - b, color);             //2
        LCD_DrawPoint(x0 + b, y0 + a, color);             //4
        LCD_DrawPoint(x0 + a, y0 - b, color);             //5
        LCD_DrawPoint(x0 + a, y0 + b, color);             //6
        LCD_DrawPoint(x0 - b, y0 + a, color);             //7
        a++;
        if ((a * a + b * b) > (r * r))//判断要画的点是否过远
        {
            b--;
        }
    }
}


/* 画实心圆 */
void Draw_Solid_Circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    uint16_t a, b, b1 = 0, b2 = 0, b3;
    for (a = 0; a < 239; a++) {
        b1 = 0;
        b2 = 240;
        for (b = 0; b < 319; b++) {
            if ((((r * r + 50) - (b - y) * (b - y)) > (a - x) * (a - x)) &
                (((r * r - 50) - (b - y) * (b - y)) < (a - x) * (a - x)))  //19*19<(a-x)*(a-x)<21*21
            {
                Lcd_PushStart(a, b, a, b);  //设置点的位置
                Lcd_WriteData16(color);       //画点
                b1 = b;
                if (b1 > b2) {
                    Lcd_PushStart(a, b2, a, b1);  //设置点的位置
                    b3 = b1 - b2;
                    while (b3--) {
                        Lcd_WriteData16(color);
                    }
                }
                b2 = b1;
            }
        }
    }
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode) {
    while (*p != '\0') {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--)result *= m;
    return result;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue;
            } else enshow = 1;

        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}


/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey) {
    uint8_t t, temp, sizex;
    uint16_t num1;
    sizex = sizey / 2;
    num1 = num * 100;
    for (t = 0; t < len; t++) {
        temp = (num1 / mypow(10, len - t - 1)) % 10;
        if (t == (len - 2)) {
            LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len += 1;
        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}


/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[]) {
    uint16_t i, j;
    uint32_t k = 0;
    Lcd_PushStart(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++) {
        for (j = 0; j < width; j++) {
            Lcd_WriteData(pic[k * 2]);
            Lcd_WriteData(pic[k * 2 + 1]);
            k++;
        }
    }
}