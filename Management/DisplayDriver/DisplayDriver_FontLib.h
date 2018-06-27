/*
 * DisplayDriver_FontLib.h
 *
 *  Created on: 2018Äê2ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_FONTLIB_H_
#define MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_FONTLIB_H_

/******************************************************************************/
#include "stm32f10x.h"

/******************************************************************************/
#define HZ16_NUM   500
#define HZ24_NUM   100
#define HZ32_NUM   100
#define HZ40_NUM   100
#define HZ48_NUM   20

/******************************************************************************/
struct __ASC_ZK
{
  u8 code;
  u8 ENCODE[16];
};

struct __HZK_ZK
{
  u16 code;
  u8  HZCODE[32];
};

struct typFNT_GB162
{
	u8 Index[2];
  char Msk[32];
};

struct typFNT_GB242
{
	u8 Index[2];
  char Msk[72]; /* 3*24 */
};

struct typFNT_GB322
{
	u8 Index[2];
  char Msk[128]; /* 4*32 */
};

struct typFNT_GB402
{
	u8 Index[2];
  char Msk[200];
};

struct typFNT_GB482
{
	u8 Index[2];
  char Msk[288]; /* 6*48 */
};

/******************************************************************************/
extern const struct typFNT_GB162 hz16[];
extern const struct typFNT_GB242 hz24[];
extern const struct typFNT_GB322 hz32[];
extern const struct typFNT_GB482 hz48[];
extern const unsigned char asc16[];

extern const unsigned char NUM24x48[];
extern const unsigned char NUM16x48[];
extern const unsigned char NUM16x24[];
extern const unsigned char NUM32x48[];
extern const unsigned char NUM48x72[];
extern const unsigned char NUM16x72[];
extern const unsigned char NUM16x24_New[];

#endif /* MANAGEMENT_DISPLAYDRIVER_DISPLAYDRIVER_FONTLIB_H_ */
