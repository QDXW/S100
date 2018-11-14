/*
 * SystemManage_RTC.c
 *
 *  Created on: 2018Äê3ÔÂ12ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "SystemManage_RTC.h"

/******************************************************************************/
RTC_DATA SystemManage_CurrentTime;
RTC_DATA SystemManage_RecordTime = {
		2018,		 /* year */
		9,			 /* month */
		6, 		 /* day */
		16, 		 /* hour */
		20, 		 	 /* minute */
		5
};

/******************************************************************************/
/* Setting time */
RTC_DATA SystemManage_SetTime = {
		2018,		 /* year */
		3,			 /* month */
		12, 		 /* day */
		16, 		 /* hour */
		8, 		 	 /* minute */
		0 			 /* second */
};

/******************************************************************************/
void Delay_us_SW(__IO uint32 nCount)
{
#define SW_72MHZ_1MS_COUNT (10)
	uint32 subCount;
	for(; nCount != 0; nCount--)
	{
		subCount = SW_72MHZ_1MS_COUNT;
		for(; subCount != 0; subCount--);
	}
}
/******************************************************************************/
void IIC_Start(void)
{
	SDA_H();
	SCL_H();
	Delay_us_SW(20);
	SDA_L();
	Delay_us_SW(20);
	SDA_L();
	Delay_us_SW(20);
}

/******************************************************************************/
void IIC_Stop(void)
{
	SCL_L();
	Delay_us_SW(20);
	SDA_L();
	Delay_us_SW(20);
	SCL_H();
	Delay_us_SW(20);
	SDA_H();
	Delay_us_SW(20);
}

/******************************************************************************/
void IIC_WaitAck(void)
{
	uint16 k;

	SCL_L();
	SDA_H();
	Delay_us_SW(20);
	SCL_H();
	k = 0;

	while((Read_SDA()!= 0) && (k < 60000))
		k++;

	Delay_us_SW(20);
	SCL_L();
	Delay_us_SW(20);
}

/******************************************************************************/
void IIC_WriteByte(uint8 byte)
{
	uint8 i = 0;

	for(i = 0; i < 8; i++)
	{
		SCL_L();
		Delay_us_SW(20);
		if(byte & 0x80)
		{
			SDA_H();
		}
		else
		{
			SDA_L();
		}

		Delay_us_SW(20);
		SCL_H();
		Delay_us_SW(20);
		byte<<=1;
	}
	SCL_L();
	Delay_us_SW(20);
}

/******************************************************************************/
uint8 IIC_ReadByte(void)
{
	uint8 i,ReadByte;
	SDA_H();
	for(i = 0; i < 8; i++)
	{
		ReadByte <<= 1;
		SCL_L();
		Delay_us_SW(20);
		SCL_H();
		Delay_us_SW(20);
		if(Read_SDA())
		{
			ReadByte |= 0x01;
		}
		else
		{
			ReadByte &= ~(0x01);
		}
	}
	return ReadByte;
}

/******************************************************************************/
void I2C_Ack(void)
{
	SCL_L();
	Delay_us_SW(20);
	SDA_L();
	Delay_us_SW(20);
	SCL_H();
	Delay_us_SW(20);
	SCL_L();
	Delay_us_SW(20);
}

/******************************************************************************/
void I2C_NoAck(void)
{
	SCL_L();
	Delay_us_SW(20);
	SDA_H();
	Delay_us_SW(20);
	SCL_H();
	Delay_us_SW(20);
	SCL_L();
	Delay_us_SW(20);
}

/******************************************************************************/
uint8 PCF8563_ReaDAdress(uint8 Adress)
{
	uint8 ReadData;

	IIC_Start();
	IIC_WriteByte(0xa2);
	IIC_WaitAck();
	IIC_WriteByte(Adress);
	IIC_WaitAck();
	IIC_Start();
	IIC_WriteByte(0xa3);
	IIC_WaitAck();
	ReadData = IIC_ReadByte();
	IIC_Stop();

	return ReadData;
}

/******************************************************************************/
void  PCF8563_WriteAdress(uint8 Adress,uint8 DataTX)
{
	IIC_Start();
	IIC_WriteByte(0xa2);
	IIC_WaitAck();
	IIC_WriteByte(Adress);
	IIC_WaitAck();
	IIC_WriteByte(DataTX);
	IIC_WaitAck();
	IIC_Stop();
}

