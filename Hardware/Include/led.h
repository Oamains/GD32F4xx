//
// Created by Oamains on 2023/4/1.
//
#ifndef __LED_H
#define __LED_H

#include "gd32f4xx.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"

#define LED1_RCU_PORT       RCU_GPIOE
#define LED1_PORT           GPIOE
#define LED1_PIN            GPIO_PIN_3

#define LED2_RCU_PORT       RCU_GPIOD
#define LED2_PORT           GPIOD
#define LED2_PIN            GPIO_PIN_7

#define LED3_RCU_PORT       RCU_GPIOG
#define LED3_PORT           GPIOG
#define LED3_PIN            GPIO_PIN_3

#define LED4_RCU_PORT       RCU_GPIOA
#define LED4_PORT           GPIOA
#define LED4_PIN            GPIO_PIN_5

/**
 * LED 初始化
 */
void Led_Init();

#endif