#include "bms_state.h"
#include "event_groups.h"
#include "queue.h"
#include "ccs_detect.h"
#include "led.h"
#include "relay.h"
#include "task_adc1_sample.h"

EventGroupHandle_t g_bms_event_group = NULL; /* 事件组句柄定义 */
static BMS_State_t current_bms_state;        /* 当前BMS状态 */
static TickType_t precharge_start_tick;      /* 预充电计时器 */

/**
 * @brief       BMS状态管理主任务
 *   @note      FreeRTOS任务入口
 * @param       parameters: 任务参数（未使用）
 * @retval      无
 */
void Task_BMS_Main(void *parameters) {
    bms_state_init(); /* 初始化状态管理 */

    while (1) { bms_state_task(); /* 执行状态机主循环 */ }
}

/**
 * @brief       BMS状态管理初始化函数
 *   @note      初始化事件组、Relay、LED，初始状态为IDLE
 * @param       无
 * @retval      无
 */
void bms_state_init(void) {

    /* 创建事件组 */
    g_bms_event_group = xEventGroupCreate(); /* 创建事件组用于任务间通信 */
    configASSERT(g_bms_event_group != NULL); /* 确保事件组创建成功 */

    /* 初始状态 */
    current_bms_state = BMS_STATE_IDLE; /* 初始状态为IDLE */
    bms_state_idle_entry();             /* 进入初始状态 */

    printf("[BMS] BMS状态管理初始化完成\n");
}

/***********************************状态切换函数**********************************************************/

/**
 * @brief       状态切换函数
 *   @note      在状态切换时调用对应的进入函数
 * @param       无
 * @retval      无
 */
static void bms_state_transition(void) {
    switch (current_bms_state) {
    case BMS_STATE_IDLE:
        bms_state_idle_entry(); /* 进入空闲状态 */
        break;
    case BMS_STATE_PRECHARGE:
        bms_state_precharge_entry(); /* 进入预充电状态 */
        break;
    case BMS_STATE_RUN:
        bms_state_run_entry(); /* 进入放电运行状态 */
        break;
    case BMS_STATE_CHARGE:
        bms_state_charge_entry(); /* 进入充电状态 */
        break;
    case BMS_STATE_FAULT:
        bms_state_fault_entry(); /* 进入故障状态 */
        break;
    default:
        break;
    }
}

/**********************************状态运行函数**********************************************************/

/**
 * @brief       BMS状态管理主任务函数
 *   @note      状态机主循环，每个状态调用对应的run函数处理逻辑
 * @param       无
 * @retval      无
 */
void bms_state_task(void) {
    BMS_State_t prev_state; /* 记录上一个状态，用于检测状态切换 */

    /* 记录当前状态 */
    prev_state = current_bms_state;

    switch (current_bms_state) {
    case BMS_STATE_IDLE:
        bms_state_idle_run();
        break;
    case BMS_STATE_PRECHARGE:
        bms_state_precharge_run();
        break;
    case BMS_STATE_RUN:
        bms_state_run_run();
        break;
    case BMS_STATE_CHARGE:
        bms_state_charge_run();
        break;
    case BMS_STATE_FAULT:
        bms_state_fault_run();
        break;
    default:
        break;
    }

    if (current_bms_state != prev_state) { /* 如果状态发生了切换，调用新状态的entry函数 */
        bms_state_transition();            /* 处理状态切换，调用对应的entry函数 */
    }
}

/*******************************外部信号接口************************************************************/

/**
 * @brief       设置充电请求
 *   @note      供外部任务调用，通过事件组通知BMS状态机
 * @param       无
 * @retval      无
 */
void BMS_SetChargeReq(void) {
    if (g_bms_event_group != NULL) { xEventGroupSetBits(g_bms_event_group, BMS_EVENT_CHARGE_REQ); }
}

/**
 * @brief       清除充电请求
 *   @note      供外部任务调用
 * @param       无
 * @retval      无
 */
void BMS_ClearChargeReq(void) {
    if (g_bms_event_group != NULL) { xEventGroupClearBits(g_bms_event_group, BMS_EVENT_CHARGE_REQ); }
}

/**
 * @brief       设置放电请求
 *   @note      供外部任务调用，通过事件组通知BMS状态机
 * @param       无
 * @retval      无
 */
void BMS_SetDischargeReq(void) {
    if (g_bms_event_group != NULL) { xEventGroupSetBits(g_bms_event_group, BMS_EVENT_DISCHARGE_REQ); }
}

