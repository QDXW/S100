/*
 * Battery_ADC.h
 *
 *  Created on: 2018Äê6ÔÂ16ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_BATTERY_ADC_BATTERY_ADC_H_
#define MANAGEMENT_BATTERY_ADC_BATTERY_ADC_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#define Battery_ADC_IN1_PORT     (GPIOC)
#define Battery_ADC_IN1_PIN      (GPIO_Pin_1)

/******************************************************************************/
uint16 Get_Adc(uint8 ch);
extern void  Adc_Init(void);
uint16 Get_Adc_Average(uint8 ch,uint8 times);

#endif /* MANAGEMENT_BATTERY_ADC_BATTERY_ADC_H_ */
