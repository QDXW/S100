/*
 * QRCode.c
 *
 *  Created on: 2018年3月7日
 *      Author: Administrator
 */

/******************************************************************************/
#include "QRCode.h"
#include "DisplayDriver.h"

/******************************************************************************/
QRCODE_STRUCT QR_Date;
QRCODE_STRUCT QR_Date_Analyze;
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
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
#endif


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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
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
			QRCode_existed = 0;
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
		if ((QRCode_Identify()) && (3 == QR_Date.head.Model))						/* Decode */
		{
			QRCode_received = 1;
			QRCode_existed = 1;
			Send_QRCode();
		}
		else
		{
			QRCode_received = 1;
			QRCode_existed = 0;
			Display_Time = 0;
			Lcd_ColorBox(5, 142,35,15,BACKCOLOR_CONTENT_BACK);
			Lcd_ColorBox(7,62,114, 78,White);

			switch(Font_Switch)
			{
			case DISPLAY_FONT_ENGLISH:
				DisplayDriver_Text16_B(27, 75, Red, White, "Invalid QR");
				DisplayDriver_Text16_B(47, 95, Red, White, "Code");
				break;

			case DISPLAY_FONT_CHINESE:
				DisplayDriver_Text16_B(24, 85, Red, White, "无效二维码");
				break;

			default:
				break;
			}
			Display_Time = 1;

			/* 清空结构体    1、QR接收结构体   2、处理后QR结构体    3、存储结构体 */
			Cup_Count = 0;
			memset(&QR_Date, 0, sizeof(QRCODE_STRUCT));
			memset(&QR_Date_Analyze, 0, sizeof(QRCODE_STRUCT));
			memset(&Storage_Data, 0, sizeof(STORAGE_SINGLE_DATA_STRUCT));
		}
		QRCode_count = 0;							/* Clear size */
	}
}

/******************************************************************************/
uint8 QRCode_Identify(void)
{
	uint8 status = 0,QR_Date_Analyze_Conut = 0,QR_Date_Conut = 0;
	uint16 crcCalc = 0, headLineSize = 0, singleLineSize = 0,crcRec = 0;

	/* Calculate CRC */
	crcCalc = Common_CalculateCRC(&QRCode_Buffer[2], QRCode_count - 2, 0xFFFF, 0x0000);

	/* Get CRC from command */
	crcRec = (QRCode_Buffer[1] << 8) | QRCode_Buffer[0];

	if (crcRec == crcCalc)
	{
		/* 清空结构体    1、QR接收结构体   2、处理后QR结构体    3、存储结构体 */
		memset(&QR_Date, 0, sizeof(QRCODE_STRUCT));
		memset(&QR_Date_Analyze, 0, sizeof(QRCODE_STRUCT));
		memset(&Storage_Data, 0, sizeof(STORAGE_SINGLE_DATA_STRUCT));

		/* 产品头信息复制  */
		memcpy(&QR_Date.head.Model, &QRCode_Buffer[2], sizeof(QRCODE_HEAD_STRUCT) - 2);
		memcpy(&QR_Date_Analyze.head.name[0], &QRCode_Buffer[2], sizeof(QRCODE_HEAD_STRUCT) - 2);

		/* 结构体长度计算  */
		singleLineSize = sizeof(QRCODE_SINGLE_LINE);
		headLineSize = sizeof(QRCODE_HEAD_STRUCT);

		/* 存储结构体产品头信息复制  */
		memcpy(&Storage_Data.Product_name[0], &QR_Date.head.name[0], sizeof(QR_Date.head.name));
		memcpy(&Storage_Data.Product_SN[0], &QR_Date.head.SN[0], sizeof(QR_Date.head.SN));

		/* 头信息计算提取  */
		Action_time = QR_Date.head.time;
		Storage_Data.StripNum = QR_Date.head.stripNum;

		switch (8)
		{
			case 8:
				memcpy(&QR_Date.ch_data[7], &QRCode_Buffer[headLineSize + 7 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 7:
				memcpy(&QR_Date.ch_data[6], &QRCode_Buffer[headLineSize + 6 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 6:
				memcpy(&QR_Date.ch_data[5], &QRCode_Buffer[headLineSize + 5 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 5:
				memcpy(&QR_Date.ch_data[4], &QRCode_Buffer[headLineSize + 4 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 4:
				memcpy(&QR_Date.ch_data[3], &QRCode_Buffer[headLineSize + 3 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 3:
				memcpy(&QR_Date.ch_data[2], &QRCode_Buffer[headLineSize + 2 * singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 2:
				memcpy(&QR_Date.ch_data[1], &QRCode_Buffer[headLineSize + singleLineSize], sizeof(QRCODE_SINGLE_LINE));
			case 1:
				memcpy(&QR_Date.ch_data[0], &QRCode_Buffer[headLineSize], sizeof(QRCODE_SINGLE_LINE));
				break;

			default:
				break;
		}

		for(QR_Date_Conut = 0; QR_Date_Conut < 8; QR_Date_Conut++)
		{
			if(QR_Date.ch_data[QR_Date_Conut].Switch_Bool)
			{
				memcpy(&QR_Date_Analyze.ch_data[QR_Date_Analyze_Conut], &QR_Date.ch_data[QR_Date_Conut], sizeof(QRCODE_SINGLE_LINE));
				QR_Date_Analyze_Conut += 1;
			}
		}

		for(QR_Date_Conut = 0; QR_Date_Conut < 8; QR_Date_Conut++)
		{
			memcpy(&Storage_Data.CH_data[QR_Date_Conut], &QR_Date_Analyze.ch_data[QR_Date_Conut], sizeof(QRCODE_SINGLE_LINE));
		}
		Cup_Count = QR_Date_Analyze_Conut;
		Storage_Data.StripNum = Cup_Count;
		status = 1;
		Existed_Data = 1;
	}
	else
	{
		status = 0;
	}
	QRCode_Trigger_Disabled();
	return status;
}

/******************************************************************************/
void Clear_Data(void)
{
	Cup_Count = 0;
	Existed_Data = 0;
	memset(&QR_Date, 0, sizeof(QRCODE_STRUCT));
	memset(&QRCode_Buffer, 0, sizeof(QRCode_Buffer));
	memset(&Storage_Data, 0, sizeof(STORAGE_SINGLE_DATA_STRUCT));
	memset(&SignalProcess_sampleBuffer, 0, sizeof(SignalProcess_sampleBuffer));
}
