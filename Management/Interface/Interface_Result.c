/*
 * Interface_Result.c
 *
 *  Created on: 2018年2月26日
 *      Author: Administrator
 */

/******************************************************************************/
#include "Interface_Result.h"

/******************************************************************************/
block_attr_Result block_Result_Back = {
	ENABLE,									/*Interface Result rect*/
	{
		0,   20,
		128, 140,
		BACKCOLOR_CONTENT_BACK
	},

	DISABLE,								/*Display HZ16X8*/
	{0},

	DISABLE,								/*Display HZ16X8*/
	{0},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_Item = {
	ENABLE,									/*Interface Result rect*/
	{
		7,   24,
		114, 18,
		BACKCOLOR_CONTENT_BAR
	},

	DISABLE,									/*Display HZ16X8*/
	{
		"Item",
		9,   25,
		Black,White,
		BACKCOLOR_CONTENT_BAR
	},

	DISABLE,								/*Display HZ16X8*/
	{0},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_AMP = {
	DISABLE,								/*Display HZ16X8*/
	{0},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[0].TName,
		12,   43,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[0].Result,
		60,   43,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_COC = {
	DISABLE,								/*Display HZ16X8*/
	{0},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[1].TName,
		12,   59,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[1].Result,
		60,   59,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_THC = {
	DISABLE,								/*Display HZ16X8*/
	{0},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[2].TName,
		12,   75,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[2].Result,
		60,   75,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_MET = {
	DISABLE,								/*Display HZ16X8*/
	{0},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[3].TName,
		12,   91,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[3].Result,
		60,   91,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_OPI = {
	DISABLE,								/*Display HZ16X8*/
	{0},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[4].TName,
		12,   107,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[4].Result,
		60,   107,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_PHD = {
	DISABLE,							/*Interface Standard rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[5].TName,
		12,   123,
		Black,White,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[5].Result,
		60,   123,
		Black,BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Result block_Result_Right_arrow = {
	ENABLE,								/*Interface Result rect*/
	{
		7,   43,
		114, 96,
		White
	},

	DISABLE,									/*Display HZ16X8*/
	{
		"Result",
		60,  25,
		Black,White,
		BACKCOLOR_CONTENT_BAR
	},

	DISABLE,							/* Display HZ16X8 */
	{0},

	ENABLE,									/* Display Picture */
	{
		gImage_Right_arrow,
		90, 142,
		35, 15
	},
};

/******************************************************************************/
block_attr_Result* UI_WindowBlocksAttrArray_Result[][9] = {/* Window: Result entry */
{&block_Result_Back,&block_Result_Item,&block_Result_Right_arrow,&block_Result_AMP,},
{&block_Result_Back,&block_Result_Item,&block_Result_Right_arrow,&block_Result_AMP,
		&block_Result_COC},
{&block_Result_Back,&block_Result_Item,&block_Result_Right_arrow,&block_Result_AMP,
		&block_Result_COC,&block_Result_THC},
{&block_Result_Back,&block_Result_Item,&block_Result_Right_arrow,&block_Result_AMP,
		&block_Result_COC,&block_Result_THC,&block_Result_MET},
{&block_Result_Back,&block_Result_Item,&block_Result_Right_arrow,&block_Result_AMP,
		&block_Result_COC,&block_Result_THC,&block_Result_MET,&block_Result_OPI},
{&block_Result_Back,&block_Result_Item,&block_Result_Right_arrow,&block_Result_AMP,
		&block_Result_COC,&block_Result_THC,&block_Result_MET,&block_Result_OPI,
		&block_Result_PHD,}
};

/******************************************************************************/
uint8 Interface_Result(uint16 KeyCode)
{
	uint8 state = 0;
	Key_State_Update = 1;
	Enter_Sleep = 1;
	Exti_lock = DISABLE;
	Page_Flag = 0;
	Interface_Key = 2;
	if(Cup_Count > 6)
	{
		UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Result[5]) >> 2;
	}
	else
	{
		UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Result[Cup_Count-1]) >> 2;
	}
	UI_Draw_Window_Result(UI_WindowBlocks);
	UI_Language_Window_Result();
	Exti_lock = ENABLE;
	loop:
	if(Cup_Count < 7 && key_state_confirm != 1)
	{
		Key_control == 1;
		goto loop;
	}
	UI_state = UI_STATE_KEY_STATE;
	return state;
}

/******************************************************************************/
void UI_Draw_block_Result(block_attr_Result* block);

/******************************************************************************/
void UI_Draw_Window_Result(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		if(Cup_Count > 6)
		{
			if(UI_WindowBlocksAttrArray_Result[5][blockIndex])
			{
				UI_Draw_block_Result(UI_WindowBlocksAttrArray_Result[5][blockIndex]);
			}
		}
		else
		{
			if(UI_WindowBlocksAttrArray_Result[Cup_Count-1][blockIndex])
			{
				UI_Draw_block_Result(UI_WindowBlocksAttrArray_Result[Cup_Count-1][blockIndex]);
			}
		}
	}
}

/******************************************************************************/
void UI_Language_Window_Result(void)
{
	Display_Time = 0;
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16_B(9,25,Black,Dark_Blue,"Item");
		DisplayDriver_Text16_B(60,  25,Black,Dark_Blue,"Result");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16_B(9,25,Black,Dark_Blue,"名称");
		DisplayDriver_Text16_B(60,  25,Black,Dark_Blue,"结果");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16_B(9,25,Black,Dark_Blue,"Analyt");
		DisplayDriver_Text16_B(60,  25,Black,Dark_Blue,"Ergebnis");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
void UI_Draw_block_Result(block_attr_Result* block)
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

	if(Cup_Count > 6 && Cup_Count < 13)					/* 字符数计算判断未完成  */
	{
		if (block->pic_enabled)				/* 2. Draw picture */
		{
			DisplayDriver_DrawPic(block->pic_attr.offsetX,
					block->pic_attr.offsetY, block->pic_attr.width,
					block->pic_attr.height,block->pic_attr.src);
		}
	}
	Display_Time = 1;
	key_state = DISABLE;
}
