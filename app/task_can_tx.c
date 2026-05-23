#include "task_can_tx.h"
#include "can.h"
#include "ccs_detect.h"
#include "queue.h"
#include "task.h"

/*******************************************************************************************************/
/* 全局变量 */

QueueHandle_t g_can_tx_queue = NULL; /* CAN发送队列句柄 */

/*******************************************************************************************************/

/**
 * @brief       CAN发送任务（20ms周期任务）
 *   @note      每20ms轮询发送队列中的待发消息。
 *              根据CAN ID自动选择CAN1或CAN2发送。
 *              发送采用非阻塞模式。
 * @param       argument: 任务参数（未使用）
 * @retval      无
 */
void Task_CAN_Tx(void *argument) {
    TickType_t xLastWakeTime;
    CAN_TxMsg_t tx_msg;

    /* 创建CAN发送队列 */
    g_can_tx_queue = xQueueCreate(CAN_TX_QUEUE_LENGTH, sizeof(CAN_TxMsg_t));
    if (g_can_tx_queue == NULL) { Error_Handler(); }

    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        /* 等待下一个20ms周期 - 使用绝对延迟保证固定周期 */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));

        /* 从队列中取出所有待发送消息并发送 */
        while (xQueueReceive(g_can_tx_queue, &tx_msg, 0) == pdTRUE) {
            if (tx_msg.can_id == CCS_CTRL_ID) {
                can2_send(&tx_msg);
            } else {
                can1_send(&tx_msg);
            }
        }
    }
}
