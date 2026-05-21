#include "io_in.h"

/**
 * @brief       初始化IO输入引脚
 *   @note      IO_WAKE_UP_SIGNAL: PA0, 外部上拉电阻, 浮空输入模式
 *              IO_FAULT00_SIGNAL: PE1, 外部上拉电阻, 浮空输入模式
 * @param       无
 * @retval      无
 */
void io_in_init(void) {
    GPIO_InitTypeDef gpio_init_struct;

    /* 使能GPIO时钟 */
    IO_WAKE_UP_SIGNAL_GPIO_CLK_ENABLE();
    IO_FAULT00_SIGNAL_GPIO_CLK_ENABLE();

    /* 通用GPIO配置：输入模式、无上下拉（外部有上拉电阻）、低速 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;      /* 输入模式 */
    gpio_init_struct.Pull = GPIO_NOPULL;          /* 无内部上下拉（外部上拉） */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW; /* 低速 */

    /* IO_WAKE_UP_SIGNAL -> PA0 */
    gpio_init_struct.Pin = IO_WAKE_UP_SIGNAL_GPIO_PIN;
    HAL_GPIO_Init(IO_WAKE_UP_SIGNAL_GPIO_PORT, &gpio_init_struct);

    /* IO_FAULT00_SIGNAL -> PE1 */
    gpio_init_struct.Pin = IO_FAULT00_SIGNAL_GPIO_PIN;
    HAL_GPIO_Init(IO_FAULT00_SIGNAL_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       读取唤醒信号
 *   @note      直接读取GPIO寄存器，返回当前唤醒信号电平状态
 * @param       无
 * @retval      1: 唤醒信号有效, 0: 唤醒信号无效
 */
uint8_t io_read_wake_up(void) { return (HAL_GPIO_ReadPin(IO_WAKE_UP_SIGNAL_GPIO_PORT, IO_WAKE_UP_SIGNAL_GPIO_PIN) == GPIO_PIN_SET) ? 1 : 0; }

/**
 * @brief       读取故障信号
 *   @note      直接读取GPIO寄存器，返回当前故障信号电平状态
 * @param       无
 * @retval      1: 故障信号有效, 0: 故障信号无效
 */
uint8_t io_read_fault00(void) { return (HAL_GPIO_ReadPin(IO_FAULT00_SIGNAL_GPIO_PORT, IO_FAULT00_SIGNAL_GPIO_PIN) == GPIO_PIN_SET) ? 1 : 0; }
