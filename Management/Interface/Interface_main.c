/*
 * Interface_main.c
 *
 *  Created on: 2018Äê2ÔÂ9ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "Interface_main.h"
#include "PIC_Interface.h"

/******************************************************************************/
uint8 Cup_Count = 0;
uint8 Read_first = 1;
uint8 Key_record = 1;
extern uint8 page_Num;
extern uint8 Page_Flag;
extern uint8 page_tatol;
uint16 adcx = 0;
float temp = 0.0;
uint8 Bluetooth_Connect = 0;

uint16 UI_WindowBlocks = 0;
uint8 UI_state = UI_STATE_MAIN_WINDOW;

/******************************************************************************/
block_attr block_standard = {
		ENABLE,									/* Interface Main rect */
		{
			0,   20,
			128, 160,
			BACKCOLOR_CONTENT_BACK
		},

		ENABLE,									/* Display Picture */
		{
			gImage_PIC_Standard,
			12, 24,
			45, 45
		},

		ENABLE,									/* Display HZ16X8 */
		{
			"Standard",
			0,  71,
			White,Magenta,
			Baby_Blue
		},

		ENABLE,									/* Parting line */
		{
			63,  20,
			63,  160,
			Light_Gray
		},
};

/******************************************************************************/
block_attr block_quick = {
		DISABLE,
		{0},

		ENABLE,									/* Display Picture */
		{
			gImage_PIC_Quick,
			71,  24,
			45,  45
		},

		ENABLE,									/* Display HZ16X8 */
		{
			"Quick",
			74,  71,
			White,Magenta,
			Baby_Blue
		},

		ENABLE,									/* Parting line */
		{
			64,  20,
			64,  160,
			Thint_Blue
		},
};

/******************************************************************************/
block_attr block_record = {
		DISABLE,
		{0},

		ENABLE,									/* Display Picture */
		{
			gImage_PIC_Record,
			12,  94,
			45, 45
		},

		ENABLE,									/* Display HZ16X8 */
		{
			"Record",
			12,  141,
			White,Magenta,
			Baby_Blue
		},

		ENABLE,									/* Parting line */
		{
			0,    90,
			128,  90,
			Light_Gray
		},
};

/******************************************************************************/
block_attr block_settings = {
		DISABLE,
		{0},

		ENABLE,									/* Display Picture */
		{
			gImage_PIC_Setting,
			71, 94,
			45, 45
		},

		ENABLE,									/* Display HZ16X8 */
		{
			"Settings",
			65,  141,
			White,Magenta,
			Baby_Blue
		},

		ENABLE,									/* Parting line */
		{
			0,    91,
			128,  91,
			Thint_Blue
		},
};

/******************************************************************************/
block_font_attr block_font = {
		ENABLE,									/* Display HZ16X8 */
		{
			"Standard",
			0,  71,
			White,Magenta,
			Baby_Blue
		},

		ENABLE,									/* Display HZ16X8 */
		{
			"Quick",
			74,  71,
			White,Magenta,
			Baby_Blue
		},

		ENABLE,									/* Display HZ16X8 */
		{
			"Record",
			12,  141,
			White,Magenta,
			Baby_Blue
		},

		ENABLE,									/* Display HZ16X8 */
		{
			"Settings",
			65,  141,
			White,Magenta,
			Baby_Blue
		},
};

/******************************************************************************/
block_attr* UI_WindowBlocksAttrArray_Main[] = {		/* Window: Main entry */
		&block_standard,
		&block_quick,
		&block_record,
		&block_settings,
};
block_font_attr* UI_WindowBlocksAttrArray_Main_font[] = {
		&block_font,
};

/******************************************************************************/
void UI_Draw_Block(block_attr* block);
void UI_Draw_Block_font(block_font_attr* block);

