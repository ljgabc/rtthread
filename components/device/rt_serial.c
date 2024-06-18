/**
 * @file kk_serial.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 串口控制通用接口
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "components/device/rt_serial.h"
#include "config.h"
#include "rtdef.h"
#include "rtdevice.h"
#include "rthw.h"
#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RT_USING_HEAP
rt_uint8_t _rx_buffer[CONFIG_SERIAL_RX_FIFOSZ];
rt_uint8_t _tx_buffer[CONFIG_SERIAL_TX_FIFOSZ];
struct rt_ringbuffer _rx_fifo, _tx_fifo;
#endif

/*
 * Serial poll routines
 */
rt_inline rt_ssize_t _serial_poll_rx(struct rt_device* dev, rt_uint8_t* data, rt_ssize_t length)
{
    int ch;
    int size;

    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    size = length;

    while (length) {
        ch = serial->ops->getc(dev);
        if (ch == -1)
            break;

        *data = ch;
        data++;
        length--;
    }

    return size - length;
}

rt_inline rt_ssize_t _serial_poll_tx(struct rt_device* dev, const rt_uint8_t* data, rt_ssize_t length)
{
    int size;
    rt_uint8_t last_char = 0;

    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    size = length;
    while (length) {
        /*
         * to be polite with serial console add a line feed
         * to the carriage return character
         */
        if (*data == '\n' && (dev->open_flag & RT_DEVICE_FLAG_STREAM) == RT_DEVICE_FLAG_STREAM && last_char != '\r') {
            serial->ops->putc(dev, '\r');
            last_char = 0;
        } else if (*data == '\r') {
            last_char = '\r';
        } else {
            last_char = 0;
        }

        serial->ops->putc(dev, *data);

        data++;
        length--;
    }

    return size - length;
}

/*
 * Serial interrupt routines
 */
rt_inline rt_ssize_t _serial_fifo_rx(struct rt_device* dev, rt_uint8_t* data, rt_ssize_t length)
{
    rt_ssize_t len, size;
    struct rt_ringbuffer* rx_fifo;
    rt_base_t level;

    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    if (length == 0)
        return 0;

    rx_fifo = (struct rt_ringbuffer*)serial->serial_rx;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();
    //    serial->ops->disable_interrupt(serial);

    len = rt_ringbuffer_data_len(rx_fifo);

    if ((len == 0) && // non-blocking io mode
        (dev->open_flag & RT_DEVICE_OFLAG_NONBLOCKING) == RT_DEVICE_OFLAG_NONBLOCKING) {
        /* enable interrupt */
        rt_hw_interrupt_enable(level);
        return 0;
    }
    if ((len == 0) && // blocking io mode
        (dev->open_flag & RT_DEVICE_OFLAG_NONBLOCKING) != RT_DEVICE_OFLAG_NONBLOCKING) {
        rt_err_t ret;
        do {
            /* enable interrupt */
            rt_hw_interrupt_enable(level);

#ifdef CONFIG_USE_RTOS
            ret = rt_completion_wait(&(serial->completion_rx), serial->timeout_tick);
#endif
            if (ret == RT_EOK) {
            } else if (ret == -RT_ETIMEOUT) {
                return -RT_ETIMEOUT;
            } else {
                return 0;
            }

            /* disable interrupt */
            level = rt_hw_interrupt_disable();

            len = rt_ringbuffer_data_len(rx_fifo);
        } while (len == 0);
    }

    if (len > length) {
        len = length;
    }

    /* read from software FIFO */
    for (size = 0; size < len; size++) {
        /* otherwise there's the data: */
        rt_ringbuffer_getchar(rx_fifo, data);
        data++;
    }

    /* enable interrupt */
    rt_hw_interrupt_enable(level);
    //    serial->ops->enable_interrupt(serial);

    return size;
}

