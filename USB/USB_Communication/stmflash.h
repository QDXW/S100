/*******************************************************************************
 * File name: STMFlash.h
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#ifndef __MANAGEMENT_STMFLASH_STMFLASH_H_
#define __MANAGEMENT_STMFLASH_STMFLASH_H_

/******************************************************************************/
#include "main.h"
#include "comDef.h"

#define DEVICE_AREA_SIZE           (50)
#define FLASH_CALI_STATUS_ADDR     (0X08000000 + 0x80000)

/******************************************************************************/
extern void STMFlash_Write(uint32 WriteAddr, uint16 *pBuffer, uint16 NumToWrite);
extern void STMFlash_Read(uint32 ReadAddr, uint16 *pBuffer, uint16 NumToRead);

#endif /* __MANAGEMENT_STMFLASH_STMFLASH_H_ */
