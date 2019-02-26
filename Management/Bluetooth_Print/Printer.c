/*******************************************************************************
 * File name: Print.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
/******************************************************************************/
#include "Printer.h"

/******************************************************************************/
bool BLEConnectStaus = FALSE;
uint8 buffer[40] = {0};
uint8 tbuf[40] = {0};

/******************************************************************************/
PRINT_CONTENT Printer_Result;

/******************************************************************************/
static void Printer_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/*Configure USART3 Tx as alternate function push-pull*/
	GPIO_InitStructure.GPIO_Pin = PRINTER_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PRINTER_TX_PORT, &GPIO_InitStructure);

	/*Configure USART3 Rx as input floating*/
	GPIO_InitStructure.GPIO_Pin = PRINTER_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(PRINTER_RX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  PRINTER_MATCH_STATUS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(PRINTER_MATCH_STATUS_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOE, GPIO_Pin_4);
	GPIO_ResetBits(PRINTER_MATCH_STATUS_PORT, PRINTER_MATCH_STATUS_PIN);
}

/******************************************************************************/
static void Printer_Config()
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	/* Configure */
	USART_Init(PRINTER_USART, &USART_InitStructure);
	/* Enable Receive and Transmit interrupts */
//	USART_ITConfig(PRINTER_USART, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(HOSTCOMM_USART, USART_IT_TXE, ENABLE);
	/* Enable */
	USART_Cmd(PRINTER_USART, ENABLE);
	/* To avoid the first byte missing issue */
	USART_ClearFlag(PRINTER_USART, USART_FLAG_TC);
}

/******************************************************************************/
void BLE_Timer_Init(uint16 period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_DeInit(TIM7);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = period*10 - 1; /* 1mS */
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	BLE_Timer_Disabled();
}

/******************************************************************************/
void Printer_BLE_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/*Initialize GPIOs*/
	Printer_GPIO_Init();
	/*Configure parameters*/
	Printer_Config();
}

/******************************************************************************/
void BLE_Timer_Disabled(void)
{
	/* Disabled timer */
	TIM_Cmd(TIM7, DISABLE);
	/* Disabled interrupt */
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}

/******************************************************************************/
void BLE_Timer_Enabled(void)
{
	/* Clear count */
	TIM7->CNT = 0;
	/* Enable timer */
	TIM_Cmd(TIM7, ENABLE);
	/* Clear interrupt flag */
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	/* Enable interrupt */
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}

/******************************************************************************/

void SetBLEStaus(bool STAUS)
{
	BLEConnectStaus=STAUS;
}

/******************************************************************************/
void Set_BLE_Exit_STATE(FunctionalState STATE)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = STATE;
	EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************/
uint8 Printer_isConnected(void)
{
	return GPIO_ReadInputDataBit(PRINTER_MATCH_STATUS_PORT,
				PRINTER_MATCH_STATUS_PIN);
}

/******************************************************************************/
void Printer_BLE_PrintOneLine(uint8 *content)
{
	while (*content != '\0')
	{
		USART_SendData(PRINTER_USART, *content++);
		while(USART_GetFlagStatus(PRINTER_USART, USART_FLAG_TC) == RESET);
	}
}

/******************************************************************************/
void BLE_Send_Phone(uint8 *data)
{
	while (*data != '\0')
	{
		USART_SendData(PRINTER_USART, *data++);
		while(USART_GetFlagStatus(PRINTER_USART, USART_FLAG_TC) == RESET);
	}
}

