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

#if CONFIG_USE_DEVICE

#ifdef CONFIG_USE_PIN
#include "components/device/rt_pin.h"
#endif

#ifdef CONFIG_USE_SERIAL
#include "components/device/rt_serial.h"
#endif

#ifdef CONFIG_USE_ADC
#include "components/device/rt_adc.h"
#endif

#ifdef CONFIG_USE_DAC
#include "components/device/rt_dac.h"
#endif

#ifdef CONFIG_USE_FLASH
#include "components/device/rt_flash.h"
#endif

#ifdef CONFIG_USE_I2C
#include "components/device/rt_i2c.h"
#endif

#ifdef CONFIG_USE_PWM
#include "components/device/rt_pwm.h"
#endif

#ifdef CONFIG_USE_RTC
#include "components/device/rt_rtc.h"
#endif

#ifdef CONFIG_USE_SPI
#include "components/device/rt_spi.h"
#endif

#ifdef CONFIG_USE_TIM
#include "components/device/rt_tim.h"
#endif

#ifdef CONFIG_USE_WDT
#include "components/device/rt_wdt.h"
#endif

#ifdef CONFIG_USE_RINGBUFFER
#include "components/ringbuffer/ringbuffer.h"
#endif

#endif
