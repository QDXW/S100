/*
 * Global_variable.c
 *
 *  Created on: 2018Äê9ÔÂ5ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "Global_variable.h"
#include "stm32f10x.h"
#include "comDef.h"

/******************************************************************************/
float temp = 0.0;
uint8 cBuffer[20] = {0},tBuffer[10] = {0},data_SN[12] = {0};
uint8 Calibration_channel = 0,Poll_Count = 0;
uint8 Page_Flag = 0,page_tatol = 1,page_Num = 1,Time_Count = 1;
uint8 Check_Lock = 0,Existed_Data = 0,Stop_Mode = 0,Result_Judge = 0;
uint8 UI_runMode = 0,Check_motor = 0,Exti_lock = DISABLE;
uint8 QRCode_received = 0,QRCode_existed = 0,Action_time = 0,Quick_Second = 0;
uint8 Cup_Count = 0,Read_first = 1,Key_record = 1,Cup_Exist = 0;
uint8 key_state = 1,key_open = 1,Key_control = 1,Interface_Key = 0;
uint8 Display_Time = 1,Open_time = 0,Power_Open = 0,Key_State_Update = 0;
uint8 key_fall_flag = 0,short_key_flag = 0,doubleClick = 0,long_key_flag = 0;
uint8 Power_Switch = 0,Bluetooth_switch = 0,Enter_Sleep = 0,Display_Battery = 1;
uint8 key_state_confirm = 0,MotorDriver_Ctr = 0,UI_state = 0,Quick_Down_time = 0;

uint16 UI_WindowBlocks = 0,Power_Second = 0,Power_Minute = 0;
uint16 reagent_Strip[4] = {0},SignalProcess_sampleBuffer[512] = {0};
uint16 adcx = 0,Data_Boundary = 800,SignalSample_count = 0,QRCode_count = 0;

uint32 Data_SN = 0;