/**
 * @brief       清除放电请求
 *   @note      供外部任务调用
 * @param       无
 * @retval      无
 */
void BMS_ClearDischargeReq(void) {
    if (g_bms_event_group != NULL) { xEventGroupClearBits(g_bms_event_group, BMS_EVENT_DISCHARGE_REQ); }
}

/**
 * @brief       设置故障信号
 *   @note      供外部任务调用，通过事件组通知BMS状态机
 * @param       无
 * @retval      无
 */
void BMS_SetFault(void) {
    if (g_bms_event_group != NULL) { xEventGroupSetBits(g_bms_event_group, BMS_EVENT_FAULT); }
}

/**
 * @brief       清除故障信号
 *   @note      供外部任务调用，用于故障恢复
 * @param       无
 * @retval      无
 */
void BMS_ClearFault(void) {
    if (g_bms_event_group != NULL) { xEventGroupSetBits(g_bms_event_group, BMS_EVENT_CLEAR_FAULT); }
}

/*******************************************************************************************************/

/**
 * @brief       进入空闲状态
 *   @note      所有继电器释放，等待充电/放电请求
 * @param       无
 * @retval      无
 */
void bms_state_idle_entry(void) {
    /* 释放所有继电器 */
    RELAY_PRE_H_CON_OFF();
    RELAY_AIR_H_CON_OFF();
    RELAY_AIR_L_CON_OFF();
    RELAY_BMS_FAULT_CON_OFF();

    /*绿色指示灯显示正常，红色指示灯关闭*/
    LED_R_OFF(); /* 无故障 */
    LED_G_ON();  /* 无高压输出，安全状态 */

    printf("[BMS] 进入空闲状态\n");
}

/**
 * @brief       空闲状态运行逻辑
 *   @note      等待充电/放电请求，收到后切换到预充电状态
 * @param       无
 * @retval      无
 */
void bms_state_idle_run(void) {
    EventBits_t bits;                                   /* 等待事件位 */
    const TickType_t xTicksToWait = pdMS_TO_TICKS(500); /* 等待500ms，保持空闲状态的响应性 */

    /* 等待事件：充电请求、放电请求、故障信号 */
    bits = xEventGroupWaitBits(g_bms_event_group,                                                /*事件组句柄*/
                               BMS_EVENT_CHARGE_REQ | BMS_EVENT_DISCHARGE_REQ | BMS_EVENT_FAULT, /* 等待充电请求、放电请求或故障信号 */
                               pdFALSE,                                                          /* 不自动清除，手动管理 */
                               pdFALSE,                                                          /* 任意一个位即可唤醒 */
                               xTicksToWait);                                                    /* 等待事件，保持空闲状态的响应性 */

    /* 发送关闭充电控制报文，确保充电机连接时通信正常，防止充电机连接后报通信超市错误，使状态机进入不了充电状态 */
    CCS_SendControlMsg(BMS_CHARGE_MAX_V, BMS_CHARGE_MAX_C, 1);

    if (bits & BMS_EVENT_FAULT) {
        /* 收到故障信号，进入故障状态 */
        current_bms_state = BMS_STATE_FAULT;
        printf("[BMS] 收到故障信号，进入故障状态\n");
    } else if ((bits & BMS_EVENT_CHARGE_REQ) || (bits & BMS_EVENT_DISCHARGE_REQ)) {
        /* 收到充电或放电请求，进入预充电状态 */
        current_bms_state = BMS_STATE_PRECHARGE;
        printf("[BMS] 收到请求，进入预充电状态\n");
    }
}

/*******************************************************************************************************/

/**
 * @brief       进入预充电状态
 *   @note      闭合预充继电器和主负继电器，开始预充计时
 * @param       无
 * @retval      无
 */
void bms_state_precharge_entry(void) {
    /* 记录预充电起始时间，用于超时判断 */
    precharge_start_tick = xTaskGetTickCount();

    RELAY_AIR_L_CON_ON(); /* 闭合主负继电器 */
    RELAY_PRE_H_CON_ON(); /* 闭合预充继电器 */

    LED_R_OFF(); /* 无故障 */
    LED_G_OFF(); /* 有高压输出，非安全状态 */

    printf("[BMS] 预充电开始\n");
}

/**
 * @brief       预充电状态运行逻辑
 *   @note      等待预充完成信号或超时
 * @param       无
 * @retval      无
 */
