                                                                                                   /*
 * Interface_Start.c
 *
 *  Created on: 2018年2月26日
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
	DISABLE,								/*Display HZ16X8*/
	{
		"Name:",
		4,   25,
		Black,White,
		White
	},

	DISABLE,									/*Display HZ16X8*/
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

	DISABLE,								/*Display HZ16X8*/
	{
		"  SN:",
		4,   45,
		Black,White,
		White
	},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date.head.SN,
		44,   45,
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
		4,   63,
		Black,White,
		White
	},

	ENABLE,									/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[0].TName,
		20,   63,
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
		4,   79,
		Black,White,
		White
	},


	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[2].TName,
		20,   79,
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
		4,   95,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[4].TName,
		20,   95,
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
		4,   111,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[6].TName,
		20,   111,
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
		68,   63,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[1].TName,
		86,   63,
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
		68,   79,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[3].TName,
		86,   79,
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
		68,   95,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[5].TName,
		86,   95,
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
		68,   111,
		Black,White,
		White
	},

	ENABLE,								/*Display HZ16X8*/
	{
		QR_Date_Analyze.ch_data[7].TName,
		86,   111,
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
		UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Start[Cup_Count-1]) >> 2;
		UI_Draw_Window_Start(UI_WindowBlocks);
		UI_Language_Window_Start();
		Display_Time = 0;
		DisplayDriver_DrawPic(5, 142,35,15,gImage_Left_arrow);
		Display_Time = 1;
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
void UI_Language_Window_Start(void)
{
	Display_Time = 0;
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16_B(4,25,Black,White,"Name:");
		DisplayDriver_Text16_B(44,25,Black,White,QR_Date.head.name);
		DisplayDriver_Text16_B(4,45,Black,White,"  SN:");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16_B(4,25,Black,White,"名称:");
		DisplayDriver_Text16_B(44,25,Black,White,"毒品检测");
		DisplayDriver_Text16_B(4,45,Black,White,"批号:");
		break;

	default:
		break;
	}
	Display_Time = 1;
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
	Quick_Second = 0;
	UI_state = UI_STATE_QUICK;
	return 0;
}
