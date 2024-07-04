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

#if CONFIG_USING_DEVICE

#include "components/device/rt_device.c"

#ifdef CONFIG_USING_PIN
#include "components/device/rt_pin.c"
#endif

#ifdef CONFIG_USING_SERIAL
#include "components/device/rt_serial.c"
#endif

#ifdef CONFIG_USING_ADC
#include "components/device/rt_adc.c"
#endif

#ifdef CONFIG_USING_DAC
#include "components/device/rt_dac.c"
#endif

#ifdef CONFIG_USING_FLASH
#include "components/device/rt_flash.c"
#endif

#ifdef CONFIG_USING_I2C
#include "components/device/rt_i2c.c"
#endif

#ifdef CONFIG_USING_RTC
#include "components/device/rt_rtc.c"
#endif

#ifdef CONFIG_USING_CRC
#include "components/device/rt_crc.c"
#endif

#ifdef CONFIG_USING_SPI
#include "components/device/rt_spi.c"
#endif

#ifdef CONFIG_USING_TIM
#include "components/device/rt_tim.c"
#endif

#ifdef CONFIG_USING_WDT
#include "components/device/rt_wdt.c"
#endif

#ifdef CONFIG_USING_RINGBUFFER
#include "components/ipc/ringbuffer.c"
#endif

#ifdef CONFIG_USING_COMPLETION
#include "components/ipc/completion.c"
#endif

#ifdef CONFIG_USING_WORKQUEUE
#include "components/ipc/workqueue.c"
#endif

#ifdef CONFIG_USING_DATAQUEUE
#include "components/ipc/dataqueue.c"
#endif

#endif
