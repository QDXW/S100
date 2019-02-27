/*
 * Interface_Start.h
 *
 *  Created on: 2018Äê2ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_START_H_
#define MANAGEMENT_INTERFACE_INTERFACE_START_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
	uint8 char_enabled2;					/* Support char or not */
	char_attr char_attr2;				/* char attribute */
} block_attr_Start;

/******************************************************************************/
extern void UI_Draw_Window_Start(uint16 blockNum);
void UI_Draw_Start_font(uint16 blockNum);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_START_H_ */


