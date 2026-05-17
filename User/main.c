#include "main.h"
#include "delay.h"
#include "sys.h"
#include "task.h"
#include "usart.h"

/**
 * @brief  主函数
 */
int main(void) {
    HAL_Init();           /* 初始化HAL库 */
    SystemClock_Config(); /* 配置系统时钟 */
    usart_init(115200);   /* 初始化USART1 */
    delay_init();         /* 初始化延时函数（TIM7） */

    /* 创建任务 */

    /* 启动FreeRTOS */
    vTaskStartScheduler();

    /* 如果调度器启动失败，程序会执行到这里 */
    vTaskEndScheduler(); /* 结束调度器 */
    while (1) {}
}

/**
 * @brief  错误处理函数
 */
void Error_Handler(void) {
    /* 用户可以添加自己的错误处理代码 */
    while (1) {}
}
