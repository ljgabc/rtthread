/**
 * @file button.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief  简单的按键扫描处理程序，可以设置事件回调函数
 * @version 0.1
 * @date 2024-07-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "components/device/rt_pin.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*button_callback)(void*, rt_uint8_t);

enum {
    BTN_EVENT_DN = 0, // 按下
    BTN_EVENT_UP, // 释放
    BTN_EVENT_LONG_PRESS, // 长按
    BTN_EVENT_NONE,
};

struct button {
    struct rt_device_pin* pin_dev;
    rt_base_t pin;
    button_callback cbk[BTN_EVENT_NONE];
    void* arg;
    rt_tick_t press_tick;
    rt_tick_t release_tick;
    rt_uint8_t active_level;
    rt_uint8_t btn_state;
    rt_uint8_t level_state;
};

#ifdef CONFIG_RT_USING_HEAP
/**
 * 创建一个按键控制器
 *
 * @param pin_dev_name  PIN设备名称
 * @param pin          引脚编号
 * @param active_level  按下是高PIN_HIGH还是低PIN_LOW
 * @return button控制器
 */
struct button* button_create(const char* pin_dev_name, rt_base_t pin, rt_uint8_t active_level);
#endif

/**
 * 按键控制器初始化
 *
 * @param btn           按键控制器
 * @param pin_dev_name  PIN设备名称
 * @param pin          引脚编号
 * @param active_level  按下是高PIN_HIGH还是低PIN_LOW
 */
void button_init(struct button* btn, const char* pin_dev_name, rt_base_t pin, rt_uint8_t active_level);

/**
 * 注册事件回调函数
 *
 * @param btn           按键控制器
 * @param cbk           回调函数指针
 * @param event         事件
 */
void button_attach(struct button* btn, rt_uint8_t event, button_callback cbk, void* arg);

/**
 * 按键扫描，定时10ms~20ms调用
 *
 * @param btn           按键控制器
 */
void button_scan(struct button* btn);

#ifdef __cplusplus
}
#endif
