/**
 * @file rt_i2c.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief I2C控制通用接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "rtdevice.h"
#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RT_I2C_FLAG_WR 0x0000
#define RT_I2C_FLAG_RD (1u << 0)
#define RT_I2C_FLAG_ADDR_10BIT (1u << 2) /* this is a ten bit chip address */
#define RT_I2C_FLAG_NO_START (1u << 4)
#define RT_I2C_FLAG_IGNORE_NACK (1u << 5)
#define RT_I2C_FLAG_NO_READ_ACK (1u << 6) /* when I2C reading, we do not ACK */
#define RT_I2C_FLAG_NO_STOP (1u << 7)

#define RT_I2C_DEV_CTRL_10BIT (0x01)
#define RT_I2C_DEV_CTRL_ADDR (0x02)
#define RT_I2C_DEV_CTRL_TIMEOUT (0x03)
#define RT_I2C_DEV_CTRL_RW (0x04)
#define RT_I2C_DEV_CTRL_CLK (0x05)
#define RT_I2C_DEV_CTRL_UNLOCK (0x06)
#define RT_I2C_DEV_CTRL_GET_STATE (0x07)
#define RT_I2C_DEV_CTRL_GET_MODE (0x08)
#define RT_I2C_DEV_CTRL_GET_ERROR (0x09)
#define RT_I2C_DEV_CTRL_SET_BITRATE (0x0A)

struct rt_i2c_msg {
    rt_uint16_t addr;
    rt_uint16_t flags;
    rt_uint16_t len;
    rt_uint8_t* buf;
};

/**
 * @brief I2C设备模型
 * 
 */
struct rt_i2c_ops;
struct rt_device_i2c {
    struct rt_device dev;
    const struct rt_i2c_ops* ops;
    struct rt_mutex lock;
    struct rt_completion completion;
    rt_uint32_t timeout;
};

struct rt_i2c_ops {
    rt_err_t (*open)(struct rt_device* dev, rt_uint16_t oflag);
    void (*start)(const struct rt_device* dev);
    void (*send_addr)(const struct rt_device* dev, rt_uint16_t addr);
    void (*stop)(const struct rt_device* dev);
    void (*enable_ack)(const struct rt_device* dev);
    void (*disable_ack)(const struct rt_device* dev);
    void (*write_byte)(const struct rt_device* dev, rt_uint8_t data);
    rt_uint8_t (*read_byte)(const struct rt_device* dev);
};

/**
 * @brief I2C传输
 * 
 * @param dev 设备模型
 * @param address 子设备地址
 * @param buffer 数据缓冲区地址
 * @param size 数据长度
 * @param flags 读写标志位
 * @return 传输的数据长度，失败或超时
 */
rt_ssize_t i2c_transfer(struct rt_device* dev, rt_uint32_t address, rt_uint8_t* buffer, rt_size_t size,
    rt_base_t flags);

/**
 * @brief 中断回调函数
 * 
 * @param dev 
 * @param event 
 */
void rt_hw_i2c_isr(struct rt_device* dev, int event);

/**
 * @brief 注册I2C设备
 * 
 * @param i2c    I2C设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param flag   设备标志
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_i2c_register(struct rt_device_i2c* i2c, const char* name, const struct rt_i2c_ops* ops, rt_uint32_t flag, void* user_data);

#ifdef __cplusplus
}
#endif
