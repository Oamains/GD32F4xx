//
// Created by Oamains on 2023/4/1.
//
#include "led.h"


void Led_Init() {
    // enable the peripherals clock
    rcu_periph_clock_enable(LED1_RCU_PORT);
    // set GPIO mode
    gpio_mode_set(LED1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_PIN);
    // set GPIO output type and speed
    gpio_output_options_set(LED1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_PIN);


    // enable the peripherals clock
    rcu_periph_clock_enable(LED2_RCU_PORT);
    // set GPIO mode
    gpio_mode_set(LED2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED2_PIN);
    // set GPIO output type and speed
    gpio_output_options_set(LED2_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED2_PIN);

    // enable the peripherals clock
    rcu_periph_clock_enable(LED3_RCU_PORT);
    // set GPIO mode
    gpio_mode_set(LED3_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED3_PIN);
    // set GPIO output type and speed
    gpio_output_options_set(LED3_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED3_PIN);
}