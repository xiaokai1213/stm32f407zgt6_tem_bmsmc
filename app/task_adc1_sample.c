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
 *              调用adc_sample_and_convert()执行采集转换，结果通过队列发送
 * @param       argument: 任务参数（未使用）
 * @retval      无
 */
void Task_ADC1_Sample(void *argument) {
    TickType_t xLastWakeTime;
    ADC_SampleData_t sample_data = {0}; /* 存储采样转换后的数据 */

    /* 创建ADC采样数据队列（仅在首次运行时创建一次） */
    if (g_adc_sample_queue == NULL) {
        g_adc_sample_queue = xQueueCreate(ADC_SAMPLE_QUEUE_LENGTH, sizeof(ADC_SampleData_t));
        if (g_adc_sample_queue == NULL) { Error_Handler(); }
    }

    /* 获取当前系统滴答作为周期起始点 */
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        /* 等待下一个10ms周期 - 使用绝对延迟保证固定周期 */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));

        /* 采集并转换三个通道数据 */
        adc_sample_and_convert(&sample_data);

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

/**
 * @brief       采集ADC并转换为实际物理量
 *   @note      采集ADC1_IN1(电池电流)、ADC1_IN2(电池电压)、ADC1_IN3(预充电压)
 *              内部完成定点数转换，输出实际mA/mV值
 * @param       p_data: 输出参数，填充采样转换后的数据
 * @retval      无
 */
void adc_sample_and_convert(ADC_SampleData_t *p_data) {
    uint16_t adc_raw;

    /* === 电流通道：分流器 ±300A，Vout = 1.25 ± 0.615V === */
    /* I(mA) = (adc_raw - 1552) × 402435 / 1024 */
    adc_raw = adc1_read_bat_current();                                     /* 读取电池总电流的ADC原始值 */
    int32_t diff = (int32_t)adc_raw - (int32_t)CURRENT_ZERO_ADC;           /* 与零电流点的差值，正负表示充放电方向 */
    p_data->bat_current = (diff * CURRENT_SLOPE_MUL) >> CURRENT_SLOPE_SHR; /* 转换为mA值，正=充电，负=放电 */

    /* === 电压通道：0~500V → 0~2V，分压比250 === */
    /* V(mV) = adc_raw × 103125 / 512 */
    adc_raw = adc1_read_bat_voltage();                                                        /* 读取电池总电压的ADC原始值 */
    p_data->bat_voltage = ((uint32_t)adc_raw * VOLTAGE_SCALE_MUL) >> VOLTAGE_SCALE_SHR;       /* 转换为mV值 */
    adc_raw = adc1_read_precharge_voltage();                                                  /* 读取电池预充电压的ADC原始值 */
    p_data->precharge_voltage = ((uint32_t)adc_raw * VOLTAGE_SCALE_MUL) >> VOLTAGE_SCALE_SHR; /* 转换为mV值 */
}
