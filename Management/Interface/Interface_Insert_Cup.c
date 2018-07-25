/*
 * Interface_Insert_Cup.c
 *
 *  Created on: 2018Äê3ÔÂ19ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#include "Interface_Insert_Cup.h"

/******************************************************************************/
extern uint8 Open_time;
extern const unsigned char gImage_Left_arrow[1050];

/******************************************************************************/
block_attr_Insert_Cup block_Insert_Cup_Back = {
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
block_attr_Insert_Cup block_Insert_Cup_Cup = {
	ENABLE,								/* Interface Insert_Cup rect */
	{
		8,   50,
		112, 80,
		White
	},

	ENABLE,									/* Display HZ16X8 */
	{
		"Insert Cup",
		20,   75,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Insert_Cup* UI_WindowBlocksAttrArray_Insert_Cup[] = {/* Window: Insert_Cup entry */
		&block_Insert_Cup_Back,
		&block_Insert_Cup_Cup,
};

/******************************************************************************/
uint8 Interface_Insert_Cup(uint16 KeyCode)
{
	uint8 state = 0;
	QRCode_Trigger_Disabled();
	Interface_Key = 8;
	Exti_lock = DISABLE;
	UI_WindowBlocks_Insert_Cup = sizeof(UI_WindowBlocksAttrArray_Insert_Cup) >> 2;
	UI_Draw_Window_Insert_Cup(UI_WindowBlocks_Insert_Cup);
	Exti_lock = ENABLE;
	UI_state = UI_STATE_KEY_STATE;
	return state;
}

/******************************************************************************/
void UI_Draw_Block_Insert_Cup(block_attr_Insert_Cup* block);

/******************************************************************************/
void UI_Draw_Window_Insert_Cup(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		UI_Draw_Block_Insert_Cup(UI_WindowBlocksAttrArray_Insert_Cup[blockIndex]);
	}
}

/******************************************************************************/
void UI_Draw_Block_Insert_Cup(block_attr_Insert_Cup* block)
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

			DisplayDriver_Text16_B(
					block->char_attr.offsetX,block->char_attr.offsetY,
					block->char_attr.color,block->char_attr.faceColor,
					block->char_attr.str);
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
uint8 Interface_Down_Time_Process(uint16 blockNum)
{
	uint8 state = 0;
	Interface_Key = 8;
	Exti_lock = ENABLE;
	if(Key_record == 1)
	{
		Open_time = 1;
		Display_Down_Time_Bar();

		while(Action_time)
		{
			if (Key_control == 1)
			{
				UI_state = UI_STATE_KEY_STATE;
				key_state_confirm = 0;
				Open_time = 0;
				Exti_lock = DISABLE;
				return state;
			}
		}
	}

	Exti_lock = DISABLE;
	UI_state = UI_STATE_TESTING;
	Delay_ms_SW(100);
	return state;
}

/******************************************************************************/
void Display_Down_Time_Bar (void)
{
	char tbuf[8] = {0};
	Display_Time = 0;
	Lcd_ColorBox(0,20,128,140,BACKCOLOR_CONTENT_BACK);
	Lcd_ColorBox(36,75,56,30,Light_Gray);
	DisplayDriver_DrawPic(5, 142,35,15,gImage_Left_arrow);
	sprintf((char*)tbuf,"%02d:%02d",(Action_time-1),59);
	DisplayDriver_Text16_B(44,82,Black,Light_Gray,tbuf);
	Display_Time = 1;
}