/******************************************************************************/
void Printer_BLE_Print(STORAGE_SINGLE_DATA_STRUCT *content)
{
	uint8 index = 0,index1 = 0;
	memset(buffer,0,sizeof(buffer));
	memset(tbuf,0,sizeof(buffer));
	PCF8563_Read(&SystemManage_CurrentTime);
	sprintf(tbuf,"%d/%02d/%02d %02d:%02d:%02d",SystemManage_CurrentTime.year,
			SystemManage_CurrentTime.month,SystemManage_CurrentTime.day,SystemManage_CurrentTime.hour,
			SystemManage_CurrentTime.min,SystemManage_CurrentTime.sec);

	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		/* Head */
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("------------Realy Tech----------\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		/* Device type */
		Printer_BLE_PrintOneLine("Device Type: RL-S100\r\n");

		buffer[35] = '\0';
		memcpy(&buffer[0], "Product name: ",14);
		memcpy(&buffer[14], content->Product_name,20);
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[15] = '\0';
		memcpy(&buffer[0], "SN: ",sizeof("SN: "));
		memcpy(&buffer[4], content->Product_SN,sizeof(content->Product_SN));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[17] = '\0';
		memcpy(&buffer[0], "Time: ",sizeof("Time: "));
		memcpy(&buffer[6],tbuf,sizeof(tbuf));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));


		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("   Item         Result\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");

		for (index = 0; index < Storage_Data.StripNum; index++)
		{
			buffer[23] = '\0';
			memcpy(&buffer[0],"   ",sizeof("   "));
			memcpy(&buffer[3],content->CH_data[index].TName,8);
			for(index1 = 3;index1 < 11;index1++)
			{
				if(buffer[index1] == '\0')
				{
					buffer[index1] = ' ';
				}
			}
			memcpy(&buffer[11],"      ",sizeof("      "));
			memcpy(&buffer[17],content->CH_data[index].Result,sizeof(content->CH_data[index].Result));
			Printer_BLE_PrintOneLine(&buffer[0]);
			Printer_BLE_PrintOneLine("\r\n");
			memset(buffer,0,sizeof(buffer));
		}
		break;

	case DISPLAY_FONT_CHINESE:
		/* Head */
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("------------Realy Tech----------\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		/* Device type */
		Printer_BLE_PrintOneLine("设备名: RL-S100\r\n");

		memcpy(&buffer[0], "产品名称: ",sizeof("产品名称: "));
		memcpy(&buffer[10], content->Product_name,sizeof( content->Product_name));
		Printer_BLE_PrintOneLine(&buffer[0]);
		buffer[35] = '\0';
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[15] = '\0';
		memcpy(&buffer[0], "批号: ",sizeof("批号: "));
		memcpy(&buffer[6], content->Product_SN,sizeof(content->Product_SN));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[17] = '\0';
		memcpy(&buffer[0], "时间: ",sizeof("时间: "));
		memcpy(&buffer[6],tbuf,sizeof(tbuf));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("   名称          结果\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");

		for (index = 0; index < Storage_Data.StripNum; index++)
		{
			buffer[23] = '\0';
			memcpy(&buffer[0],"   ",sizeof("   "));
			memcpy(&buffer[3],content->CH_data[index].TName,8);
			for(index1 = 3;index1 < 11;index1++)
			{
				if(buffer[index1] == '\0')
				{
					buffer[index1] = ' ';
				}
			}
			memcpy(&buffer[11],"      ",sizeof("      "));
			memcpy(&buffer[17],content->CH_data[index].Result,sizeof(content->CH_data[index].Result));
			Printer_BLE_PrintOneLine(&buffer[0]);
			Printer_BLE_PrintOneLine("\r\n");
			memset(buffer,0,sizeof(buffer));
		}
		break;

	case DISPLAY_FONT_GERMAN:
		/* Head */
		Printer_BLE_PrintOneLine("---------------------------------\r\n");
		Printer_BLE_PrintOneLine("------------Ulti  Med  ----------\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		/* Device type */
		Printer_BLE_PrintOneLine("Typ: RL-S100\r\n");

		buffer[35] = '\0';
		memcpy(&buffer[0], "Produkt name: ",14);
		memcpy(&buffer[14], content->Product_name,20);
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[15] = '\0';
		memcpy(&buffer[0], "SN: ",sizeof("SN: "));
		memcpy(&buffer[4], content->Product_SN,sizeof(content->Product_SN));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[17] = '\0';
		memcpy(&buffer[0], "Zeit: ",sizeof("Zeit: "));
		memcpy(&buffer[6],tbuf,sizeof(tbuf));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("   Analyt      Ergebnis\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");

		for (index = 0; index < Storage_Data.StripNum; index++)
		{
			buffer[23] = '\0';
			memcpy(&buffer[0],"   ",sizeof("   "));
			memcpy(&buffer[3],content->CH_data[index].TName,8);
			for(index1 = 3;index1 < 11;index1++)
			{
				if(buffer[index1] == '\0')
				{
					buffer[index1] = ' ';
				}
			}
			memcpy(&buffer[11],"      ",sizeof("      "));
			memcpy(&buffer[17],content->CH_data[index].Result,sizeof(content->CH_data[index].Result));
			Printer_BLE_PrintOneLine(&buffer[0]);
			Printer_BLE_PrintOneLine("\r\n");
			memset(buffer,0,sizeof(buffer));
		}
		break;

	default:
		break;
	}

	/* Tail */
	Printer_BLE_PrintOneLine("--------------------------------\r\n");
	Printer_BLE_PrintOneLine("\r\n");
	Printer_BLE_PrintOneLine("\r\n");
}

/******************************************************************************/
void UI_Process_BLE_Print(void)
{
	Printer_BLE_Print(&Storage_Data);
}

