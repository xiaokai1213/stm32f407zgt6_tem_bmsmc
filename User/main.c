#include "main.h"
#include "adc.h"
#include "bms_data.h"
#include "bms_state.h"
#include "can.h"
#include "io_in.h"
#include "led.h"
#include "relay.h"
#include "system_configuration.h"
#include "task.h"
#include "task_adc1_sample.h"
#include "task_can_rx.h"
#include "task_can_tx.h"
#include "task_fault_detect.h"
#include "task_io_read.h"

/**
 * @brief  主函数
 */
int main(void) {
    HAL_Init();           /* 初始化HAL库 */
    SystemClock_Config(); /* 配置系统时钟 */
    Interrupt_Config();   /* 配置中断优先级 */

    /* 初始化板级外设 */
    relay_init(); /* 初始化继电器 */
    led_init();   /* 初始化LED指示灯 */
    adc1_init();  /* 初始化ADC外设 */
    io_in_init(); /* 初始化IO输入引脚 */
    can1_init();  /* 初始化CAN1 (500Kbps, 含接收中断) */
    can2_init();  /* 初始化CAN2 (500Kbps, 含接收中断) */

    /* FreeRTOS任务创建 */
    xTaskCreate(Task_BMS_Main, "BMS_Main", 256, NULL, 5, NULL);         /* 创建BMS主控任务 */
    xTaskCreate(Task_ADC1_Sample, "ADC1_Sample", 256, NULL, 3, NULL);   /* 创建ADC采样任务 (10ms周期) */
    xTaskCreate(Task_IO_Read, "IO_Read", 128, NULL, 4, NULL);           /* 创建IO读取任务 (50ms周期) */
    xTaskCreate(Task_CAN_Rx, "CAN_Rx", 256, NULL, 4, NULL);             /* 创建CAN接收任务 (事件驱动) */
    xTaskCreate(Task_CAN_Tx, "CAN_Tx", 256, NULL, 4, NULL);             /* 创建CAN发送任务 (20ms周期) */
    xTaskCreate(Task_Fault_Detect, "Fault_Detect", 256, NULL, 5, NULL); /* 创建故障检测任务 (200ms周期) */

    /* 启动任务调度器 */
    vTaskStartScheduler();

    /* 正常情况下不会执行到这里 */
    while (1) {}
}

/**
 * @brief  错误处理函数
 */
void Error_Handler(void) {
    /* 用户可以添加自己的错误处理代码 */
    while (1) {}
}

/*
xTaskCreate函数的参数说明：
- pxTaskCode：任务函数的指针
- pcName：任务名称（仅用于调试）
- uxStackDepth：任务堆栈大小，以字为单位
- pvParameters：传递给任务函数的参数
- uxPriority：任务优先级,数值越大优先级越高，最大优先级由configMAX_PRIORITIES定义
- pxCreatedTask：指向任务句柄的指针，用于返回创建的任务句柄
*/