/******************************************************************************/
uint8 Interface_Process(uint16* KeyCode)
{
	/* Define each state */
	static uint8 (* const UI_stateMachine[UI_STATE_MAX_STATE_NUM])(uint16) =
	{
			Interface_Main,					/* Interface Main Display */
			Interface_Key_Event,			/* Interface Key Event */
			Interface_Main_font,			/* Interface Main font Display */
			Interface_Start,				/* Interface Start Display */
			Interface_Quick,				/* Interface Quick Display */
			Interface_Record,				/* Interface Record Display */
			Interface_Setting,				/* Interface Setting Display */
			Interface_Testing,				/* Interface Test Display */
			Interface_Result,				/* Interface Result Display */
			Interface_Result_2,				/* Interface Start Display */
			Interface_Insert_Cup,			/* Interface insert cup Display */
			Interface_Start_font,			/* Interface Start font Display */
			Interface_Record_2,				/* Interface Record2 Display */
			Interface_Setting_font,			/* Interface Setting font Display */
			Interface_About_Machine,		/* Interface Setting font Display */
			Interface_System_Time,			/* Interface Setting font Display */
			Interface_Time_Process,			/* Interface Setting font Display */
			Interface_Down_Time_Process,	/* Interface Setting font Display */
	};
	uint8 state;
	do										/* Polling each state */
	{
		if (UI_state < UI_STATE_MAX_STATE_NUM)
		{
			state = UI_stateMachine[UI_state](*KeyCode);
		}
		*KeyCode = 0;	/* Clear touch information to avoid repeated respond */
	} while(state & UI_STATE_RERUN);

	return 0;
}

/******************************************************************************/
uint8 Interface_Main(uint16 KeyCode)
{
	uint8 state = 0;
	Exti_lock = DISABLE;
	Interface_Key = DISABLE;
	key_state = 1;
	key_state_confirm = 0;
	QRCode_Trigger_Disabled();
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Main) >> 2;
	UI_Draw_Window(UI_WindowBlocks);
	UI_state = UI_STATE_MAIN_FONT;
	return state;
}

/******************************************************************************/
void UI_Draw_Window(uint16 blockNum)
{
	uint8 blockIndex = 0;						/* Draw blocks one by one */
	for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
	{
		UI_Draw_Block(UI_WindowBlocksAttrArray_Main[blockIndex]);
	}
}

/******************************************************************************/
void UI_Draw_Block(block_attr* block)
{
	Display_Time = 0;
	if (block->rect_enabled)					/* 1. Draw Rect */
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

	if (block->line_enabled)					/* 3.Parting line */
	{
		DisplayDriver_DrawLine(block->Parting_line_attr.startX,
				block->Parting_line_attr.startY,
				block->Parting_line_attr.endX,
				block->Parting_line_attr.endY,
				block->Parting_line_attr.color);
	}

	if (block->char_enabled)					/* 4. Draw character */
	{
			DisplayDriver_Text16_B(
					block->char_attr.offsetX,block->char_attr.offsetY,
					block->char_attr.color,block->char_attr.faceColor,
					block->char_attr.str);
	}
	Display_Time = 1;
}

