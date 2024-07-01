/**
 * @file drv_tim.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief N32G003 API适配TIM驱动
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "config.h"

#if defined(CONFIG_BSP_N32G003) && defined(CONFIG_USE_TIM)

#include "bsp/n32g003/library/n32g003_gpio.h"
#include "bsp/n32g003/library/n32g003_misc.h"
#include "bsp/n32g003/library/n32g003_rcc.h"
#include "bsp/n32g003/library/n32g003_tim.h"
#include "components/device/rt_tim.h"
#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

struct n32g003_tim_device {
    struct rt_device_tim tim_dev;
    TIM_Module* module;
    IRQn_Type irqn;
    rt_uint32_t APB_periph;
    rt_uint16_t prescaler;
    rt_uint16_t period;
    double clk_mhz;
};

static struct n32g003_tim_device tim1 = {
    .module = TIM1,
    .irqn = TIM1_BRK_UP_TRG_COM_IRQn,
    .APB_periph = RCC_APB_PERIPH_TIM1,
};

static struct n32g003_tim_device tim3 = {
    .module = TIM3,
    .irqn = TIM3_IRQn,
    .APB_periph = RCC_APB_PERIPH_TIM3,
};

static struct n32g003_tim_device tim6 = {
    .module = TIM6,
    .irqn = TIM6_IRQn,
    .APB_periph = RCC_APB_PERIPH_TIM6,
};

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if (TIM_Flag_Status_Get(TIM1, TIM_FLAG_UPDATE)) {
        TIM_Interrupt_Status_Clear(TIM1, TIM_FLAG_UPDATE);
        if (tim1.tim_dev.callback != RT_NULL) {
            tim1.tim_dev.callback(&tim1.tim_dev.dev, tim1.tim_dev.arg, TIM_EVENT_OVERFLOW);
        }
    }

    if (TIM_Flag_Status_Get(TIM1, TIM_FLAG_BREAK)) {
        TIM_Interrupt_Status_Clear(TIM1, TIM_FLAG_BREAK);
        if (tim1.tim_dev.callback != RT_NULL) {
            tim1.tim_dev.callback(&tim1.tim_dev.dev, tim1.tim_dev.arg, TIM_EVENT_BREAK);
        }
    }

    /* leave interrupt */
    rt_interrupt_leave();
}

void TIM1_CC_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if (TIM_Flag_Status_Get(TIM1, TIM_FLAG_CC1)) {
        TIM_Interrupt_Status_Clear(TIM1, TIM_FLAG_CC1);
        if (tim1.tim_dev.callback != RT_NULL) {
            tim1.tim_dev.callback(&tim1.tim_dev.dev, tim1.tim_dev.arg, TIM_EVENT_CAPTURE);
        }
    }

    if (TIM_Flag_Status_Get(TIM1, TIM_FLAG_CC2)) {
        TIM_Interrupt_Status_Clear(TIM1, TIM_FLAG_CC2);
        if (tim1.tim_dev.callback != RT_NULL) {
            tim1.tim_dev.callback(&tim1.tim_dev.dev, tim1.tim_dev.arg, TIM_EVENT_CAPTURE);
        }
    }

    if (TIM_Flag_Status_Get(TIM1, TIM_FLAG_CC3)) {
        TIM_Interrupt_Status_Clear(TIM1, TIM_FLAG_CC3);
        if (tim1.tim_dev.callback != RT_NULL) {
            tim1.tim_dev.callback(&tim1.tim_dev.dev, tim1.tim_dev.arg, TIM_EVENT_CAPTURE);
        }
    }

    if (TIM_Flag_Status_Get(TIM1, TIM_FLAG_CC4)) {
        TIM_Interrupt_Status_Clear(TIM1, TIM_FLAG_CC4);
        if (tim1.tim_dev.callback != RT_NULL) {
            tim1.tim_dev.callback(&tim1.tim_dev.dev, tim1.tim_dev.arg, TIM_EVENT_CAPTURE);
        }
    }

    /* leave interrupt */
    rt_interrupt_leave();
}

