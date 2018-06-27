 /*
 * RotationMotorDriver_PositionSensor.c
 *
 *  Created on: 2018Äê3ÔÂ6ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "RotationMotorDriver_PositionSensor.h"

/******************************************************************************/
void RotaMotorDriver_PositionSensor_Int_Enable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = ROTA_POSSEN_INT_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	/* Enable */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************/
void RotaMotorDriver_PositionSensor_Int_Disable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = ROTA_POSSEN_INT_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	/* Disable */
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************/
void RotaMotorDriver_PositionSensor_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the EXTI9-5 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = ROTA_POSSEN_EXTI_CH;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_EXTILineConfig(ROTA_POSSEN_EXTI_PORT, ROTA_POSSEN_EXTI_PORT);

	EXTI_InitStructure.EXTI_Line = ROTA_POSSEN_INT_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Position sensor input */
	GPIO_InitStructure.GPIO_Pin = ROTA_POSSEN_INT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ROTA_POSSEN_INT_PORT, &GPIO_InitStructure);
}

