//
// Created by Oamains on 2023/4/1.
//
#ifndef __GAME_USART_H
#define __GAME_USART_H

#include "gd32f4xx.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_dma.h"
#include "gd32f4xx_usart.h"
#include <stdio.h>

#define GAME_USART_RCU                  RCU_USART0
#define GAME_USART                      USART0
#define GAME_USART_PORT                 GPIOA
#define GAME_USART_TX                   GPIO_PIN_9
#define GAME_USART_RX                   GPIO_PIN_10
#define GAME_USART_IRQ                  USART0_IRQn
#define GAME_USART_IRQ_HANDLER          USART0_IRQHandler

#define GAME_DMA_RCU                    RCU_DMA1
#define GAME_DMA                        DMA1
#define GAME_DMA_CHANNEL                DMA_CH2
#define GAME_DMA_IQR                    DMA1_Channel2_IRQn
#define GAME_DMA_IQR_HANDLER            DMA1_Channel2_IRQHandler

#define USART_RECEIVE_SIZE              4096

extern uint8_t receive_buff_cache[USART_RECEIVE_SIZE];
extern uint16_t receive_size;
extern uint8_t receive_complete;

/**
 * 串口初始化
 * @param baudval  数据传输波特率(MHz)
 */
void Usart_Send_Init(uint32_t baudval);

/**
 * 串口DMA接收初始化
 */
void Usart_Dma_Init();

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

#endif // __GAME_USART_H