 /*******************************************************************************
 * File name: stm32f10x_it.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#include "stm32f10x_it.h"

/******************************************************************************/
extern uint8 SignalSample_moveThenSample;
extern uint8 Power_Open;
extern uint8 Display_Time;
uint8 time_second = 60,Battery_Second = 0,Check_flag = 0;
extern uint8 Open_time;
uint16 Power_Second = 0;
uint16 Power_Minute = 0,Stop_Mode_Second = 0;

/******************************************************************************/
void Delay_ms_SW(__IO uint32 nCount)
{
#define SW_72MHZ_1MS_COUNT (0x27FF)
	uint32 subCount;
	for(; nCount != 0; nCount--)
	{
		subCount = SW_72MHZ_1MS_COUNT;
		for(; subCount != 0; subCount--);
	}
}

/******************************************************************************/
void NMI_Handler(void)
{
}

/******************************************************************************/
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/******************************************************************************/
void SVC_Handler(void)
{
}

/******************************************************************************/
void DebugMon_Handler(void)
{
}

/******************************************************************************/
void PendSV_Handler(void)
{
}

/******************************************************************************/
void SysTick_Handler(void)
{
	TimingDelay_Decrement();								/* Decrease TimingDelay */

	if(Enter_Sleep)
	{
		if(!Key_State_Update)
		{
			Power_Second++;
			if(Power_Second > 999)
			{
				Power_Second = 0;
				Power_Minute++;
				if(Power_Minute > 30)
				{
					Power_Minute = 0;
					SystemManage_Sleep_Process();
				}
			}
		}
		else
		{
			Key_State_Update = 0;
			Power_Second = 0;
			Power_Minute = 0;
		}
	}
	else
	{
		Power_Second = 0;
		Power_Minute = 0;
	}


	 if(key_fall_flag == 1)									//�������������¼�
	 {
		 if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))		//������������
		 {
			 if(key_holdon_ms < 1100)
			 {
				 key_holdon_ms++;
			 }
			 else 											//�������µ�2000ms���жϳ���ʱ����������ɳ�����־
			 {
				 key_holdon_ms = 0;
				 short_key_flag=0;							//��̰�����־
				 long_key_flag = 1;							//��������־��λ
				 key_fall_flag = 0;							//�尴�����±�־
				 short_key_down = 0;
			 }
		 }
		 else 												//����̧��
		 {
			 if(key_holdon_ms>50)							//����ʱ�����50ms�����ɵ�����־
			 {
				 key_holdon_ms=0;
				 short_key_flag=0;
				 long_key_flag =0;
				 key_fall_flag=0;

				 //�����ϴε���ʱ����100~500ms֮�䣬����Ϊ���������¼�
				 if(keyupCnt > 50 && keyupCnt < 300)
				 {
					 doubleClick = TRUE;
				     short_key_flag=0;
				 }
				 keyUpFlag = TRUE;							//����̧�𰴼������ɰ���̧���־
			 }
			 else  											//��������ʱ��С��50ms������
			 {
				 key_holdon_ms = 0;
				 short_key_flag = 0;
				 long_key_flag = 0;
				 key_fall_flag = 0;
			 }
		 }
	 }

	if(keyUpFlag)											//����̧�������������������500ms
		keyupCnt++;
	if(keyupCnt > 300)
	{
		short_key_flag = 1;
		keyupCnt = 0;
		keyUpFlag = FALSE;
	}
	Key_Confirm();											//�˺�������������ʱ

	EXTI_Key_Confirm_Enable();
}

/******************************************************************************/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		SignalSample_moveThenSample = 1;
	}

	TIM_ClearITPendingBit(TIM3, ENABLE ); 					//��� TIM3 �����жϱ�־
}

