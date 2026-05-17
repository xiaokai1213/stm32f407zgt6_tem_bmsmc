#include "relay.h"

void relay_init(void) {
    GPIO_InitTypeDef gpio_init_struct; /* GPIO 初始化结构体 */

    RELAY_BMS_FAULT_CON_GPIO_CLK_ENABLE(); /* 使能继电器引脚时钟 */

    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;         /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                 /* 上拉（默认高电平，继电器释放） */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;        /* 低速 */
    gpio_init_struct.Pin = RELAY_BMS_FAULT_CON_GPIO_PIN; /* 继电器引脚 */

    HAL_GPIO_Init(RELAY_BMS_FAULT_CON_GPIO_PORT, &gpio_init_struct);

    RELAY_BMS_FAULT_CON_OFF(); /* 初始关闭继电器（高电平，正常状态） */
}
