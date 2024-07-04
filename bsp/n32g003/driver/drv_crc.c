/**
 * @file drv_crc.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief N32G003 API适配CRC驱动
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "config.h"

#if defined(CONFIG_BSP_N32G003) && defined(CONFIG_USING_CRC) && defined(CONFIG_BSP_CRC)

#include "bsp/n32g003/library/n32g003_crc.h"
#include "bsp/n32g003/library/n32g003_rcc.h"
#include "components/device/rt_crc.h"

#ifdef __cplusplus
extern "C" {
#endif

static void n32g003_crc_init(const struct rt_device* dev, rt_uint32_t init_value)
{
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_CRC);
    CRC16_Big_Endian_Format_Set();
    CRC16_Value_Clean_Enable();
    CRC16_Set(init_value);
}

static rt_uint32_t n32g003_crc_calc(const struct rt_device* dev, rt_uint8_t data)
{
    return CRC16_Calculate(data);
}

static rt_uint32_t n32g003_crc_calc_buffer(const struct rt_device* dev, const rt_uint8_t* buffer, rt_ssize_t length)
{
    return CRC16_Buffer_Calculate((rt_uint8_t*)buffer, length);
}

static rt_uint32_t n32g003_crc_read(const struct rt_device* dev)
{
    return CRC16_Get();
}


static const struct rt_crc_ops _n32g003_crc_ops = {
    .init = n32g003_crc_init,
    .calc = n32g003_crc_calc,
    .calc_buffer = n32g003_crc_calc_buffer,
    .read = n32g003_crc_read,
};

static struct rt_device_crc _n32g003_crc;

int _n32g003_crc_init(void)
{
    return rt_device_crc_register(&_n32g003_crc, "crc", &_n32g003_crc_ops, RT_NULL);
}
INIT_DEVICE_EXPORT(_n32g003_crc_init);

#ifdef __cplusplus
}
#endif

#endif
