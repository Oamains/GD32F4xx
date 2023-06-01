//
// Created by Oamains on 2023/5/13.
//

#ifndef _SPI_H
#define _SPI_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_dma.h"

/**
 * lcd spi 初始化
 */
void Spi_Init(void);

/**
 * lcd spi 写数据
 * @param dat
 */
void Spi_Write(uint8_t dat);

/**
 * lcd spi 读数据
 * @return
 */
uint8_t Spi_Read(void);


void SPI_DMA_Init(void);//SPI1-TX->DMA0-CH4

void SPI1_DMA_Transmit(const void* srcaddr, uint8_t datawidth, uint8_t srcaddrinc, uint32_t datanum);
#endif //_SPI_H
