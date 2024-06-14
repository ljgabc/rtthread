/**
 * @file kk_rtc.h
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

#include "rtthread.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 日期与时间
 * 
 */
struct rt_time_t {
    rt_uint32_t year;
    rt_uint32_t month;
    rt_uint32_t day;
    rt_uint32_t hour;
    rt_uint32_t minute;
    rt_uint32_t second;
};

/**
 * @brief RTC设备模型
 * 
 */
struct rt_device_rtc {
    struct rt_device dev;
    rt_err_t (*set_date)(const struct rt_device* dev, rt_uint32_t year, rt_uint32_t month, rt_uint32_t day);
    rt_err_t (*set_time)(const struct rt_device* dev, rt_uint32_t hour, rt_uint32_t minute, rt_uint32_t second);
    rt_uint32_t (*timestamp)(const struct rt_device* dev);
    rt_err_t (*time)(const struct rt_device* dev, struct kk_time_t* time);
};

/**
 * @brief 设置RTC日期
 *
 * @param year  年
 * @param month 月
 * @param day  日
 * @return 成功返回OK，其他见错误码
 */
rt_err_t rtc_set_date(const struct rt_device* dev, rt_uint32_t year, rt_uint32_t month, rt_uint32_t day);

/**
 * @brief 设置RTC时间
 *
 * @param hour  时
 * @param minute 分
 * @param second 秒
 * @return 成功返回OK，其他见错误码
 */
rt_err_t rtc_set_time(const struct rt_device* dev, rt_uint32_t hour, rt_uint32_t minute, rt_uint32_t second);

/**
 * @brief 获取当前时间戳
 *
 * @return 自1970年1月1日经过的秒数（忽略闰秒）
 */
rt_uint32_t rtc_timestamp(const struct rt_device* dev);

/**
 * @brief 获取当前时间
 *
 * @param time 时间结构体
 * @return 成功返回OK，其他见错误码
 */
rt_err_t rtc_time(const struct rt_device* dev, struct kk_time_t* time);

#ifdef __cplusplus
}
#endif
