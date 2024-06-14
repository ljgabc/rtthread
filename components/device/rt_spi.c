/**
 * @file kk_spi.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief SPI控制通用接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "components/device/rt_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief SPI使能配置
 *
 * @param dev       设备模型
 * @param hz        SPI时钟频率，默认100*1000
 * @param mode      模式
 *  mode | POL PHA
 * -----+--------
 *   0  |  0   0
 *   1  |  0   1
 *   2  |  1   0
 *   3  |  1   1
 * @return kk_err_t 成功返回OK，其他见错误码
 */
rt_err_t spi_open(const struct rt_device* dev, rt_base_t hz, rt_base_t mode)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_spi* spi_dev = (struct rt_device_spi*)dev;
    return spi_dev->open(dev, hz, mode);
}

/**
 * @brief Write to the SPI Slave and return the response.
 *
 * @param dev       设备模型
 * @param value     Data to be sent to the SPI slave.
 * @return Response from the SPI slave.
 */
rt_base_t spi_transfer_byte(const struct rt_device* dev, rt_base_t value)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_spi* spi_dev = (struct rt_device_spi*)dev;
    return spi_dev->transfer_byte(dev, value);
}

/**
 * @brief Write to the SPI Slave and obtain the response.
 * The total number of bytes sent and received will be the maximum of
 *  tx_length and rx_length. The bytes written will be padded with the
 *  value 0xff.
 *
 * @param dev           设备模型
 * @param tx_buffer     Pointer to the byte-array of data to write to the
 * device.
 * @param tx_length     Number of bytes to write, may be zero.
 * @param rx_buffer     Pointer to the byte-array of data to read from the
 * device.
 * @param rx_length     Number of bytes to read, may be zero.
 * @return The number of bytes written and read from the device. This is
 *      maximum of tx_length and rx_length.
 */
rt_size_t spi_transfer(const struct rt_device* dev, const rt_int8_t* tx_buffer, rt_size_t tx_length,
    rt_int8_t* rx_buffer, rt_size_t rx_length)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_spi* spi_dev = (struct rt_device_spi*)dev;
    return spi_dev->transfer(dev, tx_buffer, tx_length, rx_buffer, rx_length);
}

#ifdef __cplusplus
}
#endif
