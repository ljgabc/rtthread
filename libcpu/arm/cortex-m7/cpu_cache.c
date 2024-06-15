/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-04-02     tanek        first implementation
 * 2019-04-27     misonyo      update to cortex-m7 series
 */

#include <rtdef.h>
#include <rthw.h>

/* following defines should be used for structure members */
#define __IM volatile const /*! Defines 'read only' structure member permissions */
#define __OM volatile /*! Defines 'write only' structure member permissions */
#define __IOM volatile /*! Defines 'read / write' structure member permissions */

/* Memory mapping of Core Hardware */
#define SCS_BASE (0xE000E000UL) /*!< System Control Space Base Address */
#define ITM_BASE (0xE0000000UL) /*!< ITM Base Address */
#define DWT_BASE (0xE0001000UL) /*!< DWT Base Address */
#define TPI_BASE (0xE0040000UL) /*!< TPI Base Address */
#define CoreDebug_BASE (0xE000EDF0UL) /*!< Core Debug Base Address */
#define SysTick_BASE (SCS_BASE + 0x0010UL) /*!< SysTick Base Address */
#define NVIC_BASE (SCS_BASE + 0x0100UL) /*!< NVIC Base Address */
#define SCB_BASE (SCS_BASE + 0x0D00UL) /*!< System Control Block Base Address */

/**
  \brief  Structure type to access the System Control Block (SCB).
 */
typedef struct
{
    __IM rt_uint32_t CPUID; /*!< Offset: 0x000 (R/ )  CPUID Base Register */
    __IOM rt_uint32_t ICSR; /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
    __IOM rt_uint32_t VTOR; /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
    __IOM rt_uint32_t AIRCR; /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
    __IOM rt_uint32_t SCR; /*!< Offset: 0x010 (R/W)  System Control Register */
    __IOM rt_uint32_t CCR; /*!< Offset: 0x014 (R/W)  Configuration Control Register */
    __IOM rt_uint8_t SHPR[12U]; /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
    __IOM rt_uint32_t SHCSR; /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
    __IOM rt_uint32_t CFSR; /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
    __IOM rt_uint32_t HFSR; /*!< Offset: 0x02C (R/W)  HardFault Status Register */
    __IOM rt_uint32_t DFSR; /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
    __IOM rt_uint32_t MMFAR; /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
    __IOM rt_uint32_t BFAR; /*!< Offset: 0x038 (R/W)  BusFault Address Register */
    __IOM rt_uint32_t AFSR; /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
    __IM rt_uint32_t ID_PFR[2U]; /*!< Offset: 0x040 (R/ )  Processor Feature Register */
    __IM rt_uint32_t ID_DFR; /*!< Offset: 0x048 (R/ )  Debug Feature Register */
    __IM rt_uint32_t ID_AFR; /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register */
    __IM rt_uint32_t ID_MFR[4U]; /*!< Offset: 0x050 (R/ )  Memory Model Feature Register */
    __IM rt_uint32_t ID_ISAR[5U]; /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register */
    rt_uint32_t RESERVED0[1U];
    __IM rt_uint32_t CLIDR; /*!< Offset: 0x078 (R/ )  Cache Level ID register */
    __IM rt_uint32_t CTR; /*!< Offset: 0x07C (R/ )  Cache Type register */
    __IM rt_uint32_t CCSIDR; /*!< Offset: 0x080 (R/ )  Cache Size ID Register */
    __IOM rt_uint32_t CSSELR; /*!< Offset: 0x084 (R/W)  Cache Size Selection Register */
    __IOM rt_uint32_t CPACR; /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */
    rt_uint32_t RESERVED3[93U];
    __OM rt_uint32_t STIR; /*!< Offset: 0x200 ( /W)  Software Triggered Interrupt Register */
    rt_uint32_t RESERVED4[15U];
    __IM rt_uint32_t MVFR0; /*!< Offset: 0x240 (R/ )  Media and VFP Feature Register 0 */
    __IM rt_uint32_t MVFR1; /*!< Offset: 0x244 (R/ )  Media and VFP Feature Register 1 */
    __IM rt_uint32_t MVFR2; /*!< Offset: 0x248 (R/ )  Media and VFP Feature Register 2 */
    rt_uint32_t RESERVED5[1U];
    __OM rt_uint32_t ICIALLU; /*!< Offset: 0x250 ( /W)  I-Cache Invalidate All to PoU */
    rt_uint32_t RESERVED6[1U];
    __OM rt_uint32_t ICIMVAU; /*!< Offset: 0x258 ( /W)  I-Cache Invalidate by MVA to PoU */
    __OM rt_uint32_t DCIMVAC; /*!< Offset: 0x25C ( /W)  D-Cache Invalidate by MVA to PoC */
    __OM rt_uint32_t DCISW; /*!< Offset: 0x260 ( /W)  D-Cache Invalidate by Set-way */
    __OM rt_uint32_t DCCMVAU; /*!< Offset: 0x264 ( /W)  D-Cache Clean by MVA to PoU */
    __OM rt_uint32_t DCCMVAC; /*!< Offset: 0x268 ( /W)  D-Cache Clean by MVA to PoC */
    __OM rt_uint32_t DCCSW; /*!< Offset: 0x26C ( /W)  D-Cache Clean by Set-way */
    __OM rt_uint32_t DCCIMVAC; /*!< Offset: 0x270 ( /W)  D-Cache Clean and Invalidate by MVA to PoC */
    __OM rt_uint32_t DCCISW; /*!< Offset: 0x274 ( /W)  D-Cache Clean and Invalidate by Set-way */
    __OM rt_uint32_t BPIALL; /*!< Offset: 0x278 ( /W)  Branch Predictor Invalidate All */
    rt_uint32_t RESERVED7[5U];
    __IOM rt_uint32_t ITCMCR; /*!< Offset: 0x290 (R/W)  Instruction Tightly-Coupled Memory Control Register */
    __IOM rt_uint32_t DTCMCR; /*!< Offset: 0x294 (R/W)  Data Tightly-Coupled Memory Control Registers */
    __IOM rt_uint32_t AHBPCR; /*!< Offset: 0x298 (R/W)  AHBP Control Register */
    __IOM rt_uint32_t CACR; /*!< Offset: 0x29C (R/W)  L1 Cache Control Register */
    __IOM rt_uint32_t AHBSCR; /*!< Offset: 0x2A0 (R/W)  AHB Slave Control Register */
    rt_uint32_t RESERVED8[1U];
    __IOM rt_uint32_t ABFSR; /*!< Offset: 0x2A8 (R/W)  Auxiliary Bus Fault Status Register */
} SCB_Type;

