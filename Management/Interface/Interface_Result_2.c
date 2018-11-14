/*
 * Interface_Result_2.c
 *
 *  Created on: 2018Äê3ÔÂ16ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "Interface_Result_2.h"

/******************************************************************************/
block_attr_Result block_Result_WCG = {
	DISABLE,								/*Interface Result rect*/
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		QR_Date_Analyze.ch_data[6].TName,
		12,   43,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[6].Result,
		60,   43,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_EDG = {
	DISABLE,							/* Display HZ16X8 */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		QR_Date_Analyze.ch_data[7].TName,
		12,   59,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[7].Result,
		60,   59,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
		{0},
};

/******************************************************************************/
block_attr_Result block_Result_UZI = {
	DISABLE,							/* Display HZ16X8 */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		QR_Date_Analyze.ch_data[8].TName,
		12,   75,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[8].Result,
		60,   75,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_XZG = {
	DISABLE,							/* Display HZ16X8 */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		QR_Date_Analyze.ch_data[9].TName,
		12,   91,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[9].Result,
		60,   91,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_PCY = {
	DISABLE,							/* Display HZ16X8 */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		QR_Date_Analyze.ch_data[10].TName,
		12,   107,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[10].Result,
		60,   107,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_AKD = {
	DISABLE,							/*Interface Standard rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[11].TName,
		12,   123,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[11].Result,
		60,   123,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_BACK = {
	ENABLE,								/* Interface Result rect */
	{
		89, 140,
		37, 19,
		BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},

	DISABLE,							/* Display Picture */
	{0},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_Left_arrow = {
	ENABLE,								/* Interface Result rect */
	{
		7,   43,
		114, 96,
		White
	},

	DISABLE,							/* Display Picture */
	{0},

	DISABLE,							/* Display HZ16X8 */
	{0},

	ENABLE,								/* Display Picture */
	{
		gImage_Left_arrow,
		5, 142,
		35, 15
	},
};

/******************************************************************************/
block_attr_Result* UI_WindowBlocksAttrArray_Result_2[][8] = {/* Window: Result entry */
{&block_Result_BACK,&block_Result_Left_arrow,&block_Result_WCG},
{&block_Result_BACK,&block_Result_Left_arrow,&block_Result_WCG,&block_Result_EDG},
{&block_Result_BACK,&block_Result_Left_arrow,&block_Result_WCG,&block_Result_EDG,
		&block_Result_UZI},
{&block_Result_BACK,&block_Result_Left_arrow,&block_Result_WCG,&block_Result_EDG,
		&block_Result_UZI,&block_Result_XZG},
};

/******************************************************************************/
void UI_Draw_block_Result_2(block_attr_Result* block);

/******************************************************************************/
uint8 Interface_Result_2(uint16 KeyCode)
{
	uint8 state = 0;
	Page_Flag = 1;
	Exti_lock = DISABLE;
	Interface_Key = 2;
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Result_2[Cup_Count-7]) >> 2;
	UI_Draw_Window_Result_2(UI_WindowBlocks);
	Exti_lock = ENABLE;
//	while(!key_state);
	UI_state = UI_STATE_KEY_STATE;
	return state;
}

/******************************************************************************/
void UI_Draw_Window_Result_2(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		if(UI_WindowBlocksAttrArray_Result_2[Cup_Count-7][blockIndex])
		{
			UI_Draw_block_Result_2(UI_WindowBlocksAttrArray_Result_2[Cup_Count-7][blockIndex]);
		}
	}
}

/******************************************************************************/
void UI_Draw_block_Result_2(block_attr_Result* block)
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

	if (block->char2_enabled)				/* 2. Draw character */
	{
		DisplayDriver_Text16_B(
				block->char2_attr.offsetX,block->char2_attr.offsetY,
				block->char2_attr.color,block->char2_attr.faceColor,
				block->char2_attr.str);
	}

	if (block->pic_enabled)					/* 2. Draw picture */
	{
		DisplayDriver_DrawPic(block->pic_attr.offsetX,
				block->pic_attr.offsetY, block->pic_attr.width,
				block->pic_attr.height,block->pic_attr.src);
	}

	Display_Time = 1;
	key_state = DISABLE;
}
