/*******************************************************************************
 * File name: HostComm.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#include "HostComm.h"
#include "Common.h"
#include "SignalProcess_Sample.h"
#include "stmflash.h"
#include "Comm_FIFO.h"
#include "comDef.h"
#include "SystemManage_RTC.h"
#include "stdarg.h"
#include <string.h>

/******************************************************************************/
uint8 cmdType,cmdCode;
uint8 recBuffer[SIZE_REC_BUFFER];
uint8 cmdBuffer[SIZE_CMD_BUFFER];
uint8 HostComm_RecBufAvailable = 0;
uint8 respBuffer[SIZE_RESP_BUFFER];
uint8 contReceive = 0,keyMatched = 0;
uint16 UI_MFG_FW1 = 0,UI_MFG_FW2 = 0,UI_MFG_FW3 = 0;
uint16 HostComm_RecBufSize,cmdLength,UI_MFG[4] = {0};
uint16 recCount = 0,respLength = 0,UI_Language = 0,UI_MFG_SN = 0;

/******************************************************************************/
uint8 HostComm_Cmd_Process(void);
static uint16 HostComm_Cmd_Respond_Status(void);
static uint16 HostComm_Cmd_Respond_APP_SET_5V(void);
static uint16 HostComm_Cmd_Respond_APP_SetMFG(void);
static uint16 HostComm_Cmd_Respond_APP_SysInfo(void);
static uint16 HostComm_Cmd_Respond_APP_SetMode(void);
uint16 HostComm_Cmd_Respond_APP_Error(uint8 cmdCode);
static uint16 HostComm_Cmd_Respond_APP_SetTime(void);
static uint16 HostComm_Cmd_Respond_APP_QRCode_Data(void);
static uint16 HostComm_Cmd_Respond_APP_SetLanguage(void);
static uint16 HostComm_Cmd_Respond_APP_Calibration(void);
static uint16 HostComm_Cmd_Respond_APP_ReadBoundary(void);
static uint16 HostComm_Cmd_Respond_APP_ReadResistor(void);
static uint16 HostComm_Cmd_Respond_APP_WriteBoundary(void);
static uint16 HostComm_Cmd_Respond_APP_WriteResistor(void);
static uint16 HostComm_Cmd_Respond_APP_Clear_Record(void);
static uint16 HostComm_Cmd_Respond_APP_Seek_Record(void);

/******************************************************************************/
void HostComm_SendResp(uint8 *Data, uint16 length);
char *itoa(int32 value, char *string, int radix);

/******************************************************************************/
static void HostComm_NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/*Enable interrupt*/
	NVIC_InitStructure.NVIC_IRQChannel = HOSTCOMM_USART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
static void HostComm_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*Configure USART1 Tx as alternate function push-pull*/
	GPIO_InitStructure.GPIO_Pin = HOSTCOMM_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HOSTCOMM_TX_PORT, &GPIO_InitStructure);

	/*Configure USART1 Rx as input floating*/
	GPIO_InitStructure.GPIO_Pin = HOSTCOMM_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HOSTCOMM_RX_PORT, &GPIO_InitStructure);
}

/******************************************************************************/
static void HostComm_Config()
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
	USART_Init(HOSTCOMM_USART, &USART_InitStructure);
	/* Enable Receive and Transmit interrupts */
	USART_ITConfig(HOSTCOMM_USART, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(HOSTCOMM_USART, USART_IT_TXE, ENABLE);
	/* Enable */
	USART_Cmd(HOSTCOMM_USART, ENABLE);
	/* To avoid the first byte missing issue */
	USART_ClearFlag(HOSTCOMM_USART, USART_FLAG_TC);
}

/******************************************************************************/
void HostComm_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/*Initialize GPIOs*/
	HostComm_GPIO_Init();
	/*Enable interrupt*/
	HostComm_NVIC_Configuration();
	/*Configure parameters*/
	HostComm_Config();
}

/******************************************************************************/
void HostComm_SendThrUSB(uint16 len, uint8 *srcPtr)
{
	TxData.len = len;
	memcpy(TxData.Data, srcPtr, len);
	Comm_RequestTX = 1;
}

/******************************************************************************/
static uint8 HiByte(uint16 value)
{
	return (uint8)((value & 0xFF00) >> 8);
}

