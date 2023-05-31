//
// Created by Oamains on 2023/6/1.
//

#ifndef _LVGL_TFTLCD_H
#define _LVGL_TFTLCD_H

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LCD_WIDTH 240
#define LCD_HEIGHT 240
#define LCD_FB_BYTE_PER_PIXEL 1

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void lcd_disp_config(void);

#if defined(__cplusplus)
}
#endif
#endif //_LVGL_TFTLCD_H
