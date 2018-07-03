                                                                                                   /*
 * Interface_Start.c
 *
 *  Created on: 2018Äê2ÔÂ26ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "Interface_Start.h"

/******************************************************************************/
block_attr_Start block_Start_Back = {
	ENABLE,								/*Interface Start rect*/
	{
		0,   20,
		128, 140,
		BACKCOLOR_CONTENT_BACK
	},
	DISABLE,							/*Display HZ16X8*/
	{0},
};

/******************************************************************************/
block_attr_Start block_Start_Name = {
	ENABLE,								/*Interface Start rect*/
	{
		3,   24,
		122, 116,
		White
	},
	ENABLE,								/*Display HZ16X8*/
	{
		"Name:",
		4,   25,
		Black,White,
		White
	},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date.head.name,
		44,   25,
		Black,White,
		White
	},

};

/******************************************************************************/
block_attr_Start block_Start_Number = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"  SN:",
		4,   41,
		Black,White,
		White
	},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date.head.SN,
		44,   41,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_1 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"1:",
		4,   57,
		Black,White,
		White
	},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[0].TName,
		20,   57,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_3 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"3:",
		4,   73,
		Black,White,
		White
	},


	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[2].TName,
		20,   73,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_5 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"5:",
		4,   89,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[4].TName,
		20,   89,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_7 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"7:",
		4,   105,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[6].TName,
		20,   105,
		Black,White,
		White
	},

};

/******************************************************************************/
block_attr_Start block_Start_9 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"9:",
		4,   121,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[8].TName,
		20,   121,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_11 = {
	DISABLE,							/*Interface Standard rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"11:",
		4,   137,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[10].TName,
		20,   137,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_2 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"2:",
		68,   57,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[1].TName,
		86,   57,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_4 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"4:",
		68,   73,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[3].TName,
		86,   73,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_6 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"6:",
		68,   89,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[5].TName,
		86,   89,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_8 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"8:",
		68,   105,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[7].TName,
		86,   105,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_10 = {
	DISABLE,							/*Interface Start rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"10:",
		68,   121,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[9].TName,
		86,   121,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start block_Start_12 = {
	DISABLE,							/*Interface Standard rect*/
	{0},

	ENABLE,								/*Display HZ16X8*/
	{
		"12:",
		68,   137,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[11].TName,
		86,   137,
		Black,White,
		White
	},
};

/******************************************************************************/
block_attr_Start* UI_WindowBlocksAttrArray_Start[][13] = {/* Window: Start entry */
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
		&block_Start_3},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
		&block_Start_3,&block_Start_4},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
		&block_Start_3,&block_Start_4,&block_Start_5},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
		&block_Start_3,&block_Start_4,&block_Start_5,&block_Start_6},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
		&block_Start_3,&block_Start_4,&block_Start_5,&block_Start_6,&block_Start_7},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
		&block_Start_3,&block_Start_4,&block_Start_5,&block_Start_6,&block_Start_7,
		&block_Start_8},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
		&block_Start_3,&block_Start_4,&block_Start_5,&block_Start_6,&block_Start_7,
		&block_Start_8,&block_Start_9},
{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
		&block_Start_3,&block_Start_4,&block_Start_5,&block_Start_6,&block_Start_7,
		&block_Start_8,&block_Start_9,&block_Start_10},
//{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
//		&block_Start_3,&block_Start_4,&block_Start_5,&block_Start_6,&block_Start_7,
//		&block_Start_8,&block_Start_9,&block_Start_10,&block_Start_11},
//{&block_Start_Back,&block_Start_Name,&block_Start_Number,&block_Start_1,&block_Start_2,
//		&block_Start_3,&block_Start_4,&block_Start_5,&block_Start_6,&block_Start_7,
//		&block_Start_8,&block_Start_9,&block_Start_10,&block_Start_11,&block_Start_12},
};

/******************************************************************************/
uint8 Interface_Start(uint16 KeyCode)
{
	Enter_Sleep = 0;
	Exti_lock = DISABLE;
	doubleClick = 0;
	Interface_Key = 1;
	QRCode_Trigger_Disabled();
	if(Cup_Count != 0)
	{
		UI_WindowBlocks_Start = sizeof(UI_WindowBlocksAttrArray_Start[Cup_Count-1]) >> 2;
		UI_Draw_Window_Start(UI_WindowBlocks_Start);
		DisplayDriver_DrawPic(5, 142,35,15,gImage_Left_arrow);
		Exti_lock = ENABLE;
		UI_state = UI_STATE_KEY_STATE;
	}
	else
	{
			UI_state = UI_STATE_QUICK;
	}
	return 0;
}

/******************************************************************************/
void UI_Draw_block_Start(block_attr_Start* block);

/******************************************************************************/
void UI_Draw_Window_Start(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		if(UI_WindowBlocksAttrArray_Start[Cup_Count-1][blockIndex])
		{
			UI_Draw_block_Start(UI_WindowBlocksAttrArray_Start[Cup_Count-1][blockIndex]);
		}
	}
}

/******************************************************************************/
void UI_Draw_block_Start(block_attr_Start* block)
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

	if (block->char_enabled2)				/* 2. Draw character */
	{

		DisplayDriver_Text16_B(
				block->char_attr2.offsetX,block->char_attr2.offsetY,
				block->char_attr2.color,block->char_attr2.faceColor,
				block->char_attr2.str);
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
uint8 Interface_Start_font(uint16 KeyCode)
{
	Exti_lock = DISABLE;
	Interface_Key = 1;
	QRCode_Trigger_Disabled();
	Clear_Data();
	Exti_lock = ENABLE;
	UI_state = UI_STATE_QUICK;
	return 0;
}