/******************************************************************************/
uint8 Interface_Key_Event(uint16 KeyCode)
{
	uint8 state = 0;
	if(key_state)
	{
		switch(Interface_Key)
		{
			case 0:
				switch(key_state_confirm)
				{
					case 0:
						UI_state = UI_STATE_MAIN_FONT;
					break;

					case 1:
						switch(Key_control)
						{
							Key_record = 2;
							case 1:
								UI_state = UI_STATE_START;
							break;
							case 2:
								UI_state = UI_STATE_START;
							break;
							case 3:
								UI_state = UI_STATE_RECORD;
							break;
							case 4:
								UI_state = UI_STATE_SETTING;
							break;

							default:
							break;
						}
						Key_record = Key_control;
						key_state_confirm = DISABLE;
						Key_control = 1;
					break;

					default:
					break;
				}
			break;

			case 1:
				switch(key_state_confirm)
				{
					case 0:
						switch(Key_control)
						{
							case 2:
								UI_state = UI_STATE_MAIN_WINDOW;
								key_state_confirm = DISABLE;
								Key_control = 1;
							break;
						}
					break;

					case 1:
						UI_state = UI_STATE_INSERT_CUP;
						key_state_confirm = DISABLE;
						Key_control = 1;
					break;
					case 2:
						UI_state = UI_STATE_START_FONT;
						key_state_confirm = DISABLE;
						Key_control = 1;
					break;

					default:
					break;
				}
			break;

			case 2:
				switch(key_state_confirm)
				{
					case 0:
						if(Key_control == 1 && Page_Flag)
						{
							UI_state = UI_STATE_RESULT;
						}
						if(Key_control == 2 && (!Page_Flag))
						{
							UI_state = UI_STATE_RESULT_2;
						}
					break;

					case 1:
						UI_state = UI_STATE_MAIN_WINDOW;
						Interface_Key = DISABLE;
						Key_control = 1;
						key_state_confirm = DISABLE;
					break;

					default:
					break;
				}
			break;

			case 3:
				switch(key_state_confirm)
				{
					case 0:
						if(Key_control == 1)
						{
							UI_state = UI_STATE_MAIN_WINDOW;
							Interface_Key = DISABLE;
							Key_control = 1;
							key_state_confirm = DISABLE;
						}
					break;

					default:
					break;
				}
			break;

			case 4:
				switch(key_state_confirm)
				{
					case 0:
						switch(Key_control)
						{
							case 1:
								if (page_Num == 1 && reagent_Strip[0] == 1)
								{
									reagent_Strip[0] = 1;
								}
								else if (page_Num == 1)
								{
									reagent_Strip[0]--;
									if (reagent_Strip[0] < 1)
										reagent_Strip[0]  = reagent_Strip[1];
								}
								UI_state = UI_STATE_RECORD;

							break;

							case 2:
								if(reagent_Strip[0] <= reagent_Strip[1])
								{
									if (page_Num == 1 && reagent_Strip[0] > 99 && page_tatol == 1)
									{
										reagent_Strip[0] = 100;
										UI_state = UI_STATE_RECORD;
									}
									else
									{
										if (page_Num == 1 && page_tatol == 1)
										{
											reagent_Strip[0]++;
											if (reagent_Strip[0] > reagent_Strip[1])
												reagent_Strip[0] = 1;
											UI_state = UI_STATE_RECORD;
										}

										if (page_Num == 1 && page_tatol == 2)
										{
											UI_state = UI_STATE_RECORD2;
										}

										if (page_Num == 2 && page_tatol == 2)
										{
											reagent_Strip[0]++;
											if (reagent_Strip[0] > reagent_Strip[1])
												reagent_Strip[0] = reagent_Strip[1];
											UI_state = UI_STATE_RECORD;
										}
									}
								}
							break;

							default:
							break;
						}

					break;

					case 1:
						UI_state = UI_STATE_MAIN_WINDOW;
						Interface_Key = DISABLE;
						Key_control = 1;
						key_state_confirm = DISABLE;
					break;

					default:
					break;
				}
			break;

			case 5:
				switch(key_state_confirm)
				{
					case 0:
						UI_state = UI_STATE_SETTING_FONT;
						if(!Key_control)
						{
							UI_state = UI_STATE_MAIN_WINDOW;
							Key_control = 1;
						}
						key_state_confirm = DISABLE;
					break;
					case 1:
						switch(Key_control)
						{
							case 1:
								UI_state = UI_STATE_SYSTEM_TIME;
								key_state_confirm = DISABLE;
								Key_control = 1;
							break;
							case 2:
								UI_state = UI_STATE_ABOUT_MACHINE;
								key_state_confirm = DISABLE;
								Key_control = 1;
							break;

							default:
							break;
						}
					break;

					default:
					break;
				}
			break;
			case 6:
				if(key_state_confirm)
				{
					UI_state = UI_STATE_SETTING;
					key_state_confirm = DISABLE;
					Key_control = 2;
				}
			break;

			default:
			break;
			case 8:
				switch(key_state_confirm)
				{
					case 0:
						if(Key_control == 1)
						{
							UI_state = UI_STATE_MAIN_WINDOW;
							key_state_confirm = DISABLE;
							Key_control = 1;
						}
						break;
					case 1:
						UI_state = UI_STATE_DOWN_TIME_PROCESS;
						key_state_confirm = DISABLE;
						Key_control = 2;
					break;
				}
			}
	}
	return state;
}