#define SCB ((SCB_Type*)SCB_BASE) /*!< SCB configuration struct */

/* The L1-caches on all Cortex®-M7s are divided into lines of 32 bytes. */
#define L1CACHE_LINESIZE_BYTE (32)

void rt_hw_cpu_icache_enable(void)
{
    SCB_EnableICache();
}

void rt_hw_cpu_icache_disable(void)
{
    SCB_DisableICache();
}

rt_base_t rt_hw_cpu_icache_status(void)
{
    return 0;
}

void rt_hw_cpu_icache_ops(int ops, void* addr, int size)
{
    rt_uint32_t address = (rt_uint32_t)addr & (rt_uint32_t) ~(L1CACHE_LINESIZE_BYTE - 1);
    rt_int32_t size_byte = size + address - (rt_uint32_t)addr;
    rt_uint32_t linesize = 32U;
    if (ops & RT_HW_CACHE_INVALIDATE) {
        __DSB();
        while (size_byte > 0) {
            SCB->ICIMVAU = address;
            address += linesize;
            size_byte -= linesize;
        }
        __DSB();
        __ISB();
    }
}

void rt_hw_cpu_dcache_enable(void)
{
    SCB_EnableDCache();
}

void rt_hw_cpu_dcache_disable(void)
{
    SCB_DisableDCache();
}

rt_base_t rt_hw_cpu_dcache_status(void)
{
    return 0;
}

void rt_hw_cpu_dcache_ops(int ops, void* addr, int size)
{
    rt_uint32_t startAddr = (rt_uint32_t)addr & (rt_uint32_t) ~(L1CACHE_LINESIZE_BYTE - 1);
    rt_uint32_t size_byte = size + (rt_uint32_t)addr - startAddr;

    if (ops & (RT_HW_CACHE_FLUSH | RT_HW_CACHE_INVALIDATE)) {
        SCB_CleanInvalidateDCache_by_Addr((rt_uint32_t*)startAddr, size_byte);
    } else if (ops & RT_HW_CACHE_FLUSH) {
        SCB_CleanDCache_by_Addr((rt_uint32_t*)startAddr, size_byte);
    } else if (ops & RT_HW_CACHE_INVALIDATE) {
        SCB_InvalidateDCache_by_Addr((rt_uint32_t*)startAddr, size_byte);
    } else {
        RT_ASSERT(0);
    }
}
