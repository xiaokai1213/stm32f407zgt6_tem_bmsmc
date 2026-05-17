#ifndef __DELAY_H
#define __DELAY_H

#include "main.h"

/* 函数声明 */
void delay_init(void);       /* 延时初始化（TIM7） */
void delay_us(uint32_t nus); /* 微秒延时 */
void delay_ms(uint32_t nms); /* 毫秒延时 */

#endif