/******************************************************************************/
static uint8 LoByte(uint16 value)
{
	return (uint8)(value & 0x00FF);
}

/******************************************************************************/
void HostComm_Process(void)
{
	if(HostComm_RecBufAvailable)
	{
		HostComm_RecBufAvailable = 0;
		HostComm_Cmd_Process();
	}
}

/******************************************************************************/
uint8 HostComm_Cmd_Process(void)
{
	uint16 responseLength = 0;
	uint8 rc = 0;

	/* Command type */
	cmdType = cmdBuffer[OFFSET_CMD_TYPE_RX];
	/* Command code */
	cmdCode = cmdBuffer[OFFSET_CMD_CODE_RX];

	if (cmdType == CMD_TYPE_APP)
	{
		switch (cmdCode)
		{
		case CMD_CODE_APP_SYSINFO:
			responseLength = HostComm_Cmd_Respond_APP_SysInfo();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_SET_TIME:
			responseLength = HostComm_Cmd_Respond_APP_SetTime();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_SET_MODE:
			responseLength = HostComm_Cmd_Respond_APP_SetMode();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_SET_MFG:
			responseLength = HostComm_Cmd_Respond_APP_SetMFG();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_SET_LANGUAGE:
			responseLength = HostComm_Cmd_Respond_APP_SetLanguage();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_SET_OUT_FAB:
//			responseLength = HostComm_Cmd_Respond_APP_SetOutFab();
			break;

		case CMD_CODE_APP_READ_RESISTOR:
			responseLength = HostComm_Cmd_Respond_APP_ReadResistor();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_WRITE_RESISTOR:
			responseLength = HostComm_Cmd_Respond_APP_WriteResistor();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_SEND_QRCODE_DATA:
			responseLength = HostComm_Cmd_Respond_APP_QRCode_Data();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_READ_BOUNDARY:
			responseLength = HostComm_Cmd_Respond_APP_ReadBoundary();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_WRITE_BOUNDARY:
			responseLength = HostComm_Cmd_Respond_APP_WriteBoundary();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case CMD_CODE_APP_CALIBRATION:
			responseLength = HostComm_Cmd_Respond_APP_Calibration();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case APP_SET_5V:
			responseLength = HostComm_Cmd_Respond_APP_SET_5V();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case APP_CLEAR_RECORD:
			responseLength = HostComm_Cmd_Respond_APP_Clear_Record();
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;

		case APP_SEEK_RECORD:
			responseLength = HostComm_Cmd_Respond_APP_Seek_Record();
			break;

		default:
			responseLength = HostComm_Cmd_Respond_APP_Error(cmdCode);
			/* Send data */
			HostComm_SendResp(&respBuffer[0], responseLength);
			break;
		}
	}
	else if (cmdType == CMD_TYPE_NONE)
	{
		/* Get status information */
		responseLength = HostComm_Cmd_Respond_Status();
		/* Send data */
		HostComm_SendResp(&respBuffer[0], responseLength);
	}
	else
	{
		/* None BL command: respond error */
		responseLength = HostComm_Cmd_Respond_APP_Error(cmdCode);
		/* Send data */
		HostComm_SendResp(&respBuffer[0], responseLength);
	}

	return rc;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_Common(uint16 cmdDataNum, uint8 cmdType,
		uint8 cmdCode)
{
	uint16 packageLength = SIZE_LEN_HEAD_CMD_CRC + cmdDataNum;
	uint16 crcCal = 0;

	/* Head */
	respBuffer[OFFSET_HEADER] = '$';

	/* Package length */
	respBuffer[OFFSET_LEN_LO] = LoByte(packageLength);
	respBuffer[OFFSET_LEN_HI] = HiByte(packageLength);

	/* Command type and code */
	respBuffer[OFFSET_CMD_TYPE] = cmdType;
	respBuffer[OFFSET_CMD_CODE] = cmdCode;

	/* Calculate CRC
	 * 1. &cmdBuffer[1]: because the first byte is '$' which is not included
	 *    in CRC calculation
	 * 2. cmdLength - 2: because cmdLength includes 2-byte CRC */
	crcCal = Common_CalculateCRC(&respBuffer[1], packageLength - 2, 0xFFFF,
									0x0000);

	/* CRC */
	respBuffer[OFFSET_CMD_DATA + cmdDataNum] = LoByte(crcCal);
	respBuffer[OFFSET_CMD_DATA + cmdDataNum + 1] = HiByte(crcCal);

	/* Tail */
	respBuffer[OFFSET_CMD_DATA + cmdDataNum + 2] = '#';

	return packageLength;
}

/******************************************************************************/
void HostComm_Cmd_Send_C_T(uint16 CValue, uint16 TValue)
{
	uint8 dataBuf[8];
	uint8 dataSize;

	memcpy(&dataBuf[0], &CValue, 2);
	memcpy(&dataBuf[2], &TValue, 2);
	dataSize = 4;

	/* Send to HOST */
	HostComm_Cmd_Send_RawData(dataSize, &dataBuf[0]);
}

/******************************************************************************/
void HostComm_SendResp(uint8 *Data, uint16 length)
{
	USART1->SR;
	while((length--) != 0)
	{
		USART_SendData(HOSTCOMM_USART, *Data++);
		while(USART_GetFlagStatus(HOSTCOMM_USART, USART_FLAG_TC)==RESET);
	}
}

/******************************************************************************/
void HostComm_Send_Char(u8 data)
{
	/* Send one byte through HOSTCOMM_USART */
	USART_SendData(HOSTCOMM_USART, (u8) data);
	/* Wait while HOSTCOMM_USART TXE = 0 */
	while(USART_GetFlagStatus(HOSTCOMM_USART, USART_FLAG_TXE) == RESET)
	{
	}
}

/******************************************************************************/
void HostComm_Send_String(u8 *strPtr)
{
	while(*strPtr)
	{
		HostComm_Send_Char(*strPtr);
		strPtr++;
	}
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_Status(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	/* Status:
	 * 0: BL
	 * 1: APP
	 *  */
	respBuffer[OFFSET_CMD_DATA] = 1;

	cmdDataLength = 1;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_NONE, CMD_CODE_STATUS);

	return totalPackageLength;
}

/******************************************************************************/
uint16 HostComm_Cmd_Respond_APP_Error(uint8 cmdCode)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, cmdCode | CMD_ERROR_MASK);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetOutFab(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	uint16 value = 0;

	/* Set out fab calibration value, then user can recover it */
	value = SignalSample_resistorValueStored;
	STMFlash_Write(FLASH_SET_OUT_FAB, &value, 1);

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_OUT_FAB);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetMode(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	/*  */
	if (cmdBuffer[OFFSET_CMD_DATA_RX] == 1)
	{
		UI_runMode = UI_MODE_DEBUG;
	}
	else
	{
		UI_runMode = UI_MODE_NORMAL;
	}
//	STMFlash_Write(FLASH_SET_MODE_ADDR, &UI_runMode, 1);

	/*  */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_MODE);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetMFG(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint8 MBuffer[12] = {0};

	/* SN: 20161031001 */
	memcpy(&Data_SN, &cmdBuffer[OFFSET_CMD_DATA_RX], 12);
	sprintf(MBuffer,"20%d",Data_SN);

	Storage_Write(MBuffer, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*2),12);

