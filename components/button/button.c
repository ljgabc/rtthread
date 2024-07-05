/**
 * @file button.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief  简单的按键状态处理
 * @version 0.1
 * @date 2024-07-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "components/button/button.h"

#ifdef CONFIG_RT_USING_HEAP
/**
 * 创建一个按键控制器
 *
 * @param pin_dev_name  PIN设备名称
 * @param pin          引脚编号
 * @param active_level  按下是高PIN_HIGH还是低PIN_LOW
 * @return button控制器
 */
struct button* button_create(const char* pin_dev_name, rt_base_t pin, rt_uint8_t active_level)
{
    struct button* btn = rt_malloc(sizeof(struct button));
    if (btn != RT_NULL) {
        button_init(btn, pin_dev_name, pin, active_level);
    }
    return RT_NULL;
}
#endif

/**
 * 按键控制器初始化
 *
 * @param btn           按键控制器
 * @param pin_dev_name  PIN设备名称
 * @param pin          引脚编号
 * @param active_level  按下是高PIN_HIGH还是低PIN_LOW
 */
void button_init(struct button* btn, const char* pin_dev_name, rt_base_t pin, rt_uint8_t active_level)
{
    btn->pin_dev = (struct rt_device_pin*)rt_device_find(pin_dev_name);
    btn->pin = pin;
    btn->active_level = active_level;

    if (active_level == PIN_LOW) {
        pin_mode(btn->pin_dev, btn->pin, PIN_MODE_INPUT_PULLUP);
        btn->level_state = 0xFF;
        btn->btn_state = 0xFF;
    } else {
        pin_mode(btn->pin_dev, btn->pin, PIN_MODE_INPUT_PULLDN);
        btn->level_state = 0x00;
        btn->btn_state = 0x00;
    }
    for (uint8_t i = 0; i < BTN_EVENT_NONE; i++) {
        btn->cbk[i] = RT_NULL;
    }
    btn->last_tick = rt_tick_get();
}

/**
 * 按键扫描，定时10ms~20ms调用
 *
 * @param btn           按键控制器
 */
void button_scan(struct button* btn)
{
    rt_int8_t event = -1;
    btn->level_state = btn->level_state << 1;
    if (pin_read(btn->pin_dev, btn->pin) == PIN_HIGH) {
        btn->level_state |= 0x01;
    }

    if (btn->level_state & 0x0F == 0x0C) {
        if (btn->active_level == PIN_LOW) {
            event = BTN_EVENT_DN;
        } else {
            event = BTN_EVENT_UP;
        }
        btn->press_tick == 0;
    } else if (btn->level_state & 0x0F == 0x03) {
        if (btn->active_level == PIN_LOW) {
            event = BTN_EVENT_UP;
        } else {
            event = BTN_EVENT_DN;
        }
        btn->press_tick == 0;
    } else if(btn->level_state & 0x0F == 0x00) {
        if (btn->active_level == PIN_LOW) {
            btn->press_tick++;
        }
    } else if(btn->level_state & 0x0F == 0x0F) {
        if (btn->active_level == PIN_HIGH) {
            btn->press_tick++;
        }
    }

    if (event != -1) {
        if (btn->cbk[event] != RT_NULL) {
            btn->cbk[event](btn->arg, event);
        }
    }

    if(btn->press_tick > 100) {
        if (btn->cbk[BTN_EVENT_LONG_PRESS] != RT_NULL) {
            btn->cbk[event](btn->arg, BTN_EVENT_LONG_PRESS);
        }
    }

}

/**
 * 注册事件回调函数
 *
 * @param btn           按键控制器
 * @param cbk           回调函数指针
 * @param event         事件
 */
void button_attach(struct button* btn, button_callback cbk, rt_uint8_t event)
{
    if (event >= BTN_EVENT_NONE) {
        return
    }
    btn->cbk[event] = cbk;
}
