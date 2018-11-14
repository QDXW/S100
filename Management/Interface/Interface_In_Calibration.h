/*
 * Interface_In_Calibration.h
 *
 *  Created on: 2018Äê9ÔÂ3ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_IN_CALIBRATION_H_
#define MANAGEMENT_INTERFACE_INTERFACE_IN_CALIBRATION_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
typedef struct {
	uint8 Interface_Status;
	uint8 pic_enabled;     				/* Support picture or not */
	pic_attr pic_attr;					/* picture attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
} block_attr_In_Calibration;

/******************************************************************************/
uint8 Dichotomy_Range(void);
extern uint8 Dichotomy_Calculate (void);
void UI_Background_Plate_In_Calibration(void);
void UI_Draw_Window_In_Calibration(uint16 blockNum);
uint8 Dichotomy_Search(uint16 *Signal,uint8 Dichotomy_Value);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_IN_CALIBRATION_H_ */
