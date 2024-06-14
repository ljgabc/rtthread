/**
 * @file kk_wdt.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 看门狗接口
 * @version 0.1
 * @date 2023-04-25
 * @note 使用说明
 * 
 *  // 创建一个看门狗，并设置超时时间为100ms
 *  struct rt_device* dev = device_find("WDT");
 *  wdt_create(dev, 100);
 *  
 *  // 启动看门狗
 *  wdt_start(dev);
 * 
 *  // 暂停看门狗
 *  wdt_stop(dev);
 * 
 *  // 喂狗
 *  wdt_feed(dev);
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
 * @brief 看门狗设备模型
 * 
 */
struct rt_device_wdt {
    struct rt_device dev;
    rt_err_t (*create)(const struct rt_device* dev, rt_uint32_t period_ms);
    rt_err_t (*start)(const struct rt_device* dev);
    rt_err_t (*stop)(const struct rt_device* dev);
    rt_err_t (*feed)(const struct rt_device* dev);
};

/**
 * @brief 创建一个看门狗设备，并设置超时时间
 *
 * @param period_ms  超时时间
 * @return 成功返回OK，其他见错误码
 */
rt_err_t wdt_create(const struct rt_device* dev, rt_uint32_t period_ms);

/**
 * @brief 启动看门狗
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t wdt_start(const struct rt_device* dev);

/**
 * @brief 停止看门狗
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t wdt_stop(const struct rt_device* dev);

/**
 * @brief 喂狗
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t wdt_feed(const struct rt_device* dev);

#ifdef __cplusplus
}
#endif
