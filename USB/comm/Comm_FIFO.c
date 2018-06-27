#include "Comm_FIFO.h"
#include "comDef.h"

/******************************************************************************/
/* 接收/发送数据FIFO */
FrameFIFOTypedef RxDataFIFO;
DataTypedef TxData;
uint8 Comm_RequestTX = 0;

/******************************************************************************/
void Comm_FIFO_Init(FrameFIFOTypedef *FIFO)
{
	uint8 i;
	
	FIFO->Error = RESET;
	FIFO->Free = FIFO_Index;
	FIFO->GetPos = RESET;
	FIFO->PutPos = RESET;
	FIFO->Size = FIFO_Index;
	
	/* FIFO->Buffer清零 */
	for(i = 0; i < FIFO_Index; i++)
	{
		memset(&(FIFO->Buffer[i]), 0, FIFO_LenMax);
	}
}

/******************************************************************************/
ErrorStatus Comm_FIFO_RxDataPut(DataTypedef *SourceData,FrameFIFOTypedef *DestinationData)
{	
	/* 没有空余缓存,无法入列,返回错误 */
	if(DestinationData->Free == RESET)
	{
		return ERROR;
	}
	
	/* 数据长度到FIFO */
	memcpy((uint8*)&(DestinationData->Buffer[DestinationData->PutPos]),SourceData, SourceData->len+2);
	
	/* 入列地址增加,空白缓存减少 */
	DestinationData->PutPos++;
	DestinationData->Free--;
	
	/* 地址到缓存区末尾,从头再开始 */
	if(DestinationData->PutPos >= DestinationData->Size)
	{
		DestinationData->PutPos = RESET;
	}
	
	/* 数据入列成功 */
	return SUCCESS; 
}

/******************************************************************************/
ErrorStatus Comm_FIFO_RxDataGet(FrameFIFOTypedef *SourceData, 
							        DataTypedef *DestinationData)
{
	uint8* pData;
	uint8* pDest;
	uint16 i;
	
	/* 没有数据可读,返回错误 */
	if(SourceData->Free == SourceData->Size)
	{
		return ERROR;
	}
	
	/* 获取源数据和目标数据首地址 */
	pData = (uint8*)&(SourceData->Buffer[SourceData->GetPos]);
	pDest = (uint8*)(DestinationData);
	
	for(i = 0; i<(*(uint16*)&(SourceData->Buffer[SourceData->GetPos][0]))+2; i++)
	{
		*pDest = *pData;
		pDest++;
		pData++;
	}

	/* 出列地址增加,空白缓存释放 */
	SourceData->GetPos++;
	SourceData->Free++;
	
	/* 地址到缓存区末尾,从头再开始 */
	if(SourceData->GetPos >= SourceData->Size)
	{
		SourceData->GetPos = RESET;
	}
	
	/* 数据出列成功 */
	return SUCCESS;
}

/******************************************************************************/
ErrorStatus Comm_FIFO_TxDataPut(DataTypedef *SourceData,
							        FrameFIFOTypedef *DestinationData)
{	
	uint8* pDest;
	uint8* pData;
	uint8 i;
	
	/* 没有空余缓存,无法入列,返回错误 */
	if(DestinationData->Free == RESET)
	{
		return ERROR;
	}
	
	/* 获取源数据和目标数据首地址 */
	pData = (uint8*)SourceData;
	pDest = (uint8*)&(DestinationData->Buffer[DestinationData->PutPos]);
	
	/* 源数据和目标数据一一对应,直接Copy */
	for(i = 0; i < SourceData->len+2; i++)
	{
		*pDest = *pData;
		pDest++;
		pData++;	
	}
	
	/* 入列地址增加,空白缓存减少 */
	DestinationData->PutPos++;
	DestinationData->Free--;
	
	/* 地址到缓存区末尾,从头再开始 */
	if(DestinationData->PutPos >= DestinationData->Size)
	{
		DestinationData->PutPos = RESET;
	}
	
	/* 数据入列成功 */	
	return SUCCESS;
}

/******************************************************************************/
ErrorStatus Comm_FIFO_TxDataGet(FrameFIFOTypedef *SourceData, 
							        DataTypedef *DestinationData)
{
	/* 没有数据可读,返回错误 */
	if(SourceData->Free == SourceData->Size)
	{
		return ERROR;
	}
	
	/* 从FIFO Copy Data到发送区 */
	memcpy(DestinationData,&SourceData->Buffer[SourceData->GetPos][0],(*(uint16*)&(SourceData->Buffer[SourceData->GetPos][0]))+2);
	
	/* 出列地址增加,空白缓存释放 */
	SourceData->GetPos++;
	SourceData->Free++;
	
	/* 地址到缓存区末尾,从头再开始 */
	if(SourceData->GetPos >= SourceData->Size)
	{
		SourceData->GetPos = RESET;
	}
	
	/* 数据出列成功 */
	return SUCCESS;
}
