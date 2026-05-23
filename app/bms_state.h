#ifndef _BMS_STATE_H
#define _BMS_STATE_H

#include "event_groups.h"
#include "main.h"

/*******************************************************************************************************/
/* BMS状态管理定义 */

typedef enum {
    BMS_STATE_IDLE = 0,  /* 空闲状态 */
    BMS_STATE_PRECHARGE, /* 预充电状态 */
    BMS_STATE_RUN,       /* 放电运行状态 */
    BMS_STATE_CHARGE,    /* 充电状态 */
    BMS_STATE_FAULT      /* 故障状态 */
} BMS_State_t;

/*******************************************************************************************************/
/* 事件组位定义 - 用于任务间通信 */

#define BMS_EVENT_CHARGE_REQ    (1 << 0) /* 充电请求信号 (bit0) */
#define BMS_EVENT_DISCHARGE_REQ (1 << 1) /* 放电请求信号 (bit1) */
#define BMS_EVENT_FAULT         (1 << 2) /* 故障信号 (bit2) */
#define BMS_EVENT_CLEAR_FAULT   (1 << 3) /* 故障清除信号 (bit3) */
/* bit4 预留 */

/*******************************************************************************************************/
/* 预充电超时时间定义 */

#define BMS_PRECHARGE_TIMEOUT_MS 3000 /* 预充电超时时间 3s */
#define BMS_PRECHARGE_HOLD_MS    500  /* 预充继电器保持时间 500ms */

/* 预充完成阈值：预充电压 ≥ 90% × 电池总电压 即认为预充完成 */
#define BMS_PRECHARGE_THRESHOLD_NUM 9  /* 分子：90% = 9/10 */
#define BMS_PRECHARGE_THRESHOLD_DEN 10 /* 分母 */

/*******************************************************************************************************/
/* CCS充电控制参数定义 */

#define BMS_CHARGE_MAX_V          3201 /* 最高充电端电压 320.1V (0.1V/bit) */
#define BMS_CHARGE_MAX_C          582  /* 最高充电端电流 58.2A (0.1A/bit)  */
#define BMS_CHARGE_CTRL_PERIOD_MS 1000 /* CCS控制报文发送周期 1000ms (1s) */

/*******************************************************************************************************/
/* 外部变量声明 */

extern EventGroupHandle_t g_bms_event_group; /* BMS事件组句柄 */

/*******************************************************************************************************/
/* 对外API - 供其他任务调用 */

void BMS_SetChargeReq(void);      /* 设置充电请求 */
void BMS_ClearChargeReq(void);    /* 清除充电请求 */
void BMS_SetDischargeReq(void);   /* 设置放电请求 */
void BMS_ClearDischargeReq(void); /* 清除放电请求 */
void BMS_SetFault(void);          /* 设置故障信号 */
void BMS_ClearFault(void);        /* 清除故障信号 */

/*******************************************************************************************************/
/* BMS状态机内部接口 */

void Task_BMS_Main(void *parameters); /* BMS状态管理主任务函数 */
void bms_state_init(void);            /* BMS状态管理初始化函数 */
void bms_state_task(void);            /* BMS状态机主循环函数 */
void bms_state_transition(void);      /* 状态切换函数，调用对应状态的entry函数 */

/*******************************************************************************************************/

#endif /* _BMS_STATE_H */

/*

EventBits_t xEventGroupWaitBits(
    const EventGroupHandle_t xEventGroup,   // 1. 事件组句柄
    const EventBits_t uxBitsToWaitFor,      // 2. 要等待的位掩码
    const BaseType_t xClearOnExit,          // 3. 退出时是否清除位
    const BaseType_t xWaitForAllBits,       // 4. 等待逻辑（全部或任意）
    TickType_t xTicksToWait                 // 5. 最大等待时间（超时）
);

*/
