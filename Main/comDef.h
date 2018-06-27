/*******************************************************************************
 * File name: comDef.h
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#ifndef MAIN_COMDEF_H_
#define MAIN_COMDEF_H_

/******************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_can.h"
#include "stm32f10x_cec.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_wwdg.h"

/******************************************************************************/
/* exact-width signed integer types */
typedef   signed          char int8;
typedef   signed short     int int16;
typedef   signed           int int32;
typedef   signed       __int64 int64;

/* exact-width unsigned integer types */
typedef unsigned          char uint8;
typedef unsigned short     int uint16;
typedef unsigned           int uint32;
typedef unsigned       __int64 uint64;

/******************************************************************************/
enum NORMAL_ON_OFF{
	NORMAL_OFF = 0,
	NORMAL_ON = 1,
};

enum ABNORMAL_ON_OFF{
	ABNORMAL_ON = 0,
	ABNORMAL_OFF = 1,
};

#endif /* MAIN_COMDEF_H_ */
