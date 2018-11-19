/*
 * Font.h
 *
 *  Created on: 2018Äê2ÔÂ9ÈÕ
 *      Author: Administrator
 */
#ifndef MANAGEMENT_DISPLAYDRIVER_FONT_H_
#define MANAGEMENT_DISPLAYDRIVER_FONT_H_

/******************************************************************************/
#include "stm32f10x.h"
#include "comDef.h"

/******************************************************************************/
void DisplayDriver_Text16(unsigned int x, unsigned int y, unsigned int Color,
		 u8 *s);
void DisplayDriver_Text16_B(uint16 x, uint16 y, uint16 fc,
		uint16 bc,uint8 *s);
void DisplayDriver_Text12(unsigned int x, unsigned int y, uint16 fc,
		uint16 bc,u8 *s);
void DisplayDriver_Clear(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color);
void DisplayDriver_Clear(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color);
void DisplayDriver_DrawLine(u16 start_x, u16 start_y, u16 end_x, u16 end_y, u16 color);

#endif /* MANAGEMENT_DISPLAYDRIVER_FONT_H_ */
