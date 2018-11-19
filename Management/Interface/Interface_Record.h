/*
 * Interface_Record.h
 *
 *  Created on: 2018Äê2ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_RECORD_H_
#define MANAGEMENT_INTERFACE_INTERFACE_RECORD_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
 	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
	uint8 char2_enabled;					/* Support char or not */
	char_attr char2_attr;				/* char attribute */
} block_attr_Record;

/******************************************************************************/
extern void UI_Draw_Window_Record(uint16 blockNum);
void UI_Language_Window_Record(void);
extern void Data_Changer(void);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_RECORD_H_ */
