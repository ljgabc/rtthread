/**
 * @file drv_i2c.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief N32G003 API适配I2C驱动
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "config.h"

#if defined(CONFIG_BSP_N32G003) && defined(CONFIG_USING_I2C) && defined(CONFIG_BSP_I2C1)

#include "bsp/n32g003/library/n32g003_gpio.h"
#include "bsp/n32g003/library/n32g003_i2c.h"
#include "bsp/n32g003/library/n32g003_misc.h"
#include "bsp/n32g003/library/n32g003_rcc.h"
#include "components/device/rt_i2c.h"
#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_N32G003_I2C1_SCL_PA2)
#define PIN_SCL GPIO_PIN_2
#elif defined(CONFIG_N32G003_I2C1_SCL_PA4)
#define PIN_SCL GPIO_PIN_4
#elif defined(CONFIG_N32G003_I2C1_SCL_PA9)
#define PIN_SCL GPIO_PIN_9
#else
#error "Unsupported I2C SCL PIN."
#endif

#if defined(CONFIG_N32G003_I2C1_SDA_PA1)
#define PIN_SDA GPIO_PIN_1
#elif defined(CONFIG_N32G003_I2C1_SDA_PA3)
#define PIN_SDA GPIO_PIN_3
#elif defined(CONFIG_N32G003_I2C1_SDA_PA5)
#define PIN_SDA GPIO_PIN_5
#elif defined(CONFIG_N32G003_I2C1_SDA_PA8)
#define PIN_SDA GPIO_PIN_8
#else
#error "Unsupported I2C SDA PIN."
#endif

static void n32g003_udelay(rt_uint32_t us)
{
    rt_uint32_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->LOAD;

    ticks = us * reload / (1000000 / RT_TICK_PER_SECOND);
    told = SysTick->VAL;
    while (1) {
        tnow = SysTick->VAL;
        if (tnow != told) {
            if (tnow < told) {
                tcnt += told - tnow;
            } else {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) {
                break;
            }
        }
    }
}

/**
 * @brief i2c stop发送（可恢复从机锁死总线）
 */
static void n32g003_i2c_stop(struct rt_device* dev)
{
    RT_ASSERT(dev != RT_NULL);
    struct rt_device_i2c* i2c = (struct rt_device_i2c*)dev;

    I2C_Acknowledg_Disable(I2C);

    if (I2C_Flag_Status_Get(I2C, I2C_FLAG_BUSY)) {
        I2C_Generate_Stop_Enable(I2C);
        for (int i = 0; i < 10; i++) {
            I2C_OFF(I2C);

            GPIO_Pins_Set(GPIOA, PIN_SCL | PIN_SDA);
            GPIO_Mode_Set(GPIOA, GPIO_MODE_OUT_OD, PIN_SCL | PIN_SDA);

            for (int i = 0; i < 9; i++) {
                GPIO_Pins_Reset(GPIOA, PIN_SCL);
                n32g003_udelay(5);
                GPIO_Pins_Set(GPIOA, PIN_SCL);
                n32g003_udelay(5);
            }

            n32g003_i2c_open(dev, 0x00);

            if((I2C->CTRL1 & I2C_STOP_SET) == 0x00) {
                return;
            }
        }
    }
}

static rt_err_t n32g003_i2c_wait_flag(struct rt_device* dev, i2c_flag_enum flag ,FlagStatus wait_state)
{
    uint64_t timeout = driver_tick;    
    while(wait_state!=i2c_flag_get(i2cx->i2c_x, flag)){
        if((timeout+I2C_TIMEOUT_MS) <= driver_tick) {              
            return DRV_ERROR;
        } 
    }
    return DRV_SUCCESS;
}

static rt_err_t n32g003_i2c_open(struct rt_device* dev, rt_uint16_t oflag)
{
    I2C_InitType i2c_master;
    GPIO_InitType i2c_gpio;

    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_I2C);
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA | RCC_APB_PERIPH_AFIO);

    GPIO_Structure_Initialize(&i2c_gpio);

    i2c_gpio.Pin = PIN_SCL | PIN_SDA;
    i2c_gpio.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    i2c_gpio.GPIO_Mode = GPIO_MODE_AF_OD;
    i2c_gpio.GPIO_Alternate = GPIO_AF6_I2C;
    i2c_gpio.GPIO_Pull = GPIO_PULL_UP;
    GPIO_Peripheral_Initialize(GPIOA, &i2c_gpio);

    I2C_Reset(I2C);
    I2C_Initializes_Structure(&i2c_master);
    i2c_master.DutyCycle = I2C_FMDUTYCYCLE_2;
    i2c_master.OwnAddr1 = 0x0000;
    i2c_master.AckEnable = I2C_ACKEN;
    i2c_master.AddrMode = I2C_ADDR_MODE_7BIT;
    i2c_master.ClkSpeed = 100 * 1000;

    I2C_Initializes(I2C, &i2c_master);

    I2C_ON(I2C);

    return RT_EOK;
}

