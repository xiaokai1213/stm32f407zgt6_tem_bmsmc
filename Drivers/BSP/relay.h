#ifndef _RELAY_H
#define _RELAY_H

#include "main.h"

/*******************************************************************************************************/
/* 继电器 引脚 和 外设 定义 */

/* RELAY_BMS_FAULT_CON -> PD3 */
#define RELAY_BMS_FAULT_CON_GPIO_PORT GPIOD
#define RELAY_BMS_FAULT_CON_GPIO_PIN  GPIO_PIN_3
#define RELAY_BMS_FAULT_CON_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)

/* RELAY_DI0_CON -> PD4 */
#define RELAY_DI0_CON_GPIO_PORT GPIOD
#define RELAY_DI0_CON_GPIO_PIN  GPIO_PIN_4
#define RELAY_DI0_CON_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)

/* RELAY_DI1_CON -> PD5 */
#define RELAY_DI1_CON_GPIO_PORT GPIOD
#define RELAY_DI1_CON_GPIO_PIN  GPIO_PIN_5
#define RELAY_DI1_CON_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)

/* RELAY_PRE_H_CON -> PD8 */
#define RELAY_PRE_H_CON_GPIO_PORT GPIOD
#define RELAY_PRE_H_CON_GPIO_PIN  GPIO_PIN_8
#define RELAY_PRE_H_CON_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)

/* RELAY_AIR_H_CON -> PD9 */
#define RELAY_AIR_H_CON_GPIO_PORT GPIOD
#define RELAY_AIR_H_CON_GPIO_PIN  GPIO_PIN_9
#define RELAY_AIR_H_CON_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)

/* RELAY_AIR_L_CON -> PD10 */
#define RELAY_AIR_L_CON_GPIO_PORT GPIOD
#define RELAY_AIR_L_CON_GPIO_PIN  GPIO_PIN_10
#define RELAY_AIR_L_CON_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)

/*******************************************************************************************************/

/* 继电器 操作宏 */

/* RELAY_BMS_FAULT_CON */
#define RELAY_BMS_FAULT_CON_ON()     HAL_GPIO_WritePin(RELAY_BMS_FAULT_CON_GPIO_PORT, RELAY_BMS_FAULT_CON_GPIO_PIN, GPIO_PIN_RESET) /* 继电器吸合（低电平） */
#define RELAY_BMS_FAULT_CON_OFF()    HAL_GPIO_WritePin(RELAY_BMS_FAULT_CON_GPIO_PORT, RELAY_BMS_FAULT_CON_GPIO_PIN, GPIO_PIN_SET)   /* 继电器释放（高电平） */
#define RELAY_BMS_FAULT_CON_TOGGLE() HAL_GPIO_TogglePin(RELAY_BMS_FAULT_CON_GPIO_PORT, RELAY_BMS_FAULT_CON_GPIO_PIN)                /* 继电器翻转 */

/* RELAY_DI0_CON */
#define RELAY_DI0_CON_ON()     HAL_GPIO_WritePin(RELAY_DI0_CON_GPIO_PORT, RELAY_DI0_CON_GPIO_PIN, GPIO_PIN_RESET) /* 继电器吸合（低电平） */
#define RELAY_DI0_CON_OFF()    HAL_GPIO_WritePin(RELAY_DI0_CON_GPIO_PORT, RELAY_DI0_CON_GPIO_PIN, GPIO_PIN_SET)   /* 继电器释放（高电平） */
#define RELAY_DI0_CON_TOGGLE() HAL_GPIO_TogglePin(RELAY_DI0_CON_GPIO_PORT, RELAY_DI0_CON_GPIO_PIN)                /* 继电器翻转 */

/* RELAY_DI1_CON */
#define RELAY_DI1_CON_ON()     HAL_GPIO_WritePin(RELAY_DI1_CON_GPIO_PORT, RELAY_DI1_CON_GPIO_PIN, GPIO_PIN_RESET) /* 继电器吸合（低电平） */
#define RELAY_DI1_CON_OFF()    HAL_GPIO_WritePin(RELAY_DI1_CON_GPIO_PORT, RELAY_DI1_CON_GPIO_PIN, GPIO_PIN_SET)   /* 继电器释放（高电平） */
#define RELAY_DI1_CON_TOGGLE() HAL_GPIO_TogglePin(RELAY_DI1_CON_GPIO_PORT, RELAY_DI1_CON_GPIO_PIN)                /* 继电器翻转 */

/* RELAY_PRE_H_CON */
#define RELAY_PRE_H_CON_ON()     HAL_GPIO_WritePin(RELAY_PRE_H_CON_GPIO_PORT, RELAY_PRE_H_CON_GPIO_PIN, GPIO_PIN_RESET) /* 继电器吸合（低电平） */
#define RELAY_PRE_H_CON_OFF()    HAL_GPIO_WritePin(RELAY_PRE_H_CON_GPIO_PORT, RELAY_PRE_H_CON_GPIO_PIN, GPIO_PIN_SET)   /* 继电器释放（高电平） */
#define RELAY_PRE_H_CON_TOGGLE() HAL_GPIO_TogglePin(RELAY_PRE_H_CON_GPIO_PORT, RELAY_PRE_H_CON_GPIO_PIN)                /* 继电器翻转 */

/* RELAY_AIR_H_CON */
#define RELAY_AIR_H_CON_ON()     HAL_GPIO_WritePin(RELAY_AIR_H_CON_GPIO_PORT, RELAY_AIR_H_CON_GPIO_PIN, GPIO_PIN_RESET) /* 继电器吸合（低电平） */
#define RELAY_AIR_H_CON_OFF()    HAL_GPIO_WritePin(RELAY_AIR_H_CON_GPIO_PORT, RELAY_AIR_H_CON_GPIO_PIN, GPIO_PIN_SET)   /* 继电器释放（高电平） */
#define RELAY_AIR_H_CON_TOGGLE() HAL_GPIO_TogglePin(RELAY_AIR_H_CON_GPIO_PORT, RELAY_AIR_H_CON_GPIO_PIN)                /* 继电器翻转 */

/* RELAY_AIR_L_CON */
#define RELAY_AIR_L_CON_ON()     HAL_GPIO_WritePin(RELAY_AIR_L_CON_GPIO_PORT, RELAY_AIR_L_CON_GPIO_PIN, GPIO_PIN_RESET) /* 继电器吸合（低电平） */
#define RELAY_AIR_L_CON_OFF()    HAL_GPIO_WritePin(RELAY_AIR_L_CON_GPIO_PORT, RELAY_AIR_L_CON_GPIO_PIN, GPIO_PIN_SET)   /* 继电器释放（高电平） */
#define RELAY_AIR_L_CON_TOGGLE() HAL_GPIO_TogglePin(RELAY_AIR_L_CON_GPIO_PORT, RELAY_AIR_L_CON_GPIO_PIN)                /* 继电器翻转 */

/*******************************************************************************************************/

void relay_init(void); /* 继电器初始化函数 */

#endif
