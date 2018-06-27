/*******************************************************************************
 * File name: Common.c
 * Author: Hanson Liu
 * Mail: han_liu_zju@sina.com
 * Date: 2014.12 ~ Now
 ******************************************************************************/
#include "Common.h"

/******************************************************************************/
/* CRC */
#define CRC_INITIAL_REMAINDER        (0xFFFF)
#define CRC_FINAL_XOR_VALUE          (0x0000)
#define ERASE_PATTERN                (0xFF)
#define CRC_BIT4_MASK                (0x0F)
#define CRC_BIT4_SHIFT               (0x04)
#define CRC_BIT_WIDTH                (sizeof(uint16) * 8)

/******************************************************************************/
/* CRC CCITT16 calculation table */
const uint16 crcTable[16] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};

/******************************************************************************/
static uint16 Calc4BitsCRC(uint8 value, uint16 remainder)
{
    uint8 tableIndex;
    /* Divide the value by polynomial, via the 4bit CRC table. */
    tableIndex = ((value) & CRC_BIT4_MASK) ^ ((remainder) >> (CRC_BIT_WIDTH
    				- CRC_BIT4_SHIFT));
    remainder = (crcTable)[tableIndex] ^ (remainder << CRC_BIT4_SHIFT);
    return remainder;
}

/******************************************************************************/
uint16 Common_CalculateCRC(uint8 * message,uint32 length,
		uint16 remainder, uint16 xorMask)
{
    uint32 messageIndex;
    uint8 byteValue;

    /* don't make count down cycle! CRC will be different! */
    for(messageIndex = 0; messageIndex < length; messageIndex++)
    {
        byteValue = message[messageIndex];
        remainder = Calc4BitsCRC(byteValue>>CRC_BIT4_SHIFT, remainder);
        remainder = Calc4BitsCRC(byteValue, remainder);
    }

    /* Perform the final remainder CRC. */
    return remainder ^ xorMask;
}

/******************************************************************************/
void Common_EXTI_Init(GPIO_TypeDef* port, uint16 pin,
		uint8 portSrc, uint8 pinSrc, uint32 extiLine, EXTITrigger_TypeDef type,
		FunctionalState defaultCmd, uint8 irqCh, uint8 prePri, uint8 subPri)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(port, &GPIO_InitStructure);

	GPIO_EXTILineConfig(portSrc, pinSrc);

	EXTI_InitStructure.EXTI_Line = extiLine;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = type;
	EXTI_InitStructure.EXTI_LineCmd = defaultCmd;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_ClearITPendingBit(extiLine);

	NVIC_InitStructure.NVIC_IRQChannel = irqCh;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prePri;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
void SoftReset(void)
{
	/* Disable all the interrupts */
	__set_FAULTMASK(1);
	/* Software reset */
	NVIC_SystemReset();
}
