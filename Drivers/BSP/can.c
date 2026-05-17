#include "can.h"

CAN_HandleTypeDef g_can1_handle; /* CAN1句柄 */
CAN_HandleTypeDef g_can2_handle; /* CAN2句柄 */

/**
 * @brief       CAN1初始化函数
 *   @note      波特率: 500Kbps
 *               时钟: APB1 = 42MHz (168MHz / 4)
 *               计算公式: BaudRate = APB1_Clock / Prescaler / (TimeSeg1 + TimeSeg2 + 1)
 *               42MHz / 7 / (12 + 5 + 1) = 42MHz / 7 / 18 = 333Kbps  (示例)
 *               42MHz / 7 / (11 + 4 + 1) = 42MHz / 7 / 16 = 375Kbps
 *               42MHz / 6 / (12 + 5 + 1) = 42MHz / 6 / 18 ≈ 389Kbps
 *               42MHz / 6 / (11 + 4 + 1) = 42MHz / 6 / 16 ≈ 438Kbps
 *               42MHz / 7 / (10 + 4 + 1) = 42MHz / 7 / 15 = 400Kbps
 *               42MHz / 6 / (10 + 4 + 1) = 42MHz / 6 / 15 ≈ 467Kbps
 *               42MHz / 6 / (9 + 4 + 1) = 42MHz / 6 / 14 = 500Kbps  ✓
 * @param       无
 * @retval      无
 */
void can1_init(void) {
    CAN_FilterTypeDef can_filter_config;
    uint32_t prescaler = 6;     /* 预分频系数 */
    uint32_t ts1 = CAN_BS1_9TQ; /* 时间段1: 9个时间单元 */
    uint32_t ts2 = CAN_BS2_4TQ; /* 时间段2: 4个时间单元 */
    uint32_t sjw = CAN_SJW_1TQ; /* 同步跳转宽度: 1个时间单元 */

    /* 配置CAN1句柄基本参数 */
    g_can1_handle.Instance = CAN1_UX;
    g_can1_handle.Init.Prescaler = prescaler;
    g_can1_handle.Init.Mode = CAN_MODE_NORMAL;
    g_can1_handle.Init.SyncJumpWidth = sjw;
    g_can1_handle.Init.TimeSeg1 = ts1;
    g_can1_handle.Init.TimeSeg2 = ts2;
    g_can1_handle.Init.TimeTriggeredMode = DISABLE;
    g_can1_handle.Init.AutoBusOff = ENABLE;
    g_can1_handle.Init.AutoWakeUp = DISABLE;
    g_can1_handle.Init.AutoRetransmission = ENABLE;
    g_can1_handle.Init.ReceiveFifoLocked = DISABLE;
    g_can1_handle.Init.TransmitFifoPriority = DISABLE;

    HAL_CAN_Init(&g_can1_handle);

    /* 配置CAN1过滤器 */
    can_filter_config.FilterIdHigh = 0x0000;
    can_filter_config.FilterIdLow = 0x0000;
    can_filter_config.FilterMaskIdHigh = 0x0000;
    can_filter_config.FilterMaskIdLow = 0x0000;
    can_filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    can_filter_config.FilterBank = 0;
    can_filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_config.FilterActivation = ENABLE;
    can_filter_config.SlaveStartFilterBank = 14; /* CAN1使用0~13，CAN2使用14~27 */

    HAL_CAN_ConfigFilter(&g_can1_handle, &can_filter_config);

    /* 启动CAN1 */
    HAL_CAN_Start(&g_can1_handle);
}

/**
 * @brief       CAN2初始化函数
 *   @note      波特率: 500Kbps
 *               时钟: APB1 = 42MHz
 *               计算公式: BaudRate = APB1_Clock / Prescaler / (TimeSeg1 + TimeSeg2 + 1)
 *               42MHz / 6 / (9 + 4 + 1) = 42MHz / 6 / 14 = 500Kbps  ✓
 * @param       无
 * @retval      无
 */
