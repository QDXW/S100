/*
 * Interface_Testing.c
 *
 *  Created on: 2018年2月26日
 *      Author: Administrator
 */	 		 
/******************************************************************************/
#include "Interface_Testing.h"
#include "Signalprocess.h"

/******************************************************************************/
uint16 max = 0,Max_Postion = 0;
uint8 NowCup_Count = 0,Storage_Data_Conut = 0;
uint16 BOUNDARY_VALUE = 2500;
uint8 SignalProcess[1024] = {0};
//uint8 Record[16] = {0};
uint16 Old_Record = 1,New_Record = 0;
uint16 Old_Boundary = 0,New_Boundary = 0,Boundary_Count = 0;

/******************************************************************************/
block_attr_Testing block_Testing_1 = {
	ENABLE,								/* Interface Testing rect */
	{
		0,   20,
		128, 140,
		BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_Testing block_Testing_2 = {
	ENABLE,								/* Interface Testing rect */
	{
		16,   63,
		96, 15,
		White
	},
};

/******************************************************************************/
block_attr_Testing* UI_WindowBlocksAttrArray_Testing[] = {/* Window: Testing entry */
		&block_Testing_1,
		&block_Testing_2,
};

/******************************************************************************/
uint8 Interface_Testing(uint16 KeyCode)
{
	uint8 state = 0;
	Exti_lock = DISABLE;
	Interface_Key = 2;
	max = 0;

	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
	{
		UI_Language_Testing_Battery();
		if(temp <= 3.6)
		{
			return state;
		}
	}

	if(temp <= 3.67)
	{
		UI_Language_Testing_Low_Battery();
		return state;
	}

	Key_record = 2;
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Testing) >> 2;
	UI_Draw_Window_Testing(UI_WindowBlocks);
	UI_Language_Window_Testing();
	Display_Battery = 0;
	SystemManage_5V_Enabled();
	RotationMotor_Input_StepDrive(Foreward_Rotation,(Get_Start_Postion()));
	if(Confirm_CUP)
	{
		Acquisition_Signal();
		SystemManage_5V_Disabled();
		Storage_Record();
		UI_state = UI_STATE_RESULT;
	}
	else
	{
		UI_state = UI_STATE_INSERT_CUP;
	}
	Delay_ms_SW(200);
	return state;
}

/******************************************************************************/
void UI_Draw_block_Testing(block_attr_Testing* block);

/******************************************************************************/
void UI_Language_Window_Testing(void)
{
	Display_Time = 0;
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(26,80,White,"testing...");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(20,84,White,"正在测试...");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16(10,84,White,"Test l]uft...");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16(20,84,White,"Testanto...");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
void UI_Draw_Window_Testing(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		UI_Draw_block_Testing(UI_WindowBlocksAttrArray_Testing[blockIndex]);
	}
}

/******************************************************************************/
void UI_Draw_block_Testing(block_attr_Testing* block)
{
	Display_Time = 0;
	if (block->rect_enabled)				/* 1. Draw Rect*/
	{
		Lcd_ColorBox(block->rect_attr.startX, block->rect_attr.startY,
				block->rect_attr.width, block->rect_attr.height,
				block->rect_attr.color);
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
void Acquisition_Signal(void)
{
	uint8 j = 0,Step_Count = 28,Step_Start = 16,i = 0;
	NowCup_Count = 0;
	Storage_Data_Conut = 0;
	Storage_Time();

	/* 清除结构体信息  */
	for(i = 0;i < 8;i++)
	{
		memset(&Storage_Data.CH_data[i].Result[0],0,6);
	}

	/* 二维码信息移植入算法结构体 */
	QR_Date_SignalProcess_Alg_data();

	/* 采样与结果存储*/
	for(NowCup_Count = 0;NowCup_Count < 8;NowCup_Count++)
	{
		if(QR_Date.ch_data[NowCup_Count].Switch_Bool)
		{
			/* 采样 */
			SignalSample_SampleStrip();

			/* 运行算法 */
			memcpy(&SignalProcess_Alg_data.sampleBuffer[0],SignalProcess_sampleBuffer,SignalSample_count << 1);
			SignalProcess_Alg_data.sampleNumber = SignalSample_count;
			SignalProcess_Run();

			/* 判定结果 */
			Result_Display();
			Storage_Data_Conut += 1;

			/* 调试输出 */
			if(UI_runMode)
			{
				memset(SignalProcess,0,sizeof(SignalProcess));
				memcpy(SignalProcess,SignalProcess_sampleBuffer,1024);
				HostComm_Cmd_Send_RawData((SignalSample_count - 10) << 1, SignalProcess);
				Delay_ms_SW(30);
				memset(SignalProcess,0,sizeof(SignalProcess));
				HostComm_Cmd_Send_C_T(SignalProcess_Alg_data.calcInfo.areaC, SignalProcess_Alg_data.calcInfo.areaT);
				Delay_ms_SW(30);
			}
		}

		/* 转动电机转过相应的计算角度 */
		RotationMotor_Input_StepDrive(Foreward_Rotation,Record_Max_Postion[NowCup_Count]);

		/* 进度条刷新 */
		for(j = Step_Start;j < Step_Count;j++)		/* 每次进度条走十格 */
		{
			Delay_ms(30);
			Display_Time = 0;
			Lcd_ColorBox(j,63,1,15,BACKCOLOR_CONTENT_BAR);
			Display_Time = 1;
		}
		Step_Start = Step_Count;					/* 重置进度条开始位置 */
		Step_Count += 12;
	}
}

/******************************************************************************/
void Calculate_Max_Postion(uint16 First_Postion)
{
	uint8 i = 0;
	uint16 Before_Postion = First_Postion,Last_Postion = First_Postion + 42;
	memset(Record_Max_Postion,0,sizeof(Record_Max_Postion));
	Record_Max_Postion[7] = 10;
	memcpy(&SignalProcess_sampleBuffer[512],SignalProcess_sampleBuffer,512);
	for(i= 0;i < 7;i++)
	{
		Last_Postion = Judge_Max(&SignalProcess_sampleBuffer[0],Last_Postion);
		Record_Max_Postion[i] = Last_Postion - Before_Postion;
		Before_Postion = Last_Postion;
		Last_Postion += 42;
	}
}

/******************************************************************************/
uint16 Judge_Max (uint16* Signal,uint16 New_Postion)
{
	uint16 Postion_Plus = New_Postion - 10,Postion_Add = New_Postion + 10;
	uint16 max = Signal[New_Postion],i = New_Postion;
	for(i = Postion_Plus;i <= Postion_Add;i++)
	{
		if(Signal[i] > max)
		{
			New_Postion = i;
		}
	}

	return New_Postion;
}

/******************************************************************************/
uint16 Get_Start_Postion(void)
{
	uint8 Judge_Flag = 2,Trend_FLag = 0,Judge_Trend = 0;
	uint16 Start_Postion = 0,Postion_Down = 0,i = 0,Postion_Up = 0;

	while(Judge_Flag--)
	{
		if(0 == Judge_Flag)
		{
			RotationMotor_Input_StepDrive(Foreward_Rotation,40);
		}

		Acquisition_StartSignal();

		/* 3.找到上升沿的位置 */
		for(i = 0;i < 511;i++)
		{
			if((SignalProcess_sampleBuffer[i] < Data_Boundary) && (SignalProcess_sampleBuffer[i+1] >= Data_Boundary))
			{
				Trend_FLag = 1;
				if(Check_Trend(&SignalProcess_sampleBuffer[0],i,Trend_FLag))
				{
					Postion_Up = i;
					Judge_Trend = 1;
				}
				else
				{
					Confirm_CUP = NO_CUP;
					Judge_Trend = 0;
				}
			}
		}

		/* 3.找到下降沿的位置 */
		for(i = 0;i < 511;i++)
		{
			if((SignalProcess_sampleBuffer[i] > Data_Boundary) && (SignalProcess_sampleBuffer[i+1] <= Data_Boundary))
			{
				Trend_FLag = 0;
				if(Check_Trend(&SignalProcess_sampleBuffer[1],i,Trend_FLag))
				{
					Postion_Down = i;
					Judge_Trend = 1;
				}
				else
				{
					Confirm_CUP = NO_CUP;
					Judge_Trend = 0;
				}
			}
		}

		Postion_Up = (0 == Postion_Up)?1:Postion_Up;

		Postion_Down = (0 == Postion_Down)?1:Postion_Down;

		if(Judge_Trend)
		{
			if((Calculate_Postion_Up(&SignalProcess_sampleBuffer[1],Postion_Up)) == Calculate_Postion_Down(&SignalProcess_sampleBuffer[1],Postion_Down))
			{
				Judge_Flag = 0;
				Start_Postion = Calculate_Postion_Up(&SignalProcess_sampleBuffer[1],Postion_Up);
			}
			else
			{
				Confirm_CUP = NO_CUP;
			}
		}
	}

	if(!Confirm_CUP)
	{
		Start_Postion = 0;
	}
	else
	{
		Calculate_Max_Postion(Start_Postion);
	}

	return Start_Postion;
}

/******************************************************************************/
void Acquisition_StartSignal(void)
{
	uint16 i = 0;
	SignalSample_count = 0;
	/* 第一步:扫描电机转到中间位置 */
	memset(&SignalProcess_sampleBuffer[0],0,2*512);
	ScanMotorDriver_PositionSensor_Int_Enable();
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_CentrePosition();
	SignalSample_Sample_EnterCriticalArea();

	/* 第二步:旋转360度，环绕杯子采集信号 */
	for(i = 0;i < Whole_Circle;i++)
	{
		RotationMotor_Input_StepDrive(Foreward_Rotation,1);
		SignalProcess_sampleBuffer[SignalSample_count++]
									= SignalProcess_Collecting_Data();
	}

	if(UI_runMode)
	{
		memset(SignalProcess,0,1024);
		memcpy(SignalProcess, &SignalProcess_sampleBuffer[0], 1024);
		HostComm_Cmd_Send_RawData(1024, SignalProcess);
	}

	SignalSample_Sample_ExitCriticalArea();
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_DetectionPosition();
	/* 第三步:数据处理得到杯子检测的起始位置*/
	Get_sampleBuffer_Boundary_Value();
	Get_sampleBuffer_Max_Value();

	/* 2.有无杯子判断 */
	if((max < 1000) || (BOUNDARY_VALUE > 800))
	{
		Confirm_CUP = NO_CUP;
		return ;
	}
	else
	{
		Confirm_CUP = 1;
	}
}

/******************************************************************************/
uint16 Get_sampleBuffer_Boundary_Value(void)
{
	uint16 Old_Record = 1,New_Record = 0;
	uint16 i = 0,Old_Boundary = 0,New_Boundary = 0,Boundary_Count = 0;
	BOUNDARY_VALUE = 2500;
	for(i = 1;i < SIGNALSAMPLE_MAX_COUNT;i++)
	{
		if(BOUNDARY_VALUE >= SignalProcess_sampleBuffer[i])
		{
			BOUNDARY_VALUE = SignalProcess_sampleBuffer[i];
		}
	}

	if(BOUNDARY_VALUE > 1000)
	{
		BOUNDARY_VALUE = 2500;
		return BOUNDARY_VALUE;
	}

	Old_Boundary = BOUNDARY_VALUE;
	New_Boundary = BOUNDARY_VALUE;

	for(Data_Boundary = BOUNDARY_VALUE;Data_Boundary < 800;Data_Boundary += 10)
	{
		Boundary_Count = 0;
		for(i = 0;i < 511;i++)
		{
			if((Data_Boundary >= SignalProcess_sampleBuffer[i+1]) && (Data_Boundary <= SignalProcess_sampleBuffer[i]))
			{
				Boundary_Count++;
			}

			if((Data_Boundary <= SignalProcess_sampleBuffer[i+1]) && (Data_Boundary >= SignalProcess_sampleBuffer[i]) )
			{
				Boundary_Count++;
			}
		}

		New_Record = ((Boundary_Count < 3) && (Boundary_Count > 0))?1:0;

		if(Old_Record && (!New_Record))
		{
			New_Boundary = Data_Boundary;
		}

		if((!Old_Record) && New_Record)
		{
			Old_Boundary = Data_Boundary;
		}

		if(Old_Record && New_Record)
		{
			New_Boundary = Data_Boundary;
		}

		Old_Record = New_Record;
	}

	if(New_Boundary == BOUNDARY_VALUE)
	{
		BOUNDARY_VALUE = 2500;
		return BOUNDARY_VALUE;
	}

	Data_Boundary = (New_Boundary + Old_Boundary)/2;

	return BOUNDARY_VALUE;
}

/******************************************************************************/
uint16 Get_sampleBuffer_Max_Value(void)
{
	int j=0;
	max = 0;
	for(j = 0;j < SIGNALSAMPLE_MAX_COUNT;j++)
	{
		if(max < SignalProcess_sampleBuffer[j])
		{
			max = SignalProcess_sampleBuffer[j];
			Max_Postion = j;
		}
	}
	return max;
}

/******************************************************************************/
void SignalSample_Moving_Average_Data(uint16 *Data,uint16 Length,uint16 Period)
{
	uint16 i=0,j=0;
	uint32 Num = 0;
	/*对数据进行移动平均*/
	for(i = 0;i < Length-Period;i++)
	{
		for(j = 0;j < Period;j++)
		{
			Num += Data[i+j];
		}
		Data[i] = Num/Period;
		Num=0;
	}
}

/******************************************************************************/
uint16 Calculate_Postion_Up(uint16* Signal,uint16 Postion)
{
	uint16 Start_Postion = 0,i = 0,Confirm_Postion = 1,Reconfirm_Postion = 0;

	i = Postion;
	do
	{
		if(Signal[i] <= Signal[i+1])
		{
			i += 1;
			Start_Postion = i;
			Confirm_Postion = 1;
			if(i >511)
			{
				Confirm_Postion = 0;
				Reconfirm_Postion = 1;
			}
		}
		else
		{
			Confirm_Postion = 0;
		}
	}while(Confirm_Postion);

	if(Reconfirm_Postion)
	{
		i = 1;
		do
		{
			if(Signal[i] <= Signal[i+1])
			{
				i += 1;
				Start_Postion = i;
				Confirm_Postion = 1;
			}
			else
			{
				Confirm_Postion = 0;
			}
		}while(Confirm_Postion);
	}

	return Start_Postion;
}

/******************************************************************************/
uint16 Calculate_Postion_Down(uint16* Signal,uint16 Postion)
{
	uint16 Start_Postion = 0,i = 0,Confirm_Postion = 1,Reconfirm_Postion = 0;

	i = (Postion + 170);
	if(i < 512)
	{
		do
		{
			if(Signal[i] <= Signal[i+1])
			{
				i += 1;
				Start_Postion = i;
				Confirm_Postion = 1;
				if(i >511)
				{
					Confirm_Postion = 0;
					Reconfirm_Postion = 1;
				}
			}
			else
			{
				(Signal[i+1] > Data_Boundary)?(Confirm_Postion = 0):(i += 1);
			}
		}while(Confirm_Postion);
	}
	else
	{
		i = i - 511;
		do
		{
			if(Signal[i] <= Signal[i+1])
			{
				i += 1;
				Start_Postion = i;
				Confirm_Postion = 1;
			}
			else
			{
				(Signal[i+1] > Data_Boundary)?(Confirm_Postion = 0):(i += 1);
			}
		}while(Confirm_Postion);
	}

	if(Reconfirm_Postion)
	{
		i = 1;
		do
		{
			if(Signal[i] <= Signal[i+1])
			{
				i += 1;
				Start_Postion = i;
				Confirm_Postion = 1;
			}
			else
			{
				(Signal[i+1] > Data_Boundary)?(Confirm_Postion = 0):(i += 1);
			}
		}while(Confirm_Postion);
	}

	return Start_Postion;
}

/******************************************************************************/
uint16 Check_Trend(uint16 *Signal,uint16 Postion,uint8 Flag)
{
	uint16 i = 0,state = 0,Start_Postion = 0;
	Start_Postion  = Postion;
	if (Flag)
	{
		if((Start_Postion + 6) < 511)
		{
			for(i = Start_Postion;i < (Start_Postion + 6);i++)
			{
				if(Signal[i] < Signal[i+1])
				{
					state = 1;
				}
				else
				{
					state = 0;
				}
			}
		}
		else
		{
			for(i = Start_Postion;i < 511;i++)
			{
				if(Signal[i] < Signal[i+1])
				{
					state = 1;
				}
				else
				{
					state = 0;
				}
			}
		}
	}
	else
	{
		if((Start_Postion + 2) < 511)
		{
			for(i = Start_Postion;i < (Start_Postion + 1);i++)
			{
				if(Signal[i] > Signal[i+1])
				{
					state = 1;
				}
				else
				{
					state = 0;
				}
			}
		}
		else
		{
			for(i = Start_Postion;i < (Start_Postion + 1);i++)
			{
				if(Signal[1] > Signal[2])
				{
					state = 1;
				}
				else
				{
					state = 0;
				}
			}
		}
	}
	return state;
}

/******************************************************************************/
void QR_Date_SignalProcess_Alg_data (void)
{
	/* @TODO */
	SignalProcess_Alg_data.posInfo.C_center = QR_Date.head.midC;
	SignalProcess_Alg_data.posInfo.searchHalfRadius_C = QR_Date.head.searchHalfRadius_C;
	SignalProcess_Alg_data.posInfo.dist_C_T1 = QR_Date.head.distC_T;
	SignalProcess_Alg_data.posInfo.searchHalfRadius_T = QR_Date.head.searchHalfRadius_T;
	SignalProcess_Alg_data.posInfo.dist_peak1 = QR_Date.head.distC_Base1;
	SignalProcess_Alg_data.posInfo.dist_peak2 = QR_Date.head.distC_Base2;
	SignalProcess_Alg_data.posInfo.dist_peak3 = QR_Date.head.distC_Base3;
	SignalProcess_Alg_data.posInfo.dist_peak4 = QR_Date.head.distC_Base4;
	SignalProcess_Alg_data.posInfo.areaC_HalfRadius = QR_Date.head.areaC_HalfRadius;
	SignalProcess_Alg_data.posInfo.areaT_HalfRadius = QR_Date.head.areaT_HalfRadius;
	SignalProcess_Alg_data.posInfo.winSize = QR_Date.head.winSize;
	SignalProcess_Alg_data.limitEnabled = QR_Date.head.limitEnabled;
	SignalProcess_Alg_data.limitInfo.C_stepSize = QR_Date.head.C_stepSize;
	SignalProcess_Alg_data.limitInfo.C_magnitude = QR_Date.head.C_magnitude;
	SignalProcess_Alg_data.limitInfo.C_MIN = QR_Date.head.C_MIN;
}

/******************************************************************************/
void Result_Display(void)
{
	if (SignalProcess_Alg_data.calcInfo.validity == ALG_RESULT_ABNORMAL_C)
	{
		memcpy(Storage_Data.CH_data[Storage_Data_Conut].Result, "INV",5);
		return;
	}
	else if (SignalProcess_Alg_data.calcInfo.validity == ALG_RESULT_LOW_AREA_C)
	{
		memcpy(Storage_Data.CH_data[Storage_Data_Conut].Result, "INV",5);
		return;
	}
	else if (SignalProcess_Alg_data.calcInfo.validity == ALG_RESULT_NO_T)
	{
		memcpy(Storage_Data.CH_data[Storage_Data_Conut].Result, "Pos++",5);
		return;
	}
	else
	{
		if(SignalProcess_Alg_data.calcInfo.ratioC_T > Storage_Data.CH_data[Storage_Data_Conut].threshold1)
		{
			memcpy(Storage_Data.CH_data[Storage_Data_Conut].Result, "Pos++",5);
		}
		else if (SignalProcess_Alg_data.calcInfo.ratioC_T > Storage_Data.CH_data[Storage_Data_Conut].threshold2)
		{
			memcpy(Storage_Data.CH_data[Storage_Data_Conut].Result, "Pos+",5);
		}
		else if (SignalProcess_Alg_data.calcInfo.ratioC_T > Storage_Data.CH_data[Storage_Data_Conut].threshold3)
		{
			memcpy(Storage_Data.CH_data[Storage_Data_Conut].Result, "Neg-",5);
		}
		else
		{
			memcpy(Storage_Data.CH_data[Storage_Data_Conut].Result, "Neg--",5);
		}
	}
}
