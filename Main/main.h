/*******************************************************************************
 * File name: main.h
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
extern uint8 Power_Open,Power_Switch,Bluetooth_switch,Enter_Sleep,Key_State_Update;
extern uint8 Display_Battery;
extern uint16 adcx;
extern float temp;

/******************************************************************************/
#define One_Minute		59999

/******************************************************************************/
extern void Status_Init(void);
extern void SysTick_Init(u32 ticks);
extern void Delay_ms(__IO uint32 nTime);
extern void TimingDelay_Decrement(void);
extern void Delay_SW(__IO uint32 nCount);
extern void Delay_ms_SW(__IO uint32 nCount);
extern uint8 Interface_Process(uint16* KeyCode);
extern const unsigned char gImage_Power_on2[4608];
extern const unsigned char gImage_Power_on[10240];
extern const unsigned char gImage_Blutooth[252];

#endif /* MAIN_MAIN_H_ */
