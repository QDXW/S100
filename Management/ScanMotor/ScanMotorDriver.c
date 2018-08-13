/*
 * ScanScanMotorDriver.c
 *
 *  Created on: 2018Äê3ÔÂ2ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#include "ScanMotorDriver.h"

/******************************************************************************/
unsigned int CCR2_Val = 45000;
uint8 ScanMotorDriver_InBasePosition = 0;
uint8 MotorDriver_Ctr = 0;

/******************************************************************************/
void Delay_SW(__IO uint32 nCount)
{
	for(; nCount != 0; nCount--);
}

/******************************************************************************/
void ScanMotorDriver_NVIC_Configuration() {
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
void ScanMotorDriver_A1(void) {
	GPIO_ResetBits(ScanMotorDriver_IN4_PORT, ScanMotorDriver_IN4_PIN);
	GPIO_SetBits(ScanMotorDriver_IN3_PORT, ScanMotorDriver_IN3_PIN);
	GPIO_SetBits(ScanMotorDriver_IN2_PORT, ScanMotorDriver_IN2_PIN);
	GPIO_ResetBits(ScanMotorDriver_IN1_PORT, ScanMotorDriver_IN1_PIN);
}

/******************************************************************************/
void ScanMotorDriver_A2(void) {
	GPIO_ResetBits(ScanMotorDriver_IN4_PORT, ScanMotorDriver_IN4_PIN);
	GPIO_SetBits(ScanMotorDriver_IN3_PORT, ScanMotorDriver_IN3_PIN);
	GPIO_ResetBits(ScanMotorDriver_IN2_PORT, ScanMotorDriver_IN2_PIN);
	GPIO_SetBits(ScanMotorDriver_IN1_PORT, ScanMotorDriver_IN1_PIN);
}

/******************************************************************************/
void ScanMotorDriver_B1(void) {
	GPIO_SetBits(ScanMotorDriver_IN4_PORT, ScanMotorDriver_IN4_PIN);
	GPIO_ResetBits(ScanMotorDriver_IN3_PORT, ScanMotorDriver_IN3_PIN);
	GPIO_ResetBits(ScanMotorDriver_IN2_PORT, ScanMotorDriver_IN2_PIN);
	GPIO_SetBits(ScanMotorDriver_IN1_PORT, ScanMotorDriver_IN1_PIN);
}

/******************************************************************************/
void ScanMotorDriver_B2(void) {
	GPIO_SetBits(ScanMotorDriver_IN4_PORT, ScanMotorDriver_IN4_PIN);
	GPIO_ResetBits(ScanMotorDriver_IN3_PORT, ScanMotorDriver_IN3_PIN);
	GPIO_SetBits(ScanMotorDriver_IN2_PORT, ScanMotorDriver_IN2_PIN);
	GPIO_ResetBits(ScanMotorDriver_IN1_PORT, ScanMotorDriver_IN1_PIN);
}

/******************************************************************************/
void ScanMotorDriver_Control(uint8 enabled) {
	if (enabled)
	{
		GPIO_SetBits(ScanMotorDriver_EN_PORT, ScanMotorDriver_EN_PIN);
	}
	else
	{
		GPIO_ResetBits(ScanMotorDriver_EN_PORT, ScanMotorDriver_EN_PIN);

	}
}

/******************************************************************************/
void ScanMotorDriver_MoveOneStep(uint8 dir) {
#define MOTOR_QUICK_DELAY	 0X2000
#define MOTOR_SLOW_DELAY 	 0X3000

	uint8 index = 0;
	ScanMotorDriver_Control(MOTOR_ENABLED);
	while (index < 4)
	{
		if (dir == ScanMotorDriver_DIR_IN)
		{
			/* Direction: IN */
			switch (index)
			{
			case 0:
				ScanMotorDriver_A2();
				index++;
			break;
			case 1:
				ScanMotorDriver_A1();
				index++;
			break;
			case 2:
				ScanMotorDriver_B2();
				index++;
			break;
			case 3:
				ScanMotorDriver_B1();
				index++;
			break;

			default:
			break;
			}
		}
		else
		{
			/* Direction: OUT */
			switch (index)
			{
			case 0:
				ScanMotorDriver_A1();
				index++;
			break;
			case 1:
				ScanMotorDriver_A2();
				index++;
			break;
			case 2:
				ScanMotorDriver_B1();
				index++;
			break;
			case 3:
				ScanMotorDriver_B2();
				index++;
			break;

			default:
			break;
			}
		}
		/* Delay a while for next phase */
		Delay_SW(MOTOR_SLOW_DELAY);
	}
	ScanMotorDriver_Control(MOTOR_DISABLED);
}

/******************************************************************************/
void ScanMotorDriver_Move(uint8 direction, uint32 steps)
{
	/* Goto base position */
	if (direction == ScanMotorDriver_DIR_IN)
	{
		/* Move until slider reaches base position */
		while (!CAN_POSSEN_INT_STATE())
		{
			ScanMotorDriver_MoveOneStep(ScanMotorDriver_DIR_IN);
		}
	}
	else if ((direction == ScanMotorDriver_DIR_OUT) && (steps > 0))
	{
			/* Move out per steps */
			while (steps--)
			{
				ScanMotorDriver_MoveOneStep(ScanMotorDriver_DIR_OUT);
			}
	}
	else
	{
			/* Do nothing */
	}
}

/******************************************************************************/
void ScanMotorDriver_GPIO_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* PE6: MOTOR2 EN */
	GPIO_InitStructure.GPIO_Pin = ScanMotorDriver_EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ScanMotorDriver_EN_PORT, &GPIO_InitStructure);

	/* PA4  5  PB0  1£ºIN1  2  3  4 */
	GPIO_InitStructure.GPIO_Pin = ScanMotorDriver_IN1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ScanMotorDriver_IN1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ScanMotorDriver_IN2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ScanMotorDriver_IN2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ScanMotorDriver_IN3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ScanMotorDriver_IN3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ScanMotorDriver_IN4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ScanMotorDriver_IN4_PORT, &GPIO_InitStructure);

	ScanMotorDriver_Control(MOTOR_DISABLED);
}

