//
// Created by Oamains on 2023/6/1.
//
#include "lvgl_tftlcd.h"
#include "porting/lv_port_disp.h"
#include "gd32f4xx_gpio.h"

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

static uint8_t lv_tick_count = 0;
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * 通用定时器3中断初始化
 * @param period 周期值，是一个16位的计数器，最大值为65535，当计数器达到设置的周期数值（自动重装载寄存器）时数值清零，配合计数器时钟频率可以计算中断时间。。
 * @param prescaler 参数将决定一个计数周期的时间。设预分频值为pre,则计数器时钟频率PSC_CLK=TIMER_CK / (pre + 1)。
 */
void TIMER3_Init(uint16_t period, uint16_t prescaler) {
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER3);
    timer_deinit(TIMER3);
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = prescaler;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = period;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(TIMER3, &timer_initpara);
    timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER3, TIMER_INT_UP);
    nvic_irq_enable(TIMER3_IRQn, 1, 1);
    timer_enable(TIMER3);
}

/**
 * 定时器3中断服务程序
 */
void TIMER3_IRQHandler(void) {
    if (SET == timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP)) {
        timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
        lv_tick_count++;
        if (lv_tick_count == 500) lv_tick_count = 0;
        lv_tick_inc(1);
    }
}
