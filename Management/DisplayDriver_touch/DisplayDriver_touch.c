/*******************************************************************************
 * File name: DisplayDriver.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#include "Font.h"
#include "fsmc_sram.h"
#include "DisplayDriver_touch.h"
#include "DisplayDriver_FontLib.h"

/******************************************************************************/
_lcd_dev lcddev;

/******************************************************************************/
u16 LCD_RD_DATA(void)
{
	vu16 ram;
	ram=LCD->LCD_RAM;	
	return ram;	 
}	

/******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;
	LCD->LCD_RAM = LCD_RegValue;
}

/******************************************************************************/
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);
	return LCD_RD_DATA();
}   

/******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}

/******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;
}

/******************************************************************************/
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;

	if(x>=lcddev.width || y>=lcddev.height)
		return 0;

	DisplayDriver_SetCursor(x,y);

	if(lcddev.id==0X9341 || lcddev.id==0X7789)
	{
		LCD_WR_REG(0X2E);
	}
	else
	{
		LCD_WR_REG(0X22);
	}

 	r=LCD_RD_DATA();

	Delay_ms_SW(10);

 	r=LCD_RD_DATA();

 	if(lcddev.id==0X9341 || lcddev.id==0X7789)
 	{
		Delay_ms_SW(10);
		b=LCD_RD_DATA(); 
		g=r&0XFF;
		g<<=8;
	} 

	if(lcddev.id==0X9341 || lcddev.id==0X7789)
		return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
	else
		return DisplayDriver_SwapBlueRed(r);
}			 

/******************************************************************************/
void LCD_DisplayOn(void)
{					   
	if(lcddev.id==0X9341 || lcddev.id==0X7789)
	{
		LCD_WR_REG(0X29);
	}
	else
	{
		LCD_WriteReg(0X07,0x0173);
	}
}	 

/******************************************************************************/
void LCD_DisplayOff(void)
{	   
	if(lcddev.id==0X9341 || lcddev.id==0X7789)
	{
		LCD_WR_REG(0X28);
	}
	else
	{
		LCD_WriteReg(0X07,0x0);
	}
}   

/******************************************************************************/
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if((lcddev.dir==1&&lcddev.id!=0X6804&&lcddev.id!=0X1963)
			||(lcddev.dir==0&&lcddev.id==0X1963))
	{			   
		switch(dir)
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 

	if(lcddev.id==0x9341||lcddev.id==0X7789)
	{
		switch(dir)
		{
			case L2R_U2D:
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U:
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D:
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U:
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R:
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L:
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R:
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L:
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}

		/* Difference between 9341 and 7789, 7789 needs to bypass */
		if(lcddev.id==0x9341)
		{
			dirreg=0X36;
			regval|=0X08;
			LCD_WriteReg(dirreg,regval);
		}

		if(regval&0X20)
		{
			if(lcddev.width<lcddev.height)
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
			}
		}else
		{
			if(lcddev.width>lcddev.height)
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
			}
		}

		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(0);
		LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);
		LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(0);
		LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);
		LCD_WR_DATA((lcddev.height-1)&0XFF);
  	}
}     

/******************************************************************************/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)
	{	/* Vertical */
		lcddev.dir=0;
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341 || lcddev.id == 0x7789)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}else
		{
			/* 0x9325 */
			lcddev.wramcmd=0X22;
	 		lcddev.setxcmd=0X20;
			lcddev.setycmd=0X21;
		}
	}else
	{	/* Horizontal */
		lcddev.dir=1;
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341 || lcddev.id == 0x7789)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}else
		{
			/* 0x9325 */
			lcddev.wramcmd=0X22;
	 		lcddev.setxcmd=0X21;
			lcddev.setycmd=0X20;  
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);
}

