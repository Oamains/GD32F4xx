//
// Created by Oamains on 2023/4/1.
//
#include "pwm.h"

void Pwm_Timer_Init(uint16_t period, uint16_t prescaler) {
    rcu_periph_clock_enable(PWM_TIMER_RCU);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    //初始化TIMER5
    timer_parameter_struct timer_parameters;
    timer_parameters.period = period - 1;                     /*!< period value */
    timer_parameters.prescaler = prescaler - 1;               /*!< prescaler value */
    timer_parameters.alignedmode = TIMER_COUNTER_EDGE;        /*!< aligned mode */
    timer_parameters.counterdirection = TIMER_COUNTER_UP;     /*!< counter direction */
    timer_parameters.clockdivision = TIMER_CKDIV_DIV1;        /*!< clock division value */
    timer_parameters.repetitioncounter = 0;                   /*!< the counter repetition value */
    timer_init(PWM_TIMER, &timer_parameters);

    timer_oc_parameter_struct timer_out_parameters;
    timer_out_parameters.ocpolarity = TIMER_OC_POLARITY_HIGH;                          /*!< channel output polarity */
    timer_out_parameters.outputstate = TIMER_CCX_ENABLE;                               /*!< channel output state */
    timer_channel_output_config(PWM_TIMER, PWM_TIMER_CHANNEL, &timer_out_parameters);

    timer_channel_output_pulse_value_config(PWM_TIMER, PWM_TIMER_CHANNEL, 0);
    timer_channel_output_mode_config(PWM_TIMER, PWM_TIMER_CHANNEL, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(PWM_TIMER, PWM_TIMER_CHANNEL, TIMER_OC_SHADOW_DISABLE);

    timer_auto_reload_shadow_enable(PWM_TIMER);
    timer_enable(PWM_TIMER);
}


void Pwm_Breathing_Lamp(void) {
    static uint16_t value = 0;
    static uint8_t direct = 0;
    if (direct == 0) { // 逐渐变亮
        value += 300;
        if (value > 10000) direct = 1;
    } else { // 逐渐变暗
        value -= 300;
        if (value <= 0) direct = 0;
    }
    printf("PWM LED4 Output Pulse Value:%d \n", value);
    timer_channel_output_pulse_value_config(PWM_TIMER, PWM_TIMER_CHANNEL, value);
    delay_1ms(50);
}