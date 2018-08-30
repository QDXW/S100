/*******************************************************************************
 * File name: HumanInput_CapTS.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#include "Font.h"
#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "HumanInput_CapTS.h"
#include "DisplayDriver_touch.h"

/******************************************************************************/
const u16 CapTS_TPX_TBL[5]=
{
	FT_TP1_REG,
	FT_TP2_REG,
	FT_TP3_REG,
	FT_TP4_REG,
	FT_TP5_REG
};

enum {
	XMAX,
	YMAX,
	XMIN,
	YMIN,
};

TouchPointRefTypeDef TPR_Structure;

typedef struct {
	uint16 xPos;
	uint16 yPos;
} TouchInfo;

TouchInfo CapTS_Touch_TD; /* Touchdown */
TouchInfo CapTS_Touch_Max; /* Maximal */
TouchInfo CapTS_Touch_Min; /* Minimal */
TouchInfo CapTS_Touch_LF; /* Liftoff */

uint32 TouchCount = 0;
uint16 xPos = 0;
uint16 yPos = 0;

/******************************************************************************/
void CapTS_Write_Regsister(uint16 reg, uint8* pBuffer, uint8 NumByteToWrite);
void CapTS_Read_Regsister(uint16 reg, uint8* pBuffer, uint16 NumByteToRead);

/******************************************************************************/
void CapTS_RCC_Configuration(void)
{
	/* I2C2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
}

/******************************************************************************/
void CapTS_GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	/* Configure I2C2 pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* INT */
	GPIO_InitStructure.GPIO_Pin =  CAPTS_INT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(CAPTS_INT_PORT, &GPIO_InitStructure);

	/* RST */
	GPIO_InitStructure.GPIO_Pin =  CAPTS_RST_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CAPTS_RST_PORT, &GPIO_InitStructure);
}

/******************************************************************************/
void CapTS_I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;

	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = CAPTS_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = CapTS_I2C_Speed;

	/* I2C Peripheral Enable */
	I2C_Cmd(CAPTS_I2C, ENABLE);
	/* Apply I2C configuration after enabling it */
	I2C_Init(CAPTS_I2C, &I2C_InitStructure);
}

/******************************************************************************/
void HumanInput_CapTS_Int(FunctionalState state)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = CAPTS_INT_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = state;
	EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************/
void CapTS_Exint_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource12);

	EXTI_ClearITPendingBit(CAPTS_INT_EXTI);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	HumanInput_CapTS_Int(DISABLE);
}

/******************************************************************************/
void HumanInput_CapTS_Reset(void)
{
	/* Reset device */
	GPIO_ResetBits(CAPTS_RST_PORT, CAPTS_RST_PIN);
	Delay_SW(0xFF);
	GPIO_SetBits(CAPTS_RST_PORT, CAPTS_RST_PIN);
//	Delay_SW(0xFFFF);
}

/******************************************************************************/
void HumanInput_CapTS_Init(void)
{
	/* Initialize GPIO */
	CapTS_RCC_Configuration();
	CapTS_GPIO_Configuration();
	CapTS_I2C_Configuration();

#if CAPTS_INT_ENABLED
	/* Initialize external interrupt */
	CapTS_Exint_Init();
#endif

	HumanInput_CapTS_Reset();
}

///******************************************************************************/
//uint8 HumanInput_CapTS_SelfTestAndInit(void)
//{
//	uint8 state = 0;
//	uint8 tmp = 0;
//
//	/* Start timer */
//	SystemManage_Timer_Init(1000); /* 1000ms */
//
//	/* While the bus is busy */
//	while(I2C_GetFlagStatus(CAPTS_I2C, I2C_FLAG_BUSY));
//
//	/* Send START condition */
//	I2C_GenerateSTART(CAPTS_I2C, ENABLE);
//
//	/* Test on EV5 and clear it */
//	while(!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_MODE_SELECT));
//
//	/* Send EEPROM address for write */
//	I2C_Send7bitAddress(CAPTS_I2C, CAPTS_ADDR, I2C_Direction_Transmitter);
//
//	/* Check status */
//	while (!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//	{
//		if (SystemManage_timeout)
//		{
//			state = 1;
//			break;
////		}
//	}
//
//	/* Send STOP condition */
//	I2C_GenerateSTOP(CAPTS_I2C, ENABLE);
//
//	/* Disable timer */
//	SystemManage_Timer_Disabled();
//
//	/* Initialize touch screen if connected */
//	if (!state)
//	{
//		/* Configure device: Enter normal mode */
//		tmp=0;
//		CapTS_Write_Regsister(FT_DEVIDE_MODE, &tmp, 1);
//		/* Set finger threshold */
//		tmp = CAPTS_FINGER_THRESHOLD;
//	 	CapTS_Write_Regsister(FT_ID_G_THGROUP, &tmp, 1);
//	 	/* Activation period, 12< period <=14 */
//	 	tmp = 12;
//	 	CapTS_Write_Regsister(FT_ID_G_PERIODACTIVE, &tmp, 1);
//	}
//	return state;
//}

