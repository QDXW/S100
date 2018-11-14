/*
 * Interface_main.h
 *
 *  Created on: 2018Äê2ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_MAIN_H_
#define MANAGEMENT_INTERFACE_INTERFACE_MAIN_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#define UI_STATE_RERUN (1u)

/******************************************************************************/
/* Rectangular attribute */
typedef struct {
	uint16 startX; 			/* Start X */
	uint16 startY; 			/* Start Y */
	uint16 width; 			/* Width */
	uint16 height; 			/* Height */
	uint16 color; 			/* Color */
} rect_attr;

/******************************************************************************/
/* Straight line */
typedef struct {
	uint16 startX; 			/* Start X */
	uint16 startY; 			/* Start Y */
	uint16 endX; 			/* EndX */
	uint16 endY; 			/* EndY */
	uint16 color; 			/* Color */
} line_attr;

/******************************************************************************/
/* Picture attribute */
typedef struct {
	const uint8* src; 		/* Picture data array */
	uint16 offsetX;			/* Offset X away from rect_attr.startX */
	uint16 offsetY; 		/* Offset Y away from rect_attr.startY */
	uint16 width; 			/* Picture width */
	uint16 height;			/* Picture height */
} pic_attr;

/******************************************************************************/
/* Character attribute */
typedef struct {
	uint8* str; 			/* String to be shown */
	uint16 offsetX;			/* Offset X away from rect_attr.startX */
	uint16 offsetY; 		/* Offset Y away from rect_attr.startY */
	uint16 color; 			/* Font color */
	uint16 backColor; 		/* Back color */
	uint16 faceColor; 		/* face color */
} char_attr;

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 pic_enabled;     				/* Support picture or not */
	pic_attr pic_attr;     				/* Picture attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
	uint8 line_enabled; 				/* Support Parting line or not */
	line_attr Parting_line_attr;		/* Parting line attribute */
} block_attr;

/******************************************************************************/
typedef struct {
	uint8 char1_enabled;					/* Support char or not */
	char_attr char1_attr;				/* char attribute */
	uint8 char2_enabled;					/* Support char or not */
	char_attr char2_attr;				/* char attribute */
	uint8 char3_enabled;					/* Support char or not */
	char_attr char3_attr;				/* char attribute */
	uint8 char4_enabled;					/* Support char or not */
	char_attr char4_attr;				/* char attribute */
} block_font_attr;

/******************************************************************************/
typedef enum {
	UI_STATE_MAIN_WINDOW, 			/* Interface main display */
	UI_STATE_KEY_STATE, 			/* Interface key state */
	UI_STATE_MAIN_FONT, 			/* Interface main font display */
	UI_STATE_START,					/* Interface Start Display */
	UI_STATE_QUICK, 				/* Interface Quick Display */
	UI_STATE_RECORD, 				/* Interface Record Display */
	UI_STATE_SETTING, 				/* Interface Setting Display */
	UI_STATE_TESTING,				/* Interface Testing Display */
	UI_STATE_RESULT,				/* Interface Result Display */
	UI_STATE_RESULT_2,				/* Interface Start Display */
	UI_STATE_INSERT_CUP,			/* Interface insert cup Display */
	UI_STATE_START_FONT,			/* Interface Start font Display */
	UI_STATE_RECORD2, 				/* Interface Record2 Display */
	UI_STATE_SETTING_FONT,			/* Interface Start font Display */
	UI_STATE_ABOUT_MACHINE,			/* Interface about machine Display */
	UI_STATE_SYSTEM_TIME,			/* Interface system time Display */
	UI_STATE_TIME_PROCESS,			/* Interface time Display */
	UI_STATE_DOWN_TIME_PROCESS,		/* Interface down time Display */
	UI_STATE_BLUET_SWITCH_PROCESS,	/* Interface bluetooth switch Display */
	UI_STATE_IN_CALIBRATION_PROCESS,/* Interface calibration Display */
	UI_STATE_CALIBRATION_KEY_PROCESS,	/* Interface calibration key Display */

	UI_STATE_MAX_STATE_NUM,
} UI_STATE;

/******************************************************************************/
void Battery_Display (void);
extern void Battery_Empty_ICO(void);
extern void UI_Draw_Status_Bar (void);
extern uint16 Get_Start_Postion(void);
extern void Bluetooth_Connection (void);
extern void UI_Draw_Window(uint16 blockNum);
extern uint8 Interface_Main(uint16 KeyCode);
extern uint8 Interface_Quick(uint16 KeyCode);
extern uint8 Interface_Start(uint16 KeyCode);
extern uint8 Interface_Record(uint16 KeyCode);
extern uint8 Interface_Result(uint16 KeyCode);
extern uint8 Interface_Testing(uint16 KeyCode);
extern uint8 Interface_Setting(uint16 KeyCode);
extern uint8 Interface_Record_2(uint16 KeyCode);
extern uint8 Interface_Result_2(uint16 KeyCode);
extern uint8 Interface_Standard(uint16 KeyCode);
extern void UI_Draw_Window_font(uint16 blockNum);
extern uint8 Interface_Main_font(uint16 KeyCode);
extern uint8 Interface_Key_Event(uint16 KeyCode);
extern uint8 Interface_Start_font(uint16 KeyCode);
extern uint8 Interface_Insert_Cup(uint16 KeyCode);
extern uint8 Interface_System_Time(uint16 KeyCode);
extern uint8 Interface_Time_Process(uint16 KeyCode);
extern uint8 Interface_Setting_font(uint16 blockNum);
extern uint8 Interface_Standard_font(uint16 KeyCode);
extern uint8 Interface_About_Machine(uint16 KeyCode);
extern void UI_Draw_Window_Quick_font(uint16 blockNum);
extern uint8 Interface_Down_Time_Process(uint16 blockNum);
extern uint8 Interface_Bluet_switch_Process(uint16 blockNum);
extern uint8 Interface_In_Calibration_Process(uint16 blockNum);
extern uint8 Interface_Calibration_key_Process(uint16 blockNum);
extern void SignalSample_Moving_Average_Data(uint16 *Data,uint16 Length,uint16 Period);



#endif /* MANAGEMENT_INTERFACE_INTERFACE_MAIN_H_ */
