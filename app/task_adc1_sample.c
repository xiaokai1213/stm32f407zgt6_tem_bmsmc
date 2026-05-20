#include "task_adc1_sample.h"
#include "adc.h"
#include "queue.h"
#include "task.h"

/*******************************************************************************************************/
/* 全局变量定义 */

QueueHandle_t g_adc_sample_queue = NULL; /* ADC采样数据队列句柄 */

/**
 * @brief       ADC采样任务（10ms周期任务）
 *   @note      使用vTaskDelayUntil实现精确的10ms固定周期采样
 *              采集ADC1_IN1(电池电流)、ADC1_IN2(电池电压)、ADC1_IN3(预充电压)
 *              采样结果通过队列发送，接收方通过xQueueReceive阻塞等待最新数据
 * @param       argument: 任务参数（未使用）
 * @retval      无
 */
void Task_ADC1_Sample(void *argument) {
    TickType_t xLastWakeTime;
    ADC_SampleData_t sample_data = {0};

    /* 创建ADC采样数据队列（仅在首次运行时创建一次） */
    if (g_adc_sample_queue == NULL) {
        g_adc_sample_queue = xQueueCreate(ADC_SAMPLE_QUEUE_LENGTH, sizeof(ADC_SampleData_t));
        if (g_adc_sample_queue == NULL) {
            Error_Handler();
        }
    }

    /* 获取当前系统滴答作为周期起始点 */
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        /* 等待下一个10ms周期 - 使用绝对延迟保证固定周期 */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));

        /* 采集三个通道的ADC数据 */
        sample_data.bat_current = adc1_read_bat_current();
        sample_data.bat_voltage = adc1_read_bat_voltage();
        sample_data.precharge_voltage = adc1_read_precharge_voltage();

        /* 将采集到的数据发送到队列
         * 如果队列已满，先丢弃最旧数据，确保接收方总能拿到最新数据 */
        if (xQueueSend(g_adc_sample_queue, &sample_data, 0) != pdPASS) {
            ADC_SampleData_t discard;
            /* 队列满：丢弃最旧的一条，腾出空间后再发送最新数据 */
            xQueueReceive(g_adc_sample_queue, &discard, 0);
            xQueueSend(g_adc_sample_queue, &sample_data, 0);
        }
    }
}
