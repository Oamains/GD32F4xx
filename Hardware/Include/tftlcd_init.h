//
// Created by Oamains on 2023/5/30.
//

#ifndef _TFTLCD_INIT_H
#define _TFTLCD_INIT_H

#include "gd32f4xx.h"
#include "systick.h"

#define USE_HORIZONTAL 0  // 设置横屏或者竖屏 0或1为竖屏 2或3为横屏


#define LCD_W 240
#define LCD_H 240


#define LCD_RST_RCU    RCU_GPIOG
#define LCD_RST_PORT   GPIOG
#define LCD_RST_PIN    GPIO_PIN_6

#define LCD_DC_RCU     RCU_GPIOC
#define LCD_DC_PORT    GPIOC
#define LCD_DC_PIN     GPIO_PIN_6

#define LCD_CS_RCU     RCU_GPIOD
#define LCD_CS_PORT    GPIOD
#define LCD_CS_PIN     GPIO_PIN_1

#define LCD_BLK_RCU    RCU_GPIOD
#define LCD_BLK_PORT   GPIOD
#define LCD_BLK_PIN    GPIO_PIN_5

#define LCD_SCL_RCU    RCU_GPIOF
#define LCD_SCL_PORT   GPIOF
#define LCD_SCL_PIN    GPIO_PIN_7

#define LCD_SDA_RCU    RCU_GPIOF
#define LCD_SDA_PORT   GPIOF
#define LCD_SDA_PIN    GPIO_PIN_9


//-----------------LCD端口定义----------------

#define LCD_SCLK_Clr() gpio_bit_reset(LCD_SCL_PORT,LCD_SCL_PIN)//SCL=SCLK
#define LCD_SCLK_Set() gpio_bit_set(LCD_SCL_PORT,LCD_SCL_PIN)

#define LCD_MOSI_Clr() gpio_bit_reset(LCD_SDA_PORT,LCD_SDA_PIN)//SDA=MOSI
#define LCD_MOSI_Set() gpio_bit_set(LCD_SDA_PORT,LCD_SDA_PIN)

#define LCD_RES_Clr()  gpio_bit_reset(LCD_RST_PORT,LCD_RST_PIN)//RES
#define LCD_RES_Set()  gpio_bit_set(LCD_RST_PORT,LCD_RST_PIN)

#define LCD_DC_Clr()   gpio_bit_reset(LCD_DC_PORT,LCD_DC_PIN)//DC
#define LCD_DC_Set()   gpio_bit_set(LCD_DC_PORT,LCD_DC_PIN)

#define LCD_CS_Clr()   gpio_bit_reset(LCD_CS_PORT,LCD_CS_PIN)//CS
#define LCD_CS_Set()   gpio_bit_set(LCD_CS_PORT,LCD_CS_PIN)

#define LCD_BLK_Clr()  gpio_bit_reset(LCD_BLK_PORT,LCD_BLK_PIN)//BLK
#define LCD_BLK_Set()  gpio_bit_set(LCD_BLK_PORT,LCD_BLK_PIN)


void LCD_GPIO_Init(void);// 初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

#endif //_TFTLCD_INIT_H