void bms_state_precharge_run(void) {
    TickType_t elapsed;             /* 预充电已运行时间 */
    ADC_SampleData_t latest_sample; /* 最新ADC采样数据 */

    /* 非阻塞查询事件组，检查是否有故障信号 */
    if (xEventGroupGetBits(g_bms_event_group) & BMS_EVENT_FAULT) {
        xEventGroupClearBits(g_bms_event_group, BMS_EVENT_FAULT);
        RELAY_PRE_H_CON_OFF();
        RELAY_AIR_L_CON_OFF();
        current_bms_state = BMS_STATE_FAULT;
        printf("[BMS] 预充电故障\n");
        return;
    }

    /* 从ADC队列读取最新数据，判断预充电压是否≥90%×电池总电压 */
    if (xQueuePeek(g_adc_sample_queue, &latest_sample, 0) == pdTRUE) {
        if (latest_sample.precharge_voltage * BMS_PRECHARGE_THRESHOLD_DEN >= latest_sample.bat_voltage * BMS_PRECHARGE_THRESHOLD_NUM) {
            /* 预充完成，根据事件组中的请求位决定进入充电还是放电运行 */
            EventBits_t req_bits = xEventGroupGetBits(g_bms_event_group);

            /* 预充继电器保持一段时间，确保预充完成后再切换状态 */
            vTaskDelay(pdMS_TO_TICKS(BMS_PRECHARGE_HOLD_MS));

            RELAY_PRE_H_CON_OFF(); /* 断开预充继电器 */
            RELAY_AIR_H_CON_ON();  /* 闭合主正继电器 */

            if (req_bits & BMS_EVENT_CHARGE_REQ) {
                current_bms_state = BMS_STATE_CHARGE;
                printf("[BMS] 预充完成，进入充电状态\n");
            } else {
                current_bms_state = BMS_STATE_RUN;
                printf("[BMS] 预充完成，进入放电运行状态\n");
            }
            return;
        }
    }

    /* 检查预充是否超时 */
    elapsed = (xTaskGetTickCount() - precharge_start_tick) * portTICK_PERIOD_MS;
    if (elapsed >= BMS_PRECHARGE_TIMEOUT_MS) {
        RELAY_PRE_H_CON_OFF();
        RELAY_AIR_L_CON_OFF();
        current_bms_state = BMS_STATE_FAULT;
        printf("[BMS]预充超时(%lu ms)\n", (unsigned long)elapsed);
    }

    /* 延时10ms后继续检查，保持预充状态的响应性 */
    vTaskDelay(pdMS_TO_TICKS(10));
}

/*******************************************************************************************************/

/**
 * @brief       进入运行（放电）状态
 * @param       无
 * @retval      无
 */
void bms_state_run_entry(void) {
    LED_R_OFF(); /* 无故障 */
    LED_G_OFF(); /* 有高压输出，非安全状态 */
    printf("[BMS] 进入放电运行状态\n");
}

/**
 * @brief       运行状态逻辑
 *   @note      监测故障信号和请求撤销信号
 * @param       无
 * @retval      无
 */
void bms_state_run_run(void) {
    EventBits_t bits;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(500);

    /* 等待故障信号，或放电请求被清除 */
    bits = xEventGroupWaitBits(g_bms_event_group, /*事件组句柄*/
                               BMS_EVENT_FAULT,   /* 等待故障信号 */
                               pdFALSE,           /* 不自动清除，手动管理 */
                               pdTRUE,            /* 任意一个位即可唤醒 */
                               xTicksToWait);     /* 等待事件，保持运行状态的响应性 */

    if (bits & BMS_EVENT_FAULT) {
        /* 发生故障 */
        current_bms_state = BMS_STATE_FAULT;
        printf("[BMS] 运行故障\n");
        return;
    }

    /* 检查放电请求是否已被外部任务清除 */
    EventBits_t req_bits = xEventGroupGetBits(g_bms_event_group);
    if (!(req_bits & BMS_EVENT_DISCHARGE_REQ)) {
        /* 放电请求已撤销，回到空闲 */
        RELAY_AIR_H_CON_OFF();
        RELAY_AIR_L_CON_OFF();
        current_bms_state = BMS_STATE_IDLE;
        printf("[BMS] 放电请求撤销，回到空闲\n");
    }
}

/*******************************************************************************************************/

/**
 * @brief       进入充电状态
 *   @note      向充电机发送开启充电控制报文，启动充电
 * @param       无
 * @retval      无
 */
