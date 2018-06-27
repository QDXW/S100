/*
 * Storage_Flash.h
 *
 *  Created on: 2018Äê4ÔÂ18ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_STORAGE_FLASH_STORAGE_FLASH_H_
#define MANAGEMENT_STORAGE_FLASH_STORAGE_FLASH_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOB_ODR_Addr    (GPIOB_BASE + 12) 						//0x40010C0C
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)

#define	W25QXX_CS 		        PBout(12)
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg		0x05
#define W25X_WriteStatusReg		0x01
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F
#define ITEM_MAX_ONEPAGE_EXTENSION1 10

/******************************************************************************/
extern uint16 reagent_Strip[4];

/******************************************************************************/
typedef struct {
	uint16 year;
	uint8 month;
	uint8 day;
	uint8 hour;
	uint8 min;
} STORAGE_TIME;

/******************************************************************************/
typedef struct {
	uint16 status;
	uint16 days;
	uint32 newIndexOffset;
	uint32 newDataOffset;
} STORAGE_HEAD_STRUCT;

/******************************************************************************/
typedef struct {
    uint8 TName[8];
    float threshold1;
    float threshold2;
    float threshold3;
    uint8 Result[6];
} STORAGE_SINGLE_LINE;

/******************************************************************************/
typedef struct {
	uint8 Product_name[20];
	uint8 Product_SN[10];
	uint8 StripNum;
	STORAGE_TIME Product_Time;
	STORAGE_SINGLE_LINE CH_data[12];
} STORAGE_SINGLE_DATA_STRUCT;

/******************************************************************************/
extern STORAGE_SINGLE_DATA_STRUCT Storage_Data;

/******************************************************************************/
extern void Storage_Flash_Init(void);
extern void Storage_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
extern void Storage_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
extern void Storage_Time(void);
void Get_reagent_TestNum(void);
void Storage_Clear(void);
extern uint8 Storage_Record(void);
extern uint8 Read_Record(void);

#endif /* MANAGEMENT_STORAGE_FLASH_STORAGE_FLASH_H_ */
