#include "delay.h"

TIM_HandleTypeDef g_tim7_handle; /* TIM7 句柄 */
uint8_t g_fac_us = 0;            /* 微秒计数因子 */
uint16_t g_fac_ms = 0;           /* 毫秒计数因子 */

/**
 * @brief       初始化延时函数（TIM7 定时器）
 *   @note      TIM7 挂载于 APB1，时钟频率 = 84MHz（APB1=42MHz × 2）
 *              预分频后计数频率 = 84MHz / (prescaler + 1) = 1MHz（1us 计数一次）
 *              ARR = 0xFFFF（最大计数值 65535）
 * @param       无
 * @retval      无
 */
void delay_init(void) {
    __HAL_RCC_TIM7_CLK_ENABLE(); /* 使能 TIM7 时钟 */

    g_tim7_handle.Instance = TIM7;
    g_tim7_handle.Init.Prescaler = 84 - 1;                                /* 预分频: 84MHz / 84 = 1MHz → 1us */
    g_tim7_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                  /* 向上计数模式 */
    g_tim7_handle.Init.Period = 0xFFFF;                                   /* 自动重装载值（最大） */
    g_tim7_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            /* 时钟分频 */
    g_tim7_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* 使能自动重装载预装载 */

    HAL_TIM_Base_Init(&g_tim7_handle);

    g_fac_us = 1; /* 1MHz → 1us 因子 = 1 */

    /* 计算 ms 因子：每 ms 需要多少个 us 计数 */
    g_fac_ms = 1000;
}

/**
 * @brief       微秒级延时
 * @param       nus: 延时的微秒数（最大约 65535us）
 * @retval      无
 */
void delay_us(uint32_t nus) {
    uint32_t reload_val; /* 实际的自动重装载值 */

    if (nus == 0) return;

    /* 设置自动重装载值为 nus（1MHz 计数，1 个计数 = 1us） */
    reload_val = (nus <= 0xFFFF) ? nus : 0xFFFF;
    __HAL_TIM_SET_AUTORELOAD(&g_tim7_handle, reload_val - 1); /* 设置 ARR，注意减 1 因为计数从 0 开始 */
    __HAL_TIM_SET_COUNTER(&g_tim7_handle, 0);                 /* 计数器清零 */
    __HAL_TIM_CLEAR_FLAG(&g_tim7_handle, TIM_FLAG_UPDATE);    /* 清除溢出标志 */
    HAL_TIM_Base_Start(&g_tim7_handle);                       /* 启动定时器 */

    /* 等待计数达到自动重装载值（溢出） */
    while (__HAL_TIM_GET_FLAG(&g_tim7_handle, TIM_FLAG_UPDATE) == RESET);

    HAL_TIM_Base_Stop(&g_tim7_handle); /* 停止定时器 */
}

/**
 * @brief       毫秒级延时
 * @param       nms: 延时的毫秒数
 * @retval      无
 */
void delay_ms(uint32_t nms) {
    uint32_t repeat; /* 需要循环的 ms 次数 */

    if (nms == 0) return;

    /* 设置自动重装载值为 1000-1（即 1000us = 1ms 溢出一次） */
    __HAL_TIM_SET_AUTORELOAD(&g_tim7_handle, 1000 - 1);

    repeat = nms;

    /* 循环 nms 次，每次等待 1ms */
    for (uint32_t i = 0; i < repeat; i++) {
        __HAL_TIM_SET_COUNTER(&g_tim7_handle, 0);              /* 计数器清零 */
        __HAL_TIM_CLEAR_FLAG(&g_tim7_handle, TIM_FLAG_UPDATE); /* 清除溢出标志 */
        HAL_TIM_Base_Start(&g_tim7_handle);                    /* 启动定时器 */

        /* 等待 1000us（即从 0 计数到 1000） */
        while (__HAL_TIM_GET_FLAG(&g_tim7_handle, TIM_FLAG_UPDATE) == RESET);

        HAL_TIM_Base_Stop(&g_tim7_handle);
    }
}

/**
 * @brief       HAL 定时器基础 MSP 回调（HAL_TIM_Base_Init 自动调用）
 * @param       htim: TIM 句柄指针
 * @retval      无
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM7) {
        /* TIM7 时钟已由 delay_init() 提前使能 */
        /* 无需配置 GPIO，纯定时功能 */
    }
}
