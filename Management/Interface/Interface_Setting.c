/*
 * Interface_Setting.c
 *
 *  Created on: 2018年2月24日
 *      Author: Administrator
 */

/******************************************************************************/
#include "Interface_Setting.h"

/******************************************************************************/
block_attr_Setting block_Setting_1 = {
	ENABLE,									/* Parting line */
	{
		64,  20,
		64,  160,
		Thint_Blue
	},

	ENABLE,								/*Display Picture*/
	{
		gImage_PIC_System_Time,
		71,  24,
		45, 45
	},
};

/******************************************************************************/
block_attr_Setting block_Setting_2 = {
	ENABLE,									/* Parting line */
	{
		0,    90,
		128,  90,
		Light_Gray
	},

	ENABLE,								/*Display Picture*/
	{
		gImage_PIC_About_Machine,
		10, 24,
		45,  45
	},
};

/******************************************************************************/
block_attr_Setting block_Setting_3 = {
	ENABLE,									/* Parting line */
	{
		0,    91,
		128,  91,
		Thint_Blue
	},

	ENABLE,								/*Display Picture*/
	{
		gImage_Blutooth_Open,
		10,  92,
		45,  45
	},
};

/******************************************************************************/
block_attr_Setting block_Setting_4 = {
	ENABLE,									/* Parting line */
	{
		63,  20,
		63,  160,
		Light_Gray
	},

	DISABLE,								/*Display Picture*/
	{0},
};

/******************************************************************************/
block_attr_Setting block_Setting_Calibration = {
	DISABLE,								/* Parting line */
	{0},

	ENABLE,									/*Display Picture*/
	{
		gImage_Calibration,
		71,  92,
		45,  45
	},
};

/******************************************************************************/
block_attr_Setting* UI_WindowBlocksAttrArray_Setting[] = {/* Window: Standard entry */
		&block_Setting_1,
		&block_Setting_2,
		&block_Setting_3,
		&block_Setting_4,
		&block_Setting_Calibration,
};

/******************************************************************************/
void UI_Draw_Block_Setting(block_attr_Setting* block);
void UI_Draw_Block_Setting_font(block_attr_Setting_font* block);

/******************************************************************************/
uint8 Interface_Setting(uint16 KeyCode)
{
	uint8 state = 0;
	Exti_lock = DISABLE;
	Interface_Key = 5;
	Display_Time = 0;
	Lcd_ColorBox(0,20,128,140,BACKCOLOR_CONTENT_BACK);
	Display_Time = 1;
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Setting) >> 2;
	UI_Draw_Window_Setting(UI_WindowBlocks);
	UI_state = UI_STATE_SETTING_FONT;
	return state;
}

/******************************************************************************/
void UI_Draw_Window_Setting(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		UI_Draw_Block_Setting(UI_WindowBlocksAttrArray_Setting[blockIndex]);
	}
}

/******************************************************************************/
void UI_Draw_Block_Setting(block_attr_Setting* block)
{
	Display_Time = 0;
	if (block->line_enabled)					/* 3.Parting line */
	{
		DisplayDriver_DrawLine(block->Parting_line_attr.startX,
				block->Parting_line_attr.startY,
				block->Parting_line_attr.endX,
				block->Parting_line_attr.endY,
				block->Parting_line_attr.color);
	}

	if (block->pic_enabled)				/* 2. Draw picture */
	{
		DisplayDriver_DrawPic(block->pic_attr.offsetX,
				block->pic_attr.offsetY, block->pic_attr.width,
				block->pic_attr.height,block->pic_attr.src);
	}
	Display_Time = 1;
}

