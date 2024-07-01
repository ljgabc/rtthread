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
 * @param period_ns  溢出时间
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_open(const struct rt_device* dev, rt_uint32_t period_ns)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    return tim->ops->open(dev, period_ns);
}

/**
 * @brief 启动定时器
 *
 * @return 成功返回OK，其他见错误码
 */
void tim_start(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    return tim->ops->start(dev);
}

/**
 * @brief 设置输入或输出模式
 *
 * @param channel 通道
 * @param config  输入或输出模式配置
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_config(const struct rt_device* dev, rt_base_t channel, const struct rt_tim_config* config)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    return tim->ops->config(dev, channel, config);
}

/**
 * @brief 停止定时器
 *
 * @return 成功返回OK，其他见错误码
 */
void tim_stop(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    return tim->ops->stop(dev);
}

/**
 * @brief 喂狗
 *
 * @return 成功返回OK，其他见错误码
 */
rt_uint32_t tim_count(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    return tim->ops->count(dev);
}

/**
 * @brief 设置溢出中断函数
 *
 * @param callback  溢出回调函数
 * @param arg  回调函数参数
 * @return 成功返回OK，其他见错误码
 */
void tim_set_overflow_callback(const struct rt_device* dev, tim_overflow_callback callback, void* arg)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    tim->callback = callback;
    tim->arg = arg;
}

/**
 * @brief CAPTURE模式下读取周期
 *
 * @param channel 通道
 * @return 捕获信号的周期us
 */
rt_uint32_t tim_cc_get_period(const struct rt_device* dev, rt_base_t channel)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    return tim->ops->cc_get_period(dev, channel);
}

/**
 * @brief PWM模式下配置脉宽
 * 
 * @param channel 通道
 * @param pulse_ns 脉宽
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_pwm_set_pulse(const struct rt_device* dev, rt_base_t channel, rt_uint32_t pulse_ns)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    return tim->ops->pwm_set_pulse(dev, channel, pulse_ns);
}

/**
 * @brief PWM模式下设置死区时间
 * 
 * @param channel 通道
 * @param deadtime_ns 死区时间
 * @return rt_err_t 成功返回OK，其他见错误码
 */
rt_err_t tim_pwm_set_deadtime(const struct rt_device* dev, rt_base_t channel, rt_uint32_t deadtime_ns)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_tim* tim = (struct rt_device_tim*)dev;
    return tim->ops->pwm_set_deadtime(dev, channel, deadtime_ns);
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops tim_ops = {
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
};
#endif

/**
 * @brief 注册ADC设备
 * 
 * @param tim    TIM设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_tim_register(struct rt_device_tim* tim, const char* name, const struct rt_tim_ops* ops, void* user_data)
{
    tim->dev.type = RT_Device_Class_Timer;
    tim->dev.rx_indicate = RT_NULL;
    tim->dev.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    tim->dev.ops = &tim_ops;
#else
    adc->dev.init = RT_NULL;
    adc->dev.open = RT_NULL;
    adc->dev.close = RT_NULL;
    adc->dev.read = _pin_read;
    adc->dev.write = RT_NULL;
    adc->dev.control = _pin_control;
#endif

    tim->ops = ops;
    tim->dev.user_data = user_data;

    /* register a character device */
    return rt_device_register(&tim->dev, name, RT_DEVICE_FLAG_RDWR);
}

#ifdef __cplusplus
}
#endif
