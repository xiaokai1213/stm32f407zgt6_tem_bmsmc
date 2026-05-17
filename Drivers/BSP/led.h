#ifndef _LED_H
#define _LED_H

#include "main.h"

/*******************************************************************************************************/
/* LED 引脚 和 外设 定义 */

/* LED_G 绿灯 -> PB0 */
#define LED_G_GPIO_PORT GPIOB
#define LED_G_GPIO_PIN  GPIO_PIN_0
#define LED_G_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while (0)

/* LED_R 红灯 -> PB1 */
#define LED_R_GPIO_PORT GPIOB
#define LED_R_GPIO_PIN  GPIO_PIN_1
#define LED_R_GPIO_CLK_ENABLE() \
    do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while (0)

/*******************************************************************************************************/
/* LED 操作宏 */

#define LED_G_ON()     HAL_GPIO_WritePin(LED_G_GPIO_PORT, LED_G_GPIO_PIN, GPIO_PIN_SET)   /* 绿灯亮 */
#define LED_G_OFF()    HAL_GPIO_WritePin(LED_G_GPIO_PORT, LED_G_GPIO_PIN, GPIO_PIN_RESET) /* 绿灯灭 */
#define LED_G_TOGGLE() HAL_GPIO_TogglePin(LED_G_GPIO_PORT, LED_G_GPIO_PIN)                /* 绿灯翻转 */

#define LED_R_ON()     HAL_GPIO_WritePin(LED_R_GPIO_PORT, LED_R_GPIO_PIN, GPIO_PIN_SET)   /* 红灯亮 */
#define LED_R_OFF()    HAL_GPIO_WritePin(LED_R_GPIO_PORT, LED_R_GPIO_PIN, GPIO_PIN_RESET) /* 红灯灭 */
#define LED_R_TOGGLE() HAL_GPIO_TogglePin(LED_R_GPIO_PORT, LED_R_GPIO_PIN)                /* 红灯翻转 */

/*******************************************************************************************************/

void led_init(void); /* LED 初始化函数 */

#endif
