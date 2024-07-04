/**
 * @file drv_n32g003_serial.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief N32G003 API适配ADC驱动
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "config.h"

#if defined(CONFIG_BSP_N32G003) && defined(CONFIG_USING_SERIAL)

#include "bsp/n32g003/library/n32g003_gpio.h"
#include "bsp/n32g003/library/n32g003_rcc.h"
#include "bsp/n32g003/library/n32g003_uart.h"
#include "components/device/rt_serial.h"
#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

static struct rt_device_serial _n32g003_serial1;
static struct rt_device_serial _n32g003_serial2;

static rt_err_t n32g003_uart_init(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);

    UART_InitType uart_init_struct;
    GPIO_InitType io_init_struct;
    UART_Structure_Initializes(&uart_init_struct);
    GPIO_Structure_Initialize(&io_init_struct);

    if (PORT == UART1) {
#ifdef CONFIG_BSP_UART1
        io_init_struct.GPIO_Mode = GPIO_MODE_AF_PP;
#if defined(CONFIG_N32G003_UART1_TX_PA2)
        io_init_struct.Pin = GPIO_PIN_2;
        io_init_struct.GPIO_Alternate = GPIO_AF5_UART1;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#elif defined(CONFIG_N32G003_UART1_TX_PA14)
        io_init_struct.Pin = GPIO_PIN_14;
        io_init_struct.GPIO_Alternate = GPIO_AF2_UART1;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#elif defined(CONFIG_N32G003_UART1_TX_PB0)
        io_init_struct.Pin = GPIO_PIN_0;
        io_init_struct.GPIO_Alternate = GPIO_AF2_UART1;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB);
        GPIO_Peripheral_Initialize(GPIOB, &io_init_struct);
#else
#error "Unsupported Uart1 TX pins"
#endif

        io_init_struct.GPIO_Mode = GPIO_MODE_INPUT;
#if defined(CONFIG_N32G003_UART1_RX_PA3)
        io_init_struct.Pin = GPIO_PIN_13;
        io_init_struct.GPIO_Alternate = GPIO_AF5_UART1;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#elif defined(CONFIG_N32G003_UART1_RX_PA12)
        io_init_struct.Pin = GPIO_PIN_12;
        io_init_struct.GPIO_Alternate = GPIO_AF2_UART1;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#elif defined(CONFIG_N32G003_UART1_RX_PB1)
        io_init_struct.Pin = GPIO_PIN_1;
        io_init_struct.GPIO_Alternate = GPIO_AF2_UART1;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPB);
        GPIO_Peripheral_Initialize(GPIOB, &io_init_struct);
#else
#error "Unsupported Uart1 RX pins"
#endif
#endif
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_UART1);
        UART_Initializes(UART1, &uart_init_struct);
        UART_Enable(UART1);
        UART_Interrupt_Status_Clear(UART1, UART_INT_TXDE | UART_INT_TXC);
    } else if (PORT == UART2) {
#ifdef CONFIG_BSP_UART2
        io_init_struct.GPIO_Mode = GPIO_MODE_AF_PP;
#if defined(CONFIG_N32G003_UART2_TX_PA2)
        io_init_struct.Pin = GPIO_PIN_2;
        io_init_struct.GPIO_Alternate = GPIO_AF1_UART2;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#elif defined(CONFIG_N32G003_UART2_TX_PA8)
        io_init_struct.Pin = GPIO_PIN_8;
        io_init_struct.GPIO_Alternate = GPIO_AF1_UART2;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#elif defined(CONFIG_N32G003_UART2_TX_PA9)
        io_init_struct.Pin = GPIO_PIN_9;
        io_init_struct.GPIO_Alternate = GPIO_AF1_UART2;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#else
#error "Unsupported Uart2 TX pins"
#endif

        io_init_struct.GPIO_Mode = GPIO_MODE_INPUT;
#if defined(CONFIG_N32G003_UART2_RX_PA1)
        io_init_struct.Pin = GPIO_PIN_1;
        io_init_struct.GPIO_Alternate = GPIO_AF1_UART2;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#elif defined(CONFIG_N32G003_UART2_RX_PA7)
        io_init_struct.Pin = GPIO_PIN_7;
        io_init_struct.GPIO_Alternate = GPIO_AF6_UART2;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#elif defined(CONFIG_N32G003_UART2_RX_PA9)
        io_init_struct.Pin = GPIO_PIN_9;
        io_init_struct.GPIO_Alternate = GPIO_AF8_UART2;
        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA);
        GPIO_Peripheral_Initialize(GPIOA, &io_init_struct);
#else
#error "Unsupported Uart2 RX pins"
#endif
#endif

        RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_UART2);
        UART_Initializes(UART2, &uart_init_struct);
        UART_Enable(UART2);
        UART_Interrupt_Status_Clear(UART2, UART_INT_TXDE | UART_INT_TXC);
    }

    return RT_EOK;
}

static rt_err_t n32g003_uart_configure(struct rt_device* dev, struct serial_configure* cfg)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);

    UART_InitType uart_init_struct;
    UART_Structure_Initializes(&uart_init_struct);

    uart_init_struct.BaudRate = cfg->baud_rate;

    switch (cfg->data_bits) {
    case DATA_BITS_8:
        uart_init_struct.WordLength = UART_WL_8B;
        break;
    case DATA_BITS_9:
        uart_init_struct.WordLength = UART_WL_9B;
        break;
    default:
        uart_init_struct.WordLength = UART_WL_8B;
        break;
    }

    switch (cfg->stop_bits) {
    case STOP_BITS_1:
        uart_init_struct.StopBits = UART_STPB_1;
        break;
    case STOP_BITS_2:
        uart_init_struct.StopBits = UART_STPB_2;
        break;
    default:
        uart_init_struct.StopBits = UART_STPB_1;
        break;
    }

    switch (cfg->parity) {
    case PARITY_NONE:
        uart_init_struct.Parity = UART_PE_NO;
        break;
    case PARITY_ODD:
        uart_init_struct.Parity = UART_PE_ODD;
        break;
    case PARITY_EVEN:
        uart_init_struct.Parity = UART_PE_EVEN;
        break;
    default:
        uart_init_struct.Parity = UART_PE_NO;
        break;
    }

    uart_init_struct.Mode = UART_MODE_RX | UART_MODE_TX;

    UART_Initializes(PORT, &uart_init_struct);

    return RT_EOK;
}

static rt_err_t n32g003_uart_control(struct rt_device* dev, int cmd, void* arg)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);
    rt_uint32_t ctrl_arg = (rt_uint32_t)(arg);
    NVIC_InitType NVIC_InitStructure;

    switch (cmd) {
    case RT_DEVICE_CTRL_OPEN:
        UART_Interrput_Disable(PORT, UART_INT_TXDE);
        UART_Interrupt_Status_Clear(PORT, UART_INT_TXDE | UART_INT_TXC | UART_INT_RXDNE);
        if (PORT == UART1) {
            NVIC_EnableIRQ(UART1_IRQn);
        } else if (PORT == UART2) {
            NVIC_EnableIRQ(UART2_IRQn);
        }
        break;
    case RT_DEVICE_CTRL_CLOSE:
        if (PORT == UART1) {
            NVIC_DisableIRQ(UART1_IRQn);
        } else if (PORT == UART2) {
            NVIC_DisableIRQ(UART2_IRQn);
        }
        UART_Interrput_Disable(PORT, UART_INT_TXDE);
        UART_Interrput_Disable(PORT, UART_INT_TXC);
        UART_Interrput_Disable(PORT, UART_INT_RXDNE);
        UART_Interrupt_Status_Clear(PORT, UART_INT_TXDE | UART_INT_TXC | UART_INT_RXDNE);
        break;
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable interrupt */
        if (ctrl_arg & RT_DEVICE_FLAG_INT_RX) {
            UART_Interrput_Disable(PORT, UART_INT_RXDNE);
        }
        break;
    case RT_DEVICE_CTRL_SET_INT:
        /* enable interrupt */
        if (ctrl_arg & RT_DEVICE_FLAG_INT_RX) {
            UART_Interrput_Enable(PORT, UART_INT_RXDNE);
        }
        break;
    case RT_DEVICE_CTRL_CONFIG:
        break;
    default:
        break;
    }
    return RT_EOK;
}

