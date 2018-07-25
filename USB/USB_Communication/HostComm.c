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
//	USART_ITConfig(HOSTCOMM_USART, USART_IT_RXNE, ENABLE);
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
