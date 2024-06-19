/**
 * @file rt_tim.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 硬件定时器接口
 * @version 0.1
 * @date 2023-04-25
 * @note 使用说明
 * 
 *  // 创建一个硬件定时器，并设置溢出时间为1ms
 *  struct rt_device* dev = device_find("TIMER0");
 *  tim_create(dev, 1000);
 *  
 *  // 启动定时器
 *  tim_start(dev);
 * 
 *  // 暂停定时器
 *  tim_stop(dev);
 * 
 *  // 读取当前定时值
 *  int32_t overflow = 0;
 *  uint32_t cnt = tim_count(dev, &overflow);
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "components/device/rt_tim.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建一个硬件定时器，并设置溢出时间
 *
 * @param period_us  溢出时间
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_create(const struct rt_device* dev, rt_uint32_t period_us)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim_dev = (struct rt_device_tim*)dev;
    return tim_dev->create(dev, period_us);
}

/**
 * @brief 启动定时器
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_start(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim_dev = (struct rt_device_tim*)dev;
    return tim_dev->start(dev);
}

/**
 * @brief 停止定时器
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_stop(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim_dev = (struct rt_device_tim*)dev;
    return tim_dev->stop(dev);
}

/**
 * @brief 喂狗
 *
 * @return 成功返回OK，其他见错误码
 */
rt_uint32_t tim_count(const struct rt_device* dev, rt_uint32_t* overflow)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim_dev = (struct rt_device_tim*)dev;
    overflow = tim_dev->overflow;
    return tim_dev->count(dev);
}

/**
 * @brief 定时器中断处理函数
 * 
 */
void rt_tim_callback(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim_dev = (struct rt_device_tim*)dev;
    tim_dev->overflow++;

    if (tim_dev->callback != RT_NULL) {
        tim_dev->callback(dev);
    }
}

#ifdef __cplusplus
}
#endif
