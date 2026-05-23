#ifndef _BMS_DATA_H
#define _BMS_DATA_H

#include "main.h"

/*******************************************************************************************************/
/* PACK 电芯电压温度 CAN ID 定义 */

#define PACK1_CELLV1 0x1001FF11UL /* PACK1 电芯1电压 */
#define PACK1_CELLV2 0x1001FF12UL /* PACK1 电芯2电压 */
#define PACK1_CELLV3 0x1001FF13UL /* PACK1 电芯3电压 */
#define PACK1_CELLV4 0x1001FF14UL /* PACK1 电芯4电压 */
#define PACK1_CELLV5 0x1001FF15UL /* PACK1 电芯5电压 */
#define PACK1_TEMPV1 0x1001FF16UL /* PACK1 温度传感器1 */
#define PACK1_TEMPV2 0x1001FF17UL /* PACK1 温度传感器2 */

#define PACK2_CELLV1 0x1002FF11UL /* PACK2 电芯1电压 */
#define PACK2_CELLV2 0x1002FF12UL /* PACK2 电芯2电压 */
#define PACK2_CELLV3 0x1002FF13UL /* PACK2 电芯3电压 */
#define PACK2_CELLV4 0x1002FF14UL /* PACK2 电芯4电压 */
#define PACK2_CELLV5 0x1002FF15UL /* PACK2 电芯5电压 */
#define PACK2_TEMPV1 0x1002FF16UL /* PACK2 温度传感器1 */
#define PACK2_TEMPV2 0x1002FF17UL /* PACK2 温度传感器2 */

/*******************************************************************************************************/
/* CAN ID 区域范围定义 */

#define PACK1_CELLV_ID_MIN PACK1_CELLV1 /* PACK1 电芯电压ID最小值 */
#define PACK1_CELLV_ID_MAX PACK1_CELLV5 /* PACK1 电芯电压ID最大值 */
#define PACK1_TEMPV_ID_MIN PACK1_TEMPV1 /* PACK1 温度ID最小值 */
#define PACK1_TEMPV_ID_MAX PACK1_TEMPV2 /* PACK1 温度ID最大值 */

#define PACK2_CELLV_ID_MIN PACK2_CELLV1 /* PACK2 电芯电压ID最小值 */
#define PACK2_CELLV_ID_MAX PACK2_CELLV5 /* PACK2 电芯电压ID最大值 */
#define PACK2_TEMPV_ID_MIN PACK2_TEMPV1 /* PACK2 温度ID最小值 */
#define PACK2_TEMPV_ID_MAX PACK2_TEMPV2 /* PACK2 温度ID最大值 */

/*******************************************************************************************************/
/* 模组数据结构体定义 */

#define BMS_MODULE_TOTAL_COUNT 2 /* 系统总模组数量 */

/* PACK数据组 */
typedef struct {
    uint16_t cell_voltage1;
    uint16_t cell_voltage2;
    uint16_t cell_voltage3;
    uint16_t cell_voltage4;
    uint16_t cell_voltage5;
    uint16_t cell_voltage6;
    uint16_t cell_voltage7;
    uint16_t cell_voltage8;
    uint16_t cell_voltage9;
    uint16_t cell_voltage10;
    uint16_t cell_voltage11;
    uint16_t cell_voltage12;
    uint16_t cell_voltage13;
    uint16_t cell_voltage14;
    uint16_t cell_voltage15;
    uint16_t cell_voltage16;
    uint16_t cell_voltage17;
    uint16_t cell_voltage18;
    uint16_t cell_voltage19;
    uint16_t cell_voltage20;
    uint8_t temperature1;
    uint8_t temperature2;
    uint8_t temperature3;
    uint8_t temperature4;
    uint8_t temperature5;
    uint8_t temperature6;
    uint8_t temperature7;
    uint8_t temperature8;
    uint8_t temperature9;
    uint8_t temperature10;
} BMS_ModuleData_t;

extern volatile BMS_ModuleData_t g_pack1_module;
extern volatile BMS_ModuleData_t g_pack2_module;

/*******************************************************************************************************/
/* BMS数据池初始化函数 */
void BMS_DataPoolInit(void); /* BMS数据池初始化 */

/*******************************************************************************************************/
/* 模组电压和温度数据解析函数 */

void BMS_ParsePack1VoltTemp(uint32_t can_id, const uint8_t *data); /* PACK1 电压温度解析函数 */
void BMS_ParsePack2VoltTemp(uint32_t can_id, const uint8_t *data); /* PACK2 电压温度解析函数 */

/*******************************************************************************************************/
/* 安全读取接口 - 临界区保护，避免读端读到不一致的数据 */

void BMS_GetPack1Data(BMS_ModuleData_t *p_out); /* 原子性读取PACK1数据快照 */
void BMS_GetPack2Data(BMS_ModuleData_t *p_out); /* 原子性读取PACK2数据快照 */

#endif /* _BMS_DATA_H */

/*

*/
