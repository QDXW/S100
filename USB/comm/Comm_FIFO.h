/*
 * Comm.h
 *
 *  Created on: 2016年8月17日
 *      Author: Administrator
 */

#ifndef _COMM_FIFO_H_
#define _COMM_FIFO_H_

/******************************************************************************/
#include "stm32f10x.h"
#include "comDef.h"

/******************************************************************************/
/* 缓存级数 */
#define FIFO_Index    (3)
/*单帧的数据长度*/
#define FIFO_LenMax      (2000)

/******************************************************************************/
/* 单帧FIFO定义 */
typedef struct
{
	/* 数据缓存区:   缓存级数        缓存数据区   */
	uint8 Buffer[FIFO_Index][FIFO_LenMax];
	/* 入列位置地址 出列位置地址 */
	uint8 PutPos, GetPos;
	/* 缓存区容量  缓存区空余缓存数量 错误标志位 */
	uint8  Size, Free, Error;
} FrameFIFOTypedef;

/******************************************************************************/
#define StructRecvDataLenth (14)

/******************************************************************************/
typedef struct
{	
	/* 待发送的Data长度 */
	uint16 len;
	/* 待发送的Data */
	uint8 Data[FIFO_LenMax];
} DataTypedef;

/******************************************************************************/
extern FrameFIFOTypedef RxDataFIFO;
extern DataTypedef TxData;
extern uint8 Comm_RequestTX;

/******************************************************************************/
extern void Comm_FIFO_Init(FrameFIFOTypedef *FIFO);
extern ErrorStatus Comm_FIFO_RxDataPut(DataTypedef *SourceData,
							  FrameFIFOTypedef *DestinationData);
extern ErrorStatus Comm_FIFO_RxDataGet(FrameFIFOTypedef *SourceData,
							  DataTypedef *DestinationData);
extern ErrorStatus Comm_FIFO_TxDataPut(DataTypedef *SourceData,
							  FrameFIFOTypedef *DestinationData);
extern ErrorStatus Comm_FIFO_TxDataGet(FrameFIFOTypedef *SourceData,
							  DataTypedef *DestinationData);

#endif /* SOURCE_MODULE_COMM_COMM_FIFO_H_ */