/******************************************************************************/
uint8 Interface_Main_font(uint16 KeyCode)
{
	uint8 state = 0;
	Exti_lock = DISABLE;
	UI_WindowBlocks = 0;
	Read_first = 1;
	UI_WindowBlocks = sizeof(UI_WindowBlocksAttrArray_Main_font) >> 2;
	UI_Draw_Window_font(UI_WindowBlocks);
	Exti_lock = ENABLE;
	UI_state = UI_STATE_KEY_STATE;
	return state;
}

/******************************************************************************/
void UI_Draw_Window_font(uint16 blockNum)
{
	uint8 blockIndex = 0;
	if(key_state)								/* Draw blocks one by one */
	{
		for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
		{
			UI_Draw_Block_font(UI_WindowBlocksAttrArray_Main_font[blockIndex]);
		}
	}
}

/******************************************************************************/
void UI_Draw_Block_font(block_font_attr* block)
{
	Display_Time = 0;
	if (block->char1_enabled)				/* 2. Draw character */
	{
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(
					block->char1_attr.offsetX,block->char1_attr.offsetY,
					block->char1_attr.color,block->char1_attr.backColor,
					block->char1_attr.str);
		}
		else
		{
			DisplayDriver_Text16_B(
					block->char1_attr.offsetX,block->char1_attr.offsetY,
					block->char1_attr.color,block->char1_attr.faceColor,
					block->char1_attr.str);
		}
	}

	if (block->char2_enabled)				/* 2. Draw character */
	{
		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(
					block->char2_attr.offsetX,block->char2_attr.offsetY,
					block->char2_attr.color,block->char2_attr.backColor,
					block->char2_attr.str);
		}
		else
		{
			DisplayDriver_Text16_B(
					block->char2_attr.offsetX,block->char2_attr.offsetY,
					block->char2_attr.color,block->char2_attr.faceColor,
					block->char2_attr.str);
		}
	}

	if (block->char3_enabled)				/* 2. Draw character */
	{
		if(Key_control == 3)
		{
			DisplayDriver_Text16_B(
					block->char3_attr.offsetX,block->char3_attr.offsetY,
					block->char3_attr.color,block->char3_attr.backColor,
					block->char3_attr.str);
		}
		else
		{
			DisplayDriver_Text16_B(
					block->char3_attr.offsetX,block->char3_attr.offsetY,
					block->char3_attr.color,block->char3_attr.faceColor,
					block->char3_attr.str);
		}
	}

	if (block->char4_enabled)				/* 2. Draw character */
	{
		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(
					block->char4_attr.offsetX,block->char4_attr.offsetY,
					block->char4_attr.color,block->char4_attr.backColor,
					block->char4_attr.str);
		}
		else
		{
			DisplayDriver_Text16_B(
					block->char4_attr.offsetX,block->char4_attr.offsetY,
					block->char4_attr.color,block->char4_attr.faceColor,
					block->char4_attr.str);
		}
	}
	Display_Time = 1;
	key_state = DISABLE;
}

/******************************************************************************/
void UI_Draw_Status_Bar(void)					/* UI Draw Status Bar and Battery */
{
	char tbuf[10] = {0};
	PCF8563_Read(&SystemManage_CurrentTime);
	if(Display_Time)
	{
		sprintf((char*)tbuf,"%02d:%02d",SystemManage_CurrentTime.hour,
				SystemManage_CurrentTime.min);
		Display_Time = 0;
		DisplayDriver_Text16_B(12,2,White,BACKCOLOR_CONTENT_BAR,tbuf);
		Display_Time = 1;
	}
}

