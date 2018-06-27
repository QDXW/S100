/*
 * QRCode.c
 *
 *  Created on: 2018年3月7日
 *      Author: Administrator
 */

/******************************************************************************/
#include "QRCode.h"
#include "DisplayDriver.h"

QRCODE_STRUCT QR_Date;
uint8 QRCode_received = 0;
uint16 QRCode_count = 0;
extern uint16 SignalProcess_sampleBuffer[512];
uint8 QRCode_Buffer[QRCODE_BUFFER_SIZE] = 0;

/******************************************************************************/
void QRCode_Init(void)
{
	QRCode_GPIO_Init();

	QRCode_Uart_Init();
}

/******************************************************************************/
void QRCode_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  QRCODE_TRIG_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(QRCODE_TRIG_PORT, &GPIO_InitStructure);

	QRCode_Trigger_Disabled();
}

/******************************************************************************/
void QRCode_Uart_Init(void)
{
#if LCD_OLD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#else
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#endif
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);

	QRCode_Uart_GPIO_Init();					/* Initialize GPIOs */
	QRCode_Uart_NVIC_Configuration();			/* Enable interrupt */
	QRCode_Uart_Config();						/* Configure parameters */
}

/******************************************************************************/
void QRCode_Uart_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/*Enable interrupt*/
	NVIC_InitStructure.NVIC_IRQChannel = QRCODE_USART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
void QRCode_Uart_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*Configure USART2 Tx as alternate function push-pull*/
	GPIO_InitStructure.GPIO_Pin = QRCODE_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(QRCODE_TX_PORT, &GPIO_InitStructure);

	/*Configure USART2 Rx as input floating*/
	GPIO_InitStructure.GPIO_Pin = QRCODE_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(QRCODE_RX_PORT, &GPIO_InitStructure);
}

/******************************************************************************/
void QRCode_Uart_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure */
	USART_Init(QRCODE_USART, &USART_InitStructure);

	/* Enable Receive and Transmit interrupts */
	USART_ITConfig(QRCODE_USART, USART_IT_RXNE, ENABLE);

	/* Enable */
	USART_Cmd(QRCODE_USART, ENABLE);

	/* To avoid the first byte missing issue */
	USART_ClearFlag(QRCODE_USART, USART_FLAG_TC);
}

/******************************************************************************/
void QRCode_Trigger_Enabled(void)
{
	GPIO_ResetBits(QRCODE_TRIG_PORT, QRCODE_TRIG_PIN);
}

/******************************************************************************/
void QRCode_Trigger_Disabled(void)
{
	GPIO_SetBits(QRCODE_TRIG_PORT, QRCODE_TRIG_PIN);
}

/******************************************************************************/
void QRCode_Restart(void)
{
	QRCode_Trigger_Disabled();
	Delay_ms_SW(1000);
	QRCode_Trigger_Enabled();
}

/******************************************************************************/
#if LCD_OLD
void USART1_IRQHandler(void)
#else
void USART3_IRQHandler(void)
#endif
{
	uint8 value;

	if(USART_GetITStatus(QRCODE_USART, USART_IT_RXNE) != RESET)
	{
		value=USART_ReceiveData(QRCODE_USART);

		if (value != '\r')
		{
			QRCode_Buffer[QRCode_count++] = value;
		}
		else
		{
			QRCode_received = 1;
			QRCode_existed = 1;

		}
	}

	QRCode_Received();
	USART_ClearITPendingBit(QRCODE_USART, USART_IT_RXNE);	//清除USART1/USART3上的中断标志位
}

/******************************************************************************/
void QRCode_Received(void)
{
/* Scan interface: receive data from QR code scanner */
	if (QRCode_received == 1)
	{
		QRCode_received = 0;

		if (QRCode_Identify())						/* Decode */
		{
			QRCode_existed = 1;
		}
		else
		{
			Display_Time = 0;
			Lcd_ColorBox(5, 142,35,15,BACKCOLOR_CONTENT_BACK);
			Lcd_ColorBox(7,62,114, 78,White);
			DisplayDriver_Text16_B(27, 75, Red, White, "Invalid QR");
			DisplayDriver_Text16_B(47, 95, Red, White, "Code!");
			DisplayDriver_Text16_B(20, 115,Red,White,"Scan Again!");
			Display_Time = 1;
			QRCode_existed = 0;
		}
		QRCode_count = 0;							/* Clear size */
	}
}

