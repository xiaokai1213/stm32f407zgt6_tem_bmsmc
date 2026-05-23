#include "system_configuration.h"

/**
 * @brief  配置系统时钟
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* 使能电源控制时钟并配置电压调节器 */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* 配置外部高速晶振（HSE）和PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

    /* 配置系统时钟源为PLL，AHB分频器为1，APB1分频器为4，APB2分频器为2 */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) { Error_Handler(); }
}

/**
 * @brief  中断优先级配置
 * @note   集中配置所有外设中断的抢占优先级和子优先级
 *         STM32F4使用4位优先级(中断分组4): 4位抢占优先级, 0位子优先级
 *         FreeRTOS要求: configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY = 5
 *         因此FreeRTOS API可安全调用的中断优先级为 5~15
 * @retval None
 */
void Interrupt_Config(void) {
    /* 设置中断优先级分组为4位抢占优先级（无子优先级） */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /*设置系统滴答定时器优先级--最低优先级15，确保FreeRTOS tick中断正常工作*/
    HAL_NVIC_EnableIRQ(SysTick_IRQn);          /* 使能SysTick中断 */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0); /*设置优先级为15 */

    /* CAN1 RX0中断 - 优先级4 (可调用FreeRTOS API) */
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);         /* 使能CAN1 RX0中断 */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 4, 0); /* 设置优先级为4 */

    /* CAN2 RX0中断 - 优先级4 (可调用FreeRTOS API) */
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);         /* 使能CAN2 RX0中断 */
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 4, 0); /* 设置优先级为4 */
}
