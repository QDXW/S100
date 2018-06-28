/*
 * HumanInput.c
 *
 *  Created on: 2018��1��25��
 *      Author: Administrator
 ******************************************************************************/
#include "Font.h"
#include "HumanInput.h"
#include "DisplayDriver.h"
#include "RotationMotor.h"
#include "Version_selection.h"

/******************************************************************************/
extern uint8 Power_Open;
extern uint8 LED = 0;
extern uint8 key_fall_flag,short_key_flag,doubleClick,long_key_flag;

/*******************************************************************************
	��������HumanInput_Init
	��  ��: ��
	��  ��: ��
	����˵����ʹ�ܰ�����ӦGPIO�ܽ�
	��ʼ������Ӳ���豸��δ�����жϡ�
*******************************************************************************/
void HumanInput_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#if	 LCD_OLD
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);				//��ʼ��GPIOD2,3,4

#else
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				//��ʼ��GPIOC6,7
#endif

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//PA0���ó����룬Ĭ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				//��ʼ��GPIOB.0
}

/*******************************************************************************
	��������EXTIX_Init
	��  ��: ��
	��  ��: ��
	����˵����ʹ�ܰ�����Ӧ�ⲿ�ж�
	��ʼ������Ӳ���豸��δ�����жϡ�
*******************************************************************************/
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIOB.4	  �ж����Լ��жϳ�ʼ������ �����ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;  				//�м�ȷ�ϰ�ť
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

#if	 LCD_OLD
	//GPIOD.8 �ж����Լ��жϳ�ʼ������   �����ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource8);

	EXTI_InitStructure.EXTI_Line = EXTI_Line8;					//���Ұ�ť
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//GPIOD.9	  �ж����Լ��жϳ�ʼ������  �����ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource9);

	EXTI_InitStructure.EXTI_Line = EXTI_Line9;					//����ť
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
#else
	//GPIOC.6 �ж����Լ��жϳ�ʼ������   �����ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);

	EXTI_InitStructure.EXTI_Line = EXTI_Line6;					//���Ұ�ť
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//GPIOC.7	  �ж����Լ��жϳ�ʼ������  �����ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

	EXTI_InitStructure.EXTI_Line = EXTI_Line7;					//����ť
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
#endif

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
void EXTI_Key_Confirm_Disable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	//GPIOB.4	  �ж����Լ��жϳ�ʼ������ �����ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;  				//�м�ȷ�ϰ�ť
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************/
void EXTI_Key_Confirm_Enable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIOB.4	  �ж����Լ��жϳ�ʼ������ �����ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;  				//�м�ȷ�ϰ�ť
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
	��������KeyX_Work
	��  ��: ��
	��  ��: ��
	����˵������Ӧ��������
	��ʼ������Ӳ���豸��δ�����жϡ�
*******************************************************************************/
void Key_Confirm(void)
{
	if(Exti_lock)
	{
		if(short_key_flag)
		{
			if(doubleClick && Interface_Key == 1 )
			{
				key_state_confirm = 2;
				doubleClick = 0;
				EXTI_Key_Confirm_Enable();
			}
			else if(doubleClick && Interface_Key == 0)
			{
				if(!LED)
				{
					key_state_confirm = 0;
					doubleClick = 0;
					GPIO_SetBits(GPIOE, GPIO_Pin_5);   //�Ķ��ص�
					EXTI_Key_Confirm_Enable();
					LED = 1;
				}
				else
				{
					key_state_confirm = 0;
					doubleClick = 0;
					GPIO_ResetBits(GPIOE, GPIO_Pin_5);
					EXTI_Key_Confirm_Enable();
					LED = 0;
				}
			}
			else
			{
				key_state_confirm = ENABLE;
				EXTI_Key_Confirm_Enable();
			}
			key_state = ENABLE;
			short_key_flag = 0;
		}

		if(UI_state != UI_STATE_TESTING && Power_Open && long_key_flag)
		{
			if(Interface_Key == 2)
			{
				long_key_flag = 0;
				if (Printer_isConnected())
				{
						UI_Process_BLE_Print();
				}

				EXTI_Key_Confirm_Enable();
			}
			else
			{
				SystemManage_CheckPowerOff();
			}

			short_key_flag = 0;
		}
	}
}

/******************************************************************************/
void Key_Right(void)
{
	switch(Interface_Key)
	{
		case 0:

			key_state_confirm = 0;
			if(Key_control < 4)
			{
				Key_control += 1;
			}
			else
			{
				Key_control = 1;
			}
			key_state = ENABLE;
		break;

		case 1:
		break;

		case 2:
			key_state_confirm = 0;
			Key_control = 2;
			key_state = ENABLE;
		break;

		case 4:
			key_state_confirm = 0;
			Key_control = 2;
			key_state = ENABLE;
		break;

		case 5:
			key_state_confirm = 0;
			Key_control = 2;
			key_state = ENABLE;
		break;

		case 7:
			key_state_confirm = 0;
			Key_control = 2;
			key_state = ENABLE;
		break;

		default:
		break;
	}
}

/******************************************************************************/
void Key_Left(void)
{
	switch(Interface_Key)
	{
		case 0:
			key_state_confirm = 0;
			if(Key_control > 1)
			{
				Key_control -= 1;
			}
			else
			{
				Key_control = 4;
			}
			key_state = ENABLE;
		break;

		case 1:
			key_state_confirm = 0;
			Key_control = 2;
			key_state = ENABLE;
		break;

		case 2:
			key_state_confirm = 0;
			Key_control = 1;
			key_state = ENABLE;
		break;

		case 3:
			key_state_confirm = 0;
			Key_control = 1;
			key_state = ENABLE;
		break;

		case 4:
			key_state_confirm = 0;
			Key_control = 1;
			key_state = ENABLE;
		break;

		case 5:
			key_state_confirm = 0;
			if(Key_control == 1)
			{
				Key_control = 0;
			}
			else
			{
				Key_control = 1;
			}
			key_state = ENABLE;
		break;
		case 7:
			key_state_confirm = 0;
			Key_control = 1;
			key_state = ENABLE;
		break;

		default:
		break;
	}
}

/******************************************************************************/
void SystemManage_CheckPowerOff(void)
{
	QRCode_Trigger_Disabled();
	SystemManage_5V_Disabled();
	RotaMotorDriver_Control(MOTOR_DISABLED);
	ScanMotorDriver_Control(MOTOR_DISABLED);
	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	while(1);
}