rt_inline rt_ssize_t _serial_int_tx(struct rt_device* dev, const rt_uint8_t* data, rt_ssize_t length)
{
    rt_ssize_t len, length_t, size;
    struct rt_ringbuffer* tx_fifo;
    rt_base_t level;
    rt_uint8_t ch = 0;
    static rt_uint8_t last_char = 0;

    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    if (length == 0)
        return 0;

    tx_fifo = (struct rt_ringbuffer*)serial->serial_tx;

    size = 0;
    do {
        length_t = length - size;
        /* disable interrupt */
        level = rt_hw_interrupt_disable();

        len = CONFIG_SERIAL_TX_FIFOSZ - rt_ringbuffer_data_len(tx_fifo);

        if ((len == 0) && // non-blocking io mode
            (dev->open_flag & RT_DEVICE_OFLAG_NONBLOCKING) == RT_DEVICE_OFLAG_NONBLOCKING) {
            /* enable interrupt */
            rt_hw_interrupt_enable(level);
            break;
        }

        if ((len == 0) && // blocking io mode
            (dev->open_flag & RT_DEVICE_OFLAG_NONBLOCKING) != RT_DEVICE_OFLAG_NONBLOCKING) {
            rt_err_t ret;
            /* enable interrupt */
            rt_hw_interrupt_enable(level);
#ifdef CONFIG_USE_RTOS
            ret = rt_completion_wait(&(serial->completion_tx), serial->timeout_tick);
#endif
            if (ret == RT_EOK) {
                continue;
            } else if (ret == -RT_ETIMEOUT) {
                return -RT_ETIMEOUT;
            } else {
                return size;
            }
        }

        if (len > length_t) {
            len = length_t;
        }
        /* copy to software FIFO */
        while (len > 0) {
            /*
             * to be polite with serial console add a line feed
             * to the carriage return character
             */
            if (*data == '\n' && (dev->open_flag & RT_DEVICE_FLAG_STREAM) == RT_DEVICE_FLAG_STREAM && last_char != '\r') {
                rt_ringbuffer_putchar(tx_fifo, '\r');

                last_char = '\r';
                if (len == 1) {
                    break;
                } else {
                    len--;
                }
            } else if (*data == '\r') {
                last_char = '\r';
            } else {
                last_char = 0;
            }

            rt_ringbuffer_putchar(tx_fifo, *data);

            data++;
            len--;
            size++;
        }

        serial->ops->start_tx(dev);

        /* enable interrupt */
        rt_hw_interrupt_enable(level);
    } while (size < length);

    return size;
}

#ifdef CONFIG_SERIAL_USE_DMA_TX
/*
 * Serial DMA routines
 */
