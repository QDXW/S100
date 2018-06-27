/*
 * SignalProcess_Sample.h
 *
 *  Created on: 2018Äê3ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_SIGNALPROCESS_SAMPLE_SIGNALPROCESS_SAMPLE_H_
#define MANAGEMENT_SIGNALPROCESS_SAMPLE_SIGNALPROCESS_SAMPLE_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
#define STRIP_SENSOR_PORT              (GPIOC)
#define STRIP_SENSOR_PIN               (GPIO_Pin_7)
#define STRIP_SENSOR_STATE()            \
	GPIO_ReadInputDataBit(STRIP_SENSOR_PORT, STRIP_SENSOR_PIN)

#define ADG1609_A0_PORT                (GPIOC)
#define ADG1609_A0_PIN                 (GPIO_Pin_3)
#define ADG1609_A1_PORT                (GPIOC)
#define ADG1609_A1_PIN                 (GPIO_Pin_2)

#define SIGNAL_INPUT_PORT              (GPIOC)
#define SIGNAL_INPUT_ANALOG_DATA       (GPIO_Pin_0)
#define SIGNAL_INPUT_BAT_VOLT          (GPIO_Pin_1)

#define SIGNAL_INPUT_PIN  \
	(SIGNAL_INPUT_ANALOG_DATA | SIGNAL_INPUT_BAT_VOLT)

#define ADC_CH_VANALOG                 (ADC_Channel_10)
#define ADC_CH_VBAT                    (ADC_Channel_11)

/******************************************************************************/
/* PC0: ANALOG_DTA
 * PC1: BAT_VOLT
 * PC2: VSRIP
 * PC3: VOFFSET */
typedef enum {
	VANALOG,
	VBAT,
	VSRIP,
	VOFFSET,
} ADC_ORDER;

typedef struct {
	uint16 Vanalog;
	uint16 Vbat;
	uint16 Vsrip;
	uint16 Voffset;
} ADC_DATA_ARRAY;

typedef enum {
	CH1,
	CH2,
	CH3,
	CH4,
} CH_ENUM;

/******************************************************************************/
/* Reference voltage: 2.5V */
#define VREF_MV  (2500)
/* STM32 ADC bits */
#define ADC_BITS (12)
/* Sample number per conversion */
#define VANALOG_SAMPLE_NUM (50)
#define VBAT_SAMPLE_NUM    (50)
#define VSRIP_SAMPLE_NUM   (50)
#define VOFFSET_SAMPLE_NUM (50)

/* Delay after start */
#define SAMPLE_START_DELAY (250)
/* Interval between each sampling */
#define SAMPLE_INTERVAL (100)

/* Maximal samples */
#define SIGNALSAMPLE_MAX_COUNT  512

/* Samples per sample */
#define SIGNALSAMPLE_SAMPLE_CNT 300

/* DMA */
#define ADC1_DR_Address    ((u32)0x4001244C)

/******************************************************************************/
#define SIGNAL_LED_PORT                (GPIOE)
#define SIGNAL_LED_PIN                 (GPIO_Pin_3)

/******************************************************************************/
#define AD84XX_CS_PORT                 (GPIOA)
#define AD84XX_CS_PIN                  (GPIO_Pin_4)
#define AD84XX_SPI_PORT                (GPIOA)
#define AD84XX_SPI_PIN                 (GPIO_Pin_5 | GPIO_Pin_7)

#define SIGNALSAMPLE_AD84XX_CS_Enable() \
	GPIO_ResetBits(AD84XX_CS_PORT, AD84XX_CS_PIN)
#define SIGNALSAMPLE_AD84XX_CS_Disable() \
	GPIO_SetBits(AD84XX_CS_PORT, AD84XX_CS_PIN)

/******************************************************************************/
#define SIGNALSAMPLE_AD84XX_DEALYREF   (100)
#define SIGNALSAMPLE_AD84XX_CH1        (0X00)
#define SIGNALSAMPLE_AD84XX_CH2        (0X01)
#define SIGNALSAMPLE_AD84XX_CH3        (0X02)

/******************************************************************************/
#define SIGNALSAMPLE_AD84XX_0K         (0)
#define SIGNALSAMPLE_AD84XX_0P625K     (16)
#define SIGNALSAMPLE_AD84XX_0P9375K    (24)
#define SIGNALSAMPLE_AD84XX_1P25K      (32)
#define SIGNALSAMPLE_AD84XX_1P875K     (48)
#define SIGNALSAMPLE_AD84XX_2P1875K    (56)
#define SIGNALSAMPLE_AD84XX_2P5K       (64)
#define SIGNALSAMPLE_AD84XX_5K         (128)
#define SIGNALSAMPLE_AD84XX_7P5K       (192)
#define SIGNALSAMPLE_AD84XX_10K        (255)

/******************************************************************************/
/* Sample count */
extern uint16 SignalSample_count;
extern uint16 SignalSample_ProcessCount;
/* Motor movement flag */
extern uint8 SignalSample_moveThenSample;
/* Sample buffer */
extern uint16 SignalProcess_sampleBuffer[SIGNALSAMPLE_MAX_COUNT];
/* Digital resistor */
extern uint8 SignalSample_resistorValue;
extern uint8 SignalSample_resistorValueStored;
extern uint8 SignalProcess_output;

/******************************************************************************/
void  SignalSample_Sample_Init(void);
vu16 SignalSample_Sample_Sampling(uint8 ch, uint16 sampleNum);
void SignalSample_Sample_Set_LED(uint8 flag);
void SignalSample_Clear_Buffer(void);
void SignalSample_Sample_AD84XX_Init(void);
void SignalSample_SetResistor(u8 channel, u8 data);
void SignalSample_Sample_Start(uint8 ch);
void SignalSample_Sample_Stop(uint8 ch);
ADC_DATA_ARRAY SignalSample_Test(void);
uint8 SignalSample_Sample_IsStripInserted(void);
vu16 SignalProcess_Collecting_Data(void);
vu16 SignalSample_Sample_Vbat(void);
void SignalSample_OutputSamples(uint16 sampleCount,uint16 *Sample_Data);
void SignalSample_Sample_Timer_Int(FunctionalState state);
void SignalSample_Sample_Timer_Init(void);
void SignalSample_Sample_Timer_Disabled(void);
void SignalSample_Sample_EnterCriticalArea(void);
void SignalSample_Sample_ExitCriticalArea(void);
void SignalSample_OutputUint16_4C1S(uint16 value);
void SignalSample_SampleStrip(void);
uint8 SignalSample_GetVbat(void);
void SignalSample_Sample_LED_Init(void);
void SignalSample_Sample_Strip_Sensor_Int(FunctionalState state);
void SignalSample_Sample_SetResistor(void);
void SignalSample_Sample_Select_Channel(CH_ENUM ch);
void SignalSample_SampleStrip_MultiCh(uint8 runCycle);
void SignalProcess_sampleBuffer_Changer(void);

#endif /* __MANAGEMENT_SIGNALPROCESS_SIGNALPROCESS_SAMPLE_H_ */