/******************************************************************************/
uint8 Interface_Setting_font(uint16 KeyCode)
{
	uint8 state = 0,status = 0;
	Exti_lock = DISABLE;
	Interface_Key = 5;
	Display_Time = 0;
	Lcd_ColorBox(14,71,40,18,BACKCOLOR_CONTENT_BACK);
	Lcd_ColorBox(14,141,48,18,BACKCOLOR_CONTENT_BACK);

	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(12,71,White,Magenta,"About");
		}
		else
		{
			DisplayDriver_Text16_B(12,71,White,Baby_Blue,"About");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(80,71,White,Magenta,"Time");
		}
		else
		{
			DisplayDriver_Text16_B(80,71,White,Baby_Blue,"Time");
		}

		if(Bluetooth_switch)
		{
			DisplayDriver_Text16_B(24,141,White,White,"ON");
		}
		else
		{
			DisplayDriver_Text16_B(20,141,White,Baby_Blue,"OFF");
		}

		if(Key_control == 3)
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(24,141,White,Magenta,"ON");
				status = 0;
			}
			else
			{
				DisplayDriver_Text16_B(20,141,White,Magenta,"OFF");
				status = 1;
			}
			Storage_Write(&status, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*4),1);
		}
		else
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(24,141,White,White,"ON");
			}
			else
			{
				DisplayDriver_Text16_B(20,141,White,Baby_Blue,"OFF");
			}
		}

		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(64,141,White,Magenta,"Language");
		}
		else
		{
			DisplayDriver_Text16_B(64,141,White,Baby_Blue,"Language");
		}
		break;

	case DISPLAY_FONT_CHINESE:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(0,71,White,Magenta,"关于本机");
		}
		else
		{
			DisplayDriver_Text16_B(0,71,White,Baby_Blue,"关于本机");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(64,71,White,Magenta,"系统时间");
		}
		else
		{
			DisplayDriver_Text16_B(64,71,White,Baby_Blue,"系统时间");
		}

		if(Key_control == 3)
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(16,141,White,Magenta,"打开");
				status = 0;
			}
			else
			{
				DisplayDriver_Text16_B(16,141,White,Magenta,"关闭");
				status = 1;
			}
			Storage_Write(&status, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*4),1);
		}
		else
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(16,141,White,White,"打开");
			}
			else
			{
				DisplayDriver_Text16_B(16,141,White,Baby_Blue,"关闭");
			}
		}

		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(80,141,White,Magenta,"语言");
		}
		else
		{
			DisplayDriver_Text16_B(80,141,White,Baby_Blue,"语言");
		}
		break;

	case DISPLAY_FONT_GERMAN:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(16,71,White,Magenta,"$ber");
		}
		else
		{
			DisplayDriver_Text16_B(16,71,White,Baby_Blue,"$ber");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(80,71,White,Magenta,"Zeit");
		}
		else
		{
			DisplayDriver_Text16_B(80,71,White,Baby_Blue,"Zeit");
		}

		if(Key_control == 3)
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(24,141,White,Magenta,"An");
				status = 0;
			}
			else
			{
				DisplayDriver_Text16_B(20,141,White,Magenta,"Aus");
				status = 1;
			}

			Storage_Write(&status, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*4),1);
		}
		else
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(24,141,White,White,"An");
			}
			else
			{
				DisplayDriver_Text16_B(20,141,White,Baby_Blue,"Aus");
			}
		}
		break;

//		if(Key_control == 4)
//		{
//			DisplayDriver_Text16_B(68,141,White,Magenta,"Language");
//		}
//		else
//		{
//			DisplayDriver_Text16_B(68,141,White,Baby_Blue,"Language");
//		}
//		break;

	default:
		break;
	}

	Display_Time = 1;
	Exti_lock = ENABLE;
	Display_Battery = 1;
	key_state = DISABLE;
	UI_state = UI_STATE_SETTING_KEY;
	return state;
}

/******************************************************************************/
uint8 Interface_Bluet_switch_Process(uint16 blockNum)
{
	uint8 state = 0;
	if(!Bluetooth_switch)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_4);
		Bluetooth_switch = 1;
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_4);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		Bluetooth_switch = 0;
	}
	UI_state = UI_STATE_SETTING_KEY;
	return state;
}

/******************************************************************************/
uint8 Interface_Settings_key_Process(uint16 blockNum)
{
	uint8 state = 0;
	Interface_Key = 5;
	if(key_state)
	{
		switch(key_state_confirm)
		{
		case 0:
//			if(5 == Key_control)
//			{
//				UI_state = UI_STATE_LANGUAGE_PROCESS;
//				UI_Interface_Language();
//				Key_control = 1;
//			}
//			else
			{
				UI_state = UI_STATE_SETTING_FONT;
				if(!Key_control)
				{
					UI_state = UI_STATE_MAIN_WINDOW;
					Key_control = 1;
				}
			}
			key_state_confirm = DISABLE;
		break;

		case 1:
			switch(Key_control)
			{
				case 1:
					UI_state = UI_STATE_ABOUT_MACHINE;
					key_state_confirm = DISABLE;
					Key_control = 1;
				break;
				case 2:
					UI_state = UI_STATE_SYSTEM_TIME;
					key_state_confirm = DISABLE;
					Key_control = 1;
				break;

				case 3:
					UI_state = UI_STATE_BLUET_SWITCH_PROCESS;
					key_state_confirm = DISABLE;
					Key_control = 3;
				break;

				case 4:
					UI_state = UI_STATE_LANGUAGE_PROCESS;
					UI_Interface_Language();
					key_state_confirm = DISABLE;
					Key_control = 1;
				break;

				default:
				break;
			}
			break;

			default:
			break;
		}
	}
	return state;
}

/******************************************************************************/
void UI_Interface_Language(void)
{
	Display_Time = 0;
	Lcd_ColorBox(0,20,128,140,BACKCOLOR_CONTENT_BACK);
	Lcd_ColorBox(0,20,128,140,BACKCOLOR_CONTENT_BACK);
	Lcd_ColorBox(0,26,128,128,Light_Gray);
	Display_Time = 1;
}
