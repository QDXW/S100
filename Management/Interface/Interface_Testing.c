/*
 * Interface_Testing.c
 *
 *  Created on: 2018��2��26��
 *      Author: Administrator
 */	 		 
/******************************************************************************/
#include "Interface_Testing.h"
#include "Signalprocess.h"

/******************************************************************************/
uint16 max = 0,Max_Postion = 0;
uint8 NowCup_Count = 0,Storage_Data_Conut = 0;
extern uint8  Cup_Count;
uint16 BOUNDARY_VALUE = 2500;
extern uint16 SignalSample_count;
extern uint16 SignalProcess_sampleBuffer[SIGNALSAMPLE_MAX_COUNT];

uint8 SignalProcess[1024] = {0};

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
		16,   63,
		96, 15,
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

	if(temp <= 3.67)
	{
		Display_Time = 0;
		Lcd_ColorBox(0,20,128, 140,White);
		DisplayDriver_Text16(11, 80, Red,"Low battery!");
		Display_Time = 1;
		UI_state = UI_STATE_MAIN_WINDOW;
		Delay_ms_SW(1500);
		return state;
	}

	UI_WindowBlocks_Testing = sizeof(UI_WindowBlocksAttrArray_Testing) >> 2;
	UI_Draw_Window_Testing(UI_WindowBlocks_Testing);
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
	uint8 j = 0,Step_Count = 28,Step_Start = 16,i = 0;
	NowCup_Count = 0;
	Storage_Data_Conut = 0;
	Storage_Time();

	/* ����ṹ����Ϣ  */
	for(i = 0;i < 12;i++)
	{
		memset(&Storage_Data.CH_data[i].Result[0],0,6);
	}

	/* ��ά����Ϣ��ֲ���㷨�ṹ�� */
	QR_Date_SignalProcess_Alg_data();

	/* ���������洢*/
	for(NowCup_Count = 0;NowCup_Count < 8;NowCup_Count++)
	{
		if(QR_Date.ch_data[NowCup_Count].Switch_Bool)
		{
			/* ���� */
			SignalSample_SampleStrip();

			/* �����㷨 */
			memcpy(&SignalProcess_Alg_data.sampleBuffer[0], SignalProcess_sampleBuffer, SignalSample_count << 1);
			SignalProcess_Alg_data.sampleNumber = SignalSample_count;
			SignalProcess_Run();

			/* �ж���� */
			Result_Judge();
			Storage_Data_Conut += 1;

			/* ������� */
			memset(SignalProcess,0,500);
			memcpy(SignalProcess, &SignalProcess_sampleBuffer[0], SignalSample_count<< 1);
			HostComm_Cmd_Send_RawData(SignalSample_count << 1, SignalProcess);
			Delay_ms_SW(50);
			HostComm_Cmd_Send_C_T(SignalProcess_Alg_data.calcInfo.areaC, SignalProcess_Alg_data.calcInfo.areaT);
		}

		/* ת�����ת��30�� */
		if(NowCup_Count%2 == 0)
		{
			RotationMotor_Input_StepDrive(Foreward_Rotation,43);
		}
		else
		{
			RotationMotor_Input_StepDrive(Foreward_Rotation,42);
		}

		/* ������ˢ�� */
		for(j = Step_Start;j < Step_Count;j++)		/* ÿ�ν�������ʮ�� */
		{
			Delay_ms(30);
			Display_Time = 0;
			Lcd_ColorBox(j,63,1,15,BACKCOLOR_CONTENT_BAR);
			Display_Time = 1;
		}
		Step_Start = Step_Count;					/* ���ý�������ʼλ�� */
		Step_Count += 12;
	}
}

/******************************************************************************/
uint16 Get_Start_Postion(void)
{
	uint8 Judge_Flag = 2,Trend_FLag = 0,Judge_Trend = 0;
	uint16 Start_Postion = 0,Postion_Down = 0,i = 0,Postion_Up = 0,Forever_Value = 800;

	while(Judge_Flag--)
	{
		if(0 == Judge_Flag)
		{
			RotationMotor_Input_StepDrive(Foreward_Rotation,30);
		}

		Acquisition_StartSignal();

		/* 3.�ҵ������ص�λ�� */
		for(i = 0;i < 511;i++)
		{
			if((SignalProcess_sampleBuffer[i] < Forever_Value) && (SignalProcess_sampleBuffer[i+1] >= Forever_Value))
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

		/* 3.�ҵ��½��ص�λ�� */
		for(i = 0;i < 511;i++)
		{
			if((SignalProcess_sampleBuffer[i] > Forever_Value) && (SignalProcess_sampleBuffer[i+1] <= Forever_Value))
			{
				Trend_FLag = 0;
				if(Check_Trend(&SignalProcess_sampleBuffer[0],i,Trend_FLag))
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

		if (0 == Postion_Up)
		{
			Postion_Up = 1;
		}

		if (0 == Postion_Down)
		{
			Postion_Down = 1;
		}

		if(Judge_Trend)
		{
			if((Calculate_Postion_Up(&SignalProcess_sampleBuffer[0],Postion_Up)) == Calculate_Postion_Down(&SignalProcess_sampleBuffer[0],Postion_Down))
			{
				Judge_Flag = 0;
				Start_Postion = Calculate_Postion_Up(&SignalProcess_sampleBuffer[0],Postion_Up);
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

	return Start_Postion;
}

/******************************************************************************/
void Acquisition_StartSignal(void)
{
	uint16 i = 0;
	SignalSample_count = 0;
	/* ��һ��:ɨ����ת���м�λ�� */
	memset(&SignalProcess_sampleBuffer[0],0,2*512);
	ScanMotorDriver_PositionSensor_Int_Enable();
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_CentrePosition();
	SignalSample_Sample_EnterCriticalArea();

	/* �ڶ���:��ת360�ȣ����Ʊ��Ӳɼ��ź� */
	for(i = 0;i < Whole_Circle;i++)
	{
		RotationMotor_Input_StepDrive(Foreward_Rotation,1);
		SignalProcess_sampleBuffer[SignalSample_count++]
									= SignalProcess_Collecting_Data();
	}

	memset(SignalProcess,0,1024);
	memcpy(SignalProcess, &SignalProcess_sampleBuffer[0], 1024);
	HostComm_Cmd_Send_RawData(1024, SignalProcess);

	SignalSample_Sample_ExitCriticalArea();
	ScanMotorDriver_Goto_BasePosition();
	ScanMotorDriver_Goto_DetectionPosition();
	/* ������:���ݴ���õ����Ӽ�����ʼλ��*/
	Get_sampleBuffer_Boundary_Value();
	Get_sampleBuffer_Max_Value();

	/* 2.���ޱ����ж� */
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
	/*�����ݽ����ƶ�ƽ��*/
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

	i = (Postion + 175);
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
				Confirm_Postion = 0;
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
				Confirm_Postion = 0;
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
				Confirm_Postion = 0;
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
		if((Start_Postion + 6) < 511)
		{
			for(i = Start_Postion;i < (Start_Postion + 6);i++)
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
			for(i = Start_Postion;i < 511;i++)
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
