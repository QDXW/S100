/*
 * Interface_Input_Cup.h
 *
 *  Created on: 2018Äê3ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_INSERT_CUP_H_
#define MANAGEMENT_INTERFACE_INTERFACE_INSERT_CUP_H_

/******************************************************************************/
#include "Interface_main.h"

/******************************************************************************/
uint16 UI_WindowBlocks_Insert_Cup = 0;

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
} block_attr_Insert_Cup;

/******************************************************************************/
extern void UI_Draw_Window_Insert_Cup(uint16 blockNum);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_INSERT_CUP_H_ */
