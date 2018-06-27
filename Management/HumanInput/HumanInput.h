/*
 * HumanInput.h
 *
 *  Created on: 2018Äê1ÔÂ25ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_HUMANINPUT_HUMANINPUT_H_
#define MANAGEMENT_HUMANINPUT_HUMANINPUT_H_

/******************************************************************************/
#include "stm32f10x_exti.h"
#include "comDef.h"
#include "Interface_main.h"

/******************************************************************************/
uint8 key_open = 1;
uint8 key_state = 1;
uint8 Key_control = 1;
uint8 Interface_Key = 0;
uint8 key_state_confirm = 0;

/******************************************************************************/
extern void Key_Left(void);
extern void Key_Right(void);
extern void EXTIX_Init(void);
extern void Key_Confirm(void);
extern void HumanInput_Init(void);
extern void EXTI_Key_Confirm_Enable(void);
extern void EXTI_Key_Confirm_Disable(void);
extern void SystemManage_CheckPowerOff(void);

#endif /* MANAGEMENT_HUMANINPUT_HUMANINPUT_H_ */
