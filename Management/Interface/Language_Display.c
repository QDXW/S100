/*
 * Language_Display.c
 *
 *  Created on: 2019年2月27日
 *      Author: Administrator
 */

/******************************************************************************/
#include "Language_Display.h"

/******************************************************************************/
uint8 buffer[40] = {0};
uint8 tbuf[40] = {0};

/******************************************************************************/
void UI_Draw_Block_font(void)
{
	Display_Time = 0;
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(0,71,White,Magenta,"Standard");
		}
		else
		{
			DisplayDriver_Text16_B(0,71,White,Baby_Blue,"Standard");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(74,71,White,Magenta,"Quick");
		}
		else
		{
			DisplayDriver_Text16_B(74,71,White,Baby_Blue,"Quick");
		}

		if(Key_control == 3)
		{
			DisplayDriver_Text16_B(12,141,White,Magenta,"Record");
		}
		else
		{
			DisplayDriver_Text16_B(12,141,White,Baby_Blue,"Record");
		}

		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(65,141,White,Magenta,"Settings");
		}
		else
		{
			DisplayDriver_Text16_B(65,141,White,Baby_Blue,"Settings");
		}
		break;

	case DISPLAY_FONT_CHINESE:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(0,71,White,Magenta,"标准检测");
		}
		else
		{
			DisplayDriver_Text16_B(0,71,White,Baby_Blue,"标准检测");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(64,71,White,Magenta,"快速检测");
		}
		else
		{
			DisplayDriver_Text16_B(64,71,White,Baby_Blue,"快速检测");
		}

		if(Key_control == 3)
		{
			DisplayDriver_Text16_B(16,141,White,Magenta,"记录");
		}
		else
		{
			DisplayDriver_Text16_B(16,141,White,Baby_Blue,"记录");
		}

		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(80,141,White,Magenta,"设置");
		}
		else
		{
			DisplayDriver_Text16_B(80,141,White,Baby_Blue,"设置");
		}
		break;

	case DISPLAY_FONT_GERMAN:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(0,71,White,Magenta,"Standard");
		}
		else
		{
			DisplayDriver_Text16_B(0,71,White,Baby_Blue,"Standard");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(68,71,White,Magenta,"Schnell");
		}
		else
		{
			DisplayDriver_Text16_B(68,71,White,Baby_Blue,"Schnell");
		}

		if(Key_control == 3)
		{
			DisplayDriver_Text16_B(0,141,White,Magenta,"Speicher");
		}
		else
		{
			DisplayDriver_Text16_B(0,141,White,Baby_Blue,"Speicher");
		}

		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(84,141,White,Magenta,"Set");
		}
		else
		{
			DisplayDriver_Text16_B(84,141,White,Baby_Blue,"Set");
		}
		break;

	case DISPLAY_FONT_PROTUGAL:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(12,71,White,Magenta,"Pad]o");
		}
		else
		{
			DisplayDriver_Text16_B(12,71,White,Baby_Blue,"Pad]o");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(72,71,White,Magenta,"R^pido");
		}
		else
		{
			DisplayDriver_Text16_B(72,71,White,Baby_Blue,"R^pido");
		}

		if(Key_control == 3)
		{
			DisplayDriver_Text16_B(0,141,White,Magenta,"Registro");
		}
		else
		{
			DisplayDriver_Text16_B(0,141,White,Baby_Blue,"Registro");
		}

		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(68,141,White,Magenta,"Ajustes");
		}
		else
		{
			DisplayDriver_Text16_B(68,141,White,Baby_Blue,"Ajustes");
		}
		break;

	default:
		break;
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

	case DISPLAY_FONT_PROTUGAL:
			DisplayDriver_Text16_B(20,73,Baby_Blue,White,"Scaneia o");
			DisplayDriver_Text16_B(20,91,Baby_Blue,White,"c_digo QR");
			break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
