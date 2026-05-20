#ifndef _CAN_H
#define _CAN_H

#include "main.h"

/*******************************************************************************************************/
/* CAN1 引脚 和 外设 定义 */

#define CAN1_TX_GPIO_PORT GPIOA
#define CAN1_TX_GPIO_PIN  GPIO_PIN_12
#define CAN1_TX_GPIO_AF   GPIO_AF9_CAN1
#define CAN1_TX_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOA_CLK_ENABLE(); } while (0) /* 发送引脚时钟使能 */

#define CAN1_RX_GPIO_PORT GPIOA
#define CAN1_RX_GPIO_PIN  GPIO_PIN_11
#define CAN1_RX_GPIO_AF   GPIO_AF9_CAN1
#define CAN1_RX_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOA_CLK_ENABLE(); } while (0) /* 接收引脚时钟使能 */

#define CAN1_UX CAN1
#define CAN1_UX_CLK_ENABLE() \
    do { __HAL_RCC_CAN1_CLK_ENABLE(); } while (0) /* CAN1 时钟使能 */

/*******************************************************************************************************/
/* CAN2 引脚 和 外设 定义 */

#define CAN2_TX_GPIO_PORT GPIOB
#define CAN2_TX_GPIO_PIN  GPIO_PIN_13
#define CAN2_TX_GPIO_AF   GPIO_AF9_CAN2
#define CAN2_TX_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while (0) /* 发送引脚时钟使能 */

#define CAN2_RX_GPIO_PORT GPIOB
#define CAN2_RX_GPIO_PIN  GPIO_PIN_12
#define CAN2_RX_GPIO_AF   GPIO_AF9_CAN2
#define CAN2_RX_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while (0) /* 接收引脚时钟使能 */

#define CAN2_UX CAN2
#define CAN2_UX_CLK_ENABLE() \
    do { __HAL_RCC_CAN2_CLK_ENABLE(); } while (0) /* CAN2 时钟使能 */

/*******************************************************************************************************/

extern CAN_HandleTypeDef g_can1_handle; /* CAN1句柄 */
extern CAN_HandleTypeDef g_can2_handle; /* CAN2句柄 */

void can1_init(void); /* CAN1初始化函数 */
void can2_init(void); /* CAN2初始化函数 */

#endif
