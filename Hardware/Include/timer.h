//
// Created by Duanfz on 2023/4/1.
//

#ifndef _TIMER_H
#define _TIMER_H
#include "gd32f4xx.h"
#include "gd32f4xx_misc.h"
#include "gd32f4xx_syscfg.h"
#include "gd32f4xx_exti.h"
#include "gd32f4xx_timer.h"
#include "usart.h"
#include "led.h"


void Timer5_Init(uint16_t period, uint16_t prescaler);
void Timer2_Init(uint16_t period, uint16_t prescaler);
#endif //_TIMER_H
