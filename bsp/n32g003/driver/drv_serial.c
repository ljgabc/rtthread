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

#if defined(CONFIG_BSP_N32G003) && defined(CONFIG_USE_SERIAL)

#include "rtthread.h"
#include "components/device/rt_serial.h"
#include "bsp/n32g003/library/n32g003_gpio.h"
#include "bsp/n32g003/library/n32g003_rcc.h"
#include "bsp/n32g003/library/n32g003_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

struct n32g003_serial_dev {
  struct kk_serial_dev serial_dev;
  int32_t port;
};


static kk_err_t _n32g003_serial_enable(const struct kk_dev* dev, int32_t mode,
                                       int32_t baud, int32_t databit,
                                       int32_t parity, int32_t stopbit) {
  struct kk_serial_dev* serial_dev =
      kk_container_of(dev, struct kk_serial_dev, dev);
  struct n32g003_serial_dev* serial =
      kk_container_of(serial_dev, struct n32g003_serial_dev, serial_dev);

  if (mode & SERIAL_MODE_RX_INT || mode & SERIAL_MODE_TX_INT) {
    NVIC_InitType NVIC_InitStructure;
    if (serial->port == 1) {
      NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    } else {
      NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
    }
    NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_PRIORITY_0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
  }

  UART_InitType uart_init_struct;
  UART_Structure_Initializes(&uart_init_struct);

  uart_init_struct.BaudRate = baud;
  if (databit == SERIAL_DATABIT_8B) {
    uart_init_struct.WordLength = UART_WL_8B;
  } else {
    uart_init_struct.WordLength = UART_WL_9B;
  }

  if (stopbit == SERIAL_STOPBIT_1B) {
    uart_init_struct.StopBits = UART_STPB_1;
  } else {
    uart_init_struct.StopBits = UART_STPB_2;
  }

  if (parity == SERIAL_PARITY_NONE) {
    uart_init_struct.Parity = UART_PE_NO;
  } else if (parity == SERIAL_PARITY_EVEN) {
    uart_init_struct.Parity = UART_PE_EVEN;
  } else {
    uart_init_struct.Parity = UART_PE_ODD;
  }
  uart_init_struct.Mode = UART_MODE_RX | UART_MODE_TX;

  if (serial->port == 1) {
#ifdef CONFIG_N32G003_UART1
    GPIO_InitType io_init_struct;
    GPIO_Structure_Initialize(&io_init_struct);
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
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_UART1);
    UART_Initializes(UART1, &uart_init_struct);

    // if (mode & SERIAL_MODE_TX_INT) {
    //   UART_Interrput_Enable(UART1, UART_INT_TXC);
    // }

    if (mode & SERIAL_MODE_RX_INT) {
      UART_Interrput_Enable(UART1, UART_INT_RXDNE);
    }

    UART_Enable(UART1);
#endif
    return KK_OK;
  } else if (serial->port == 2) {
#ifdef CONFIG_N32G003_UART2
    GPIO_InitType io_init_struct;
    GPIO_Structure_Initialize(&io_init_struct);
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
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_UART2);
    UART_Initializes(UART2, &uart_init_struct);

    // if (mode & SERIAL_MODE_TX_INT) {
    //   UART_Interrput_Enable(UART2, UART_INT_TXC);
    // }

    if (mode & SERIAL_MODE_RX_INT) {
      UART_Interrput_Enable(UART2, UART_INT_RXDNE);
    }

    UART_Enable(UART2);
#endif
    return KK_OK;
  } else {
    return KK_INVALID;
  }
}

static int32_t _n32g003_serial_getchar(const struct kk_dev* dev) {
  struct kk_serial_dev* serial_dev =
      kk_container_of(dev, struct kk_serial_dev, dev);
  struct n32g003_serial_dev* serial =
      kk_container_of(serial_dev, struct n32g003_serial_dev, serial_dev);
  if (serial->port == 1) {
    while (UART_Flag_Status_Get(UART1, UART_FLAG_RXDNE) == RESET) {
    }
    /* Store the received byte in RxBuffer */
    return UART_Data_Receive(UART1);
  } else {
    while (UART_Flag_Status_Get(UART2, UART_FLAG_RXDNE) == RESET) {
    }
    /* Store the received byte in RxBuffer */
    return UART_Data_Receive(UART2);
  }
}