void bms_state_charge_entry(void) {
    LED_R_OFF(); /* 无故障 */
    LED_G_OFF(); /* 有高压输出，非安全状态 */

    /* 发送充电控制报文，开启充电机 (control=0: 开启充电) */
    CCS_SendControlMsg(BMS_CHARGE_MAX_V, BMS_CHARGE_MAX_C, 0);
    printf("[BMS] 进入充电状态\n");
}

/**
 * @brief       充电状态逻辑
 *   @note      周期性发送CCS控制报文维持充电，监测故障和请求撤销
 * @param       无
 * @retval      无
 */
void bms_state_charge_run(void) {
    EventBits_t bits;
    static TickType_t last_ctrl_tick = 0; /* 上次发送控制报文的时间戳 */
    const TickType_t xTicksToWait = pdMS_TO_TICKS(500);

    /* 等待故障信号 */
    bits = xEventGroupWaitBits(g_bms_event_group, /*事件组句柄*/
                               BMS_EVENT_FAULT,   /* 等待故障信号 */
                               pdFALSE,           /* 不自动清除，手动管理 */
                               pdTRUE,            /* 任意一个位即可唤醒 */
                               xTicksToWait);

    if (bits & BMS_EVENT_FAULT) {
        /* 发生故障 → 发送停止充电报文，然后切换状态 */
        CCS_SendControlMsg(BMS_CHARGE_MAX_V, BMS_CHARGE_MAX_C, 1);
        current_bms_state = BMS_STATE_FAULT;
        printf("[BMS] 充电故障\n");
        return;
    }

    /* 检查充电请求是否已被外部任务清除 */
    EventBits_t req_bits = xEventGroupGetBits(g_bms_event_group);
    if (!(req_bits & BMS_EVENT_CHARGE_REQ)) {
        /* 充电请求已撤销 → 发送停止充电报文，断开继电器 */
        CCS_SendControlMsg(BMS_CHARGE_MAX_V, BMS_CHARGE_MAX_C, 1);
        RELAY_AIR_H_CON_OFF();
        RELAY_AIR_L_CON_OFF();
        current_bms_state = BMS_STATE_IDLE;
        printf("[BMS] 充电请求撤销，回到空闲\n");
        return;
    }

    /* 周期性发送CCS控制报文维持充电状态 (1s间隔) */
    if ((xTaskGetTickCount() - last_ctrl_tick) >= pdMS_TO_TICKS(BMS_CHARGE_CTRL_PERIOD_MS)) {
        CCS_SendControlMsg(BMS_CHARGE_MAX_V, BMS_CHARGE_MAX_C, 0);
        last_ctrl_tick = xTaskGetTickCount();
    }
}

/*******************************************************************************************************/

/**
 * @brief       进入故障状态
 * @param       无
 * @retval      无
 */
void bms_state_fault_entry(void) {
    /* 断开所有继电器 */
    RELAY_PRE_H_CON_OFF();
    RELAY_AIR_H_CON_OFF();
    RELAY_AIR_L_CON_OFF();
    RELAY_BMS_FAULT_CON_ON(); /* 吸合故障继电器，指示故障状态 */

    LED_R_ON(); /* 有故障 */
    LED_G_ON(); /* 断开高压输出，安全状态 */

    printf("[BMS] 进入故障状态\n");
}

/**
 * @brief       故障状态逻辑
 *   @note      等待故障清除信号后回到空闲
 * @param       无
 * @retval      无
 */
void bms_state_fault_run(void) {
    EventBits_t bits;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

    bits = xEventGroupWaitBits(g_bms_event_group,     /*事件组句柄*/
                               BMS_EVENT_CLEAR_FAULT, /* 等待故障清除信号 */
                               pdTRUE,                /* 自动清除 */
                               pdTRUE,                /* 任意一个位即可唤醒 */
                               xTicksToWait);

    if (bits & BMS_EVENT_CLEAR_FAULT) {
        /* 清除故障，回到空闲状态 */
        RELAY_BMS_FAULT_CON_OFF();
        current_bms_state = BMS_STATE_IDLE;
        printf("[BMS] 故障清除，回到空闲\n");
    }

    /* 故障灯闪烁指示 */
    LED_R_TOGGLE();

    /* 断开所有继电器 */
    RELAY_PRE_H_CON_OFF();
    RELAY_AIR_H_CON_OFF();
    RELAY_AIR_L_CON_OFF();
    RELAY_BMS_FAULT_CON_ON(); /* 吸合故障继电器，指示故障状态 */
}
