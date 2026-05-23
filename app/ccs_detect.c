#include "ccs_detect.h"
#include "task.h"
#include "bms_state.h"
#include "task_can_tx.h"

/**
 * @brief       CCS事件处理函数
 *   @note      解析CCS充电机报文，检测各状态位，
 *              若所有状态位均正常则发送充电事件到BMS状态机，
 *              若存在异常则清除充电请求不进入充电流程。
 * @param       data: CCS报文的8字节数据指针
 * @retval      无
 */
void CCS_ProcessEvent(uint8_t *data) {
    uint16_t out_v; /* 输出电压 (0.1V/bit) */
    uint16_t out_c; /* 输出电流 (0.1A/bit) */

    /* 解析数据 */
    /* CCS_OUT_V: 输出电压 data[0]~data[1], 16bit, 0.1V/bit, Motorola MSB */
    out_v = (uint16_t)((uint16_t)data[0] << 8) | data[1];
    /* CCS_OUT_C: 输出电流 data[2]~data[3], 16bit, 0.1A/bit, Motorola MSB */
    out_c = (uint16_t)((uint16_t)data[2] << 8) | data[3];

    /* 检测异常状态位 (Byte4 bit4~bit0, LSB优先) */
    uint8_t is_fault = (data[4] >> 0) & 0x01;     /* CCS_FAULT: 硬件故障 */
    uint8_t is_temp_prot = (data[4] >> 1) & 0x01; /* CCS_T: 温度过高保护 */
    uint8_t is_in_v_err = (data[4] >> 2) & 0x01;  /* CCS_IN_V: 输入电压错误 */
    uint8_t is_shutdown = (data[4] >> 3) & 0x01;  /* CCS_STATE: 0启动/1关闭 */
    uint8_t is_timeout = (data[4] >> 4) & 0x01;   /* COM_STATE: 0正常/1超时 */

    /* 根据检测结果决定是否发送充电事件 */
    if (!is_fault && !is_temp_prot && !is_in_v_err && !is_timeout) {
        /* 所有状态位正常 → 发送充电请求 */
        BMS_SetChargeReq();
        printf("[CCS]充电机状态正常,发送充电请求(V=%u.%uV, C=%u.%uA)\n", out_v / 10, out_v % 10, out_c / 10, out_c % 10);
    } else {
        /* 存在异常 → 清除充电请求 */
        BMS_ClearChargeReq();
        printf("[CCS] 充电机状态异常:");

        if (is_fault) { printf("硬件故障 "); }
        if (is_temp_prot) { printf("温度过高 "); }
        if (is_in_v_err) { printf("输入电压错误 "); }
        if (is_shutdown) { printf("充电机关闭或电池反接 "); }
        if (is_timeout) { printf("通信超时 "); }
        printf("\n");
    }
}

/**
 * @brief       发送CCS充电控制报文到充电机
 *   @note      通过CAN发送队列投递到Task_CAN_Tx任务处理。
 * @param       max_charg_v: 最高充电端电压 (0.1V/bit)，例：3201 = 320.1V
 * @param       max_charg_c: 最高充电端电流 (0.1A/bit)，例：582 = 58.2A
 * @param       control: 控制状态 (0:开启充电, 1:关闭输出)
 * @retval      pdPASS=成功, pdFAIL=队列满
 */
BaseType_t CCS_SendControlMsg(uint16_t max_charg_v, uint16_t max_charg_c, uint8_t control) {
    CAN_TxMsg_t tx_msg = {0};

    /* 组装CAN帧头 */
    tx_msg.can_id = CCS_CTRL_ID; /* 扩展帧ID: 0x1806E5F4 */
    tx_msg.dlc = 8;              /* 8字节数据 */
    tx_msg.ide = 1;              /* 扩展帧 */
    tx_msg.rtr = 0;              /* 数据帧 */

    /* 组装数据 (Motorola MSB格式) */
    /* Byte[0~1]: MAX_CHARG_V - 最高充电端电压, 16bit, 0.1V/bit */
    tx_msg.data[0] = (uint8_t)(max_charg_v >> 8);   /* 高字节在前 */
    tx_msg.data[1] = (uint8_t)(max_charg_v & 0xFF); /* 低字节在后 */

    /* Byte[2~3]: MAX_CHARG_C - 最高充电端电流, 16bit, 0.1A/bit */
    tx_msg.data[2] = (uint8_t)(max_charg_c >> 8);   /* 高字节在前 */
    tx_msg.data[3] = (uint8_t)(max_charg_c & 0xFF); /* 低字节在后 */

    /* Byte[4]: CCS_CON - 控制状态
                 0x00: 充电机开启充电
                 0x01: 电池保护，充电器关闭输出 */
    tx_msg.data[4] = control;

    /* Byte[5~7]: 预留填充0 */
    tx_msg.data[5] = 0;
    tx_msg.data[6] = 0;
    tx_msg.data[7] = 0;

    /* 投递到CAN发送队列 (非阻塞，队列满立即返回) */
    if (g_can_tx_queue == NULL) { return pdFAIL; }
    return xQueueSend(g_can_tx_queue, &tx_msg, 0);
}
