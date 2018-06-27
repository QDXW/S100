/*
 * Storage_Flash.c
 *
 *  Created on: 2018Äê4ÔÂ18ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#include "Storage_Flash.h"

/******************************************************************************/
u16 W25QXX_TYPE = W25Q128;

#define BUFFER_SIZE (sizeof(STORAGE_SINGLE_DATA_STRUCT))
uint8 Storage_writeBuffer[BUFFER_SIZE];
uint8 Storage_readBuffer[BUFFER_SIZE];
uint16 reagent_Strip[4] = {0};

#define DEFAULT_VALUE (0XFF)
/* Head */
#define START_OFFSET (0)
/* Index */
#define INDEX_OFFSET (2048)
#define SINGLE_INDEX_SIZE (10)
#define SINGLE_DATA_SIZE (10)

STORAGE_SINGLE_DATA_STRUCT Storage_Data;

/******************************************************************************/
uint16 W25QXX_ReadID(void);
uint8 W25QXX_ReadSR(void);
void W25QXX_Write_SR(uint8 sr);
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disable(void);
void W25QXX_Write_NoCheck(uint8* pBuffer,uint32 WriteAddr,uint16 NumByteToWrite);
void W25QXX_Erase_Chip(void);
void W25QXX_Erase_Sector(uint32 Dst_Addr);
void W25QXX_Wait_Busy(void);
void W25QXX_PowerDown(void);
void W25QXX_WAKEUP(void);
uint8 SPI2_ReadWriteByte(uint8 TxData);
void SPI2_SetSpeed(uint8 SPI_BaudRatePrescaler);

/******************************************************************************/
void Storage_Flash_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	W25QXX_CS=1;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);

	SPI2_ReadWriteByte(0xff);

	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);

	W25QXX_TYPE = W25QXX_ReadID();
}

/******************************************************************************/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;
	SPI_Cmd(SPI2,ENABLE);
}

/******************************************************************************/
u8 SPI2_ReadWriteByte(u8 TxData)
{
	u8 retry=0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry>200)
			return 0;
	}

	SPI_I2S_SendData(SPI2, TxData);
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry>200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI2);
}

/******************************************************************************/
u8 W25QXX_ReadSR(void)
{
	u8 byte=0;
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_ReadStatusReg);
	byte=SPI2_ReadWriteByte(0Xff);
	W25QXX_CS=1;
	return byte;
}

/******************************************************************************/
void W25QXX_Write_SR(u8 sr)
{
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_WriteStatusReg);
	SPI2_ReadWriteByte(sr);
	W25QXX_CS=1;
}
void W25QXX_Write_Enable(void)
{
	W25QXX_CS=0;
    SPI2_ReadWriteByte(W25X_WriteEnable);
	W25QXX_CS=1;
}

/******************************************************************************/
void W25QXX_Write_Disable(void)
{
	W25QXX_CS=0;
    SPI2_ReadWriteByte(W25X_WriteDisable);
	W25QXX_CS=1;
}

/******************************************************************************/
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;
	W25QXX_CS=0;
	SPI2_ReadWriteByte(0x90);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0x00);
	Temp|=SPI2_ReadWriteByte(0xFF)<<8;
	Temp|=SPI2_ReadWriteByte(0xFF);
	W25QXX_CS=1;
	return Temp;
}

/******************************************************************************/
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;
    W25QXX_Write_Enable();
	W25QXX_CS=0;
    SPI2_ReadWriteByte(W25X_PageProgram);
    SPI2_ReadWriteByte((u8)((WriteAddr)>>16));
    SPI2_ReadWriteByte((u8)((WriteAddr)>>8));
    SPI2_ReadWriteByte((u8)WriteAddr);
    for(i=0;i<NumByteToWrite;i++)SPI2_ReadWriteByte(pBuffer[i]);
	W25QXX_CS=1;
	W25QXX_Wait_Busy();
}

/******************************************************************************/
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain=256-WriteAddr%256;
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;
	while(1)
	{
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;
	 	else
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;

			NumByteToWrite-=pageremain;
			if(NumByteToWrite>256)pageremain=256;
			else pageremain=NumByteToWrite;
		}
	};
}

/******************************************************************************/
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
  	W25QXX_CS=0;
    SPI2_ReadWriteByte(W25X_ChipErase);
	W25QXX_CS=1;
	W25QXX_Wait_Busy();
}

/******************************************************************************/
void W25QXX_Erase_Sector(u32 Dst_Addr)
{
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
  	W25QXX_CS=0;
    SPI2_ReadWriteByte(W25X_SectorErase);
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));
    SPI2_ReadWriteByte((u8)Dst_Addr);
	W25QXX_CS=1;
    W25QXX_Wait_Busy();
}

/******************************************************************************/
void W25QXX_Wait_Busy(void)
{
	while((W25QXX_ReadSR()&0x01)==0x01);
}

/******************************************************************************/
void W25QXX_PowerDown(void)
{
  	W25QXX_CS=0;
    SPI2_ReadWriteByte(W25X_PowerDown);
	W25QXX_CS=1;
    Delay_SW(100);
}

/******************************************************************************/
void W25QXX_WAKEUP(void)
{
  	W25QXX_CS=0;
    SPI2_ReadWriteByte(W25X_ReleasePowerDown);
	W25QXX_CS=1;
	Delay_SW(100);
}

