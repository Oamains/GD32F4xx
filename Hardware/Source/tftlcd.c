//
// Created by Oamains on 2023/5/30.
//
#include "tftlcd.h"
#include "lcdfont.h"


void LCD_Fill_GAM(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, const void *color) {
    uint32_t num, times, numlast;
    num = (xend - xsta + 1) * (yend - ysta + 1) * 2;
    times = 1;
    LCD_Address_Set(xsta, ysta, xend, yend); //设置显示范围
    LCD_CS_Clr();
    //delay_us(2);
    while (times) {
        if (num > 65534) {
            num -= 65534;
            numlast = 65534;
        } else {
            times = 0;
            numlast = num;
        }
        SPI2_DMA_Transmit((uint8_t *) color, 8, 1, numlast);
    }
    LCD_CS_Set();
    spi_i2s_data_frame_format_config(SPI2, SPI_FRAMESIZE_8BIT); //设置8位传输模式
    spi_enable(SPI2);
}

void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color) {
    u16 i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1);//设置显示范围
    for (i = ysta; i < yend; i++) {
        for (j = xsta; j < xend; j++) {
            LCD_WR_DATA(color);
        }
    }
}

void LCD_DrawPoint(u16 x, u16 y, u16 color) {
    LCD_Address_Set(x, y, x, y);//设置光标位置
    LCD_WR_DATA(color);
}


void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color) {
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //设置坐标增量
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

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color) {
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}


void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color) {
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


void LCD_ShowChinese(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode) {
    while (*s != 0) {
        if (sizey == 12) LCD_ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 16) LCD_ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 24) LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 32) LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
        else return;
        s += 2;
        x += sizey;
    }
}

void LCD_ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode) {
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12);    //统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode)//非叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else//叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j)) LCD_DrawPoint(x, y, fc);//画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵立即退出，防止多个汉字重复取模带来影响
    }
}

void LCD_ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode) {
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16);    //统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode)//非叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else//叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j)) LCD_DrawPoint(x, y, fc);//画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵立即退出，防止多个汉字重复取模带来影响
    }
}

void LCD_ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode) {
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;//ºº×ÖÊýÄ¿
    u16 TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óÐ¡
    u16 x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24);    //Í³¼Æºº×ÖÊýÄ¿
    for (k = 0; k < HZnum; k++) {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode)//·Çµþ¼Ó·½Ê½
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else//µþ¼Ó·½Ê½
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j)) LCD_DrawPoint(x, y, fc);//»­Ò»¸öµã
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //²éÕÒµ½¶ÔÓ¦µãÕó×Ö¿âÁ¢¼´ÍË³ö£¬·ÀÖ¹¶à¸öºº×ÖÖØ¸´È¡Ä£´øÀ´Ó°Ïì
    }
}

void LCD_ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode) {
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;//ºº×ÖÊýÄ¿
    u16 TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óÐ¡
    u16 x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32);    //Í³¼Æºº×ÖÊýÄ¿
    for (k = 0; k < HZnum; k++) {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode)//·Çµþ¼Ó·½Ê½
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else//µþ¼Ó·½Ê½
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j)) LCD_DrawPoint(x, y, fc);//»­Ò»¸öµã
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //²éÕÒµ½¶ÔÓ¦µãÕó×Ö¿âÁ¢¼´ÍË³ö£¬·ÀÖ¹¶à¸öºº×ÖÖØ¸´È¡Ä£´øÀ´Ó°Ïì
    }
}

void LCD_ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode) {
    u8 temp, sizex, t, m = 0;
    u16 i, TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óÐ¡
    u16 x0 = x;
    sizex = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num = num - ' ';    //µÃµ½Æ«ÒÆºóµÄÖµ
    LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1);  //ÉèÖÃ¹â±êÎ»ÖÃ
    for (i = 0; i < TypefaceNum; i++) {
        if (sizey == 12)temp = ascii_1206[num][i];               //µ÷ÓÃ6x12×ÖÌå
        else if (sizey == 16)temp = ascii_1608[num][i];         //µ÷ÓÃ8x16×ÖÌå
        else if (sizey == 24)temp = ascii_2412[num][i];         //µ÷ÓÃ12x24×ÖÌå
        else if (sizey == 32)temp = ascii_3216[num][i];         //µ÷ÓÃ16x32×ÖÌå
        else return;
        for (t = 0; t < 8; t++) {
            if (!mode)//·Çµþ¼ÓÄ£Ê½
            {
                if (temp & (0x01 << t))LCD_WR_DATA(fc);
                else LCD_WR_DATA(bc);
                m++;
                if (m % sizex == 0) {
                    m = 0;
                    break;
                }
            } else//µþ¼ÓÄ£Ê½
            {
                if (temp & (0x01 << t))LCD_DrawPoint(x, y, fc);//»­Ò»¸öµã
                x++;
                if ((x - x0) == sizex) {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode) {
    while (*p != '\0') {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}


u32 mypow(u8 m, u8 n) {
    u32 result = 1;
    while (n--)result *= m;
    return result;
}

void LCD_ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey) {
    u8 t, temp;
    u8 enshow = 0;
    u8 sizex = sizey / 2;
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


void LCD_ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey) {
    u8 t, temp, sizex;
    u16 num1;
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

void LCD_ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[]) {
    u16 i, j;
    u32 k = 0;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++) {
        for (j = 0; j < width; j++) {
            LCD_WR_DATA8(pic[k * 2]);
            LCD_WR_DATA8(pic[k * 2 + 1]);
            k++;
        }
    }
}
