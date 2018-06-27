/*
 * DisplayDriver.h
 *
 *  Created on: 2018年2月9日
 *      Author: Administrator
 */

#ifndef MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_H_
#define MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_H_
/******************************************************************************/
#include "stm32f10x.h"
#include "comDef.h"

/******************************************************************************/
#define Bank1_LCD_D    ((u32)0x60020000)    //Disp Data ADDR
#define Bank1_LCD_C    ((u32)0x60000000)	   //Disp Reg ADDR

#define LCD_OLD			0

#define Set_Rst GPIOD->BSRR = GPIO_Pin_13;
#define Clr_Rst GPIOD->BRR  = GPIO_Pin_13;

#define Lcd_Light_ON   GPIOA->BSRR = GPIO_Pin_1;
#define Lcd_Light_OFF  GPIOA->BRR  = GPIO_Pin_1;

extern uint8 Display_Time;

/******************************************************************************/
extern void DisplayDriver_Init(void);

/******************************************************************************/
//Lcd初始化及其低级控制函数
void Lcd_WR_Start(void);
void DataToWrite(u16 data);
void RCC_Configuration(void);
void Lcd_Display_Clear(void);
void DisplayDriver_Init(void);
void DisplayDriver_Init(void);
void LCD_WR_REG(u16 Index,u16 CongfigTemp);

/******************************************************************************/
//Lcd高级控制函数
u16 ssd1289_GetPoint(u16 x,u8 y);
char display_picture(char *filename);
void DrawPixel(u16 x, u16 y, u16 Color);
char Tiky_Button(char *filename,u16 x,u16 y);
void Lcd_ColorSpot(u16 xStart,u16 yStart,u16 Color);
void BlockWrite_Test(unsigned int Xstart,unsigned int Ystart);
void Lcd_ColorBox(u16 x,u16 y,u16 xLong,u16 yLong,u16 Color);
void DisplayDriver_DrawPic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);

/* 定义常见颜色 */
/******************************************************************************/
#define White          0xFFFF
#define Black          0x0000
#define Red            0x001F
#define Blue2          0x051F
#define Blue           0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define Baby_Blue      0xFCE0
#define Light_Blue     0xFE48

/*back color*/
#define Dark_Blue      0xC2E0
#define Thint_Green    0xF7F7

/* HZ Colour*/
#define Thint_Purple   0xCBBA

/*Parting line*/
#define Light_Gray     0xDC40
#define Thint_Blue     0xFD45

#define Deep_Blue      0x4460

#define BACKCOLOR_CONTENT_BACK  Baby_Blue
#define BACKCOLOR_CONTENT_BAR  Dark_Blue

#endif /* MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_H_ */
