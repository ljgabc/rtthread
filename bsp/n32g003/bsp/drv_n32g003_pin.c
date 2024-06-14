/**
 * @file drv_n32g003_pin.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief N32G003 API适配PIN驱动
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "config.h"

#if defined(CONFIG_N32G003) && defined(CONFIG_USE_PIN)

#include "common/kk_dev.h"
#include "hal/kk_pin.h"
#include "mcu/n32g003/library/n32g003_exti.h"
#include "mcu/n32g003/library/n32g003_gpio.h"
#include "mcu/n32g003/library/n32g003_misc.h"
#include "mcu/n32g003/library/n32g003_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 引脚描述，
 * index：引脚在芯片上的物理编号
 * port：引脚所属端口，如A、B、C、D
 * pin：引脚在端口的序列号，如PA1，pin就是1
 */
#define __PIN(index, port, pin)                                      \
    {                                                                \
        index, RCC_APB_PERIPH_IOP##port, GPIO##port, GPIO_PIN_##pin, \
            EXTI_LINE##pin                                           \
    }

#define __PIN_DEFAULT \
    {                 \
        -1, 0, 0, 0, 0 \
    }

struct gpio_desc {
    int32_t index; // 引脚物理编号
    uint32_t clk; // 引脚的时钟源
    GPIO_Module* periph; // 引脚所在总线端口
    uint16_t pin; // 引脚在端口的序列号
    uint32_t isr_line;
};

struct gpio_isr_desc {
    void (*isr)(void* arg);
    void* arg;
};

static struct gpio_isr_desc gpio_isrs[18] = { NULL };

// N32G003有两种封装，QFN20和TSSOP20
static const struct gpio_desc gpios[] = {
#ifdef CONFIG_N32G003F5Q7
    __PIN(1, A, 0),
    __PIN(2, A, 1),
    __PIN(3, A, 2),
    __PIN_DEFAULT,
    __PIN(5, A, 8),
    __PIN_DEFAULT,
    __PIN(7, A, 3),
    __PIN(8, A, 5),
    __PIN(9, A, 4),
    __PIN(10, A, 11),
    __PIN(11, A, 13),
    __PIN(12, A, 15),
    __PIN(13, A, 6),
    __PIN(14, A, 7),
    __PIN(15, A, 9),
    __PIN(16, A, 10),
    __PIN(17, A, 12),
    __PIN(18, A, 14),
    __PIN(19, B, 0),
    __PIN(20, B, 1),
#else
    __PIN(1, A, 14),
    __PIN(2, B, 0),
    __PIN(3, B, 1),
    __PIN(4, A, 0),
    __PIN(5, A, 1),
    __PIN(6, A, 2),
    __PIN_DEFAULT,
    __PIN(8, A, 8),
    __PIN_DEFAULT,
    __PIN(10, A, 3),
    __PIN(11, A, 5),
    __PIN(12, A, 4),
    __PIN(13, A, 11),
    __PIN(14, A, 13),
    __PIN(15, A, 15),
    __PIN(16, A, 6),
    __PIN(17, A, 7),
    __PIN(18, A, 9),
    __PIN(19, A, 10),
    __PIN(20, A, 12),
#endif
};

/**
 * @brief 通过引脚物理编号获取引脚描述结构体
 * 
 * @param pin 引脚编号
 * @return 引脚描述结构体
 */
KK_INLINE struct gpio_desc* _get_gpio(int32_t pin)
{
    if (pin == 0 || pin >= kk_length_of(gpios)) {
        return NULL;
    }

    pin = pin - 1;
		
		if(gpios[pin].index == -1) {
			return NULL;
		}
		
    return &gpios[pin];
}

/**
 * @brief GPIO引脚输入输出配置
 *
 * @param pin   编号
 * @param mode  模式
 * @return 成功返回OK，其他见错误码
 */