static int32_t _n32g003_serial_putchar(const struct kk_dev* dev, int32_t ch) {
  struct kk_serial_dev* serial_dev =
      kk_container_of(dev, struct kk_serial_dev, dev);
  struct n32g003_serial_dev* serial =
      kk_container_of(serial_dev, struct n32g003_serial_dev, serial_dev);

  if (serial->port == 1) {
    while (UART_Flag_Status_Get(UART1, UART_FLAG_TXDE) == RESET);
    UART_Data_Send(UART1, (uint8_t)ch);
  } else {
    while (UART_Flag_Status_Get(UART2, UART_FLAG_TXDE) == RESET);
    UART_Data_Send(UART2, (uint8_t)ch);
  }
  return 1;
}

static int32_t _n32g003_serial_start_int_tx(const struct kk_dev* dev) {
  struct kk_serial_dev* serial_dev =
      kk_container_of(dev, struct kk_serial_dev, dev);
  struct n32g003_serial_dev* serial =
      kk_container_of(serial_dev, struct n32g003_serial_dev, serial_dev);
  if (serial_dev->mode & SERIAL_MODE_TX_INT) {
    if (serial->port == 1) {
      UART_Interrput_Enable(UART1, UART_INT_TXDE);
    } else {
      UART_Interrput_Enable(UART2, UART_INT_TXDE);
    }
  }
}

static struct n32g003_serial_dev _n32g003_serial_dev[] = {
    {
        .serial_dev.open = _n32g003_serial_enable,
        .serial_dev.getchar = _n32g003_serial_getchar,
        .serial_dev.putchar = _n32g003_serial_putchar,
        .serial_dev.start_int_tx = _n32g003_serial_start_int_tx,
        .port = 1,
    },
    {
        .serial_dev.open = _n32g003_serial_enable,
        .serial_dev.getchar = _n32g003_serial_getchar,
        .serial_dev.putchar = _n32g003_serial_putchar,
        .serial_dev.start_int_tx = _n32g003_serial_start_int_tx,
        .port = 2,
    }};

void UART1_IRQHandler(void) {
  int32_t ch;
  if (UART_Interrupt_Status_Get(UART1, UART_INT_RXDNE) != RESET) {
    UART_Interrupt_Status_Clear(UART1, UART_INT_RXDNE);
    ch = UART_Data_Receive(UART1);
    kk_serial_callback(&(_n32g003_serial_dev[0].serial_dev.dev),
                       SERIAL_EVENT_RX_INT_DONE, &ch);
  }

  if (UART_Interrupt_Status_Get(UART1, UART_INT_TXDE) != RESET) {
    UART_Interrupt_Status_Clear(UART1, UART_INT_TXDE);
    kk_serial_callback(&(_n32g003_serial_dev[0].serial_dev.dev),
                       SERIAL_EVENT_TX_INT_DONE, &ch);
    if (ch != -1) {
      UART_Data_Send(UART1, (uint8_t)ch);
    } else {
      UART_Interrput_Disable(UART1, UART_INT_TXDE);
    }
  }
}

void UART2_IRQHandler(void) {
  int32_t ch;
  if (UART_Interrupt_Status_Get(UART2, UART_INT_RXDNE) != RESET) {
    UART_Interrupt_Status_Clear(UART2, UART_INT_RXDNE);
    ch = UART_Data_Receive(UART2);
    kk_serial_callback(&(_n32g003_serial_dev[1].serial_dev.dev),
                       SERIAL_EVENT_RX_INT_DONE, &ch);
  }

  if (UART_Interrupt_Status_Get(UART2, UART_INT_TXDE) != RESET) {
    UART_Interrupt_Status_Clear(UART2, UART_INT_TXDE);
    kk_serial_callback(&(_n32g003_serial_dev[1].serial_dev.dev),
                       SERIAL_EVENT_TX_INT_DONE, &ch);
    if (ch != -1) {
      UART_Data_Send(UART1, (uint8_t)ch);
    } else {
      UART_Interrput_Disable(UART2, UART_INT_TXDE);
    }
  }
}

KK_INLINE void _n32g003_serial_init(void) {
  device_register(&(_n32g003_serial_dev[0].serial_dev.dev), "UART1");
  device_register(&(_n32g003_serial_dev[1].serial_dev.dev), "UART2");
}
KK_INIT_PLATFORM_EXPORT(_n32g003_serial_init);

#ifdef __cplusplus
}
#endif

#endif