rt_inline rt_ssize_t _serial_dma_tx(struct rt_device* dev, const rt_uint8_t* data, rt_ssize_t length)
{
    rt_ssize_t len, length_t, size, i;
    struct rt_ringbuffer* tx_fifo;
    rt_base_t level;
    rt_uint8_t ch;
    static rt_uint8_t last_char = 0;

    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    if (length == 0)
        return 0;

    tx_fifo = (struct rt_ringbuffer*)serial->serial_tx;

    size = 0;
    do {
        length_t = length - size;
        /* disable interrupt */
        level = rt_hw_interrupt_disable();
        //        serial->ops->disable_interrupt(serial);

        len = CONFIG_SERIAL_TX_FIFOSZ - rt_ringbuffer_data_len(tx_fifo);

        if ((len == 0) && // non-blocking io mode
            (dev->open_flag & RT_DEVICE_OFLAG_NONBLOCKING) == RT_DEVICE_OFLAG_NONBLOCKING) {
            /* enable interrupt */
            rt_hw_interrupt_enable(level);
            break;
        }

        if ((len == 0) && // blocking io mode
            (dev->open_flag & RT_DEVICE_OFLAG_NONBLOCKING) != RT_DEVICE_OFLAG_NONBLOCKING) {
            rt_err_t ret;
            /* enable interrupt */
            rt_hw_interrupt_enable(level);

#ifdef CONFIG_USE_RTOS
            ret = rt_completion_wait(&(serial->completion_tx), serial->timeout_tick);
#endif
            if (ret == RT_EOK) {
                continue;
            } else if (ret == -RT_ETIMEOUT) {
                return -RT_ETIMEOUT;
            } else {
                return size;
            }
        }

        if (len > length_t) {
            len = length_t;
        }
        /* copy to software FIFO */
        while (len > 0) {
            /*
             * to be polite with serial console add a line feed
             * to the carriage return character
             */
            if (*data == '\n' && (dev->open_flag & RT_DEVICE_FLAG_STREAM) == RT_DEVICE_FLAG_STREAM && last_char != '\r') {
                rt_ringbuffer_putchar(tx_fifo, '\r');

                last_char = '\r';
                if (len == 1) {
                    break;
                } else {
                    len--;
                }
            } else if (*data == '\r') {
                last_char = '\r';
            } else {
                last_char = 0;
            }

            rt_ringbuffer_putchar(tx_fifo, *data);

            data++;
            len--;
            size++;
        }

        /* enable interrupt */
        rt_hw_interrupt_enable(level);
        //        serial->ops->enable_interrupt(serial);

        // TODO: start tx
        /* disable interrupt */
        level = rt_hw_interrupt_disable();
        //            serial->ops->disable_interrupt(serial);
        if (serial->ops->is_dma_txing(serial) == RT_FALSE) {
            /* calucate fifo data size */
            len = rt_ringbuffer_data_len(tx_fifo);

            if (len > CONFIG_SERIAL_DMA_RX_BUFSZ) {
                len = CONFIG_SERIAL_DMA_RX_BUFSZ;
            }
            /* read from software FIFO */
            for (i = 0; i < len; i++) {
                /* pop one byte data */
                ch = _serial_fifo_pop_data(tx_fifo);

                serial->serial_dma_tx[i] = ch;
            }
            serial->ops->start_dma_tx(serial, serial->serial_dma_tx, len);
        }
        /* enable interrupt */
        rt_hw_interrupt_enable(level);
        //            serial->ops->enable_interrupt(serial);
    } while (size < length);

    return size;
}
#endif /* RT_SERIAL_USING_DMA */

/* RT-Thread Device Interface */
/*
 * This function initializes serial device.
 */
static rt_err_t rt_serial_init(struct rt_device* dev)
{
    rt_err_t result = RT_EOK;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    /* initialize rx/tx */
    serial->serial_rx = RT_NULL;
    serial->serial_tx = RT_NULL;

    /* initialize hardware */
    if (serial->ops->init)
        result = serial->ops->init(dev);

    return result;
}

