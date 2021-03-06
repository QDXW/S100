/*
 * QRCode.h
 *
 *  Created on: 2018��3��7��
 *      Author: Administrator
 */

#ifndef MANAGEMENT_QRCODE_QRCODE_H_
#define MANAGEMENT_QRCODE_QRCODE_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#define QRCODE_TEST_NAME_MAX         (8)
#define QRCODE_BUFFER_SIZE           (500)

#define QRCODE_TRIG_PORT      		 (GPIOA)
#define QRCODE_TRIG_PIN         	 (GPIO_Pin_3)

#if	 LCD_OLD
#define QRCODE_TX_PORT      		 (GPIOA)
#define QRCODE_TX_PIN         		 (GPIO_Pin_9)

#define QRCODE_RX_PORT     			 (GPIOA)
#define QRCODE_RX_PIN        		 (GPIO_Pin_10)

#define QRCODE_USART          		 (USART1)
#define QRCODE_USART_IRQN          	 (USART1_IRQn)

#else

#define QRCODE_TX_PORT      		 (GPIOD)
#define QRCODE_TX_PIN         		 (GPIO_Pin_8)

#define QRCODE_RX_PORT     			 (GPIOD)
#define QRCODE_RX_PIN        		 (GPIO_Pin_9)

#define QRCODE_USART          		 (USART3)
#define QRCODE_USART_IRQN          	 (USART3_IRQn)
#endif

/******************************************************************************/

extern uint8 QRCode_Buffer[QRCODE_BUFFER_SIZE];

/******************************************************************************/
typedef struct {
    uint16 crc;
    uint8 Model;
    uint8 name[20];
    uint8 SN[10];
    uint8 time;
    uint8 midC;
    uint8 distC_T;
    uint8 distC_Base1;
    uint8 distC_Base2;
    uint8 distC_Base3;
    uint8 distC_Base4;
    uint8 stripNum;
    uint16 searchHalfRadius_C;
    uint16 searchHalfRadius_T;
    uint16 areaC_HalfRadius;
    uint16 areaT_HalfRadius;
    uint16 winSize;
    uint8 limitEnabled;
    uint8 Invalied;
    uint16 C_stepSize;
    uint16 C_magnitude;
    uint16 C_MIN;
} QRCODE_HEAD_STRUCT;

/******************************************************************************/
typedef struct {
    uint8 TName[QRCODE_TEST_NAME_MAX];
    uint8 Switch_Bool;
    float threshold1;
    float threshold2;
    float threshold3;
} QRCODE_SINGLE_LINE;

/******************************************************************************/
typedef struct {
    QRCODE_HEAD_STRUCT head;
    QRCODE_SINGLE_LINE ch_data[12];
} QRCODE_STRUCT;

/******************************************************************************/
extern QRCODE_STRUCT QR_Date;
extern QRCODE_STRUCT QR_Date_Analyze;

/******************************************************************************/
extern void Clear_Data(void);
extern void QRCode_Init(void);
extern void QRCode_Restart(void);
extern void QRCode_Received(void);
extern void QRCode_GPIO_Init(void);
extern void QRCode_Uart_Init(void);
extern uint8 QRCode_Identify(void);
extern void QRCode_Uart_Config(void);
extern void QRCode_Uart_GPIO_Init(void);
extern void QRCode_Trigger_Enabled(void);
extern void QRCode_Trigger_Disabled(void);
extern void QRCode_Uart_NVIC_Configuration(void);

#endif /* MANAGEMENT_QRCODE_QRCODE_H_ */
