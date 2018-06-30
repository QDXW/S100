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
uint8 HostComm_RecBufAvailable = 0;
uint16 HostComm_RecBufSize;

uint16 cmdLength;
uint8 cmdType;
uint8 cmdCode;

uint8 recBuffer[SIZE_REC_BUFFER];
uint8 cmdBuffer[SIZE_CMD_BUFFER];
uint8 respBuffer[SIZE_RESP_BUFFER];

uint8 contReceive = 0;
uint16 recCount = 0;

uint16 respLength = 0;

uint8 keyMatched = 0;

struct binFileAttr binFileAttr_Input;
extern RTC_DATA SystemManage_SetTime;

/******************************************************************************/
static uint8 HostComm_Cmd_Process(void);
static uint16 HostComm_Cmd_Respond_Status(void);
// static uint16 HostComm_Cmd_Respond_CRC_Error(uint8 cmdCode);
static uint16 HostComm_Cmd_Respond_APP_SysInfo(void);
static uint16 HostComm_Cmd_Respond_APP_LaunchBL(void);
static uint16 HostComm_Cmd_Respond_APP_ReadIDCard(void);
static uint16 HostComm_Cmd_Respond_APP_WriteIDCard(void);
static uint16 HostComm_Cmd_Respond_APP_SetTime(void);
static uint16 HostComm_Cmd_Respond_APP_SetMode(void);
static uint16 HostComm_Cmd_Respond_APP_SetMFG(void);
static uint16 HostComm_Cmd_Respond_APP_SetLanguage(void);
static uint16 HostComm_Cmd_Respond_APP_SetPrinter(void);
static uint16 HostComm_Cmd_Respond_APP_SetOutFab(void);
static uint16 HostComm_Cmd_Respond_APP_StartStop_ReadRecords(void);
static uint16 HostComm_Cmd_Respond_APP_ReadRecords_PerDate(void);
static uint16 HostComm_Cmd_Respond_APP_SetBLEName(void);
static uint16 HostComm_Cmd_Respond_APP_SetInstitute(void);
static uint16 HostComm_Cmd_Respond_APP_ClearIDCard(void);
static uint16 HostComm_Cmd_Respond_APP_SetOutputData(void);
static uint16 HostComm_Cmd_Respond_APP_ReadResistor(void);
static uint16 HostComm_Cmd_Respond_APP_WriteResistor(void);
static uint16 HostComm_Cmd_Respond_APP_SetCalcParameters(void);
static uint16 HostComm_Cmd_Respond_APP_Error(uint8 cmdCode);
void HostComm_SendResp(uint8 *Data, uint16 length);
char *itoa(int32 value, char *string, int radix);

/******************************************************************************/
static void HostComm_NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/*Enable interrupt*/
	NVIC_InitStructure.NVIC_IRQChannel = HOSTCOMM_USART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
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
void HostComm_Process(void)
{
	if (HostComm_RecBufAvailable)
	{
		/* Receive host command */
		HostComm_RecBufAvailable = 0;
		HostComm_Cmd_Process();
	}
}

/******************************************************************************/
void HostComm_Send_LIS(uint8 *data)
{
	uint8 i=0;
	while(*(data+i)!='\0')
	{
		i++;
	}
	HostComm_SendThrUSB(i,data);

	Delay_ms_SW(5);
}