static rt_err_t rt_serial_open(struct rt_device* dev, rt_uint16_t oflag)
{
    rt_uint16_t stream_flag = 0;
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    //LOG_D("open serial device: 0x%08x with open flag: 0x%04x", dev, oflag);

    /* check device flag with the open flag */
    if ((oflag & RT_DEVICE_FLAG_DMA_RX) && !(dev->flag & RT_DEVICE_FLAG_DMA_RX))
        return -RT_EIO;
    if ((oflag & RT_DEVICE_FLAG_DMA_TX) && !(dev->flag & RT_DEVICE_FLAG_DMA_TX))
        return -RT_EIO;
    if ((oflag & RT_DEVICE_FLAG_INT_RX) && !(dev->flag & RT_DEVICE_FLAG_INT_RX))
        return -RT_EIO;
    if ((oflag & RT_DEVICE_FLAG_INT_TX) && !(dev->flag & RT_DEVICE_FLAG_INT_TX))
        return -RT_EIO;

    /* keep steam flag */
    if ((oflag & RT_DEVICE_FLAG_STREAM) || (dev->open_flag & RT_DEVICE_FLAG_STREAM))
        stream_flag = RT_DEVICE_FLAG_STREAM;

    /* get open flags */
    dev->open_flag = oflag & 0xff;

    if (oflag & RT_DEVICE_FLAG_INT_RX) {
        /* initialize the Rx/Tx structure according to open flag */
        if (serial->serial_rx == RT_NULL) {
#ifndef RT_USING_HEAP
            rt_ringbuffer_init(&_rx_fifo, _rx_buffer, CONFIG_SERIAL_RX_FIFOSZ);
            serial->serial_rx = &_rx_fifo;
#else
            serial->serial_rx = rt_ringbuffer_create(CONFIG_SERIAL_RX_FIFOSZ);
#endif
        }
        dev->open_flag |= RT_DEVICE_FLAG_INT_RX;
        serial->_cb_rx = _serial_fifo_rx;
#ifdef CONFIG_USE_RTOS
        rt_completion_init(&(serial->completion_rx));
#endif
        /* configure low level device */
        serial->ops->control(dev, RT_DEVICE_CTRL_SET_INT, (void*)RT_DEVICE_FLAG_INT_RX);
    }
#ifdef CONFIG_SERIAL_USE_DMA_RX
    else if (oflag & RT_DEVICE_FLAG_DMA_RX) {
        /* initialize the Rx/Tx structure according to open flag */
        if (serial->serial_rx == RT_NULL) {
#ifndef RT_USING_HEAP
            rt_ringbuffer_init(&_rx_fifo, _rx_buffer, CONFIG_SERIAL_RX_FIFOSZ);
            serial->serial_rx = &_rx_fifo;
#else
            serial->serial_rx = rt_ringbuffer_create(CONFIG_SERIAL_RX_FIFOSZ);
#endif
        }
        dev->open_flag |= RT_DEVICE_FLAG_DMA_RX;

        serial->dma_idx_rx = 0;
        serial->_cb_rx = _serial_fifo_rx;

#ifdef CONFIG_USE_RTOS
        rt_completion_init(&(serial->completion_rx));
#endif
        /* configure fifo address and length to low level device */
        serial->ops->control(serial, RT_DEVICE_CTRL_CONFIG, (void*)RT_DEVICE_FLAG_DMA_RX);
    }
#endif /* RT_SERIAL_USING_DMA */
    else {
        serial->serial_rx = RT_NULL;
        serial->_cb_rx = _serial_poll_rx;
    }

    if (oflag & RT_DEVICE_FLAG_INT_TX) {
        if (serial->serial_tx == RT_NULL) {
#ifndef RT_USING_HEAP
            rt_ringbuffer_init(&_tx_fifo, _tx_buffer, CONFIG_SERIAL_TX_FIFOSZ);
            serial->serial_tx = &_tx_fifo;
#else
            serial->serial_tx = rt_ringbuffer_create(CONFIG_SERIAL_TX_FIFOSZ);
#endif
        }
        dev->open_flag |= RT_DEVICE_FLAG_INT_TX;
        /* configure low level device */
        // serial->ops->control(serial, RT_DEVICE_CTRL_SET_INT, (void *)RT_DEVICE_FLAG_INT_TX);

        serial->_cb_tx = _serial_int_tx;

#ifdef CONFIG_USE_RTOS
        rt_completion_init(&(serial->completion_tx));
#endif
    }
#ifdef CONFIG_SERIAL_USE_DMA_TX
    else if (oflag & RT_DEVICE_FLAG_DMA_TX) {
        if (serial->serial_tx == RT_NULL) {
#ifndef RT_USING_HEAP
            rt_ringbuffer_init(&_tx_fifo, _tx_buffer, CONFIG_SERIAL_TX_FIFOSZ);
            serial->serial_tx = &_tx_fifo;
#else
            serial->serial_tx = rt_ringbuffer_create(CONFIG_SERIAL_TX_FIFOSZ);
#endif
        }
        dev->open_flag |= RT_DEVICE_FLAG_DMA_TX;

        serial->_cb_tx = _serial_dma_tx;

#ifdef CONFIG_USE_RTOS
        rt_completion_init(&(serial->completion_tx));
#endif

        /* configure low level device */
        serial->ops->control(serial, RT_DEVICE_CTRL_CONFIG, (void*)RT_DEVICE_FLAG_DMA_TX);
    }
#endif /* RT_SERIAL_USING_DMA */
    else {
        serial->serial_tx = RT_NULL;
        serial->_cb_tx = _serial_poll_tx;
    }

    serial->ops->control(dev, RT_DEVICE_CTRL_OPEN, (void*)0);

    /* set stream flag */
    dev->open_flag |= stream_flag;

    return RT_EOK;
}