void UI_Language_Window_Insert_Cup(void)
{
	Display_Time = 0;
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16_B(20,75,Black,White,"Insert Cup");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16_B(32,82,Baby_Blue,White,"插入杯子");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16_B(16,82,Baby_Blue,White,"Einf$gen cup");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16_B(16,82,Baby_Blue,White,"Inserir Ta$a");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
void UI_Language_Window_Start(void)
{
	Display_Time = 0;

	DisplayDriver_Text16_B(44,25,Black,White,QR_Date.head.name);
	DisplayDriver_Text16_B(4,45,Black,White,"  SN:");
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16_B(4,25,Black,White,"Name:");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16_B(4,25,Black,White,"名称:");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16_B(4,25,Black,White,"Name:");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16_B(4,25,Black,White,"Nome:");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
void UI_Language_Testing_Battery (void)
{
	if(temp > 3.6)
	{

	}
	else
	{
		Display_Time = 0;
		Lcd_ColorBox(0,20,128, 140,White);

		switch(Font_Switch)
		{
		case DISPLAY_FONT_ENGLISH:
			DisplayDriver_Text16(24, 80, Red,"No Battery!");
			break;

		case DISPLAY_FONT_CHINESE:
			DisplayDriver_Text16(36, 80, Red,"无电池!");
			break;

		case DISPLAY_FONT_GERMAN:
			DisplayDriver_Text16(4, 80, Red,"Keine batterie!");
			break;

		case DISPLAY_FONT_PROTUGAL:
			DisplayDriver_Text16(16, 80, Red,"Sem bateria!");
			break;

		default:
			break;
		}

		Display_Time = 1;
		UI_state = UI_STATE_MAIN_WINDOW;
		Delay_ms_SW(1500);
	}
}

/******************************************************************************/
void UI_Language_Testing_Low_Battery (void)
{
	Display_Time = 0;
	Lcd_ColorBox(0,20,128, 140,White);
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(11, 80, Red,"Low battery!");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(28,80, Red,"电量过低!");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16(0,80, Red,"Batterie schwach");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16(12,80, Red,"Bateria Fraca");
		break;

	default:
		break;
	}
	Display_Time = 1;
	UI_state = UI_STATE_MAIN_WINDOW;
	Delay_ms_SW(1500);
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

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16_B(9,25,Black,Dark_Blue,"`tme");
		DisplayDriver_Text16_B(48,  25,Black,Dark_Blue,"Resultado");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
uint8 Interface_Setting_font(uint16 KeyCode)
{
	uint8 state = 0,status = 0;
	Exti_lock = DISABLE;
	Interface_Key = 5;
	Display_Time = 0;
	Lcd_ColorBox(14,71,40,18,BACKCOLOR_CONTENT_BACK);
	Lcd_ColorBox(0,141,63,18,BACKCOLOR_CONTENT_BACK);

	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(12,71,White,Magenta,"About");
		}
		else
		{
			DisplayDriver_Text16_B(12,71,White,Baby_Blue,"About");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(80,71,White,Magenta,"Time");
		}
		else
		{
			DisplayDriver_Text16_B(80,71,White,Baby_Blue,"Time");
		}

		if(Bluetooth_switch)
		{
			DisplayDriver_Text16_B(24,141,White,White,"ON");
		}
		else
		{
			DisplayDriver_Text16_B(20,141,White,Baby_Blue,"OFF");
		}

		if(Key_control == 3)
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(24,141,White,Magenta,"ON");
				status = 0;
			}
			else
			{
				DisplayDriver_Text16_B(20,141,White,Magenta,"OFF");
				status = 1;
			}
			Storage_Write(&status, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*4),1);
		}
		else
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(24,141,White,White,"ON");
			}
			else
			{
				DisplayDriver_Text16_B(20,141,White,Baby_Blue,"OFF");
			}
		}

		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(64,141,White,Magenta,"Language");
		}
		else
		{
			DisplayDriver_Text16_B(64,141,White,Baby_Blue,"Language");
		}
		break;

	case DISPLAY_FONT_CHINESE:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(0,71,White,Magenta,"关于本机");
		}
		else
		{
			DisplayDriver_Text16_B(0,71,White,Baby_Blue,"关于本机");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(64,71,White,Magenta,"系统时间");
		}
		else
		{
			DisplayDriver_Text16_B(64,71,White,Baby_Blue,"系统时间");
		}

		if(Key_control == 3)
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(16,141,White,Magenta,"打开");
				status = 0;
			}
			else
			{
				DisplayDriver_Text16_B(16,141,White,Magenta,"关闭");
				status = 1;
			}
			Storage_Write(&status, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*4),1);
		}
		else
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(16,141,White,White,"打开");
			}
			else
			{
				DisplayDriver_Text16_B(16,141,White,Baby_Blue,"关闭");
			}
		}

		if(Key_control == 4)
		{
			DisplayDriver_Text16_B(80,141,White,Magenta,"语言");
		}
		else
		{
			DisplayDriver_Text16_B(80,141,White,Baby_Blue,"语言");
		}
		break;

	case DISPLAY_FONT_GERMAN:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(16,71,White,Magenta,"$ber");
		}
		else
		{
			DisplayDriver_Text16_B(16,71,White,Baby_Blue,"$ber");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(80,71,White,Magenta,"Zeit");
		}
		else
		{
			DisplayDriver_Text16_B(80,71,White,Baby_Blue,"Zeit");
		}

		if(Key_control == 3)
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(24,141,White,Magenta,"An");
				status = 0;
			}
			else
			{
				DisplayDriver_Text16_B(20,141,White,Magenta,"Aus");
				status = 1;
			}

			Storage_Write(&status, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*4),1);
		}
		else
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(24,141,White,White,"An");
			}
			else
			{
				DisplayDriver_Text16_B(20,141,White,Baby_Blue,"Aus");
			}
		}
		break;

	case DISPLAY_FONT_PROTUGAL:
		if(Key_control == 1)
		{
			DisplayDriver_Text16_B(16,71,White,Magenta,"Sobre");
		}
		else
		{
			DisplayDriver_Text16_B(16,71,White,Baby_Blue,"Sobre");
		}

		if(Key_control == 2)
		{
			DisplayDriver_Text16_B(80,71,White,Magenta,"Hora");
		}
		else
		{
			DisplayDriver_Text16_B(80,71,White,Baby_Blue,"Hora");
		}

		if(Key_control == 3)
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(16,141,White,Magenta,"Open");
				status = 0;
			}
			else
			{
				DisplayDriver_Text16_B(4,141,White,Magenta,"Fechado");
				status = 1;
			}

			Storage_Write(&status, (FLASH_CALI_ADDR+FLASH_OFFSET_ADDR*4),1);
		}
		else
		{
			if(Bluetooth_switch)
			{
				DisplayDriver_Text16_B(16,141,White,White,"Open");
			}
			else
			{
				DisplayDriver_Text16_B(4,141,White,Baby_Blue,"Fechado");
			}
		}
		break;

	default:
		break;
	}

	Display_Time = 1;
	Exti_lock = ENABLE;
	Display_Battery = 1;
	key_state = DISABLE;
	UI_state = UI_STATE_SETTING_KEY;
	return state;
}

