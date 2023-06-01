//
// Created by Oamains on 2023/6/1.
//

#ifndef _LVGL_TFTLCD_H
#define _LVGL_TFTLCD_H

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void lcd_disp_config(void);
void TIM3_Int_Init(uint16_t arr,uint16_t psc);
#if defined(__cplusplus)
}
#endif
#endif //_LVGL_TFTLCD_H
