#ifndef _CCS_DETECT_H
#define _CCS_DETECT_H

#include "can.h"
#include "main.h"

/*******************************************************************************************************/
/* CCS充电机CAN报文定义 */

/* CCS充电机→BMS 报文ID (TX：充电机发送，BMS接收) */
#define CCS_TX_ID 0x18FF50E5UL

/* BMS→CCS充电机 控制报文ID (TX：BMS发送，充电机接收) */
#define CCS_CTRL_ID 0x1806E5F4UL

/* CCS控制报文CAN端口选择 (0=CAN1, 1=CAN2) */
#define CCS_CTRL_CAN_PORT 1

/*******************************************************************************************************/
/* 函数声明 */

void CCS_ProcessEvent(uint8_t *data);             /* 解析CCS报文，检测状态位，决定是否发送充电请求 */
BaseType_t CCS_SendControlMsg(uint16_t max_charg_v, /* 发送充电控制报文：最高充电电压(0.1V/bit) */
                              uint16_t max_charg_c, /* 发送充电控制报文：最高充电电流(0.1A/bit) */
                              uint8_t control);     /* 发送充电控制报文：控制状态 (0=开启, 1=关闭) */

#endif /* _CCS_DETECT_H */

/*
CCS_RX	0X1806E5F4	Cycle		8																Tx
					Motorola Msb	0		16	UINT16	MAX_CHARG_V	Maximum charging terminal voltage	最高充电端电压	0.1V/bit	0	V	例：Vset =3201，对应电压为320.1v	0			Tx
					Motorola Msb	2		16	UINT16	MAX_CHARG_C	Maximum charging terminal current	最高充电端电流	0.1A/bit	0	A	例：Iset =582，对应电流为58.2A	0			Tx
					Motorola Msb	4		8	UINT8	CCS_CON	CCS Control	控制充电机	1	0		0：充电机开启充电。1：电池保护，充电器关闭输出。	0			Tx
					Motorola Msb	5						预留	1	0			0	0		Tx

*/
