/*
 * Interface_Setting.h
 *
 *  Created on: 2018Äê2ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_SETTING_H_
#define MANAGEMENT_INTERFACE_INTERFACE_SETTING_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
extern uint8 key_state;
extern uint8 Exti_lock;
extern uint8 Key_control;
extern uint8 Interface_Key;
extern uint8 key_state_confirm;
uint16 UI_WindowBlocks_Setting = 0;
extern const unsigned char gImage_PIC_System_Time[4050];
extern const unsigned char gImage_PIC_About_Machine[4050];

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
	uint8 pic_enabled;     				/* Support picture or not */
	pic_attr pic_attr;     				/* Picture attribute */
} block_attr_Setting;

/******************************************************************************/
typedef struct {
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
	uint8 char1_enabled;					/* Support char or not */
	char_attr char1_attr;				/* char attribute */
} block_attr_Setting_font;

/******************************************************************************/
void UI_Draw_Window_Setting(uint16 blockNum);
void UI_Draw_Window_Setting_font(uint16 blockNum);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_SETTING_H_ */
