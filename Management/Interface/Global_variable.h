/*
 * Global_variable.h
 *
 *  Created on: 2018Äê9ÔÂ5ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#ifndef MANAGEMENT_INTERFACE_GLOBAL_VARIABLE_H_
#define MANAGEMENT_INTERFACE_GLOBAL_VARIABLE_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
extern float temp;
extern uint8 cBuffer[20],tBuffer[10],data_SN[12],SignalBuffer[1024];
extern uint8 Calibration_channel,Poll_Count,Quick_Down_time;
extern uint8 key_fall_flag,short_key_flag,long_key_flag,Result_Judge;
extern uint8 Power_Open,Power_Switch,Bluetooth_switch,Enter_Sleep,Time_Count;
extern uint8 page_Num,page_tatol,Page_Flag,MotorDriver_Ctr,key_open;
extern uint8 UI_state,key_state,Exti_lock,Key_control,Interface_Key;
extern uint8 UI_runMode,Open_time,key_state_confirm,Key_State_Update;
extern uint8 Display_Time,Cup_Count,doubleClick,Read_first,Key_record;
extern uint8 Display_Battery,Check_motor,Check_Lock,Existed_Data,Stop_Mode;
extern uint8 Cup_Exist,QRCode_received,QRCode_existed,Action_time,Quick_Second;

extern uint16 Power_Second,Power_Minute;
extern uint16 SignalSample_count,adcx,Data_Boundary,UI_WindowBlocks,QRCode_count;
extern uint16 SignalProcess_sampleBuffer[512],reagent_Strip[4];

extern uint32 Data_SN;

#endif /* MANAGEMENT_INTERFACE_GLOBAL_VARIABLE_H_ */
