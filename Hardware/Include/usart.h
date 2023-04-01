//
// Created by Oamains on 2023/4/1.
//
#ifndef __USART_H
#define __USART_H

#include "gd32f4xx.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_usart.h"
#include <stdio.h>

/**
 * 串口初始化
 * @param baudval  数据传输波特率(MHz)
 */
void Usart_Init(uint32_t baudval);

/**
 * 串口数据发送
 * @param ucch
 */
void usart_send(uint8_t ucch);

/**
 * 串口字符串发送
 * @param ucstr
 */
void usart_str_send(uint8_t *ucstr);
#endif