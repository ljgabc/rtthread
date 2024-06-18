/**
 * @file kk_serial.h
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 串口控制通用接口
 * @version 0.1
 * @date 2023-04-25
 * @note 使用说明
 * 
 *  // 串口收发数据有3种模式：轮询、中断、DMA，可以在打开串口设备时分别指定接收和发送使用的模式
 *  // 另外，串口收发数据可以选择阻塞模式和非阻塞模式：
 *      - 阻塞模式下读，直到读取到至少一个字节后才返回；写的时候直到所有数据都被写入成功才会返回
 *      - 非阻塞模式下读写，会立即返回读到或写入的字节数
 *  
 *  // 使用轮询模式打开串口，读写数据
 *  // struct rt_device* dev = device_find("UART1");
 *  // serial_open(dev, SERIAL_MODE_RX_POLL | SERIAL_MODE_TX_POLL | SERIAL_MODE_RX_BLOCKING | SERIAL_MODE_TX_BLOCKING, 115200, 8, SERIAL_PARITY_NONE, 1)
 *  // int32_t ch = serial_getchar(dev)
 *  // serial_putchar(dev, ch) 
 * 
 *  // 使用中断模式打开串口，读写数据，阻塞模式
 *  // struct rt_device* dev = device_find("UART2");
 *  // serial_open(dev, SERIAL_MODE_RX_INT | SERIAL_MODE_TX_INT | SERIAL_MODE_RX_BLOCKING | SERIAL_MODE_TX_BLOCKING, 115200, 8, SERIAL_PARITY_NONE, 1)
 *  // uint8_t buf[128];
 *  // int32_t cnt = serial_read(dev, buf, sizeof(buf))
 *  // serial_write(dev, buf, cnt)
 * 
 *  // 使用中断模式打开串口，读写数据，非阻塞模式
 *  // struct rt_device* dev = device_find("UART3");
 *  // serial_open(dev, SERIAL_MODE_RX_INT | SERIAL_MODE_TX_INT | SERIAL_MODE_RX_NON_BLOCKING | SERIAL_MODE_TX_NON_BLOCKING, 115200, 8, SERIAL_PARITY_NONE, 1)
 *  // uint8_t buf[128];
 *  // int32_t cnt = serial_read(dev, buf, sizeof(buf));
 *  // while(serial_write(dev, buf, cnt) != cnt);
 *  
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "components/modules.h"
#include "config.h"
#include "rtdevice.h"
#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 打开模式，默认POLL模式、NON BLOCKING
 *
 */
#define SERIAL_MODE_RX_POLL (0 << 0) // 轮询输入模式
#define SERIAL_MODE_TX_POLL (0 << 0) // 轮询发送模式

#define SERIAL_MODE_RX_INT (1 << 0) // 中断输入模式
#define SERIAL_MODE_TX_INT (1 << 1) // 中断发送模式

#define SERIAL_MODE_RX_DMA (1 << 2) // DMA输入模式
#define SERIAL_MODE_TX_DMA (1 << 3) // DMA发送模式

#define SERIAL_MODE_RX_BLOCKING (1 << 4) // 阻塞输入模式
#define SERIAL_MODE_TX_BLOCKING (1 << 5) // 阻塞发送模式

#define BAUD_RATE_2400 2400
#define BAUD_RATE_4800 4800
#define BAUD_RATE_9600 9600
#define BAUD_RATE_19200 19200
#define BAUD_RATE_38400 38400
#define BAUD_RATE_57600 57600
#define BAUD_RATE_115200 115200
#define BAUD_RATE_230400 230400
#define BAUD_RATE_460800 460800
#define BAUD_RATE_921600 921600
#define BAUD_RATE_2000000 2000000
#define BAUD_RATE_3000000 3000000

#define DATA_BITS_5 5
#define DATA_BITS_6 6
#define DATA_BITS_7 7
#define DATA_BITS_8 8
#define DATA_BITS_9 9

#define STOP_BITS_1 0
#define STOP_BITS_2 1
#define STOP_BITS_3 2
#define STOP_BITS_4 3

#define PARITY_NONE 0
#define PARITY_ODD 1
#define PARITY_EVEN 2

#define BIT_ORDER_LSB 0
#define BIT_ORDER_MSB 1

#define NRZ_NORMAL 0 /* Non Return to Zero : normal mode */
#define NRZ_INVERTED 1 /* Non Return to Zero : inverted mode */

