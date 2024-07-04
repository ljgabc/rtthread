/**
 * @file rt_i2c.c
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
 * @brief I2C传输
 * 
 * @param dev 设备模型
 * @param address 子设备地址
 * @param buffer 数据缓冲区地址
 * @param size 数据长度
 * @param flags 读写标志位
 * @return 传输的数据长度，失败或超时
 */
rt_ssize_t i2c_transfer(struct rt_device* dev, rt_uint32_t address, rt_uint8_t* buffer, rt_size_t size,
    rt_base_t flags)
{
    rt_ssize_t ret;
    rt_err_t err;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c = (struct rt_device_i2c*)dev;

    err = rt_mutex_take(&i2c->lock, RT_WAITING_FOREVER);
    if (err != RT_EOK) {
        return (rt_ssize_t)err;
    }

    rt_completion_init(&i2c->completion);

    // 发送起始位，等待进入主机模式
    i2c->ops->start(dev);
    err = rt_completion_wait(&i2c->completion, i2c->timeout);
    if (err != RT_EOK) {
        return 0;
    }

    // 发送地址，等待ACK响应
    address <<= 1;
    if ((flags & RT_I2C_FLAG_RD) == RT_I2C_FLAG_RD) {
        address |= 0x01;
    }
    i2c->ops->send_addr(dev, address);
    err = rt_completion_wait(&i2c->completion, i2c->timeout);
    if (err != RT_EOK) {
        return 0;
    }

    if ((flags & RT_I2C_FLAG_RD) == RT_I2C_FLAG_RD) {
        for (rt_ssize_t i = 0; i < size; i++) {
            if (i == (size - 1)) {
                i2c->ops->disable_ack(dev);
                i2c->ops->stop(dev);
            }
            err = rt_completion_wait(&i2c->completion, i2c->timeout);
            if (err != RT_EOK) {
                return 0;
            }
            buffer[i] = i2c->ops->read_byte(dev);
        }
    } else {
        for (rt_ssize_t i = 0; i < size; i++) {
            err = rt_completion_wait(&i2c->completion, i2c->timeout);
            if (err != RT_EOK) {
                return 0;
            }
            i2c->ops->write_byte(dev, buffer[i]);
        }
        i2c->ops->stop(dev);
    }

    err = rt_mutex_release(&i2c->lock);
    if (err != RT_EOK) {
        return (rt_ssize_t)err;
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
    rt_size_t size)
{
    rt_uint16_t addr;
    rt_uint16_t flags;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;

    addr = pos & 0xffff;
    flags = (pos >> 16) & 0xffff;
    flags |= RT_I2C_FLAG_RD;

    return i2c_transfer(dev, addr, (rt_uint8_t*)buffer, size, flags);
}

static rt_ssize_t _i2c_write(rt_device_t dev,
    rt_off_t pos,
    const void* buffer,
    rt_size_t size)
{
    rt_uint16_t addr;
    rt_uint16_t flags;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;

    addr = pos & 0xffff;
    flags = (pos >> 16) & 0xffff;
    flags &= ~RT_I2C_FLAG_RD;

    return i2c_transfer(dev, addr, (const rt_uint8_t*)buffer, size, flags);
}

/**
 * @brief 中断回调函数
 * 
 * @param dev 
 * @param event 
 */
void rt_hw_i2c_isr(struct rt_device* dev, int event)
{
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops i2c_ops = {
    RT_NULL,
    _i2c_open,
    RT_NULL,
    _i2c_read,
    _i2c_write,
    RT_NULL,
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
    i2c->dev.control = RT_NULL;
#endif

    i2c->ops = ops;
    i2c->dev.user_data = user_data;
    i2c->timeout = 10; // 10 tick

    /* register a character device */
    return rt_device_register(&i2c->dev, name, flag);
}

#ifdef __cplusplus
}
#endif
