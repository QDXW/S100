/*******************************************************************************
 * File name: stm32f10x_it.h
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#ifndef ISR_STM32F10x_IT_H
#define ISR_STM32F10x_IT_H

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
extern uint8 ScanMotorDriver_InBasePosition;
uint16 se_count = 0,keyupCnt = 0,key_holdon_ms = 0;

/******************************************************************************/
void SVC_Handler(void);
void NMI_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void BusFault_Handler(void);
void DebugMon_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void UsageFault_Handler(void);
void Display_Down_Time_Msec (void);
void Display_Down_Time_second (void);
					 
#endif /* ISR_STM32F10x_IT_H */
