#include "task_fault_detect.h"
#include "bms_state.h"
#include "io_in.h"
#include "task.h"

/*******************************************************************************************************/
/* 从控故障状态变量定义 */

volatile PACK1_FaultStatus_t g_pack1_fault = {0}; /* PACK1从控故障状态 */

/*******************************************************************************************************/
/**
 * @brief       VT故障报文解析 (ID: 0x1601FF21)
 *   @note      电芯过压/欠压 + 温度过温/低温
 *              Motorola MSB编码:
 *                data[0] bit0~bit7  = Cell1~Cell8  OV
 *                data[1] bit0~bit7  = Cell9~Cell16 OV
 *                data[2] bit0~bit3  = Cell17~Cell20 OV
 *                data[2] bit4~bit7  = Cell1~Cell4  UV
 *                data[3] bit0~bit7  = Cell5~Cell12 UV
 *                data[4] bit0~bit7  = Cell13~Cell20 UV
 *                data[5] bit0~bit7  = Temp1~Temp8  OT
 *                data[6] bit0~bit1  = Temp9~Temp10 OT
 *                data[6] bit2~bit7  = Temp1~Temp6  UT
 *                data[7] bit0~bit3  = Temp7~Temp10 UT
 * @param       data: 8字节CAN数据指针
 * @retval      无
 */
void BMS_ParsePack1FaultVT(const uint8_t *data) {
    taskENTER_CRITICAL();

    g_pack1_fault.cell_ov_mask = ((uint32_t)data[0]) | ((uint32_t)data[1] << 8) | ((uint32_t)(data[2] & 0x0F) << 16);

    g_pack1_fault.cell_uv_mask = ((uint32_t)((data[2] >> 4) & 0x0F)) | ((uint32_t)data[3] << 4) | ((uint32_t)data[4] << 12);

    g_pack1_fault.temp_ot_mask = ((uint16_t)data[5]) | ((uint16_t)(data[6] & 0x03) << 8);

    g_pack1_fault.temp_ut_mask = ((uint16_t)((data[6] >> 2) & 0x3F)) | ((uint16_t)((data[7] & 0x0F) << 6));

    g_pack1_fault.has_vt_fault =
        (g_pack1_fault.cell_ov_mask != 0) || (g_pack1_fault.cell_uv_mask != 0) || (g_pack1_fault.temp_ot_mask != 0) || (g_pack1_fault.temp_ut_mask != 0);

    g_pack1_fault.has_any_fault = g_pack1_fault.has_vt_fault || g_pack1_fault.has_bk_fault;

    taskEXIT_CRITICAL();
}

/*******************************************************************************************************/
/**
 * @brief       BK故障报文解析 (ID: 0x1601FF22)
 *   @note      电芯断线 + 温度断线
 *              Motorola MSB编码:
 *                data[0] bit0~bit7  = C0~C7  BK
 *                data[1] bit0~bit7  = C8~C15 BK
 *                data[2] bit0~bit4  = C16~C20 BK
 *                data[2] bit5~bit7  = T1~T3  BK
 *                data[3] bit0~bit7  = T4~T10 BK
 * @param       data: 8字节CAN数据指针
 * @retval      无
 */
void BMS_ParsePack1FaultBK(const uint8_t *data) {
    taskENTER_CRITICAL();

    g_pack1_fault.cell_bk_mask = ((uint32_t)data[0]) | ((uint32_t)data[1] << 8) | ((uint32_t)(data[2] & 0x1F) << 16);

    g_pack1_fault.temp_bk_mask = ((uint16_t)((data[2] >> 5) & 0x07)) | ((uint16_t)data[3] << 3);

    g_pack1_fault.has_bk_fault = (g_pack1_fault.cell_bk_mask != 0) || (g_pack1_fault.temp_bk_mask != 0);

    g_pack1_fault.has_any_fault = g_pack1_fault.has_vt_fault || g_pack1_fault.has_bk_fault;

    taskEXIT_CRITICAL();
}

/*******************************************************************************************************/

/**
 * @brief       故障检测任务（200ms周期任务）
 *   @note      每个周期检测两类故障源：
 *              1. IO层故障信号 (io_read_fault00)
 *              2. 从控CAN故障报文 (g_pack1_fault)
 *              若检测到任一故障则向BMS状态机发送故障事件。
 * @param       argument: 任务参数（未使用）
 * @retval      无
 */
void Task_Fault_Detect(void *argument) {
    TickType_t xLastWakeTime;
    PACK1_FaultStatus_t fault_snapshot = {0};

    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));

        /* 故障源1: IO层故障信号 */
        uint8_t io_fault = (io_read_fault00() != 0) ? 1 : 0;

        /* 故障源2: 从控CAN故障报文 (原子读取) */
        taskENTER_CRITICAL();
        fault_snapshot = g_pack1_fault;
        taskEXIT_CRITICAL();

        /* 故障综合判断 - 有任一故障即向状态机发送事件 */
        if (io_fault || fault_snapshot.has_any_fault) { BMS_SetFault(); }
    }
}
