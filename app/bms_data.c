#include "bms_data.h"
#include "string.h"
#include "task.h"

/** PACK1 模组数据 (volatile防止编译器优化缓存，多任务共享) */
volatile BMS_ModuleData_t g_pack1_module = {0};

/** PACK2 模组数据 (volatile防止编译器优化缓存，多任务共享) */
volatile BMS_ModuleData_t g_pack2_module = {0};

/**
 * @brief       BMS数据池初始化
 *   @note      清零所有PACK模组的电芯电压和温度数据
 *              在临界区中执行，防止初始化过程中被读端干扰
 * @param       无
 * @retval      无
 */
void BMS_DataPoolInit(void) {
    taskENTER_CRITICAL();
    memset((void *)&g_pack1_module, 0, sizeof(BMS_ModuleData_t));
    memset((void *)&g_pack2_module, 0, sizeof(BMS_ModuleData_t));
    taskEXIT_CRITICAL();
}

/**
 * @brief       模组电压温度数据解析函数
 *   @note      电压编码格式: Motorola MSB (Big-Endian), 每帧4个uint16_t，单位: mV
 *              温度编码格式: Motorola MSB, 每字节1个UINT8温度值，单位: ℃，范围 -40~120
 * @param       can_id: CAN帧ID (29-bit扩展帧)
 * @param       data:   8字节CAN数据指针
 * @retval      无
 */
void BMS_ParsePack1VoltTemp(uint32_t can_id, const uint8_t *data) {
    taskENTER_CRITICAL();
    switch (can_id) {
    case PACK1_CELLV1: /* 电芯电压 1~4 */
        g_pack1_module.cell_voltage1 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack1_module.cell_voltage2 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack1_module.cell_voltage3 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack1_module.cell_voltage4 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK1_CELLV2: /* 电芯电压 5~8 */
        g_pack1_module.cell_voltage5 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack1_module.cell_voltage6 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack1_module.cell_voltage7 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack1_module.cell_voltage8 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK1_CELLV3: /* 电芯电压 9~12 */
        g_pack1_module.cell_voltage9 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack1_module.cell_voltage10 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack1_module.cell_voltage11 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack1_module.cell_voltage12 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK1_CELLV4: /* 电芯电压 13~16 */
        g_pack1_module.cell_voltage13 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack1_module.cell_voltage14 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack1_module.cell_voltage15 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack1_module.cell_voltage16 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK1_CELLV5: /* 电芯电压 17~20 */
        g_pack1_module.cell_voltage17 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack1_module.cell_voltage18 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack1_module.cell_voltage19 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack1_module.cell_voltage20 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK1_TEMPV1: /* 温度传感器 NTC1~NTC8 (0x1001FF16) */
        g_pack1_module.temperature1 = data[0];
        g_pack1_module.temperature2 = data[1];
        g_pack1_module.temperature3 = data[2];
        g_pack1_module.temperature4 = data[3];
        g_pack1_module.temperature5 = data[4];
        g_pack1_module.temperature6 = data[5];
        g_pack1_module.temperature7 = data[6];
        g_pack1_module.temperature8 = data[7];
        break;

    case PACK1_TEMPV2: /* 温度传感器 NTC9~NTC10 (0x1001FF17) */
        g_pack1_module.temperature9 = data[0];
        g_pack1_module.temperature10 = data[1];
        /* data[2]~data[7] 为预留字节 */
        break;

    default:
        break;
    }
    taskEXIT_CRITICAL();
}

void BMS_ParsePack2VoltTemp(uint32_t can_id, const uint8_t *data) {
    taskENTER_CRITICAL();
    switch (can_id) {
    case PACK2_CELLV1: /* 电芯电压 1~4 */
        g_pack2_module.cell_voltage1 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack2_module.cell_voltage2 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack2_module.cell_voltage3 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack2_module.cell_voltage4 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK2_CELLV2: /* 电芯电压 5~8 */
        g_pack2_module.cell_voltage5 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack2_module.cell_voltage6 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack2_module.cell_voltage7 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack2_module.cell_voltage8 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK2_CELLV3: /* 电芯电压 9~12 */
        g_pack2_module.cell_voltage9 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack2_module.cell_voltage10 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack2_module.cell_voltage11 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack2_module.cell_voltage12 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK2_CELLV4: /* 电芯电压 13~16 */
        g_pack2_module.cell_voltage13 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack2_module.cell_voltage14 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack2_module.cell_voltage15 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack2_module.cell_voltage16 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK2_CELLV5: /* 电芯电压 17~20 */
        g_pack2_module.cell_voltage17 = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
        g_pack2_module.cell_voltage18 = (uint16_t)(((uint16_t)data[2] << 8) | (uint16_t)data[3]);
        g_pack2_module.cell_voltage19 = (uint16_t)(((uint16_t)data[4] << 8) | (uint16_t)data[5]);
        g_pack2_module.cell_voltage20 = (uint16_t)(((uint16_t)data[6] << 8) | (uint16_t)data[7]);
        break;

    case PACK2_TEMPV1: /* 温度传感器 NTC1~NTC8 (0x1002FF16) */
        g_pack2_module.temperature1 = data[0];
        g_pack2_module.temperature2 = data[1];
        g_pack2_module.temperature3 = data[2];
        g_pack2_module.temperature4 = data[3];
        g_pack2_module.temperature5 = data[4];
        g_pack2_module.temperature6 = data[5];
        g_pack2_module.temperature7 = data[6];
        g_pack2_module.temperature8 = data[7];
        break;

    case PACK2_TEMPV2: /* 温度传感器 NTC9~NTC10 (0x1002FF17) */
        g_pack2_module.temperature9 = data[0];
        g_pack2_module.temperature10 = data[1];
        /* data[2]~data[7] 为预留字节 */
        break;

    default:
        break;
    }
    taskEXIT_CRITICAL();
}

/*******************************************************************************************************/
/* 安全读取接口 - 在临界区内拷贝整个结构体，保证读取数据的一致性 */

void BMS_GetPack1Data(BMS_ModuleData_t *p_out) {
    taskENTER_CRITICAL();
    memcpy((void *)p_out, (const void *)&g_pack1_module, sizeof(BMS_ModuleData_t));
    taskEXIT_CRITICAL();
}

void BMS_GetPack2Data(BMS_ModuleData_t *p_out) {
    taskENTER_CRITICAL();
    memcpy((void *)p_out, (const void *)&g_pack2_module, sizeof(BMS_ModuleData_t));
    taskEXIT_CRITICAL();
}


