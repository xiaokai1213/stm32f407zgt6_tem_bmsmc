#ifndef _IO_IN_H
#define _IO_IN_H

#include "main.h"

/*******************************************************************************************************/
/* IO输入 引脚 和 外设 定义 */

/* IO_WAKE_UP_SIGNAL 唤醒信号输入 -> PA0 */
#define IO_WAKE_UP_SIGNAL_GPIO_PORT GPIOE
#define IO_WAKE_UP_SIGNAL_GPIO_PIN  GPIO_PIN_0
#define IO_WAKE_UP_SIGNAL_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0)

/* IO_FAULT00_SIGNAL 故障信号输入 -> PE1 */
#define IO_FAULT00_SIGNAL_GPIO_PORT GPIOE
#define IO_FAULT00_SIGNAL_GPIO_PIN  GPIO_PIN_1
#define IO_FAULT00_SIGNAL_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0)

/* IO_DI5_SIGNAL DI5信号输入 -> PE2 */
#define IO_DI5_SIGNAL_GPIO_PORT GPIOE
#define IO_DI5_SIGNAL_GPIO_PIN  GPIO_PIN_2
#define IO_DI5_SIGNAL_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0)

/*******************************************************************************************************/
/* 函数声明 */

void io_in_init(void);         /* IO输入初始化函数 */
uint8_t io_read_wake_up(void); /* 读取唤醒信号 (返回1:唤醒, 0:未唤醒) */
uint8_t io_read_fault00(void); /* 读取故障信号 (返回1:有效, 0:无效) */
uint8_t io_read_di5(void);     /* 读取DI5信号 (返回1:有效, 0:无效) */

#endif