void TIM3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if (TIM_Flag_Status_Get(TIM3, TIM_FLAG_UPDATE)) {
        TIM_Interrupt_Status_Clear(TIM3, TIM_FLAG_UPDATE);
        if (tim3.tim_dev.callback != RT_NULL) {
            tim3.tim_dev.callback(&tim3.tim_dev.dev, tim3.tim_dev.arg, TIM_EVENT_OVERFLOW);
        }
    }

    if (TIM_Flag_Status_Get(TIM3, TIM_FLAG_CC1)) {
        TIM_Interrupt_Status_Clear(TIM3, TIM_FLAG_CC1);
        if (tim3.tim_dev.callback != RT_NULL) {
            tim3.tim_dev.callback(&tim3.tim_dev.dev, tim3.tim_dev.arg, TIM_EVENT_CAPTURE);
        }
    }

    if (TIM_Flag_Status_Get(TIM3, TIM_FLAG_CC2)) {
        TIM_Interrupt_Status_Clear(TIM3, TIM_FLAG_CC2);
        if (tim3.tim_dev.callback != RT_NULL) {
            tim3.tim_dev.callback(&tim3.tim_dev.dev, tim3.tim_dev.arg, TIM_EVENT_CAPTURE);
        }
    }

    /* leave interrupt */
    rt_interrupt_leave();
}

void TIM6_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if (TIM_Flag_Status_Get(TIM6, TIM_FLAG_UPDATE)) {
        TIM_Interrupt_Status_Clear(TIM6, TIM_FLAG_UPDATE);
        if (tim6.tim_dev.callback != RT_NULL) {
            tim6.tim_dev.callback(&tim6.tim_dev.dev, tim6.tim_dev.arg, TIM_EVENT_OVERFLOW);
        }
    }

    /* leave interrupt */
    rt_interrupt_leave();
}

static rt_err_t n32g003_tim_open(const struct rt_device* dev, rt_uint32_t period_ns)
{
    RT_ASSERT(dev != RT_NULL);

    struct n32g003_tim_device* tim = (struct n32g003_tim_device*)(dev->user_data);

    RCC_APB_Peripheral_Clock_Enable(tim->APB_periph);

    TIM_TimeBaseInitType tim_base_init;
    TIM_Base_Struct_Initialize(&tim_base_init);
    tim->prescaler = period_ns / 1365000;
    tim->period = (period_ns / 1000.0f * 48.0f) / (tim->prescaler + 1);
    tim->clk_mhz = (double)48.0 / (double)(tim->prescaler + 1);
    tim_base_init.Prescaler = tim->prescaler;
    tim_base_init.Period = tim->period; // freq = 48MHz/ (Prescaler + 1) / period = 100kHz
    tim_base_init.ClkDiv = 0;
    tim_base_init.CntMode = TIM_CNT_MODE_UP;

    TIM_Base_Initialize(tim->module, &tim_base_init);

    return RT_EOK;
}

static __inline void n32g003_tim_gpio_init(struct n32g003_tim_device* tim, rt_base_t channel, rt_bool_t complementary_en)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current = GPIO_LOW_DREIVE;

    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_AFIO);

    if (tim->module == TIM1) {
        switch (channel) {
#ifndef CONFIG_N32G003_TIM1_CH1_NONE
        case 1:
#if defined(CONFIG_N32G003_TIM1_CH1_PA6)
            GPIO_InitStructure.Pin = GPIO_PIN_6;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH1_PA11)
            GPIO_InitStructure.Pin = GPIO_PIN_11;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH1_PB0)
            GPIO_InitStructure.Pin = GPIO_PIN_0;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB);
            GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
#else
#error "Please define the TIM1_CH1 pin"
#endif
            if (complementary_en == RT_TRUE) {
#if defined(CONFIG_N32G003_TIM1_CH1N_PA5)
                GPIO_InitStructure.Pin = GPIO_PIN_5;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH1N_PA11)
                GPIO_InitStructure.Pin = GPIO_PIN_11;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH1N_PA13)
                GPIO_InitStructure.Pin = GPIO_PIN_13;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#else
#error "Please define the TIM1_CH1N pin"
#endif
            }
            break;
#endif
#ifndef CONFIG_N32G003_TIM1_CH2_NONE
        case 2:
#if defined(CONFIG_N32G003_TIM1_CH2_PA7)
            GPIO_InitStructure.Pin = GPIO_PIN_7;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH2_PA12)
            GPIO_InitStructure.Pin = GPIO_PIN_12;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH2_PA14)
            GPIO_InitStructure.Pin = GPIO_PIN_14;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH2_PA15)
            GPIO_InitStructure.Pin = GPIO_PIN_15;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#else