static kk_err_t _n32g003_pin_mode(const struct kk_dev* dev, int32_t pin, int32_t mode)
{
    const struct gpio_desc* gpio = _get_gpio(pin);
    if (!gpio) {
        return KK_INVALID;
    }

    if (mode == PIN_MODE_DISABLE) {
        return KK_OK;
    }

    RCC_APB_Peripheral_Clock_Enable(gpio->clk | RCC_APB_PERIPH_AFIO);

    GPIO_InitType gpio_init_struct;
    GPIO_Structure_Initialize(&gpio_init_struct);

    gpio_init_struct.Pin = gpio->pin;
    switch (mode) {
    case PIN_MODE_OUTPUT:
        gpio_init_struct.GPIO_Mode = GPIO_MODE_OUT_PP;
        gpio_init_struct.GPIO_Pull = GPIO_NO_PULL;
        break;
    case PIN_MODE_OUTPUT_OD:
        gpio_init_struct.GPIO_Mode = GPIO_MODE_OUT_OD;
        gpio_init_struct.GPIO_Pull = GPIO_NO_PULL;
        break;
    case PIN_MODE_INPUT:
        gpio_init_struct.GPIO_Mode = GPIO_MODE_INPUT;
        gpio_init_struct.GPIO_Pull = GPIO_NO_PULL;
        break;
    case PIN_MODE_INPUT_PULLUP:
        gpio_init_struct.GPIO_Mode = GPIO_MODE_INPUT;
        gpio_init_struct.GPIO_Pull = GPIO_PULL_UP;
        break;
    case PIN_MODE_INPUT_PULLDN:
        gpio_init_struct.GPIO_Mode = GPIO_MODE_INPUT;
        gpio_init_struct.GPIO_Pull = GPIO_PULL_DOWN;
        break;
    default:
        break;
    }
    GPIO_Peripheral_Initialize(gpio->periph, &gpio_init_struct);
    return KK_OK;
}

/**
 * @brief 设置引脚中断处理函数
 *
 * @param pin       编号
 * @param mode      中断模式
 * @param hdr       中断处理函数
 * @param args      传递给中断处理函数的参数
 * @return kk_err_t 成功返回OK，其他见错误码
 */
static kk_err_t _n32g003_pin_enable_irq(const struct kk_dev* dev, int32_t pin, int32_t mode,
    void (*hdr)(void* args), const void* args)
{
    const struct gpio_desc* gpio = _get_gpio(pin);
    if (!gpio) {
        return KK_INVALID;
    }

    if (mode == PIN_IRQ_MODE_HIGH || mode == PIN_IRQ_MODE_LOW || PIN_IRQ_MODE_DISABLE) {
        return KK_INVALID;
    }

    gpio_isrs[gpio->pin].isr = hdr;
    gpio_isrs[gpio->pin].arg = args;

    EXTI_InitType exti_init_struct;
    exti_init_struct.EXTI_Line = gpio->isr_line;
    exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
    switch (mode) {
    case PIN_IRQ_MODE_RISING:
        exti_init_struct.EXTI_Trigger = EXTI_Trigger_Rising;
        exti_init_struct.EXTI_LineCmd = ENABLE;
        break;
    case PIN_IRQ_MODE_FALLING:
        exti_init_struct.EXTI_Trigger = EXTI_Trigger_Falling;
        exti_init_struct.EXTI_LineCmd = ENABLE;
        break;
    case PIN_IRQ_MODE_CHANGE:
        exti_init_struct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
        exti_init_struct.EXTI_LineCmd = ENABLE;
        break;
    case PIN_IRQ_MODE_DISABLE:
        exti_init_struct.EXTI_LineCmd = DISABLE;
        break;
    default:
        exti_init_struct.EXTI_LineCmd = DISABLE;
        break;
    }
    EXTI_Peripheral_Initializes(&exti_init_struct);

    NVIC_InitType NVIC_InitStructure;
    if (exti_init_struct.EXTI_LineCmd == ENABLE) {
        if (gpio->pin == GPIO_PIN_0 || gpio->pin == GPIO_PIN_1) {
            NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
        } else if (gpio->pin == GPIO_PIN_2 || gpio->pin == GPIO_PIN_3) {
            NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
        } else {
            NVIC_InitStructure.NVIC_IRQChannel = EXTI4_17_IRQn;
        }
        NVIC_InitStructure.NVIC_IRQChannelPriority = 4;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    } else {
        NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    }
    NVIC_Initializes(&NVIC_InitStructure);

    return KK_OK;
}

/**
 * @brief 取消引脚中断处理函数
 *
 * @param pin       编号
 * @return kk_err_t 成功返回OK，其他见错误码
 */
static kk_err_t _n32g003_pin_disable_irq(const struct kk_dev* dev, int32_t pin)
{
    const struct gpio_desc* gpio = _get_gpio(pin);
    if (!gpio) {
        return KK_INVALID;
    }

    gpio_isrs[gpio->pin].isr = NULL;
    gpio_isrs[gpio->pin].arg = NULL;

    EXTI_InitType exti_init_struct;
    exti_init_struct.EXTI_Line = gpio->isr_line;
    exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_init_struct.EXTI_LineCmd = DISABLE;
    EXTI_Peripheral_Initializes(&exti_init_struct);

    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    return KK_OK;
}

