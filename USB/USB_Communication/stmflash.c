/*******************************************************************************
 * File name: STMFlash.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#include "stmflash.h"
#include "QRCode.h"
#include "SystemManage_RTC.h"

extern QRCODE_STRUCT QR_Date; 

/******************************************************************************/
#define STM32_FLASH_SIZE (512) 	 	  /* Flash size, unit is Kbytes */

#if (STM32_FLASH_SIZE < 256)
#define STM_SECTOR_SIZE (1024)
#else
#define STM_SECTOR_SIZE	(2048)
#endif

/******************************************************************************/
uint16 flashTmpBuffer[STM_SECTOR_SIZE/2];
uint16 Information_Num = 1;

/******************************************************************************/
uint16 STMFlash_ReadHalfWord(uint32 faddr);
void STMFlash_Write_NoCheck(uint32 WriteAddr, uint16 *pBuffer,
		uint16 NumToWrite);

/******************************************************************************/
uint16 STMFlash_ReadHalfWord(uint32 faddr)
{
	return *(volatile uint16*)faddr;
}

/******************************************************************************/
void STMFlash_Write_NoCheck(uint32 WriteAddr, uint16 *pBuffer,
		uint16 NumToWrite)
{ 			 		 
	uint16 i;
	for(i = 0; i < NumToWrite; i++)
	{
		FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
	    WriteAddr += 2;
	}  
} 

/******************************************************************************/
void STMFlash_Write(uint32 WriteAddr, uint16 *pBuffer, uint16 NumToWrite)
{
	uint32 secpos;
	uint16 secoff;
	uint16 secremain;
 	uint16 i;
	uint32 offaddr;

	if(WriteAddr < FLASH_BASE ||
			(WriteAddr >= (FLASH_BASE + (STM32_FLASH_SIZE << 10))))
		return;

	FLASH_Unlock();
	offaddr = WriteAddr - FLASH_BASE;
	secpos = offaddr / STM_SECTOR_SIZE;
	secoff = (offaddr % STM_SECTOR_SIZE) >> 1;
	secremain = (STM_SECTOR_SIZE >> 1) - secoff;

	if(NumToWrite <= secremain)
		secremain = NumToWrite;

	while(1) 
	{
		STMFlash_Read(secpos * STM_SECTOR_SIZE + FLASH_BASE, flashTmpBuffer,
				STM_SECTOR_SIZE >> 1);
		for(i=0; i < secremain; i++)
		{
			if(flashTmpBuffer[secoff + i] != 0XFFFF)
				break;
		}

		if(i < secremain)
		{
			FLASH_ErasePage(secpos * STM_SECTOR_SIZE + FLASH_BASE);
			for(i = 0; i < secremain; i++)
			{
				flashTmpBuffer[i + secoff] = pBuffer[i];
			}

			STMFlash_Write_NoCheck(secpos * STM_SECTOR_SIZE + FLASH_BASE,
					flashTmpBuffer, STM_SECTOR_SIZE >> 1);
		}else
			STMFlash_Write_NoCheck(WriteAddr, pBuffer, secremain);

		if(NumToWrite == secremain)
			break;
		else
		{
			secpos++;
			secoff = 0;
		   	pBuffer += secremain;
			WriteAddr += secremain;
		   	NumToWrite -= secremain;
			if(NumToWrite > (STM_SECTOR_SIZE >> 1))
				secremain = STM_SECTOR_SIZE >> 1;
			else
				secremain = NumToWrite;
		}	 
	};

	FLASH_Lock();
}

/******************************************************************************/
void STMFlash_Read(uint32 ReadAddr, uint16 *pBuffer, uint16 NumToRead)
{
	uint16 i;

	for(i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = STMFlash_ReadHalfWord(ReadAddr);
		ReadAddr += 2;
	}
}

/******************************************************************************/
void Flash_storage(void)
{
	uint8 i = 0;
	uint16 pBuffer[10];
//	uint8 cBuffer[10];
//	uint8 tBuffer[10];
	uint16 pBuffer_time;

	STMFlash_Read(FLASH_CALI_STATUS_ADDR - Information_Num * STM_SECTOR_SIZE,&Information_Num,1);
	if(Information_Num < 0)
		return;

	if(Information_Num > 72)
		Information_Num = 1;

	Information_Num++;
	/* product name storage */
	for(i = 0;i < sizeof(QR_Date.head.name);i++)
	{
		pBuffer[i] = QR_Date.head.name[i];
	}
	STMFlash_Write(FLASH_CALI_STATUS_ADDR - STM_SECTOR_SIZE,pBuffer,10);

	/* product number storage */
	for(i = 0;i < sizeof(QR_Date.head.SN);i++)
	{
		pBuffer[i] = QR_Date.head.SN[i];
	}
	STMFlash_Write(FLASH_CALI_STATUS_ADDR - STM_SECTOR_SIZE + 10,pBuffer,10);

	/* time storage */
	STMFlash_Write(FLASH_CALI_STATUS_ADDR, &Information_Num, 1);
	STMFlash_Write(FLASH_CALI_STATUS_ADDR - Information_Num * STM_SECTOR_SIZE+20,&SystemManage_CurrentTime.year,1);

	pBuffer_time = SystemManage_CurrentTime.month;
	STMFlash_Write(FLASH_CALI_STATUS_ADDR - Information_Num * STM_SECTOR_SIZE+22,&pBuffer_time,1);

	pBuffer_time = SystemManage_CurrentTime.day;
	STMFlash_Write(FLASH_CALI_STATUS_ADDR - Information_Num * STM_SECTOR_SIZE+24,&pBuffer_time,1);

	pBuffer_time = SystemManage_CurrentTime.hour;
	STMFlash_Write(FLASH_CALI_STATUS_ADDR - Information_Num * STM_SECTOR_SIZE+26,&pBuffer_time,1);

	pBuffer_time = SystemManage_CurrentTime.min;
	STMFlash_Write(FLASH_CALI_STATUS_ADDR - Information_Num * STM_SECTOR_SIZE+28,&pBuffer_time,1);

	/* reagent strips */

}

/******************************************************************************/
void Flash_DataRead(void)
{
	//	if(Information_Num < 2)
	//		Information_Num = 2;
	//	switch(Key_control)
	//	{
	//		case 1:
	//			Information_Num++;
	//			break;
	//		case 2:
	//			Information_Num--;
	//			break;
	//		default:
	//			break;
	//	}
}
