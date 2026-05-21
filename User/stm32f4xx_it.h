#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler(void);        /* NMI异常处理函数，空实现 */
void HardFault_Handler(void);  /* HardFault异常处理函数，进入无限循环 */
void MemManage_Handler(void);  /* MemManage异常处理函数，进入无限循环 */
void BusFault_Handler(void);   /* BusFault异常处理函数，进入无限循环 */
void UsageFault_Handler(void); /* UsageFault异常处理函数，进入无限循环 */
void DebugMon_Handler(void);   /* DebugMon异常处理函数，空实现 */

/*  外设中断处理器名称请参考启动文件(startup_stm32f4xx.s)。 */
void USART1_IRQHandler(void); /* USART1中断处理函数 */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */
