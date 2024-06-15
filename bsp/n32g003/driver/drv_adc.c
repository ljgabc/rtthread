/**
 * @file drv_n32g003_adc.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief N32G003 API适配ADC驱动
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "config.h"

#if defined(CONFIG_BSP_N32G003) && defined(CONFIG_USE_ADC)

#include "bsp/n32g003/library/n32g003_adc.h"
#include "bsp/n32g003/library/n32g003_gpio.h"
#include "bsp/n32g003/library/n32g003_misc.h"
#include "bsp/n32g003/library/n32g003_rcc.h"
#include "components/device/rt_adc.h"
#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 引脚描述，
 * index：引脚在芯片上的物理编号
 * port：引脚所属端口，如A、B、C、D
 * pin：引脚在端口的序列号，如PA1，pin就是1
 */
#define __ADC(channel, index, port, pin)                                     \
    {                                                                        \
        index, channel, GPIO_PIN_##pin, RCC_APB_PERIPH_IOP##port, GPIO##port \
    }

struct adc_desc {
    uint8_t index; // 引脚物理编号
    uint8_t channel;
    uint16_t pin; // 引脚在端口的序列号
    uint32_t clk; // 引脚的时钟源
    GPIO_Module* periph; // 引脚所在总线端口
};

static const struct adc_desc adcs[] = {
    __ADC(0, 5, A, 1),
    __ADC(1, 6, A, 2),
    __ADC(2, 11, A, 5),
    __ADC(3, 19, A, 10),
    __ADC(4, 20, A, 12),
    __ADC(5, 14, A, 13),
    __ADC(6, 1, A, 14),
    __ADC(7, 2, B, 0),
    __ADC(8, 3, B, 1),
};

/**
 * @brief 通过引脚物理编号获取ADC描述结构体
 *
 * @param channel 通道
 * @return 引脚描述结构体
 */
rt_inline struct adc_desc* _get_adc(rt_base_t channel)
{
    if (channel == 0 || channel > 9) {
        return RT_NULL;
    }

    channel = channel - 1;
    return &adcs[channel];
}

static rt_err_t n32g003_adc_enable(const struct rt_device* dev, rt_base_t channel, rt_base_t mode)
{
    const struct adc_desc* adc = _get_adc(channel);
    if (!adc) {
        return RT_EINVAL;
    }

    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_ADC);
    RCC_APB_Peripheral_Clock_Enable(adc->clk);

    GPIO_InitType gpio_init_struct;
    GPIO_Structure_Initialize(&gpio_init_struct);
    gpio_init_struct.Pin = adc->pin;
    gpio_init_struct.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_Peripheral_Initialize(adc->periph, &gpio_init_struct);

    ADC_InitType adc_init_struct;
    adc_init_struct.MultiChEn = DISABLE;
    adc_init_struct.ContinueConvEn = DISABLE;
    adc_init_struct.ExtTrigSelect = ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH;
    adc_init_struct.DatAlign = ADC_DAT_ALIGN_R;
    adc_init_struct.ChsNumber = 1;
    ADC_Initializes(&adc_init_struct);

    ADC_ON();
    while (ADC_Flag_Status_Get(ADC_RD_FLAG, ADC_FLAG_STR, ADC_FLAG_RDY) == RESET)
        ;
    while (ADC_Flag_Status_Get(ADC_RD_FLAG, ADC_FLAG_STR, ADC_FLAG_PD_RDY))
        ;

    return RT_EOK;
}

/**
 * @brief 获取引脚电压
 *
 * @param channel 通道
 * @return ADC转换值
 */
static rt_uint32_t n32g003_adc_read(const struct rt_device* dev, rt_base_t channel)
{
    const struct adc_desc* adc = _get_adc(channel);
    if (!adc) {
        return RT_EINVAL;
    }

    ADC_Regular_Sequence_Single_Config(adc->channel);
    ADC_Channel_Sample_Time_Config(ADC_SAMP_TIME_240CYCLES);

    ADC_Regular_Channels_Software_Conversion_Operation(
        ADC_EXTRTRIG_SWSTRRCH_ENABLE);
    while (ADC_Flag_Status_Get(ADC_RUN_FLAG, ADC_FLAG_ENDC, ADC_FLAG_RDY) == 0) {
    }
    ADC_Flag_Status_Clear(ADC_FLAG_ENDC);
    ADC_Flag_Status_Clear(ADC_FLAG_STR);
    return ADC_Regular_Conversion_Data_Get();
}

rt_uint8_t n32g003_adc_get_resolution(struct rt_device* dev)
{
    return 12;
}

static const struct rt_adc_ops _n32g003_adc_ops = {
    .enable = n32g003_adc_enable,
    .read = n32g003_adc_read,
    .get_resolution = n32g003_adc_get_resolution,
};

static struct rt_device_adc _n32g003_adc;

int _n32g003_adc_init(void)
{
    return rt_device_adc_register(&_n32g003_adc, "adc", &_n32g003_adc_ops, RT_NULL);
}
INIT_DEVICE_EXPORT(_n32g003_adc_init);

#ifdef __cplusplus
}
#endif

#endif
