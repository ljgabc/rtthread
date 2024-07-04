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

static rt_err_t n32g003_i2c_open(struct rt_device* dev, rt_uint16_t oflag)
{
    I2C_InitType i2c_master;
    GPIO_InitType i2c_gpio;

    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_I2C);
    RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_IOPA | RCC_APB_PERIPH_AFIO);

    GPIO_Structure_Initialize(&i2c_gpio);

    /*PA4 -- SCL; PA5 -- SDA*/
#if defined(CONFIG_N32G003_I2C1_SCL_PA2)
    i2c_gpio.Pin = GPIO_PIN_2;
#elif defined(CONFIG_N32G003_I2C1_SCL_PA4)
    i2c_gpio.Pin = GPIO_PIN_4;
#elif defined(CONFIG_N32G003_I2C1_SCL_PA9)
    i2c_gpio.Pin = GPIO_PIN_9;
#else
#error "Unsupported I2C SCL PIN."
#endif

#if defined(CONFIG_N32G003_I2C1_SDA_PA1)
    i2c_gpio.Pin |= GPIO_PIN_1;
#elif defined(CONFIG_N32G003_I2C1_SDA_PA3)
    i2c_gpio.Pin |= GPIO_PIN_3;
#elif defined(CONFIG_N32G003_I2C1_SDA_PA5)
    i2c_gpio.Pin |= GPIO_PIN_5;
#elif defined(CONFIG_N32G003_I2C1_SDA_PA8)
    i2c_gpio.Pin |= GPIO_PIN_8;
#else
#error "Unsupported I2C SDA PIN."
#endif
    i2c_gpio.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    i2c_gpio.GPIO_Mode = GPIO_MODE_AF_OD;
    i2c_gpio.GPIO_Alternate = GPIO_AF6_I2C;
    i2c_gpio.GPIO_Pull = GPIO_PULL_UP;
    GPIO_Peripheral_Initialize(GPIOA, &i2c_gpio);

    I2C_Reset(I2C);
    I2C_Initializes_Structure(&i2c_master);
    i2c_master.DutyCycle = I2C_SMDUTYCYCLE_1;
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

#ifdef __cplusplus
}
#endif

#endif