static rt_err_t rt_serial_close(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    serial->ops->control(dev, RT_DEVICE_CTRL_CLOSE, RT_NULL);

    if (dev->open_flag & RT_DEVICE_FLAG_INT_RX) {
        struct rt_ringbuffer* rx_fifo;

        /* configure low level device */
        serial->ops->control(dev, RT_DEVICE_CTRL_CLR_INT, (void*)RT_DEVICE_FLAG_INT_RX);

        dev->open_flag &= ~RT_DEVICE_FLAG_INT_RX;

        rx_fifo = (struct rt_ringbuffer*)serial->serial_rx;
        rt_free(rx_fifo);

        serial->serial_rx = RT_NULL;
#ifdef CONFIG_USE_RTOS
        rt_completion_init(&(serial->completion_rx));
#endif
    }
#ifdef CONFIG_SERIAL_USE_DMA_RX
    else if (dev->open_flag & RT_DEVICE_FLAG_DMA_RX) {
        struct rt_ringbuffer* rx_fifo;

        /* configure low level device */
        serial->ops->control(serial, RT_DEVICE_CTRL_CLR_INT, (void*)RT_DEVICE_FLAG_DMA_RX);

        dev->open_flag &= ~RT_DEVICE_FLAG_DMA_RX;

        rx_fifo = (struct rt_ringbuffer*)serial->serial_rx;
        rt_free(rx_fifo);

        serial->serial_rx = RT_NULL;
#ifdef CONFIG_USE_RTOS
        rt_completion_init(&(serial->completion_rx));
#endif
    }
#endif /* RT_SERIAL_USING_DMA */

    if (dev->open_flag & RT_DEVICE_FLAG_INT_TX) {
        struct rt_ringbuffer* tx_fifo;

        /* configure low level device */
        serial->ops->control(dev, RT_DEVICE_CTRL_CLR_INT, (void*)RT_DEVICE_FLAG_INT_TX);

        dev->open_flag &= ~RT_DEVICE_FLAG_INT_TX;

        tx_fifo = (struct rt_ringbuffer*)serial->serial_tx;
        rt_free(tx_fifo);

        serial->serial_tx = RT_NULL;
#ifdef CONFIG_USE_RTOS
        rt_completion_init(&(serial->completion_tx));
#endif
    }
#ifdef CONFIG_SERIAL_USE_DMA_TX
    else if (dev->open_flag & RT_DEVICE_FLAG_DMA_TX) {
        struct rt_ringbuffer* tx_fifo;

        /* configure low level device */
        serial->ops->control(serial, RT_DEVICE_CTRL_CLR_INT, (void*)RT_DEVICE_FLAG_DMA_TX);

        dev->open_flag &= ~RT_DEVICE_FLAG_DMA_TX;

        tx_fifo = (struct rt_ringbuffer*)serial->serial_tx;
        rt_free(tx_fifo);

        serial->serial_tx = RT_NULL;
#ifdef CONFIG_USE_RTOS
        rt_completion_init(&(serial->completion_tx));
#endif
    }
#endif /* RT_SERIAL_USING_DMA */

    dev->flag &= ~RT_DEVICE_FLAG_ACTIVATED;

    return RT_EOK;
}

