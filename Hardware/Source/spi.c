//
// Created by Oamains on 2023/5/13.
//
#include "spi.h"

void Spi_Init() {
    spi_parameter_struct spi_init_struct;
    rcu_periph_clock_enable(RCU_GPIOF); // 使用F端口
    rcu_periph_clock_enable(RCU_SPI4);  // 使能SPI4
    /* configure SPI4 GPIO */
    gpio_af_set(GPIOF, GPIO_AF_5, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
    gpio_bit_set(GPIOF, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
    /* configure SPI4 parameter */
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX; // 传输模式全双工
    spi_init_struct.device_mode = SPI_MASTER;              // 配置为主机
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;       // 8位数据
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT; // 软件cs
    // spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.prescale = SPI_PSC_2;
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI4, &spi_init_struct);
    /* enable SPI */
    spi_enable(SPI4);
}

void Spi_Write(uint8_t dat) {
    while (RESET == spi_i2s_flag_get(SPI4, SPI_FLAG_TBE));
    spi_i2s_data_transmit(SPI4, dat);
    while (RESET == spi_i2s_flag_get(SPI4, SPI_FLAG_TBE));
}

uint8_t Spi_Read(void) {
    while (RESET == spi_i2s_flag_get(SPI4, SPI_FLAG_RBNE));
    return spi_i2s_data_receive(SPI4);
    while (RESET == spi_i2s_flag_get(SPI4, SPI_FLAG_RBNE));
}


void SPI1_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_SPI1);

    /* SPI1 GPIO config:SCK/PB13, MISO/PB13, MOSI/PB15 */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

    spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI1);
    spi_struct_para_init(&spi_init_struct);

    /* SPI1 parameter config */
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_2;
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);

    spi_nss_internal_high(SPI1);
    spi_enable(SPI1);

    SPI1_DMA_Init();
}

uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
    //uint8_t retry=0;
    while (spi_i2s_flag_get(SPI1, SPI_FLAG_TBE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        //retry++;
        //if(retry>200)return 0;
    }
    spi_i2s_data_transmit(SPI1, (uint16_t)TxData); //通过外设SPIx发送一个数据
    //retry=0;

    while (spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        //retry++;
        //if(retry>200)return 0;
    }
    return spi_i2s_data_receive(SPI1); //返回通过SPIx最近接收的数据
}


void SPI1_DMA_Init(void) // SPI1-TX->DMA0-CH4
{
    dma_parameter_struct dma_init_struct;
    rcu_periph_clock_enable(RCU_DMA0); /* enable DMA clock */

    dma_channel_disable(DMA0, DMA_CH4);
    dma_deinit(DMA0, DMA_CH4);
    dma_struct_para_init(&dma_init_struct);

    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;      //内存到外设
    dma_init_struct.memory_addr = 0;                           //内存基地址,先不写
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_DISABLE;  //内存地址不变
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;      //八位数据
    dma_init_struct.number = 0;                                //数据量，先不写
    dma_init_struct.periph_addr = (uint32_t)(&SPI_DATA(SPI1)); //外设地址
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;  //外设地址不变
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH; //优先级
    dma_init(DMA0, DMA_CH4, &dma_init_struct);

    /* configure DMA mode */
    dma_channel_disable(DMA0, DMA_CH4);
    dma_circulation_disable(DMA0, DMA_CH4);
    dma_memory_to_memory_disable(DMA0, DMA_CH4);
    // dma_flag_clear(DMA0,DMA_CH4,DMA_FLAG_FTF);

    // spi_dma_enable(SPI0, SPI_DMA_TRANSMIT)
}

void SPI1_DMA_Transmit(const void* srcaddr, uint8_t datawidth, uint8_t srcaddrinc, uint32_t datanum)
{
    spi_disable(SPI1);
    spi_dma_disable(SPI1, SPI_DMA_TRANSMIT);
    dma_channel_disable(DMA0, DMA_CH4);
    if (datawidth == 16) // 16位数据传输
    {
        spi_i2s_data_frame_format_config(SPI1, SPI_FRAMESIZE_16BIT);
        dma_memory_width_config(DMA0, DMA_CH4, DMA_MEMORY_WIDTH_16BIT); //内存16位
        dma_periph_width_config(DMA0, DMA_CH4, DMA_MEMORY_WIDTH_16BIT); //外设16位
    }
    else if (datawidth == 8) //八位数据传输
    {
        spi_i2s_data_frame_format_config(SPI1, SPI_FRAMESIZE_8BIT);
        dma_memory_width_config(DMA0, DMA_CH4, DMA_MEMORY_WIDTH_8BIT); //内存8位
        dma_periph_width_config(DMA0, DMA_CH4, DMA_MEMORY_WIDTH_8BIT); //外设8位
    }
    if (srcaddrinc)//如果数据地址递增
        dma_memory_increase_enable(DMA0, DMA_CH4);//开启数据地址递增
    else
        dma_memory_increase_disable(DMA0, DMA_CH4);
    dma_memory_address_config(DMA0, DMA_CH4, (uint32_t)srcaddr);//配置源地址
    dma_transfer_number_config(DMA0, DMA_CH4, datanum);//配置数据量
    dma_channel_enable(DMA0, DMA_CH4);
    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
    spi_enable(SPI1); //开启传输

    while (1)
    {
        if (dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF))
        {
            dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_G);
            break;
        }
        //printf("transfer_number_last:%d\r\n", dma_transfer_number_get(DMA0, DMA_CH4)); //打印剩余数据量
    }
    spi_i2s_data_frame_format_config(SPI1, SPI_FRAMESIZE_8BIT); //恢复
    spi_enable(SPI1);
    while (spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
    //spi_i2s_data_receive(SPI1); //返回通过SPIx最近接收的数据
}