//	Display_Time = 0;
//	DisplayDriver_Text16_B(4, 90, Black,White,MBuffer);
//	Display_Time = 1;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_MFG);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetLanguage(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0,Value = 0;

	/*  */
	memcpy(&Font_Switch, &cmdBuffer[OFFSET_CMD_DATA_RX], 1);
	Storage_Write(&Font_Switch, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*3),1);

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_LANGUAGE);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SysInfo(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	/* Product ID */
	respBuffer[OFFSET_CMD_DATA] = 1;
	respBuffer[OFFSET_CMD_DATA + 1] = 0;

	/* Version information */
	respBuffer[OFFSET_CMD_DATA + 2] = LoByte(APP_REVISION);
	respBuffer[OFFSET_CMD_DATA + 3] = HiByte(APP_REVISION);
	respBuffer[OFFSET_CMD_DATA + 4] = APP_VERSION_MINOR;
	respBuffer[OFFSET_CMD_DATA + 5] = APP_VERSION_MAJOR;

	cmdDataLength = 6;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SYSINFO);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetTime(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint16 dataBuf[10] = {0};

	memcpy(&SystemManage_SetTime, &cmdBuffer[OFFSET_CMD_DATA_RX],
			sizeof(RTC_DATA));

	/* Set time */
	SystemManage_RTC_Set(SystemManage_SetTime.year,
			SystemManage_SetTime.month, SystemManage_SetTime.day,
			SystemManage_SetTime.hour, SystemManage_SetTime.min,
			SystemManage_SetTime.sec);

	/* Update time */
	UI_Draw_Status_Bar();

	/* Respond to host */
	respBuffer[OFFSET_CMD_DATA] = 1;

	cmdDataLength = 1;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_TIME);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_ReadResistor(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; 	/* Include head and tail */
	uint16 cmdDataLength = 0;
	uint8 value = 0;
	uint8 MBuffer[10] = {0};

	/* Read from flash */
	Storage_Read(&value,FLASH_CALI_ADDR,1);
	SignalSample_resistorValue = value;
	SignalSample_resistorValueStored = value;

	respBuffer[OFFSET_CMD_DATA] = value;
	cmdDataLength = 1;

	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_READ_RESISTOR);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_ReadBoundary(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; 	/* Include head and tail */
	uint16 cmdDataLength = 0;
	uint16 Boundary = 0;
	uint8 value[3] = {0};

	/* Read from flash */
	Storage_Read(value,(FLASH_CALI_ADDR+FLASH_OFFSET_ADDR),2);
	memcpy(&Boundary,value,2);
	Data_Boundary = Boundary;

	respBuffer[OFFSET_CMD_DATA] = LoByte(Boundary);
	respBuffer[OFFSET_CMD_DATA + 1] = HiByte(Boundary);
	cmdDataLength = 2;

	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_READ_BOUNDARY);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_WriteBoundary(void)
{

	uint16 totalPackageLength = SIZE_HEAD_TAIL; 	/* Include head and tail */
	uint16 cmdDataLength = 0;
	uint16 Boundary = 0;
	uint8 value[2] = {0};

	/* 数据打包 */
	if (cmdBuffer[OFFSET_CMD_DATA_RX] <= 0xFF)
	{
		value[0] = cmdBuffer[OFFSET_CMD_DATA_RX];
		value[1] = cmdBuffer[OFFSET_CMD_DATA_RX + 1];
		Storage_Write(value, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR),2);
		memcpy(&Boundary,value,2);
		Data_Boundary = Boundary;
	}

	cmdDataLength = 2;

	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_WRITE_BOUNDARY);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_WriteResistor(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint8 value = 0;
	uint8 MBuffer[10] = {0};

	/* 数据打包 */
	if (cmdBuffer[OFFSET_CMD_DATA_RX] <= 0xFF)
	{
		value = cmdBuffer[OFFSET_CMD_DATA_RX];
		Storage_Write(&value, FLASH_CALI_ADDR,1);
		SignalSample_resistorValue = value;
		SignalSample_resistorValueStored = value;
		/* Use it immediately */
		SignalSample_Sample_SetResistor();
	}

	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_WRITE_RESISTOR);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_QRCode_Data(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint16 value = 0;

	/* 数据打包 */
	if(Existed_Data)
	{
		cmdDataLength = sizeof(QRCODE_HEAD_STRUCT);
		respBuffer[OFFSET_CMD_DATA_RX] = 1;
		memcpy(&respBuffer[OFFSET_CMD_DATA_RX +1],&QR_Date,sizeof(QRCODE_HEAD_STRUCT));
	}
	else
	{
		cmdDataLength = 1;
		respBuffer[OFFSET_CMD_DATA_RX] = 0;
	}

	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SEND_QRCODE_DATA);

	return totalPackageLength;
}