#error "Please define the TIM1_CH2 pin"
#endif

            if (complementary_en == RT_TRUE) {
#if defined(CONFIG_N32G003_TIM1_CH2N_PA6)
                GPIO_InitStructure.Pin = GPIO_PIN_6;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH2N_PA12)
                GPIO_InitStructure.Pin = GPIO_PIN_12;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH2N_PA13)
                GPIO_InitStructure.Pin = GPIO_PIN_13;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#else
#error "Please define the TIM1_CH2N pin"
#endif
            }
            break;
#endif
#ifndef CONFIG_N32G003_TIM1_CH3_NONE
        case 3:
#if defined(CONFIG_N32G003_TIM1_CH3_PA1)
            GPIO_InitStructure.Pin = GPIO_PIN_1;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH3_PA7)
            GPIO_InitStructure.Pin = GPIO_PIN_7;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH3_PA10)
            GPIO_InitStructure.Pin = GPIO_PIN_10;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH3_PA11)
            GPIO_InitStructure.Pin = GPIO_PIN_11;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#else
#error "Please define the TIM1_CH3 pin"
#endif

            if (complementary_en == RT_TRUE) {
#if defined(CONFIG_N32G003_TIM1_CH3N_PA2)
                GPIO_InitStructure.Pin = GPIO_PIN_2;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH3N_PA3)
                GPIO_InitStructure.Pin = GPIO_PIN_3;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH3N_PA4)
                GPIO_InitStructure.Pin = GPIO_PIN_4;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH3N_PA10)
                GPIO_InitStructure.Pin = GPIO_PIN_10;
                GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
                RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
                GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#else
#error "Please define the TIM1_CH3N pin"
#endif
            }
            break;
#endif
#ifndef CONFIG_N32G003_TIM1_CH4_NONE
        case 4:
#if defined(CONFIG_N32G003_TIM1_CH4_PA3)
            GPIO_InitStructure.Pin = GPIO_PIN_3;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA6)
            GPIO_InitStructure.Pin = GPIO_PIN_6;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA7)
            GPIO_InitStructure.Pin = GPIO_PIN_7;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA8)
            GPIO_InitStructure.Pin = GPIO_PIN_8;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA10)
            GPIO_InitStructure.Pin = GPIO_PIN_10;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA11)
            GPIO_InitStructure.Pin = GPIO_PIN_11;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA12)
            GPIO_InitStructure.Pin = GPIO_PIN_12;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA13)
            GPIO_InitStructure.Pin = GPIO_PIN_13;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA14)
            GPIO_InitStructure.Pin = GPIO_PIN_14;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PA15)
            GPIO_InitStructure.Pin = GPIO_PIN_15;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM1_CH4_PB1)
            GPIO_InitStructure.Pin = GPIO_PIN_1;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM1;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB);
            GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
#else
#error "Please define the TIM1_CH4 pin"
#endif
            break;
#endif
        default:
            break;
        }
    } else if (tim->module == TIM3) {
        switch (channel) {
#ifndef CONFIG_N32G003_TIM3_CH1_NONE
        case 1:
#if defined(CONFIG_N32G003_TIM3_CH1_PA1)
            GPIO_InitStructure.Pin = GPIO_PIN_6;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA3)
            GPIO_InitStructure.Pin = GPIO_PIN_3;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA6)
            GPIO_InitStructure.Pin = GPIO_PIN_6;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA7)
            GPIO_InitStructure.Pin = GPIO_PIN_7;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA10)
            GPIO_InitStructure.Pin = GPIO_PIN_10;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA11)
            GPIO_InitStructure.Pin = GPIO_PIN_11;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA12)
            GPIO_InitStructure.Pin = GPIO_PIN_12;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA13)
            GPIO_InitStructure.Pin = GPIO_PIN_13;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA14)
            GPIO_InitStructure.Pin = GPIO_PIN_14;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH1_PA15)
            GPIO_InitStructure.Pin = GPIO_PIN_15;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#else
#error "Please define the TIM3_CH1 pin"
#endif
            break;
#endif
#ifndef CONFIG_N32G003_TIM3_CH2_NONE
        case 2:
#if defined(CONFIG_N32G003_TIM3_CH2_PA2)
            GPIO_InitStructure.Pin = GPIO_PIN_2;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA3)
            GPIO_InitStructure.Pin = GPIO_PIN_3;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA6)
            GPIO_InitStructure.Pin = GPIO_PIN_6;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA7)
            GPIO_InitStructure.Pin = GPIO_PIN_7;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA10)
            GPIO_InitStructure.Pin = GPIO_PIN_10;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA11)
            GPIO_InitStructure.Pin = GPIO_PIN_11;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA12)
            GPIO_InitStructure.Pin = GPIO_PIN_12;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA13)
            GPIO_InitStructure.Pin = GPIO_PIN_13;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA14)
            GPIO_InitStructure.Pin = GPIO_PIN_14;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#elif defined(CONFIG_N32G003_TIM3_CH2_PA15)
            GPIO_InitStructure.Pin = GPIO_PIN_15;
            GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
            RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
            GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
