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

#define DIR_PIN 13
#define STEP_CHANNEL 2

#define SW1_PIN 12
#define SW2_PIN 11
#define PB1_PIN 17
#define PB2_PIN 16

static rt_device_t _tim3;
static rt_device_t _pin;
static rt_timer_t _timer_keyscan;
static volatile int32_t _step = 0;
static volatile int32_t cnt = 0;

static struct button btn_sw1;
static struct button btn_sw2;
static struct button btn_pb1;
static struct button btn_pb2;

void tim_cbk(const struct rt_device* dev, void* arg, rt_base_t event)
{
    if (event == TIM_EVENT_OVERFLOW) {
        cnt++;
        _step--;
        if (_step == 0) {
            tim_stop(_tim3);
        }
    }
}

void motor_step(int32_t step)
{
    tim_stop(_tim3);
    if (step < 0) {
        pin_write(_pin, DIR_PIN, PIN_LOW);
        _step = (0 - step) * 2;
    } else if (step > 0) {
        pin_write(_pin, DIR_PIN, PIN_HIGH);
        _step = step * 2;
    } else {
        _step = 0;
        return;
    }
    tim_start(_tim3);
}

static void key_scan(void* arg)
{
    button_scan(&btn_sw1);
    button_scan(&btn_sw2);
    button_scan(&btn_pb1);
    button_scan(&btn_pb2);
}

void sw1_cbk(void* arg, rt_uint8_t event) {
    tim_stop(_tim3);
}

void sw2_cbk(void* arg, rt_uint8_t event) {
    
}

void pb1_cbk(void* arg, rt_uint8_t event) {
    motor_step(-1000);
}

void pb2_cbk(void* arg, rt_uint8_t event) {
    
}

int main(void)
{
    // 200ms
    rt_uint32_t period_ns = 200 * 1000 * 1000;

    _tim3 = rt_device_find("tim3");
    _pin = rt_device_find("pin");
    _timer_keyscan = rt_timer_create("timer1", key_scan,
        RT_NULL, 2,
        RT_TIMER_FLAG_PERIODIC);

    button_init(&btn_sw1, "pin", SW1_PIN, PIN_LOW);
    button_attach(&btn_sw1, BTN_EVENT_DN, sw1_cbk, RT_NULL);

    button_init(&btn_sw2, "pin", SW2_PIN, PIN_LOW);
    button_attach(&btn_sw2, BTN_EVENT_DN, sw2_cbk, RT_NULL);

    button_init(&btn_pb1, "pin", PB1_PIN, PIN_LOW);
    button_attach(&btn_pb1, BTN_EVENT_DN, pb1_cbk, RT_NULL);

    button_init(&btn_pb2, "pin", PB2_PIN, PIN_LOW);
    button_attach(&btn_pb2, BTN_EVENT_DN, pb2_cbk, RT_NULL);

    pin_mode(_pin, DIR_PIN, PIN_MODE_OUTPUT);
    pin_write(_pin, DIR_PIN, PIN_LOW);

    tim_open(_tim3, period_ns);
    tim_set_overflow_callback(_tim3, tim_cbk, NULL);

    struct rt_tim_config config = {
        .mode = TIM_MODE_TOGGLE,
    };

    tim_config(_tim3, STEP_CHANNEL, &config);
    tim_pwm_set_pulse(_tim3, STEP_CHANNEL, 10);
    //tim_start(_tim3);

    /* 启动定时器1 */
    if (_timer_keyscan != RT_NULL) {
        rt_timer_start(_timer_keyscan);
    }

    motor_step(1234);

    return 0;
}
