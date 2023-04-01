//
// Created by Oamains on 2023/4/1.
//
#include "timer_led.h"


void Timer5_Init(uint16_t prescaler, uint16_t period) {

    rcu_periph_clock_enable(RCU_TIMER5);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    //初始化TIMER5
    timer_parameter_struct timer_parameters;
    timer_parameters.period = prescaler - 1;                  /*!< period value */
    timer_parameters.prescaler = period - 1;                  /*!< prescaler value */
    timer_parameters.alignedmode = TIMER_COUNTER_EDGE;        /*!< aligned mode */
    timer_parameters.counterdirection = TIMER_COUNTER_UP;     /*!< counter direction */
    timer_parameters.clockdivision = TIMER_CKDIV_DIV1;        /*!< clock division value */
    timer_parameters.repetitioncounter = 0;                   /*!< the counter repetition value */
    timer_init(TIMER5, &timer_parameters);


    nvic_irq_enable(TIMER5_DAC_IRQn, 3, 2);

    timer_interrupt_enable(TIMER5, TIMER_INT_UP);

    timer_enable(TIMER5);
}

void Timer2_Init(uint16_t period, uint16_t prescaler) {

    rcu_periph_clock_enable(RCU_TIMER2);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    //初始化TIMER5
    timer_parameter_struct timer_parameters;
    timer_parameters.period = period - 1;                     /*!< period value */
    timer_parameters.prescaler = prescaler - 1;               /*!< prescaler value */
    timer_parameters.alignedmode = TIMER_COUNTER_EDGE;        /*!< aligned mode */
    timer_parameters.counterdirection = TIMER_COUNTER_UP;     /*!< counter direction */
    timer_parameters.clockdivision = TIMER_CKDIV_DIV1;        /*!< clock division value */
    timer_parameters.repetitioncounter = 0;                   /*!< the counter repetition value */
    timer_init(TIMER2, &timer_parameters);


    nvic_irq_enable(TIMER2_IRQn, 3, 2);

    timer_interrupt_enable(TIMER2, TIMER_INT_UP);

    timer_enable(TIMER2);
}


void TIMER5_DAC_IRQHandler(void) {
    if (timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP) == SET) {
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
        gpio_bit_toggle(LED1_PORT, LED1_PIN);
        printf("TIMER5 LED1 toggle status \n");
    }
}


void TIMER2_IRQHandler(void) {
    if (timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP) == SET) {
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
        gpio_bit_toggle(LED2_PORT, LED2_PIN);
        printf("TIMER5 LED2 toggle status \n");
    }
}