/****************************************************************************/
void Driver_Init_9341(void)
{
#ifdef HONGRUIBO_NEW_GLASS
	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xAA);
	LCD_WR_DATA(0XE0);

	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x67);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0X12);
	LCD_WR_DATA(0X81);

	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x8A);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x78);

	LCD_WR_REG(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);

	LCD_WR_REG(0xF7);
	LCD_WR_DATA(0x20);

	LCD_WR_REG(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xC0); 						//Power control
	LCD_WR_DATA(0x23); 						//VRH[5:0]
	LCD_WR_REG(0xC1); 						//Power control
	LCD_WR_DATA(0x11); 						//SAP[2:0];BT[3:0]

	LCD_WR_REG(0xC5); 						//VCM control
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x4c);

	LCD_WR_REG(0xC7); 						//VCM control2
	LCD_WR_DATA(0xA0);

	LCD_WR_REG(0x36); 						// Memory Access Control
	LCD_WR_DATA(0x48);
	LCD_WR_REG(0x3A); 						// Memory Access Control
	LCD_WR_DATA(0x05);
	LCD_WR_REG(0xB6); 						//Set Gamma
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x02);
	LCD_WR_REG(0xF2); 						// 3Gamma Function Disable
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x26); 						//Gamma curve selected
	LCD_WR_DATA(0x01);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA( 0x1f);						//p1  //0f
	LCD_WR_DATA( 0x36);						//p2  //30
	LCD_WR_DATA( 0x36);						//p3
	LCD_WR_DATA( 0x3A);						//p4
	LCD_WR_DATA( 0x0C);						//p5
	LCD_WR_DATA( 0x05);						//p6
	LCD_WR_DATA( 0x4F);						//p7
	LCD_WR_DATA( 0x87);						//p8
	LCD_WR_DATA( 0x3C);						//p9
	LCD_WR_DATA( 0x08);						//p10
	LCD_WR_DATA( 0x11);						//p11
	LCD_WR_DATA( 0x35);						//p12
	LCD_WR_DATA( 0x19);						//p13
	LCD_WR_DATA( 0x13);						//p14
	LCD_WR_DATA( 0x00);						//p15

	LCD_WR_REG(0xE1);
	LCD_WR_DATA( 0x00);						//p1
	LCD_WR_DATA( 0x09);						//p2
	LCD_WR_DATA( 0x09);						//p3
	LCD_WR_DATA( 0x05);						//p4
	LCD_WR_DATA( 0x13);						//p5
	LCD_WR_DATA( 0x0A);						//p6
	LCD_WR_DATA( 0x30);						//p7
	LCD_WR_DATA( 0x78);						//p8
	LCD_WR_DATA( 0x43);						//p9
	LCD_WR_DATA( 0x07);						//p10
	LCD_WR_DATA( 0x0E);						//p11
	LCD_WR_DATA( 0x0A);						//p12
	LCD_WR_DATA( 0x26);						//p13
	LCD_WR_DATA( 0x2C);						//p14
	LCD_WR_DATA( 0x1f);						//p15

	LCD_WR_REG(0x11); 						//Exit Sleep
	Delay_ms_SW(120);
	LCD_WR_REG(0x21);
	LCD_WR_REG(0x29);						//Display on
#else
	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC1);
	LCD_WR_DATA(0X30);

	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0X12);
	LCD_WR_DATA(0X81);

	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x85);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x78);

	LCD_WR_REG(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);

	LCD_WR_REG(0xF7);
	LCD_WR_DATA(0x20);

	LCD_WR_REG(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC0);    					//Power control
	LCD_WR_DATA(0x21);  					//VRH[5:0]

	LCD_WR_REG(0xC1);    					//Power control
	LCD_WR_DATA(0x12);   					//SAP[2:0];BT[3:0]

	LCD_WR_REG(0xC5);    					//VCM control
	LCD_WR_DATA(0x4a); 	 					//3F
	LCD_WR_DATA(0x3d); 	 					//3C

	LCD_WR_REG(0xC7);    					//VCM control2
	LCD_WR_DATA(0X83);

	LCD_WR_REG(0x36);    					// Memory Access Control
	LCD_WR_DATA(0x08);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);

	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x15);

	LCD_WR_REG(0xB6);    					// Display Function Control
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0xA2);

	LCD_WR_REG(0xF2);    					// 3Gamma Function Disable
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xF6);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x30);

	LCD_WR_REG(0x26);    					//Gamma curve selected
	LCD_WR_DATA(0x01);

	LCD_WR_REG(0xE0);    					//Set Gamma
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x1E);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0X6D);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0XE1);    					//Set Gamma
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x30);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x0F);

	LCD_WR_REG(0x2A); 						//Frame rate control
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);

	LCD_WR_REG(0x2B); 						//Display function control
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);

	LCD_WR_REG(0x11); 						//Exit Sleep
	Delay_ms_SW(200);
	LCD_WR_REG(0x29); 						//display on
	LCD_WR_REG(0x2c);
#endif
}

