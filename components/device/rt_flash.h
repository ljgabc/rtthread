/**
 * @file rt_flash.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief Flash读写接口
 * @version 0.1
 * @date 2023-04-25
 * @note 使用说明
 * 
 *  // 打开Flash设备
 *  struct kk_dev* dev = device_find("FLASH");
 *  
 *  // 擦除从0x1000到0x2000之间的数据
 *  flash_erase(dev, 0x1000, 0x2000);
 *  
 *  // 向0x1000写入数据
 *  flash_write(dev, 0x1000, buf, 128);
 * 
 *  // 从0x1000读出数据
 *  flash_read(dev, 0x1000, buf, 128);
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
 * @brief 看门狗设备模型
 * 
 */
struct rt_device_flash {
    struct rt_device dev;
    rt_err_t (*erase)(const struct rt_device* dev, rt_uint32_t start, rt_uint32_t end);
    rt_err_t (*write)(const struct rt_device* dev, rt_uint32_t address, const rt_uint32_t* buf, rt_ssize_t size);
    rt_err_t (*read)(const struct rt_device* dev, rt_uint32_t address, rt_uint32_t* buf, rt_ssize_t size);
};

/**
 * @brief 擦拭Flash
 *
 * @param start 起始地址
 * @param end 结束地址
 * @return 成功返回OK，其他见错误码
 */
rt_err_t flash_erase(const struct kk_drt_deviceev* dev, rt_uint32_t start, rt_uint32_t end);

/**
 * @brief 写入Flash
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t flash_write(const struct rt_device* dev, rt_uint32_t address, const rt_uint32_t* buf, rt_ssize_t size);

/**
 * @brief 读取Flash数据
 *
 * @return 成功返回OK，其他见错误码
 */
rt_err_t flash_read(const struct rt_device* dev, rt_uint32_t address, rt_uint32_t* buf, rt_ssize_t size);

#ifdef __cplusplus
}
#endif
