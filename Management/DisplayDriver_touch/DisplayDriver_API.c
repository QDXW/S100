/*******************************************************************************
 * File name: DisplayDriver_API.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#include "DisplayDriver.h"
#include "DisplayDriver_API.h"
#include "DisplayDriver_FontLib.h"

/******************************************************************************/
#define LCDBUFSIZE	800*480
#define BACK_COLOR WHITE

/*Index of 24X48 dot*/
#define DOT_24X48 12

#define GRAM_COLOR 0X2C
#define HORSTARTREG  0X50
#define HORENDREG    0X51
#define VERSTARTREG  0X52
#define VERENDREG    0X53
#define XPOSTION     0X20
#define YPOSTION     0X21
#define PANNELLENGTH 239
#define PANNELHEIGHT 319

/******************************************************************************/
uint16 fontbuf[256];

/******************************************************************************/
u16 DisplayDriver_SwapBlueRed(u16 c)
{
	return c;
}

/******************************************************************************/
void DisplayDriver_SetCursor(u16 x, u16 y)
{
 	if(lcddev.id==0X9341 || lcddev.id==0X7789)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(x>>8);
		LCD_WR_DATA(x&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(y>>8);
		LCD_WR_DATA(y&0XFF);
	}
 	else
	{
		if(lcddev.dir==1)
			x=lcddev.width-1-x;
		LCD_WriteReg(lcddev.setxcmd, x);
		LCD_WriteReg(lcddev.setycmd, y);
	}
}


/******************************************************************************/
void DisplayDriver_Clear_Touch(uint16 color)
{	
	register uint32 size;
	DisplayDriver_SetCursor(0, 0);

    WriteCommand(GRAM_COLOR);
    size = LCDBUFSIZE;
    while(size--)
    WriteData(color);
}

/******************************************************************************/
void DisplayDriver_DrawBmp(const uint8* bmpbit,uint16 x, uint16 y)
{
	u16 indexWidth, indexHeight;
	u16 width = 0;
	u16 height = 0;
	u16 temp;

	width = ((uint16)(bmpbit[3] << 8) + bmpbit[2]);
	height = ((uint16)(bmpbit[5] << 8) + bmpbit[4]);

	/* Row by row */
	for (indexHeight = 0; indexHeight < height; indexHeight++)
	{
		DisplayDriver_SetCursor(x, y + indexHeight);
		LCD_WriteRAM_Prepare();
		/* Write one row data */
		indexWidth =0;
		while (indexWidth < width)
		{
			temp = ((uint16)((bmpbit[9 + indexHeight * ( 2 * width) + 2 * indexWidth] << 8)
					+ bmpbit[8 + indexHeight * (2 * width) + 2 * indexWidth]));
			WriteData(temp);
			indexWidth++;
		}
	}
}

/******************************************************************************/
void DisplayDriver_TextAny(unsigned short x, unsigned short y, unsigned char w,
		unsigned char h, unsigned short fc, unsigned short bc,
		const unsigned char *s, unsigned short addr)
{
  unsigned char i,j,k,c;
  w=w/8;

  for(i=0;i<h;i++)
      for(j=0;j<w;j++)
              {
              c=*(s+addr*w*h+i*w+j);
              for (k=0;k<8;k++)	{

              if(c&(0x80>>k))
                      DisplayDriver_DrawPoint(x+j*8+k,y+i,fc);
                      else {
                              if (fc!=bc)
                               DisplayDriver_DrawPoint(x+j*8+k,y+i,bc);
                      }
              }
      }
}

/******************************************************************************/
void DisplayDriver_Text16_Any(unsigned short startX, unsigned short startY,
		uint8 oneLineNum, unsigned short fc, unsigned short bc, uint8 *s)
{
	unsigned char i,j;
	unsigned short k, x, y;
	uint8 num = 0;
	uint8 lines = 0;

	x = startX;
	y = startY;

	while(*s)
	{
		if ((*(s - 1)=='.') && (*(s)=='-'))
		{
			s++;
			num = 0;
			lines++;
			x = startX;
			y = startY + lines*18;
		}
		else if (x > 210)
		{
			num = 0;
			lines++;
			x = startX;
			y = startY + lines*18;
		}

		if( *s < 0x80 )
		{
			k=*s;
			if (k>32)
				k-=32;
			else
				k=0;

			for(i=0;i<16;i++)
				for(j=0;j<8;j++)
				{
					if(asc16[k*16+i]&(0x80>>j))
						DisplayDriver_DrawPoint(x+j,y+i,fc);
					else
					{
						if (fc!=bc)
							DisplayDriver_DrawPoint(x+j,y+i,bc);
					}
				}
			s++;x+=8;
		}
	}
}

