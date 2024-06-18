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
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t pin_mode(const struct rt_device* dev, rt_base_t pin, rt_uint8_t mode)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->ops->pin_mode(dev, pin, mode);
}

/**
 * @brief 引脚电平控制
 *
 * @param pin   引脚编号
 * @param val   引脚电平
 */
void pin_write(const struct rt_device* dev, rt_base_t pin, rt_uint8_t val)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->ops->pin_write(dev, pin, val);
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
    return pin_dev->ops->pin_read(dev, pin);
}

/**
 * @brief 设置引脚中断处理函数
 *
 * @param pin       引脚编号
 * @param mode      中断模式
 * @param hdr       中断处理函数
 * @param args      传递给中断处理函数的参数
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t pin_enable_irq(const struct rt_device* dev, rt_base_t pin, rt_uint8_t mode,
    void (*hdr)(void* args), void* args)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->ops->pin_enable_irq(dev, pin, mode, hdr, args);
}

/**
 * @brief 取消引脚中断处理函数
 *
 * @param pin       引脚编号
 */
void pin_disable_irq(const struct rt_device* dev, rt_base_t pin)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;
    return pin_dev->ops->pin_disable_irq(dev, pin);
}

/**
 * @brief 读引脚状态
 * 
 * @param dev   设备
 * @param pos   引脚编号
 * @param buffer    目的缓冲区地址
 * @param size  缓冲区大小
 * @return size 
 */
static rt_ssize_t _pin_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_ssize_t size)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;

    rt_base_t pin = pos;

    for (rt_ssize_t i = 0; i < size; i++) {
        ((rt_base_t*)buffer)[i] = pin_read(dev, pin);
    }

    return size;
}

/**
 * @brief 写引脚
 * 
 * @param dev       设备
 * @param pos       引脚编号
 * @param buffer    引脚电平缓冲区
 * @param size      缓冲区大小
 * @return size 
 */
static rt_ssize_t _pin_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_ssize_t size)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;

    rt_base_t pin = pos;
    rt_uint8_t val;

    for (rt_ssize_t i = 0; i < size; i++) {
        val = ((rt_base_t*)buffer)[i];
        pin_write(dev, pin, (val == 0) ? PIN_LOW : PIN_HIGH);
    }

    return size;
}

/**
 * @brief 引脚模式控制
 * 
 * @param dev       设备
 * @param cmd       引脚编号
 * @param args      模式缓冲区
 * @return rt_err_t 
 */
static rt_err_t _pin_control(rt_device_t dev, int cmd, void* args)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_pin* pin_dev = (struct rt_device_pin*)dev;

    rt_base_t pin = cmd;

    rt_base_t mode = ((rt_base_t*)args)[0];

    return pin_mode(dev, pin, mode);
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops pin_ops = {
    RT_NULL,
    RT_NULL,
    RT_NULL,
    _pin_read,
    _pin_write,
    _pin_control,
};
#endif

/**
 * @brief 注册PIN设备
 * 
 * @param pin    PIN设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_pin_register(struct rt_device_pin* pin, const char* name, const struct rt_pin_ops* ops, void* user_data)
{
    RT_ASSERT(pin != RT_NULL);
    pin->dev.type = RT_Device_Class_Pin;
    pin->dev.rx_indicate = RT_NULL;
    pin->dev.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    pin->dev.ops = &pin_ops;
#else
    pin->dev.init = RT_NULL;
    pin->dev.open = RT_NULL;
    pin->dev.close = RT_NULL;
    pin->dev.read = _pin_read;
    pin->dev.write = _pin_write;
    pin->dev.control = _pin_control;
#endif

    pin->ops = ops;
    pin->dev.user_data = user_data;

    /* register a character device */
    return rt_device_register(&pin->dev, name, RT_DEVICE_FLAG_RDWR);
}

#ifdef __cplusplus
}
#endif