static rt_err_t n32g003_i2c_control(struct rt_device* dev, int cmd, void* arg)
{
    RT_ASSERT(dev != RT_NULL);
    rt_uint32_t ctrl_arg = (rt_uint32_t)(arg);

    switch (cmd) {
    case RT_DEVICE_CTRL_OPEN:
        break;
    case RT_DEVICE_CTRL_CLOSE:
        NVIC_DisableIRQ(I2C_EV_IRQn);
        NVIC_DisableIRQ(I2C_ER_IRQn);
        I2C_Interrupts_Disable(I2C, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR);
        I2C_Interrupt_Statu_Clear(I2C, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR);
        break;
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable interrupt */
        I2C_Interrupts_Disable(I2C, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR);
        I2C_Interrupt_Statu_Clear(I2C, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR);
        NVIC_DisableIRQ(I2C_EV_IRQn);
        NVIC_DisableIRQ(I2C_ER_IRQn);
        break;
    case RT_DEVICE_CTRL_SET_INT:
        /* enable interrupt */
        I2C_Interrupts_Enable(I2C, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR);
        I2C_Interrupt_Statu_Clear(I2C, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR);
        NVIC_EnableIRQ(I2C_EV_IRQn);
        NVIC_EnableIRQ(I2C_ER_IRQn);
        break;
    case RT_DEVICE_CTRL_CONFIG:
        I2C_Clock_Speed_Config(I2C, ctrl_arg, I2C_FMDUTYCYCLE_2);
        break;
    default:
        break;
    }
    return RT_EOK;
}

static rt_ssize_t n32g003_i2c_write(const struct rt_device* dev, rt_uint32_t address, const char* buffer, rt_ssize_t size,
    rt_base_t flags)
{
    char* pbuf = (char*)buffer;

    if (I2C_Flag_Status_Get(I2C, I2C_FLAG_BUSY) != RESET) {
        n32g003_i2c_stop(dev);
    }

    if (I2C_Flag_Status_Get(I2C, I2C_FLAG_BUSY) != RESET) {
        return 0;
    }

    I2C_Generate_Start_Enable(I2C);

    I2C_7bit_Addr_Send(I2C, address, I2C_DIRECTION_SEND);

    /* send data */
    while (size-- > 0) {
        I2C_Data_Send(I2C, *pbuf++);
    }

    if ((flags & RT_I2C_FLAG_NO_STOP) != RT_I2C_FLAG_NO_STOP) {
        I2C_Generate_Stop_Enable(I2C);
    }

    return size;
}

static rt_ssize_t n32g003_i2c_read(const struct rt_device* dev, rt_uint32_t address, char* buffer, rt_ssize_t size,
    rt_base_t flags)
{
    char* pbuf = buffer;

    if (I2C_Flag_Status_Get(I2C, I2C_FLAG_BUSY)) {
        return 0;
    }

    I2C_Acknowledg_Enable(I2C);

    /* send start */
    I2C_Generate_Start_Enable(I2C);

    /* send addr */
    I2C_7bit_Addr_Send(I2C, address, I2C_DIRECTION_RECV);

    if (size == 1) {
        I2C_Acknowledg_Disable(I2C);

        I2C_Generate_Stop_Enable(I2C);

        *pbuf++ = I2C_Data_Recv(I2C);

    } else if (size == 2) {
        I2C->CTRL1 |= 0x0800; /*/ set ACKPOS */
        I2C_Acknowledg_Disable(I2C);

        I2C_Generate_Stop_Enable(I2C);

        *pbuf++ = I2C_Data_Recv(I2C);
        *pbuf++ = I2C_Data_Recv(I2C);
    } else {
        I2C_Acknowledg_Enable(I2C);

        while (size) {
            if (size == 3) {
                I2C_Acknowledg_Disable(I2C);
                *pbuf++ = I2C_Data_Recv(I2C);
                size--;
                I2C_Generate_Stop_Enable(I2C);

                *pbuf++ = I2C_Data_Recv(I2C);
                size--;
                *pbuf++ = I2C_Data_Recv(I2C);
                size--;
                break;
            }

            *pbuf++ = I2C_Data_Recv(I2C);
            size--;
        }
    }

    return 0;
}

