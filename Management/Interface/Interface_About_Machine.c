/*
 * Interface_About_Machine.c
 *
 *  Created on: 2018年6月19日
 *      Author: Administrator
 *Deep_Blue/

/******************************************************************************/
#include "Interface_About_Machine.h"

/******************************************************************************/
block_attr_About_Machine block_About_Machine_Back = {
	ENABLE,									/*Interface Insert_Cup rect*/
	{
		0,   20,
		128, 160,
		BACKCOLOR_CONTENT_BACK
	},

	DISABLE,								/*Display HZ16X8*/
	{0},
};

/******************************************************************************/
block_attr_About_Machine block_About_Machine_Deep_Blue = {
	ENABLE,									/*Interface Insert_Cup rect*/
	{
		4,   50,
		120, 95,
		Light_Gray
	},

	DISABLE,								/*Display HZ16X8*/
	{0},
};

/******************************************************************************/
block_attr_About_Machine* UI_WindowBlocksAttrArray_About_Machine[] = {/* Window: Insert_Cup entry */
		&block_About_Machine_Back,
		&block_About_Machine_Deep_Blue,
};

/******************************************************************************/
uint8 Interface_About_Machine(uint16 KeyCode)
{
	uint8 state = 0,Exti_lock = DISABLE;
	Interface_Key = 6,key_state_confirm = 0;
	Read_SN();
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_About_Machine) >> 2;
	UI_Draw_Window_About_Machine(UI_WindowBlocks);
	UI_Language_Window_About_Machine();
	Exti_lock = ENABLE;
	UI_state = UI_STATE_KEY_STATE;
	return state;
}

/******************************************************************************/
void UI_Draw_Block_About_Machine(block_attr_About_Machine* block);

/******************************************************************************/
void UI_Draw_Window_About_Machine(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		UI_Draw_Block_About_Machine(UI_WindowBlocksAttrArray_About_Machine[blockIndex]);
	}
}

/******************************************************************************/
void UI_Draw_Block_About_Machine(block_attr_About_Machine* block)
{
	Display_Time = 0;
	if (block->rect_enabled)				/* 1. Draw Rect */
	{
		Lcd_ColorBox(block->rect_attr.startX, block->rect_attr.startY,
				block->rect_attr.width, block->rect_attr.height,
				block->rect_attr.color);
	}

	if (block->char_enabled)				/* 2. Draw character */
	{

			DisplayDriver_Text16(
					block->char_attr.offsetX,block->char_attr.offsetY,
					block->char_attr.color,block->char_attr.str);
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
void UI_Language_Window_About_Machine(void)
{
	Display_Time = 0;
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(4,56,White,"Type:RL-S100");
		DisplayDriver_Text16(4,76,White,"SN:");
		DisplayDriver_Text16(28,76,White,data_SN);
		DisplayDriver_Text16(4,96,White,"HW:1.0");
		DisplayDriver_Text16(4,116,White,"FW:1.8.0619");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(4,56,White,"型号:RL-S100");
		DisplayDriver_Text16(4,76,White,"批号");
		DisplayDriver_Text16(39,76,White,data_SN);
		DisplayDriver_Text16(4,96,White,"固件版本:1.0");
		DisplayDriver_Text16(4,116,White,"硬件版本:8.1115");
		break;

	default:
		break;
	}
	Display_Time = 1;
}
