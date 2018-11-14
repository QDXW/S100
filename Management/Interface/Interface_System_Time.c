/*
 * Interface_System_Time.c
 *
 *  Created on: 2018Äê6ÔÂ19ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "Interface_System_Time.h"

/******************************************************************************/
block_attr_System_Time block_System_Time_Back = {
	ENABLE,								/*Interface Setting rect*/
	{
		0,   20,
		128, 140,
		BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_System_Time block_System_Time_White1 = {
	ENABLE,								/*Interface Setting rect*/
	{
		20,   65,
		90,   20,
		White
	},
};

/******************************************************************************/
block_attr_System_Time block_System_Time_White2 = {
	ENABLE,								/*Interface Setting rect*/
	{
		28,   95,
		75,  20,
		White
	},
};

/******************************************************************************/
block_attr_System_Time* UI_WindowBlocksAttrArray_System_Time[] = {/* Window: Standard entry */
		&block_System_Time_Back,
		&block_System_Time_White1,
		&block_System_Time_White2,
};

/******************************************************************************/
void UI_Draw_Block_System_Time(block_attr_System_Time* block);

/******************************************************************************/
uint8 Interface_System_Time(uint16 KeyCode)
{
	uint8 state = 0;
	Exti_lock = DISABLE;
	Interface_Key = 7;
	Time_Count = 1;
	key_state_confirm = 0;
	Lcd_ColorBox(0,20,128,140,BACKCOLOR_CONTENT_BACK);
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_System_Time) >> 2;
	UI_Draw_Window_System_Time(UI_WindowBlocks);
	Line_between();
	Exti_lock = ENABLE;
	UI_state = UI_STATE_TIME_PROCESS;

	return state;
}

/******************************************************************************/
void UI_Draw_Window_System_Time(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		UI_Draw_Block_System_Time(UI_WindowBlocksAttrArray_System_Time[blockIndex]);
	}
}

/******************************************************************************/
void UI_Draw_Block_System_Time(block_attr_System_Time* block)
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
void Line_between(void)
{
	Display_Time = 0;
	DisplayDriver_DrawLine(61,65,61,85,Light_Gray);
	DisplayDriver_DrawLine(86,65,86,85,Light_Gray);
	DisplayDriver_DrawLine(53,95,53,115,Light_Gray);
	DisplayDriver_DrawLine(78,95,78,115,Light_Gray);
	Display_Time_Start();
	Display_Time = 1;
}

/******************************************************************************/
uint8 Interface_Time_Process(uint16 KeyCode)
{
	uint8 MBuffer[10] = {0};
	uint8 state = 0;
	if(key_state)
	{
		switch(key_state_confirm)
		{
			case 0:
				switch(Time_Count)
				{
					case 1:
						if((SystemManage_RecordTime.year < 2128) && (SystemManage_RecordTime.year > 1900))
						{
							if(Key_control == 2)
							{
								SystemManage_RecordTime.year--;
							}

							if(Key_control == 1)
							{
								SystemManage_RecordTime.year++;
							}
						}
						else
						{
							SystemManage_RecordTime.year = 2127;
						}
						Display_Time_Set();
					break;
					case 2:
						if((SystemManage_RecordTime.month < 12) && (SystemManage_RecordTime.month > 0))
						{
							if(Key_control == 2)
							{
								SystemManage_RecordTime.month--;
								if(SystemManage_RecordTime.month < 1)
								{
									SystemManage_RecordTime.month = 1;
								}
							}

							if(Key_control == 1)
							{
								SystemManage_RecordTime.month++;
							}
						}
						else
						{
							SystemManage_RecordTime.month = 1;
						}
						Display_Time_Set();
					break;
					case 3:
						if((SystemManage_RecordTime.day < 31) && (SystemManage_RecordTime.day > 0))
						{
							if(Key_control == 2)
							{
								SystemManage_RecordTime.day--;
								if(SystemManage_RecordTime.day < 1)
								{
									SystemManage_RecordTime.day = 1;
								}
							}

							if(Key_control == 1)
							{
								SystemManage_RecordTime.day++;
							}
						}
						else
						{
							SystemManage_RecordTime.day = 1;
						}
						Display_Time_Set();
					break;
					case 4:
						if((SystemManage_RecordTime.hour < 24) && (SystemManage_RecordTime.hour >= 0))
						{
							if(Key_control == 2)
							{
								SystemManage_RecordTime.hour--;
								if(SystemManage_RecordTime.hour > 24)
								{
									SystemManage_RecordTime.hour = 0;
								}

							}

							if(Key_control == 1)
							{
								SystemManage_RecordTime.hour++;
							}
						}
						else
						{
							SystemManage_RecordTime.hour = 0;
						}
						Display_Time_Set();
					break;
					case 5:
						if((SystemManage_RecordTime.min < 59) && (SystemManage_RecordTime.min >= 0))
						{
							if(Key_control == 2)
							{
								SystemManage_RecordTime.min--;
								if(SystemManage_RecordTime.min > 59)
								{
									SystemManage_RecordTime.min = 0;
								}
							}

							if(Key_control == 1)
							{
								SystemManage_RecordTime.min++;
							}
						}
						else
						{
							SystemManage_RecordTime.min = 0;
						}
						Display_Time_Set();
					break;
					case 6:
						if((SystemManage_RecordTime.sec < 59) && (SystemManage_RecordTime.sec >= 0))
						{
							if(Key_control == 2)
							{
								SystemManage_RecordTime.sec--;
								if(SystemManage_RecordTime.sec > 59)
								{
									SystemManage_RecordTime.sec = 0;
								}
							}

							if(Key_control == 1)
							{
								SystemManage_RecordTime.sec++;
							}
						}
						else
						{
							SystemManage_RecordTime.sec = 0;
						}
						Display_Time_Set();
					break;
					default:
						break;
				}
			break;
			case 1:
				Time_Count++;
				key_state_confirm = DISABLE;
				Display_Time_Set();
			break;

			default:
				break;
		}
	}

	if(Time_Count > 6)
	{
		UI_state = UI_STATE_SETTING;
		Time_Count = 1;
		key_state_confirm = DISABLE;
		Key_control = 1;
		SystemManage_RTC_Set(SystemManage_RecordTime.year, SystemManage_RecordTime.month,SystemManage_RecordTime.day,SystemManage_RecordTime.hour,
				SystemManage_RecordTime.min,SystemManage_RecordTime.sec);
		UI_Draw_Status_Bar();
	}

	return state;
}

