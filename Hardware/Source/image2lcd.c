#include "gd32f4xx.h"
#include "lcd.h"
#include "image2lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//FLASH图片显示		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/26
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
/**
 * 从8位数据获得16位颜色
 * mode:0,低位在前,高位在后.
 * 1,高位在前,低位在后.
 * str:数据
 * @param mode
 * @param str
 * @return
 */
uint16_t image_getcolor(uint8_t mode, uint8_t *str) {
    uint16_t color;
    if (mode) {
        color = ((uint16_t) *str++) << 8;
        color |= *str;
    } else {
        color = *str++;
        color |= ((uint16_t) *str) << 8;
    }
    return color;
}

/**
 * 在液晶上画图（仅支持：从左到右，从上到下 or 从上到下，从左到右 的扫描方式！）
 * xsta,ysta,width,height:画图区域
 * *p:图像数据
 * @param xsta
 * @param ysta
 * @param width
 * @param height
 * @param scan
 * @param p
 */
void image_show(uint16_t xsta, uint16_t ysta, uint16_t width, uint16_t height, uint8_t scan, uint8_t *p) {
    uint32_t i;
    uint32_t len = 0;

    LCD_Scan_Dir(L2R_U2D);//从上到下,从左到右
    LCD_Set_Window(xsta, ysta, width, height);
    LCD_SetCursor(xsta, ysta);//设置光标位置

    LCD_WriteRAM_Prepare();    //开始写入GRAM
    len = width * height;            //写入的数据长度
    for (i = 0; i < len; i++) {
        LCD_WriteRAM(image_getcolor(scan & (1 << 4), p));
        p += 2;
    }
    LCD_Set_Window(0, 0, lcddev.width, lcddev.height);
}

/**
 * 在指定的位置显示一个图片
 * 此函数可以显示image2lcd软件生成的任意16位真彩色图片.
 * 限制:
 *      1,尺寸不能超过屏幕的区域.
 *      2,生成数据时不能勾选:高位在前(MSB First)
 *      3,必须包含图片信息头数据
 * 注意:针对STM32,不能选择image2lcd的"高位在前(MSB First)"选项,否则imginfo的数据将不正确!!
 * @param x 指定位置
 * @param y 指定位置
 * @param imgx  图片数据(必须包含图片信息头,"4096色/16位真彩色/18位真彩色/24位真彩色/32位真彩色”的图像数据头)
 */
void image_display(uint16_t x, uint16_t y, uint8_t *imgx) {
    HEADCOLOR *imginfo;
    uint8_t ifosize = sizeof(HEADCOLOR);//得到HEADCOLOR结构体的大小
    imginfo = (HEADCOLOR *) imgx;
    image_show(x, y, imginfo->w, imginfo->h, imginfo->scan, imgx + ifosize);
}















