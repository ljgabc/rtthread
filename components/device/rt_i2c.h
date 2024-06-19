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
    char* buf;
};

/**
 * @brief I2C设备模型
 * 
 */
struct rt_i2c_ops;
struct rt_device_i2c {
    struct rt_device dev;
    const struct rt_i2c_ops* ops;
    rt_uint16_t flags;
#ifdef CONFIG_USE_RTOS
    struct rt_mutex lock;
#endif
    rt_uint32_t timeout;
    rt_uint32_t retries;
    void* priv;
};

struct rt_i2c_ops {
    rt_err_t (*open)(struct rt_device* dev, rt_uint16_t oflag);
    rt_err_t (*control)(struct rt_device* dev, int cmd, void* args);
    rt_ssize_t (*write)(const struct rt_device* dev, rt_uint32_t address, const char* buffer, rt_ssize_t size,
        rt_base_t flags);
    rt_ssize_t (*read)(const struct rt_device* dev, rt_uint32_t address, char* buffer, rt_ssize_t size,
        rt_base_t flags);
};

/**
 * @brief Write to an I2C slave
 *
 * @param address   8-bit I2C slave address [ addr | 0 ]
 * @param buffer    Pointer to the byte-array data to send
 * @param size      Number of bytes to send
 * @param flags     I2C Message Flags
 * @return size of bytes write
 */
rt_ssize_t i2c_write(const struct rt_device* dev, rt_uint32_t address, const char* buffer, rt_ssize_t size,
    rt_base_t flags);

/**
 * @brief Read from an I2C slave
 *
 * @param address       8-bit I2C slave address [ addr | 1 ]
 * @param buffer        Pointer to the byte-array to read data in to
 * @param size          Number of bytes to read
 * @param flags     I2C Message Flags
 * @return size of bytes read
 */
rt_ssize_t i2c_read(const struct rt_device* dev, rt_uint32_t address, char* buffer, rt_ssize_t size,
    rt_base_t flags);

/**
 * @brief 
 * 
 * @param dev 
 * @param msgs 
 * @param num 
 * @return 传输的num
 */
rt_ssize_t i2c_transfer(struct rt_device* dev, struct rt_i2c_msg msgs[], rt_uint32_t num);

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
