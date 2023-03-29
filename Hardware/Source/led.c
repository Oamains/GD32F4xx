#include "../Include/led.h"


void Led_Init() {
    // enable the peripherals clock
    rcu_periph_clock_enable(RCU_GPIOA);
    // set GPIO mode
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
    // set GPIO output type and speed
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
}