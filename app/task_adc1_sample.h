#ifndef _TASK_ADC1_SAMPLE_H
#define _TASK_ADC1_SAMPLE_H

#include "main.h"
#include "queue.h"

/*******************************************************************************************************/
/* 电压电流采样数据结构 */

typedef struct {
    int32_t bat_current;        /* 电池总电流 (mA，正=充电，负=放电) */
    uint32_t bat_voltage;       /* 电池总电压 (mV，0~500V对应0~500000mV) */
    uint32_t precharge_voltage; /* 电池预充电压 (mV，0~500V对应0~500000mV) */
} ADC_SampleData_t;

/*******************************************************************************************************/
/* ADC采样数据队列句柄声明 */

/* 电流传感器参数：分流器 ±300A，Vout = 1.25 ± 0.615V，Vref = 3.3V */
#define CURRENT_ZERO_ADC  1552U  /* 零电流时ADC理论值：(1.25 × 4096 / 3.3) */
#define CURRENT_SLOPE_MUL 402435 /* 转换因子分子：(3300 × 2439 × 1024) / 20480 */
#define CURRENT_SLOPE_SHR 10     /* 转换因子右移位数：÷ 1024 */

/* 电压检测参数：0~500V → 0~2V，分压比250，Vref = 3.3V */
#define VOLTAGE_SCALE_MUL 103125 /* 转换因子分子：(3300 × 250 × 512) / 4096 */
#define VOLTAGE_SCALE_SHR 9      /* 转换因子右移位数：÷ 512 */

#define ADC_SAMPLE_QUEUE_LENGTH 5        /* 队列深度：保留最近5组采样数据 */
extern QueueHandle_t g_adc_sample_queue; /* ADC采样数据队列句柄 */

/*******************************************************************************************************/
/* 函数声明 */

void Task_ADC1_Sample(void *argument);                 /* ADC1采样任务入口函数 */
void adc_sample_and_convert(ADC_SampleData_t *p_data); /* 采集ADC并转换为实际物理量 */

#endif // _TASK_ADC1_SAMPLE_H
