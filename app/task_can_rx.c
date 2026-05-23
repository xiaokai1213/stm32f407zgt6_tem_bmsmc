#include "task_can_rx.h"
#include "bms_data.h"
#include "bms_state.h"
#include "can.h"
#include "ccs_detect.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"
#include "task_fault_detect.h"

/*******************************************************************************************************/
/* 全局变量 */

TaskHandle_t g_can_rx_task_handle = NULL; /* 本任务句柄 - 供BSP回调发通知 */
QueueHandle_t g_can_rx_queue = NULL;      /* CAN消息队列 - ISR→任务 */

/************************************************************************************************/

/**
 * @brief       CAN消息接收任务
 *   @note      无周期，阻塞在任务通知上等待接收。
 *              ISR → 接收消息入队列 + 发通知 → 本任务读取队列并解析。
 *              根据CAN ID分发到bms_data数据池进行结构化存储。
 * @param       argument: 任务参数（未使用）
 * @retval      无
 */
void Task_CAN_Rx(void *argument) {
    CAN_RxMsg_t rx_msg;

    /* 创建CAN消息队列 */
    g_can_rx_queue = xQueueCreate(CAN_RX_QUEUE_LENGTH, sizeof(CAN_RxMsg_t));
    if (g_can_rx_queue == NULL) { Error_Handler(); }

    /* 保存本任务句柄，供ISR发通知 */
    g_can_rx_task_handle = xTaskGetCurrentTaskHandle();

    while (1) {
        /* 阻塞等待CAN中断发来的通知（无限等待） */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* 从队列中读取所有待处理消息并解析 */
        while (xQueueReceive(g_can_rx_queue, &rx_msg, 0) == pdTRUE) {
            /* 根据CAN ID范围分发到对应PACK的解析函数 */
            if ((rx_msg.can_id >= PACK1_CELLV_ID_MIN) && (rx_msg.can_id <= PACK1_TEMPV_ID_MAX)) {
                BMS_ParsePack1VoltTemp(rx_msg.can_id, rx_msg.data);
            } else if ((rx_msg.can_id >= PACK2_CELLV_ID_MIN) && (rx_msg.can_id <= PACK2_TEMPV_ID_MAX)) {
                BMS_ParsePack2VoltTemp(rx_msg.can_id, rx_msg.data);
            } else if (rx_msg.can_id == PACK1_FAULT_VT_ID) {
                /* 从控VT故障报文解析 */
                BMS_ParsePack1FaultVT(rx_msg.data);
            } else if (rx_msg.can_id == PACK1_FAULT_BK_ID) {
                /* 从控BK故障报文解析 */
                BMS_ParsePack1FaultBK(rx_msg.data);
            } else if (rx_msg.can_id == CCS_TX_ID) {
                /* CCS充电机报文解析及事件处理，内部完成状态位检测，正常则发充电事件，异常则清除充电请求 */
                CCS_ProcessEvent(rx_msg.data);
            }
        }
    }
}
