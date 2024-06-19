/**
 * @file rt_adc.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief ADC控制通用接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief IOCTL指令
 *
 */
enum {
    ADC_IOCTL_SET_STATE = (1UL << 16),
};

/**
 * @brief ADC设备模型
 *
 */
struct rt_adc_ops;
struct rt_device_adc {
    struct rt_device dev;
    const struct rt_adc_ops *ops;
};

/**
 * @brief ADC设备操作符
 * 
 */
struct rt_adc_ops {
    rt_err_t (*enable)(const struct rt_device *dev, rt_base_t channel, rt_base_t mode);
    rt_uint32_t (*read)(const struct rt_device* dev, rt_base_t channel);
    rt_uint8_t (*get_resolution)(const struct rt_device *dev);
};


/**
 * @brief ADC使能配置
 *
 * @param channel 通道
 * @param mode 打开参数
 * @return 成功返回OK，其他见错误码
 */
rt_err_t adc_open(const struct rt_device* dev, rt_base_t channel, rt_base_t mode);

/**
 * @brief 获取引脚电压
 *
 * @param channel 通道
 * @return ADC转换值
 */
rt_uint32_t adc_read(const struct rt_device* dev, rt_base_t channel);

/**
 * @brief 获取引脚电压
 *
 * @return ADC位数
 */
rt_uint8_t adc_get_resolution(const struct rt_device* dev);

/**
 * @brief 注册ADC设备
 * 
 * @param adc    ADC设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_adc_register(struct rt_device_adc* adc, const char* name, const struct rt_adc_ops* ops, void* user_data);

#ifdef __cplusplus
}
#endif
