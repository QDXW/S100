/*
 * Interface_About_Machine.c
 *
 *  Created on: 2018Äê6ÔÂ19ÈÕ
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
