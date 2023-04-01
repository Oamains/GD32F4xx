//
// Created by Oamains on 2023/4/1.
//

#ifndef _PWM_H
#define _PWM_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_timer.h"
#include "usart.h"
#include "led.h"

#define PWM_LED_RCU             RCU_GPIOA
#define PWM_LED_PORT            GPIOA
#define PWM_LED_PIN             GPIO_PIN_5
#define PWM_LED_AF              GPIO_AF_1


#define PWM_TIMER_RCU           RCU_TIMER1
#define PWM_TIMER               TIMER1
#define PWM_TIMER_CHANNEL       TIMER_CH_0

void Pwm_Timer_Init(uint16_t period, uint16_t prescaler);

void Pwm_Breathing_Lamp(void);

#endif //_PWM_H
