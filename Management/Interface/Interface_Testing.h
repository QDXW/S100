/*
 * Interface_Testing.h
 *
 *  Created on: 2018Äê2ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_TESTING_H_
#define MANAGEMENT_INTERFACE_INTERFACE_TESTING_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#define  NO_CUP        		   		(0)

/******************************************************************************/
uint8 Confirm_CUP = 0;
extern uint8 Exti_lock;
extern uint8 key_state;
extern uint8 Key_control;
extern uint8 NowCup_Count;
extern uint16 BOUNDARY_VALUE;
uint16 UI_WindowBlocks_Testing = 0;

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
} block_attr_Testing;

/******************************************************************************/
extern uint16 Get_Start_Postion(void);

extern void Acquisition_Signal(void);
uint16 Get_sampleBuffer_Max_Value(void);
void Get_sampleBuffer_Start_Position(void);
uint16 Get_sampleBuffer_Boundary_Value(void);
extern void UI_Draw_Window_Testing(uint16 blockNum);
void QR_Date_SignalProcess_Alg_data (void);
void Result_Judge(void);
uint16 Calculate_Start_Postion(uint16* Signal,uint16 Postion);
void SignalSample_Moving_Average_Data(uint16 *Data,uint16 Length,uint16 Period);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_TESTING_H_ */