/******************************************************************************/
uint8 QRCode_Identify(void)
{
	uint8 status = 0;
	uint16 crcCalc = 0;
	uint16 crcRec = 0;
	uint8 singleLineSize = 0;
	uint8 headLineSize = 0;

	status = 0;
	QRCode_received = 1;

	/* Calculate CRC */
	crcCalc = Common_CalculateCRC(&QRCode_Buffer[2], QRCode_count - 2, 0xFFFF, 0x0000);

	/* Get CRC from command */
	crcRec = (QRCode_Buffer[1] << 8) | QRCode_Buffer[0];

	if (crcRec == crcCalc)
	{
		memset(&QR_Date, 0, sizeof(QRCODE_STRUCT));
		memset(&Storage_Data, 0, sizeof(STORAGE_SINGLE_DATA_STRUCT));
		memcpy(&QR_Date.head.name[0], &QRCode_Buffer[2], sizeof(QRCODE_HEAD_STRUCT) - 2);
		singleLineSize = sizeof(QRCODE_SINGLE_LINE);
		headLineSize = sizeof(QRCODE_HEAD_STRUCT);
		memcpy(&Storage_Data.Product_name[0], &QR_Date.head.name[0], sizeof(QR_Date.head.name));
		memcpy(&Storage_Data.Product_SN[0], &QR_Date.head.SN[0], sizeof(QR_Date.head.SN));
		if(QR_Date.head.stripNum > 8)
			QR_Date.head.stripNum = 8;
		Cup_Count = QR_Date.head.stripNum;
		Storage_Data.StripNum = QR_Date.head.stripNum;

		switch (QR_Date.head.stripNum)
		{
			case 12:
				memcpy(&QR_Date.ch12_data, &QRCode_Buffer[headLineSize + 11 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[11], &QRCode_Buffer[headLineSize + 11 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 11:
				memcpy(&QR_Date.ch11_data, &QRCode_Buffer[headLineSize + 10 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[10], &QRCode_Buffer[headLineSize + 10 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 10:
				memcpy(&QR_Date.ch10_data, &QRCode_Buffer[headLineSize + 9 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[9], &QRCode_Buffer[headLineSize + 9 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 9:
				memcpy(&QR_Date.ch9_data, &QRCode_Buffer[headLineSize + 8 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[8], &QRCode_Buffer[headLineSize + 8 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 8:
				memcpy(&QR_Date.ch8_data, &QRCode_Buffer[headLineSize + 7 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[7], &QRCode_Buffer[headLineSize + 7 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 7:
				memcpy(&QR_Date.ch7_data, &QRCode_Buffer[headLineSize + 6 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[6], &QRCode_Buffer[headLineSize + 6 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 6:
				memcpy(&QR_Date.ch6_data, &QRCode_Buffer[headLineSize + 5 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[5], &QRCode_Buffer[headLineSize + 5 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 5:
				memcpy(&QR_Date.ch5_data, &QRCode_Buffer[headLineSize + 4 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[4], &QRCode_Buffer[headLineSize + 4 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 4:
				memcpy(&QR_Date.ch4_data, &QRCode_Buffer[headLineSize + 3 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[3], &QRCode_Buffer[headLineSize + 3 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 3:
				memcpy(&QR_Date.ch3_data, &QRCode_Buffer[headLineSize + 2 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[2], &QRCode_Buffer[headLineSize + 2 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 2:
				memcpy(&QR_Date.ch2_data, &QRCode_Buffer[headLineSize + singleLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[1], &QRCode_Buffer[headLineSize + singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 1:
				memcpy(&QR_Date.ch1_data, &QRCode_Buffer[headLineSize], sizeof(QRCODE_SINGLE_LINE));
				memcpy(&Storage_Data.CH_data[0], &QRCode_Buffer[headLineSize], sizeof(QRCODE_SINGLE_LINE));
				break;
			default:
				break;
		}

		QRCode_received = 1;
		QRCode_existed = 1;
		status = 1;
	}
	else
	{
		QRCode_received = 1;
		QRCode_existed = 0;
		status = 0;
	}
	QRCode_Trigger_Disabled();
	return status;
}

/******************************************************************************/
void Clear_Data(void)
{
	Cup_Count = 0;
	memset(&QR_Date, 0, sizeof(QRCODE_STRUCT));
	memset(&QRCode_Buffer, 0, sizeof(QRCode_Buffer));
	memset(&Storage_Data, 0, sizeof(STORAGE_SINGLE_DATA_STRUCT));
	memset(&SignalProcess_sampleBuffer, 0, sizeof(SignalProcess_sampleBuffer));
}
