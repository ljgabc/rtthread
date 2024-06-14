/**
 * @file kk_rtc.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 看门狗接口
 * @version 0.1
 * @date 2023-04-25
 * @note 使用说明
 * 
 *  // 获取RTC设备
 *  struct kk_dev* dev = device_find("RTC");
 * 
 *  // 设置日期
 *  rtc_set_data(dev, 2023, 4, 25);
 *  
 *  // 设置时间
 *  rtc_set_time(dev, 21, 58, 34);
 * 
 *  // 获取时间
 *  uint32_t time = rtc_timestamp(dev);
 *  // 获取结构化时间
 *  struct kk_time_t time;
 *  rtc_time(dev, &time);
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "components/device/rt_rtc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 设置RTC日期
 *
 * @param year  年
 * @param month 月
 * @param day  日
 * @return 成功返回OK，其他见错误码
 */
rt_err_t rtc_set_date(const struct kk_dev* dev, rt_uint32_t year, rt_uint32_t month, rt_uint32_t day)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_rtc* rtc_dev = (struct rt_device_rtc*)dev;
    return rtc_dev->set_date(dev, year, month, day);
}

/**
 * @brief 设置RTC时间
 *
 * @param hour  时
 * @param minute 分
 * @param second 秒
 * @return 成功返回OK，其他见错误码
 */
rt_err_t rtc_set_time(const struct rt_device* dev, rt_uint32_t hour, rt_uint32_t minute, rt_uint32_t second)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_rtc* rtc_dev = (struct rt_device_rtc*)dev;
    return rtc_dev->set_time(dev, hour, minute, second);
}

/**
 * @brief 获取当前时间戳
 *
 * @return 自1970年1月1日经过的秒数（忽略闰秒）
 */
rt_uint32_t rtc_timestamp(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_rtc* rtc_dev = (struct rt_device_rtc*)dev;
    return rtc_dev->timestamp(dev);
}

/**
 * @brief 获取当前时间
 *
 * @param time 时间结构体
 * @return 成功返回OK，其他见错误码
 */
rt_err_t rtc_time(const struct rt_device* dev, struct kk_time_t* time)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_rtc* rtc_dev = (struct rt_device_rtc*)dev;
    return rtc_dev->time(dev, time);
}

#ifdef __cplusplus
}
#endif
