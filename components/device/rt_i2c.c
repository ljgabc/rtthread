/**
 * @file kk_i2c.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief I2C控制通用接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "components/device/rt_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 使能I2C
 *
 * @param dev   设备模型
 * @param sda   sda引脚，硬件I2C引脚确定的情况下，忽略此参数
 * @param scl   scl引脚，硬件I2C引脚确定的情况下，忽略此参数
 * @return kk_err_t 成功返回OK，其他见错误码
 */
rt_err_t i2c_open(const struct rt_device* dev, rt_base_t hz, rt_base_t sda, rt_base_t scl)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;
    return i2c_dev->open(dev, hz, sda, scl);
}

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
    rt_base_t flags)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;
    return i2c_dev->write(dev, address, buffer, size, flags);
}

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
    rt_base_t flags)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c_dev = (struct rt_device_i2c*)dev;
    return i2c_dev->read(dev, address, buffer, size, flags);
}

#ifdef __cplusplus
}
#endif
