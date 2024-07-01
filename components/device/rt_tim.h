/**
 * @file rt_tim.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 硬件定时器接口
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

#define TIM_MODE_NONE 0x00 // 纯计数模式
#define TIM_MODE_CAPTURE 0x01 // 捕获模式一般用来测量外部输入信号的周期       \
                              // 检测到外部信号的上升沿或下降沿时记录当前定时器的计数值 \
                              // 测量两个沿之间的时间

#define TIM_MODE_PWM 0x2 // PWM模式一般用来输出指定占空比的PWM信号 \
                              // PWM可以有互补输出，两个信号CHx和CHxN的电平相反

#define TIM_MODE_TOGGLE 0x3 // 翻转模式一般用来输出固定个数的脉冲，比如用来控制步仅电机时 \
                              // 当定时器计数值与输出比较值相同时，翻转输出。控制定时器溢出的次数就可以控制引脚翻转的次数。

#define TIM_CNTMODE_UP 0x00 // 向上计数
#define TIM_CNTMODE_DN 0x01 // 向上计数
#define TIM_CNTMODE_UPDN 0x02 // 向上向下计数

#define TIM_CAPTURE_RISING (0x00) // 捕获上升沿
#define TIM_CAPTURE_FALLING (0x01) // 捕获下降沿

#define TIM_PWM_HIGHACTIVE (0x00) // PWM高电平有效，默认输出低电平
#define TIM_PWM_LOWACTIVE (0x01) // PWM低电平有效，默认输出高电平

#define TIM_PWM_IDLESTATE_LOW (0x00) // PWM IDLE时输出低电平
#define TIM_PWM_IDLESTATE_HIGH (0x01) // PWM IDLE时输出高电平

#define TIM_PWM_NON_COMPLEMENTARY (0x00) // 关闭互补输出
#define TIM_PWM_COMPLEMENTARY (0x01) // 打开互补输出

#define TIM_PWM_DT_DISABLE (0x00) // 关闭死区功能
#define TIM_PWM_DT_ENABLE (0x01) // 打开死区功能

#define TIM_PWM_BK_DISABLE (0x00) // 关闭刹车功能
#define TIM_PWM_BK_ENABLE (0x01) // 打开刹车功能

#define TIM_PWM_BK_HIGHACTIVE (0x00)
#define TIM_PWM_BK_LOWACTIVE (0x01)

#define TIM_EVENT_OVERFLOW 0
#define TIM_EVENT_CAPTURE 1
#define TIM_EVENT_BREAK 2

typedef void (*tim_overflow_callback)(const struct rt_device* dev, void* arg, rt_base_t event);

struct rt_tim_config {
    rt_uint8_t mode : 4;
    rt_uint8_t cnt_mode : 4;
    union {
        struct {
            rt_uint8_t edge : 1; // 上升沿 or 下降沿
            rt_uint8_t filter : 4; // 滤波器，用来过滤毛刺，连续多个tCLK采集到电平才认为是有效信号
            rt_uint8_t : 0;
        } capture_config;
        struct {
            rt_uint8_t polarity : 1; // PWM输出极性
            rt_uint8_t idle_state : 1; // IDLE时状态
            rt_uint8_t complementary_en : 1; // 使能互补输出
            rt_uint8_t dt_en : 1; // 使能死区控制
            rt_uint8_t bk_en : 1; // 使能刹车功能
            rt_uint8_t bk_polarity : 1; // 刹车信号极性
            rt_uint8_t : 0;
        } pwm_config;
    };
};

/**
 * @brief 高级硬件定时器设备模型
 * 
 */
struct rt_tim_ops;
struct rt_device_tim {
    struct rt_device dev;
    const struct rt_tim_ops* ops;
    void (*callback)(const struct rt_device* dev, void* arg, rt_base_t event);
    void* arg;
};

struct rt_tim_ops {
    rt_err_t (*open)(const struct rt_device* dev, rt_uint32_t period_ns);
    rt_err_t (*config)(const struct rt_device* dev, rt_base_t channel, const struct rt_tim_config* config);
    void (*start)(const struct rt_device* dev);
    void (*stop)(const struct rt_device* dev);
    rt_uint32_t (*count)(const struct rt_device* dev);
    rt_uint32_t (*cc_get_period)(const struct rt_device* dev, rt_base_t channel);
    rt_err_t (*pwm_set_pulse)(const struct rt_device* dev, rt_base_t channel, rt_uint32_t pulse_ns);
    rt_err_t (*pwm_set_deadtime)(const struct rt_device* dev, rt_base_t channel, rt_uint32_t deadtime_ns);
};

/**
 * @brief 打开硬件定时器，并设置周期(溢出频率)
 *
 * @param period_ns  定时器周期
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_open(const struct rt_device* dev, rt_uint32_t period_ns);

/**
 * @brief 设置溢出中断函数
 *
 * @param callback  溢出回调函数
 * @param arg  回调函数参数
 * @return 成功返回OK，其他见错误码
 */
void tim_set_overflow_callback(const struct rt_device* dev, tim_overflow_callback callback, void* arg);

/**
 * @brief 设置输入或输出模式
 *
 * @param channel 通道
 * @param config  输入或输出模式配置
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_config(const struct rt_device* dev, rt_base_t channel, const struct rt_tim_config* config);

/**
 * @brief 读取定时器当前计数值
 *
 * @return 当前计数值
 */
rt_uint32_t tim_count(const struct rt_device* dev);

/**
 * @brief 启动定时器
 * 
 */
void tim_start(const struct rt_device* dev);

/**
 * @brief 停止定时器
 *
 */
void tim_stop(const struct rt_device* dev);

/**
 * @brief CAPTURE模式下读取周期
 *
 * @param channel 通道
 * @return 捕获信号的周期us
 */
rt_uint32_t tim_cc_get_period(const struct rt_device* dev, rt_base_t channel);

/**
 * @brief PWM模式下配置脉宽
 * 
 * @param channel 通道
 * @param pulse_ns 脉宽
 * @return 成功返回OK，其他见错误码
 */
rt_err_t tim_pwm_set_pulse(const struct rt_device* dev, rt_base_t channel, rt_uint32_t pulse_ns);

/**
 * @brief PWM模式下设置死区时间
 * 
 * @param channel 通道
 * @param deadtime_ns 死区时间
 * @return rt_err_t 成功返回OK，其他见错误码
 */
rt_err_t tim_pwm_set_deadtime(const struct rt_device* dev, rt_base_t channel, rt_uint32_t deadtime_ns);

/**
 * @brief 注册TIM设备
 * 
 * @param tim    TIM设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_tim_register(struct rt_device_tim* tim, const char* name, const struct rt_tim_ops* ops, void* user_data);

#ifdef __cplusplus
}
#endif
