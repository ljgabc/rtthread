/**
 * @file drv_n32g003.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "config.h"

#include "mcu/n32g003/library/n32g003_rcc.c"
#include "mcu/n32g003/library/n32g003_flash.c"
#include "mcu/n32g003/library/n32g003_misc.c"
#include "mcu/n32g003/system/system_n32g003.c"
#include "mcu/n32g003/system/system_n32g003.h"

#ifdef CONFIG_USE_ADC
#include "mcu/n32g003/bsp/drv_n32g003_adc.c"
#include "mcu/n32g003/library/n32g003_adc.c"
#endif

#ifdef CONFIG_USE_PIN
#include "mcu/n32g003/bsp/drv_n32g003_pin.c"
#include "mcu/n32g003/library/n32g003_exti.c"
#include "mcu/n32g003/library/n32g003_gpio.c"
#endif

#ifdef CONFIG_USE_SERIAL
#include "mcu/n32g003/bsp/drv_n32g003_serial.c"
#include "mcu/n32g003/library/n32g003_uart.c"
#endif

#ifdef CONFIG_USE_SPI
#include "mcu/n32g003/bsp/drv_spi.c"
#include "mcu/n32g003/library/n32g003_spi.c"
#endif

#ifdef CONFIG_USE_I2C
#include "mcu/n32g003/bsp/drv_i2c.c"
#include "mcu/n32g003/library/n32g003_i2c.c"
#endif

static void System_Clock_Initializes(void)
{
    RCC_Hclk_Config(RCC_SYSCLK_DIV1);
    RCC_Pclk_Config(RCC_HCLK_DIV1);
    RCC_TIM1_Clock_Config(RCC_TIM1_CLKSRC_SYSCLK);
    RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_DIV48);
    RCC_ADC_Clock_Config(RCC_ADC_DIV2);
    RCC_TIM6_Clock_Config(RCC_TIM6_CLKSRC_PCLK);
}

KK_INLINE void _n32g003_board_init(void)
{
    System_Core_Clock_Frequency_Update();
    System_Clock_Initializes();
#if CONFIG_SYSTICK_FREQ
    SysTick_Config(SystemCoreClockFrequency / CONFIG_SYSTICK_FREQ);
#endif
}
KK_INIT_BOARD_EXPORT(_n32g003_board_init);