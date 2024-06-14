/**
 * @file kk_pwm.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief PWM控制通用接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief IOCTL指令
 *
 */
enum {
    PWM_IOCTL_SET_STATE = (1UL << 16),
    PWM_IOCTL_SET_PERIOD,
    PWM_IOCTL_SET_PULSE,
    PWM_IOCTL_SET_PHASE,
};

/**
 * @brief PWM设备模型
 * 
 */
struct rt_device_pwm {
    struct rt_device dev;
    rt_err_t (*open)(const struct rt_device* dev, rt_int32_t period, rt_int32_t mode);
    rt_err_t (*set)(const struct rt_device* dev, rt_int32_t period, rt_int32_t pulse);
    rt_err_t (*set_period)(const struct rt_device* dev, rt_int32_t period);
    rt_err_t (*set_pulse)(const struct rt_device* dev, rt_int32_t pulse);
    rt_err_t (*set_deadtime)(const struct rt_device* dev, rt_int32_t deadtime);
};

/**
 * @brief PWM使能配置
 *
 * @param dev      设备
 * @param period   周期(us)
 * @param mode     模式，比如高有效、地有效等
 * @return rt_err_t 成功返回OK，其他见错误码
 */
rt_err_t pwm_open(const struct rt_device* dev, rt_int32_t period, rt_int32_t mode);

/**
 * @brief 配置PWM
 * 
 * @param dev       设备
 * @param period    周期(us)
 * @param pulse     脉宽(us)
 * @return rt_err_t 成功返回OK，其他见错误码
 */
rt_err_t pwm_set(const struct rt_device* dev, rt_int32_t period, rt_int32_t pulse);

/**
 * @brief 配置PWM周期
 * 
 * @param dev       设备
 * @param period    周期(us)
 * @return rt_err_t 成功返回OK，其他见错误码
 */
rt_err_t pwm_set_period(const struct rt_device* dev, rt_int32_t period);

/**
 * @brief 配置PWM脉宽
 * 
 * @param dev       设备
 * @param pulse     脉宽
 * @return rt_err_t 成功返回OK，其他见错误码
 */
rt_err_t pwm_set_pulse(const struct rt_device* dev, rt_int32_t pulse);

/**
 * @brief 设置PWM死区时间
 * 
 * @param dev       设备
 * @param dead_time 死区时间(us)
 * @return rt_err_t 成功返回OK，其他见错误码
 */
rt_err_t pwm_set_deadtime(const struct rt_device* dev, rt_int32_t deadtime);

#ifdef __cplusplus
}
#endif
