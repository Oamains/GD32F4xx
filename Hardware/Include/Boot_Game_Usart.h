//
// Created by Oamains on 2023/4/1.
//
#ifndef __BOOT_GAME_USART_H
#define __BOOT_GAME_USART_H

#include "gd32f4xx.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_usart.h"
#include <stdio.h>

#define USART_RECEIVE_SIZE          4096

extern uint8_t receive_buff_cache[USART_RECEIVE_SIZE];
extern uint16_t receive_size;
extern uint8_t receive_complete;
/**
 * 串口初始化
 * @param baudval  数据传输波特率(MHz)
 */
void Usart_Send_Init(uint32_t baudval);

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

#endif // __BOOT_GAME_USART_H