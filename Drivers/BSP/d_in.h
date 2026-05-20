#ifndef _D_IN_H
#define _D_IN_H

#include "main.h"

/*******************************************************************************************************/
/* 数字量输入 引脚 和 外设 定义 */

/* DI_WAKE_UP_SIGNAL 唤醒信号输入 -> PA0 */
#define DI_WAKE_UP_SIGNAL_GPIO_PORT GPIOA
#define DI_WAKE_UP_SIGNAL_GPIO_PIN  GPIO_PIN_0
#define DI_WAKE_UP_SIGNAL_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOA_CLK_ENABLE(); } while (0)

/*******************************************************************************************************/
/* 数字量输入 操作宏 */

/* DI_WAKE_UP_SIGNAL 读取 */
#define DI_WAKE_UP_SIGNAL_READ() (HAL_GPIO_ReadPin(DI_WAKE_UP_SIGNAL_GPIO_PORT, DI_WAKE_UP_SIGNAL_GPIO_PIN) == GPIO_PIN_SET)

/*******************************************************************************************************/

void d_in_init(void); /* 数字量输入初始化函数 */

#endif