/******************************************************************************/
void HostComm_SendThrUSB(uint16 len, uint8 *srcPtr)
{
	TxData.len = len;
	memcpy(TxData.Data, srcPtr, len);
	Comm_RequestTX = 1;
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
			break;
		case CMD_CODE_APP_LAUNCH_BL:
			responseLength = HostComm_Cmd_Respond_APP_LaunchBL();
			break;
		case CMD_CODE_APP_READ_IDCARD:
			responseLength = HostComm_Cmd_Respond_APP_ReadIDCard();
			break;
		case CMD_CODE_APP_WRITE_IDCARD:
			responseLength = HostComm_Cmd_Respond_APP_WriteIDCard();
			break;
		case CMD_CODE_APP_SET_TIME:
			responseLength = HostComm_Cmd_Respond_APP_SetTime();
			break;
		case CMD_CODE_APP_SET_MODE:
			responseLength = HostComm_Cmd_Respond_APP_SetMode();
			break;
		case CMD_CODE_APP_SET_MFG:
			responseLength = HostComm_Cmd_Respond_APP_SetMFG();
			break;
		case CMD_CODE_APP_SET_LANGUAGE:
			responseLength = HostComm_Cmd_Respond_APP_SetLanguage();
			break;
		case CMD_CODE_APP_SET_PRINTER:
			responseLength = HostComm_Cmd_Respond_APP_SetPrinter();
			break;
		case CMD_CODE_APP_SET_OUT_FAB:
			responseLength = HostComm_Cmd_Respond_APP_SetOutFab();
			break;
		case CMD_CODE_APP_READ_RESISTOR:
			responseLength = HostComm_Cmd_Respond_APP_ReadResistor();
			break;
		case CMD_CODE_APP_WRITE_RESISTOR:
			responseLength = HostComm_Cmd_Respond_APP_WriteResistor();
			break;
		case CMD_CODE_APP_SET_CALC_PARAMETERS:
			responseLength = HostComm_Cmd_Respond_APP_SetCalcParameters();
			break;
#if !RL_A3000
		case CMD_CODE_APP_START_STOP_READ_RECORD:
			responseLength = HostComm_Cmd_Respond_APP_StartStop_ReadRecords();
			break;
		case CMD_CODE_APP_READ_RECORDS_PERDATE:
			responseLength = HostComm_Cmd_Respond_APP_ReadRecords_PerDate();
			break;
#endif
#if BLE_4_0_ENABLED
		case CMD_CODE_APP_SET_BLE_NAME:
			responseLength = HostComm_Cmd_Respond_APP_SetBLEName();
			break;
#endif
#if YINBO_CUSTOMIZATION_ENABLED
		case CMD_CODE_APP_SET_INSTITUTE:
			responseLength = HostComm_Cmd_Respond_APP_SetInstitute();
			break;
		case CMD_CODE_APP_CLEAR_IDCARD:
			responseLength = HostComm_Cmd_Respond_APP_ClearIDCard();
			break;
#endif
		case CMD_CODE_APP_SET_OUTPUT_DATA:
			responseLength = HostComm_Cmd_Respond_APP_SetOutputData();
			break;
		default:
			responseLength = HostComm_Cmd_Respond_APP_Error(cmdCode);
			break;
		}
	}
	else if (cmdType == CMD_TYPE_NONE)
	{
		/* Get status information */
		responseLength = HostComm_Cmd_Respond_Status();
	}
	else
	{
		/* None BL command: respond error */
		responseLength = HostComm_Cmd_Respond_APP_Error(cmdCode);
	}

	/* Send data */
	HostComm_SendThrUSB(responseLength, &respBuffer[0]);

	/* Special case: Launch BL. Launch BL after data is sent. */
	if (cmdCode == CMD_CODE_APP_LAUNCH_BL)
	{
		/* Set to BL mode */
//		BL_RUN_TYPE = BL_RUN_TYPE_BL;
		/* Software reset */
		SoftReset();
	}
	return rc;
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
static uint16 HostComm_Cmd_Respond_APP_SetMode(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	/*  */
	if (cmdBuffer[OFFSET_CMD_DATA_RX] == 1)
	{
//		UI_runMode = UI_MODE_DEBUG;
	}
	else
	{
//		UI_runMode = UI_MODE_NORMAL;
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
//	uint16 cmdDataLength = 0;
//
//	/* SN: 20161031001 */
//	memcpy(UI_MFG, &cmdBuffer[OFFSET_CMD_DATA_RX], 8);
//
//	STMFlash_Write(FLASH_SET_MFG_ADDR, UI_MFG, 4);
//
//	UI_MFG_SN = (UI_MFG[1] << 16) + UI_MFG[0];
//	UI_MFG_FW1 = 0x00FF & UI_MFG[2];
//	UI_MFG_FW2 = (0xFF00 & UI_MFG[2]) >> 8;
//	UI_MFG_FW3 = UI_MFG[3];
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_SET_MFG);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetLanguage(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//
//	/*  */
//	memcpy(&UI_Language, &cmdBuffer[OFFSET_CMD_DATA_RX], 2);
//
//	STMFlash_Write(FLASH_SET_LANGUAGE_ADDR, &UI_Language, 1);
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_SET_LANGUAGE);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetPrinter(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//
//	/*  */
//	memcpy(&UI_Printer, &cmdBuffer[OFFSET_CMD_DATA_RX], 2);
//
//	STMFlash_Write(FLASH_SET_PRINTER_ADDR, &UI_Printer, 1);
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_SET_PRINTER);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetOutFab(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//
//	uint16 value = 0;
//
//	/* Set out fab calibration value, then user can recover it */
//	value = SignalSample_resistorValueStored;
//	STMFlash_Write(FLASH_SET_OUT_FAB, &value, 1);
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_SET_OUT_FAB);

	return totalPackageLength;
}

