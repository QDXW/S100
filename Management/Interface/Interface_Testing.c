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
uint16 max = 0;
uint8 NowCup_Count = 0,Storage_Data_Conut = 0;
uint8 Zero_Count = 0;
uint8 Forever_Value = 2;
extern uint8  Cup_Count;
uint16 BOUNDARY_VALUE = 2500;
extern uint16 SignalSample_count;
extern uint16 SignalProcess_sampleBuffer[SIGNALSAMPLE_MAX_COUNT];

//uint8 SignalBuffer[500] = {0};

/******************************************************************************/
block_attr_Testing block_Testing_1 = {
	ENABLE,								/* Interface Testing rect */
	{
		0,   20,
		128, 140,
		BACKCOLOR_CONTENT_BACK
	},
	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Testing block_Testing_2 = {
	ENABLE,								/* Interface Testing rect */
	{
		14,   63,
		100, 15,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		"testing...",
		26,   80,
		Black,BACKCOLOR_CONTENT_BACK,
		BACKCOLOR_CONTENT_BACK
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
		if(temp > 3.6)
		{

		}
		else
		{
			Display_Time = 0;
			Lcd_ColorBox(0,20,128, 140,White);
			DisplayDriver_Text16(24, 80, Red,"No Battery!");
			Display_Time = 1;
			UI_state = UI_STATE_MAIN_WINDOW;
			Delay_ms_SW(1500);
			return state;
		}
	}

	UI_WindowBlocks_Testing = sizeof(UI_WindowBlocksAttrArray_Testing) >> 2;
	UI_Draw_Window_Testing(UI_WindowBlocks_Testing);
	SystemManage_5V_Enabled();
	RotationMotor_Input_StepDrive(Foreward_Rotation,Get_Start_Postion() + 22);
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
	if (block->char_enabled)				/* 2. Draw character */
	{
		DisplayDriver_Text16_B(
				block->char_attr.offsetX,block->char_attr.offsetY,
				block->char_attr.color,block->char_attr.faceColor,
				block->char_attr.str);
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
void Acquisition_Signal(void)
{
	uint8 j = 0,Step_Count = 24,Step_Start = 14,i = 0;
	NowCup_Count = 0;
	Storage_Data_Conut = 0;
	Storage_Time();
	for(i = 0;i < 12;i++)
	{
		memset(&Storage_Data.CH_data[i].Result[0],0,6);
	}
	/* 第二步:旋转360度，环绕杯子采集信号 */

	/* 二维码 */
	QR_Date_SignalProcess_Alg_data();

	/* 采样与结果存储*/
	for(NowCup_Count = 0;NowCup_Count< 10;NowCup_Count++)
	{
		if(QR_Date.ch_data[NowCup_Count].Switch_Bool)
		{
			/* 采样 */
			SignalSample_SampleStrip();

			/* 运行算法 */
			memcpy(&SignalProcess_Alg_data.sampleBuffer[0], SignalProcess_sampleBuffer, SignalSample_count << 1);
			SignalProcess_Alg_data.sampleNumber = SignalSample_count;
			SignalProcess_Run();

			/* 判定结果 */
			Result_Judge();
			Storage_Data_Conut += 1;
		}

		/* 调试输出 */
//		memset(SignalBuffer,0,500);
//		memcpy(SignalBuffer, &SignalProcess_Alg_data.processBuffer[0], SignalProcess_Alg_data.processNumder << 1);
//		HostComm_Cmd_Send_RawData(SignalProcess_Alg_data.processNumder << 1, SignalBuffer);
//		HostComm_Cmd_Send_C_T(SignalProcess_Alg_data.calcInfo.areaC, SignalProcess_Alg_data.calcInfo.areaT);

		/* 转动电机转动30° */
		if(NowCup_Count%3 == 1)
		{
			RotationMotor_Input_StepDrive(Foreward_Rotation,43);
		}
		else
		{
			RotationMotor_Input_StepDrive(Foreward_Rotation,42);
		}

		/* 进度条刷新 */
		for(j = Step_Start;j < Step_Count;j++)		/* 每次进度条走十格 */
		{
			Delay_ms(30);
			Display_Time = 0;
			Lcd_ColorBox(j,63,1,15,BACKCOLOR_CONTENT_BAR);
			Display_Time = 1;
		}
		Step_Start = Step_Count;					/* 重置进度条开始位置 */
		Step_Count += 10;
	}
}

/******************************************************************************/
uint16 Get_Start_Postion(void)
{
	uint16 i = 0;
	uint16 Start_Postion=0;
	SignalSample_count = 0;
	Zero_Count = 0;
	Forever_Value = 2;
	/* 第一步:扫描电机转到中间位置 */
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
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_DetectionPosition();
	/* 第三步:数据处理得到杯子检测的起始位置*/
	/* 1.对数据进行移动平均 */
	SignalSample_Moving_Average_Data(SignalProcess_sampleBuffer,SIGNALSAMPLE_MAX_COUNT,15);
	Get_sampleBuffer_Boundary_Value();
	Get_sampleBuffer_Max_Value();
	/* 2.有无杯子判断 */
	if((max < 1000) || (BOUNDARY_VALUE > 900))
	{
		Confirm_CUP = NO_CUP;
		return Confirm_CUP;
	}
	else
	{
		Confirm_CUP = 1;
	}

	/* 3.减去临界值，获得易处理的数据 */
	for(i = 0;i < SIGNALSAMPLE_MAX_COUNT;i++)
	{
		if(SignalProcess_sampleBuffer[i]  == 0)
		{
			Zero_Count++;
		}

		if((SignalProcess_sampleBuffer[i] == SignalProcess_sampleBuffer[i-1]) && (SignalProcess_sampleBuffer[i-1] == SignalProcess_sampleBuffer[i-2]))
		{
			Forever_Value++;
		}
	}

	/* 3.减去临界值，获得易处理的数据 */
	for(i = 0;i < SIGNALSAMPLE_MAX_COUNT;i++)
	{
		if(SignalProcess_sampleBuffer[i] < BOUNDARY_VALUE)
		{
			SignalProcess_sampleBuffer[i] = 0;
		}
		else
		{
				SignalProcess_sampleBuffer[i] = SignalProcess_sampleBuffer[i] - BOUNDARY_VALUE;
		}
	}

	/* 4.得到杯子的起始位置 */
	for(i = 0;i < SIGNALSAMPLE_MAX_COUNT;i++)
	{
		if((SignalProcess_sampleBuffer[i] == 0) && (SignalProcess_sampleBuffer[i+1] > 0) )
		{
			Start_Postion = i;
			break;
		}
	}

	if(Zero_Count)
	{
		Start_Postion += ((187 - Forever_Value)/2);
	}
	else
	{
		Start_Postion = (Start_Postion + 93) - (Forever_Value+3)/2;
	}

	if(Start_Postion >= 511 )
	{
		Start_Postion = 511;
	}

	if(Start_Postion <= 4 )
	{
		Start_Postion = 2;
	}

	return Start_Postion;
}

/******************************************************************************/
uint16 Get_sampleBuffer_Boundary_Value(void)
{
	uint16 i = 0;
	BOUNDARY_VALUE = 2500;
	for(i = 0;i < SIGNALSAMPLE_MAX_COUNT;i++)
	{
		if(BOUNDARY_VALUE >= SignalProcess_sampleBuffer[i])
		{
			BOUNDARY_VALUE = SignalProcess_sampleBuffer[i];
		}
	}
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
void Result_Judge(void)
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
