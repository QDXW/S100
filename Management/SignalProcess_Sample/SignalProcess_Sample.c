/*
 * SignalProcess_Sample.c
 *
 *  Created on: 2018Äê3ÔÂ13ÈÕ
 *      Author: Administrator
 */
#define ADC_TIMER_PRESCALER (7199)
#define ADC_TIMER_PERIOD (60)

/******************************************************************************/
#include "SignalProcess_Sample.h"
#include "Signalprocess.h"

/******************************************************************************/
uint8 SignalSample_moveThenSample = 0;
uint8 SignalSample_endDetection = 0;

/* Number of samples */
uint16 SignalSample_ProcessCount = 0;
uint8 SignalSample_startSampling = 0;	 		//always sample

/* Original data */
uint16 SignalProcess_sampleBuffer_BK[SIGNALSAMPLE_MAX_COUNT] = {0};
uint8 SignalProcess_outputBuffer[SIGNALSAMPLE_MAX_COUNT] = {0};

uint8 SignalSample_resistorValue = 0;   		//MAX = 14
uint8 SignalSample_resistorValueStored = 0;

uint8 SignalProcess_output = 0;

/******************************************************************************/
void SignalSample_Sample_Strip_Sensor_Init(void);
void SignalSample_Sample_ADC_Init(void);
void SignalSample_Sample_SetResistor(void);

/******************************************************************************/
void SignalSample_Sample_Init(void)
{
	/* Initialize signal LED */
	SignalSample_Sample_LED_Init();

	/* Initialize AD84XX */
	SignalSample_Sample_AD84XX_Init();

	/* Set resistor */
	SignalSample_Sample_SetResistor();

	SignalSample_Sample_Timer_Init();

#if RL_A3000
	/* Initialize ADG1609 */
	SignalSample_Sample_ADG1609_Init();
#endif

	/* Initialize ADC */
	SignalSample_Sample_ADC_Init();
}

/******************************************************************************/
void SignalSample_Sample_SetResistor(void)
{
	SignalSample_SetResistor(SIGNALSAMPLE_AD84XX_CH1,
			SignalSample_resistorValue);
}

/******************************************************************************/
void SignalSample_Sample_Strip_Sensor_Init(void)
{
	Common_EXTI_Init(STRIP_SENSOR_PORT, STRIP_SENSOR_PIN,
			GPIO_PortSourceGPIOC, GPIO_PinSource7, EXTI_Line7, EXTI_Trigger_Rising_Falling,
			DISABLE, EXTI9_5_IRQn, 0X01, 0X06);
}

/******************************************************************************/
void SignalSample_Sample_Strip_Sensor_Int(FunctionalState state)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = state;
	EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************/
void SignalSample_Sample_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

    /* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);

	/* Configure PC0/1/2/3/4/5 as analog input */
	GPIO_InitStructure.GPIO_Pin = SIGNAL_INPUT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(SIGNAL_INPUT_PORT, &GPIO_InitStructure);

	/* Configure ADC */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;//SIGNALSAMPLE_CHANNEL_NUM;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular configuration */
	ADC_RegularChannelConfig(ADC1, ADC_CH_VANALOG, 1, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);

	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	/* Disable ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
}

/******************************************************************************/
void SignalSample_Sample_Start(uint8 ch)
{
	/* Configure ADC channel per input */
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_55Cycles5);
}

/******************************************************************************/
void SignalSample_Sample_Stop(uint8 ch)
{
	/* Disable ADC1 */
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
}

/******************************************************************************/
vu16 SignalSample_Sample_Sampling(uint8 ch, uint16 sampleNum)
{
#define ALGORITHM_AVERAGE 1
#define ALGORITHM_MID_AVERAGE 0
	vu32 sampleDataSum = 0;
	vu16 sampleDataAvr = 0;
	vu16 result = 0;
	uint32 index;

	/* Start ADC */
	SignalSample_Sample_Start(ch);

#if ALGORITHM_AVERAGE
	for (index = 0; index < sampleNum; index++)
	{
		/* Start conversion */
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		/* Wait for completion */
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
		/* Get sum */
		sampleDataSum += ADC_GetConversionValue(ADC1);
	}

	/* Get average value */
	sampleDataAvr = sampleDataSum / sampleNum;

#elif ALGORITHM_MID_AVERAGE

#endif

	/* Convert to voltage (unit is mV) */
	result = (sampleDataAvr * VREF_MV) >> ADC_BITS;

	/* Stop ADC */
	SignalSample_Sample_Stop(ch);

	return result;
}

/******************************************************************************/
void SignalSample_Sample_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SIGNAL_LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SIGNAL_LED_PORT, &GPIO_InitStructure);

	/* Turn off LED */
	SignalSample_Sample_Set_LED(ABNORMAL_OFF);
}

/******************************************************************************/
void SignalSample_Sample_Set_LED(u8 flag)
{
	if(flag)
	{
		GPIO_SetBits(SIGNAL_LED_PORT, SIGNAL_LED_PIN);
	}
	else
	{
		GPIO_ResetBits(SIGNAL_LED_PORT, SIGNAL_LED_PIN);
	}
}

