#include "stm32f4xx_it.h"
#include "can.h"
#include "main.h"
#include "task.h"
#include "usart.h"

/* FreeRTOS 内核中断处理函数声明 (定义在 port.c 中) */
extern void xPortSysTickHandler(void);

/******************************************************************************/
/*                          Cortex-M4 处理器异常处理器                         */
/******************************************************************************/

/**
 * @brief  此函数处理NMI异常。
 * @param  无
 * @retval 无
 */
void NMI_Handler(void) {}

/**
 * @brief  此函数处理硬故障异常。
 * @param  无
 * @retval 无
 */
void HardFault_Handler(void) {
    /* 当硬故障异常发生时进入无限循环 */
    while (1) {}
}

/**
 * @brief  此函数处理内存管理异常。
 * @param  无
 * @retval 无
 */
void MemManage_Handler(void) {
    /* 当内存管理异常发生时进入无限循环 */
    while (1) {}
}

/**
 * @brief  此函数处理总线故障异常。
 * @param  无
 * @retval 无
 */
void BusFault_Handler(void) {
    /* 当总线故障异常发生时进入无限循环 */
    while (1) {}
}

/**
 * @brief  此函数处理使用故障异常。
 * @param  无
 * @retval 无
 */
void UsageFault_Handler(void) {
    /* 当使用故障异常发生时进入无限循环 */
    while (1) {}
}

/**
 * @brief  此函数处理调试监视器异常。
 * @param  无
 * @retval 无
 */
void DebugMon_Handler(void) {}

/**
 * @brief  此函数处理系统滴答定时器中断。
 * @param  无
 * @retval 无
 */
void SysTick_Handler(void) {
    HAL_IncTick();
    /* OS开始跑了,才执行正常的调度处理 */
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { xPortSysTickHandler(); /* 调用FreeRTOS的SysTick处理函数，进行任务调度 */ }
}

/******************************************************************************/
/*                 STM32F4xx 外设中断处理器                   */
/*  在此处添加用于使用的外设(PPP)的中断处理器，可用的 */
/*  外设中断处理器名称请参考启动文件(startup_stm32f4xx.s)。                                               */
/******************************************************************************/

/**
 * @brief  此函数处理USART1中断。
 * @param  无
 * @retval 无
 */
void USART1_IRQHandler(void) {
    /* 调用HAL库中断处理公用函数 */
    HAL_UART_IRQHandler(&g_uart1_handle);
}

/**
 * @brief  此函数处理CAN1 RX0中断（FIFO0消息挂起）。
 * @param  无
 * @retval 无
 */
void CAN1_RX0_IRQHandler(void) {
    /* 调用HAL库中断处理公用函数 */
    HAL_CAN_IRQHandler(&g_can1_handle);
}

/**
 * @brief  此函数处理CAN2 RX0中断（FIFO0消息挂起）。
 * @param  无
 * @retval 无
 */
void CAN2_RX0_IRQHandler(void) {
    /* 调用HAL库中断处理公用函数 */
    HAL_CAN_IRQHandler(&g_can2_handle);
}
