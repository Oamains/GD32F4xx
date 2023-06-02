//
// Created by Oamains on 2023/4/1.
//
#include "pwm_led.h"

static void Pwm_Led_Init() {
    // enable the peripherals clock
    rcu_periph_clock_enable(PWM_LED_RCU);
    // set GPIO mode
    gpio_mode_set(PWM_LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_LED_PIN);
    // set GPIO output type and speed
    gpio_output_options_set(PWM_LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_LED_PIN);
    gpio_af_set(PWM_LED_PORT, PWM_LED_AF, PWM_LED_PIN);
}

void Pwm_Timer_Init(uint16_t prescaler, uint16_t period) {
    timer_parameter_struct timer_init_parameters;
    timer_oc_parameter_struct timer_oc_int_para;

    Pwm_Led_Init();

    rcu_periph_clock_enable(PWM_TIMER_RCU);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(PWM_TIMER);
    timer_init_parameters.prescaler = prescaler - 1;               /*!< prescaler value */
    timer_init_parameters.alignedmode = TIMER_COUNTER_EDGE;        /*!< aligned mode */
    timer_init_parameters.counterdirection = TIMER_COUNTER_UP;     /*!< counter direction */
    timer_init_parameters.clockdivision = TIMER_CKDIV_DIV1;        /*!< clock division value */
    timer_init_parameters.period = period - 1;                     /*!< period value */
    timer_init_parameters.repetitioncounter = 0;                   /*!< the counter repetition value */
    timer_init(PWM_TIMER, &timer_init_parameters);


    timer_oc_int_para.ocpolarity = TIMER_OC_POLARITY_HIGH;                          /*!< channel output polarity */
    timer_oc_int_para.outputstate = TIMER_CCX_ENABLE;                               /*!< channel output state */
    timer_channel_output_config(PWM_TIMER, PWM_TIMER_CHANNEL, &timer_oc_int_para);

    timer_channel_output_pulse_value_config(PWM_TIMER, PWM_TIMER_CHANNEL, 0);
    timer_channel_output_mode_config(PWM_TIMER, PWM_TIMER_CHANNEL, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(PWM_TIMER, PWM_TIMER_CHANNEL, TIMER_OC_SHADOW_DISABLE);

    timer_auto_reload_shadow_enable(PWM_TIMER);
    timer_enable(PWM_TIMER);
}

void Pwm_Breathing_Lamp(void) {
    static uint16_t value = 0;
    static uint8_t direct = 0;
    if (direct == 0) {
        value += 300;
        if (value > 10000)direct = 1;
    } else {
        value -= 300;
        if (value <= 0)direct = 0;
    }
    timer_channel_output_pulse_value_config(PWM_TIMER, PWM_TIMER_CHANNEL, value);
    delay_1ms(50);
//    printf("PWM LED4 Output Pulse Value:%d \n", value);
}

static uint8_t state = 1;

void EXTI0_IRQHandler(void) {
    if (exti_interrupt_flag_get(EXTI_0) == SET) {
        if (state) {
            timer_channel_output_state_config(PWM_TIMER,PWM_TIMER_CHANNEL,TIMER_CCX_DISABLE);
            timer_disable(PWM_TIMER);
            timer_flag_clear(PWM_TIMER,TIMER_FLAG_CH0);
            state = 0;
            if (gpio_input_bit_get(LED4_PORT, LED4_PIN) == SET) {
                gpio_bit_toggle(LED4_PORT, LED4_PIN);
            }
        } else {
            if (gpio_input_bit_get(LED4_PORT, LED4_PIN) == RESET) {
                gpio_bit_toggle(LED4_PORT, LED4_PIN);
            }
            Pwm_Timer_Init(200, 10000);
            timer_channel_output_state_config(PWM_TIMER,PWM_TIMER_CHANNEL,TIMER_CCX_ENABLE);
            state = 1;
        }
        exti_interrupt_flag_clear(EXTI_0);
    }
}