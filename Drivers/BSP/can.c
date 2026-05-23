#include "can.h"
#include "task.h"
#include "task_can_rx.h"

CAN_HandleTypeDef g_can1_handle; /* CAN1句柄 */
CAN_HandleTypeDef g_can2_handle; /* CAN2句柄 */

/**
 * @brief       CAN1初始化函数
 *   @note      波特率: 500Kbps
 *              时钟: APB1 = 42MHz (168MHz / 4)
 *              计算公式: BaudRate = APB1_Clock / Prescaler / (TimeSeg1 + TimeSeg2 + 1)
 *              42MHz / 6 / (9 + 4 + 1) = 42MHz / 6 / 14 = 500Kbps  ✓
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

    /* 启动CAN1接收中断 - FIFO0消息挂起 */
    HAL_CAN_ActivateNotification(&g_can1_handle, CAN_IT_RX_FIFO0_MSG_PENDING);

    /* 启动CAN1 */
    HAL_CAN_Start(&g_can1_handle);
}

/**
 * @brief       CAN2初始化函数
 *   @note      波特率: 500Kbps
 *              时钟: APB1 = 42MHz
 *              计算公式: BaudRate = APB1_Clock / Prescaler / (TimeSeg1 + TimeSeg2 + 1)
 *              42MHz / 6 / (9 + 4 + 1) = 42MHz / 6 / 14 = 500Kbps  ✓
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

    /* 启动CAN2接收中断 - FIFO0消息挂起 */
    HAL_CAN_ActivateNotification(&g_can2_handle, CAN_IT_RX_FIFO0_MSG_PENDING);

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

/**
 * @brief       CAN接收FIFO0消息挂起回调函数
 *   @note      获取CAN消息通过队列发送给 Task_CAN_Rx 任务，并唤醒该任务。
 *              退出时无条件做一次上下文切换。
 * @param       hcan: CAN句柄指针
 * @retval      无
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header; /* HAL库接收消息结构体 */
    CAN_RxMsg_t rx_msg = {0};      /* CAN接收任务队列消息结构体 */

    /* 从硬件FIFO接收消息 */
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_msg.data) == HAL_OK) {
        /* 组装消息头 */
        rx_msg.can_id = (rx_header.IDE == CAN_ID_EXT) ? rx_header.ExtId : rx_header.StdId;
        rx_msg.dlc = rx_header.DLC;
        rx_msg.ide = (rx_header.IDE == CAN_ID_EXT) ? 1 : 0;
        rx_msg.rtr = (rx_header.RTR == CAN_RTR_REMOTE) ? 1 : 0;

        /* 通过队列发送给CAN接收任务 */
        xQueueSendFromISR(g_can_rx_queue, &rx_msg, NULL);
    }

    /* 唤醒CAN接收任务 */
    xTaskNotifyGiveFromISR(g_can_rx_task_handle, NULL);

    /* 退出中断回调时做一次上下文切换 */
    portYIELD_FROM_ISR(pdTRUE);
    /* 上下文切换的作用是让系统退出中断时依据任务优先级执行一次任务切换
       当中断打断的任务优先级低于被唤醒的任务时，上下文切换在退出中断时
       会直接切换到被唤醒的任务，如果不是则继续执行被打断的任务 */
}

/**
 * @brief       CAN1发送函数（非阻塞）
 *   @note      向CAN1发送消息。若3个硬件发送邮箱均被占用则立即返回HAL_BUSY，不等待。
 * @param       p_msg: 指向待发送的CAN_TxMsg_t结构体
 * @retval      HAL_OK=成功, HAL_BUSY=邮箱满, HAL_ERROR=参数错误
 */
HAL_StatusTypeDef can1_send(CAN_TxMsg_t *p_msg) {
    CAN_TxHeaderTypeDef tx_header;
    uint32_t tx_mailbox;

    if ((p_msg == NULL) || (p_msg->dlc > 8U)) { return HAL_ERROR; }

    if (HAL_CAN_GetTxMailboxesFreeLevel(&g_can1_handle) == 0) { return HAL_BUSY; }

    tx_header.StdId = (p_msg->ide == 0) ? p_msg->can_id : 0;
    tx_header.ExtId = (p_msg->ide != 0) ? p_msg->can_id : 0;
    tx_header.IDE = (p_msg->ide != 0) ? CAN_ID_EXT : CAN_ID_STD;
    tx_header.RTR = (p_msg->rtr != 0) ? CAN_RTR_REMOTE : CAN_RTR_DATA;
    tx_header.DLC = p_msg->dlc;
    tx_header.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(&g_can1_handle, &tx_header, (uint8_t *)p_msg->data, &tx_mailbox);
}

/**
 * @brief       CAN2发送函数（非阻塞）
 *   @note      向CAN2发送消息。若3个硬件发送邮箱均被占用则立即返回HAL_BUSY，不等待。
 * @param       p_msg: 指向待发送的CAN_TxMsg_t结构体
 * @retval      HAL_OK=成功, HAL_BUSY=邮箱满, HAL_ERROR=参数错误
 */
HAL_StatusTypeDef can2_send(CAN_TxMsg_t *p_msg) {
    CAN_TxHeaderTypeDef tx_header;
    uint32_t tx_mailbox;

    if ((p_msg == NULL) || (p_msg->dlc > 8U)) { return HAL_ERROR; }

    if (HAL_CAN_GetTxMailboxesFreeLevel(&g_can2_handle) == 0) { return HAL_BUSY; }

    tx_header.StdId = (p_msg->ide == 0) ? p_msg->can_id : 0;
    tx_header.ExtId = (p_msg->ide != 0) ? p_msg->can_id : 0;
    tx_header.IDE = (p_msg->ide != 0) ? CAN_ID_EXT : CAN_ID_STD;
    tx_header.RTR = (p_msg->rtr != 0) ? CAN_RTR_REMOTE : CAN_RTR_DATA;
    tx_header.DLC = p_msg->dlc;
    tx_header.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(&g_can2_handle, &tx_header, (uint8_t *)p_msg->data, &tx_mailbox);
}
