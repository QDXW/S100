/*
 * PIC_Interface.h
 *
 *  Created on: 2018Äê2ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_DISPLAYDRIVER_PIC_INTERFACE_H_
#define MANAGEMENT_DISPLAYDRIVER_PIC_INTERFACE_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#if PROTZEK_ICO
extern const unsigned char gImage_START[16400];
#endif

#if REALY_ICO
extern const unsigned char gImage_Realy_ico[15872];
#endif

#if HENGRUI_ICO
extern const unsigned char gImage_HENGRUI_ICO[14400];
#endif

#if ULTIMED
extern const unsigned char gImage_ULTIMED[24200];
#endif


/******************************************************************************/
extern const unsigned char gImage_Blutooth[252];
extern const unsigned char gImage_All_Step[2640];
extern const unsigned char gImage_PIC_Quick[4050];
extern const unsigned char gImage_LED_Switch[450];
extern const unsigned char gImage_Empty_Step[2640];
extern const unsigned char gImage_Left_arrow[1050];
extern const unsigned char gImage_PIC_Record[4050];
extern const unsigned char gImage_PIC_Setting[4050];
extern const unsigned char gImage_Right_arrow[1050];
extern const unsigned char gImage_Calibration[4050];
extern const unsigned char gImage_PIC_Standard[4050];
extern const unsigned char gImage_Blutooth_Open[4050];
extern const unsigned char gImage_Blutooth_Close[4050];
extern const unsigned char gImage_PIC_System_Time[4050];
extern const unsigned char gImage_statusbar_charging[374];
extern const unsigned char gImage_PIC_About_Machine[4050];

#endif /* MANAGEMENT_DISPLAYDRIVER_PIC_INTERFACE_H_ */
