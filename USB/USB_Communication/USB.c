/*
 * USB.c
 *
 *  Created on: 2017Äê4ÔÂ20ÈÕ
 *      Author: Administrator
 */

#include "USB.h"
#include "Comm_FIFO.h"
#include "hw_config.h"
#include "HostComm.h"
#include "comDef.h"

/******************************************************************************/
void USB_BasicInit(void);

/******************************************************************************/
void USB_VirtualCOM_Init(void)
{
    USB_BasicInit();
    USB_Config();
	Comm_FIFO_Init(&RxDataFIFO);
}

/******************************************************************************/
void USB_BasicInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 9;
	TIM_TimeBaseStructure.TIM_Prescaler = 719;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2, DISABLE);
}

/******************************************************************************/
void USB_VirtualCOM_Process(void)
{
//	ErrorStatus Rxstatus;
//	DataTypedef RxData;
//	Rxstatus=Comm_FIFO_RxDataGet(&RxDataFIFO, &RxData);
//	if(Rxstatus)
//	{
//		HostComm_RecBufAvailable = 1;
//		memcpy(cmdBuffer, RxData.Data, RxData.len);
//	}
}
