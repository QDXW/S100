/*
 * RotationMotorDriver_PositionSensor.h
 *
 *  Created on: 2018Äê3ÔÂ6ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_SCANMOTOR_ROTATIONMOTORDRIVER_POSITIONSENSOR_H_
#define MANAGEMENT_SCANMOTOR_ROTATIONMOTORDRIVER_POSITIONSENSOR_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#define ROTA_POSSEN_INT_PORT      (GPIOA)
#define ROTA_POSSEN_INT_PIN       (GPIO_Pin_2)
#define ROTA_POSSEN_INT_EXTI      (EXTI_Line2)
#define ROTA_POSSEN_EXTI_PORT     (GPIO_PortSourceGPIOA)
#define ROTA_POSSEN_EXTI_PIN      (GPIO_PinSource2)
#define ROTA_POSSEN_EXTI_CH       (EXTI2_IRQn)

#define ROTA_POSSEN_INT_STATE()	(GPIO_ReadInputDataBit(ROTA_POSSEN_INT_PORT, ROTA_POSSEN_INT_PIN))

/******************************************************************************/
void RotaMotorDriver_PositionSensor_Int_Enable(void);
void RotaMotorDriver_PositionSensor_Int_Disable(void);
void RotaMotorDriver_PositionSensor_Init(void);

#endif /* MANAGEMENT_SCANMOTOR_ROTATIONMOTORDRIVER_POSITIONSENSOR_H_ */
