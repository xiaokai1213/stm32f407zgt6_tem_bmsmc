#include "relay.h"

void relay_init(void) {
    GPIO_InitTypeDef gpio_init_struct; /* GPIO 初始化结构体 */

    RELAY_BMS_FAULT_CON_GPIO_CLK_ENABLE();
    RELAY_DI0_CON_GPIO_CLK_ENABLE();
    RELAY_DI1_CON_GPIO_CLK_ENABLE();
    RELAY_PRE_H_CON_GPIO_CLK_ENABLE();
    RELAY_AIR_H_CON_GPIO_CLK_ENABLE();
    RELAY_AIR_L_CON_GPIO_CLK_ENABLE();

    /* 通用GPIO配置：推挽输出、上拉、低速 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;  /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;          /* 上拉（默认高电平，继电器释放） */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW; /* 低速 */

    /* RELAY_BMS_FAULT_CON -> PD3 */
    gpio_init_struct.Pin = RELAY_BMS_FAULT_CON_GPIO_PIN;
    HAL_GPIO_Init(RELAY_BMS_FAULT_CON_GPIO_PORT, &gpio_init_struct);
    RELAY_BMS_FAULT_CON_OFF();

    /* RELAY_DI0_CON -> PD4 */
    gpio_init_struct.Pin = RELAY_DI0_CON_GPIO_PIN;
    HAL_GPIO_Init(RELAY_DI0_CON_GPIO_PORT, &gpio_init_struct);
    RELAY_DI0_CON_OFF();

    /* RELAY_DI1_CON -> PD5 */
    gpio_init_struct.Pin = RELAY_DI1_CON_GPIO_PIN;
    HAL_GPIO_Init(RELAY_DI1_CON_GPIO_PORT, &gpio_init_struct);
    RELAY_DI1_CON_OFF();

    /* RELAY_PRE_H_CON -> PD8 */
    gpio_init_struct.Pin = RELAY_PRE_H_CON_GPIO_PIN;
    HAL_GPIO_Init(RELAY_PRE_H_CON_GPIO_PORT, &gpio_init_struct);
    RELAY_PRE_H_CON_OFF();

    /* RELAY_AIR_H_CON -> PD9 */
    gpio_init_struct.Pin = RELAY_AIR_H_CON_GPIO_PIN;
    HAL_GPIO_Init(RELAY_AIR_H_CON_GPIO_PORT, &gpio_init_struct);
    RELAY_AIR_H_CON_OFF();

    /* RELAY_AIR_L_CON -> PD10 */
    gpio_init_struct.Pin = RELAY_AIR_L_CON_GPIO_PIN;
    HAL_GPIO_Init(RELAY_AIR_L_CON_GPIO_PORT, &gpio_init_struct);
    RELAY_AIR_L_CON_OFF();
}
