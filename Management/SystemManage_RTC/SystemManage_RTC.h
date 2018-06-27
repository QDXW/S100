/*
 * SystemManage_RTC.h
 *
 *  Created on: 2018Äê3ÔÂ12ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_SYSTEMMANAGE_RTC_SYSTEMMANAGE_RTC_H_
#define MANAGEMENT_SYSTEMMANAGE_RTC_SYSTEMMANAGE_RTC_H_

/******************************************************************************/
#include "comDef.h"
#include "stm32f10x.h"

/******************************************************************************/
#define SECONDS_PER_DAY (86400) 			//24*60*60
#define SECONDS_PER_LEAP_YEAR (31622400) 	//366*24*60*60
#define SECONDS_PER_NORMAL_YEAR (31536000)  //365*24*60*60

/******************************************************************************/
#define SCL_H() GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define SCL_L() GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define SDA_H() GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define SDA_L() GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define Read_SDA() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define ReadCode 0xa3
#define WriteCode 0xa2

/******************************************************************************/
typedef struct
{
	uint16 year;
	uint8 month;
	uint8 day;
	uint8 hour;
	uint8 min;
	uint8 sec;
} RTC_DATA;

/******************************************************************************/
/* Current time */
extern RTC_DATA SystemManage_CurrentTime;
/* Record time */
extern RTC_DATA SystemManage_RecordTime;
/* User setting time */
extern RTC_DATA SystemManage_UserSetTime;

#endif /* MANAGEMENT_SYSTEMMANAGE_RTC_SYSTEMMANAGE_RTC_H_ */
