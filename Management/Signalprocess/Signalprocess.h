/*
 * Signalprocess.h
 *
 *  Created on: 2018Äê4ÔÂ4ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_SIGNALPROCESS_SIGNALPROCESS_H_
#define MANAGEMENT_SIGNALPROCESS_SIGNALPROCESS_H_

/******************************************************************************/
#include "comDef.h"

/******************************************************************************/
#define SAMPLE_MAX                          (512)
#define ALG_RESULT_ABNORMAL_C               (1)
#define ALG_RESULT_LOW_AREA_C               (2)
#define ALG_RESULT_NO_T                     (3)

/******************************************************************************/
typedef struct {
	volatile uint16 indexC;
	volatile uint16 indexT;
	volatile uint16 indexBase1;
	volatile uint16 indexBase2;
	volatile uint16 indexBase3;
	volatile uint16 indexBase4;
	volatile float coefA;
	volatile float coefB;
	volatile uint16 areaC;
	volatile uint16 areaT;
	volatile uint8 validity;
	volatile float ratioC_T;
} ALG_CALCINFO;

/******************************************************************************/
typedef struct {
	uint16 C_center;
	uint16 searchHalfRadius_C;
	uint16 dist_C_T1;
	uint16 searchHalfRadius_T;
	uint16 dist_peak1;
	uint16 dist_peak2;
	uint16 dist_peak3;
	uint16 dist_peak4;
	uint16 areaC_HalfRadius;
	uint16 areaT_HalfRadius;
	uint16 winSize;
} ALG_POSINFO;

/******************************************************************************/
typedef struct {
	uint16 C_stepSize;
	uint16 C_magnitude;
	uint16 C_MIN;
} ALG_LIMIT;

/******************************************************************************/
/* Signal Data */
typedef struct {
	uint16 sampleNumber;
	uint16 sampleBuffer[SAMPLE_MAX];
	uint16 processNumder;
	uint16 processBuffer[SAMPLE_MAX];
	ALG_POSINFO posInfo;
	uint8 limitEnabled;
	ALG_LIMIT limitInfo;
	ALG_CALCINFO calcInfo;
} ALG_DATA;

/******************************************************************************/
extern ALG_DATA SignalProcess_Alg_data;

/******************************************************************************/
extern void SignalProcess_Run(void);

#endif /* MANAGEMENT_SIGNALPROCESS_SIGNALPROCESS_H_ */
