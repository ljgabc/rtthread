/**
 * @file kk_pin.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief GPIO控制通用接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "components/device/rt_pin.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief GPIO引脚输入输出配置
 *
 * @param pin   引脚编号
 * @param mode  输入输出模式
 * @return 成功返回OK，其他见错误码
 */
rt_err_t pin_mode(const struct rt_device* dev, rt_base_t pin, rt_uint8_t mode)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->mode(dev, pin, mode);
}

/**
 * @brief 引脚电平控制
 *
 * @param pin   引脚编号
 * @param val   引脚电平
 * @return 成功返回OK，其他见错误码
 */
void pin_write(const struct rt_device* dev, rt_base_t pin, rt_uint8_t val)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->write(dev, pin, val);
}

/**
 * @brief 获取引脚电平状态
 *
 * @param pin   引脚编号
 * @return 低电平PIN_LOW，高电平PIN_HIGH
 */
rt_base_t pin_read(const struct rt_device* dev, rt_base_t pin)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->read(dev, pin);
}

/**
 * @brief 设置引脚中断处理函数
 *
 * @param pin       引脚编号
 * @param mode      中断模式
 * @param hdr       中断处理函数
 * @param args      传递给中断处理函数的参数
 * @return 成功返回OK，其他见错误码
 */
rt_err_t pin_enable_irq(const struct rt_device* dev, rt_base_t pin, rt_uint8_t mode,
    void (*hdr)(void* args), const void* args)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->enable_irq(dev, pin, mode, hdr, args);
}

/**
 * @brief 取消引脚中断处理函数
 *
 * @param pin       引脚编号
 * @return 成功返回OK，其他见错误码
 */
void pin_disable_irq(const struct rt_device* dev, rt_base_t pin)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->disable_irq(dev, pin);
}

#ifdef __cplusplus
}
#endif
