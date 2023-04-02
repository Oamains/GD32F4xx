//
// Created by Oamains on 2023/4/2.
//
#include <string.h>
#include "led_light_flow.h"

static uint8_t numbers = 0;

void Control_Led_state() {

    if (numbers == 1) {
        gpio_bit_toggle(LED1_PORT, LED1_PIN);
    } else if (numbers == 2) {
        gpio_bit_toggle(LED2_PORT, LED2_PIN);
    } else if (numbers == 3) {
        gpio_bit_toggle(LED3_PORT, LED3_PIN);
    } else if (numbers == 4) {
        gpio_bit_toggle(LED4_PORT, LED4_PIN);
    } else if (numbers > 4) {
        gpio_bit_toggle(LED1_PORT, LED1_PIN);
        gpio_bit_toggle(LED2_PORT, LED2_PIN);
        gpio_bit_toggle(LED3_PORT, LED3_PIN);
        gpio_bit_toggle(LED4_PORT, LED4_PIN);
        numbers = 0;
    }
    ++numbers;
}

void Usart_Switch_Led_State(uint8_t *led) {
    if (strcmp((const char *) led, "led1 on") == 0){
        if (gpio_input_bit_get(LED1_PORT, LED1_PIN) == RESET) {
            gpio_bit_toggle(LED1_PORT, LED1_PIN);
        }
    }

    if (strcmp((const char *) led, "led2 on") == 0){
        if (gpio_input_bit_get(LED2_PORT, LED2_PIN) == RESET) {
            gpio_bit_toggle(LED2_PORT, LED2_PIN);
        }
    }

    if (strcmp((const char *) led, "led3 on") == 0){
        if (gpio_input_bit_get(LED3_PORT, LED3_PIN) == RESET) {
            gpio_bit_toggle(LED3_PORT, LED3_PIN);
        }
    }

    if (strcmp((const char *) led, "led1 off") == 0){
        if (gpio_input_bit_get(LED1_PORT, LED1_PIN) == SET) {
            gpio_bit_toggle(LED1_PORT, LED1_PIN);
        }
    }

    if (strcmp((const char *) led, "led2 off") == 0){
        if (gpio_input_bit_get(LED2_PORT, LED2_PIN) == SET) {
            gpio_bit_toggle(LED2_PORT, LED2_PIN);
        }
    }

    if (strcmp((const char *) led, "led3 off") == 0){
        if (gpio_input_bit_get(LED3_PORT, LED3_PIN) == SET) {
            gpio_bit_toggle(LED3_PORT, LED3_PIN);
        }
    }
}