/******************************************************************************/
void SignalSample_Clear_Buffer(void)
{
	uint16 count = 0;

	for (count = 0; count < SIGNALSAMPLE_MAX_COUNT; count++)
	{
		SignalProcess_sampleBuffer[count] = 0;
	}
}

/******************************************************************************/
void SignalSample_Sample_AD84XX_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* CS */
	GPIO_InitStructure.GPIO_Pin = AD84XX_CS_PIN;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(AD84XX_CS_PORT, &GPIO_InitStructure);

	/* SPI1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
	/* SCK, MISO, MOSI */
	GPIO_InitStructure.GPIO_Pin = AD84XX_SPI_PIN;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(AD84XX_SPI_PORT, &GPIO_InitStructure);

	/* SPI1 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* SPI1 */
	SPI_Cmd(SPI1, ENABLE);

	/* Select */
	SIGNALSAMPLE_AD84XX_CS_Disable();
}

/******************************************************************************/
void SignalSample_Sample_AD84XX_SendByte(u8 byte)
{
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI2 peripheral */
	SPI_I2S_SendData(SPI1, byte);
}

/******************************************************************************
¡Á AD84XX-A10      RAB = 10K; RW = 45¦¸
* Data format£º A1 A0 |  D7 D6 D5 D4 D3 D2 D1 D0
*      A1 A0: Channel
*      0  0    RDAC1
*      0  1    RDAC2
*      D7~D0: Data
¡Á      RWB =  RAB*(D/256)+RW
¡Á  eg:   D 255    10006
         D 128    5045
         D  1       84
         D  0       45
******************************************************************************/
void SignalSample_SetResistor(u8 channel, u8 data)
{
	SIGNALSAMPLE_AD84XX_CS_Enable();
	Delay_ms(SIGNALSAMPLE_AD84XX_DEALYREF);
	SignalSample_Sample_AD84XX_SendByte(channel);
	SignalSample_Sample_AD84XX_SendByte(data);
	Delay_ms(SIGNALSAMPLE_AD84XX_DEALYREF);
	SIGNALSAMPLE_AD84XX_CS_Disable();
}

/******************************************************************************/
uint8 SignalSample_Sample_IsStripInserted(void)
{
#if REAGENT_CARD_TRIGGER_LEVEL_REVERSED
	return STRIP_SENSOR_STATE();
#else
	return !STRIP_SENSOR_STATE();
#endif
}

/******************************************************************************/
vu16 SignalProcess_Collecting_Data(void)
{
	vu16 sample_result;

#if TIME_MEASUREMENT_ENABLED
	Alarm_Time_Pin_High(); /* 50 times time consumption: 0.16ms */
#endif

	sample_result =
			SignalSample_Sample_Sampling(ADC_CH_VANALOG, VANALOG_SAMPLE_NUM);

#if TIME_MEASUREMENT_ENABLED
	Alarm_Time_Pin_Low();
#endif
	return sample_result;
}

/******************************************************************************/
vu16 SignalSample_Sample_Vbat(void)
{
	return SignalSample_Sample_Sampling(ADC_CH_VBAT, VBAT_SAMPLE_NUM);
}

/******************************************************************************/
#define BATTERY_MIN (3.3) /* Minimal voltage */
#define BATTERY_MAX (4.2) /* Maximal voltage */

/******************************************************************************/
uint8 SignalSample_GetVbat(void)
{
/*
y = kx + b = 0.00203x - 0.0131

Battery voltage, AD converted value
4.2, 2080,
4.0, 1980,
3.8, 1880,
3.6, 1780,
3.4, 1680,
3.3, 1640, Inaccurate when below 3.3V
3.2, 1610,
3.0, 1622
*/

	/* voltage = coefA*sample + coefB */
	float batLinCoefA = 0.00203;
	float batLinCoefB = -0.0131;

	uint16 batteryHalfCali = 0;
	float batteryRealCali = 0;
	uint8 percentage = 0;

	/* Sample */
	batteryHalfCali = SignalSample_Sample_Vbat();

	/* Calibration */
	batteryRealCali = batLinCoefA * batteryHalfCali + batLinCoefB;

	if (batteryRealCali >= BATTERY_MAX)
	{
		percentage = 100;
	}
	else if (batteryRealCali <= BATTERY_MIN)
	{
		percentage = 0;
	}
	else
	{
		percentage = 100 * (batteryRealCali - BATTERY_MIN)/(BATTERY_MAX - BATTERY_MIN);
	}

#if BATTERY_DEBUG_ENABLED
	/* Output battery voltage: (X.XXX * 1000) mV */
	SignalSample_OutputUint16_4C1S((uint16)(batteryRealCali * 1000));
#endif

	return percentage;
}

