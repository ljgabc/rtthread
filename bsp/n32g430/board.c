/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */

#include "config.h"
#include <rthw.h>
#include <rtthread.h>
#include "bsp/n32g430/system/system_n32g430.h"

#ifdef CONFIG_USE_RTOS

#define _SCB_BASE (0xE000E010UL)
#define _SYSTICK_CTRL (*(rt_uint32_t*)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD (*(rt_uint32_t*)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL (*(rt_uint32_t*)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB (*(rt_uint32_t*)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI (*(rt_uint8_t*)(0xE000ED23UL))

// #define ROM_START              ((uint32_t)0x08000000)
// #define ROM_SIZE               (64 * 1024)
// #define ROM_END                ((uint32_t)(ROM_START + ROM_SIZE))
 
#define RAM_START              (0x20000000)
#define RAM_SIZE               (16 * 1024)
#define RAM_END                (RAM_START + RAM_SIZE)	

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      ((void *)&__bss_end)
#endif
 
#define HEAP_END                (RAM_END)

static rt_uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFF) {
        return 1;
    }

    _SYSTICK_LOAD = ticks - 1;
    _SYSTICK_PRI = 0xFF;
    _SYSTICK_VAL = 0;
    _SYSTICK_CTRL = 0x07;

    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
// #define RT_HEAP_SIZE CONFIG_HEAP_SIZE
// static rt_uint32_t rt_heap[RT_HEAP_SIZE]; // heap default size: 4K(1024 * 4)
// RT_WEAK void* rt_heap_begin_get(void)
// {
//     return rt_heap;
// }

// RT_WEAK void* rt_heap_end_get(void)
// {
//     return rt_heap + RT_HEAP_SIZE;
// }
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    /* System Clock Update */
    System_Core_Clock_Frequency_Update();

    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClockFrequency / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init((void*)HEAP_BEGIN, (void*)HEAP_END);
#endif
}

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

#endif

#include "bsp/n32g430/library/n32g430_rcc.c"
#include "bsp/n32g430/library/n32g430_flash.c"
#include "bsp/n32g430/library/n32g430_misc.c"
#include "bsp/n32g430/system/system_n32g430.c"

#ifdef CONFIG_USE_ADC
#include "bsp/n32g430/driver/drv_adc.c"
#include "bsp/n32g430/library/n32g430_adc.c"
#endif

#ifdef CONFIG_USE_PIN
#include "bsp/n32g430/driver/drv_pin.c"
#include "bsp/n32g430/library/n32g430_exti.c"
#include "bsp/n32g430/library/n32g430_gpio.c"
#endif

#ifdef CONFIG_USE_SERIAL
#include "bsp/n32g430/driver/drv_serial.c"
#include "bsp/n32g430/library/n32g430_uart.c"
#endif

#ifdef CONFIG_USE_SPI
#include "mcu/n32g430/bsp/drv_spi.c"
#include "mcu/n32g430/library/n32g430_spi.c"
#endif

#ifdef CONFIG_USE_I2C
#include "bsp/n32g430/driver/drv_i2c.c"
#include "bsp/n32g430/library/n32g430_i2c.c"
#endif