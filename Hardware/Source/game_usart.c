//
// Created by Oamains on 2023/4/1.
//
#include "game_usart.h"

uint8_t receive_buff_cache[USART_RECEIVE_SIZE];
uint16_t receive_size = 0;
uint8_t receive_complete = 0;

void Usart_Send_Init(uint32_t baudval) {
    // enabled usart clock
    rcu_periph_clock_enable(GAME_USART_RCU);
    rcu_periph_clock_enable(GAME_USART_PORT);
    // usart reuse function
    gpio_af_set(GAME_USART_PORT, GPIO_AF_7, GAME_USART_TX);
    gpio_af_set(GAME_USART_PORT, GPIO_AF_7, GAME_USART_RX);
    // gpio mode
    gpio_mode_set(GAME_USART_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GAME_USART_TX);
    gpio_mode_set(GAME_USART_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GAME_USART_RX);
    // gpio out speed
    gpio_output_options_set(GAME_USART_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GAME_USART_TX);
    gpio_output_options_set(GAME_USART_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GAME_USART_RX);

    // config usart
    usart_deinit(GAME_USART);
    usart_baudrate_set(GAME_USART, baudval);
    usart_parity_config(GAME_USART, USART_PM_NONE);
    usart_word_length_set(GAME_USART, USART_WL_8BIT);
    usart_stop_bit_set(GAME_USART, USART_STB_1BIT);

    // enabled usart 0
    usart_enable(GAME_USART);
    usart_transmit_config(GAME_USART, USART_TRANSMIT_ENABLE);
    usart_receive_config(GAME_USART, USART_RECEIVE_ENABLE);
    // config usart irq
    nvic_irq_enable(GAME_USART_IRQ, 2U, 2U);
//    usart_interrupt_enable(USART0, USART_INT_RBNE);
    usart_interrupt_enable(GAME_USART, USART_INT_IDLE);
    Usart_Dma_Init();
}


void Usart_Dma_Init() {
    dma_single_data_parameter_struct dma_parameters;

    rcu_periph_clock_enable(GAME_DMA_RCU);
    dma_deinit(GAME_DMA, GAME_DMA_CHANNEL);

    dma_parameters.periph_addr = (uint32_t) &USART_DATA(GAME_USART);              /*!< peripheral base address */
    dma_parameters.periph_inc = DMA_PERIPH_INCREASE_DISABLE;                      /*!< peripheral increasing mode */
    dma_parameters.memory0_addr = (uint32_t) &receive_buff_cache;                 /*!< memory 0 base address */
    dma_parameters.memory_inc = DMA_MEMORY_INCREASE_ENABLE;                       /*!< memory increasing mode */
    dma_parameters.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;                   /*!< transfer data size of peripheral */
    dma_parameters.circular_mode = DMA_CIRCULAR_MODE_DISABLE;                     /*!< DMA circular mode */
    dma_parameters.direction = DMA_PERIPH_TO_MEMORY;                              /*!< channel data transfer direction */
    dma_parameters.number = USART_RECEIVE_SIZE;                                   /*!< channel transfer number */
    dma_parameters.priority = DMA_PRIORITY_ULTRA_HIGH;                            /*!< channel priority level */
    dma_single_data_mode_init(GAME_DMA, GAME_DMA_CHANNEL, &dma_parameters);

    dma_channel_subperipheral_select(GAME_DMA, GAME_DMA_CHANNEL, DMA_SUBPERI4);
    dma_channel_enable(GAME_DMA, GAME_DMA_CHANNEL);
    dma_interrupt_enable(GAME_DMA, GAME_DMA_CHANNEL, DMA_CHXCTL_FTFIE);
    nvic_irq_enable(GAME_DMA_IQR, 2, 1);
    // enabled usart dma receive
    usart_dma_receive_config(GAME_USART, USART_DENR_ENABLE);
}

void usart_send(uint8_t ucch) {
    usart_data_transmit(GAME_USART, (uint8_t) ucch);
    // 等待缓冲区位置为空
    while (usart_flag_get(GAME_USART, USART_FLAG_TBE) == RESET);
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

int _write(int file, char *ptr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        usart_send(*ptr++);
    }
    return len;
}

void GAME_DMA_IQR_HANDLER(void) {
    if (dma_interrupt_flag_get(GAME_DMA, GAME_DMA_CHANNEL, DMA_INT_FLAG_FTF) == SET) {
        dma_interrupt_flag_clear(GAME_DMA, GAME_DMA_CHANNEL, DMA_INT_FLAG_FTF);
    }
}

void GAME_USART_IRQ_HANDLER(void) {
    if (usart_interrupt_flag_get(GAME_USART, USART_INT_FLAG_IDLE) == SET) {
        usart_data_receive(GAME_USART);
        receive_size = USART_RECEIVE_SIZE - dma_transfer_number_get(GAME_DMA, GAME_DMA_CHANNEL);
        receive_buff_cache[receive_size] = '\0';
        receive_complete = 1;
        dma_channel_disable(GAME_DMA, GAME_DMA_CHANNEL);
        Usart_Dma_Init();
    }
}