#else
#error "Please define the TIM3_CH2 pin"
#endif
            break;
#endif
        default:
            break;
        }
    } else {
        return;
    }
}

static rt_err_t n32g003_tim_config(const struct rt_device* dev, rt_base_t channel, const struct rt_tim_config* config)
{
    RT_ASSERT(dev != RT_NULL);
    struct n32g003_tim_device* tim = (struct n32g003_tim_device*)(dev->user_data);

    if (channel < 0 || channel > 5) {
        return RT_EINVAL;
    }

    TIM_ICInitType ic_init;
    OCInitType oc_init;

    switch (config->mode) {
    case TIM_MODE_CAPTURE:

        TIM_Input_Struct_Initialize(&ic_init);
        switch (channel) {
        case 1:
            ic_init.Channel = TIM_CH_1;
            break;
        case 2:
            ic_init.Channel = TIM_CH_2;
            break;
        case 3:
            ic_init.Channel = TIM_CH_3;
            break;
        case 4:
            ic_init.Channel = TIM_CH_4;
            break;
        case 5:
            ic_init.Channel = TIM_CH_5;
            break;
        default:
            break;
        }
        ic_init.IcPolarity = TIM_IC_POLARITY_RISING;
        ic_init.IcSelection = TIM_IC_SELECTION_DIRECTTI;
        ic_init.IcPrescaler = TIM_IC_PSC_DIV1;
        ic_init.IcFilter = config->capture_config.filter;
        TIM_Input_Channel_Initialize(tim->module, &ic_init);
        TIM_Interrupt_Enable(TIM3, TIM_INT_CC1 | TIM_INT_CC2);
        break;
    case TIM_MODE_PWM:

        n32g003_tim_gpio_init(tim, channel, config->pwm_config.complementary_en);

        TIM_Output_Channel_Struct_Initialize(&oc_init);

        if (config->cnt_mode == TIM_CNTMODE_DN) {
            TIM_Base_Count_Mode_Set(tim->module, TIM_CNT_MODE_DOWN);
        } else if (config->cnt_mode == TIM_CNTMODE_UPDN) {
            TIM_Base_Count_Mode_Set(tim->module, TIM_CNT_MODE_CENTER_ALIGN3);
        }

        oc_init.OcMode = TIM_OCMODE_PWM2;
        oc_init.OutputState = TIM_OUTPUT_STATE_ENABLE;
        if (config->pwm_config.complementary_en == TIM_PWM_COMPLEMENTARY) {
            oc_init.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
        }
        oc_init.Pulse = 0;

        if (config->pwm_config.polarity == TIM_PWM_LOWACTIVE) {
            oc_init.OcPolarity = TIM_OC_POLARITY_LOW;
            oc_init.OcNPolarity = TIM_OC_POLARITY_LOW;
        }

        if (config->pwm_config.idle_state == TIM_PWM_IDLESTATE_HIGH) {
            oc_init.OcIdleState = TIM_OC_IDLE_STATE_SET;
            oc_init.OcNIdleState = TIM_OC_IDLE_STATE_SET;
        }

        switch (channel) {
        case 1:
            TIM_Output_Channel1_Initialize(tim->module, &oc_init);
            break;
        case 2:
            TIM_Output_Channel2_Initialize(tim->module, &oc_init);
            break;
        case 3:
            TIM_Output_Channel3_Initialize(tim->module, &oc_init);
            break;
        case 4:
            TIM_Output_Channel4_Initialize(tim->module, &oc_init);
            break;
        case 5:
            TIM_Output_Channel5_Initialize(tim->module, &oc_init);
            break;
        default:
            break;
        }

        if (config->pwm_config.dt_en == TIM_PWM_DT_ENABLE || config->pwm_config.bk_en == TIM_PWM_BK_ENABLE) {
            TIM_BDTRInitType bdtr_init;
            TIM_Break_And_Dead_Time_Struct_Initialize(&bdtr_init);
            bdtr_init.OssrState = TIM_OSSR_STATE_DISABLE;
            bdtr_init.OssiState = TIM_OSSR_STATE_DISABLE;
            bdtr_init.LockLevel = TIM_LOCK_LEVEL_1;
            bdtr_init.DeadTime = 0;
            if (config->pwm_config.bk_en == TIM_PWM_BK_ENABLE) {
                bdtr_init.Break = TIM_BREAK_IN_ENABLE;
            }

            if (config->pwm_config.bk_polarity == TIM_PWM_BK_HIGHACTIVE) {
                bdtr_init.BreakPolarity = TIM_BREAK_POLARITY_HIGH;
            } else {
                bdtr_init.BreakPolarity = TIM_BREAK_POLARITY_LOW;
            }
            bdtr_init.AutomaticOutput = TIM_AUTO_OUTPUT_ENABLE;
            TIM_Break_And_Dead_Time_Set(tim->module, &bdtr_init);
        }

        TIM_PWM_Output_Enable(tim->module);
        break;
    case TIM_MODE_TOGGLE:
        n32g003_tim_gpio_init(tim, channel, config->pwm_config.complementary_en);

        TIM_Output_Channel_Struct_Initialize(&oc_init);

        if (config->cnt_mode == TIM_CNTMODE_DN) {
            TIM_Base_Count_Mode_Set(tim->module, TIM_CNT_MODE_DOWN);
        } else if (config->cnt_mode == TIM_CNTMODE_UPDN) {
            TIM_Base_Count_Mode_Set(tim->module, TIM_CNT_MODE_CENTER_ALIGN3);
        }

        oc_init.OcMode = TIM_OCMODE_TOGGLE;
        oc_init.OutputState = TIM_OUTPUT_STATE_ENABLE;
        oc_init.Pulse = 0;

        switch (channel) {
        case 1:
            TIM_Output_Channel1_Initialize(tim->module, &oc_init);
            TIM_Output_Channel1_Preload_Set(tim->module, TIM_OC_PRELOAD_DISABLE);
            break;
        case 2:
            TIM_Output_Channel2_Initialize(tim->module, &oc_init);
            TIM_Output_Channel2_Preload_Set(tim->module, TIM_OC_PRELOAD_DISABLE);
            break;
        case 3:
            TIM_Output_Channel3_Initialize(tim->module, &oc_init);
            TIM_Output_Channel3_Preload_Set(tim->module, TIM_OC_PRELOAD_DISABLE);
            break;
        case 4:
            TIM_Output_Channel4_Initialize(tim->module, &oc_init);
            TIM_Output_Channel4_Preload_Set(tim->module, TIM_OC_PRELOAD_DISABLE);
            break;
        case 5:
            TIM_Output_Channel5_Initialize(tim->module, &oc_init);
            TIM_Output_Channel5_Preload_Set(tim->module, TIM_OC_PRELOAD_DISABLE);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return RT_EOK;
}

static void n32g003_tim_start(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct n32g003_tim_device* tim = (struct n32g003_tim_device*)(dev->user_data);
    if (tim->tim_dev.callback != RT_NULL) {
        TIM_Update_Event_Enable(tim->module);
        TIM_Update_Request_Source_Set(tim->module, TIM_UPDATE_SRC_REGULAR);
        TIM_Interrupt_Enable(tim->module, TIM_INT_UPDATE);
        NVIC_EnableIRQ(tim->irqn);
    }
    TIM_On(tim->module);
}

static void n32g003_tim_stop(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct n32g003_tim_device* tim = (struct n32g003_tim_device*)(dev->user_data);
    TIM_Off(tim->module);
    NVIC_DisableIRQ(tim->irqn);
    TIM_Interrupt_Disable(tim->module, TIM_INT_UPDATE);
    TIM_Update_Event_Disable(tim->module);
}

static rt_uint32_t n32g003_tim_count(const struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct n32g003_tim_device* tim = (struct n32g003_tim_device*)(dev->user_data);
    return TIM_Base_Count_Get(tim->module);
}

static rt_err_t n32g003_tim_pwm_set_pulse(const struct rt_device* dev, rt_base_t channel, rt_uint32_t pulse_ns)
{
    RT_ASSERT(dev != RT_NULL);
    struct n32g003_tim_device* tim = (struct n32g003_tim_device*)(dev->user_data);
    rt_uint16_t pulse = pulse_ns * tim->clk_mhz / 1000.0f;
    if (pulse > tim->period) {
        return RT_EINVAL;
    }
    switch (channel) {
    case 1:
        TIM_Compare1_Set(tim->module, pulse);
        break;
    case 2:
        TIM_Compare2_Set(tim->module, pulse);
        break;
    case 3:
        TIM_Compare3_Set(tim->module, pulse);
        break;
    case 4:
        TIM_Compare4_Set(tim->module, pulse);
        break;
    case 5:
        TIM_Compare5_Set(tim->module, pulse);
        break;
    default:
        break;
    }
    return RT_EOK;
}

static rt_err_t n32g003_tim_pwm_set_deadtime(const struct rt_device* dev, rt_base_t channel, rt_uint32_t deadtime_ns)
{
    RT_ASSERT(dev != RT_NULL);
    struct n32g003_tim_device* tim = (struct n32g003_tim_device*)(dev->user_data);

    rt_uint8_t dtgn = 0x00;

    /**
     * @note 
     *  DTGN[7:5]=0xx => DT=DTGN[7:0] × Tdtgn，Tdtgn = TDTS；
     *  DTGN[7:5]=10x => DT=(64+DTGN[5:0]) × Tdtgn，Tdtgn = 2 × TDTS；
     *  DTGN[7:5]=110 => DT=(32+DTGN[4:0]) × Tdtgn，Tdtgn = 8 × TDTS；
     *  DTGN[7:5]=111 => DT=(32+DTGN[4:0])× Tdtgn，Tdtgn = 16 × TDTS；

     *  deadtime_ns > 752 * tCLK  deadtime: error
     *  deadtime_ns > 511 * tCLK, deadtime: 512*tCLK ~ 752*tCLK
     *  deadtime_ns > 376 * tCLK, deadtime: 512*tCLK
     *  deadtime_ns > 255 * tCLK, deadtime: 256*tCLK ~ 376*tCLK
     *  deadtime_ns > 190 * tCLK, deadtime: 256*tCLK
     *  deadtime_ns > 127 * tCLK, deadtime: 128*tCLK ~ 190*tCLK
     *  deadtime: 0*tCLK ~ 127*tCLK
     */

    if (deadtime_ns > (752000.f / tim->clk_mhz)) {
        return RT_EINVAL;
    } else if (deadtime_ns > (376000.0f / tim->clk_mhz)) {
        dtgn = ((rt_uint8_t)((deadtime_ns * tim->clk_mhz) / 16000.0f - 32.0f) & 0x0F) | 0xE0;
    } else if (deadtime_ns > (190000.0f / tim->clk_mhz)) {
        dtgn = ((rt_uint8_t)((deadtime_ns * tim->clk_mhz) / 8000.0f - 32.0f) & 0x0F) | 0xC0;
    } else if (deadtime_ns > (127000.0f / tim->clk_mhz)) {
        dtgn = ((rt_uint8_t)((deadtime_ns * tim->clk_mhz) / 2000.0f - 64.0f) & 0x1F) | 0x80;
    } else {
        dtgn = (rt_uint8_t)((deadtime_ns * tim->clk_mhz) / 1000.0f) & 0x7F;
    }

    tim->module->BKDT &= 0xFFFFFF00;
    tim->module->BKDT |= dtgn;
    return RT_EOK;
}

static const struct rt_tim_ops _n32g003_tim_ops = {
    .open = n32g003_tim_open,
    .config = n32g003_tim_config,
    .start = n32g003_tim_start,
    .stop = n32g003_tim_stop,
    .count = n32g003_tim_count,
    .pwm_set_pulse = n32g003_tim_pwm_set_pulse,
    .pwm_set_deadtime = n32g003_tim_pwm_set_deadtime,
};

int _n32g003_tim_init(void)
{
    int ret = 0;
#ifdef CONFIG_BSP_TIM1
    ret = rt_device_tim_register(&tim1.tim_dev, "tim1", &_n32g003_tim_ops, (void*)&tim1);
#endif

#ifdef CONFIG_BSP_TIM3
    ret = rt_device_tim_register(&tim3.tim_dev, "tim3", &_n32g003_tim_ops, (void*)&tim3);
#endif

#ifdef CONFIG_BSP_TIM6
    ret = rt_device_tim_register(&tim6.tim_dev, "tim6", &_n32g003_tim_ops, (void*)&tim6);
#endif

    return ret;
}
INIT_DEVICE_EXPORT(_n32g003_tim_init);

#ifdef __cplusplus
}
#endif

#endif
