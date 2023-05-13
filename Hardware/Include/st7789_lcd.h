//
// Created by Oamains on 2023/5/13.
//

#ifndef ST7789_LCD_H
#define ST7789_LCD_H

#include "gd32f4xx.h"
#include "systick.h"
#include "spi.h"

#define LCD_W 240
#define LCD_H 280

#define LCD_RST_RCU                 RCU_GPIOF
#define LCD_RST_PORT                GPIOF
#define LCD_RST_PIN                 GPIO_PIN_10

#define LCD_DC_RCU                  RCU_GPIOA
#define LCD_DC_PORT                 GPIOA
#define LCD_DC_PIN                  GPIO_PIN_6

#define LCD_CS_RCU                  RCU_GPIOA
#define LCD_CS_PORT                 GPIOA
#define LCD_CS_PIN                  GPIO_PIN_4

#define LCD_BLK_RCU                 RCU_GPIOA
#define LCD_BLK_PORT                GPIOA
#define LCD_BLK_PIN                 GPIO_PIN_7


#define LCD_RST_L                   gpio_bit_reset(LCD_RST_PORT,LCD_RST_PIN)//RST  PF10
#define LCD_RST_H                   gpio_bit_set(LCD_RST_PORT,LCD_RST_PIN)

#define LCD_DC_L                    gpio_bit_reset(LCD_DC_PORT,LCD_DC_PIN)//DC  PA6
#define LCD_DC_H                    gpio_bit_set(LCD_DC_PORT,LCD_DC_PIN)

#define LCD_CS_L                    gpio_bit_reset(LCD_CS_PORT,LCD_CS_PIN)//CS  PA4
#define LCD_CS_H                    gpio_bit_set(LCD_CS_PORT,LCD_CS_PIN)

#define LCD_BLK_L                   gpio_bit_reset(LCD_BLK_PORT,LCD_BLK_PIN)//BLK  PA7
#define LCD_BLK_H                   gpio_bit_set(LCD_BLK_PORT,LCD_BLK_PIN)


#define Lcd_SpiRead                 Spi4_Read
#define Lcd_SpiWrite                Spi4_Write

typedef enum {
    lcdOrientation_Portrait = 0x00,  //竖屏
    lcdOrientation_APortrait = 0xC0, //竖屏，手机倒过来
    lcdOrientation_Left = 0x70,      //横屏，屏幕在home键左边（比较常用）
    lcdOrientation_Right = 0xA0      //横屏，屏幕在home键右边
} LcdOrientation;


void Lcd_Init();

void Lcd_SetOrientation(LcdOrientation lcdOrientation);

uint16_t Lcd_GetW();

uint16_t Lcd_GetH();

void Lcd_PushStart(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void Lcd_WriteData(uint8_t data);

void Lcd_WriteData16(uint16_t data);

#endif //ST7789_LCD_H