/******************************************************************************/
void UI_Language_Window_About_Machine(void)
{
	Display_Time = 0;
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(4,56,White,"Type:RL-S100");
		DisplayDriver_Text16(4,76,White,"SN:");
		DisplayDriver_Text16(28,76,White,data_SN);
		DisplayDriver_Text16(4,96,White,"HW:1.1");
		DisplayDriver_Text16(4,116,White,"FW:1.9.0304");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(4,56,White,"型号:RL-S100");
		DisplayDriver_Text16(4,76,White,"SN:");
		DisplayDriver_Text16(28,76,White,data_SN);
		DisplayDriver_Text16(4,96,White,"HW:1.1");
		DisplayDriver_Text16(4,116,White,"FW:1.9.0304");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16(4,56,White,"Typ:RL-S100");
		DisplayDriver_Text16(4,76,White,"SN:");
		DisplayDriver_Text16(28,76,White,data_SN);
		DisplayDriver_Text16(4,96,White,"HW:1.1");
		DisplayDriver_Text16(4,116,White,"FW:1.9.0304");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16(4,56,White,"Tipo:BR 3000");
		DisplayDriver_Text16(4,76,White,"SN:");
		DisplayDriver_Text16(28,76,White,data_SN);
		DisplayDriver_Text16(4,96,White,"HW:1.1");
		DisplayDriver_Text16(4,116,White,"FW:1.9.0304");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
void UI_Language_Window_Record(void)
{
	Display_Time = 0;
	DisplayDriver_DrawLine(6,42,120,42,BACKCOLOR_CONTENT_BACK);
	DisplayDriver_DrawLine(6,58,120,58,BACKCOLOR_CONTENT_BACK);
	DisplayDriver_DrawLine(6,90,120,90,BACKCOLOR_CONTENT_BACK);
	DisplayDriver_DrawLine(39,24,39,90,BACKCOLOR_CONTENT_BACK);
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(8,26,Black,"Name");
		DisplayDriver_Text16(41,26,Black,Storage_Data.Product_name);
		DisplayDriver_Text16(8,42,Black,"SN");
		DisplayDriver_Text16(8,58,Black,"Time");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(8,26,Black,"名称");
		DisplayDriver_Text16(41,26,Black,Storage_Data.Product_name);
		DisplayDriver_Text16(8,42,Black,"批号");
		DisplayDriver_Text16(8,58,Black,"时间");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16(8,26,Black,"Name");
		DisplayDriver_Text16(41,26,Black,Storage_Data.Product_name);
		DisplayDriver_Text16(8,42,Black,"SN");
		DisplayDriver_Text16(8,58,Black,"Zeit");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16(8,26,Black,"Nome");
		DisplayDriver_Text16(41,26,Black,Storage_Data.Product_name);
		DisplayDriver_Text16(8,42,Black,"SN");
		DisplayDriver_Text16(8,58,Black,"Hora");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
void UI_Language_Window_No_Record(void)
{
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(24, 80, Red,"No Record!");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(36, 80, Red,"无记录!");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16(20, 80, Red,"Kein rekord!");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16(12, 80, Red,"Sem Registro!");
		break;
	default:
		break;
	}
}

/******************************************************************************/
void UI_Language_Window_Scan_QR_Code (void)
{
	Display_Time = 0;
	Lcd_ColorBox(5, 142,35,15,BACKCOLOR_CONTENT_BACK);
	Lcd_ColorBox(7,62,114, 78,White);

	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16_B(27, 75, Red, White, "Invalid QR");
		DisplayDriver_Text16_B(47, 95, Red, White, "Code");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16_B(24, 85, Red, White, "无效二维码");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16_B(24, 75, Red, White, "Ung$ltiger");
		DisplayDriver_Text16_B(36, 95, Red, White, "QR Code");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16_B(20, 85, Red, White, "O c_digo QR");
		break;

	default:
		break;
	}
	Display_Time = 1;
}

