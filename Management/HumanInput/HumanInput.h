/*
 * HumanInput.h
 *
 *  Created on: 2018Äê1ÔÂ25ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_HUMANINPUT_HUMANINPUT_H_
#define MANAGEMENT_HUMANINPUT_HUMANINPUT_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
extern const unsigned char gImage_LED_Switch[450];

/******************************************************************************/
extern void Key_Left(void);
extern void Key_Right(void);
extern void EXTIX_Init(void);
extern void Key_Confirm(void);
extern void HumanInput_Init(void);
extern void SYSCLKConfig_STOP(void);
extern void EXTI_Key_Left_Enable(void);
extern void EXTI_Key_Right_Enable(void);
extern void EXTI_Key_Left_Disable(void);
extern void EXTI_Key_Right_Disable(void);
extern void EXTI_Key_Confirm_Enable(void);
extern void EXTI_Key_Confirm_Disable(void);
extern void SystemManage_Sleep_Process(void);
extern void SystemManage_CheckPowerOff(void);
extern void SystemManage_EnterExitStop(void);

#endif /* MANAGEMENT_HUMANINPUT_HUMANINPUT_H_ */