/******************************************************************************/
void ScanMotorDriver_Init(void) {
	/* Initialize position sensor input pin */
	ScanMotorDriver_PositionSensor_Init();
	/* Initialize motor driver pins */
	ScanMotorDriver_GPIO_Init();
}

/******************************************************************************/
void ScanMotorDriver_Goto_BasePosition(void) {
	if (CAN_POSSEN_INT_STATE())
	{
		return;
	}
	else
	{
		/* Not in base position */
		ScanMotorDriver_Move(ScanMotorDriver_DIR_IN, 0);
	}
}

/******************************************************************************/
void ScanMotorDriver_Goto_DetectionPosition(void) {
	/* Move to detection position */
	ScanMotorDriver_Move(ScanMotorDriver_DIR_OUT, MOTOR_TOTAL_STEPS);
}

/******************************************************************************/
void ScanMotorDriver_Goto_CentrePosition(void)
{
	/* Move to detection position */
	ScanMotorDriver_Move(ScanMotorDriver_DIR_OUT, MOTOR_HALF_STEPS);
}

/******************************************************************************/
void ScanMotorDriver_StartDetection(void) {
	/* Goto base position */
	ScanMotorDriver_Goto_BasePosition();
//	Delay_ms(200);
	/* Goto detection start position */
	ScanMotorDriver_Goto_DetectionPosition();
}

/******************************************************************************/
void ScanMotorDriver_SelfCheck_StepDrive(void)
{
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_DetectionPosition();
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_DetectionPosition();
}

/******************************************************************************/
void SystemManage_5V_Enabled(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_6);
	MotorDriver_Ctr = 1;
}

/******************************************************************************/
void SystemManage_5V_Disabled(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_6);
	MotorDriver_Ctr = 0;
}
