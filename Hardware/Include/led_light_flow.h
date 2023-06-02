//
// Created by Oamains on 2023/4/2.
//

#ifndef _LED_LIGHT_FLOW_H
#define _LED_LIGHT_FLOW_H

#include "systick.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_timer.h"
#include "led.h"

void Control_Led_state();

void Usart_Switch_Led_State(uint8_t *led);
#endif //_LED_LIGHT_FLOW_H