/******************************************************************************/
uint16 HostComm_Cmd_Respond_APP_Calibration(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint16 value = 0;
	uint8 Record_Sleep_State = 0;

	/* 防止进入低功耗模式，并记录低功耗状态，关掉触摸 */
	Record_Sleep_State = Enter_Sleep;
	Enter_Sleep = 0,Calibration_channel = 0;
	MotorDriver_Ctr = 1;
	Display_Battery = 0;
	SYSCLKConfig_STOP();
	Exti_lock = DISABLE;

	cmdDataLength = 1;

	Dichotomy_Calculate();

	respBuffer[OFFSET_CMD_DATA_RX] = Result_Judge? 1: 0;

	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_CALIBRATION);

	/* 还原低功耗状态，启动触摸  */
	MotorDriver_Ctr = 0;
	Delay_ms_SW(1010);
	SYSCLKConfig_STOP();
	Enter_Sleep = Record_Sleep_State,Display_Battery = 1;

	return totalPackageLength;
}

/******************************************************************************/
uint16 HostComm_Cmd_Respond_APP_SET_5V(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	cmdDataLength = 1;

	respBuffer[OFFSET_CMD_DATA_RX] = cmdBuffer[OFFSET_CMD_DATA]? 1:0;

	if(cmdBuffer[OFFSET_CMD_DATA])
	{
		SystemManage_5V_Enabled();
	}
	else
	{
		SystemManage_5V_Disabled();
	}

	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, APP_SET_5V);

	return totalPackageLength;
}

