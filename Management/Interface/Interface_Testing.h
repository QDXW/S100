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

/******************************************************************************/
typedef struct {
	uint8 rect_enabled; 				/* Support rectangular or not */
	rect_attr rect_attr;				/* Rectangular attribute */
} block_attr_Testing;

/******************************************************************************/
void Result_Display(void);
void Acquisition_Signal(void);
void Acquisition_StartSignal(void);
extern uint16 Get_Start_Postion(void);
void UI_Language_block_Testing (void);
void UI_Language_Window_Testing(void);
uint16 Get_sampleBuffer_Max_Value(void);
void Get_sampleBuffer_Start_Position(void);
void QR_Date_SignalProcess_Alg_data (void);
uint16 Get_sampleBuffer_Boundary_Value(void);
void UI_Draw_Window_Testing(uint16 blockNum);
extern void Calculate_Max_Postion(uint16 First_Postion);
extern uint16 Judge_Max (uint16* Signal,uint16 Before_Postion);
uint16 Calculate_Postion_Up(uint16* Signal,uint16 Postion);
uint16 Check_Trend(uint16 *Signal,uint16 Postion,uint8 Flag);
uint16 Calculate_Postion_Down(uint16* Signal,uint16 Postion);
void SignalSample_Moving_Average_Data(uint16 *Data,uint16 Length,uint16 Period);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_TESTING_H_ */
