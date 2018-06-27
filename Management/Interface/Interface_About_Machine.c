/*
 * Interface_About_Machine.c
 *
 *  Created on: 2018Äê6ÔÂ19ÈÕ
 *      Author: Administrator
 *Deep_Blue/

/******************************************************************************/
#include "Interface_About_Machine.h"

uint16 UI_WindowBlocks_About_Machine = 0;

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
		120, 75,
		Light_Gray
	},

	DISABLE,								/*Display HZ16X8*/
	{0},
};

/******************************************************************************/
block_attr_About_Machine block_About_Machine_Deep_Blue_Type = {
	DISABLE,								/*Display HZ16X8*/
	{0},

	ENABLE,									/* Display HZ16X8 */
	{
		"Type:RL-S100",
		5,   56,
		White,White,
		White
	},
};

/******************************************************************************/
block_attr_About_Machine block_About_Machine_HW = {
	DISABLE,								/*Display HZ16X8*/
	{0},

	ENABLE,									/* Display HZ16X8 */
	{
		"  HW:1.0",
		5,   75,
		White,White,
		White
	},
};

/******************************************************************************/
block_attr_About_Machine block_About_Machine_FW = {
	DISABLE,								/*Display HZ16X8*/
	{0},

	ENABLE,									/* Display HZ16X8 */
	{
		"  FW:1.8.0619",
		5,   94,
		White,White,
		White
	},

};

/******************************************************************************/
block_attr_About_Machine* UI_WindowBlocksAttrArray_About_Machine[] = {/* Window: Insert_Cup entry */
		&block_About_Machine_Back,
		&block_About_Machine_Deep_Blue,
		&block_About_Machine_Deep_Blue_Type,
		&block_About_Machine_HW,
		&block_About_Machine_FW,
};

/******************************************************************************/
uint8 Interface_About_Machine(uint16 KeyCode)
{
	uint8 state = 0;
	Exti_lock = DISABLE;
	Interface_Key = 6;
	key_state_confirm = 0;
	UI_WindowBlocks_About_Machine = sizeof(UI_WindowBlocksAttrArray_About_Machine) >> 2;
	UI_Draw_Window_About_Machine(UI_WindowBlocks_About_Machine);
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


