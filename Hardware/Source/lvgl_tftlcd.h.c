//
// Created by Oamains on 2023/6/1.
//
#include "lvgl_tftlcd.h"
#include "porting/lv_port_disp.h"
#include "gd32f4xx_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(uint16_t arr, uint16_t psc) {
    /* ----------------------------------------------------------------------------
    TIMER3 Configuration:
    TIMER3CLK = SystemCoreClock/108 = 1000KHz, the period is 1s(1000/1000000 = 1ms).
    ---------------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER3);

    timer_deinit(TIMER3);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER3 configuration */
    timer_initpara.prescaler = psc;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = arr;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(TIMER3, &timer_initpara);

    timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER3, TIMER_INT_UP);
    nvic_irq_enable(TIMER3_IRQn, 1, 1);
    timer_enable(TIMER3);
}

//定时器3中断服务程序
void TIMER3_IRQHandler(void)   //TIM3中断
{
    static uint16_t count = 0;
    if (SET == timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP)) {
        timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
        count++;
        if (count == 500) {
            count = 0;
        }
        lv_tick_inc(1);
    }
}
