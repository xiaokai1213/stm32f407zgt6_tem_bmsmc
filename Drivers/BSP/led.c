#include "led.h"

/**
 * @brief       初始化 LED 引脚（PB0 绿灯, PB1 红灯）
 * @param       无
 * @retval      无
 */
void led_init(void) {
    GPIO_InitTypeDef gpio_init_struct;

    LED_G_GPIO_CLK_ENABLE();
    LED_R_GPIO_CLK_ENABLE();

    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;  /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;          /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW; /* 低速 */

    /* 绿灯 PB0 */
    gpio_init_struct.Pin = LED_G_GPIO_PIN;
    HAL_GPIO_Init(LED_G_GPIO_PORT, &gpio_init_struct);

    /* 红灯 PB1 */
    gpio_init_struct.Pin = LED_R_GPIO_PIN;
    HAL_GPIO_Init(LED_R_GPIO_PORT, &gpio_init_struct);

    LED_G_OFF(); /* 初始关闭绿灯 */
    LED_R_OFF(); /* 初始关闭红灯 */
}