/******************************************************************************/
uint16 HostComm_Cmd_Respond_APP_Clear_Record(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint8 MBuffer[12] = {0};
	uint16 insk[4] = {65535,65535,65535,65535};

	cmdDataLength = 1;

	respBuffer[OFFSET_CMD_DATA_RX] = 1;

	memcpy(MBuffer,insk,8);

	Storage_Write(MBuffer, 0x00, 8);

	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, APP_CLEAR_RECORD);

	return totalPackageLength;
}

/******************************************************************************/
uint16 HostComm_Cmd_Respond_APP_Seek_Record(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint16 Data_Size = 0,Information_Count = 0,Information_All = 0;
	uint8 Information[12] = 0;
	uint8 Device_S100 = 1;

	/* 读取记录时保持常亮  */
	Exti_lock = DISABLE;
	Stop_Mode = 0;
	GPIO_SetBits(GPIOD, GPIO_Pin_2);

	Storage_Read(Information,0x00,8);
	memcpy(&Information_Count,Information,2);
	memcpy(&Information_All,&Information[2],2);

	/* 发送总的测试次数  */
	memcpy(&respBuffer[OFFSET_CMD_DATA_RX],&Information_All,2);
	memcpy(&respBuffer[OFFSET_CMD_DATA_RX + 2],&Device_S100,1);
	cmdDataLength = 3;
	/* CRC校验 */
	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, APP_RECORD_COUNT);
	/* Send data */
	HostComm_SendResp(&respBuffer[0], totalPackageLength);

	cmdDataLength = sizeof(STORAGE_SINGLE_DATA_STRUCT) + 4;
	if((Information_Count != 0)&&(Information_Count != 65535))
	{
		for (Data_Size = 1;Data_Size <= Information_All;Data_Size++)
		{
			Power_Minute = 0;
			Delay_ms_SW(2);
			memset(respBuffer,0,sizeof(respBuffer));
			Storage_Read(Storage_writeBuffer,(Data_Size *4096),sizeof(STORAGE_SINGLE_DATA_STRUCT));
			memcpy(&respBuffer[OFFSET_CMD_DATA_RX],&Data_Size,2);
			memcpy(&respBuffer[OFFSET_CMD_DATA_RX + 2],&Information_All,2);
			memcpy(&respBuffer[OFFSET_CMD_DATA_RX + 4],Storage_writeBuffer,sizeof(STORAGE_SINGLE_DATA_STRUCT));

			/* CRC校验 */
			totalPackageLength = SIZE_HEAD_TAIL;
			totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
					CMD_TYPE_APP, APP_SEEK_RECORD);

			/* Send data */
			HostComm_SendResp(&respBuffer[0], totalPackageLength);
		}
	}
	Exti_lock = ENABLE;
	return 0;
}