/******************************************************************************/
void Storage_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
 	u16 i;
	W25QXX_CS=0;
    SPI2_ReadWriteByte(W25X_ReadData);
    SPI2_ReadWriteByte((u8)((ReadAddr)>>16));
    SPI2_ReadWriteByte((u8)((ReadAddr)>>8));
    SPI2_ReadWriteByte((u8)ReadAddr);
    for(i=0;i<NumByteToRead;i++)
	{
        pBuffer[i]=SPI2_ReadWriteByte(0XFF);
    }
	W25QXX_CS=1;
}

/******************************************************************************/
u8 W25QXX_BUFFER[4096];
void Storage_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
 	u16 i;
	u8 * W25QXX_BUF;
   	W25QXX_BUF=W25QXX_BUFFER;
 	secpos=WriteAddr/4096;
	secoff=WriteAddr%4096;
	secremain=4096-secoff;
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;
	while(1)
	{
		Storage_Read(W25QXX_BUF,secpos*4096,4096);
		for(i=0;i<secremain;i++)
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;
		}
		if(i<secremain)
		{
			W25QXX_Erase_Sector(secpos);
			for(i=0;i<secremain;i++)
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);
		if(NumByteToWrite==secremain)break;
		else
		{
			secpos++;
			secoff=0;

		   	pBuffer+=secremain;
			WriteAddr+=secremain;
		   	NumByteToWrite-=secremain;
			if(NumByteToWrite>4096)secremain=4096;
			else secremain=NumByteToWrite;
		}
	};
}

/******************************************************************************/
uint8 ByteHi1ST_32b(uint32 value)
{
	return (uint8)((value & 0xFF000000) >> 24);
}

/******************************************************************************/
uint8 ByteHi2ND_32b(uint32 value)
{
	return (uint8)((value & 0x00FF0000) >> 16);
}

/******************************************************************************/
uint8 ByteHi3RD_32b(uint32 value)
{
	return (uint8)((value & 0x0000FF00) >> 8);
}

/******************************************************************************/
uint8 ByteHi4TH_32b(uint32 value)
{
	return (uint8)(value & 0x000000FF);
}

/******************************************************************************/
uint8 HiByte_16b(uint16 value)
{
	return (uint8)((value & 0xFF00) >> 8);
}

/******************************************************************************/
uint8 LoByte_16b(uint16 value)
{
	return (uint8)(value & 0x00FF);
}

/******************************************************************************/
void Storage_Clear(void)
{
	STORAGE_HEAD_STRUCT headData = {0};
	/* Set to default */
	memset(&headData, DEFAULT_VALUE, sizeof(headData));
	/* Fill write buffer */
	memcpy(Storage_writeBuffer, &headData, sizeof(headData));
	/* Execute */
	Storage_Write(Storage_writeBuffer, START_OFFSET, sizeof(headData));
}

/******************************************************************************/
uint8 Storage_Record(void)
{
	Get_reagent_TestNum();
	memcpy(Storage_writeBuffer, &Storage_Data, sizeof(STORAGE_SINGLE_DATA_STRUCT));
	Storage_Write(Storage_writeBuffer, (reagent_Strip[0] *4096),sizeof(STORAGE_SINGLE_DATA_STRUCT));
}

/******************************************************************************/
uint8 Read_Record(void)
{
	uint8 Information[12] = 0;
	memset(Information,0,sizeof(Information));
	memset(&Storage_Data, 0, sizeof(STORAGE_SINGLE_DATA_STRUCT));
	if(Read_first)
	{
		Storage_Read(Information,0x00,4);
		memcpy(&reagent_Strip[0],Information,4);
		Read_first = 0;
	}

	if(reagent_Strip[0] == 0)
		return 0;
	Storage_Read(Storage_writeBuffer,(reagent_Strip[0] *4096),sizeof(STORAGE_SINGLE_DATA_STRUCT));
	memcpy(&Storage_Data,Storage_writeBuffer,sizeof(STORAGE_SINGLE_DATA_STRUCT));
}

/******************************************************************************/
void Get_reagent_TestNum(void)
{
	uint8 Information[12] = 0;
	reagent_Strip[3] = 0x55;
	memset(Information,0,sizeof(Information));
	Storage_Read(Information,0x00,4);
	memcpy(reagent_Strip,Information,4);
	reagent_Strip[0] += 1;
	if(reagent_Strip[0] > 100)
	{
		reagent_Strip[0] = 1;
		reagent_Strip[1] = 100;
		reagent_Strip[2] = 1;
	}

	if(reagent_Strip[2] == 0)
	{
		reagent_Strip[1] = reagent_Strip[0];
	}
	memcpy(Information,&reagent_Strip[0],4);
	Storage_Write(Information, 0x00, 4);
}

/******************************************************************************/
void Storage_Time(void)
{
	Storage_Data.Product_Time.year = SystemManage_CurrentTime.year;
	Storage_Data.Product_Time.month = SystemManage_CurrentTime.month;
	Storage_Data.Product_Time.day = SystemManage_CurrentTime.day;
	Storage_Data.Product_Time.hour = SystemManage_CurrentTime.hour;
	Storage_Data.Product_Time.min = SystemManage_CurrentTime.min;
}
