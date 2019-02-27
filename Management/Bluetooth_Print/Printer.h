/*******************************************************************************
 * File name: Print.h
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#ifndef __MANAGEMENT_PRINTER_PRINTER_H_
#define __MANAGEMENT_PRINTER_PRINTER_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#define PRINTER_USART (UART4)

#define PRINTER_TX_PORT (GPIOC)
#define PRINTER_TX_PIN (GPIO_Pin_10)
#define PRINTER_RX_PORT (GPIOC)
#define PRINTER_RX_PIN (GPIO_Pin_11)

#define PRINTER_MATCH_STATUS_PORT (GPIOC)
#define PRINTER_MATCH_STATUS_PIN (GPIO_Pin_9)

/******************************************************************************/
typedef struct {
	uint8 *Product_name;
	uint8 *Product_SN;
	uint8 *StripNum;
	uint8 *Product_Time;
	uint8 *result;
	uint8 *item1;
	uint8 *item2;
	uint8 *item3;
	uint8 *item4;
	uint8 *item5;
	uint8 *item6;
	uint8 *item7;
	uint8 *item8;
	uint8 *item9;
	uint8 *item10;
	uint8 *item11;
	uint8 *item12;
} PRINT_CONTENT;

extern PRINT_CONTENT Printer_Result;

/******************************************************************************/
extern void Printer_BLE_Print(STORAGE_SINGLE_DATA_STRUCT *content);
extern void Printer_BLE_Init(void);
extern void BLE_Timer_Disabled(void);
extern void BLE_Timer_Enabled(void);
extern void SetBLEStaus(bool STAUS);
extern void BLE_Send_Phone(uint8 *data);
extern void Set_BLE_Exit_STATE(FunctionalState STATE);
uint8 Printer_isConnected(void);
void UI_Process_BLE_Print(void);

#endif /* __MANAGEMENT_PRINTER_PRINTER_H_ */
