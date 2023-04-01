#include "led_key.h"


void Key_Init() {

    rcu_periph_clock_enable(RCU_GPIOA);
    //
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);
    // set GPIO output type and speed
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

}

void Key_Nvic_Key_Init() {
    // 1. enabled GPIO clock
    rcu_periph_clock_enable(RCU_GPIOA);
    // 2. enabled sys config clock
    rcu_periph_clock_enable(RCU_SYSCFG);
    // set gpio made
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);
    // set config irq level
    nvic_irq_enable(EXTI0_IRQn, 3U, 3U);
    // set nvic line connect
    syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN0);
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_enable(EXTI_0);
    exti_interrupt_flag_clear(EXTI_0);
}

void Press_Key_Scan() {
    if (gpio_input_bit_get(GPIOA, GPIO_PIN_0) == SET) {
        delay_1ms(30);
        if (gpio_input_bit_get(GPIOA, GPIO_PIN_0) == SET) {
            gpio_bit_toggle(GPIOA, GPIO_PIN_5);
            while (gpio_input_bit_get(GPIOA, GPIO_PIN_0) == SET);
        }
    }
}


void EXTI0_IRQHandler(void) {
    if (exti_interrupt_flag_get(EXTI_0) == SET) {
        if (gpio_input_bit_get(GPIOA, GPIO_PIN_0) == SET) {
            gpio_bit_toggle(GPIOA, GPIO_PIN_5);
            printf("key press \n");
        }else {
            printf("key release \n");
        }
        exti_interrupt_flag_clear(EXTI_0);
    }
}