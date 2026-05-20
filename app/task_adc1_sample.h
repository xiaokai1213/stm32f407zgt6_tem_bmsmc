#ifndef _TASK_ADC1_SAMPLE_H
#define _TASK_ADC1_SAMPLE_H

#include "main.h"
#include "queue.h"

/*******************************************************************************************************/
/* ADC采样数据结构 */

typedef struct {
    uint16_t bat_current;       /* 电池总电流 ADC原始值 */
    uint16_t bat_voltage;       /* 电池总电压 ADC原始值 */
    uint16_t precharge_voltage; /* 电池预充电压 ADC原始值 */
} ADC_SampleData_t;

/*******************************************************************************************************/
/* ADC采样数据队列句柄声明 */

#define ADC_SAMPLE_QUEUE_LENGTH 5        /* 队列深度：保留最近5组采样数据 */
extern QueueHandle_t g_adc_sample_queue; /* ADC采样数据队列句柄 */

/*******************************************************************************************************/
/* 任务函数声明 */

void Task_ADC1_Sample(void *argument); /* ADC1采样任务入口函数 */

#endif // _TASK_ADC1_SAMPLE_H