static const struct rt_i2c_ops _n32g003_i2c_ops = {
    .open = n32g003_i2c_open,
    .control = n32g003_i2c_control,
    .write = n32g003_i2c_write,
    .read = n32g003_i2c_read,
};

static struct rt_device_i2c _n32g003_i2c;

int _n32g003_i2c_init(void)
{
    return rt_device_i2c_register(&_n32g003_i2c, "i2c", &_n32g003_i2c_ops,
        RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX, RT_NULL);
}
INIT_DEVICE_EXPORT(_n32g003_i2c_init);

void I2C_EV_IRQHandler(void)
{
    uint32_t last_event = 0;

    last_event = I2C_Last_Event_Get(I2C);
    if ((last_event & I2C_ROLE_MASTER) == I2C_ROLE_MASTER) /* master mode */
    {
        switch (last_event)
        {
        case I2C_EVT_MASTER_MODE_FLAG: /* 0x00030001.EV5 Send addr */
            
            if(flag_trans_direct)     /* read */
            {
                Memset(rx_buf, 0, TEST_BUFFER_SIZE); /* clear recv buf, ready to recv data */
                I2C_7bit_Addr_Send(I2C, I2C_SLAVE_ADDR, I2C_DIRECTION_RECV);
                rx_num = 0;
            }
            else /* write */
            {
                I2C_7bit_Addr_Send(I2C, I2C_SLAVE_ADDR, I2C_DIRECTION_SEND);
                tx_num = 0;
            }
            break;

           /* MasterTransmitter	 */
        case I2C_EVT_MASTER_TXMODE_FLAG: /* 0x00070082. EV6 Send first data */
            Comm_Flag = C_READY;
            I2C_Data_Send(I2C, tx_buf[tx_num++]);
            break;

        case I2C_EVT_MASTER_DATA_SENDING: /* 0x00070080. EV8 Sending data  */
            if (tx_num < TEST_BUFFER_SIZE)
            {
                I2C_Data_Send(I2C, tx_buf[tx_num++]);
            }
            break;
            
        case I2C_EVT_MASTER_DATA_SENDED: /* 0x00070084.EV8_2 Send finish */
            if (tx_num == TEST_BUFFER_SIZE)
            {
                if (Comm_Flag == C_READY)
                {
                    Comm_Flag = C_STOP_BIT;
                    I2C_Generate_Stop_Enable(I2C);
                    flag_master_send_finish = 1;
                }
            }
            break;
            
            /* MasterReceiver */
        case I2C_EVT_MASTER_RXMODE_FLAG: /* 0x00030002.EV6 */
            Comm_Flag = C_READY;
            if (TEST_BUFFER_SIZE == 1)
            {
                I2C_Acknowledg_Disable(I2C);
                if (Comm_Flag == C_READY)
                {
                    Comm_Flag = C_STOP_BIT;
                    I2C_Generate_Stop_Enable(I2C);
                }
            }
            else if (TEST_BUFFER_SIZE == 2)
            {
                I2C->CTRL1 |= I2C_NACK_POS_NEXT; /* set ACKPOS */
                I2C_Acknowledg_Disable(I2C);
            }
            break;

        case I2C_EVT_MASTER_DATA_RECVD_FLAG: /* 0x00030040. EV7.one byte recved  */
        case I2C_EVT_MASTER_DATA_RECVD_BSF_FLAG: /* 0x00030044. EV7.When the I2C communication rate is too high, BSF = 1 */
            
            rx_buf[rx_num++] = I2C_Data_Recv(I2C);
            if (rx_num == (TEST_BUFFER_SIZE - 1))
            { 
                I2C_Acknowledg_Disable(I2C);   /* Disable I2C acknowledgement. */
                if (Comm_Flag == C_READY)
                {
                    Comm_Flag = C_STOP_BIT;
                    I2C_Generate_Stop_Enable(I2C); /* Send I2C STOP Condition. */
                }
            }
            else if (rx_num == TEST_BUFFER_SIZE)
            {
                flag_master_recv_finish = 1;
            }
            break;
        case 0x00030201: /* Arbitration lost */
        case 0x00030401: /* Acknowledge failure */
        case 0x00030501: /* Acknowledge failure and Bus error */
        case 0x00070000: /* TRF, BUSY, MSMODE flags */
            I2C_OFF(I2C);
            I2C_ON(I2C);
            break;

        default:
            log_info("I2C error status:0x%x\r\n", last_event);
            break;
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif
