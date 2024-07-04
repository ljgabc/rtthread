/**
 * @file main.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 夹爪控制，使用步仅电机
 * @version 0.1
 * @date 2024-07-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "rtdevice.h"
#include "rtthread.h"

void tim_cbk(const struct rt_device* dev, void* arg, rt_base_t event)
{
    //rt_uint32_t *cnt = (rt_uint32_t*)arg;
    //struct rt_device *tim3 = (struct rt_device*)arg;
    //if(event == TIM_EVENT_OVERFLOW) {
    //*cnt = *cnt + 1;
    //	if(cnt++ >= 4) {
    //		tim_stop(dev);
    //		tim1_cnt = tim_count(dev);
    //		tim3_cnt++;
    //	}
    //}
}

int main(void)
{
    //struct rt_device *tim1 = rt_device_find("tim1");
    struct rt_device* tim3 = rt_device_find("tim3");
    //struct rt_device *tim6 = rt_device_find("tim6");

    rt_uint32_t freq = 3000 * 1000;

    //tim_open(tim1, freq);
    tim_open(tim3, freq);
    //tim_open(tim6, 10000);

    //tim_set_overflow_callback(tim1, tim_cbk, (void *)&tim1_cnt);
    tim_set_overflow_callback(tim3, tim_cbk, (void*)tim3);
    //tim_set_overflow_callback(tim6, tim_cbk, (void *)&tim6_cnt);

    struct rt_tim_config config = {
        .mode = TIM_MODE_TOGGLE,
    };

    tim_config(tim3, 1, &config);
    tim_pwm_set_pulse(tim3, 1, freq / 2);

    config.mode = TIM_MODE_PWM;
    tim_config(tim3, 2, &config);
    tim_pwm_set_pulse(tim3, 2, freq / 3);

    //config.mode = TIM_MODE_TOGGLE;
    //config.pwm_config.complementary_en = TIM_PWM_COMPLEMENTARY;
    //config.pwm_config.dt_en = TIM_PWM_DT_ENABLE;

    //tim_config(tim1, 2, &config);
    //tim_pwm_set_pulse(tim1, 2, freq/2);
    //tim_start(tim1);

    //tim_pwm_set_deadtime(tim1, 2, 8000);

    tim_start(tim3);

    //tim_pwm_set_pulse(tim3, 1, freq/2);
    //tim_start(tim6);

    return 0;
}
