#ifndef _ADC_H
#define _ADC_H

#include "main.h"

/*******************************************************************************************************/
/* ADC 引脚 和 外设 定义 */

/* 电池总电流 -> PA1 -> ADC1_IN1 */
#define ADC_BAT_CURRENT_GPIO_PORT GPIOA
#define ADC_BAT_CURRENT_GPIO_PIN  GPIO_PIN_1
#define ADC_BAT_CURRENT_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOA_CLK_ENABLE(); } while (0)

/* 电池总电压 -> PA2 -> ADC1_IN2 */
#define ADC_BAT_VOLTAGE_GPIO_PORT GPIOA
#define ADC_BAT_VOLTAGE_GPIO_PIN  GPIO_PIN_2
#define ADC_BAT_VOLTAGE_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOA_CLK_ENABLE(); } while (0)

/* 电池预充电压 -> PA3 -> ADC1_IN3 */
#define ADC_PRECHARGE_VOLTAGE_GPIO_PORT GPIOA
#define ADC_PRECHARGE_VOLTAGE_GPIO_PIN  GPIO_PIN_3
#define ADC_PRECHARGE_VOLTAGE_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOA_CLK_ENABLE(); } while (0)

#define ADC_UX ADC1
#define ADC_UX_CLK_ENABLE() \
    do { __HAL_RCC_ADC1_CLK_ENABLE(); } while (0)

/*******************************************************************************************************/

extern ADC_HandleTypeDef g_adc1_handle; /* ADC1句柄 */

void adc1_init(void);                       /* ADC1初始化函数 */
uint16_t adc1_read_bat_current(void);       /* 读取电池总电流 */
uint16_t adc1_read_bat_voltage(void);       /* 读取电池总电压 */
uint16_t adc1_read_precharge_voltage(void); /* 读取电池预充电压 */

#endif
