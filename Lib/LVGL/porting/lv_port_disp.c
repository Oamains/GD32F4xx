/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "lvgl.h"
#include "tftlcd_init.h"
#include "tftlcd.h"
#include "lvgl_tftlcd.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

#define MY_DISP_HOR_RES (LCD_W)
#define MY_DISP_VER_RES (LCD_H)

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void) {
    disp_init();
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES *
                            10];                                                               /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/
    static lv_disp_drv_t disp_drv;                                                                               /*Descriptor of a display driver*/
    lv_disp_drv_init(
            &disp_drv);                                                                           /*Basic initialization*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf_dsc_1;
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void) {
    /*You code here*/
    LCD_Init();
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    TIMER3_Init(10000, 1000);//1ms
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    // method 1
    LCD_Fill_GAM(area->x1, area->y1, area->x2, area->y2, color_p);
//    uint32_t x;
//    uint32_t y;
//    for (y = area->y1; y <= area->y2; y++) {
//        for (x = area->x1; x <= area->x2; x++) {
//            LCD_DrawPoint(x, y, color_p->full);
//            color_p++;
//        }
//    }
    lv_disp_flush_ready(disp_drv);
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
