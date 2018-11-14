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
typedef struct {
	uint8 line_enabled; 				/* Support Parting line or not */
	line_attr Parting_line_attr;		/* Parting line attribute */
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
uint8 char2_enabled;					/* Support char or not */
	char_attr char2_attr;				/* char attribute */
uint8 char3_enabled;					/* Support char or not */
	char_attr char3_attr;				/* char attribute */
} block_attr_Setting_font;

/******************************************************************************/
void UI_Draw_Window_Setting(uint16 blockNum);
void UI_Draw_Window_Setting_font(uint16 blockNum);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_SETTING_H_ */
