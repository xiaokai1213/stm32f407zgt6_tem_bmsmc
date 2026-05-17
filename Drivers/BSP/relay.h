#ifndef _RELAY_H
#define _RELAY_H

#include "main.h"

/*******************************************************************************************************/
/* 继电器 引脚 和 外设 定义 */

/* RELAY_BMS_FAULT_CON -> PD3 */
#define RELAY_BMS_FAULT_CON_GPIO_PORT     GPIOD
#define RELAY_BMS_FAULT_CON_GPIO_PIN      GPIO_PIN_3
#define RELAY_BMS_FAULT_CON_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)

/*******************************************************************************************************/
/* 继电器 操作宏
 *   @note  低电平 = 继电器吸合（报警状态）
 *          高电平 = 继电器释放（正常状态）
 */

#define RELAY_BMS_FAULT_CON_ON()     HAL_GPIO_WritePin(RELAY_BMS_FAULT_CON_GPIO_PORT, RELAY_BMS_FAULT_CON_GPIO_PIN, GPIO_PIN_RESET) /* 继电器吸合（低电平） */
#define RELAY_BMS_FAULT_CON_OFF()    HAL_GPIO_WritePin(RELAY_BMS_FAULT_CON_GPIO_PORT, RELAY_BMS_FAULT_CON_GPIO_PIN, GPIO_PIN_SET)   /* 继电器释放（高电平） */
#define RELAY_BMS_FAULT_CON_TOGGLE() HAL_GPIO_TogglePin(RELAY_BMS_FAULT_CON_GPIO_PORT, RELAY_BMS_FAULT_CON_GPIO_PIN)                /* 继电器翻转 */

/*******************************************************************************************************/

void relay_init(void); /* 继电器初始化函数 */

#endif
