//
// Created by Oamains on 2023/4/1.
//
#include "usart.h"

void Usart_Init(uint32_t baudval) {

    // enabled usart clock
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(GPIOA);
    // usart reuse function
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);
    // gpio mode
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    // gpio out speed
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    // config usart
    usart_deinit(USART0);
    usart_baudrate_set(USART0, baudval);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);

    // enabled usart 0
    usart_enable(USART0);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
//    usart_receive_config(USART0,);
}


void usart_send(uint8_t ucch) {
    usart_data_transmit(USART0, (uint8_t) ucch);
    // 等待缓冲区位置为空
    while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
}


void usart_str_send(uint8_t *ucstr) {

    while (ucstr && *ucstr) {
        usart_send(*ucstr++);
    }
}

int fputc(int ch, FILE *f) {
    usart_send(ch);
    return ch;
}

int _write(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        usart_send(*ptr++);
    }
    return len;
}