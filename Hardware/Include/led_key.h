#ifndef __LED_KEY_H
#define __LED_KEY_H
#include "systick.h"
#include "gd32f4xx_misc.h"
#include "gd32f4xx_syscfg.h"
#include "gd32f4xx_exti.h"
#include "led.h"

/**
 * 按键初始化
 */
void Key_Init();

void Key_Nvic_Key_Init();

void Press_Key_Scan();
#endif