/******************************************************************************/
void Battery_Empty_ICO(void)
{
	int i = 0;

	Display_Time = 0;
	Lcd_ColorBox(103,7,15,7,BACKCOLOR_CONTENT_BAR);

	DisplayDriver_DrawLine(101,5,119,5,White);
	DisplayDriver_DrawLine(101,6,119,6,White);

	DisplayDriver_DrawLine(101,5,101,16,White);
	DisplayDriver_DrawLine(102,5,102,16,White);

	DisplayDriver_DrawLine(101,16,119,16,White);
	DisplayDriver_DrawLine(101,15,119,15,White);

	DisplayDriver_DrawLine(119,16,119,12,White);
	DisplayDriver_DrawLine(120,15,120,12,White);
	DisplayDriver_DrawLine(119,6,119,9,White);
	DisplayDriver_DrawLine(120,6,120,9,White);

	DisplayDriver_DrawLine(120,8,120,8,White);
	DisplayDriver_DrawLine(120,9,120,9,White);
	DisplayDriver_DrawLine(121,9,121,12,White);
	DisplayDriver_DrawLine(122,9,122,12,White);

	Battery_Display();

	Display_Time = 1;
}

/******************************************************************************/
void Battery_Display (void)
{
	int i = 0;


	adcx = Get_Adc_Average(ADC_Channel_11,10);
	temp = (float)adcx*(5.185/4096.0);

	Lcd_ColorBox(103,7,15,8,BACKCOLOR_CONTENT_BAR);
	if((!Power_Open) || (UI_state == UI_STATE_TESTING))
	{
		temp += 0.36;
	}

	if((GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)) && (temp < 3.6))
	{
		Lcd_ColorBox(103,7,1,8,Red);
	}
	else
	{
		if(temp > 4.1)
		{
			for(i= 104;i<118;)
			{
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
				{
					Lcd_ColorBox(i,8,2,6,Green);
					i += 3;
				}
				else
				{
					Lcd_ColorBox(i,8,2,6,White);
					i += 3;
				}
			}
		}
		else if(temp > 4.0)
		{
			for(i= 104;i<116;)
			{
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
				{
					Lcd_ColorBox(i,8,2,6,Green);
					i += 3;
				}
				else
				{
					Lcd_ColorBox(i,8,2,6,White);
					i += 3;
				}
			}
		}
		else if(temp > 3.9)
		{
			for(i= 104;i<112;)
			{
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
				{
					Lcd_ColorBox(i,8,2,6,Green);
					i += 3;
				}
				else
				{
					Lcd_ColorBox(i,8,2,6,White);
					i += 3;
				}
			}
		}
		else if(temp > 3.8)
		{
			for(i= 104;i<108;)
			{
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
				{
					Lcd_ColorBox(i,8,2,6,Green);
					i += 3;
				}
				else
				{
					Lcd_ColorBox(i,8,2,6,White);
					i += 3;
				}
			}
		}
		else if(temp > 3.7)
		{
			for(i= 104;i<106;)
			{
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
				{
					Lcd_ColorBox(i,8,2,6,Green);
					i += 3;
				}
				else
				{
					Lcd_ColorBox(i,8,2,6,White);
					i += 3;
				}
			}
		}
		else if(temp > 3.65)
		{
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
			{
				Lcd_ColorBox(i,8,2,6,Green);
				i += 3;
			}
			else
			{
				Lcd_ColorBox(103,7,1,8,Red);
			}
		}
		else
		{
//				SystemManage_CheckPowerOff();
		}
	}
}

/******************************************************************************/
void Bluetooth_Connection (void)
{
	if (Printer_isConnected())
	{
		if (!Bluetooth_Connect)
		{
			DisplayDriver_DrawPic(85,4,9,14,gImage_Blutooth);
			Bluetooth_Connect = 1;
		}
	}
	else
	{
		if (Bluetooth_Connect)
		{
			Lcd_ColorBox(85,4,9,14,BACKCOLOR_CONTENT_BAR);
			Bluetooth_Connect = 0;
		}
	}
}
