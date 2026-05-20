#include "adc.h"

ADC_HandleTypeDef g_adc1_handle; /* ADC1句柄 */

/**
 * @brief       ADC1初始化函数
 *   @note      使用ADC1_IN1(PA1)采集电池总电流
 *              ADC1_IN2(PA2)采集电池总电压
 *              ADC1_IN3(PA3)采集电池预充电压
 *              单次转换模式，软件触发，12位分辨率，长采样时间(480周期)提高精度
 *              ADC时钟 = APB2 / 4 = 84MHz / 4 = 21MHz
 *              转换时间 = (480 + 12) / 21MHz ≈ 23.4us/通道
 * @param       无
 * @retval      无
 */
void adc1_init(void) {
    GPIO_InitTypeDef gpio_init_struct;

    ADC_UX_CLK_ENABLE();

    /* ADC1_IN1(PA1), ADC1_IN2(PA2), ADC1_IN3(PA3) */
    ADC_BAT_CURRENT_GPIO_CLK_ENABLE();

    /* PA1 - 电池总电流 ADC输入 */
    gpio_init_struct.Pin = ADC_BAT_CURRENT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ADC_BAT_CURRENT_GPIO_PORT, &gpio_init_struct);

    /* PA2 - 电池总电压 ADC输入 */
    gpio_init_struct.Pin = ADC_BAT_VOLTAGE_GPIO_PIN;
    HAL_GPIO_Init(ADC_BAT_VOLTAGE_GPIO_PORT, &gpio_init_struct);

    /* PA3 - 电池预充电压 ADC输入 */
    gpio_init_struct.Pin = ADC_PRECHARGE_VOLTAGE_GPIO_PIN;
    HAL_GPIO_Init(ADC_PRECHARGE_VOLTAGE_GPIO_PORT, &gpio_init_struct);

    /* ADC1 句柄配置 */
    g_adc1_handle.Instance = ADC1;
    g_adc1_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;            /* APB2/4 = 21MHz */
    g_adc1_handle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 12位精度 */
    g_adc1_handle.Init.ScanConvMode = DISABLE;                               /* 非扫描模式（单通道） */
    g_adc1_handle.Init.ContinuousConvMode = DISABLE;                         /* 单次转换 */
    g_adc1_handle.Init.DiscontinuousConvMode = DISABLE;                      /* 禁止不连续模式 */
    g_adc1_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* 软件触发 */
    g_adc1_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    g_adc1_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT; /* 右对齐 */
    g_adc1_handle.Init.NbrOfConversion = 1;             /* 单通道转换 */
    g_adc1_handle.Init.DMAContinuousRequests = DISABLE;
    g_adc1_handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    HAL_ADC_Init(&g_adc1_handle);
}

/**
 * @brief       ADC MSP初始化函数（HAL_ADC_Init()会自动调用此函数）
 * @param       hadc: ADC句柄类型指针
 * @note        完成时钟使能
 * @retval      无
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) { ADC_UX_CLK_ENABLE(); /* ADC1 时钟使能 */ }
}

/**
 * @brief       读取电池总电流 (ADC1_IN1, PA1)
 * @param       无
 * @retval      ADC转换值 (0~4095)
 *   @note      高精度采样: 采样时间480周期
 */
uint16_t adc1_read_bat_current(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    uint16_t adc_value = 0;

    sConfig.Channel = ADC_CHANNEL_1;                 /* PA1 -> ADC1_IN1 */
    sConfig.Rank = 1;                                /* 正则组序号1 */
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; /* 长采样时间，提高精度 */
    HAL_ADC_ConfigChannel(&g_adc1_handle, &sConfig);

    HAL_ADC_Start(&g_adc1_handle);
    if (HAL_ADC_PollForConversion(&g_adc1_handle, 50) == HAL_OK) { adc_value = HAL_ADC_GetValue(&g_adc1_handle); }
    HAL_ADC_Stop(&g_adc1_handle);

    return adc_value;
}

/**
 * @brief       读取电池总电压 (ADC1_IN2, PA2)
 * @param       无
 * @retval      ADC转换值 (0~4095)
 *   @note      高精度采样: 采样时间480周期
 */
uint16_t adc1_read_bat_voltage(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    uint16_t adc_value = 0;

    sConfig.Channel = ADC_CHANNEL_2;                 /* PA2 -> ADC1_IN2 */
    sConfig.Rank = 1;                                /* 正则组序号1 */
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; /* 长采样时间，提高精度 */
    HAL_ADC_ConfigChannel(&g_adc1_handle, &sConfig);

    HAL_ADC_Start(&g_adc1_handle);
    if (HAL_ADC_PollForConversion(&g_adc1_handle, 50) == HAL_OK) { adc_value = HAL_ADC_GetValue(&g_adc1_handle); }
    HAL_ADC_Stop(&g_adc1_handle);

    return adc_value;
}

/**
 * @brief       读取电池预充电压 (ADC1_IN3, PA3)
 * @param       无
 * @retval      ADC转换值 (0~4095)
 *   @note      高精度采样: 采样时间480周期
 */
uint16_t adc1_read_precharge_voltage(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    uint16_t adc_value = 0;

    sConfig.Channel = ADC_CHANNEL_3;                 /* PA3 -> ADC1_IN3 */
    sConfig.Rank = 1;                                /* 正则组序号1 */
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; /* 长采样时间，提高精度 */
    HAL_ADC_ConfigChannel(&g_adc1_handle, &sConfig);

    HAL_ADC_Start(&g_adc1_handle);
    if (HAL_ADC_PollForConversion(&g_adc1_handle, 50) == HAL_OK) { adc_value = HAL_ADC_GetValue(&g_adc1_handle); }
    HAL_ADC_Stop(&g_adc1_handle);

    return adc_value;
}
