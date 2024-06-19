/**
 * @file rt_flash.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief Flash读写接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "components/device/rt_flash.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 擦拭Flash
 *
 * @param start 起始地址
 * @param end 结束地址
 * @return 成功返回OK，其他见错误码
 */
rt_err_t flash_erase(const struct rt_device* dev, rt_uint32_t start, rt_uint32_t end)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_flash* flash_dev = (struct rt_device_flash*)dev;
    return flash_dev->erase(dev, start, end);
}

/**
 * @brief 写入Flash
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t flash_write(const struct rt_device* dev, rt_uint32_t address, const rt_uint32_t* buf, rt_ssize_t size)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_flash* flash_dev = (struct rt_device_flash*)dev;
    return flash_dev->write(dev, address, buf, size);
}

/**
 * @brief 读取Flash数据
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t flash_read(const struct rt_device* dev, rt_uint32_t address, rt_uint32_t* buf, rt_ssize_t size)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_flash* flash_dev = (struct rt_device_flash*)dev;
    return flash_dev->read(dev, address, buf, size);
}

#ifdef __cplusplus
}
#endif