/******************************************************************************/
void CapTS_Write_Regsister(uint16 reg, uint8* pBuffer, uint8 NumByteToWrite)
{
	/* While the bus is busy */
	while(I2C_GetFlagStatus(CAPTS_I2C, I2C_FLAG_BUSY));

	/* Send START condition */
	I2C_GenerateSTART(CAPTS_I2C, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(CAPTS_I2C, CAPTS_ADDR, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Send the EEPROM's internal address to write to */
	I2C_SendData(CAPTS_I2C, reg&0xFF);

	/* Test on EV8 and clear it */
	while(! I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* While there is data to be written */
	while(NumByteToWrite--)
	{
		/* Send the current byte */
		I2C_SendData(CAPTS_I2C, *pBuffer);

		/* Point to the next byte to be written */
		pBuffer++;

		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}

	/* Send STOP condition */
	I2C_GenerateSTOP(CAPTS_I2C, ENABLE);
}

/******************************************************************************/
void CapTS_Read_Regsister(uint16 reg, uint8* pBuffer, uint16 NumByteToRead)
{
	uint16 retries = 0;
	uint8 status = 0;

	/* While the bus is busy */
	while(I2C_GetFlagStatus(CAPTS_I2C, I2C_FLAG_BUSY));

	/* Send START condition */
	I2C_GenerateSTART(CAPTS_I2C, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(CAPTS_I2C, CAPTS_ADDR, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if (retries++ > 1000)
		{
			status = 1;
			break;
		}
	}

	retries = 0;

	if (status)
	{
		HumanInput_CapTS_Reset();

		I2C_DeInit(CAPTS_I2C);
		CapTS_I2C_Configuration();

		return;
	}

	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(CAPTS_I2C, ENABLE);

	/* Send the EEPROM's internal address to write to */
	I2C_SendData(CAPTS_I2C, reg&0xFF);

	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Send STRAT condition a second time */
	I2C_GenerateSTART(CAPTS_I2C, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for read */
	I2C_Send7bitAddress(CAPTS_I2C, CAPTS_ADDR, I2C_Direction_Receiver);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	/* While there is data to be read */
	while(NumByteToRead)
	{
		if(NumByteToRead == 1)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(CAPTS_I2C, DISABLE);

			/* Send STOP Condition */
			I2C_GenerateSTOP(CAPTS_I2C, ENABLE);

			/* Test on EV7 and clear it */
			if(I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
			{
				/* Read a byte from the EEPROM */
				*pBuffer = I2C_ReceiveData(CAPTS_I2C);

				/* Point to the next location where the byte read will be saved */
				pBuffer++;

				/* Decrement the read bytes counter */
				NumByteToRead--;
			}
			else
			{
				if (retries++ > 1000)
				{
					HumanInput_CapTS_Reset();

					I2C_DeInit(CAPTS_I2C);
					CapTS_I2C_Configuration();

					return;
				}
			}
		}
		else
		{
			/* Test on EV7 and clear it */
			if(I2C_CheckEvent(CAPTS_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
			{
				/* Read a byte from the EEPROM */
				*pBuffer = I2C_ReceiveData(CAPTS_I2C);

				/* Point to the next location where the byte read will be saved */
				pBuffer++;

				/* Decrement the read bytes counter */
				NumByteToRead--;
			}
		}
	}

	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(CAPTS_I2C, ENABLE);
}

/******************************************************************************/
void CapTS_GetMaxMin(void)
{
	if (TPR_Structure.x[0] > CapTS_Touch_Max.xPos)
	{
		CapTS_Touch_Max.xPos = TPR_Structure.x[0];
	}

	if (TPR_Structure.y[0] > CapTS_Touch_Max.yPos)
	{
		CapTS_Touch_Max.yPos = TPR_Structure.y[0];
	}

	if (TPR_Structure.x[0] < CapTS_Touch_Min.xPos)
	{
		CapTS_Touch_Min.xPos = TPR_Structure.x[0];
	}

	if (TPR_Structure.y[0] < CapTS_Touch_Min.yPos)
	{
		CapTS_Touch_Min.yPos = TPR_Structure.y[0];
	}
}

/******************************************************************************/
void CapTS_EnterDeepSleep(void)
{
	uint8 tmp;

	tmp = 0x03;
	CapTS_Write_Regsister(0xA5, &tmp, 1);
}

/******************************************************************************/
void CapTS_ExitDeepSleep(void)
{
//	uint8 tmp;
//
//	tmp = 0x00;
//	CapTS_Write_Regsister(0xA5, &tmp, 1); /* I2C no response under deepsleep */

	GPIO_ResetBits(CAPTS_RST_PORT, CAPTS_RST_PIN);
	Delay_SW(0xFFF);
	GPIO_SetBits(CAPTS_RST_PORT, CAPTS_RST_PIN);
	Delay_SW(0xFFFF);
}

/******************************************************************************/
void HumanInput_CapTS_Process(void)
{
	u8 i = 0;
	u8 sta = 0;
	u8 buf[4] = {0};

	/* Read status */
	CapTS_Read_Regsister(0x02, &sta, 1);

	/* Judge touched or not, the lower 4bit stands for the touch number */
 	if(sta & 0x0f)
 	{
 		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));
 		for(i=0;i<5;i++)
 		{
 			if(TPR_Structure.TouchSta & (1<<i))
 			{
 				/* Read touch position */
 				CapTS_Read_Regsister(CapTS_TPX_TBL[i],buf,4);
				TPR_Structure.x[i] = ((u16)(buf[0]&0X0F)<<8) + buf[1];
				TPR_Structure.y[i] = ((u16)(buf[2]&0X0F)<<8) + buf[3];

 				if((buf[0]&0XC0)!=0X80)
 				{
					TPR_Structure.x[i] = TPR_Structure.y[i] = 0;//必须是contact事件，才认为有效
					return;
				}
 			}
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;
 	}
 	else
 	{
 		/* No touch */
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN)
 		{
 			TPR_Structure.TouchSta &= ~TP_PRES_DOWN;

 			/* Valid click */
			if (((CapTS_Touch_Max.xPos - CapTS_Touch_Min.xPos) < CAPTS_CLICK_MAX_DISTANCE)
					&& ((CapTS_Touch_Max.yPos - CapTS_Touch_Min.yPos) < CAPTS_CLICK_MAX_DISTANCE))
			{
				xPos = CapTS_Touch_TD.xPos;
				yPos = CapTS_Touch_TD.yPos;
			}

 			/* Lift off */
 			TPR_Structure.TouchSta |= TP_PRES_LIFT;

 			/* Clear touch number */
 			TouchCount = 0;
 		}
 		else
 		{
 			TPR_Structure.x[0] = 0;
 			TPR_Structure.y[0] = 0;
 			xPos = yPos = 0;
 			TPR_Structure.TouchSta &=0xe0;
 		}
 	}

 	/* Touchdown process */
 	if (TPR_Structure.TouchSta &= TP_PRES_DOWN)
 	{
 		if (TouchCount == 0)
 		{
 			/* First touch down */
 			CapTS_Touch_TD.xPos = TPR_Structure.x[0];
 			CapTS_Touch_TD.yPos = TPR_Structure.y[0];
 			CapTS_Touch_Max.xPos = TPR_Structure.x[0];
 			CapTS_Touch_Max.yPos = TPR_Structure.y[0];
 			CapTS_Touch_Min.xPos = TPR_Structure.x[0];
 			CapTS_Touch_Min.yPos = TPR_Structure.y[0];
 		}
 		else
 		{
 			/* Subsequent touch */
 			CapTS_GetMaxMin();
 		}

 		/* Increase touch number */
 		TouchCount++;

 		if (UI_MODE_DEBUG == UI_runMode)
 			DisplayDriver_DrawPoint(TPR_Structure.x[0], TPR_Structure.y[0], RED);
 	}
}

/******************************************************************************/
void HumanInput_TouchScreen_Process(void)
{
	/* Capacitive touchscreen */
	if(TPR_Structure.TouchSta & TP_COORD_UD)
	{
		/* Clear flag */
		TPR_Structure.TouchSta &= ~TP_COORD_UD;

		/* Read touch position from touch screen controller */
		HumanInput_CapTS_Process();

	}
}