#if !RL_A3000
/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_StartStop_ReadRecords(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//
//	if (1 == cmdBuffer[OFFSET_CMD_DATA_RX])
//	{
//		/* Start */
//		/* Only read records under MAIN interface */
//		if (UI_state == UI_STATE_MAIN_WINDOW)
//		{
//			UI_dataTransfer = UI_DATA_TRANSFER_ON;
//			respBuffer[OFFSET_CMD_DATA] = 1;
//		}
//		else
//		{
//			respBuffer[OFFSET_CMD_DATA] = 0;
//		}
//	}
//	else if (0 == cmdBuffer[OFFSET_CMD_DATA_RX])
//	{
//		/* Stop */
//		UI_dataTransfer = UI_DATA_TRANSFER_OFF;
//		respBuffer[OFFSET_CMD_DATA] = 2;
//	}
//
//	cmdDataLength = 1;
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_START_STOP_READ_RECORD);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_ReadRecords_PerDate(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//
//	memcpy(&SystemManage_RecordTime, &cmdBuffer[OFFSET_CMD_DATA_RX], 4);
//
//	if (UI_DATA_TRANSFER_ON == UI_dataTransfer)
//	{
//		/* Under data transfer mode */
//		/* Look up date firstly */
//		if (Storage_HaveRecordOrNot(&SystemManage_RecordTime))
//		{
//			/* Have data */
//			respBuffer[OFFSET_CMD_DATA] = 1;
//			cmdDataLength = 1;
//			UI_readRecord_NewDate = 1;
//		}
//		else
//		{
//			/* No data */
//			respBuffer[OFFSET_CMD_DATA] = 2;
//			cmdDataLength = 1;
//			/* Automatically exit data transfer mode */
//			UI_dataTransfer = UI_DATA_TRANSFER_OFF;
//		}
//	}
//	else
//	{
//		/* Not under data transfer mode */
//		respBuffer[OFFSET_CMD_DATA] = 0;
//		cmdDataLength = 1;
//		/* Automatically exit data transfer mode */
//		UI_dataTransfer = UI_DATA_TRANSFER_OFF;
//	}
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_READ_RECORDS_PERDATE);

	return totalPackageLength;
}

/******************************************************************************/
void HostComm_Report_ReadRecords(HOSTCOMM_REPORT_ATTR data)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = sizeof(HOSTCOMM_REPORT_ATTR);

	memcpy(&respBuffer[OFFSET_CMD_DATA], &data, cmdDataLength);

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_READ_RECORD);

	/* Send data */
	HostComm_SendThrUSB(totalPackageLength, &respBuffer[0]);
}

