#ifndef _TASK_CAN_TX_H
#define _TASK_CAN_TX_H

#include "can.h"
#include "main.h"

/*******************************************************************************************************/
/* CAN发送队列 - 用于任务间传递待发送CAN消息 */

#define CAN_TX_QUEUE_LENGTH 16 /* 发送队列深度 */

extern QueueHandle_t g_can_tx_queue; /* CAN发送队列句柄 */

/*******************************************************************************************************/
/* 任务函数声明 */

void Task_CAN_Tx(void *argument); /* CAN发送任务入口函数 */

#endif /* _TASK_CAN_TX_H */
