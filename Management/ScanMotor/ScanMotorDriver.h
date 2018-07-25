/*
 * ScanScanMotorDriver.h
 *
 *  Created on: 2018Äê3ÔÂ2ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_SCANMOTOR_SCANScanMotorDriver_H_
#define MANAGEMENT_SCANMOTOR_SCANScanMotorDriver_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
/* PE11 */
#define ScanMotorDriver_IN1_PORT     (GPIOC)
#define ScanMotorDriver_IN1_PIN      (GPIO_Pin_4)
/* PE12 */
#define ScanMotorDriver_IN2_PORT     (GPIOC)
#define ScanMotorDriver_IN2_PIN      (GPIO_Pin_5)
/* PE13 */
#define ScanMotorDriver_IN3_PORT     (GPIOB)
#define ScanMotorDriver_IN3_PIN      (GPIO_Pin_0)
/* PE14 */
#define ScanMotorDriver_IN4_PORT     (GPIOB)
#define ScanMotorDriver_IN4_PIN      (GPIO_Pin_1)
/* PE15 */
#define ScanMotorDriver_EN_PORT      (GPIOA)
#define ScanMotorDriver_EN_PIN       (GPIO_Pin_6)

#define MOTOR_DISABLED     		 		0
#define MOTOR_ENABLED    		 		1

#define ScanMotorDriver_DIR_OUT     	0
#define ScanMotorDriver_DIR_IN    	 	1

/******************************************************************************/
#define MOTOR_TOTAL_STEPS        (240)
#define MOTOR_HALF_STEPS         (147)
#define MOTOR_GOTO_DET_POS_STEPS (50)

#define MOTOR_SAMPLE_STEPS       (MOTOR_TOTAL_STEPS - MOTOR_GOTO_DET_POS_STEPS)

/******************************************************************************/
extern uint8 ScanMotorDriver_InBasePosition;
extern uint8 MotorDriver_Ctr;

/******************************************************************************/
extern void ScanMotorDriver_Enable(void);
extern void ScanMotorDriver_Disable(void);
extern void ScanMotorDriver_PWM_LOW(void);
extern void ScanMotorDriver_Process(void);
extern void SystemManage_5V_Enabled(void);
extern void SystemManage_5V_Disabled(void);
extern void ScanScanMotorDriver_Init(void);
extern void ScanMotorDriver_PWM_HIGH(void);
extern void ScanMotorDriver_PWM_Toggle(void);
extern void ScanMotorDriver_StartDetection(void);
extern void ScanMotorDriver_Control(uint8 enabled);
extern void ScanMotorDriver_Goto_BasePosition(void);
extern uint8 ScanMotorDriver_SelfTest_Mechanic(void);
extern void ScanMotorDriver_Goto_CentrePosition(void);
extern void ScanMotorDriver_SelfCheck_StepDrive(void);
extern void ScanMotorDriver_Set_Direction(u8 direction);
extern void ScanMotorDriver_Goto_DetectionPosition(void);
extern void ScanMotorDriver_Goto_OutPosition(uint32 steps);
extern void ScanMotorDriver_Move(uint8 direction, uint32 steps);
extern void ScanMotorDriver_MovePerPulseNumber(uint8 direction, uint32 pulseNumber);
extern void ScanMotorDriver_MoveOneStep(uint8 dir);
#endif /* MANAGEMENT_SCANMOTOR_SCANScanMotorDriver_H_ */