static rt_ssize_t rt_serial_read(struct rt_device* dev,
    rt_off_t pos,
    void* buffer,
    rt_ssize_t size)
{
    struct rt_device_serial* serial;

    RT_ASSERT(dev != RT_NULL);
    if (size == 0)
        return 0;

    serial = (struct rt_device_serial*)dev;

    return serial->_cb_rx(dev, (rt_uint8_t*)buffer, size);
}

static rt_ssize_t rt_serial_write(struct rt_device* dev,
    rt_off_t pos,
    const void* buffer,
    rt_ssize_t size)
{
    struct rt_device_serial* serial;

    RT_ASSERT(dev != RT_NULL);
    if (size == 0)
        return 0;

    serial = (struct rt_device_serial*)dev;

    return serial->_cb_tx(dev, (const rt_uint8_t*)buffer, size);
}

static rt_err_t rt_serial_flush(struct rt_device* dev)
{
    struct rt_device_serial* serial;
    rt_ssize_t len;
    struct rt_ringbuffer *tx_fifo, *rx_fifo;
    rt_base_t level;

    RT_ASSERT(dev != RT_NULL);

    serial = (struct rt_device_serial*)dev;

    if ((dev->open_flag & RT_DEVICE_FLAG_INT_RX)
#ifdef CONFIG_SERIAL_USE_DMA_RX
        || (dev->open_flag & RT_DEVICE_FLAG_DMA_RX)
#endif /* RT_SERIAL_USING_DMA */
    ) {
        rx_fifo = (struct rt_ringbuffer*)serial->serial_rx;
        level = rt_hw_interrupt_disable();
        rt_ringbuffer_reset(rx_fifo);
        rt_hw_interrupt_enable(level);
    }

    if ((dev->open_flag & RT_DEVICE_FLAG_INT_TX)
#ifdef CONFIG_SERIAL_USE_DMA_TX
        || (dev->open_flag & RT_DEVICE_FLAG_DMA_TX)
#endif /* RT_SERIAL_USING_DMA */
    ) {
        tx_fifo = (struct rt_ringbuffer*)serial->serial_tx;

        while (1) {
            /* disable interrupt */
            level = rt_hw_interrupt_disable();
            //        serial->ops->disable_interrupt(serial);

            len = rt_ringbuffer_data_len(tx_fifo);

            if (len == 0) {
                /* enable interrupt */
                rt_hw_interrupt_enable(level);
                //            serial->ops->enable_interrupt(serial);
                break;
            } else {
                /* enable interrupt */
                rt_hw_interrupt_enable(level);
                //            serial->ops->enable_interrupt(serial);
#ifdef CONFIG_USE_RTOS
                rt_completion_wait(&(serial->completion_tx), RT_WAITING_FOREVER);
#endif
            }
        }

        if (dev->open_flag & RT_DEVICE_FLAG_INT_TX) {
            serial->ops->flush(dev);
        }
#ifdef CONFIG_SERIAL_USE_DMA_TX
        else if (dev->open_flag & RT_DEVICE_FLAG_DMA_TX) {
            while (serial->ops->is_dma_txing(serial) == RT_TRUE)
                ;
        }
#endif /* RT_SERIAL_USING_DMA */
    } else {
        serial->ops->flush(dev);
    }

    return RT_EOK;
}

