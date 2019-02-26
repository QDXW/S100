/*******************************************************************************
 * File name: main.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#include "main.h"

/******************************************************************************/
#define SYSTICK_FREQ_72MHz  72000000
#define SYSTICK_FREQ_1MS    SYSTICK_FREQ_72MHz/1000

/******************************************************************************/
static u32 TimingDelay;					/* Timer Delay Count */
uint8 MBuffer[10] = {0},asd = 1;
uint16 insk[4] = {1,10,65535,65535};

/******************************************************************************/
void main(void)
{
	RCC_Configuration();				/* RCC Initialize */

	HumanInput_Init();					/* KEY HumanInput Initialize */

	DisplayDriver_Init();				/* LCD DisplayDriver Initialize */

	SysTick_Init(SYSTICK_FREQ_1MS);		/* Initialize system tick timer */

	PCF8563_Init();						/* PCF8563 Initialize */

	Adc_Init();							/* Adc Initialize */

	Printer_BLE_Init();					/* Printer BLE Initialize */

	EXTIX_Init();						/* EXTIX Initialize */

	QRCode_Init();						/* QRCode Initialize */

	RotationMotor_Init();				/* RotationMotorDriver Initialize */

	ScanMotorDriver_Init();				/* ScanMotorDriver Initialize */

	SignalSample_Sample_Init();			/* SignalSample Sample Initialize */

	HostComm_Init();					/* HostComm Initialize */

	TIM4_Int_Init(9999,7199);			/* 10Khz的计数频率，计数到10000为1s */

	Storage_Flash_Init();				/* Storage Flash Initialize */

	Status_Init();						/* Status Initialize */

	while(1)
	{
		HostComm_Process();

		Interface_Process(0);			/* User Interface */

//		if(asd)
//		{
//			Debug_Function();
//			asd = 0;
//		}
	}
}

/******************************************************************************/
void SysTick_Init(u32 ticks)
{
	if(SysTick_Config(ticks))
	{
		while (1);						/* Capture error */
	}
}

/*******************************************************************************
* Timer Delay
*******************************************************************************/
void Delay_ms(u32 nTime)
{
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

/******************************************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

/******************************************************************************/
void Status_Init(void)
{
   	GPIO_SetBits(GPIOD,GPIO_Pin_2);
   	Display_Time = 0;
   	Lcd_ColorBox(0,0,128,160,White);

#if PROTZEK_ICO
	DisplayDriver_DrawPic(14,39,100,82,gImage_START);
#endif
#if REALY_ICO
	DisplayDriver_DrawPic(0,55,128,62,gImage_Realy_ico);
#endif
#if HENGRUI_ICO
	DisplayDriver_DrawPic(4,55,120,60,gImage_HENGRUI_ICO);
#endif
#if ULTIMED
	DisplayDriver_DrawPic(7,25,110,110,gImage_ULTIMED);
#endif
	Set_Fixed_Parameter();
	Display_Time = 1;
	SystemManage_5V_Enabled();
	ScanMotorDriver_SelfCheck_StepDrive();
	RotationMotor_SelfCheck_StepDrive();
	SystemManage_5V_Disabled();
	Enter_Sleep = 1;
	if(Check_Lock)
	{
		Exti_lock = ENABLE;
  		while(1);
	}

 	Display_Time = 0;
 	UI_Draw_Status_Bar();
	Lcd_ColorBox(0,0,128,20,Dark_Blue);
	Battery_Empty_ICO();
	Power_Open = 1;
}

/******************************************************************************/
void Debug_Function(void)
{
//	uint8 value = 0,resistorValue_1 = 0;
//	uint16 resistorValue = 0,value_1 = 8;
//
//	memcpy(&resistorValue_1,&value_1,1);
//
//	sprintf(MBuffer,"%d",value_1);
//
//	Display_Time = 0;
//	DisplayDriver_Text16(4, 70, Black,MBuffer);
//	Display_Time = 1;
//
//	Storage_Write(&resistorValue_1, 0x12C000,1);
//
//	Storage_Read(&value,0x12C000,1);
//
//	sprintf(MBuffer,"%d",value);
//
//	Display_Time = 0;
//	DisplayDriver_Text16(4, 90, Black,MBuffer);
//	Display_Time = 1;
//
//	memcpy(&resistorValue,&value,1);
//
//	sprintf(MBuffer,"%d",resistorValue);
//
//	Display_Time = 0;
//	DisplayDriver_Text16(4, 110, Black,MBuffer);
//	Display_Time = 1;

	memcpy(MBuffer,insk,4);

	Storage_Write(MBuffer, 0x00, 4);

	Storage_Read(MBuffer,0x00,4);
	memcpy(insk,MBuffer,4);
	sprintf(MBuffer,"%d %d %d %d",insk[0],insk[1],insk[2],insk[3]);
	Display_Time = 0;
	DisplayDriver_Text16(4, 90, Black,MBuffer);
	Display_Time = 1;

	/*
	SystemManage_5V_Enabled();
	Get_Start_Postion();

	SignalSample_Sample_EnterCriticalArea();

	QRCode_Trigger_Enabled();

	SignalSample_Sample_EnterCriticalArea();

	GPIO_SetBits(GPIOE, GPIO_Pin_5);

	ScanMotorDriver_SelfCheck_StepDrive();

	RotationMotor_SelfCheck_StepDrive();

	SystemManage_5V_Disabled();

	Delay_ms(1000);
	ScanMotorDriver_SelfCheck_StepDrive();
	Exti_lock = ENABLE;
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_CentrePosition();
	Read_Record();
	*/
}