/**
 * @brief 获取引脚电平状态
 *
 * @param pin   编号
 * @return 低电平PIN_LOW，高电平PIN_HIGH
 */
static int32_t _n32g003_pin_read(const struct kk_dev* dev, int32_t pin)
{
    const struct gpio_desc* gpio = _get_gpio(pin);
    if (!gpio) {
        return 0;
    }

    if (GPIO_Input_Pin_Data_Get(gpio->periph, gpio->pin) == PIN_RESET) {
        return PIN_LOW;
    }
    return PIN_HIGH;
}

/**
 * @brief 引脚电平控制
 *
 * @param pin   编号
 * @param val   电平
 * @return 成功返回OK，其他见错误码
 */
static kk_err_t _n32g003_pin_write(const struct kk_dev* dev, int32_t pin, int32_t val)
{
    const struct gpio_desc* gpio = _get_gpio(pin);
    if (!gpio) {
        return 0;
    }

    if (val == PIN_HIGH) {
        GPIO_Pins_Set(gpio->periph, gpio->pin);
    } else {
        GPIO_Pins_Reset(gpio->periph, gpio->pin);
    }

    return KK_OK;
}

static void EXTI0_1_IRQHandler(void)
{
    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE0)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE0);
        if (gpio_isrs[0].isr != NULL) {
            gpio_isrs[0].isr(gpio_isrs[0].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE1)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE1);
        if (gpio_isrs[1].isr != NULL) {
            gpio_isrs[1].isr(gpio_isrs[1].arg);
        }
    }
}

static void EXTI2_3_IRQHandler(void)
{
    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE2)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE2);
        if (gpio_isrs[2].isr != NULL) {
            gpio_isrs[2].isr(gpio_isrs[2].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE3)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE3);
        if (gpio_isrs[3].isr != NULL) {
            gpio_isrs[3].isr(gpio_isrs[3].arg);
        }
    }
}

static void EXTI4_17_IRQHandler(void)
{
    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE4)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE4);
        if (gpio_isrs[4].isr != NULL) {
            gpio_isrs[4].isr(gpio_isrs[4].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE5)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE5);
        if (gpio_isrs[5].isr != NULL) {
            gpio_isrs[5].isr(gpio_isrs[5].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE6)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE6);
        if (gpio_isrs[6].isr != NULL) {
            gpio_isrs[6].isr(gpio_isrs[6].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE7)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE7);
        if (gpio_isrs[7].isr != NULL) {
            gpio_isrs[7].isr(gpio_isrs[7].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE8)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE8);
        if (gpio_isrs[8].isr != NULL) {
            gpio_isrs[8].isr(gpio_isrs[8].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE9)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE9);
        if (gpio_isrs[9].isr != NULL) {
            gpio_isrs[9].isr(gpio_isrs[9].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE10)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE10);
        if (gpio_isrs[10].isr != NULL) {
            gpio_isrs[10].isr(gpio_isrs[10].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE11)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE11);
        if (gpio_isrs[11].isr != NULL) {
            gpio_isrs[11].isr(gpio_isrs[11].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE12)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE12);
        if (gpio_isrs[12].isr != NULL) {
            gpio_isrs[12].isr(gpio_isrs[12].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE13)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE13);
        if (gpio_isrs[13].isr != NULL) {
            gpio_isrs[13].isr(gpio_isrs[13].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE14)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE14);
        if (gpio_isrs[14].isr != NULL) {
            gpio_isrs[14].isr(gpio_isrs[14].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE15)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE15);
        if (gpio_isrs[15].isr != NULL) {
            gpio_isrs[15].isr(gpio_isrs[15].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE16)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE16);
        if (gpio_isrs[16].isr != NULL) {
            gpio_isrs[16].isr(gpio_isrs[16].arg);
        }
    }

    if (RESET != EXTI_Interrupt_Status_Get(EXTI_LINE17)) {
        EXTI_Interrupt_Status_Clear(EXTI_LINE17);
        if (gpio_isrs[17].isr != NULL) {
            gpio_isrs[17].isr(gpio_isrs[17].arg);
        }
    }
}

static struct kk_pin_dev _n32g003_pin_dev = {
    .mode = _n32g003_pin_mode,
    .read = _n32g003_pin_read,
    .write = _n32g003_pin_write,
    .enable_irq = _n32g003_pin_enable_irq,
    .disable_irq = _n32g003_pin_disable_irq,
};

KK_INLINE void _n32g003_pin_init(void)
{
    device_register(&(_n32g003_pin_dev.dev), "PIN");
}
KK_INIT_PLATFORM_EXPORT(_n32g003_pin_init);

#ifdef __cplusplus
}
#endif

#endif