/******************************************************************************/
void Display_Time_Set (void)
{
	Display_Time = 0;
	switch(Time_Count)
	{
	case 6:
		sprintf(Minute,"%02d",SystemManage_RecordTime.min);
		DisplayDriver_Text16_B(57,97,Black,White,Minute);
	case 5:
		sprintf(Hour,"%02d",SystemManage_RecordTime.hour);
		DisplayDriver_Text16_B(32,97,Black,White,Hour);
	case 4:
		sprintf(Day,"%02d",SystemManage_RecordTime.day);
		DisplayDriver_Text16_B(90,67,Black,White,Day);
	case 3:
		sprintf(Month,"%02d",SystemManage_RecordTime.month);
		DisplayDriver_Text16_B(65,67,Black,White,Month);
	case 2:
		sprintf(Year,"%02d",SystemManage_RecordTime.year);
		DisplayDriver_Text16_B(24,67,Black,White,Year);
	case 1:
		break;
	default:
		break;
	}


	switch(Time_Count)
	{
	case 1:
		sprintf(Year,"%02d",SystemManage_RecordTime.year);
		DisplayDriver_Text16_B(24,67,Black,Magenta,Year);
		break;
	case 2:
		sprintf(Month,"%02d",SystemManage_RecordTime.month);
		DisplayDriver_Text16_B(65,67,Black,Magenta,Month);
		break;
	case 3:
		sprintf(Day,"%02d",SystemManage_RecordTime.day);
		DisplayDriver_Text16_B(90,67,Black,Magenta,Day);
		break;
	case 4:
		sprintf(Hour,"%02d",SystemManage_RecordTime.hour);
		DisplayDriver_Text16_B(32,97,Black,Magenta,Hour);
		break;
	case 5:
		sprintf(Minute,"%02d",SystemManage_RecordTime.min);
		DisplayDriver_Text16_B(57,97,Black,Magenta,Minute);
		break;
	case 6:
		sprintf(Sec,"%02d",SystemManage_RecordTime.sec);
		DisplayDriver_Text16_B(82,97,Black,Magenta,Sec);
		break;
	default:
		break;
	}
	key_state = DISABLE;
	Display_Time = 1;
}

/******************************************************************************/
void Display_Time_Start (void)
{
	Display_Time = 0;
	sprintf(Year,"%02d",SystemManage_RecordTime.year);
	DisplayDriver_Text16_B(24,67,Black,Magenta,Year);
	sprintf(Month,"%02d",SystemManage_RecordTime.month);
	DisplayDriver_Text16_B(65,67,Black,White,Month);
	sprintf(Day,"%02d",SystemManage_RecordTime.day);
	DisplayDriver_Text16_B(90,67,Black,White,Day);
	sprintf(Hour,"%02d",SystemManage_RecordTime.hour);
	DisplayDriver_Text16_B(32,97,Black,White,Hour);
	sprintf(Minute,"%02d",SystemManage_RecordTime.min);
	DisplayDriver_Text16_B(57,97,Black,White,Minute);
	sprintf(Sec,"%02d",SystemManage_RecordTime.sec);
	DisplayDriver_Text16_B(82,97,Black,White,Sec);
	Display_Time = 1;
}
