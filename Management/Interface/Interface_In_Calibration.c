/*
 * Interface_In_Calibration.c
 *
 *  Created on: 2018年9月3日
 *      Author: Administrator
 */

/******************************************************************************/
#include "Interface_In_Calibration.h"

/******************************************************************************/
uint8 Potential_Value = 255,Left_Value = 0,Right_Value = 255;
uint16 Mid_Value = 1;
uint8 SignalBuffer[1024] = {0};

/******************************************************************************/
uint8 Interface_In_Calibration_Process(uint16 blockNum)
{
	uint8 state = 0,Calibration_State = 0;
	Enter_Sleep = 0,Calibration_channel = 1;
	MotorDriver_Ctr = 1,Display_Battery = 0;
	Exti_lock = DISABLE,key_state_confirm = 0;

	/* 开始校准，无低功耗，按键无效 */
	UI_Background_Plate_In_Calibration();
	Calibration_State = Dichotomy_Calculate();

	/* 结果判定显示 */
	if(Calibration_State)
	{
		Display_Time = 0;
		Lcd_ColorBox(10,52,108,78,White);
		switch(Font_Switch)
		{
		case DISPLAY_FONT_ENGLISH:
			DisplayDriver_Text16(20,82,BACKCOLOR_CONTENT_BACK,"Successful!");
			break;

		case DISPLAY_FONT_CHINESE:
			DisplayDriver_Text16(28,82,BACKCOLOR_CONTENT_BACK,"校准成功!");
			break;

		case DISPLAY_FONT_GERMAN:
			DisplayDriver_Text16(6,82,BACKCOLOR_CONTENT_BACK,"Erfolgreich!");
			break;

		default:
			break;
		}
		Display_Time = 1;
	}
	else
	{
		Display_Time = 0;
		Lcd_ColorBox(10,52,108,78,White);
		switch(Font_Switch)
		{
		case DISPLAY_FONT_ENGLISH:
			DisplayDriver_Text16(28,82,BACKCOLOR_CONTENT_BACK,"Failure!");
			break;

		case DISPLAY_FONT_CHINESE:
			DisplayDriver_Text16(28,82,BACKCOLOR_CONTENT_BACK,"校准失败!");
			break;

		case DISPLAY_FONT_GERMAN:
			DisplayDriver_Text16(24,82,BACKCOLOR_CONTENT_BACK,"Scheitern!");
			break;

		default:
			break;
		}
		Display_Time = 1;
	}

	Exti_lock = ENABLE;
	UI_state = UI_STATE_CALIBRATION_KEY_PROCESS;
	Enter_Sleep = 1,MotorDriver_Ctr = 0;
	Delay_ms_SW(1010);
	return state;
}

