/*******************************************************************************
 * File name: Common.h
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#ifndef __MANAGEMENT_COMMON_COMMON_H_
#define __MANAGEMENT_COMMON_COMMON_H_

/******************************************************************************/
#include "main.h"
#include "stm32f10x.h"
#include "comDef.h"

/******************************************************************************/
uint16 Common_CalculateCRC(uint8 * message,uint32 length,
		uint16 remainder, uint16 xorMask);
void Common_EXTI_Init(GPIO_TypeDef* port, uint16 pin,
		uint8 portSrc, uint8 pinSrc, uint32 extiLine, EXTITrigger_TypeDef type,
		FunctionalState defaultCmd, uint8 irqCh, uint8 prePri, uint8 subPri);
void SoftReset(void);

#endif /* __MANAGEMENT_COMMON_COMMON_H_ */
