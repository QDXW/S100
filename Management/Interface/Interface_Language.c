/*
 * Interface_Language.c
 *
 *  Created on: 2018年11月29日
 *      Author: Administrator
 */
/******************************************************************************/
#include "Interface_Language.h"
uint8 Language_Record = 0;

/******************************************************************************/
block_Language_attr block_Language_English = {
		DISABLE,

		ENABLE,									/* Display Picture */
		{
			"English",
			3,  28,
			White,Magenta,
			Light_Gray
		},
};

/******************************************************************************/
block_Language_attr block_Language_Chinese = {
		DISABLE,

		ENABLE,									/* Display Picture */
		{
			"简体中文",
			65,  28,
			White,Magenta,
			Light_Gray
		},
};

/******************************************************************************/
block_Language_attr* UI_WindowBlocksAttrArray_Language[] = {/* Window: Standard entry */
		&block_Language_English,
		&block_Language_Chinese,
};

/******************************************************************************/
void UI_Draw_Block_Language(block_Language_attr* block);

/******************************************************************************/
uint8 Interface_Language_Process(uint16 blockNum)
{
	uint8 state = 0;
	Exti_lock = DISABLE;
	Interface_Key = 9;
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Language) >> 2;
	UI_Draw_Window_Language(UI_WindowBlocks);
	Exti_lock = ENABLE;
	UI_state = UI_STATE_LANGUAGE_KEY;
	return state;
}

/******************************************************************************/
void UI_Draw_Window_Language(uint16 blockNum)
{
	uint8 blockIndex = 0;					/* Draw blocks one by one */

	switch(Key_control)
	{
	case 1:
		block_Language_English.char_status = 1;
		block_Language_Chinese.char_status = 0;
		Language_Record = 1;
		break;
	case 2:
		block_Language_English.char_status = 0;
		block_Language_Chinese.char_status = 1;
		Language_Record = 2;
		break;

	default:
		break;
	}

	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		UI_Draw_Block_Language(UI_WindowBlocksAttrArray_Language[blockIndex]);
	}
}

/******************************************************************************/
void UI_Draw_Block_Language(block_Language_attr* block)
{
	Display_Time = 0;
	if (block->char_enabled)				/* 2. Draw character */
	{
		if(block->char_status)
		{
			DisplayDriver_Text16_B(
				block->char_attr.offsetX,block->char_attr.offsetY,
				block->char_attr.color,block->char_attr.backColor,
				block->char_attr.str);
		}
		else
		{
			DisplayDriver_Text16_B(
				block->char_attr.offsetX,block->char_attr.offsetY,
				block->char_attr.color,block->char_attr.faceColor,
				block->char_attr.str);
		}
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
uint8 Interface_Language_key_Process(uint16 blockNum)
{
	uint8 state = 0;
	Interface_Key = 9;
	if(key_state)
	{
		switch(key_state_confirm)
		{
		case 0:
			switch(Key_control)
			{
				case 1:
					UI_state = UI_STATE_LANGUAGE_PROCESS;
					key_state_confirm = DISABLE;
				break;
				case 2:
					UI_state = UI_STATE_LANGUAGE_PROCESS;
					key_state_confirm = DISABLE;
				break;

				default:
				break;
			}
		break;

		case 1:
			UI_state = UI_STATE_SETTING;
			key_state_confirm = DISABLE;
			Key_control = 1;
			Language_Set();
			break;

		default:
		break;
		}
	}
	return state;
}

/******************************************************************************/
void Language_Set(void)
{
	switch(Language_Record)
	{
	case 1:
		Font_Switch = DISPLAY_FONT_ENGLISH;
		Storage_Write(&Font_Switch, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*3),1);
		break;

	case 2:
		Font_Switch = DISPLAY_FONT_CHINESE;
		Storage_Write(&Font_Switch, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*3),1);
		break;

	default:
		break;
	}
}