/******************************************************************************/
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  	//���TIM4�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  		//���TIMx�����жϱ�־
		if(Display_Time)
		{
			Display_Time = 0;
			Bluetooth_Connection();
			Display_Time = 1;
		}

		if((!MotorDriver_Ctr) && Display_Time)
		{
			Display_Time = 0;
			Battery_Display();
			Display_Time = 1;
			UI_Draw_Status_Bar();
		}
	}

	if(Open_time)
	{
		time_second--;
		Display_Down_Time_second();
		if(time_second < 1)
		{
			time_second = 60;
			Action_time--;
			Display_Down_Time_Msec();
			if(Action_time < 1)
			{
				Open_time = 0;
				time_second = 60;
			}
		}
	}
	else
	{
		time_second = 60;
	}

	if(Check_motor)
	{
		EXTI_Key_Left_Disable();
		EXTI_Key_Right_Disable();
		Interface_Key = 100;
		Display_Time = 0;
		if(1 == Check_Lock)
		{
			Lcd_ColorBox(0,20,128, 140,White);
			DisplayDriver_Text16(8, 52, Red,"Error: 001");
			DisplayDriver_Text16(8, 70, Red,"Screw motor");
			Check_flag = 1;
		}

		if(2 == Check_Lock)
		{
			if(!Check_flag)
			{
				Lcd_ColorBox(0,20,128, 140,White);
			}
			DisplayDriver_Text16(8, 90, Red,"Error: 002");
			DisplayDriver_Text16(8, 108, Red,"Rotating motor");
		}

		Display_Time = 1;
		Check_motor = 0;
	}
	else
	{
		Check_motor = 0;
		if(0 == Check_Lock)
		{
			Check_flag = 0;
		}
	}

	if(Stop_Mode)
	{
		Stop_Mode_Second++;
		if(Stop_Mode_Second > 299)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_3);
		}
	}
	else
	{
		Stop_Mode_Second = 0;
	}

}

/******************************************************************************/
void TIM5_IRQHandler(void)
{

}

/******************************************************************************/
void TIM7_IRQHandler(void)
{

}

/******************************************************************************/
void EXTI0_IRQHandler(void)
{

}

/******************************************************************************/
void EXTI1_IRQHandler(void)
{
//	ScanMotorDriver_InBasePosition = 1;
//	ScanMotorDriver_PositionSensor_Int_Disable();
	EXTI_ClearITPendingBit(EXTI_Line1);		 				//���LINE1�ϵ��жϱ�־λ
}

/******************************************************************************/
void EXTI2_IRQHandler(void)
{
//	RotaMotorDriver_PositionSensor_Int_Disable();
	EXTI_ClearITPendingBit(EXTI_Line2);			 			//���LINE2�ϵ��жϱ�־λ
}

/******************************************************************************/
void EXTI3_IRQHandler(void)
{
}

/******************************************************************************/
void EXTI4_IRQHandler(void)
{
	if(Exti_lock)
	{
		if(EXTI_GetITStatus(EXTI_Line4))	 				//ȷ�ϰ���
		{
			Key_State_Update = 1;
			key_fall_flag = 1;								//ȷ�ϰ���
			EXTI_ClearITPendingBit(EXTI_Line4);				//���LINE4�ϵ��жϱ�־λ
			EXTI_Key_Confirm_Disable();
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);			 			//���LINE4�ϵ��жϱ�־λ
}

/******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(Exti_lock)
	{
#if	 LCD_OLD
		if(EXTI_GetITStatus(EXTI_Line8))	 				//���Ұ���
		{
			Delay_ms_SW(160);
			Key_Right();
		}

		if(EXTI_GetITStatus(EXTI_Line9))	 				//���󰴼�
		{
			Delay_ms_SW(190);
			Key_Left();
		}
#else
		if(EXTI_GetITStatus(EXTI_Line6))	 				//���Ұ���
		{
			Key_State_Update = 1;
			Delay_ms_SW(160);
			Key_Right();
		}

		if(EXTI_GetITStatus(EXTI_Line7))	 				//���󰴼�
		{
			Key_State_Update = 1;
			Delay_ms_SW(190);
			Key_Left();
		}
#endif
	}

#if	 LCD_OLD
	EXTI_ClearITPendingBit(EXTI_Line8);  					//���LINE8�ϵ��жϱ�־λ
	EXTI_ClearITPendingBit(EXTI_Line9);  					//���LINE9�ϵ��жϱ�־λ
#else
	EXTI_ClearITPendingBit(EXTI_Line6);  					//���LINE6�ϵ��жϱ�־λ
	EXTI_ClearITPendingBit(EXTI_Line7);  					//���LINE7�ϵ��жϱ�־λ
#endif
}

/******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
}

/******************************************************************************/
void PVD_IRQHandler(void)
{
}

/******************************************************************************/
void Display_Down_Time_second (void)
{
	char tbuf[4] = {0};
	sprintf((char*)tbuf,"%02d",time_second);
	Display_Time = 0;
	DisplayDriver_Text16_B(68,82,Black,Light_Gray,tbuf);
	Display_Time = 1;
}

/******************************************************************************/
void Display_Down_Time_Msec (void)
{
	char tbuf[4] = {0};
	sprintf((char*)tbuf,"%02d:",Action_time);
	Display_Time = 0;
	DisplayDriver_Text16_B(44,82,Black,Light_Gray,tbuf);
	Display_Time = 1;
}
