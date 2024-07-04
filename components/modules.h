/**
 * @file modules.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "config.h"

#ifdef CONFIG_USING_RINGBUFFER
#include "components/ipc/ringbuffer.h"
#endif

#ifdef CONFIG_USING_COMPLETION
#include "components/ipc/completion.h"
#endif

#ifdef CONFIG_USING_WORKQUEUE
#include "components/ipc/workqueue.h"
#endif

#ifdef CONFIG_USING_DATAQUEUE
#include "components/ipc/dataqueue.h"
#endif

#if CONFIG_USING_DEVICE

#ifdef CONFIG_USING_PIN
#include "components/device/rt_pin.h"
#endif

#ifdef CONFIG_USING_SERIAL
#include "components/device/rt_serial.h"
#endif

#ifdef CONFIG_USING_ADC
#include "components/device/rt_adc.h"
#endif

#ifdef CONFIG_USING_DAC
#include "components/device/rt_dac.h"
#endif

#ifdef CONFIG_USING_FLASH
#include "components/device/rt_flash.h"
#endif

#ifdef CONFIG_USING_I2C
#include "components/device/rt_i2c.h"
#endif

#ifdef CONFIG_USING_RTC
#include "components/device/rt_rtc.h"
#endif

#ifdef CONFIG_USING_CRC
#include "components/device/rt_crc.h"
#endif

#ifdef CONFIG_USING_SPI
#include "components/device/rt_spi.h"
#endif

#ifdef CONFIG_USING_TIM
#include "components/device/rt_tim.h"
#endif

#ifdef CONFIG_USING_WDT
#include "components/device/rt_wdt.h"
#endif

#endif
