/**
 * @file rt_pin.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief GPIO控制通用接口
 * @version 0.1
 * @date 2023-04-25
 * @note 使用说明
 * 
 *  // 设置1脚为输出引脚，并设置为高电平
 *  struct rt_device* dev = device_find("PIN");
 *  pin_mode(dev, 1, PIN_MODE_OUTPUT);
 *  pin_write(dev, 1, PIN_HIGH);
 *  
 *  // 设置2脚为上拉输入引脚，读取电平，设置上升沿中断
 *  pin_mode(dev, 2, PIN_MODE_INPUT_PULLUP);
 *  rt_int32_t ret = pin_read(dev, 2);
 *  pin_irq_enable(dev, 2, PIN_IRQ_MODE_RISING, pin_irq_callback, NULL);
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 引脚输入/输出模式
 *
 */
enum {
    PIN_MODE_DISABLE, // 禁用
    PIN_MODE_OUTPUT, // 推挽输出
    PIN_MODE_OUTPUT_OD, // 开漏输出
    PIN_MODE_INPUT, // 浮空输入
    PIN_MODE_INPUT_PULLUP, // 上拉输入
    PIN_MODE_INPUT_PULLDN, // 下拉输入
    PIN_MODE_DLPS, // 低功耗模式
};

/**
 * @brief 引脚电平值
 *
 */
enum {
    PIN_LOW, // 低电平
    PIN_HIGH, // 高电平
};

/**
 * @brief 引脚中断模式
 *
 */
enum {
    PIN_IRQ_MODE_DISABLE, // 禁用
    PIN_IRQ_MODE_RISING, // 上升沿
    PIN_IRQ_MODE_FALLING, // 下降沿
    PIN_IRQ_MODE_CHANGE, // 上升/下降沿
    PIN_IRQ_MODE_HIGH, // 高电平
    PIN_IRQ_MODE_LOW, // 低电平
};

/**
 * @brief PIN设备模型
 * 
 */
struct rt_pin_ops;
struct rt_device_pin {
    struct rt_device dev;
    const struct rt_pin_ops* ops;
};

struct rt_pin_ops {
    rt_err_t (*pin_mode)(const struct rt_device* dev, rt_base_t pin, rt_uint8_t mode);
    void (*pin_write)(const struct rt_device* dev, rt_base_t pin, rt_uint8_t val);
    rt_base_t (*pin_read)(const struct rt_device* dev, rt_base_t pin);
    rt_err_t (*pin_enable_irq)(const struct rt_device* dev, rt_base_t pin, rt_uint8_t mode,
        void (*hdr)(void* args), void* args);
    void (*pin_disable_irq)(const struct rt_device* dev, rt_base_t pin);
};

/**
 * @brief GPIO引脚输入输出配置
 *
 * @param pin   编号
 * @param mode  模式
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t pin_mode(const struct rt_device* dev, rt_base_t pin, rt_uint8_t mode);

/**
 * @brief 引脚电平控制
 *
 * @param pin   编号
 * @param val   电平
 */
void pin_write(const struct rt_device* dev, rt_base_t pin, rt_uint8_t val);

/**
 * @brief 获取引脚电平状态
 *
 * @param pin   编号
 * @return 低电平PIN_LOW，高电平PIN_HIGH
 */
rt_base_t pin_read(const struct rt_device* dev, rt_base_t pin);

/**
 * @brief 设置引脚中断处理函数
 *
 * @param pin       编号
 * @param mode      中断模式
 * @param hdr       中断处理函数
 * @param args      传递给中断处理函数的参数
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t pin_enable_irq(const struct rt_device* dev, rt_base_t pin, rt_uint8_t mode,
    void (*hdr)(void* args), void* args);

/**
 * @brief 取消引脚中断处理函数
 *
 * @param pin       编号
 */
void pin_disable_irq(const struct rt_device* dev, rt_base_t pin);

/**
 * @brief 注册PIN设备
 * 
 * @param pin    PIN设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_pin_register(struct rt_device_pin* pin, const char* name, const struct rt_pin_ops* ops, void* user_data);

#ifdef __cplusplus
}
#endif