static rt_err_t rt_serial_control(struct rt_device* dev,
    int cmd,
    void* args)
{
    rt_err_t ret = RT_EOK;
    struct rt_device_serial* serial;

    RT_ASSERT(dev != RT_NULL);
    serial = (struct rt_device_serial*)dev;

    switch (cmd) {
    case RT_DEVICE_CTRL_SUSPEND:
        /* suspend device */
        dev->flag |= RT_DEVICE_FLAG_SUSPENDED;
        break;
    case RT_DEVICE_CTRL_RESUME:
        /* resume device */
        dev->flag &= ~RT_DEVICE_FLAG_SUSPENDED;
        break;
    case RT_DEVICE_CTRL_BLOCKING:
        if (((rt_uint32_t)args & RT_DEVICE_OFLAG_NONBLOCKING) == RT_DEVICE_OFLAG_NONBLOCKING) {
            dev->open_flag |= RT_DEVICE_OFLAG_NONBLOCKING;
        } else {
            dev->open_flag &= ~RT_DEVICE_OFLAG_NONBLOCKING;
        }
        break;
    case RT_DEVICE_CTRL_CONFIG:
        if (args) {
            struct serial_configure* pconfig = (struct serial_configure*)args;

            /* serial device has been opened, to configure it */
            ret = serial->ops->configure(dev, pconfig);
        }
        break;
#ifdef CONFIG_USE_RTOS
    case RT_DEVICE_CTRL_TIMEOUT: {
        rt_tick_t timeout_tick = (rt_tick_t)args;
        serial->timeout_tick = timeout_tick;
    } break;
#endif
    default:
        /* control device */
        ret = serial->ops->control(dev, cmd, args);
        break;
    }

    return ret;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops serial_ops = {
    .init = rt_serial_init,
    .open = rt_serial_open,
    .close = rt_serial_close,
    .read = rt_serial_read,
    .write = rt_serial_write,
    .flush = rt_serial_flush,
    .control = rt_serial_control
};
#endif

/* ISR for serial interrupt */
void rt_hw_serial_isr(struct rt_device* dev, int event)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_serial* serial = (struct rt_device_serial*)dev;

    switch (event & 0xff) {
    case RT_SERIAL_EVENT_RX_IND: {
        int ch = -1;
        struct rt_ringbuffer* rx_fifo;

        /* interrupt mode receive */
        rx_fifo = (struct rt_ringbuffer*)serial->serial_rx;

        ch = serial->ops->getc(dev);

        /* push a new data */
        rt_ringbuffer_putchar_force(rx_fifo, ch);

#ifdef CONFIG_USE_RTOS
        rt_completion_done(&(serial->completion_rx));
#endif
        /* invoke callback */
        if (dev->rx_indicate != RT_NULL) {
            rt_ssize_t rx_length;

            /* get rx length */
            rx_length = rt_ringbuffer_data_len(rx_fifo);
            if (rx_length) {
                dev->rx_indicate(dev, rx_length);
            }
        }
    } break;
    case RT_SERIAL_EVENT_TX_DONE: {
        rt_ssize_t len;
        rt_uint8_t ch = 0;
        struct rt_ringbuffer* tx_fifo;

        tx_fifo = (struct rt_ringbuffer*)serial->serial_tx;

        /* calucate fifo data size */
        len = rt_ringbuffer_data_len(tx_fifo);
        if (len == 0) {
            // TODO: stop tx
            serial->ops->stop_tx(dev);
#ifdef CONFIG_USE_RTOS
            rt_completion_done(&(serial->completion_tx));
#endif
            /* invoke callback */
            if (dev->tx_complete != RT_NULL) {
                dev->tx_complete(dev, (void*)len);
            }
            break;
        }
        /* pop one byte data */
        rt_ringbuffer_getchar(tx_fifo, &ch);

        serial->ops->putc(dev, ch);
    } break;
#ifdef CONFIG_SERIAL_USE_DMA_RX
    case RT_SERIAL_EVENT_RX_DMADONE: {
        int dma_idx, ch = -1;
        struct rt_ringbuffer* rx_fifo;

        dma_idx = event >> 8;

        if (serial->dma_idx_rx == dma_idx)
            break;

        rx_fifo = (struct rt_ringbuffer*)serial->serial_rx;

        while (serial->dma_idx_rx != dma_idx) {
            ch = serial->serial_dma_rx[serial->dma_idx_rx];

            /* push a new data */
            rt_ringbuffer_putchar_force(rx_fifo, ch);

            serial->dma_idx_rx++;
            if (serial->dma_idx_rx == CONFIG_SERIAL_DMA_RX_BUFSZ) {
                serial->dma_idx_rx = 0;
            }
        }
        serial->dma_idx_rx = dma_idx;
#ifdef CONFIG_USE_RTOS
        rt_completion_done(&(serial->completion_rx));
#endif

        /* invoke callback */
        if (dev->rx_indicate != RT_NULL) {
            rt_ssize_t rx_length;

            /* get rx length */
            rx_length = rt_ringbuffer_data_len(rx_fifo);
            if (rx_length) {
                dev->rx_indicate(dev, rx_length);
            }
        }
    } break;
    case RT_SERIAL_EVENT_TX_DMADONE: {
        int i;
        rt_ssize_t len;
        rt_uint8_t ch = 0;
        struct rt_ringbuffer* tx_fifo;

        tx_fifo = (struct rt_ringbuffer*)serial->serial_tx;

        /* calucate fifo data size */
        len = rt_ringbuffer_data_len(tx_fifo);
        if (len == 0) {
            // TODO: stop tx
            serial->ops->stop_dma_tx(serial);
#ifdef CONFIG_USE_RTOS
            rt_completion_done(&(serial->completion_tx));
#endif
            /* invoke callback */
            if (dev->tx_complete != RT_NULL) {
                dev->tx_complete(dev, (void*)len);
            }
            break;
        }

        if (len > CONFIG_SERIAL_DMA_TX_BUFSZ) {
            len = CONFIG_SERIAL_DMA_TX_BUFSZ;
        }
        /* read from software FIFO */
        rt_ringbuffer_get(tx_fifo, serial->serial_dma_tx, len);
        serial->ops->start_dma_tx(serial, serial->serial_dma_tx, len);
    } break;
#endif /* RT_SERIAL_USING_DMA */
    }
}