#define RT_SERIAL_EVENT_RX_IND 0x01 /* Rx indication */
#define RT_SERIAL_EVENT_TX_DONE 0x02 /* Tx complete   */
#define RT_SERIAL_EVENT_RX_DMADONE 0x03 /* Rx DMA transfer done */
#define RT_SERIAL_EVENT_TX_DMADONE 0x04 /* Tx DMA transfer done */
#define RT_SERIAL_EVENT_RX_TIMEOUT 0x05 /* Rx timeout    */

#define RT_SERIAL_DMA_RX 0x01
#define RT_SERIAL_DMA_TX 0x02

#define RT_SERIAL_RX_INT 0x01
#define RT_SERIAL_TX_INT 0x02

#define RT_SERIAL_ERR_OVERRUN 0x01
#define RT_SERIAL_ERR_FRAMING 0x02
#define RT_SERIAL_ERR_PARITY 0x03

#define RT_SERIAL_TX_DATAQUEUE_SIZE 2048
#define RT_SERIAL_TX_DATAQUEUE_LWM 30

#define RT_SERIAL_FLOWCONTROL_CTSRTS 1
#define RT_SERIAL_FLOWCONTROL_NONE 0

typedef rt_ssize_t (*cb_serial_tx)(struct rt_device* dev, const rt_uint8_t* data, rt_ssize_t length);
typedef rt_ssize_t (*cb_serial_rx)(struct rt_device* dev, rt_uint8_t* data, rt_ssize_t length);

struct serial_configure {
    rt_uint32_t baud_rate;
    rt_uint32_t data_bits : 4;
    rt_uint32_t stop_bits : 2;
    rt_uint32_t parity : 2;
    rt_uint32_t bit_order : 1;
    rt_uint32_t invert : 1;
    rt_uint32_t bufsz : 16; // unused, move it to struct rt_serial_device
    rt_uint32_t flowcontrol : 1;
    rt_uint32_t reserved : 5;
};

/**
 * serial operators
 */
struct rt_serial_ops {
    rt_err_t (*init)(struct rt_device* dev);

    rt_err_t (*configure)(struct rt_device* dev, struct serial_configure* cfg);
    rt_err_t (*control)(struct rt_device* dev, int cmd, void* arg);

    int (*putc)(struct rt_device* dev, char c);
    int (*getc)(struct rt_device* dev);
    int (*flush)(struct rt_device* dev);

    void (*start_tx)(struct rt_device* dev);
    void (*stop_tx)(struct rt_device* dev);

#ifdef CONFIG_SERIAL_USE_DMA_TX
    rt_bool_t (*is_dma_txing)(struct rt_device* dev);
    void (*start_dma_tx)(struct rt_device* dev, rt_uint8_t* buf, rt_ssize_t size);
    void (*stop_dma_tx)(struct rt_device* dev);
#endif

    void (*enable_interrupt)(struct rt_device* dev);
    void (*disable_interrupt)(struct rt_device* dev);
};

/**
 * @brief 串口设备模型
 * 
 */
struct rt_device_serial {
    struct rt_device dev;
    // rt_err_t (*open)(const struct rt_device* dev, rt_base_t mode,
    //     rt_base_t baud, rt_base_t databit, rt_base_t parity, rt_base_t stopbit);
    // rt_base_t (*getchar)(const struct rt_device* dev);
    // rt_base_t (*putchar)(const struct rt_device* dev, int32_t ch);
    // rt_base_t (*start_int_tx)(const struct rt_device* dev);
    // rt_base_t (*start_dma_tx)(const struct rt_device* dev);
    const struct rt_serial_ops* ops;

    void* serial_rx;
    void* serial_tx;

#ifdef CONFIG_SERIAL_USE_DMA_RX
    rt_ssize_t dma_idx_rx;
    rt_uint8_t serial_dma_rx[CONFIG_SERIAL_DMA_RX_BUFSZ];
#endif

#ifdef CONFIG_SERIAL_USE_DMA_TX
    rt_uint8_t serial_dma_tx[CONFIG_SERIAL_DMA_TX_BUFSZ];
#endif

    cb_serial_rx _cb_rx;
    cb_serial_tx _cb_tx;

#ifdef CONFIG_USE_RTOS
    rt_tick_t timeout_tick;
    struct rt_completion completion_rx;
    struct rt_completion completion_tx;
#endif
};

void rt_hw_serial_isr(struct rt_device* dev, int event);

/**
 * @brief 注册Serial设备
 * 
 * @param pin    Serial设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_serial_register(struct rt_device_serial* serial, const char* name,
    const struct rt_serial_ops* ops, rt_uint32_t flag, void* user_data);

#ifdef __cplusplus
}
#endif