/******************************************************************************/
void UI_Background_Plate_In_Calibration(void)
{
	Display_Time = 0;
	Lcd_ColorBox(0,20,128,140,BACKCOLOR_CONTENT_BACK);
	DisplayDriver_DrawPic(9,84,110,12,gImage_Empty_Step);
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(16,100,White,"Calibrating");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(20,100,White,"正在校准...");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16(0,100,White,"Die kalibrierung");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
uint8 Dichotomy_Calculate (void)
{
	uint16 i = 0,Max_Value = 0;
	uint8 num = 0,Poll_Count = 0;
	uint8 MBuffer[20] = {0};
	uint8 Result = 1,Step = 0,Num = 26,Start_Step =26;
	SignalSample_count = 0,Result_Judge = 0,Poll_Count = 0;
	Mid_Value = 255,Left_Value = 0,Right_Value = 255;

	SystemManage_5V_Enabled();
	ScanMotorDriver_Goto_BasePosition();
	SystemManage_5V_Disabled();

	/* 先判断0和255是否符合要求  */
	for(num = 0;num < 2;num++)
	{
		SignalSample_count = 0;
		Mid_Value = (num > 0)?255:0;
		SignalSample_resistorValue = (uint8)Mid_Value;
		SignalSample_resistorValueStored = (uint8)Mid_Value;
		SignalSample_Sample_SetResistor();

		SystemManage_5V_Enabled();
		ScanMotorDriver_Goto_BasePosition();
		SignalSample_SampleStrip();
		SystemManage_5V_Disabled();

		/* 调试模式下可输出数据  */
		if(UI_runMode)
		{
			memset(SignalBuffer,0,SignalSample_count << 1);
			memcpy(SignalBuffer, SignalProcess_sampleBuffer, SignalSample_count << 1);
			HostComm_Cmd_Send_RawData(SignalSample_count << 1, SignalBuffer);
		}

		if(Mid_Value)
		{
			for(i = 10;i < 135;i++)
			{
				if(SignalProcess_sampleBuffer[i] > 300)
				{
					Result_Judge = 0;
					return 0;
				}
			}
		}
		else
		{
			/* 求取最大值  */
			for(i = 10;i < 136;i++)
			{
				if(Max_Value < SignalProcess_sampleBuffer[i])
				{
					Max_Value = SignalProcess_sampleBuffer[i];
				}
			}
			if(Max_Value < 1900)
			{
				Result_Judge = 0;
				return 0;
			}
		}
	}

	Mid_Value = 20;
	do
	{
		if((Mid_Value < 1) && (Mid_Value > 254))
		{
			Result_Judge = 0;
			return 0;
		}

		/* 根据二分法设定的值重新采集数据 */
		SignalSample_count = 0,Max_Value = 0;
		Mid_Value = (Left_Value+Right_Value)/2;
		SignalSample_resistorValue = (uint8)Mid_Value;
		SignalSample_resistorValueStored = (uint8)Mid_Value;
		SignalSample_Sample_SetResistor();

		/* 丝杆电机上下信号采集  */
		memset(&SignalProcess_sampleBuffer[0],0,2*512);
		SystemManage_5V_Enabled();
		ScanMotorDriver_Goto_BasePosition();
		SignalSample_SampleStrip();
		SystemManage_5V_Disabled();

		/* 调试模式下可输出数据  */
		if(UI_runMode)
		{
			memset(SignalBuffer,0,SignalSample_count << 1);
			memcpy(SignalBuffer, SignalProcess_sampleBuffer, SignalSample_count << 1);
			HostComm_Cmd_Send_RawData(SignalSample_count << 1, SignalBuffer);
		}

		/* 求取最大值  */
		for(i = 1;i < 136;i++)
		{
			if(Max_Value < SignalProcess_sampleBuffer[i])
			{
				Max_Value = SignalProcess_sampleBuffer[i];
			}
		}

		/* 限定条件  */
		Result_Judge = (Max_Value > 1900)?1:0;
		Right_Value = (!Result_Judge)?Mid_Value:Right_Value;
		if(!Result_Judge)
			continue;

		for(i = 10;i < 135;i++)
		{
			Result_Judge = (SignalProcess_sampleBuffer[i] < 2100)?1:0;
			Left_Value = (!Result_Judge)?Mid_Value:Left_Value;
			if(!Result_Judge)
				break;

			Result_Judge = (SignalProcess_sampleBuffer[i] >= 200)?1:0;
			Right_Value = (!Result_Judge)?Mid_Value:Right_Value;
		}

		/* 进度刷新  */
		if(Calibration_channel)
		{
			if(Step < 8)
			{
				Step ++;
				for(Num = Start_Step;Num < (Start_Step + 12);Num++ )
				{
					Display_Time = 0;
					if(Num >= 111)
					{
						Display_Time = 0;
						DisplayDriver_DrawPic(9,84,110,12,gImage_All_Step);
						Display_Time = 1;
					}
					else
					{
						Lcd_ColorBox(Num,86,1,8,0x07AC);
					}
					Display_Time = 1;
					Delay_ms(30);
				}
				Start_Step += 12;
			}
		}

		/* 大于15次退出，校验失败 */
		Poll_Count++;
		if(Poll_Count > 12)
		{
			Result_Judge = 0;
			return Result_Judge;
		}

		Result = 1;
		/* 判定结果是否符合要求  */
		Result_Judge = Result_Judge?Dichotomy_Search(SignalProcess_sampleBuffer,Mid_Value):0;
		Result = Result_Judge;

		if(!Result)
			Storage_Write(&SignalSample_resistorValue, FLASH_CALI_ADDR,1);

	}while(!Result_Judge);

	if(Result)
	{
		Storage_Write(&SignalSample_resistorValue, FLASH_CALI_ADDR,1);
		/* Use it immediately */
		SignalSample_Sample_SetResistor();
	}

	return Result_Judge;
}

/******************************************************************************/
uint8 Dichotomy_Search(uint16 *Signal,uint8 Dichotomy_Value)
{
	uint8  Result_Flag = 0;
	uint16 i = 0,j = 0,Max_Value = 0,Max_Position = 0,Low_Value = 0,Low_Position = 0;
	uint16 Up_Trend = 0,Down_Trend = 0,Boundary_Down = 0;
	Max_Value = 0,Low_Value = 0;

	SignalSample_count = 0;
	SystemManage_5V_Enabled();
	SignalSample_Sample_SetResistor();
	memset(&SignalProcess_sampleBuffer[0],0,2*512);
	ScanMotorDriver_PositionSensor_Int_Enable();
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_CentrePosition();
	SignalSample_Sample_EnterCriticalArea();
	for(i = 0;i < Whole_Circle;i++)
	{
		RotationMotor_Input_StepDrive(Foreward_Rotation,1);
		SignalProcess_sampleBuffer[SignalSample_count++]
									= SignalProcess_Collecting_Data();
	}
	SignalSample_Sample_ExitCriticalArea();
	SystemManage_5V_Disabled();

	/* 调试模式下可输出数据  */
	if(UI_runMode)
	{
		memset(SignalBuffer,0,1024);
		memcpy(SignalBuffer, SignalProcess_sampleBuffer, 1024);
		HostComm_Cmd_Send_RawData(1024, SignalBuffer);
	}

	/* 得到上升沿、下降沿、边界值以下的数据个数  */
	for(i = 0;i < 511;i++)
	{
		if(SignalProcess_sampleBuffer[i] < 2400)
		{
			if((SignalProcess_sampleBuffer[i] < Data_Boundary) && (SignalProcess_sampleBuffer[i+1] >= Data_Boundary))
			{
				Up_Trend += 1;
			}

			if((SignalProcess_sampleBuffer[i] > Data_Boundary) && (SignalProcess_sampleBuffer[i+1] <= Data_Boundary))
			{
				Down_Trend += 1;
			}

			if(Data_Boundary > 200)
			{
				if(SignalProcess_sampleBuffer[i] < Data_Boundary)
				{
					Boundary_Down += 1;
				}
			}
		}
		else
		{
			Left_Value = Mid_Value;
			Result_Judge = 0;
			return 0;
		}
	}

	/* 判定上升沿下降沿都是一个、边界值以下的数据大于 160个，且谷值大于边界值250 */
	if((1 == Up_Trend) && (1 == Down_Trend) && (Boundary_Down > 160) )
	{
		Result_Judge = 1;
		Result_Flag = 1;
	}
	else
	{
		if(Boundary_Down > 160)
		{
			Right_Value = Mid_Value;
		}
		else
		{
			Left_Value = Mid_Value;
		}

		Result_Judge = 0;
		Result_Flag = 0;
	}

	return Result_Flag;
}

/******************************************************************************/
uint8 Interface_Calibration_key_Process(uint16 blockNum)
{
	uint8 state = 0;
	if(key_state)
	{
		if(key_state_confirm)
		{
			UI_state = UI_STATE_SETTING;
			key_state_confirm = DISABLE;
			Key_control = 1;
		}
	}
	return state;
}