/*
 * serial register
 */
// rt_err_t rt_hw_serial_register(struct rt_device_serial* serial,
//     const char* name,
//     rt_uint32_t flag,
//     void* data)
// {
//     rt_err_t ret;
//     struct rt_device* device;
//     RT_ASSERT(serial != RT_NULL);

// #ifdef CONFIG_USE_RTOS
//     serial->timeout_tick = RT_WAITING_FOREVER;
// #endif

//     device = &(serial->dev);

//     device->type = RT_Device_Class_Char;
//     device->rx_indicate = RT_NULL;
//     device->tx_complete = RT_NULL;

// #ifdef RT_USING_DEVICE_OPS
//     device->ops = &serial_ops;
// #else
//     device->init = rt_serial_init;
//     device->open = rt_serial_open;
//     device->close = rt_serial_close;
//     device->read = rt_serial_read;
//     device->write = rt_serial_write;
//     device->flush = rt_serial_flush;
//     device->control = rt_serial_control;
// #endif
//     device->user_data = data;

//     /* register a character device */
//     ret = rt_device_register(device, name, flag);

//     return ret;
// }

/**
 * @brief 注册Serial设备
 * 
 * @param pin    Serial设备
 * @param name   设备名称
 * @param ops    设备操作符
 * @param user_data 自定义数据
 * @return 成功返回RT_EOK，其他见错误码
 */
rt_err_t rt_device_serial_register(struct rt_device_serial* serial, const char* name, const struct rt_serial_ops* ops, rt_uint32_t flag, void* user_data)
{
    RT_ASSERT(serial != RT_NULL);

    serial->dev.type = RT_Device_Class_Char;
    serial->dev.rx_indicate = RT_NULL;
    serial->dev.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    serial->dev.ops = &serial_ops;
#else
    serial->dev.init = RT_NULL;
    serial->dev.open = RT_NULL;
    serial->dev.close = RT_NULL;
    serial->dev.read = _pin_read;
    serial->dev.write = _pin_write;
    serial->dev.control = _pin_control;
#endif

#ifdef CONFIG_USE_RTOS
    serial->timeout_tick = RT_WAITING_FOREVER;
#endif
    serial->ops = ops;
    serial->dev.user_data = user_data;

    /* register a character device */
    return rt_device_register(&serial->dev, name, flag);
}

#ifdef __cplusplus
}
#endif
