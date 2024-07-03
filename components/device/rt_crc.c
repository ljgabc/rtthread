/**
 * @file rt_crc.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief CRC通用接口
 * @version 0.1
 * @date 2023-04-25
 * 
 * @copyright Copyright (c) 2023
 *
 */
#include "components/device/rt_crc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief CRC初始化
 *
 * @param init_value 初始值
 */
void crc_init(const struct rt_device* dev, rt_uint32_t init_value)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_crc* crc = (struct rt_device_crc*)dev;
    crc->ops->init(dev, init_value);
}

/**
 * @brief CRC计算
 *
 * @param data 数据
 * @return CRC值
 */
rt_uint32_t crc_calc(const struct rt_device* dev, rt_uint8_t data)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_crc* crc = (struct rt_device_crc*)dev;
    return crc->ops->calc(dev, data);
}

/**
 * @brief CRC计算
 *
 * @param buffer 缓冲区
 * @param length 缓冲区长度
 * @return CRC值
 */
rt_uint32_t crc_calc_buffer(const struct rt_device* dev, const rt_uint8_t* buffer, rt_ssize_t length)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_crc* crc = (struct rt_device_crc*)dev;
    return crc->ops->calc_buffer(dev, buffer, length);
}

static rt_ssize_t _crc_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_crc* crc = (struct rt_device_crc*)dev;
    *((rt_uint32_t*)buffer) = crc->ops->read(dev);
    return size;
}

static rt_ssize_t _crc_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_crc* crc = (struct rt_device_crc*)dev;
    crc->ops->calc_buffer(dev, (const rt_uint8_t*)buffer, size);
    return size;
}

static rt_err_t _crc_control(rt_device_t dev, int cmd, void* arg)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_crc* crc = (struct rt_device_crc*)dev;
		rt_uint32_t init_value = (rt_uint32_t)arg;
    switch (cmd) {
		case RT_CRC_INIT:
        crc->ops->init(dev, init_value);
        break;
    default:
        break;
    }
		return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops crc_ops = {
    RT_NULL,
    RT_NULL,
    RT_NULL,
    _crc_read,
    _crc_write,
    _crc_control,
};
#endif

/**
 * @brief 注册ADC设备
 * 
 * @param adc    ADC设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_crc_register(struct rt_device_crc* crc, const char* name, const struct rt_crc_ops* ops, void* user_data)
{
    crc->dev.type = RT_Device_Class_CRC;
    crc->dev.rx_indicate = RT_NULL;
    crc->dev.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    crc->dev.ops = &crc_ops;
#else
    crc->dev.init = RT_NULL;
    crc->dev.open = RT_NULL;
    crc->dev.close = RT_NULL;
    crc->dev.read = _crc_read;
    crc->dev.write = _crc_write;
    crc->dev.control = _crc_control;
#endif

    crc->ops = ops;
    crc->dev.user_data = user_data;

    /* register a character device */
    return rt_device_register(&crc->dev, name, RT_DEVICE_FLAG_RDWR);
}

#ifdef __cplusplus
}
#endif
