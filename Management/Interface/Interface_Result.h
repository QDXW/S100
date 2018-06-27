/*
 * Interface_Result.h
 *
 *  Created on: 2018Äê2ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_RESULT_H_
#define MANAGEMENT_INTERFACE_INTERFACE_RESULT_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
uint16 Char_Count = 1;
uint8 Page_Flag = 0;
extern uint8 UI_state,key_state,Exti_lock,Key_control,Interface_Key;

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
	uint8 char2_enabled;					/* Support char or not */
	char_attr char2_attr;				/* char attribute */
	uint8 pic_enabled;     				/* Support picture or not */
	pic_attr pic_attr;
} block_attr_Result;

/******************************************************************************/
extern void UI_Draw_Window_Result(uint16 blockNum);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_RESULT_H_ */
