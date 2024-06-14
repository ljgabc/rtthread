/**
 * @file rt_adc.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief ADC控制通用接口
 * @version 0.1
 * @date 2023-04-25
 * @note 使用说明
 *  struct kk_dev* dev = device_find("ADC");
 *  adc_open(dev, 0, 0);
 *  uint32_t ret = adc_read(dev, 0);
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
struct rt_device_adc {
    struct rt_device dev;
    rt_err_t (*open)(const struct rt_device* dev, rt_base_t channel, rt_uint8_t mode);
    rt_uint32_t (*read)(const struct rt_device* dev, rt_base_t channel);
};

/**
 * @brief ADC使能配置
 *
 * @param channel 通道
 * @param mode 打开参数
 * @return 成功返回OK，其他见错误码
 */
rt_err_t adc_open(const struct rt_device* dev, rt_base_t channel, rt_uint8_t mode);

/**
 * @brief 获取引脚电压
 *
 * @param channel 通道
 * @return ADC转换值
 */
rt_uint32_t adc_read(const struct rt_device* dev, rt_base_t channel);

#ifdef __cplusplus
}
#endif
