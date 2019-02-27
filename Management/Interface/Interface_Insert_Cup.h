/*
 * Interface_Insert_Cup.h
 *
 *  Created on: 2018Äê6ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_INSERT_CUP_H_
#define MANAGEMENT_INTERFACE_INTERFACE_INSERT_CUP_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
} block_attr_Insert_Cup;

/******************************************************************************/
extern void UI_Draw_Window_Insert_Cup(uint16 blockNum);
extern void Display_Down_Time_Bar(void);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_INSERT_CUP_H_ */