/******************************************************************************/
void HostComm_Report_ReadRecords_LastOne(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_READ_RECORD_LASTONE);

	/* Send data */
	HostComm_SendThrUSB(totalPackageLength, &respBuffer[0]);
}
#endif

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
static uint16 HostComm_Cmd_Respond_APP_LaunchBL(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_LAUNCH_BL);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_ReadIDCard(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//	uint8 dataLengthIDCard = 0;
//
//	/* Read 256 bytes from ID card */
//	if (IDCard_HostRead())
//	{
//		/* Indicate ID card read success */
//		respBuffer[OFFSET_CMD_DATA] = 1;
//
//		/* Get real data length */
////		dataLengthIDCard = IDCARD_HEAD_LENGTH +
////				8 * IDCard_rxBuffer[IDCARD_POINTS_OFFSET];
//		dataLengthIDCard = 0xFF;
//
//		/* Only move valid data to buffer */
//		memcpy(&respBuffer[OFFSET_CMD_DATA + 1], IDCard_rxBuffer,
//				dataLengthIDCard);
//
//		/* Data format: status + data in ID card */
//		cmdDataLength = 1 + dataLengthIDCard;
//	}
//	else
//	{
//		/* Indicate ID card doesn't exist */
//		respBuffer[OFFSET_CMD_DATA] = 0;
//		cmdDataLength = 1;
//	}
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_READ_IDCARD);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_WriteIDCard(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//	uint8 dataLengthIDCard = 0;
//
//	/* Write to ID card */
//	if (IDCard_IsCardInserted())
//	{
//		/* Clear buffer */
//		memset(IDCard_txBuffer, 0x00, 0xFF);
//
//		/* Clear E2 before write data */
//		if (!IDCard_HostWrite())
//		{
//			/* Get valid data length */
////			dataLengthIDCard = IDCARD_HEAD_LENGTH +
////					8 * cmdBuffer[OFFSET_CMD_DATA_RX + IDCARD_POINTS_OFFSET];
//			dataLengthIDCard = 0xFF;
//
//			/* Move valid data */
//			memcpy(IDCard_txBuffer, &cmdBuffer[OFFSET_CMD_DATA_RX], dataLengthIDCard);
//
//			/* Execute write operation */
//			IDCard_HostWrite();
//
//			respBuffer[OFFSET_CMD_DATA] = 1;
//		}
//		else
//		{
//			respBuffer[OFFSET_CMD_DATA] = 0;
//		}
//	}
//	else
//	{
//		respBuffer[OFFSET_CMD_DATA] = 0;
//	}
//
//	cmdDataLength = 1;
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_WRITE_IDCARD);

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

//	memcpy(&SignalSample_caliStdRatioCT,
//			&cmdBuffer[OFFSET_CMD_DATA_RX + 7], sizeof(float));

	/* Set time */
	SystemManage_RTC_Set(SystemManage_SetTime.year,
			SystemManage_SetTime.month, SystemManage_SetTime.day,
			SystemManage_SetTime.hour, SystemManage_SetTime.min,
			SystemManage_SetTime.sec);

	/* Read time */
	SystemManage_RTC_Get();

//	/* Update time */
//	UI_Process_Update_StatusBar();

//	/* Clear head of record area */
//	Storage_Clear();

	/* Record set time */
	memcpy(dataBuf, &SystemManage_SetTime, sizeof(RTC_DATA));
