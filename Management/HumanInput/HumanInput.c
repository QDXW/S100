/*
 * HumanInput.c
 *
 *  Created on: 2018年1月25日
 *      Author: Administrator
 ******************************************************************************/
#include "HumanInput.h"

/******************************************************************************/
extern uint8 LED = 0,HSEStartUpStatusPwr = 0;

/*******************************************************************************
	函数名：HumanInput_Init
	输  入: 无
	输  出: 无
	功能说明：使能按键相应GPIO管脚
	初始化串口硬件设备，未启用中断。
*******************************************************************************/
void HumanInput_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#if	 LCD_OLD
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);				//初始化GPIOD2,3,4

#else
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化GPIOC6,7
#endif

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//PA4设置成输入，默认下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				//初始化GPIOB.0
}

/*******************************************************************************
	函数名：EXTIX_Init
	输  入: 无
	输  出: 无
	功能说明：使能按键相应外部中断
	初始化串口硬件设备，未启用中断。
*******************************************************************************/
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIOB.4	  中断线以及中断初始化配置 上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;  				//中间确认按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

#if	 LCD_OLD
	//GPIOD.8 中断线以及中断初始化配置   上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource8);

	EXTI_InitStructure.EXTI_Line = EXTI_Line8;					//向右按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//GPIOD.9	  中断线以及中断初始化配置  上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource9);

	EXTI_InitStructure.EXTI_Line = EXTI_Line9;					//向左按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
#else
	//GPIOC.6 中断线以及中断初始化配置   上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);

	EXTI_InitStructure.EXTI_Line = EXTI_Line6;					//向右按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//GPIOC.7	  中断线以及中断初始化配置  上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

	EXTI_InitStructure.EXTI_Line = EXTI_Line7;					//向左按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
#endif

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
void EXTI_Key_Confirm_Disable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	//GPIOB.4	  中断线以及中断初始化配置 上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;  				//中间确认按钮
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

	//GPIOB.4	  中断线以及中断初始化配置 上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;  				//中间确认按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
void EXTI_Key_Left_Disable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIOC.7	  中断线以及中断初始化配置  上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

	EXTI_InitStructure.EXTI_Line = EXTI_Line7;					//向左按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************/
void EXTI_Key_Right_Disable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIOC.6 中断线以及中断初始化配置   上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);

	EXTI_InitStructure.EXTI_Line = EXTI_Line6;					//向右按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************/
void EXTI_Key_Left_Enable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIOC.7	  中断线以及中断初始化配置  上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

	EXTI_InitStructure.EXTI_Line = EXTI_Line7;					//向左按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
void EXTI_Key_Right_Enable(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIOC.6 中断线以及中断初始化配置   上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);

	EXTI_InitStructure.EXTI_Line = EXTI_Line6;					//向右按钮
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
	函数名：KeyX_Work
	输  入: 无
	输  出: 无
	功能说明：相应按键功能
	初始化串口硬件设备，未启用中断。
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
					GPIO_SetBits(GPIOE, GPIO_Pin_5);   //改动关掉
					Display_Time = 0;
					DisplayDriver_DrawPic(60,3,15,15,gImage_LED_Switch);
					Display_Time = 1;
					EXTI_Key_Confirm_Enable();
					LED = 1;
				}
				else
				{
					key_state_confirm = 0;
					doubleClick = 0;
					GPIO_ResetBits(GPIOE, GPIO_Pin_5);
					Display_Time = 0;
					Lcd_ColorBox(60,3,15,15,BACKCOLOR_CONTENT_BAR);
					Display_Time = 1;
					EXTI_Key_Confirm_Enable();
					LED = 0;
				}
			}
			else
			{
				if(Stop_Mode)
				{
					SYSCLKConfig_STOP();
					Stop_Mode = 0;
				}
				else
				{
					key_state_confirm = ENABLE;
				}
				EXTI_Key_Confirm_Enable();
			}
			key_state = ENABLE;
			short_key_flag = 0;
		}

		if(UI_state != UI_STATE_TESTING && Power_Open && long_key_flag)
		{
			if(Interface_Key == 2 || Interface_Key == 4)
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
//			key_state = DISABLE;
			short_key_flag = 0;
		}

		if(!Power_Open && Exti_lock && long_key_flag)
		{
			SystemManage_CheckPowerOff();
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
			if(Key_control < 3)
			{
				Key_control += 1;
			}
			else
			{
				Key_control = 3;
			}
			key_state = ENABLE;
		break;

		case 7:
			key_state_confirm = 0;
			Key_control = 2;
			key_state = ENABLE;
		break;

		case 9:
			key_state_confirm = 0;
			if(Key_control < 2)
			{
				Key_control += 1;
			}
			else
			{
				Key_control = 2;
			}
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
			if(Key_control > 0)
			{
				Key_control -= 1;
			}
			else
			{
				Key_control = 0;
			}
			key_state = ENABLE;
		break;

		case 7:
			key_state_confirm = 0;
			Key_control = 1;
			key_state = ENABLE;
		break;

		case 8:
			key_state_confirm = 0;
			Key_control = 1;
			key_state = ENABLE;
		break;

		case 9:
			key_state_confirm = 0;
			if(Key_control > 1)
			{
				Key_control -= 1;
			}
			else
			{
				Key_control = 1;
			}
			key_state = ENABLE;
		break;

		default:
		break;
	}
}

/******************************************************************************/
void SystemManage_Sleep_Process(void)
{
	Stop_Mode = 1;

	Exti_lock = DISABLE;

	/* 左键中断关闭  */
	EXTI_Key_Left_Disable();

	/* 右键中断关闭  */
	EXTI_Key_Right_Disable();

	/* 关闭5V电源  */
	SystemManage_5V_Disabled();

	/* 关闭背光  */
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);

	Exti_lock = ENABLE;

//	SystemManage_EnterExitStop();
}

/******************************************************************************/
void SystemManage_EnterExitStop(void)
{
    /* Request to enter STOP mode with regulator in low power mode*/
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

    /* At this stage the system has resumed from STOP mode,
     * like external interrupt, internal timer interrupt, etc */

    /* Configures system clock after wake-up from STOP: enable HSE, PLL and
     * select PLL as system clock source
     *  (HSE and PLL are disabled in STOP mode) */
    SYSCLKConfig_STOP();
}

/******************************************************************************/
void SYSCLKConfig_STOP(void)
{
	Power_Second = 0,Power_Minute = 0,Exti_lock = DISABLE;
	EXTI_Key_Left_Enable();

	EXTI_Key_Right_Enable();

	/* 打开背光  */
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	Delay_ms_SW(150);
	Exti_lock = ENABLE;
	key_state = DISABLE;
	key_state_confirm = 0;
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
