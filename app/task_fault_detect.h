#ifndef _TASK_FAULT_DETECT_H
#define _TASK_FAULT_DETECT_H

#include "main.h"

/*******************************************************************************************************/
/* 从控故障报文 CAN ID 定义 */

#define PACK1_FAULT_VT_ID 0x1601FF21UL /* PACK1 从控VT故障报文 (电芯过压/欠压 + 温度过温/低温) */
#define PACK1_FAULT_BK_ID 0x1601FF22UL /* PACK1 从控BK故障报文 (电芯断线 + 温度断线) */

/*******************************************************************************************************/
/* 从控故障状态结构体 - 存储PACK1故障报文解析后的各告警位 */

typedef struct {
    uint32_t cell_ov_mask; /* Cell1~Cell20 过压告警掩码 */
    uint32_t cell_uv_mask; /* Cell1~Cell20 欠压告警掩码 */
    uint16_t temp_ot_mask; /* Temp1~Temp10 过温告警掩码 */
    uint16_t temp_ut_mask; /* Temp1~Temp10 低温告警掩码 */
    uint32_t cell_bk_mask; /* C0~C20 断线告警掩码 */
    uint16_t temp_bk_mask; /* T1~T10 温度断线告警掩码 */

    uint8_t has_vt_fault;  /* VT故障汇总 */
    uint8_t has_bk_fault;  /* BK故障汇总 */
    uint8_t has_any_fault; /* 有任意故障 */
} PACK1_FaultStatus_t;

extern volatile PACK1_FaultStatus_t g_pack1_fault; /* PACK1从控故障状态 */

/*******************************************************************************************************/
/* 故障报文解析函数声明 */

void BMS_ParsePack1FaultVT(const uint8_t *data); /* 解析VT故障 (ID: 0x1601FF21) */
void BMS_ParsePack1FaultBK(const uint8_t *data); /* 解析BK故障 (ID: 0x1601FF22) */

/*******************************************************************************************************/
/* 任务函数声明 */

void Task_Fault_Detect(void *argument); /* 故障检测任务入口函数 (200ms周期) */

#endif /* _TASK_FAULT_DETECT_H */

