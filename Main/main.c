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
uint8 UI_runMode = 0;
uint8 Display_Time = 1,Open_time = 0,Power_Open = 0,Key_State_Update = 0,Check_motor = 0;
uint8 Power_Switch = 0,Bluetooth_switch = 0,Enter_Sleep = 0,Display_Battery = 1;
uint8 MBuffer[10] = {0},asd = 1,Check_Lock = 0,Existed_Data = 0,Stop_Mode = 0;
uint16 insk[4] = {100,100,1,255};
uint16 adcx = 0,Data_Boundary = 800;
float temp = 0.0;

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
//		if(asd)
//		{
//			Debug_Function();
//			asd = 0;
//		}
//		SignalSample_SampleStrip();

		HostComm_Process();

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
	ReadResistor_Valid();
	Power_Open = 1;
	Enter_Sleep = 1;
	Display_Time = 1;
	if(Check_Lock)
	{
		Exti_lock = ENABLE;
		while(1);
	}
}

/******************************************************************************/
void Debug_Function(void)
{
	uint8 value = 0,resistorValue_1 = 0;
	uint16 resistorValue = 0,value_1 = 8;

	memcpy(&resistorValue_1,&value_1,1);

	sprintf(MBuffer,"%d",value_1);

	Display_Time = 0;
	DisplayDriver_Text16(4, 70, Black,MBuffer);
	Display_Time = 1;

	Storage_Write(&resistorValue_1, 0x12C000,1);

	Storage_Read(&value,0x12C000,1);

	sprintf(MBuffer,"%d",value);

	Display_Time = 0;
	DisplayDriver_Text16(4, 90, Black,MBuffer);
	Display_Time = 1;

	memcpy(&resistorValue,&value,1);

	sprintf(MBuffer,"%d",resistorValue);

	Display_Time = 0;
	DisplayDriver_Text16(4, 110, Black,MBuffer);
	Display_Time = 1;

	/*
	memcpy(MBuffer,insk,4);

	Storage_Write(MBuffer, 0x00, 4);

	Storage_Read(MBuffer,0x00,4);
	memcpy(insk,MBuffer,4);
	sprintf(MBuffer,"%d %d %d %d",insk[0],insk[1],insk[2],insk[3]);
	Display_Time = 0;
	DisplayDriver_Text16(4, 90, Black,MBuffer);
	Display_Time = 1;

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
