/*
 * Interface_Quick.h
 *
 *  Created on: 2018Äê2ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_QUICK_H_
#define MANAGEMENT_INTERFACE_INTERFACE_QUICK_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
extern uint8 Cup_Exist;
extern uint8 Exti_lock;
extern uint8 key_state;
extern uint8 Key_control;
extern uint8 Interface_Key;
uint16 UI_WindowBlocks_Quick = 0;
uint16 UI_WindowBlocks_Quick_font = 0;

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
	uint8 pic_enabled;     				/* Support picture or not */
	pic_attr pic_attr;					/* picture attribute */
} block_attr_Quick;

/******************************************************************************/
extern void UI_Draw_Window_Quick(uint16 blockNum);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_QUICK_H_ */
