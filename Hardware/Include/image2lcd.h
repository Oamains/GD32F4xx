#ifndef __IMAGE2LCD_H
#define __IMAGE2LCD_H
#include "gd32f4xx.h"
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

//4096色/16位真彩色/18位真彩色/24位真彩色/32位真彩色
//图像数据头结构体  
__packed typedef struct _HEADCOLOR
{
   unsigned char scan;
   unsigned char gray;
   unsigned short w;
   unsigned short h;
   unsigned char is565;
   unsigned char rgb;
}HEADCOLOR;  
//scan: 扫描模式 	    
//Bit7: 0:自左至右扫描，1:自右至左扫描。 
//Bit6: 0:自顶至底扫描，1:自底至顶扫描。 
//Bit5: 0:字节内象素数据从高位到低位排列，1:字节内象素数据从低位到高位排列。 
//Bit4: 0:WORD类型高低位字节顺序与PC相同，1:WORD类型高低位字节顺序与PC相反。 
//Bit3~2: 保留。 
//Bit1~0: [00]水平扫描，[01]垂直扫描，[10]数据水平,字节垂直，[11]数据垂直,字节水平。 
//gray: 灰度值 
//   灰度值，1:单色，2:四灰，4:十六灰，8:256色，12:4096色，16:16位彩色，24:24位彩色，32:32位彩色。
//w: 图像的宽度。	 
//h: 图像的高度。
//is565: 在4096色模式下为0表示使用[16bits(WORD)]格式，此时图像数据中每个WORD表示一个象素；为1表示使用[12bits(连续字节流)]格式，此时连续排列的每12Bits代表一个象素。
//在16位彩色模式下为0表示R G B颜色分量所占用的位数都为5Bits，为1表示R G B颜色分量所占用的位数分别为5Bits,6Bits,5Bits。
//在18位彩色模式下为0表示"6Bits in Low Byte"，为1表示"6Bits in High Byte"。
//在24位彩色和32位彩色模式下is565无效。	  
//rgb: 描述R G B颜色分量的排列顺序，rgb中每2Bits表示一种颜色分量，[00]表示空白，[01]表示Red，[10]表示Green，[11]表示Blue。

void image_display(uint16_t x,uint16_t y,uint8_t * imgx);//在指定位置显示图片
void image_show(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint8_t scan,uint8_t *p);//在指定区域开始显示图片
uint16_t image_getcolor(uint8_t mode,uint8_t *str);//获取颜色

#endif













