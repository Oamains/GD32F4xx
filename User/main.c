/*!
    \file    main.c
    \brief   led spark with systick

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include <string.h>
#include "systick.h"
#include "gd32f4xx_misc.h"
#include "main.h"
#include "led.h"
#include "game_usart.h"
#include "led_key.h"
#include "timer_led.h"
#include "pwm_led.h"
#include "led_light_flow.h"
#include "lcd_gui.h"
#include "st7789_lcd.h"
#include "pic.h"

/*!
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void) {
    systick_config();
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    Key_Nvic_Key_Init();
    Led_Init();
    Usart_Send_Init(115200U);
    Timer5_Init(20000, 10000);
    Timer2_Init(10000, 10000);
    Pwm_Timer_Init(200, 10000);

//    while (1) {
//        Pwm_Breathing_Lamp();
//        if (receive_complete) {
//            printf("data size : %d\t ==> data : %s\t\n", receive_size, receive_buff_cache);
//            Usart_Switch_Led_State(receive_buff_cache);
//            memset(receive_buff_cache, 0, receive_size);
//            receive_complete = 0;
//            receive_size = 0;
//        }
//    }

    /* 扩展板 */
    Lcd_Init();                // spi lcd初始化
    /* SPI LCD // 深蓝色 背景 */
    LCD_Fill(0, 0, LCD_W, LCD_H, COLOR_BLUE);
    // 显示图片
    LCD_ShowPicture((LCD_W - 222) / 2, 0, 222, 60, gImage_lCKFB);
    // 绘制一个矩形
    LCD_DrawRectangle(10, 60 + 10, LCD_W - 10, LCD_H - 10, COLOR_BLACK);
    // 显示12汉字字符串
//    LCD_ShowChinese(15, 60 + 10 + 5, "开始游戏", COLOR_WHITE, COLOR_BLUE, 12, 0);
//    // 显示16汉字字符串
//    LCD_ShowChinese(15, 60 + 10 + 5 + 12, "开始游戏", COLOR_WHITE, COLOR_BLUE, 16, 0);
//    // 显示24汉字字符串
//    LCD_ShowChinese(15, 60 + 10 + 5 + 12 + 16, "开始游戏", COLOR_WHITE, COLOR_BLUE, 24, 0);
//    // 显示32汉字字符串
//    LCD_ShowChinese(15, 60 + 10 + 5 + 12 + 16 + 24, "开始游戏", COLOR_WHITE, COLOR_BLUE, 32, 0);
    // 显示16字符串
    LCD_ShowString(15, 60 + 10 + 5 + 12 + 16 + 24 + 32, "123abc", COLOR_WHITE, COLOR_BLUE, 16, 0);
    // 显示24字符串
    LCD_ShowString(15, 60 + 10 + 5 + 12 + 16 + 24 + 32 + 16, "123abc", COLOR_WHITE, COLOR_BLUE, 24, 0);
    // 显示32字符串
    LCD_ShowString(15, 60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24, "123abc", COLOR_WHITE, COLOR_BLUE, 32, 0);

    while (1);
}