/******************************************************************************/
void Printer_BLE_Print(STORAGE_SINGLE_DATA_STRUCT *content)
{
	uint8 index = 0,index1 = 0;
	memset(buffer,0,sizeof(buffer));
	memset(tbuf,0,sizeof(buffer));
	PCF8563_Read(&SystemManage_CurrentTime);
	sprintf(tbuf,"%d/%02d/%02d %02d:%02d:%02d",SystemManage_CurrentTime.year,
			SystemManage_CurrentTime.month,SystemManage_CurrentTime.day,SystemManage_CurrentTime.hour,
			SystemManage_CurrentTime.min,SystemManage_CurrentTime.sec);

	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		/* Head */
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("------------Realy Tech----------\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		/* Device type */
		Printer_BLE_PrintOneLine("Device Type: RL-S100\r\n");

		buffer[35] = '\0';
		memcpy(&buffer[0], "Product name: ",14);
		memcpy(&buffer[14], content->Product_name,20);
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[15] = '\0';
		memcpy(&buffer[0], "SN: ",sizeof("SN: "));
		memcpy(&buffer[4], content->Product_SN,sizeof(content->Product_SN));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[17] = '\0';
		memcpy(&buffer[0], "Time: ",sizeof("Time: "));
		memcpy(&buffer[6],tbuf,sizeof(tbuf));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));


		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("   Item         Result\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");

		for (index = 0; index < Storage_Data.StripNum; index++)
		{
			buffer[23] = '\0';
			memcpy(&buffer[0],"   ",sizeof("   "));
			memcpy(&buffer[3],content->CH_data[index].TName,8);
			for(index1 = 3;index1 < 11;index1++)
			{
				if(buffer[index1] == '\0')
				{
					buffer[index1] = ' ';
				}
			}
			memcpy(&buffer[11],"      ",sizeof("      "));
			memcpy(&buffer[17],content->CH_data[index].Result,sizeof(content->CH_data[index].Result));
			Printer_BLE_PrintOneLine(&buffer[0]);
			Printer_BLE_PrintOneLine("\r\n");
			memset(buffer,0,sizeof(buffer));
		}
		break;

	case DISPLAY_FONT_CHINESE:
		/* Head */
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("------------Realy Tech----------\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		/* Device type */
		Printer_BLE_PrintOneLine("设备名: RL-S100\r\n");

		memcpy(&buffer[0], "产品名称: ",sizeof("产品名称: "));
		memcpy(&buffer[10], content->Product_name,sizeof( content->Product_name));
		Printer_BLE_PrintOneLine(&buffer[0]);
		buffer[35] = '\0';
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[15] = '\0';
		memcpy(&buffer[0], "批号: ",sizeof("批号: "));
		memcpy(&buffer[6], content->Product_SN,sizeof(content->Product_SN));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[17] = '\0';
		memcpy(&buffer[0], "时间: ",sizeof("时间: "));
		memcpy(&buffer[6],tbuf,sizeof(tbuf));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("   名称          结果\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");

		for (index = 0; index < Storage_Data.StripNum; index++)
		{
			buffer[23] = '\0';
			memcpy(&buffer[0],"   ",sizeof("   "));
			memcpy(&buffer[3],content->CH_data[index].TName,8);
			for(index1 = 3;index1 < 11;index1++)
			{
				if(buffer[index1] == '\0')
				{
					buffer[index1] = ' ';
				}
			}
			memcpy(&buffer[11],"      ",sizeof("      "));
			memcpy(&buffer[17],content->CH_data[index].Result,sizeof(content->CH_data[index].Result));
			Printer_BLE_PrintOneLine(&buffer[0]);
			Printer_BLE_PrintOneLine("\r\n");
			memset(buffer,0,sizeof(buffer));
		}
		break;

	case DISPLAY_FONT_GERMAN:
		/* Head */
		Printer_BLE_PrintOneLine("----------------------------------\r\n");
		Printer_BLE_PrintOneLine("-----------Ulti  Med  ----------\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		/* Device type */
		Printer_BLE_PrintOneLine("Typ: RL-S100\r\n");

		buffer[35] = '\0';
		memcpy(&buffer[0], "Produkt name: ",14);
		memcpy(&buffer[14], content->Product_name,20);
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[15] = '\0';
		memcpy(&buffer[0], "SN: ",sizeof("SN: "));
		memcpy(&buffer[4], content->Product_SN,sizeof(content->Product_SN));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[17] = '\0';
		memcpy(&buffer[0], "Zeit: ",sizeof("Zeit: "));
		memcpy(&buffer[6],tbuf,sizeof(tbuf));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("   Analyt      Ergebnis\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");

		for (index = 0; index < Storage_Data.StripNum; index++)
		{
			buffer[23] = '\0';
			memcpy(&buffer[0],"   ",sizeof("   "));
			memcpy(&buffer[3],content->CH_data[index].TName,8);
			for(index1 = 3;index1 < 11;index1++)
			{
				if(buffer[index1] == '\0')
				{
					buffer[index1] = ' ';
				}
			}
			memcpy(&buffer[11],"      ",sizeof("      "));
			memcpy(&buffer[17],content->CH_data[index].Result,sizeof(content->CH_data[index].Result));
			Printer_BLE_PrintOneLine(&buffer[0]);
			Printer_BLE_PrintOneLine("\r\n");
			memset(buffer,0,sizeof(buffer));
		}
		break;

	case DISPLAY_FONT_PROTUGAL:
		/* Head */
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("-----------Ulti  Med  ----------\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		/* Device type */
		Printer_BLE_PrintOneLine("Tipo de dispositivo: BR 3000\r\n");

		buffer[35] = '\0';
		memcpy(&buffer[0], "Nome do Produto: ",17);
		memcpy(&buffer[17], content->Product_name,20);
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[15] = '\0';
		memcpy(&buffer[0], "SN: ",sizeof("SN: "));
		memcpy(&buffer[4], content->Product_SN,sizeof(content->Product_SN));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));

		buffer[17] = '\0';
		memcpy(&buffer[0], "Hora: ",sizeof("Hora: "));
		memcpy(&buffer[6],tbuf,sizeof(tbuf));
		Printer_BLE_PrintOneLine(&buffer[0]);
		Printer_BLE_PrintOneLine("\r\n");
		memset(buffer,0,sizeof(buffer));


		Printer_BLE_PrintOneLine("--------------------------------\r\n");
		Printer_BLE_PrintOneLine("   Item        Resultado\r\n");
		Printer_BLE_PrintOneLine("--------------------------------\r\n");

		for (index = 0; index < Storage_Data.StripNum; index++)
		{
			buffer[23] = '\0';
			memcpy(&buffer[0],"   ",sizeof("   "));
			memcpy(&buffer[3],content->CH_data[index].TName,8);
			for(index1 = 3;index1 < 11;index1++)
			{
				if(buffer[index1] == '\0')
				{
					buffer[index1] = ' ';
				}
			}
			memcpy(&buffer[11],"      ",sizeof("      "));
			memcpy(&buffer[17],content->CH_data[index].Result,sizeof(content->CH_data[index].Result));
			Printer_BLE_PrintOneLine(&buffer[0]);
			Printer_BLE_PrintOneLine("\r\n");
			memset(buffer,0,sizeof(buffer));
		}
		break;

	default:
		break;
	}

	/* Tail */
	Printer_BLE_PrintOneLine("--------------------------------\r\n");
	Printer_BLE_PrintOneLine("\r\n");
	Printer_BLE_PrintOneLine("\r\n");
}

