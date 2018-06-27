/*
 * Interface_Record.c
 *
 *  Created on: 2018Äê2ÔÂ24ÈÕ
 *      Author: Administrator
 ******************************************************************************/
#include "Interface_Record.h"

/******************************************************************************/
uint8 cBuffer[20] = {0};
uint8 tBuffer[10] = {0};
uint8 page_tatol = 1;
uint8 page_Num = 1;
uint16 UI_WindowBlocks_Record = 0;

/******************************************************************************/
block_attr_Record block_Record_BACKCOLOR_BACK = {
	ENABLE,								/* Interface Record rect */
	{
		0,   20,
		128, 140,
		BACKCOLOR_CONTENT_BACK
	},

	DISABLE,							/* Display HZ16X8 */
	{0},

	DISABLE,							/* Display HZ16X8 */
	{0},
};

/******************************************************************************/
block_attr_Record block_Record_Pname = {
	ENABLE,							/* Interface Record rect */
	{
		6,   24,
		116, 130,
		White
	},

	ENABLE,								/* Display HZ16X8 */
	{
		"Name",
		8,   26,
		Black,BACKCOLOR_CONTENT_BACK
	},


	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.Product_name,
		41,   26,
		Black,BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_Record block_Record_SN = {
	DISABLE,							/* Interface Record rect */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		"SN",
		8,   42,
		Black,BACKCOLOR_CONTENT_BACK
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.Product_SN,
		41,   42,
		Black,BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_Record block_Record_Time = {
	DISABLE,							/* Interface Record rect */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		"Time",
		8,   58,
		Black,BACKCOLOR_CONTENT_BACK
	},

	ENABLE,								/* Display HZ16X8 */
	{
		cBuffer,
		41,   58,
		Black,BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_Record block_Record_Time2 = {
	DISABLE,							/* Interface Record rect */
	{0},

	DISABLE,								/* Display HZ16X8 */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		tBuffer,
		41,   74,
		Black,BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_Record block_Record_CH1 = {
	DISABLE,							/* Interface Record rect */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[0].TName,
		8,   90,
		Black,BACKCOLOR_CONTENT_BACK
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[0].Result,
		50,   90,
		Black,BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_Record block_Record_CH2 = {
	DISABLE,							/* Interface Record rect */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[1].TName,
		8,   106,
		Black,BACKCOLOR_CONTENT_BACK
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[1].Result,
		50,   106,
		Black,BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_Record block_Record_CH3 = {
	DISABLE,							/* Interface Record rect */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[2].TName,
		8,   122,
		Black,BACKCOLOR_CONTENT_BACK
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[2].Result,
		50,   122,
		Black,BACKCOLOR_CONTENT_BACK
	},
};

/******************************************************************************/
block_attr_Record block_Record_CH4 = {
	DISABLE,							/* Interface Record rect */
	{0},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[3].TName,
		8,   138,
		Black,BACKCOLOR_CONTENT_BACK
	},

	ENABLE,								/* Display HZ16X8 */
	{
		Storage_Data.CH_data[3].Result,
		50,   138,
		Black,BACKCOLOR_CONTENT_BACK
	}
};

/******************************************************************************/
block_attr_Record* UI_WindowBlocksAttrArray_Record[][9] = {/* Window: Standard entry */
{&block_Record_BACKCOLOR_BACK,&block_Record_Pname,&block_Record_Time,&block_Record_SN,
		&block_Record_Time2,&block_Record_CH1},
{&block_Record_BACKCOLOR_BACK,&block_Record_Pname,&block_Record_Time,&block_Record_SN,
		&block_Record_Time2,&block_Record_CH1,&block_Record_CH2},
{&block_Record_BACKCOLOR_BACK,&block_Record_Pname,&block_Record_Time,&block_Record_SN,
		&block_Record_Time2,&block_Record_CH1,&block_Record_CH2,&block_Record_CH3},
{&block_Record_BACKCOLOR_BACK,&block_Record_Pname,&block_Record_Time,&block_Record_SN,
		&block_Record_Time2,&block_Record_CH1,&block_Record_CH2,&block_Record_CH3,
		&block_Record_CH4},
};

/******************************************************************************/
void UI_Draw_Block_Record(block_attr_Record* block);

/******************************************************************************/
uint8 Interface_Record(uint16 KeyCode)
{
	uint8 state = 0;
	Exti_lock = DISABLE;
	Interface_Key = 4;
	Read_Record();
	Data_Changer();
	if(reagent_Strip[1] == 0)
	{
		Display_Time = 0;
		Lcd_ColorBox(0,20,128, 140,White);
		DisplayDriver_Text16(24, 80, Red,"No Record!");
		Display_Time = 1;
		Key_control = 0;
		Exti_lock = ENABLE;
		UI_state = UI_STATE_KEY_STATE;
		return state;
	}

	if(Storage_Data.StripNum > 4)
	{
		page_tatol = 2;
		page_Num = 1;
		UI_WindowBlocks_Record = sizeof(UI_WindowBlocksAttrArray_Record[3]) >> 2;
	}
	else
	{
		page_tatol = 1;
		UI_WindowBlocks_Record = sizeof(UI_WindowBlocksAttrArray_Record[Storage_Data.StripNum-1]) >> 2;
	}
	UI_Draw_Window_Record(UI_WindowBlocks_Record);
	DisplayDriver_DrawLine(6,42,120,42,BACKCOLOR_CONTENT_BACK);
	DisplayDriver_DrawLine(6,58,120,58,BACKCOLOR_CONTENT_BACK);
	DisplayDriver_DrawLine(6,90,120,90,BACKCOLOR_CONTENT_BACK);
	DisplayDriver_DrawLine(39,24,39,154,BACKCOLOR_CONTENT_BACK);
	Exti_lock = ENABLE;
	UI_state = UI_STATE_KEY_STATE;
	return state;
}

/******************************************************************************/
void UI_Draw_Window_Record(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		if(Storage_Data.StripNum > 4)
		{
			if(UI_WindowBlocksAttrArray_Record[3][blockIndex])
			{
				UI_Draw_Block_Record(UI_WindowBlocksAttrArray_Record[3][blockIndex]);
			}
		}
		else
		{
			if(UI_WindowBlocksAttrArray_Record[Storage_Data.StripNum-1][blockIndex])
			{
				UI_Draw_Block_Record(UI_WindowBlocksAttrArray_Record[Storage_Data.StripNum-1][blockIndex]);
			}
		}
	}
}

/******************************************************************************/
void UI_Draw_Block_Record(block_attr_Record* block)
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

	if (block->char2_enabled)				/* 2. Draw character */
	{

		DisplayDriver_Text16_B(
				block->char2_attr.offsetX,block->char2_attr.offsetY,
				block->char2_attr.color,block->char2_attr.faceColor,
				block->char2_attr.str);
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
void Data_Changer(void)
{
	memset(cBuffer,0,sizeof(cBuffer));
	memset(tBuffer,0,sizeof(tBuffer));
	sprintf(cBuffer,"%d/%02d/%02d",Storage_Data.Product_Time.year,Storage_Data.Product_Time.month,Storage_Data.Product_Time.day);
	sprintf(tBuffer,"%02d:%02d",Storage_Data.Product_Time.hour,Storage_Data.Product_Time.min);
}
