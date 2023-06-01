//
// Created by Oamains on 2023/5/30.
//
#include "tftlcd_init.h"


void LCD_GPIO_Init(void) {
    rcu_periph_clock_enable(LCD_RST_RCU);
    gpio_mode_set(LCD_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_RST_PIN);
    gpio_output_options_set(LCD_RST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_RST_PIN);
    gpio_bit_set(LCD_RST_PORT,LCD_RST_PIN);

    rcu_periph_clock_enable(LCD_DC_RCU);
    gpio_mode_set(LCD_DC_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_DC_PIN);
    gpio_output_options_set(LCD_DC_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_DC_PIN);
    gpio_bit_set(LCD_DC_PORT,LCD_DC_PIN);

    rcu_periph_clock_enable(LCD_CS_RCU);
    gpio_mode_set(LCD_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_CS_PIN);
    gpio_output_options_set(LCD_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_CS_PIN);
    gpio_bit_set(LCD_CS_PORT,LCD_CS_PIN);

    rcu_periph_clock_enable(LCD_BLK_RCU);
    gpio_mode_set(LCD_BLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_BLK_PIN);
    gpio_output_options_set(LCD_BLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_BLK_PIN);
    gpio_bit_set(LCD_BLK_PORT,LCD_BLK_PIN);

//    rcu_periph_clock_enable(LCD_SCL_RCU);
//    gpio_mode_set(LCD_SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_SCL_PIN);
//    gpio_output_options_set(LCD_SCL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_SCL_PIN);
//    gpio_bit_set(LCD_SCL_PORT,LCD_SCL_PIN);
//
//    rcu_periph_clock_enable(LCD_SDA_RCU);
//    gpio_mode_set(LCD_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_SDA_PIN);
//    gpio_output_options_set(LCD_SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_SDA_PIN);
//    gpio_bit_set(LCD_SDA_PORT,LCD_SDA_PIN);


}
void delay(int t)
{
    while(t--);
}

void LCD_Writ_Bus(u8 dat) {
    LCD_CS_Clr();
    Lcd_SpiWrite(dat);
    delay(4);
    LCD_CS_Set();
}

void LCD_WR_DATA8(u8 dat) {
    LCD_Writ_Bus(dat);
}

void LCD_WR_DATA(u16 dat) {
    LCD_Writ_Bus(dat >> 8);
    LCD_Writ_Bus(dat);
}

void LCD_WR_REG(u8 dat) {
    LCD_DC_Clr();//写命令
    LCD_Writ_Bus(dat);
    LCD_DC_Set();//写数据
}


void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2) {
    if (USE_HORIZONTAL == 0) {
        LCD_WR_REG(0x2a);// 列表地址设置
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b); // 行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c); // 存储器写
    } else if (USE_HORIZONTAL == 1) {
        LCD_WR_REG(0x2a); // 列表地址设置
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1 + 80);
        LCD_WR_DATA(y2 + 80);
        LCD_WR_REG(0x2c);// 存储器写
    } else if (USE_HORIZONTAL == 2) {
        LCD_WR_REG(0x2a);//列表地址设置
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);// 行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);// 存储器写
    } else {
        LCD_WR_REG(0x2a);//列表地址设置
        LCD_WR_DATA(x1 + 80);
        LCD_WR_DATA(x2 + 80);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//存储器写
    }
}


void LCD_Init(void) {
    Spi2_Init();
    LCD_GPIO_Init();//初始化GPIO
    LCD_RES_Clr();// 复位
    delay_ms(100);
    LCD_RES_Set();
    delay_ms(100);

    LCD_BLK_Set();// 打开背光
    delay_ms(100);

    //************* Start Initial Sequence **********//
    LCD_WR_REG(0x11); //Sleep out
    delay_ms(120);              //Delay 120ms
    //************* Start Initial Sequence **********//
    LCD_WR_REG(0x36);
    if (USE_HORIZONTAL == 0)LCD_WR_DATA8(0x00);
    else if (USE_HORIZONTAL == 1)LCD_WR_DATA8(0xC0);
    else if (USE_HORIZONTAL == 2)LCD_WR_DATA8(0x70);
    else LCD_WR_DATA8(0xA0);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);

    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);

    LCD_WR_REG(0xB7);
    LCD_WR_DATA8(0x35);

    LCD_WR_REG(0xBB);
    LCD_WR_DATA8(0x32); //Vcom=1.35V

    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x15); //GVDD=4.8V  ÑÕÉ«Éî¶È

    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x20); //VDV, 0x20:0v

    LCD_WR_REG(0xC6);
    LCD_WR_DATA8(0x0F); //0x0F:60Hz

    LCD_WR_REG(0xD0);
    LCD_WR_DATA8(0xA4);
    LCD_WR_DATA8(0xA1);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x48);
    LCD_WR_DATA8(0x17);
    LCD_WR_DATA8(0x14);
    LCD_WR_DATA8(0x15);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x34);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x15);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x48);
    LCD_WR_DATA8(0x17);
    LCD_WR_DATA8(0x14);
    LCD_WR_DATA8(0x15);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x34);
    LCD_WR_REG(0x21);

    LCD_WR_REG(0x29);
}