/*
PACK1_FAULT_VT	0X1601FF21	Cycle		8															RX
                    Motorola Msb		0	1		Fault_Cell1_OV	Fault Cell1 Over Voltage	1号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		1	1		Fault_Cell2_OV	Fault Cell2 Over Voltage	2号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		2	1		Fault_Cell3_OV	Fault Cell3 Over Voltage	3号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		3	1		Fault_Cell4_OV	Fault Cell4 Over Voltage	4号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		4	1		Fault_Cell5_OV	Fault Cell5 Over Voltage	5号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		5	1		Fault_Cell6_OV	Fault Cell6 Over Voltage	6号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		6	1		Fault_Cell7_OV	Fault Cell7 Over Voltage	7号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		7	1		Fault_Cell8_OV	Fault Cell8 Over Voltage	8号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		8	1		Fault_Cell9_OV	Fault Cell9 Over Voltage	9号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		9	1		Fault_Cell10_OV	Fault Cell10 Over Voltage	10号电芯单体过压告警	1	0		0-无告警，1-告警	0	1
RX Motorola Msb		10	1		Fault_Cell11_OV	Fault Cell11 Over Voltage	11号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb
11	1		Fault_Cell12_OV	Fault Cell12 Over Voltage	12号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		12	1
Fault_Cell13_OV	Fault Cell13 Over Voltage	13号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		13	1		Fault_Cell14_OV
Fault Cell14 Over Voltage	14号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		14	1		Fault_Cell15_OV	Fault Cell15 Over
Voltage	15号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		15	1		Fault_Cell16_OV	Fault Cell16 Over Voltage
16号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		16	1		Fault_Cell17_OV	Fault Cell17 Over Voltage	17号电芯单体过压告警
1	0		0-无告警，1-告警	0	1	RX Motorola Msb		17	1		Fault_Cell18_OV	Fault Cell18 Over Voltage	18号电芯单体过压告警	1	0
0-无告警，1-告警	0	1	RX Motorola Msb		18	1		Fault_Cell19_OV	Fault Cell19 Over Voltage	19号电芯单体过压告警	1	0		0-无告警，1-告警
0	1	RX Motorola Msb		19	1		Fault_Cell20_OV	Fault Cell20 Over Voltage	20号电芯单体过压告警	1	0		0-无告警，1-告警	0	1	RX Motorola
Msb		20	1		Fault_Cell1_UV	Fault Cell1 Under Voltage	1号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		21	1
Fault_Cell2_UV	Fault Cell2 Under Voltage	2号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		22	1		Fault_Cell3_UV	Fault
Cell3 Under Voltage	3号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		23	1		Fault_Cell4_UV	Fault Cell4 Under Voltage
4号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		24	1		Fault_Cell5_UV	Fault Cell5 Under Voltage	5号电芯单体欠压告警	1
0		0-无告警，1-告警	0	1	RX Motorola Msb		25	1		Fault_Cell6_UV	Fault Cell6 Under Voltage	6号电芯单体欠压告警	1	0		0-无告警，1-告警
0	1	RX Motorola Msb		26	1		Fault_Cell7_UV	Fault Cell7 Under Voltage	7号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb
27	1		Fault_Cell8_UV	Fault Cell8 Under Voltage	8号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		28	1
Fault_Cell9_UV	Fault Cell9 Under Voltage	9号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		29	1		Fault_Cell10_UV	Fault
Cell10 Under Voltage	10号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		30	1		Fault_Cell11_UV	Fault Cell11 Under
Voltage	11号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		31	1		Fault_Cell12_UV	Fault Cell12 Under Voltage
12号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		32	1		Fault_Cell13_UV	Fault Cell13 Under Voltage	13号电芯单体欠压告警
1	0		0-无告警，1-告警	0	1	RX Motorola Msb		33	1		Fault_Cell14_UV	Fault Cell14 Under Voltage	14号电芯单体欠压告警	1	0
0-无告警，1-告警	0	1	RX Motorola Msb		34	1		Fault_Cell15_UV	Fault Cell15 Under Voltage	15号电芯单体欠压告警	1	0		0-无告警，1-告警
0	1	RX Motorola Msb		35	1		Fault_Cell16_UV	Fault Cell16 Under Voltage	16号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola
Msb		36	1		Fault_Cell17_UV	Fault Cell17 Under Voltage	17号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		37	1
Fault_Cell18_UV	Fault Cell18 Under Voltage	18号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		38	1		Fault_Cell19_UV
Fault Cell19 Under Voltage	19号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		39	1		Fault_Cell20_UV	Fault Cell20 Under
Voltage	20号电芯单体欠压告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		40	1		Fault_Temp1_OT	Fault T1 Over Temperature
NTC1温度过温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		41	1		Fault_Temp2_OT	Fault T2 Over Temperature	NTC2温度过温告警	1
0		0-无告警，1-告警	0	1	RX Motorola Msb		42	1		Fault_Temp3_OT	Fault T3 Over Temperature	NTC3温度过温告警	1	0		0-无告警，1-告警
0	1	RX Motorola Msb		43	1		Fault_Temp4_OT	Fault T4 Over Temperature	NTC4温度过温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb
44	1		Fault_Temp5_OT	Fault T5 Over Temperature	NTC5温度过温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		45	1
Fault_Temp6_OT	Fault T6 Over Temperature	NTC6温度过温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		46	1		Fault_Temp7_OT	Fault T7
Over Temperature	NTC7温度过温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		47	1		Fault_Temp8_OT	Fault T8 Over Temperature
NTC8温度过温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		48	1		Fault_Temp9_OT	Fault T9 Over Temperature	NTC9温度过温告警	1
0		0-无告警，1-告警	0	1	RX Motorola Msb		49	1		Fault_Temp10_OT	Fault T10 Over Temperature	NTC10温度过温告警	1	0		0-无告警，1-告警
0	1	RX Motorola Msb		50	1		Fault_Temp1_UT	Fault T1 Under Temperature	NTC1温度低温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb
51	1		Fault_Temp2_UT	Fault T2 Under Temperature	NTC2温度低温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		52	1
Fault_Temp3_UT	Fault T3 Under Temperature	NTC3温度低温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		53	1		Fault_Temp4_UT	Fault T4
Under Temperature	NTC4温度低温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		54	1		Fault_Temp5_UT	Fault T5 Under Temperature
NTC5温度低温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		55	1		Fault_Temp6_UT	Fault T6 Under Temperature	NTC6温度低温告警	1
0		0-无告警，1-告警	0	1	RX Motorola Msb		56	1		Fault_Temp7_UT	Fault T7 Under Temperature	NTC7温度低温告警	1	0		0-无告警，1-告警
0	1	RX Motorola Msb		57	1		Fault_Temp8_UT	Fault T8 Under Temperature	NTC8温度低温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb
58	1		Fault_Temp9_UT	Fault T9 Under Temperature	NTC9温度低温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		59	1
Fault_Temp10_UT	Fault T10 Under Temperature	NTC10温度低温告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		60					预留	1	0
0	0	0	RX PACK1_FAULT_BK	0X1601FF22	Cycle		8															RX Motorola Msb		0	1		Fault_C0_BK
Fault C0 Disconnected	1号模组c0断线告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		1	1		Fault_C1_BK	Fault C1 Disconnected
1号模组c1断线告警	1	0		0-无告警，1-告警	0	1	RX Motorola Msb		2	1		Fault_C2_BK	Fault C2 Disconnected	1号模组c2断线告警	1	0
0-无告警，1-告警	0	1	RX Motorola Msb		3	1		Fault_C3_BK	Fault C3 Disconnected	1号模组c3断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		4	1		Fault_C4_BK	Fault C4 Disconnected	1号模组c4断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		5	1		Fault_C5_BK	Fault C5 Disconnected	1号模组c5断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		6	1		Fault_C6_BK	Fault C6 Disconnected	1号模组c6断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		7	1		Fault_C7_BK	Fault C7 Disconnected	1号模组c7断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		8	1		Fault_C8_BK	Fault C8 Disconnected	1号模组c8断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		9	1		Fault_C9_BK	Fault C9 Disconnected	1号模组c9断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		10	1		Fault_C10_BK	Fault C10 Disconnected	1号模组c10断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		11	1		Fault_C11_BK	Fault C11 Disconnected	1号模组c11断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		12	1		Fault_C12_BK	Fault C12 Disconnected	1号模组c12断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		13	1		Fault_C13_BK	Fault C13 Disconnected	1号模组c13断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		14	1		Fault_C14_BK	Fault C14 Disconnected	1号模组c14断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		15	1		Fault_C15_BK	Fault C15 Disconnected	1号模组c15断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		16	1		Fault_C16_BK	Fault C16 Disconnected	1号模组c16断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		17	1		Fault_C17_BK	Fault C17 Disconnected	1号模组c17断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		18	1		Fault_C18_BK	Fault C18 Disconnected	1号模组c18断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		19	1		Fault_C19_BK	Fault C19 Disconnected	1号模组c19断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		20	1		Fault_C20_BK	Fault C20 Disconnected	1号模组c20断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		21	1		Fault_T1_BK	Fault T1 Disconnected	NTC1温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		22	1		Fault_T2_BK	Fault T2 Disconnected	NTC2温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		23	1		Fault_T3_BK	Fault T3 Disconnected	NTC3温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		24	1		Fault_T4_BK	Fault T4 Disconnected	NTC4温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		25	1		Fault_T5_BK	Fault T5 Disconnected	NTC5温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		26	1		Fault_T6_BK	Fault T6 Disconnected	NTC6温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		27	1		Fault_T7_BK	Fault T7 Disconnected	NTC7温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		28	1		Fault_T8_BK	Fault T8 Disconnected	NTC8温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		29	1		Fault_T9_BK	Fault T9 Disconnected	NTC9温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		30	1		Fault_T10_BK	Fault T10 Disconnected	NTC10温度断线告警	1	0		0-无告警，1-告警	0	1	RX
                    Motorola Msb		31					预留	1	0		0	0	0	RX

*/
