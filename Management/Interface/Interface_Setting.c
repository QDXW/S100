/*
 * Interface_Setting.c
 *
 *  Created on: 2018Äê2ÔÂ24ÈÕ
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

	DISABLE,								/*Display HZ16X8*/
	{
		"Time",
		74,  71,
		White,BACKCOLOR_CONTENT_BAR,
		White
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

	DISABLE,								/*Display HZ16X8*/
	{
		"About",
		12,  71,
		White,BACKCOLOR_CONTENT_BAR,
		White
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

	DISABLE,								/*Display HZ16X8*/
	{
		"BLT",
		20,  141,
		White,BACKCOLOR_CONTENT_BAR,
		White
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

	DISABLE,								/*Display HZ16X8*/
	{
		"BLT",
		20,  141,
		White,BACKCOLOR_CONTENT_BAR,
		White
	},

	DISABLE,								/*Display Picture*/
	{0},
};

/******************************************************************************/
block_attr_Setting_font Setting_block_font = {
	ENABLE,								/*Display HZ16X8*/
	{
		"About",
		12,  71,
		White,Magenta,
		Baby_Blue
	},
	ENABLE,								/*Display HZ16X8*/
	{
		"Time",
		80,  71,
		White,Magenta,
		Baby_Blue
	},

	DISABLE,								/*Display HZ16X8*/
	{
		"NO",
		24,  141,
		White,Magenta,
		White
	},
};

/******************************************************************************/
block_attr_Setting_font Setting_block_font_2 = {
	ENABLE,								/*Display HZ16X8*/
	{
		"About",
		12,  71,
		White,Magenta,
		Baby_Blue
	},
	ENABLE,								/*Display HZ16X8*/
	{
		"Time",
		80,  71,
		White,Magenta,
		Baby_Blue
	},

	ENABLE,								/*Display HZ16X8*/
	{
		"OFF",
		20,  141,
		White,Magenta,
		Baby_Blue
	},

};

/******************************************************************************/
block_attr_Setting* UI_WindowBlocksAttrArray_Setting[] = {/* Window: Standard entry */
		&block_Setting_1,
		&block_Setting_2,
		&block_Setting_3,
		&block_Setting_4,
};

block_attr_Setting_font* UI_WindowBlocksAttrArray_Setting_font[] = {
		&Setting_block_font,
};

block_attr_Setting_font* UI_WindowBlocksAttrArray_Setting_font_2[] = {
		&Setting_block_font_2,
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
	Lcd_ColorBox(0,20,128,140,BACKCOLOR_CONTENT_BACK);
	UI_WindowBlocks_Setting = sizeof(UI_WindowBlocksAttrArray_Setting) >> 2;
	UI_Draw_Window_Setting(UI_WindowBlocks_Setting);
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

	if (block->char_enabled)				/* 2. Draw character */
	{

		DisplayDriver_Text16_B(
				block->char_attr.offsetX,block->char_attr.offsetY,
				block->char_attr.color,block->char_attr.faceColor,
				block->char_attr.str);
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
	uint8 state = 0;
	Exti_lock = DISABLE;
	Interface_Key = 5;
	if(Bluetooth_switch)
	{
		UI_WindowBlocks_Setting = sizeof(UI_WindowBlocksAttrArray_Setting_font) >> 2;
		UI_Draw_Window_Setting_font(UI_WindowBlocks_Setting);
	}
	else
	{
		UI_WindowBlocks_Setting = sizeof(UI_WindowBlocksAttrArray_Setting_font_2) >> 2;
		UI_Draw_Window_Setting_font(UI_WindowBlocks_Setting);
	}
	Exti_lock = ENABLE;
	UI_state = UI_STATE_KEY_STATE;
	return state;
}
/******************************************************************************/
void UI_Draw_Window_Setting_font(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		if(Bluetooth_switch)
		{
			Lcd_ColorBox(20,141,26,18,BACKCOLOR_CONTENT_BACK);
			UI_Draw_Block_Setting_font(UI_WindowBlocksAttrArray_Setting_font[blockIndex]);
		}
		else
		{
			UI_Draw_Block_Setting_font(UI_WindowBlocksAttrArray_Setting_font_2[blockIndex]);
		}
	}
}

/******************************************************************************/
void UI_Draw_Block_Setting_font(block_attr_Setting_font* block)
{
	Display_Time = 0;
	if (block->char_enabled)				/* 2. Draw character */
	{
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(
					block->char_attr.offsetX,block->char_attr.offsetY,
					block->char_attr.color,block->char_attr.backColor,
					block->char_attr.str);
		}
		else
		{
			DisplayDriver_Text16_B(
					block->char_attr.offsetX,block->char_attr.offsetY,
					block->char_attr.color,block->char_attr.faceColor,
					block->char_attr.str);
		}
	}

	if (block->char1_enabled)				/* 2. Draw character */
	{
		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(
					block->char1_attr.offsetX,block->char1_attr.offsetY,
					block->char1_attr.color,block->char1_attr.backColor,
					block->char1_attr.str);
		}
		else
		{
			DisplayDriver_Text16_B(
					block->char1_attr.offsetX,block->char1_attr.offsetY,
					block->char1_attr.color,block->char1_attr.faceColor,
					block->char1_attr.str);
		}
	}

	if (block->char1_enabled)				/* 2. Draw character */
	{
		if(Key_control == 3)
		{
			DisplayDriver_Text16_B(
					block->char2_attr.offsetX,block->char2_attr.offsetY,
					block->char2_attr.color,block->char2_attr.backColor,
					block->char2_attr.str);
		}
		else
		{
			DisplayDriver_Text16_B(
					block->char2_attr.offsetX,block->char2_attr.offsetY,
					block->char2_attr.color,block->char2_attr.faceColor,
					block->char2_attr.str);
		}
	}
	Display_Time = 1;
	key_state = DISABLE;
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
	UI_state = UI_STATE_SETTING_FONT;
	return state;
}
