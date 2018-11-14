/*
 * Interface_System_Time.h
 *
 *  Created on: 2018Äê6ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_SYSTEM_TIME_H_
#define MANAGEMENT_INTERFACE_INTERFACE_SYSTEM_TIME_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
uint8 Year[2]={0};
uint8 Month[1]={0};
uint8 Day[1]={0};
uint8 Hour[1]={0};
uint8 Minute[1]={0};
uint8 Sec[1]={0};

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
} block_attr_System_Time;

/******************************************************************************/
typedef struct {
	uint8 char1_enabled;				/* Support char or not */
	char_attr char1_attr;				/* char attribute */
	uint8 char2_enabled;				/* Support char or not */
	char_attr char2_attr;				/* char attribute */
	uint8 char3_enabled;				/* Support char or not */
	char_attr char3_attr;				/* char attribute */
	uint8 char4_enabled;				/* Support char or not */
	char_attr char4_attr;				/* char attribute */
	uint8 char5_enabled;				/* Support char or not */
	char_attr char5_attr;				/* char attribute */
	uint8 char6_enabled;				/* Support char or not */
	char_attr char6_attr;				/* char attribute */
} block_attr_Set_Time;

/******************************************************************************/
void Line_between(void);
void Display_Time_Set (void);
void Display_Time_Start (void);
void UI_Draw_Window_System_Time(uint16 blockNum);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_SYSTEM_TIME_H_ */
