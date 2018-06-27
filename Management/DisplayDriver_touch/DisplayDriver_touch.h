/*******************************************************************************
 * File name: DisplayDriver.h
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#ifndef __MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_H_
#define __MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_H_

/******************************************************************************/
#include "main.h"
#include "comDef.h"

/******************************************************************************/
//#define HONGRUIBO_NEW_GLASS

#define DISPLAY_CS_PORT  (GPIOD)
#define DISPLAY_CS_PIN   (GPIO_Pin_6)
#define DISPLAY_RST_PORT (GPIOD)
#define DISPLAY_RST_PIN  (GPIO_Pin_7)
#define DISPLAY_BL_PORT  (GPIOD)
#define DISPLAY_BL_PIN   (GPIO_Pin_2)

/******************************************************************************/
#define X_PIXELS         (240)
#define POS_X_MIN        (0)
#define POS_X_MAX        (X_PIXELS - 1)

#define Y_PIXELS         (320)
#define POS_Y_MIN        (0)
#define POS_Y_MAX        (Y_PIXELS - 1)

/******************************************************************************/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色

/******************************************************************************/
#define LCD_BASE        ((u32)(0x60000000 | 0x0001FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

#define LCD_WR_REG(regval) 		{LCD->LCD_REG=regval;}
#define LCD_WR_DATA(data)  		{LCD->LCD_RAM=data;}
#define WriteCommand(regval)	LCD_WR_REG(regval)
#define WriteData(data)     	LCD_WR_DATA(data)

#define L2R_U2D  0
#define L2R_D2U  1
#define R2L_U2D  2
#define R2L_D2U  3
#define U2D_L2R  4
#define U2D_R2L  5
#define D2U_L2R  6
#define D2U_R2L  7

#ifdef HONGRUIBO_NEW_GLASS
#define DFT_SCAN_DIR  R2L_U2D
#else
#define DFT_SCAN_DIR  L2R_U2D
#endif

/******************************************************************************/
typedef struct
{
	u16  width;
	u16  height;
	u16  id;
	u8   dir;
	u16	 wramcmd;
	u16  setxcmd;
	u16  setycmd;
}_lcd_dev;
extern _lcd_dev lcddev;

typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;

/******************************************************************************/
void DisplayDriver_Init_Touch(void);
void DisplayDriver_Control_BL(uint8 state);
void RA8875_WAITSTATUS(void);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue);

#endif /* __MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_H_ */
