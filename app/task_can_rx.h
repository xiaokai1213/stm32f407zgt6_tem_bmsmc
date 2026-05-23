#ifndef _TASK_CAN_RX_H
#define _TASK_CAN_RX_H

#include "can.h"
#include "main.h"
#include "queue.h"
#include "task.h"

/*******************************************************************************************************/
/* 全局变量声明 */

extern TaskHandle_t g_can_rx_task_handle; /* CAN接收任务句柄 */
extern QueueHandle_t g_can_rx_queue;      /* CAN消息队列句柄 */

#define CAN_RX_QUEUE_LENGTH 16 /* 队列深度 (5模组×7帧+余量) */

/*******************************************************************************************************/
/* 任务函数声明 */

void Task_CAN_Rx(void *argument); /* CAN消息接收任务入口函数 */

#endif /* _TASK_CAN_RX_H */
