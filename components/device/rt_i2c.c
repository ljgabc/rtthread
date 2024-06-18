/**
 * @file kk_i2c.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief I2C控制通用接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "components/device/rt_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Write to an I2C slave
 *
 * @param address   8-bit I2C slave address [ addr | 0 ]
 * @param buffer    Pointer to the byte-array data to send
 * @param size      Number of bytes to send
 * @param flags     I2C Message Flags
 * @return size of bytes write
 */
rt_ssize_t i2c_write(const struct rt_device* dev, rt_uint32_t address, const char* buffer, rt_ssize_t size,
    rt_base_t flags)
{
    rt_ssize_t ret;
    rt_err_t err;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;

#ifdef CONFIG_USE_RTOS
    err = rt_mutex_take(&i2c_dev->lock, RT_WAITING_FOREVER);
    if (err != RT_EOK) {
        return (rt_ssize_t)err;
    }
#endif

    ret = i2c_dev->ops->write(dev, address, buffer, size, flags);

#ifdef CONFIG_USE_RTOS
    err = rt_mutex_release(&i2c_dev->lock);
    if (err != RT_EOK) {
        return (rt_ssize_t)err;
    }
#endif

    return ret;
}

/**
 * @brief Read from an I2C slave
 *
 * @param address       8-bit I2C slave address [ addr | 1 ]
 * @param buffer        Pointer to the byte-array to read data in to
 * @param size          Number of bytes to read
 * @param flags     I2C Message Flags
 * @return size of bytes read
 */
rt_ssize_t i2c_read(const struct rt_device* dev, rt_uint32_t address, char* buffer, rt_ssize_t size,
    rt_base_t flags)
{
    rt_ssize_t ret;
    rt_err_t err;

    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;

#ifdef CONFIG_USE_RTOS
    err = rt_mutex_take(&i2c_dev->lock, RT_WAITING_FOREVER);
    if (err != RT_EOK) {
        return (rt_ssize_t)err;
    }
#endif

    ret = i2c_dev->ops->read(dev, address, buffer, size, flags);

#ifdef CONFIG_USE_RTOS
    err = rt_mutex_release(&i2c_dev->lock);
    if (err != RT_EOK) {
        return (rt_ssize_t)err;
    }
#endif

    return ret;
}

rt_ssize_t i2c_transfer(struct rt_device* dev, struct rt_i2c_msg msgs[], rt_uint32_t num)
{
    rt_ssize_t ret;
    rt_err_t err;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;

    for (rt_uint32_t i = 0; i < num; i++) {
        if ((msgs[i].flags & RT_I2C_FLAG_RD) == RT_I2C_FLAG_RD) {
            ret = i2c_read(dev, msgs[i].addr, msgs[i].buf, msgs[i].len, msgs[i].flags);
        } else {
            ret = i2c_write(dev, msgs[i].addr, msgs[i].buf, msgs[i].len, msgs[i].flags);
        }
    }
    return ret;
}

rt_err_t _i2c_open(rt_device_t dev, rt_uint16_t oflag)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;
    return i2c_dev->ops->open(dev, oflag);
}

static rt_ssize_t _i2c_read(rt_device_t dev,
    rt_off_t pos,
    void* buffer,
    rt_ssize_t size)
{
    rt_uint16_t addr;
    rt_uint16_t flags;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;

    addr = pos & 0xffff;
    flags = (pos >> 16) & 0xffff;

    return i2c_read(dev, addr, (char*)buffer, size, flags);
}

static rt_ssize_t _i2c_write(rt_device_t dev,
    rt_off_t pos,
    const void* buffer,
    rt_ssize_t size)
{
    rt_uint16_t addr;
    rt_uint16_t flags;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;

    addr = pos & 0xffff;
    flags = (pos >> 16) & 0xffff;

    return i2c_write(dev, addr, (const char*)buffer, size, flags);
}

static rt_err_t _i2c_control(rt_device_t dev, int cmd, void* args)
{
    rt_err_t ret;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;

    RT_ASSERT(bus != RT_NULL);

    switch (cmd) {
    /* set 10-bit addr mode */
    case RT_I2C_DEV_CTRL_10BIT:
        i2c_dev->flags |= RT_I2C_FLAG_ADDR_10BIT;
        break;
    case RT_I2C_DEV_CTRL_TIMEOUT:
        i2c_dev->timeout = *(rt_uint32_t*)args;
        break;
    default:
        return i2c_dev->ops->control(dev, cmd, args);
    }

    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops i2c_ops = {
    RT_NULL,
    _i2c_open,
    RT_NULL,
    _i2c_read,
    _i2c_write,
    _i2c_control,
};
#endif

/**
 * @brief 注册I2C设备
 * 
 * @param i2c    I2C设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param flag   设备标志
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_i2c_register(struct rt_device_i2c* i2c, const char* name, const struct rt_i2c_ops* ops, rt_uint32_t flag, void* user_data)
{
    RT_ASSERT(i2c != RT_NULL);
    i2c->dev.type = RT_Device_Class_I2CBUS;
    i2c->dev.rx_indicate = RT_NULL;
    i2c->dev.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    i2c->dev.ops = &i2c_ops;
#else
    i2c->dev.init = RT_NULL;
    i2c->dev.open = _i2c_open;
    i2c->dev.close = RT_NULL;
    i2c->dev.read = _i2c_read;
    i2c->dev.write = _i2c_write;
    i2c->dev.control = _i2c_control;
#endif

    i2c->ops = ops;
    i2c->dev.user_data = user_data;

    /* register a character device */
    return rt_device_register(&i2c->dev, name, flag);
}

#ifdef __cplusplus
}
#endif
