#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含 ------------------------------------------------------------------*/
/* 导出的类型 ------------------------------------------------------------*/
/* 导出的常量 --------------------------------------------------------*/
/* 导出的宏 ------------------------------------------------------------*/
/* 导出的函数 ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);

/*  外设中断处理器名称请参考启动文件(startup_stm32f4xx.s)。 */
void USART1_IRQHandler(void); /* USART1中断处理函数 */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */
