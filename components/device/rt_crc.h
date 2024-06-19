/**
 * @file rt_crc.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief CRC通用接口
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

#define RT_CRC_INIT 0x01

/**
 * @brief CRC设备模型
 *
 */
struct rt_crc_ops;
struct rt_device_crc {
    struct rt_device dev;
    const struct rt_crc_ops* ops;
};

/**
 * @brief CRC设备操作符
 * 
 */
struct rt_crc_ops {
    void (*init)(const struct rt_device* dev, rt_uint32_t init_value);
    rt_uint32_t (*calc)(const struct rt_device* dev, rt_uint8_t data);
    rt_uint32_t (*calc_buffer)(const struct rt_device* dev, const rt_uint8_t* buffer, rt_ssize_t length);
    rt_uint32_t (*read)(const struct rt_device* dev);
};

/**
 * @brief CRC初始化
 *
 * @param init_value 初始值
 */
void crc_init(const struct rt_device* dev, rt_uint32_t init_value);

/**
 * @brief CRC计算
 *
 * @param data 数据
 * @return CRC值
 */
rt_uint32_t crc_calc(const struct rt_device* dev, rt_uint8_t data);

/**
 * @brief CRC计算
 *
 * @param buffer 缓冲区
 * @param length 缓冲区长度
 * @return CRC值
 */
rt_uint32_t crc_calc_buffer(const struct rt_device* dev, const rt_uint8_t* buffer, rt_ssize_t length);

/**
 * @brief 注册CRC设备
 * 
 * @param crc    CRC设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_crc_register(struct rt_device_crc* crc, const char* name, const struct rt_crc_ops* ops, void* user_data);

#ifdef __cplusplus
}
#endif