/******************************************************************************/
void Driver_Init_7789(void)
{
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x05);

	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x00);
	/* ST7789S Frame rate setting */
	LCD_WR_REG(0xb2);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x33);

	LCD_WR_REG(0xb7);
	LCD_WR_DATA(0x35);
	/* ST7789S Power setting */
	LCD_WR_REG(0xbb);
	LCD_WR_DATA(0x3F);//vcom

	LCD_WR_REG(0xc0);
	LCD_WR_DATA(0x2C);

	LCD_WR_REG(0xc2);
	LCD_WR_DATA(0x01);

	LCD_WR_REG(0xc3);
	LCD_WR_DATA(0x0F);

	LCD_WR_REG(0xc4);
	LCD_WR_DATA(0x20);

	LCD_WR_REG(0xc6);
	LCD_WR_DATA(0x11);

	LCD_WR_REG(0xd0);
	LCD_WR_DATA(0xa4);
	LCD_WR_DATA(0xa1);

	LCD_WR_REG(0xe8);
	LCD_WR_DATA(0x03);

	LCD_WR_REG(0xe9);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x08);
	/* ST7789S gamma setting */
	LCD_WR_REG(0xe0);
	LCD_WR_DATA(0xd0);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x30);

	LCD_WR_REG(0xe1);
	LCD_WR_DATA(0xd0);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x3B);
	LCD_WR_DATA(0x38);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x2F);

	LCD_WR_REG(0x21);

	/* Frame rate control */
	LCD_WR_REG(0x2A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xEF);

	/* Display function control */
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3F);

	/* Exit Sleep */
	LCD_WR_REG(0x11);
	Delay_ms_SW(200);
	/* Display on */
	LCD_WR_REG(0x29);
	LCD_WR_REG(0x2c);
}

/******************************************************************************/
void DisplayDriver_Control_BL(uint8 state)
{
	if (state == NORMAL_ON)
		GPIO_SetBits(DISPLAY_BL_PORT, DISPLAY_BL_PIN); /* ON */
	else
		GPIO_ResetBits(DISPLAY_BL_PORT, DISPLAY_BL_PIN); /* OFF */
}

/******************************************************************************/
void DisplayDriver_Reset(void)
{
	GPIO_SetBits(DISPLAY_RST_PORT, DISPLAY_RST_PIN);
	Delay_ms_SW(10);
	GPIO_ResetBits(DISPLAY_RST_PORT, DISPLAY_RST_PIN);
	Delay_ms_SW(100);
	GPIO_SetBits(DISPLAY_RST_PORT, DISPLAY_RST_PIN);
	Delay_ms_SW(100);
}

/******************************************************************************/
void DisplayDriver_Init_Touch(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming;
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

	
	/* CS */
	GPIO_InitStructure.GPIO_Pin = DISPLAY_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DISPLAY_CS_PORT, &GPIO_InitStructure);

	/* Reset */
	GPIO_InitStructure.GPIO_Pin = DISPLAY_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DISPLAY_RST_PORT, &GPIO_InitStructure);
	
	/* PortD */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5
			|GPIO_Pin_11|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14
			|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* PortE */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10
			|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Backlight */
	GPIO_InitStructure.GPIO_Pin = DISPLAY_BL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DISPLAY_BL_PORT, &GPIO_InitStructure);

	/* FSMC data */
	readWriteTiming.FSMC_AddressSetupTime = 0x01;
	readWriteTiming.FSMC_AddressHoldTime = 0x00;
	readWriteTiming.FSMC_DataSetupTime = 0x0f;
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;

	writeTiming.FSMC_AddressSetupTime = 0x00;
	writeTiming.FSMC_AddressHoldTime = 0x00;
	writeTiming.FSMC_DataSetupTime = 0x03;
	writeTiming.FSMC_BusTurnAroundDuration = 0x00;
	writeTiming.FSMC_CLKDivision = 0x00;
	writeTiming.FSMC_DataLatency = 0x00;
	writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
	
	/* Reset display controller */
	DisplayDriver_Reset();

	lcddev.id=LCD_ReadReg(0x0000);

  	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)
	{	
		LCD_WR_REG(0XD3);				   
		lcddev.id=LCD_RD_DATA();
 		lcddev.id=LCD_RD_DATA();
  		lcddev.id=LCD_RD_DATA();
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();
	}

	if(lcddev.id==0X9341)
	{
		Driver_Init_9341();
	}
	else
	{
		Driver_Init_7789();

		lcddev.id = 0X7789;
	}

	LCD_Display_Dir(0); /* Vertical */

	/* Fill color */
	DisplayDriver_Clear_Touch(White);
}
/******************************************************************************/
