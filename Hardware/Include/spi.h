//
// Created by Oamains on 2023/5/13.
//

#ifndef _SPI_H
#define _SPI_H

#include "gd32f4xx.h"
#include "systick.h"

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

#endif //_SPI_H