void can2_init(void) {
    CAN_FilterTypeDef can_filter_config;
    uint32_t prescaler = 6;     /* 预分频系数 */
    uint32_t ts1 = CAN_BS1_9TQ; /* 时间段1: 9个时间单元 */
    uint32_t ts2 = CAN_BS2_4TQ; /* 时间段2: 4个时间单元 */
    uint32_t sjw = CAN_SJW_1TQ; /* 同步跳转宽度: 1个时间单元 */

    /* 配置CAN2句柄基本参数 */
    g_can2_handle.Instance = CAN2_UX;
    g_can2_handle.Init.Prescaler = prescaler;
    g_can2_handle.Init.Mode = CAN_MODE_NORMAL;
    g_can2_handle.Init.SyncJumpWidth = sjw;
    g_can2_handle.Init.TimeSeg1 = ts1;
    g_can2_handle.Init.TimeSeg2 = ts2;
    g_can2_handle.Init.TimeTriggeredMode = DISABLE;
    g_can2_handle.Init.AutoBusOff = ENABLE;
    g_can2_handle.Init.AutoWakeUp = DISABLE;
    g_can2_handle.Init.AutoRetransmission = ENABLE;
    g_can2_handle.Init.ReceiveFifoLocked = DISABLE;
    g_can2_handle.Init.TransmitFifoPriority = DISABLE;

    HAL_CAN_Init(&g_can2_handle);

    /* 配置CAN2过滤器 */
    can_filter_config.FilterIdHigh = 0x0000;
    can_filter_config.FilterIdLow = 0x0000;
    can_filter_config.FilterMaskIdHigh = 0x0000;
    can_filter_config.FilterMaskIdLow = 0x0000;
    can_filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    can_filter_config.FilterBank = 14;
    can_filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_config.FilterActivation = ENABLE;
    can_filter_config.SlaveStartFilterBank = 14; /* CAN1使用0~13，CAN2使用14~27 */

    HAL_CAN_ConfigFilter(&g_can2_handle, &can_filter_config);

    /* 启动CAN2 */
    HAL_CAN_Start(&g_can2_handle);
}

/**
 * @brief       CAN MSP初始化函数（HAL_CAN_Init()会自动调用此函数）
 * @param       hcan: CAN句柄类型指针
 * @note        完成时钟使能，引脚配置
 * @retval      无
 */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan) {
    GPIO_InitTypeDef gpio_init_struct;

    if (hcan->Instance == CAN1_UX) /* 如果是CAN1 */
    {
        CAN1_UX_CLK_ENABLE();      /* CAN1 时钟使能 */
        CAN1_TX_GPIO_CLK_ENABLE(); /* 发送引脚时钟使能 */
        CAN1_RX_GPIO_CLK_ENABLE(); /* 接收引脚时钟使能 */

        /* CAN1 TX 引脚配置 */
        gpio_init_struct.Pin = CAN1_TX_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = CAN1_TX_GPIO_AF;
        HAL_GPIO_Init(CAN1_TX_GPIO_PORT, &gpio_init_struct);

        /* CAN1 RX 引脚配置 */
        gpio_init_struct.Pin = CAN1_RX_GPIO_PIN;
        gpio_init_struct.Alternate = CAN1_RX_GPIO_AF;
        HAL_GPIO_Init(CAN1_RX_GPIO_PORT, &gpio_init_struct);
    }

    if (hcan->Instance == CAN2_UX) /* 如果是CAN2 */
    {
        CAN2_UX_CLK_ENABLE();      /* CAN2 时钟使能 */
        CAN2_TX_GPIO_CLK_ENABLE(); /* 发送引脚时钟使能 */
        CAN2_RX_GPIO_CLK_ENABLE(); /* 接收引脚时钟使能 */

        /* CAN2 TX 引脚配置 */
        gpio_init_struct.Pin = CAN2_TX_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = CAN2_TX_GPIO_AF;
        HAL_GPIO_Init(CAN2_TX_GPIO_PORT, &gpio_init_struct);

        /* CAN2 RX 引脚配置 */
        gpio_init_struct.Pin = CAN2_RX_GPIO_PIN;
        gpio_init_struct.Alternate = CAN2_RX_GPIO_AF;
        HAL_GPIO_Init(CAN2_RX_GPIO_PORT, &gpio_init_struct);
    }
}
