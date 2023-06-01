//
// Created by Oamains on 2023/5/13.
//

#ifndef _SPI_H
#define _SPI_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_dma.h"

#define BUFFER_SIZE     (256U)
void Spi2_Init(void);
uint8_t SPI2_ReadWriteByte(uint8_t TxData);
void SPI2_DMA_Init(void);//SPI1-TX->DMA0-CH4
void SPI2_DMA_Transmit(const void* srcaddr, uint8_t datawidth, uint8_t srcaddrinc, uint32_t datanum);
#endif //_SPI_H
