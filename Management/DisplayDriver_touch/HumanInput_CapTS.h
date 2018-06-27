/*******************************************************************************
 * File name: HumanInput_CapTS.h
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#ifndef __MANAGEMENT_HUMANINPUT_HUMANINPUT_CAPTS_H_
#define __MANAGEMENT_HUMANINPUT_HUMANINPUT_CAPTS_H_

/******************************************************************************/
#include "main.h"
#include "comDef.h"
#include "stm32f10x.h"

/******************************************************************************/
#define CAPTS_CLICK_MAX_TOUCHES              (0x20)
#define CAPTS_CLICK_MAX_DISTANCE             (30)

#define CAPTS_I2C                            (I2C2)
#define CAPTS_INT_PORT                       (GPIOD)
#define CAPTS_INT_PIN                        (GPIO_Pin_12)

#define CAPTS_RST_PORT                       (GPIOD)
#define CAPTS_RST_PIN                        (GPIO_Pin_13)

/******************************************************************************/
#define CAPTS_INT_EXTI                       (EXTI_Line12)

#define CapTS_INT_STATE() 	                 \
	GPIO_ReadInputDataBit(CAPTS_INT_PORT, CAPTS_INT_PIN)

#define CapTS_I2C_Speed                      (100000) /* 100kHz */
#define CAPTS_ADDR                           (0x70) /* Device address */
#define CAPTS_FINGER_THRESHOLD               (200) /* Finger threshold */

#define FT_DEVIDE_MODE 			0x00   		//FT6236 mode control register
#define FT_REG_NUM_FINGER       0x02		//Status register

#define FT_TP1_REG 				0X03	  	//1st point
#define FT_TP2_REG 				0X09		//2nd point
#define FT_TP3_REG 				0X0F		//3rd
#define FT_TP4_REG 				0X15		//4th
#define FT_TP5_REG 				0X1B		//5th

#define	FT_ID_G_LIB_VERSION		0xA1		//Version
#define FT_ID_G_MODE 			0xA4   		//FT6236 interrupt mode control register
#define FT_ID_G_THGROUP			0x80   		//Touch valid value set register
#define FT_ID_G_PERIODACTIVE	0x88   		//Activation status period set register
#define Chip_Vendor_ID          0xA3        //Silicon ID(0x36)
#define ID_G_FT6236ID			0xA8		//0x11

#define TP_PRES_DOWN            0x80        //Touch down
#define TP_COORD_UD             0x40        //Touch updated flag
#define TP_PRES_LIFT            0x20        //Touch down

/******************************************************************************/
typedef struct
{
	u8 TouchSta;	//触摸情况，b7:按下1/松开0; b6:0没有按键按下/1有按键按下;bit5:保留；bit4-bit0触摸点按下有效标志，有效为1，分别对应触摸点5-1；
	u16 x[5];
	u16 y[5];

}TouchPointRefTypeDef;

/******************************************************************************/
extern TouchPointRefTypeDef TPR_Structure;

/******************************************************************************/
void HumanInput_CapTS_Init(void);
uint8 HumanInput_CapTS_SelfTestAndInit(void);
void HumanInput_CapTS_Process(void);
void CapTS_EnterDeepSleep(void);
void CapTS_ExitDeepSleep(void);
void HumanInput_CapTS_Int(FunctionalState state);

#endif /* __MANAGEMENT_HUMANINPUT_HUMANINPUT_CAPTS_H_ */