/******************************************************************************/
void DisplayDriver_Text24(unsigned short x, unsigned short y, unsigned short fc,
		unsigned short bc, unsigned char *s)
{
  unsigned char i,j;
  unsigned short k;

  while(*s)
  {
    if( *s < 0x80 )
    {
            k=*s;
            if(k>32)
              k-=32;
            else
              k=0;

        for(i=0;i<16;i++)
            for(j=0;j<8;j++)
                    {
                    if(asc16[k*16+i]&(0x80>>j))
                            DisplayDriver_DrawPoint(x+j,y+i,fc);
                            else {
                                    if (fc!=bc)
                                    DisplayDriver_DrawPoint(x+j,y+i,bc);
                            }
                    }
            s++;
            x+=8;
    }
   }
}

/******************************************************************************/
void DisplayDriver_Text32(unsigned short x, unsigned short y, unsigned short fc,
		unsigned short bc, unsigned char *s)
{
  unsigned char i,j;
  unsigned short k;

  while(*s)
  {
    if( *s < 0x80 )
    {
            k=*s;
            if(k>32)
              k-=32;
            else
              k=0;

        for(i=0;i<16;i++)
            for(j=0;j<8;j++)
                    {
                    if(asc16[k*16+i]&(0x80>>j))
                            DisplayDriver_DrawPoint(x+j,y+i,fc);
                            else {
                                    if (fc!=bc)
                                    DisplayDriver_DrawPoint(x+j,y+i,bc);
                            }
                    }
            s++;
            x+=8;
    }
   }
}

/******************************************************************************/
void DisplayDriver_Text48(unsigned short x, unsigned short y, unsigned short fc,
		unsigned short bc, unsigned char *s)
{
  unsigned char i,j;
  unsigned short k;

  while(*s)
  {
    if( *s < 0x80 )
    {
            k=*s;
            if(k>32)
              k-=32;
            else
              k=0;

        for(i=0;i<16;i++)
            for(j=0;j<8;j++)
                    {
                    if(asc16[k*16+i]&(0x80>>j))
                            DisplayDriver_DrawPoint(x+j,y+i,fc);
                            else {
                                    if (fc!=bc)
                                    DisplayDriver_DrawPoint(x+j,y+i,bc);
                            }
                    }
            s++;
            x+=8;
    }
   }
}

/******************************************************************************/
void DisplayDriver_DrawPoint_Big(u16 x,u16 y, u16 color)
{
	DisplayDriver_DrawPoint(x - 1, y - 1, color);
	DisplayDriver_DrawPoint(x, y - 1, color);
	DisplayDriver_DrawPoint(x + 1, y - 1, color);

	DisplayDriver_DrawPoint(x - 1, y, color);
	DisplayDriver_DrawPoint(x, y, color);
	DisplayDriver_DrawPoint(x + 1, y, color);

	DisplayDriver_DrawPoint(x - 1, y + 1, color);
	DisplayDriver_DrawPoint(x, y + 1, color);
	DisplayDriver_DrawPoint(x + 1, y + 1, color);
}

/******************************************************************************/
void DisplayDriver_DrawPoint(u16 x,u16 y, u16 color)
{
	DisplayDriver_SetCursor(x,y);
	LCD_WriteRAM_Prepare();
	WriteData(color);
}

/******************************************************************************/
void DisplayDriver_Fill(u16 start_x,u16 start_y,u16 end_x,u16 end_y,u16 color)
{
	u16 i,j;
	u16 xlen=0;

	xlen=end_x-start_x+1;
	for(i=start_y;i<=end_y;i++)
	{
		DisplayDriver_SetCursor(start_x,i);
		LCD_WriteRAM_Prepare();
		for(j=0;j<xlen;j++)
		{
			LCD->LCD_RAM=color;
		}
	}
}

/******************************************************************************/
void DisplayDriver_DrawRectangle(u16 start_x,u16 start_y,u16 end_x,u16 end_y, u16 color)
{
	DisplayDriver_DrawLine(start_x,start_y,end_x,start_y, color);
	DisplayDriver_DrawLine(start_x,start_y,start_x,end_y, color);
	DisplayDriver_DrawLine(start_x,end_y,end_x,end_y, color);
	DisplayDriver_DrawLine(end_x,start_y,end_x,end_y, color);
}

/******************************************************************************/
void DisplayDriver_DrawCircle(u16 x_postion,u16 y_postion,u8 radius, u16 color)
{
	int a,b;
	int di;
	a = 0;
	b = radius;
	di = 3 - (radius << 1);
	while(a<=b)
	{
		DisplayDriver_DrawPoint(x_postion+a,y_postion-b, color);             //5
 		DisplayDriver_DrawPoint(x_postion+b,y_postion-a, color);             //0
		DisplayDriver_DrawPoint(x_postion+b,y_postion+a, color);             //4
		DisplayDriver_DrawPoint(x_postion+a,y_postion+b, color);             //6
		DisplayDriver_DrawPoint(x_postion-a,y_postion+b, color);             //1
 		DisplayDriver_DrawPoint(x_postion-b,y_postion+a, color);
		DisplayDriver_DrawPoint(x_postion-a,y_postion-b, color);             //2
		DisplayDriver_DrawPoint(x_postion-b,y_postion-a, color);             //7
		a++;

		if(di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
	}
}
