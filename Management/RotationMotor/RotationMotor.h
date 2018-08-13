/*
 * motor.h
 *
 *  Created on: 2018Äê1ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_ROTATIONMOTOR_MOTOR_H_
#define MANAGEMENT_ROTATIONMOTOR_MOTOR_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
/* PE11 */
#define RotaMotorDriver_IN1_PORT     (GPIOE)
#define RotaMotorDriver_IN1_PIN      (GPIO_Pin_11)
/* PE12 */
#define RotaMotorDriver_IN2_PORT     (GPIOE)
#define RotaMotorDriver_IN2_PIN      (GPIO_Pin_12)
/* PE13 */
#define RotaMotorDriver_IN3_PORT     (GPIOE)
#define RotaMotorDriver_IN3_PIN      (GPIO_Pin_13)
/* PE14 */
#define RotaMotorDriver_IN4_PORT     (GPIOE)
#define RotaMotorDriver_IN4_PIN      (GPIO_Pin_14)
/* PE15 */
#define RotaMotorDriver_EN_PORT      (GPIOE)
#define RotaMotorDriver_EN_PIN       (GPIO_Pin_15)

#define MOTOR_DISABLED     		 		0
#define MOTOR_ENABLED    		 		1

/******************************************************************************/
#define Foreward_Rotation			 (1)
#define Reversal_Rotation   	 	 (0)
#define Whole_Circle			 	 (512)
/******************************************************************************/
void RotationMotor_Init(void);
void RotaMotorDriver_GPIO_Init(void);
extern void Abnormal_motor_allstop (void);
void RotaMotorDriver_Control(uint8 enabled);
void RotationMotor_SelfCheck_StepDrive(void);
void RotationMotor_StepDrive_Min(uint8 Rotation_Direction);
void RotationMotor_PIN(uint8 Rotation_Direction,uint8 MUTU);
void RotationMotor_Input_StepDrive(uint8 Rotation_Direction,uint16 Step);

#endif /* MANAGEMENT_ROTATIONMOTOR_MOTOR_H_ */