static int n32g003_uart_putc(struct rt_device* dev, char c)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);

    while (UART_Flag_Status_Get(PORT, UART_FLAG_TXDE) == RESET)
        ;
    UART_Data_Send(PORT, (uint8_t)c);

    return 1;
}

static int n32g003_uart_getc(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);

    int ch = -1;

    if (UART_Flag_Status_Get(PORT, UART_FLAG_RXDNE) != RESET) {
        ch = UART_Data_Receive(PORT);
    }

    return ch;
}

static int n32g003_uart_flush(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);
    UART_Interrput_Enable(PORT, UART_INT_TXDE);
    while (
        !(
            (UART_Interrupt_Status_Get(PORT, UART_INT_TXDE) != RESET) && (UART_Interrupt_Status_Get(PORT, UART_INT_TXC) != RESET)))
        ;
    UART_Interrput_Disable(PORT, UART_INT_TXDE);
    return 0;
}

static void n32g003_start_tx(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);
    UART_Interrput_Enable(PORT, UART_INT_TXDE);
}

static void n32g003_stop_tx(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);
    UART_Interrput_Disable(PORT, UART_INT_TXDE);
}

static void n32g003_enable_interrupt(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);

    if (PORT == UART1) {
        NVIC_EnableIRQ(UART1_IRQn);
    } else if (PORT == UART2) {
        NVIC_EnableIRQ(UART2_IRQn);
    }
}

