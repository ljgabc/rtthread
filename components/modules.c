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
#include "config.h"

#if CONFIG_USE_DEVICE

#include "components/device/rt_device.c"

#ifdef CONFIG_USE_PIN
#include "components/device/rt_pin.c"
#endif

#ifdef CONFIG_USE_SERIAL
#include "components/device/rt_serial.c"
#endif

#ifdef CONFIG_USE_ADC
#include "components/device/rt_adc.c"
#endif

#ifdef CONFIG_USE_DAC
#include "components/device/rt_dac.c"
#endif

#ifdef CONFIG_USE_FLASH
#include "components/device/rt_flash.c"
#endif

#ifdef CONFIG_USE_I2C
#include "components/device/rt_i2c.c"
#endif

#ifdef CONFIG_USE_RTC
#include "components/device/rt_rtc.c"
#endif

#ifdef CONFIG_USE_CRC
#include "components/device/rt_crc.c"
#endif

#ifdef CONFIG_USE_SPI
#include "components/device/rt_spi.c"
#endif

#ifdef CONFIG_USE_TIM
#include "components/device/rt_tim.c"
#endif

#ifdef CONFIG_USE_WDT
#include "components/device/rt_wdt.c"
#endif

#ifdef CONFIG_USE_RINGBUFFER
#include "components/ipc/ringbuffer.c"
#endif

#ifdef CONFIG_USE_COMPLETION
#include "components/ipc/completion.c"
#endif

#ifdef CONFIG_USE_WORKQUEUE
#include "components/ipc/workqueue.c"
#endif

#ifdef CONFIG_USE_DATAQUEUE
#include "components/ipc/dataqueue.c"
#endif

#endif