//	STMFlash_Write(FLASH_SET_TIME_ADDR, dataBuf, sizeof(RTC_DATA) >> 1);
//
//	/* Store standard ratio of CT */
//	memcpy(floatBuf, &SignalSample_caliStdRatioCT,
//			sizeof(SignalSample_caliStdRatioCT));
//	STMFlash_Write(FLASH_CALI_STD_RATIOCT_ADDR, floatBuf, 2);

	/* Respond to host */
	respBuffer[OFFSET_CMD_DATA] = 1;

	cmdDataLength = 1;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_TIME);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetBLEName(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint8 dataLengthIDCard = 0;

//	BLE_Send_Phone("AT+NAME=RL-A2000");
	Delay_ms_SW(100);
//	BLE_Send_Phone("AT+NAME=RL-A2000");

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_BLE_NAME);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_ReadResistor(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//	uint16 value = 0;
//
//	/* Read from flash */
//	STMFlash_Read(FLASH_CALI_RESULT_ADDR, &value, 1);
//	SignalSample_resistorValue = value;
//	SignalSample_resistorValueStored = value;
//
//	respBuffer[OFFSET_CMD_DATA] = (uint8)value;
//	cmdDataLength = 1;
//
//	/*  */
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_READ_RESISTOR);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_WriteResistor(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//	uint16 value = 0;
//
//	/*  */
//	if (cmdBuffer[OFFSET_CMD_DATA_RX] <= 0xFF)
//	{
//		value = CALI_VALID;
//		STMFlash_Write(FLASH_CALI_STATUS_ADDR, &value, 1);
//		value = cmdBuffer[OFFSET_CMD_DATA_RX];
//		STMFlash_Write(FLASH_CALI_RESULT_ADDR, &value, 1);
//		SignalSample_digitalResistor = value;
//		SignalSample_resistorValue = value;
//		SignalSample_resistorValueStored = value;
//	}
//
//	/*  */
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_WRITE_RESISTOR);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetCalcParameters(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//	uint16 posInfo[7];
//	uint16 posInfoStatus = 1;
//
//	memcpy(&SignalProcess_Alg_data.posInfo, &cmdBuffer[OFFSET_CMD_DATA_RX], sizeof(STRUCT_POS_INFO));
//	memcpy(&posInfo[0], &SignalProcess_Alg_data.posInfo, 13);
//
//	STMFlash_Write(FLASH_CALC_POSINFO_STATUS_ADDR, &posInfoStatus, 1);
//	STMFlash_Write(FLASH_CALC_POSINFO_DATA_ADDR, &posInfo[0], 7);
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_SET_CALC_PARAMETERS);

	return totalPackageLength;
}

#if YINBO_CUSTOMIZATION_ENABLED
/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetInstitute(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint8 dataLengthIDCard = 0;
	uint8 index = 0;

	/*  */
	memcpy(&UI_instituteName[0], &cmdBuffer[OFFSET_CMD_DATA_RX], 20);
	STMFlash_Write(FLASH_YINBO_PRINT_INSTITUTE_ADDR, (uint16*)UI_instituteName, 10);

	/* Respond to host */
	respBuffer[OFFSET_CMD_DATA] = 1;

	cmdDataLength = 1;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_INSTITUTE);

	return totalPackageLength;
}

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_ClearIDCard(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint8 dataLengthIDCard = 0;

	/* Clear ID card storage */
	Storage_IDCard_ClearNum();

	/* Respond to host */
	respBuffer[OFFSET_CMD_DATA] = 1;
	cmdDataLength = 1;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_CLEAR_IDCARD);

	return totalPackageLength;
}
#endif

