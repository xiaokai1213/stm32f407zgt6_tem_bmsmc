#include "d_in.h"

/**
 * @brief       初始化数字量输入引脚
 *   @note      DI_WAKE_UP_SIGNAL: PA0, 外部上拉电阻, 浮空输入模式
 * @param       无
 * @retval      无
 */
void d_in_init(void) {
    GPIO_InitTypeDef gpio_init_struct;

    DI_WAKE_UP_SIGNAL_GPIO_CLK_ENABLE();

    /* 通用GPIO配置：输入模式、无上下拉（外部有上拉电阻）、低速 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;      /* 输入模式 */
    gpio_init_struct.Pull = GPIO_NOPULL;          /* 无内部上下拉（外部上拉） */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW; /* 低速 */

    /* DI_WAKE_UP_SIGNAL -> PA0 */
    gpio_init_struct.Pin = DI_WAKE_UP_SIGNAL_GPIO_PIN;
    HAL_GPIO_Init(DI_WAKE_UP_SIGNAL_GPIO_PORT, &gpio_init_struct);
}
