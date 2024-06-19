/**
 * @file rt_adc.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief ADC控制通用接口
 * @version 0.1
 * @date 2023-04-25
 * 
 * @copyright Copyright (c) 2023
 *
 */
#include "components/device/rt_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ADC使能配置
 *
 * @param channel 通道编号
 * @param mode 打开参数
 * @return 成功返回OK，其他见错误码
 */
rt_err_t adc_open(const struct rt_device* dev, rt_base_t channel, rt_base_t mode)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_adc* adc_dev = (struct rt_device_adc*)dev;
    return adc_dev->ops->enable(dev, channel, mode);
}

/**
 * @brief 获取引脚电压
 *
 * @param channel 通道
 * @return ADC转换值
 */
rt_uint32_t adc_read(const struct rt_device* dev, rt_base_t channel)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_adc* adc_dev = (struct rt_device_adc*)dev;
    return adc_dev->ops->read(dev, channel);
}

/**
 * @brief 获取引脚电压
 *
 * @return ADC位数
 */
rt_uint8_t adc_get_resolution(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_adc* adc_dev = (struct rt_device_adc*)dev;
    return adc_dev->ops->get_resolution(dev);
}

/**
 * @brief 读ADC值
 * 
 * @param dev       设备
 * @param pos       通道
 * @param buffer    目的缓冲区地址
 * @param size      缓冲区大小
 * @return size 
 */
static rt_ssize_t _adc_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_ssize_t size)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_adc* adc = (struct rt_device_adc*)dev;

    rt_base_t channel = pos;

    for (rt_ssize_t i = 0; i < size; i++) {
        ((rt_uint32_t*)buffer)[i] = adc_read(dev, channel);
    }

    return size;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops adc_ops = {
    RT_NULL,
    RT_NULL,
    RT_NULL,
    _adc_read,
    RT_NULL,
    RT_NULL,
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
rt_err_t rt_device_adc_register(struct rt_device_adc* adc, const char* name, const struct rt_adc_ops* ops, void* user_data)
{
    adc->dev.type = RT_Device_Class_ADC;
    adc->dev.rx_indicate = RT_NULL;
    adc->dev.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    adc->dev.ops = &adc_ops;
#else
    adc->dev.init = RT_NULL;
    adc->dev.open = RT_NULL;
    adc->dev.close = RT_NULL;
    adc->dev.read = _pin_read;
    adc->dev.write = RT_NULL;
    adc->dev.control = _pin_control;
#endif

    adc->ops = ops;
    adc->dev.user_data = user_data;

    /* register a character device */
    return rt_device_register(&adc->dev, name, RT_DEVICE_FLAG_RDONLY);
}

#ifdef __cplusplus
}
#endif
