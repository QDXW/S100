/*
 * Interface_About_Machine.h
 *
 *  Created on: 2018Äê6ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_ABOUT_MACHINE_H_
#define MANAGEMENT_INTERFACE_INTERFACE_ABOUT_MACHINE_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
} block_attr_About_Machine;

/******************************************************************************/
void UI_Draw_Window_About_Machine(uint16 blockNum);
void UI_Language_Window_About_Machine(void);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_ABOUT_MACHINE_H_ */
