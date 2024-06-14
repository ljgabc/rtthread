/**
 * @file kk_i2c.h
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

/**
 * @brief IOCTL指令
 *
 */
enum {
    I2C_IOCTL_SET_STATE = (1UL << 16),
    I2C_IOCTL_SET_FREQ,
    I2C_IOCTL_SET_SDA,
    I2C_IOCTL_SET_SCL,
};

/**
 * @brief I2C读写标志位
 * 
 */
enum {
    I2C_MODE_TEN = 0x0001,
    I2C_MODE_NOSTART = 0x0002,
    I2C_MODE_NOSTOP = 0x0004,
    I2C_MODE_IGNORE_NAK = 0x0008,
};

/**
 * @brief I2C设备模型
 * 
 */
struct rt_device_i2c {
    struct rt_device dev;
    rt_err_t (*open)(const struct rt_device* dev, rt_base_t hz, rt_base_t sda, rt_base_t scl);
    rt_size_t (*read)(const struct rt_device* dev, rt_uint32_t address, void* buffer, rt_size_t size, rt_base_t flags);
    rt_size_t (*write)(const struct rt_device* dev, rt_uint32_t address, const void* buffer, rt_size_t size, rt_base_t flags);
};

/**
 * @brief 使能I2C
 *
 * @param dev   设备模型
 * @param hz    总线频率
 * @param sda   sda引脚，硬件I2C引脚确定的情况下，忽略此参数
 * @param scl   scl引脚，硬件I2C引脚确定的情况下，忽略此参数
 * @return 成功返回OK，其他见错误码
 */
rt_err_t i2c_open(const struct rt_device* dev, rt_base_t hz, rt_base_t sda, rt_base_t scl);

/**
 * @brief Write to an I2C slave
 *
 * @param address   8-bit I2C slave address [ addr | 0 ]
 * @param buffer    Pointer to the byte-array data to send
 * @param size      Number of bytes to send
 * @param flags     I2C Message Flags
 * @return size of bytes write
 */
rt_size_t i2c_write(const struct rt_device* dev, rt_uint32_t address, const char* buffer, rt_size_t size,
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
rt_size_t i2c_read(const struct rt_device* dev, rt_uint32_t address, char* buffer, rt_size_t size,
    rt_base_t flags);

#ifdef __cplusplus
}
#endif