/******************************************************************************/
static uint16 HostComm_Cmd_Respond_APP_SetOutputData(void)
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;
	uint8 dataLengthIDCard = 0;

	SignalProcess_output = 1;

	/* Respond to host */
	respBuffer[OFFSET_CMD_DATA] = 1;
	cmdDataLength = 1;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SET_OUTPUT_DATA);

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
void HostComm_Cmd_Output_C_T(uint8 length, uint8 dataBuf[])
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	memcpy(&respBuffer[OFFSET_CMD_DATA], dataBuf, length);
	cmdDataLength = length;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SEND_C_T);

	/* Send data */
	HostComm_SendThrUSB(totalPackageLength, &respBuffer[0]);
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
//void HostComm_Cmd_Send_RawData(uint8 size, uint8 *ptr)
//{
//	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
//	uint16 cmdDataLength = 0;
//
//	respBuffer[OFFSET_CMD_DATA] = size;
//	memcpy(&respBuffer[OFFSET_CMD_DATA + 1], ptr, size << 1);
//	cmdDataLength = 1 + size;
//
//	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
//			CMD_TYPE_APP, CMD_CODE_APP_SEND_RAWDATA);
//
//	/* Send data */
//	HostComm_SendThrUSB(totalPackageLength, &respBuffer[0]);
//}

// /******************************************************************************/
// static uint16 HostComm_Cmd_Respond_CRC_Error(uint8 cmdCode)
// {
// 	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
// 	uint16 cmdDataLength = 0;

// 	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
// 			CMD_TYPE_APP, cmdCode | CRC_ERROR_MASK);

// 	return totalPackageLength;
// }

/******************************************************************************
!! ISR: Host communication interrupt service routine
******************************************************************************/
//void USART1_IRQHandler(void)
//{
//	uint8 value;
//	if(USART_GetITStatus(HOSTCOMM_USART, USART_IT_RXNE) != RESET)
//	{
//		value=USART_ReceiveData(HOSTCOMM_USART);
//
//		if ((value == '$') && (contReceive == 0))
//		{
//			contReceive = 1;
//			recCount = 0;
//			recBuffer[recCount++] = value;
//		}
//		else
//		{
//			recBuffer[recCount++] = value;
//		}
//
//		/* Get the package length: header + data + tail */
//		if (recCount >= SIZE_HEAD_LEN)
//		{
//			respLength =
//					(recBuffer[OFFSET_LEN_LO] | (recBuffer[OFFSET_LEN_HI] << 8))
//					+ SIZE_HEAD_TAIL;
//		}
//		else
//		{
//			respLength = 0;
//		}
//
//		/* Receive all: start processing response data */
//		if ((respLength > 0) && (recCount >= respLength))
//		{
//			HostComm_RecBufAvailable = 1;
//			HostComm_RecBufSize = recCount;
//
//			memcpy(cmdBuffer, recBuffer, recCount);
//			memset(recBuffer, 0, recCount);
//
//			contReceive = 0;
//			respLength = 0;
//			recCount = 0;
//		}
//	}
//}

/******************************************************************************/
void HostComm_SendResp(uint8 *Data, uint16 length)
{
	while(length-- != 0) {
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
void HostComm_Send(USART_TypeDef* USARTx, uint8_t *Data,...)
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
	uint8 value;
	if(USART_GetITStatus(HOSTCOMM_USART, USART_IT_RXNE) != RESET)
	{
		value=USART_ReceiveData(HOSTCOMM_USART);

		if ((value == '$') && (contReceive == 0))
		{
			contReceive = 1;
			recCount = 0;
			recBuffer[recCount++] = value;
		}
		else
		{
			recBuffer[recCount++] = value;
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

			memcpy(cmdBuffer, &recBuffer[3], recCount - 4);
			memset(recBuffer, 0, recCount);

			contReceive = 0;
			respLength = 0;
			recCount = 0;
		}
	}
}

/******************************************************************************/
void HostComm_Cmd_Send_RawData(uint16 length, uint8 dataBuf[])
{
	uint16 totalPackageLength = SIZE_HEAD_TAIL; /* Include head and tail */
	uint16 cmdDataLength = 0;

	memcpy(&respBuffer[OFFSET_CMD_DATA], dataBuf, length);
	cmdDataLength = length;

	totalPackageLength += HostComm_Cmd_Respond_Common(cmdDataLength,
			CMD_TYPE_APP, CMD_CODE_APP_SEND_C_T);

	HostComm_SendResp(&respBuffer[0], totalPackageLength);
}
