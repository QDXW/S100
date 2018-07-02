/*
 * HumanInput.h
 *
 *  Created on: 2018��1��25��
 *      Author: Administrator
 */

#ifndef MANAGEMENT_HUMANINPUT_HUMANINPUT_H_
#define MANAGEMENT_HUMANINPUT_HUMANINPUT_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
uint8 key_open = 1;
uint8 key_state = 1;
uint8 Key_control = 1;
uint8 Interface_Key = 0;
uint8 key_state_confirm = 0;
extern const unsigned char gImage_LED_Switch[570];

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
extern void SystemManage_CheckPowerOff(void);
extern void SystemManage_EnterExitStop(void);

#endif /* MANAGEMENT_HUMANINPUT_HUMANINPUT_H_ */