/******************************************************************************/
void HostComm_Send(USART_TypeDef* USARTx, uint8 *Data,...)
{
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //end of data string
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //enter key
					USART_SendData(USARTx, 0x0d);

					Data++;
					break;
				case 'n':							          //change line key
					USART_SendData(USARTx, 0x0a);
					Data++;
					break;

				default:
					Data++;
				    break;
			}
		}
		else if(*Data=='%'){									  //
			switch (*++Data){
				case 's':										  //string
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //decimal
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}

/******************************************************************************/
char *itoa(int32 value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 1000000000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/******************************************************************************
!!! ISR: Host communication interrupt service routine
******************************************************************************/
void USART1_IRQHandler(void)
{
	uint8 RX_dat;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)//USART_IT_RXNE：接收中断
	{
		RX_dat=USART_ReceiveData(USART1);

		if ((RX_dat == '$') && (contReceive == 0))
		{
			contReceive = 1;
			recCount = 0;
			recBuffer[recCount++] = RX_dat;
		}
		else
		{
			recBuffer[recCount++] = RX_dat;
		}

		/* Get the package length: header + data + tail */
		if (recCount >= SIZE_HEAD_LEN)
		{
			respLength =
					(recBuffer[OFFSET_LEN_LO] | (recBuffer[OFFSET_LEN_HI] << 8))
					+ SIZE_HEAD_TAIL;
		}
		else
		{
			respLength = 0;
		}

		/* Receive all: start processing response data */
		if ((respLength > 0) && (recCount >= respLength))
		{
			HostComm_RecBufAvailable = 1;
			HostComm_RecBufSize = recCount;

			memcpy(cmdBuffer, recBuffer, recCount);
			memset(recBuffer, 0, recCount);
			contReceive = 0;
			respLength = 0;
			recCount = 0;
		}
	}
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

/******************************************************************************/
void HostComm_Cmd_Send_RawData(uint16 length, uint8 dataBuf[])
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	memset(respBuffer,0,sizeof(respBuffer));
	memcpy(&respBuffer[OFFSET_CMD_DATA], dataBuf, length);
	cmdDataLength = length;
	if(length >10)
	{
		totalPackageLength = SIZE_HEAD_TAIL;
		totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
							CMD_TYPE_APP, CMD_CODE_APP_SEND_RAWDATA);
	}
	else
	{
		totalPackageLength = SIZE_HEAD_TAIL;
		totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
					CMD_TYPE_APP, CMD_CODE_APP_SEND_C_T);
	}
	HostComm_SendResp(&respBuffer[0], totalPackageLength);
}

/******************************************************************************/
void Language_Valid (void)
{
	uint8 value[3] = {0};
	/* Read from flash */
	Storage_Read(value, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*3),2);

	Font_Switch = (value[0] == 0xFF)?DISPLAY_FONT_ENGLISH:(value[0]);
//	Font_Switch = DISPLAY_FONT_PROTUGAL;
}

/******************************************************************************/
void ReadResistor_Valid (void)
{
	uint8 value = 0;
	/* Read from flash */
	Storage_Read(&value,FLASH_CALI_ADDR,1);
	SignalSample_resistorValue = value;
	SignalSample_resistorValueStored = value;

	SignalSample_Sample_SetResistor();
}

/******************************************************************************/
void Set_Fixed_Parameter(void)
{
	ReadBoundary_Value();
	ReadResistor_Valid();
	Language_Valid();
	ReadBlutooth_Status();
}

/******************************************************************************/
void ReadBoundary_Value(void)
{
	uint16 Boundary = 0;
	uint8 value[2] = {0};

	/* Read from flash */
	Storage_Read(value,(FLASH_CALI_ADDR+FLASH_OFFSET_ADDR),2);
	memcpy(&Boundary,value,2);
	Data_Boundary = Boundary;
}

/******************************************************************************/
void ReadBlutooth_Status(void)
{
	uint16 Blutooth_status = 0;
	uint8 value[1] = {0};

	/* Read from flash */
	Storage_Read(value,(FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*4),1);
	memcpy(&Blutooth_status,value,1);
	Bluetooth_switch = Blutooth_status;
	if(!Bluetooth_switch)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_4);
		Bluetooth_switch = 1;
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_4);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		Bluetooth_switch = 0;
	}
}

/******************************************************************************/
void Read_SN (void)
{
	Storage_Read(data_SN, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*2),12);
}

/******************************************************************************/
void Send_QRCode (void)
{
	uint16 responseLength = 0;
	responseLength = HostComm_Cmd_Respond_APP_QRCode_Data();
	HostComm_SendResp(&respBuffer[0], responseLength);
}