/******************************************************************************/
void UI_Language_Window_Screw_Motor_Fault (void)
{
	Lcd_ColorBox(0,20,128, 140,White);
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(8, 42, Red,"Error: 001");
		DisplayDriver_Text16(16, 62, Red,"Screw motor");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(8, 42, Red,"错误: 001");
		DisplayDriver_Text16(24, 62, Red,"丝杆电机故障");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16(8, 22, Red,"Fehler: 001");
		DisplayDriver_Text16(12, 42, Red,"Entfernen Sie");
		DisplayDriver_Text16(16, 62, Red,"die Kassette");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16(8, 22, Red,"Erro: 001");
		DisplayDriver_Text16(32, 42, Red,"Parafuso");
		DisplayDriver_Text16(32, 62, Red,"do motor");
		break;

	default:
		break;
	}
}

/******************************************************************************/
void UI_Language_Window_Rotating_Motor_Fault (void)
{
	switch(Font_Switch)
	{
	case DISPLAY_FONT_ENGLISH:
		DisplayDriver_Text16(8, 100, Red,"Fehler: 002");
		DisplayDriver_Text16(8, 120, Red,"Rotating motor");
		break;

	case DISPLAY_FONT_CHINESE:
		DisplayDriver_Text16(8, 100, Red,"错误: 002");
		DisplayDriver_Text16(24, 120, Red,"转动电机故障");
		break;

	case DISPLAY_FONT_GERMAN:
		DisplayDriver_Text16(8, 82, Red,"Erreur: 002");
		DisplayDriver_Text16(24, 102, Red,"Drehen Sie");
		DisplayDriver_Text16(16, 122, Red,"die Kassette");
		break;

	case DISPLAY_FONT_PROTUGAL:
		DisplayDriver_Text16(8, 100, Red,"Erro: 002");
		DisplayDriver_Text16(8, 120, Red,"Motor rotativo");
		break;

	default:
		break;
	}
}

