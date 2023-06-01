//
// Created by Oamains on 2023/5/13.
//
#include "spi.h"

void Spi2_Init(void) {
    spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(RCU_GPIOF); // 使用F端口
    rcu_periph_clock_enable(RCU_SPI2);  // 使能SPI2
    /* configure SPI2 GPIO */
    gpio_af_set(GPIOB, GPIO_AF_5, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    gpio_bit_set(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    spi_i2s_deinit(SPI2);
    /* configure SPI2 parameter */
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX; // 传输模式全双工
    spi_init_struct.device_mode = SPI_MASTER;              // 配置为主机
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;       // 8位数据
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT; // 软件cs
    // spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.prescale = SPI_PSC_2;

    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI2, &spi_init_struct);

//    spi_nss_internal_high(SPI2);
    /* enable SPI */
    spi_enable(SPI2);

    SPI2_DMA_Init();
}

uint8_t SPI2_ReadWriteByte(uint8_t TxData) {
    //uint8_t retry=0;
    while (spi_i2s_flag_get(SPI2, SPI_FLAG_TBE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        //retry++;
        //if(retry>200)return 0;
    }
    spi_i2s_data_transmit(SPI2, (uint16_t) TxData); //通过外设SPIx发送一个数据
    //retry=0;

    while (spi_i2s_flag_get(SPI2, SPI_FLAG_RBNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        //retry++;
        //if(retry>200)return 0;
    }
    return spi_i2s_data_receive(SPI2); //返回通过SPIx最近接收的数据
}

// SPI2-TX->DMA0-CH4
void SPI2_DMA_Init(void) {
    dma_multi_data_parameter_struct dma_init_parameter;
    /* peripheral clock enable */
    rcu_periph_clock_enable(RCU_DMA1);
    /* DMA peripheral configure */
    dma_deinit(SPI2, DMA_CH0);
    dma_init_parameter.periph_addr = (uint32_t) (&SPI_DATA(SPI2));
    dma_init_parameter.periph_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_parameter.memory0_addr = 0;
    dma_init_parameter.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_parameter.memory_burst_width = DMA_MEMORY_BURST_4_BEAT;
    dma_init_parameter.periph_burst_width = DMA_PERIPH_BURST_4_BEAT;
    dma_init_parameter.critical_value = DMA_FIFO_2_WORD;
    dma_init_parameter.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_init_parameter.direction = DMA_MEMORY_TO_MEMORY;
    dma_init_parameter.number = 0;
    dma_init_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_multi_data_mode_init(DMA1, DMA_CH0, &dma_init_parameter);
    dma_channel_enable(DMA1, DMA_CH0);
}

void SPI2_DMA_Transmit(const void *srcaddr, uint8_t datawidth, uint8_t srcaddrinc, uint32_t datanum) {
    spi_disable(SPI2);
    spi_dma_disable(SPI2, SPI_DMA_TRANSMIT);
    dma_channel_disable(DMA0, DMA_CH4);
    if (datawidth == 16) // 16位数据传输
    {
        spi_i2s_data_frame_format_config(SPI2, SPI_FRAMESIZE_16BIT);
        dma_memory_width_config(DMA0, DMA_CH4, DMA_MEMORY_WIDTH_16BIT); //内存16位
        dma_periph_width_config(DMA0, DMA_CH4, DMA_MEMORY_WIDTH_16BIT); //外设16位
    } else if (datawidth == 8) //八位数据传输
    {
        spi_i2s_data_frame_format_config(SPI2, SPI_FRAMESIZE_8BIT);
        dma_memory_width_config(DMA0, DMA_CH4, DMA_MEMORY_WIDTH_8BIT); //内存8位
        dma_periph_width_config(DMA0, DMA_CH4, DMA_MEMORY_WIDTH_8BIT); //外设8位
    }
    if (srcaddrinc)//如果数据地址递增
        //开启数据地址递增
        dma_memory_address_generation_config(DMA0, DMA_CH4, DMA_MEMORY_INCREASE_ENABLE);
    else
        dma_memory_address_generation_config(DMA0, DMA_CH4, DMA_MEMORY_INCREASE_DISABLE);
    dma_memory_address_config(DMA0, DMA_CH4, DMA_MEMORY_0, (uint32_t) srcaddr);//配置源地址
    dma_transfer_number_config(DMA0, DMA_CH4, datanum);//配置数据量
    dma_channel_enable(DMA0, DMA_CH4);
    spi_dma_enable(SPI2, SPI_DMA_TRANSMIT);
    spi_enable(SPI2); //开启传输

    while (1) {
        if (dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF)) {
            dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_FTF);
            break;
        }
        //printf("transfer_number_last:%d\r\n", dma_transfer_number_get(DMA0, DMA_CH4)); //打印剩余数据量
    }
    spi_i2s_data_frame_format_config(SPI2, SPI_FRAMESIZE_8BIT); //恢复
    spi_enable(SPI2);
    while (spi_i2s_flag_get(SPI2, SPI_FLAG_RBNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
    //spi_i2s_data_receive(SPI2); //返回通过SPIx最近接收的数据
}