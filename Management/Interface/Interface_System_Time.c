/*
 * Interface_System_Time.c
 *
 *  Created on: 2018Äê6ÔÂ19ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "Interface_System_Time.h"

uint16 UI_WindowBlocks_System_Time = 0;
uint8 Confirm_Count = 1;

START_DATA start_data = {2018,6,19,17,53,25};

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
	UI_WindowBlocks_System_Time = sizeof(UI_WindowBlocksAttrArray_System_Time) >> 2;
	UI_Draw_Window_System_Time(UI_WindowBlocks_System_Time);
	Line_between();
	key_state_confirm = 0;
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
	uint8 state = 0;
	if(key_state)
	{
		switch(key_state_confirm)
		{
			case 0:
				switch(Confirm_Count)
				{
					case 1:
						if((start_data.year < 2128) && (start_data.year > 1900))
						{
							if(Key_control == 2)
							{
								start_data.year--;
							}

							if(Key_control == 1)
							{
								start_data.year++;
							}
						}
						else
						{
							start_data.year = 2127;
						}
						Display_Time_Set();
					break;
					case 2:
						if((start_data.month < 12) && (start_data.month > 0))
						{
							if(Key_control == 2)
							{
								start_data.month--;
							}

							if(Key_control == 1)
							{
								start_data.month++;
							}
						}
						else
						{
							start_data.month = 1;
						}
						Display_Time_Set();
					break;
					case 3:
						if((start_data.day < 31) && (start_data.day > 0))
						{
							if(Key_control == 2)
							{
								start_data.day--;
							}

							if(Key_control == 1)
							{
								start_data.day++;
							}
						}
						else
						{
							start_data.day = 1;
						}
						Display_Time_Set();
					break;
					case 4:
						if((start_data.hour < 24) && (start_data.hour >= 0))
						{
							if(Key_control == 2)
							{
								start_data.hour--;
							}

							if(Key_control == 1)
							{
								start_data.hour++;
							}
						}
						else
						{
							start_data.hour = 0;
						}
						Display_Time_Set();
					break;
					case 5:
						if((start_data.min < 59) && (start_data.min >= 0))
						{
							if(Key_control == 2)
							{
								start_data.min--;
							}

							if(Key_control == 1)
							{
								start_data.min++;
							}
						}
						else
						{
							start_data.min = 0;
						}
						Display_Time_Set();
					break;
					case 6:
						if((start_data.sec < 59) && (start_data.sec >= 0))
						{
							if(Key_control == 2)
							{
								start_data.sec--;
							}

							if(Key_control == 1)
							{
								start_data.sec++;
							}
						}
						else
						{
							start_data.sec = 0;
						}
						Display_Time_Set();
					break;
					default:
						break;
				}
			break;
			case 1:
				Confirm_Count++;
				key_state_confirm = DISABLE;
				Display_Time_Set();
			break;

			default:
				break;
		}
	}

	if(Confirm_Count > 6)
	{
		UI_state = UI_STATE_SETTING;
		Confirm_Count = 1;
		key_state_confirm = DISABLE;
		Key_control = 1;
		SystemManage_RTC_Set(start_data.year, start_data.month,start_data.day,start_data.hour,
				start_data.min,start_data.sec);
		UI_Draw_Status_Bar();
	}
	return state;
}

/******************************************************************************/
void Display_Time_Set (void)
{
	Display_Time = 0;
	switch(Confirm_Count)
	{
	case 6:
		sprintf(Minute,"%02d",start_data.min);
		DisplayDriver_Text16_B(57,97,Black,White,Minute);
	case 5:
		sprintf(Hour,"%02d",start_data.hour);
		DisplayDriver_Text16_B(32,97,Black,White,Hour);
	case 4:
		sprintf(Day,"%02d",start_data.day);
		DisplayDriver_Text16_B(90,67,Black,White,Day);
	case 3:
		sprintf(Month,"%02d",start_data.month);
		DisplayDriver_Text16_B(65,67,Black,White,Month);
	case 2:
		sprintf(Year,"%02d",start_data.year);
		DisplayDriver_Text16_B(24,67,Black,White,Year);
	case 1:
		break;
	}


	switch(Confirm_Count)
	{
	case 1:
		sprintf(Year,"%02d",start_data.year);
		DisplayDriver_Text16_B(24,67,Black,Magenta,Year);
		break;
	case 2:
		sprintf(Month,"%02d",start_data.month);
		DisplayDriver_Text16_B(65,67,Black,Magenta,Month);
		break;
	case 3:
		sprintf(Day,"%02d",start_data.day);
		DisplayDriver_Text16_B(90,67,Black,Magenta,Day);
		break;
	case 4:
		sprintf(Hour,"%02d",start_data.hour);
		DisplayDriver_Text16_B(32,97,Black,Magenta,Hour);
		break;
	case 5:
		sprintf(Minute,"%02d",start_data.min);
		DisplayDriver_Text16_B(57,97,Black,Magenta,Minute);
		break;
	case 6:
		sprintf(Sec,"%02d",start_data.sec);
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
	sprintf(Year,"%02d",start_data.year);
	DisplayDriver_Text16_B(24,67,Black,Magenta,Year);
	sprintf(Month,"%02d",start_data.month);
	DisplayDriver_Text16_B(65,67,Black,White,Month);
	sprintf(Day,"%02d",start_data.day);
	DisplayDriver_Text16_B(90,67,Black,White,Day);
	sprintf(Hour,"%02d",start_data.hour);
	DisplayDriver_Text16_B(32,97,Black,White,Hour);
	sprintf(Minute,"%02d",start_data.min);
	DisplayDriver_Text16_B(57,97,Black,White,Minute);
	sprintf(Sec,"%02d",start_data.sec);
	DisplayDriver_Text16_B(82,97,Black,White,Sec);
//	key_state = DISABLE;
	Display_Time = 1;
}