/******************************************************************************/
void PCF8563_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SCL_H();
	SDA_H();
}

/******************************************************************************/
void PCF8563_Set(uint16 syear, uint8 smon, uint8 sday, uint8 hour,
		uint8 min, uint8 sec, uint8 week)
{
	uint8 PCF8563_Time[7] = {0};

	/* Convert to BCD */
	PCF8563_Time[0] = ((sec/10) << 4) | (sec%10);
	PCF8563_Time[1] = ((min/10) << 4) | (min%10);
	PCF8563_Time[2] = ((hour/10) << 4) | (hour%10);
	PCF8563_Time[3] = ((sday/10) << 4) | (sday%10);
	PCF8563_Time[4] = ((week/10) << 4) | (week%10);
	PCF8563_Time[5] = ((smon/10) << 4) | (smon%10);
	PCF8563_Time[6] = (((syear%2000)/10 << 4)) | ((syear%2000)%10);

	//Disable RTC source clock
	PCF8563_WriteAdress(0x00, 0x20);

	//Set time
	PCF8563_WriteAdress(0x02, PCF8563_Time[0]);
	PCF8563_WriteAdress(0x03, PCF8563_Time[1]);
	PCF8563_WriteAdress(0x04, PCF8563_Time[2]);
	PCF8563_WriteAdress(0x05, PCF8563_Time[3]);
	PCF8563_WriteAdress(0x06, PCF8563_Time[4]);
	PCF8563_WriteAdress(0x07, PCF8563_Time[5]);
	PCF8563_WriteAdress(0x08, PCF8563_Time[6]);

	//Enable RTC sorce clock
	PCF8563_WriteAdress(0x00, 0x00);
}

/******************************************************************************/
void PCF8563_Read(RTC_DATA *currTime)
{
	uint8 PCF8563_Time[7] = {0};

	IIC_Start();
	IIC_WriteByte(0xa2);
	IIC_WaitAck();
	IIC_WriteByte(0x02);
	IIC_WaitAck();
	IIC_Start();
	IIC_WriteByte(0xa3);
	IIC_WaitAck();

	/* Read time */
	PCF8563_Time[0] = IIC_ReadByte()&0x7f;
	I2C_Ack();
	PCF8563_Time[1] = IIC_ReadByte()&0x7f;
	I2C_Ack();
	PCF8563_Time[2] = IIC_ReadByte()&0x3f;
	I2C_Ack();
	PCF8563_Time[3] = IIC_ReadByte()&0x3f;
	I2C_Ack();
	PCF8563_Time[4] = IIC_ReadByte()&0x07;
	I2C_Ack();
	PCF8563_Time[5] = IIC_ReadByte()&0x1f;
	I2C_Ack();
	PCF8563_Time[6] = IIC_ReadByte();
	I2C_NoAck();
	IIC_Stop();

	currTime->sec = ((PCF8563_Time[0]&0xf0)>>4)*10 + (PCF8563_Time[0]&0x0f);
	currTime->min = ((PCF8563_Time[1]&0xf0)>>4)*10 + (PCF8563_Time[1]&0x0f);
	currTime->hour =((PCF8563_Time[2]&0xf0)>>4)*10 + (PCF8563_Time[2]&0x0f);
	currTime->day = ((PCF8563_Time[3]&0xf0)>>4)*10 + (PCF8563_Time[3]&0x0f);
//	((PCF8563_Time[4]&0xf0)>>4)*10 + (PCF8563_Time[4]&0x0f);
	currTime->month = ((PCF8563_Time[5]&0xf0)>>4)*10 + (PCF8563_Time[5]&0x0f);
	currTime->year = 2000 + ((PCF8563_Time[6]&0xf0)>>4)*10 + (PCF8563_Time[6]&0x0f);
}

/******************************************************************************/
uint8 SystemManage_RTC_Set(uint16 syear, uint8 smon, uint8 sday, uint8 hour,
		uint8 min,uint8 sec)
{
	uint8 week = 0;

//	/* Calculate week */
//	week = RTC_Get_Week(syear, smon, sday);
//	/* Set PCF8563 */
	PCF8563_Set(syear, smon, sday, hour, min, sec, week);

	return 0;
}

/******************************************************************************/
uint8 SystemManage_RTC_Get(void)
{
	PCF8563_Read(&SystemManage_CurrentTime);
	return 0;
}