static void n32g003_disable_interrupt(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);

    if (PORT == UART1) {
        NVIC_DisableIRQ(UART1_IRQn);
    } else if (PORT == UART2) {
        NVIC_DisableIRQ(UART2_IRQn);
    }
}

static void n32g003_uart_isr(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    UART_Module* PORT = (UART_Module*)(dev->user_data);

    if (UART_Interrupt_Status_Get(PORT, UART_INT_RXDNE) != RESET) {
        if (UART_Flag_Status_Get(PORT, UART_FLAG_PEF) == RESET) {
            rt_hw_serial_isr(dev, RT_SERIAL_EVENT_RX_IND);
        }
        UART_Interrupt_Status_Clear(PORT, UART_INT_RXDNE);
    }

    if (UART_Interrupt_Status_Get(PORT, UART_INT_TXDE) != RESET) {
        if (UART_Flag_Status_Get(PORT, UART_FLAG_PEF) == RESET) {
            rt_hw_serial_isr(dev, RT_SERIAL_EVENT_TX_DONE);
        }
        UART_Interrupt_Status_Clear(PORT, UART_INT_TXDE);
    }

    if (UART_Flag_Status_Get(PORT, UART_FLAG_OREF) == SET) {
        n32g003_uart_getc(dev);
    }
}

void UART1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    n32g003_uart_isr(&(_n32g003_serial1.dev));

    /* leave interrupt */
    rt_interrupt_leave();
}

void UART2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    n32g003_uart_isr(&(_n32g003_serial2.dev));

    /* leave interrupt */
    rt_interrupt_leave();
}

static const struct rt_serial_ops _n32g003_serial_ops = {
    .init = n32g003_uart_init,
    .configure = n32g003_uart_configure,
    .control = n32g003_uart_control,
    .putc = n32g003_uart_putc,
    .getc = n32g003_uart_getc,
    .flush = n32g003_uart_flush,
    .start_tx = n32g003_start_tx,
    .stop_tx = n32g003_stop_tx,
    .enable_interrupt = n32g003_enable_interrupt,
    .disable_interrupt = n32g003_disable_interrupt,
};

int _n32g003_serial_init(void)
{
    rt_err_t ret = RT_EOK;
    ret = rt_device_serial_register(&_n32g003_serial1, "uart1", &_n32g003_serial_ops,
        RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX, (void*)UART1);
    if (ret != RT_EOK) {
        return ret;
    }
    return rt_device_serial_register(&_n32g003_serial2, "uart2", &_n32g003_serial_ops,
        RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX, (void*)UART2);
}
INIT_DEVICE_EXPORT(_n32g003_serial_init);

#ifdef __cplusplus
}
#endif

#endif
