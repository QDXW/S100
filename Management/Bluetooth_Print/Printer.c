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
void UI_Process_BLE_Print(void)
{
	Printer_BLE_Print(&Storage_Data);
}