/******************************************************************************/
void SignalSample_OutputUint16_4C1S(uint16 value)
{
// 	uint8 outputData[5] = {0};
// 	uint16 tmp, data;

// 	/* 4 chars and 1 space */
// 	data = value;
// 	outputData[0] = data/1000 + 0x30;
// 	tmp = data%1000;
// 	outputData[1] = tmp/100 + 0x30;
// 	tmp = data%100;
// 	outputData[2] = tmp/10 + 0x30;
// 	tmp = data%10;
// 	outputData[3] = tmp + 0x30;
//	outputData[4] = '\0';

//	HostComm_Send(HOSTCOMM_USART, outputData);
//	HostComm_Send(HOSTCOMM_USART, " ");
}

/******************************************************************************/
void SignalSample_OutputUint16_Each(uint16 value, uint8 *targetPtr)
{
	uint8 outputData[5] = {0};
	uint16 tmp, data;

	/* 4 chars and 1 space */
	data = value;
	outputData[0] = data/1000 + 0x30;
	tmp = data%1000;
	outputData[1] = tmp/100 + 0x30;
	tmp = data%100;
	outputData[2] = tmp/10 + 0x30;
	tmp = data%10;
	outputData[3] = tmp + 0x30;
	outputData[4] = ' ';

	memcpy(targetPtr, outputData, 4);
}

/******************************************************************************/
void SignalSample_OutputSamples(uint16 sampleCount, uint16 *Sample_Data)
{
	unsigned short count = 0;
	uint16 totalLength = sampleCount + 1;

	//		if (!SignalProcess_output)
	//			return;

	/* Debugging: Output samples
	* Format: data length, data[0], data[1], ..., data[data length - 1] */

	SignalProcess_sampleBuffer_BK[0] = sampleCount;
	memcpy(&SignalProcess_sampleBuffer_BK[1], Sample_Data, sampleCount << 1);

	for (count = 0; count < totalLength; count++)
	{
		SignalSample_OutputUint16_Each(SignalProcess_sampleBuffer_BK[count],
		&SignalProcess_outputBuffer[5 * count]);
	}

	HostComm_SendThrUSB(5 * count, &SignalProcess_outputBuffer[0]);
}

/******************************************************************************/
void SignalSample_Sample_Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = ADC_TIMER_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = ADC_TIMER_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3, ENABLE);

	SignalSample_Sample_Timer_Int(ENABLE);
}

/******************************************************************************/
void SignalSample_Sample_Timer_Disabled(void)
{
	SignalSample_Sample_Timer_Int(DISABLE);
	TIM_Cmd(TIM3, DISABLE);
}

/******************************************************************************/
void SignalSample_Sample_Timer_Int(FunctionalState state)
{
	TIM_ITConfig(TIM3, TIM_IT_Update, state);
}

/******************************************************************************/
void SignalSample_Sample_EnterCriticalArea(void)
{
	/* Turn on current source */
	SignalSample_Sample_Set_LED(ABNORMAL_ON);

	/* Initialize variables */
//	SignalSample_moveThenSample = 0;
}

/******************************************************************************/
void SignalSample_Sample_ExitCriticalArea(void)
{
	/* Turn off current source */
	SignalSample_Sample_Set_LED(ABNORMAL_OFF);
}

/******************************************************************************/
void SignalSample_SampleStrip(void)
{
	uint16 moveSteps = MOTOR_SAMPLE_STEPS - 1;

	/* 1.3 Enter critical area */
	SignalSample_Sample_EnterCriticalArea();
	memset(&SignalProcess_sampleBuffer[0],0,512);

	/* 2nd stage: start timer, move motor per interval then sample */
	/* 2.1 Initialize timer */
	SignalSample_Sample_Timer_Init();

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
	{
		SignalSample_count = 180;
		/* 2.2 Move motor per interval, then sample */
		for (;;)
		{
//			if (SignalSample_moveThenSample)
			{
				/* Timer notifies */
				SignalSample_moveThenSample = 0;
				/* Move one step */
				ScanMotorDriver_Move(ScanMotorDriver_DIR_OUT,1);
				/* Sample one time */
//				SignalProcess_sampleBuffer[SignalSample_count--]
//										   = SignalProcess_Collecting_Data();
				/* Determine exit condition */
				if (!(moveSteps--))
					break;
			}
		}
		SignalSample_count = 180;
	}
	else
	{
		SignalSample_count = 0;
		/* Move until slider reaches base position */
		for (SignalSample_count = 0;SignalSample_count < 160;)
		{
			/* Timer notifies */
			ScanMotorDriver_MoveOneStep(ScanMotorDriver_DIR_IN);
			/* Sample one time */
			SignalProcess_sampleBuffer[SignalSample_count++]
											= SignalProcess_Collecting_Data();
		}

		/* Move until slider reaches base position */
		while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
		{
			/* Timer notifies */
			ScanMotorDriver_MoveOneStep(ScanMotorDriver_DIR_IN);
		}
	}

	/* 3rd stage: Post process */
	/* 3.1 Disable timer */
	SignalSample_Sample_Timer_Disabled();
	SignalSample_Moving_Average_Data(SignalProcess_sampleBuffer,SignalSample_count,10);

	/* 3.2 Exit critical area */
	SignalSample_Sample_ExitCriticalArea();
	ScanMotorDriver_Goto_DetectionPosition();
}
