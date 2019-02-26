/*
 * Interface_Quick.c
 *
 *  Created on: 2018年2月24日
 *      Author: Administrator
 ******************************************************************************/
#include "Interface_Quick.h"

/******************************************************************************/
block_attr_Quick block_Quick_1 = {
	ENABLE,								/* Interface Quick rect */
	{
		0,   20,
		128, 140,
		BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display Picture */
	{0},
};

/******************************************************************************/
block_attr_Quick block_Quick_2 = {
	ENABLE,									/* Interface Quick rect */
	{
		7,   52,
		114, 78,
		White
	},

	DISABLE,								/* Display Picture */
	{0},
};

/******************************************************************************/
block_attr_Quick block_Quick_4 = {
	DISABLE,							/*Interface Quick rect */
	{0},

	ENABLE,								/* Display Picture */
	{
		gImage_Left_arrow,
		5, 142,
		35, 15
	}
};

/******************************************************************************/
block_attr_Quick* UI_WindowBlocksAttrArray_Quick[] = {/* Window: Standard entry */
		&block_Quick_1,
		&block_Quick_2,
		&block_Quick_4,
};

/******************************************************************************/
void UI_Draw_Block_Quick(block_attr_Quick* block);

/******************************************************************************/
uint8 Interface_Quick(uint16 KeyCode)
{
	uint8 state = 0;
	QRCode_Trigger_Disabled();
	Exti_lock = DISABLE;
	Key_control = 2;
	Quick_Down_time = 1;
	Interface_Key = 3;
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Quick) >> 2;
	UI_Draw_Window_Quick(UI_WindowBlocks);
	UI_Language_Window_Quick();
	QRCode_Trigger_Enabled();
	Exti_lock = ENABLE;
	while (!QRCode_received)
	{
		if (Key_control == 1)
		{
			UI_state = UI_STATE_KEY_STATE;
			key_state_confirm = 0;
			Quick_Down_time = 0;
			Exti_lock = DISABLE;
			return state;
		}

		if (Quick_Second > 20)
		{
			UI_state = UI_STATE_MAIN_WINDOW;
			Quick_Down_time = 0;
			Quick_Second = 0;
			return state;
		}
	}
	Exti_lock = DISABLE;

	if (QRCode_existed)
	{
		UI_state = UI_STATE_INSERT_CUP;
		Quick_Down_time = 0;
	}
	else
	{
		UI_state = UI_STATE_MAIN_WINDOW;
		Quick_Down_time = 0;
		Delay_ms_SW(1200);
	}

	QRCode_existed = 0;
	QRCode_received = 0;
	key_state_confirm = 0;
	Delay_ms_SW(20);
	return state;
}

/******************************************************************************/
void UI_Draw_Window_Quick(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		UI_Draw_Block_Quick(UI_WindowBlocksAttrArray_Quick[blockIndex]);
	}
}

/******************************************************************************/
void UI_Draw_Block_Quick(block_attr_Quick* block)
{
	Display_Time = 0;
	if (block->rect_enabled)				/* 1. Draw Rect*/
	{
		Lcd_ColorBox(block->rect_attr.startX, block->rect_attr.startY,
				block->rect_attr.width, block->rect_attr.height,
				block->rect_attr.color);
	}

	if (block->pic_enabled)						/* 2. Draw picture */
	{
		DisplayDriver_DrawPic(block->pic_attr.offsetX,
				block->pic_attr.offsetY, block->pic_attr.width,
				block->pic_attr.height,block->pic_attr.src);
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
void UI_Language_Window_Quick(void)
{
	Display_Time = 0;
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16_B(15,72,Black,White,"Scan the QR");
		DisplayDriver_Text16_B(52,90,Black,White,"code");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16_B(24,83,Baby_Blue,White,"扫描二维码");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16_B(20,73,Baby_Blue,White,"Scannen Sie");
		DisplayDriver_Text16_B(20,91,Baby_Blue,White,"den QR Code");
		break;

	default:
		break;
	}
	Display_Time = 1;
}
