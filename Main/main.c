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
uint8 Power_Open = 0;
uint8 Display_Time = 1;

uint8 MBuffer[20] = {0};
uint8 asd = 1;
uint16 insk[4] = {0,0,0,255};

/******************************************************************************/
void main(void)
{
	uint8 Bluetooth_Connect = 0;

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

	USB_VirtualCOM_Init();				/* USB VirtualCOM Initialize */

	HostComm_Init();					/* HostComm Initialize */

	Status_Init();						/* Status Initialize */

	Storage_Flash_Init();				/* Storage Flash Initialize */

	while(1)
	{
//		USB_VirtualCOM_Process();
//
//		HostComm_Process();


//		if(asd)
//		{
//			memcpy(MBuffer,insk,4);
//
//			Storage_Write(MBuffer, 0x00, 4);
//
//			Storage_Read(MBuffer,0x00,4);
//			memcpy(insk,MBuffer,4);
//			sprintf(MBuffer,"%d %d %d %d",insk[0],insk[1],insk[2],insk[3]);
//			Display_Time = 0;
//			DisplayDriver_Text16(4, 90, Black,MBuffer);
//			Display_Time = 1;
//
//			SystemManage_5V_Enabled();
//
//			QRCode_Trigger_Enabled();
//
//			SignalSample_Sample_EnterCriticalArea();
//
//			GPIO_SetBits(GPIOE, GPIO_Pin_5);
//
//			ScanMotorDriver_SelfCheck_StepDrive();
//
//			RotationMotor_SelfCheck_StepDrive();
//
//			SystemManage_5V_Disabled();
//			asd = 0;
//			Delay_ms(1000);
//
//			SystemManage_5V_Disabled();
//			QRCode_Trigger_Enabled();
//			SystemManage_5V_Enabled();
//			ScanMotorDriver_Goto_BasePosition();
//			SignalSample_SampleStrip();
//			SystemManage_5V_Disabled();
//			ScanMotorDriver_SelfCheck_StepDrive();
//			Delay_ms_SW(1000);
//			RotationMotor_Input_StepDrive(Foreward_Rotation,Get_Start_Postion());
//			Acquisition_Signal();
//
//			ScanMotorDriver_SelfCheck_StepDrive();
//			Exti_lock = ENABLE;
//
//			ScanMotorDriver_Goto_BasePosition();
//			ScanMotorDriver_Goto_CentrePosition();
//			Read_Record();
//	}
//
//		Storage_Read(MBuffer,0x00,4);
//		memcpy(insk,MBuffer,4);
//		sprintf(MBuffer,"%d %d %d %d",insk[0],insk[1],insk[2],insk[3]);
//		Display_Time = 0;
//		DisplayDriver_Text16(4, 90, Black,MBuffer);
//		Display_Time = 1;
//		SignalSample_SampleStrip();

		Interface_Process(0);			/* User Interface */
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
   	Lcd_ColorBox(0,20,128,140,White);
	DisplayDriver_DrawPic(0,50,128,40,gImage_Power_on);
	DisplayDriver_DrawPic(0,117,128,18,gImage_Power_on2);
	Lcd_ColorBox(0,0,128,20,Dark_Blue);
	Display_Time = 1;
	UI_Draw_Status_Bar();
	Display_Time = 0;
	Battery_Empty_ICO();
	SystemManage_5V_Enabled();
	ScanMotorDriver_SelfCheck_StepDrive();
	RotationMotor_SelfCheck_StepDrive();
	SystemManage_5V_Disabled();
	Power_Open = 1;
	Display_Time = 1;
}