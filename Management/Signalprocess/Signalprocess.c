/*
 * Signalprocess.c
 *
 *  Created on: 2018Äê4ÔÂ4ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "Signalprocess.h"

/******************************************************************************/
ALG_DATA SignalProcess_Alg_data;

/******************************************************************************/
uint16 Alg_WindowAverage(uint16 *src, uint16 *dest, uint16 count, uint8 winSize);
uint16 Alg_GetValleyIndex(uint16 *src, uint16 startIndex, uint16 endIndex);
uint8 Alg_JudgeCValidity(uint16 *src, uint16 index, uint16 stepSize, uint16 INC);
void Alg_FittingLine(uint16 arrayX[], uint16 arrayY[], uint16 size,
		float *coefA, float *coefB);
uint16 Alg_MoveToFitArray(uint16 dataArray[], uint16 midIndex, uint16 *xPtr,
		uint16 *yPtr, uint16 curSize);
float Alg_CalcualteArea(uint16 src[], uint16 midIndex, uint16 halfWidth,
		float *coefA, float *coefB);
uint16 Alg_GetMin(uint16 *src, uint16 count);
uint16 Alg_GetMax(uint16 *src, uint16 count);

/******************************************************************************/
void SignalProcess_Run(void)
{
    uint16 searchStart = 0;
    uint8 validity = 0;
    uint16 arrayXFit[20] = {0};
    uint16 arrayYFit[20] = {0};
    uint16 sizeFit = 0;
    uint8 fitArraySize = 0;
    /* Clear */
    memset(&SignalProcess_Alg_data.calcInfo, 0, sizeof(ALG_CALCINFO));

    /* Window average */
    SignalProcess_Alg_data.processNumder = Alg_WindowAverage(
            &SignalProcess_Alg_data.sampleBuffer[0],
            &SignalProcess_Alg_data.processBuffer[0],
            SignalProcess_Alg_data.sampleNumber,
            SignalProcess_Alg_data.posInfo.winSize);

    /* Look for C valley */
    if (SignalProcess_Alg_data.posInfo.C_center >= SignalProcess_Alg_data.posInfo.searchHalfRadius_C)
        searchStart = SignalProcess_Alg_data.posInfo.C_center - SignalProcess_Alg_data.posInfo.searchHalfRadius_C;
    else
        searchStart = 0;
    SignalProcess_Alg_data.calcInfo.indexC = Alg_GetValleyIndex(
        &SignalProcess_Alg_data.processBuffer[0], searchStart,
        SignalProcess_Alg_data.posInfo.C_center + SignalProcess_Alg_data.posInfo.searchHalfRadius_C);

    if (SignalProcess_Alg_data.limitEnabled > 0)
    {
        /* Judge validity of C */
        validity = Alg_JudgeCValidity(&SignalProcess_Alg_data.processBuffer[0],
                        SignalProcess_Alg_data.calcInfo.indexC,
                        SignalProcess_Alg_data.limitInfo.C_stepSize,
                        SignalProcess_Alg_data.limitInfo.C_magnitude);

        if (0 == validity)
        {
            SignalProcess_Alg_data.calcInfo.validity = ALG_RESULT_ABNORMAL_C;
            return;
        }
    }

    /* Look for T1 valley */
    SignalProcess_Alg_data.calcInfo.indexT = Alg_GetValleyIndex(
            &SignalProcess_Alg_data.processBuffer[0],
            SignalProcess_Alg_data.calcInfo.indexC + SignalProcess_Alg_data.posInfo.dist_C_T1 - SignalProcess_Alg_data.posInfo.searchHalfRadius_T,
            SignalProcess_Alg_data.calcInfo.indexC + SignalProcess_Alg_data.posInfo.dist_C_T1 + SignalProcess_Alg_data.posInfo.searchHalfRadius_T);

    /* Get base points: left of C */
    if (SignalProcess_Alg_data.posInfo.dist_peak1 > 0)
    {
        if (SignalProcess_Alg_data.calcInfo.indexC > SignalProcess_Alg_data.posInfo.dist_peak1)
        {
            SignalProcess_Alg_data.calcInfo.indexBase1 =
                    SignalProcess_Alg_data.calcInfo.indexC - SignalProcess_Alg_data.posInfo.dist_peak1;

            /* Prepare data for fitting line */
            fitArraySize = Alg_MoveToFitArray(&SignalProcess_Alg_data.processBuffer[0],
                    SignalProcess_Alg_data.calcInfo.indexBase1, &arrayXFit[0], &arrayYFit[0], fitArraySize);
        }
    }

    /* Get base points: between C and the edge T */
    if (SignalProcess_Alg_data.posInfo.dist_peak2 > 0)
    {
        SignalProcess_Alg_data.calcInfo.indexBase2 =
                SignalProcess_Alg_data.calcInfo.indexC + SignalProcess_Alg_data.posInfo.dist_peak2;

        /* Prepare data for fitting line */
        fitArraySize = Alg_MoveToFitArray(&SignalProcess_Alg_data.processBuffer[0],
                SignalProcess_Alg_data.calcInfo.indexBase2, &arrayXFit[fitArraySize], &arrayYFit[fitArraySize], fitArraySize);
    }

    /* Get base points: between C and the edge T */
    if (SignalProcess_Alg_data.posInfo.dist_peak3 > 0)
    {
        SignalProcess_Alg_data.calcInfo.indexBase3 =
                SignalProcess_Alg_data.calcInfo.indexC + SignalProcess_Alg_data.posInfo.dist_peak3;

        /* Prepare data for fitting line */
        fitArraySize = Alg_MoveToFitArray(&SignalProcess_Alg_data.processBuffer[0],
                SignalProcess_Alg_data.calcInfo.indexBase3, &arrayXFit[fitArraySize], &arrayYFit[fitArraySize], fitArraySize);
    }

    /* Get base points: between T and edge */
    if (SignalProcess_Alg_data.posInfo.dist_peak4 > 0)
    {
        SignalProcess_Alg_data.calcInfo.indexBase4 =
                SignalProcess_Alg_data.calcInfo.indexC + SignalProcess_Alg_data.posInfo.dist_peak4;

        /* Prepare data for fitting line */
        fitArraySize = Alg_MoveToFitArray(&SignalProcess_Alg_data.processBuffer[0],
                SignalProcess_Alg_data.calcInfo.indexBase4, &arrayXFit[fitArraySize], &arrayYFit[fitArraySize], fitArraySize);
    }

    /* Fit line */
    Alg_FittingLine(arrayXFit, arrayYFit, fitArraySize, &SignalProcess_Alg_data.calcInfo.coefA,
            &SignalProcess_Alg_data.calcInfo.coefB);

    /* Calculate area */
    SignalProcess_Alg_data.calcInfo.areaC = (uint32)Alg_CalcualteArea(
            &SignalProcess_Alg_data.processBuffer[0],
            SignalProcess_Alg_data.calcInfo.indexC,
            SignalProcess_Alg_data.posInfo.areaC_HalfRadius,
            &SignalProcess_Alg_data.calcInfo.coefA,
            &SignalProcess_Alg_data.calcInfo.coefB);

    SignalProcess_Alg_data.calcInfo.areaT = (uint32)Alg_CalcualteArea(
            &SignalProcess_Alg_data.processBuffer[0],
            SignalProcess_Alg_data.calcInfo.indexT,
            SignalProcess_Alg_data.posInfo.areaT_HalfRadius,
            &SignalProcess_Alg_data.calcInfo.coefA,
            &SignalProcess_Alg_data.calcInfo.coefB);

    if (SignalProcess_Alg_data.limitEnabled > 0)
    {
        if (SignalProcess_Alg_data.calcInfo.areaC < SignalProcess_Alg_data.limitInfo.C_MIN)
        {
            SignalProcess_Alg_data.calcInfo.validity = ALG_RESULT_LOW_AREA_C;
            return;
        }
    }

    if (SignalProcess_Alg_data.calcInfo.areaT == 0)
    {
        SignalProcess_Alg_data.calcInfo.validity = ALG_RESULT_NO_T;
        return;
    }

    /* Calculate C/T */
    SignalProcess_Alg_data.calcInfo.ratioC_T =
            (float)SignalProcess_Alg_data.calcInfo.areaC / (float)SignalProcess_Alg_data.calcInfo.areaT;
}

/******************************************************************************/
uint16 Alg_WindowAverage(uint16 *src, uint16 *dest, uint16 count, uint8 winSize)
{
	uint16 index, winIndex;
	uint16 newLength, winIndexMax;
	uint32 sumVal;

	if (winSize > count)
		return 0;

	/* Don't calculate the tail */
	newLength = count - winSize + 1;

	for (index = 0; index < newLength; index++)
	{
		sumVal = 0;
		/* Window */
		winIndexMax = index + winSize;
		for (winIndex = index; winIndex < winIndexMax; winIndex++)
		{
			sumVal += src[winIndex];
		}
		/* Store window average value */
		dest[index] = sumVal/winSize;
	}

	return newLength;
}

/******************************************************************************/
uint16 Alg_GetValleyIndex(uint16 *src, uint16 startIndex, uint16 endIndex)
{
	uint16 index, valleyIndex;
	uint16 valleyValue;

	valleyIndex = startIndex;

	/* Search to get the index of minimal value */
	valleyValue = src[startIndex];
	for (index = startIndex; index < endIndex; index++)
	{
		if (valleyValue > src[index])
		{
			valleyValue = src[index];
			valleyIndex = index;
		}
	}

	return valleyIndex;
}

/******************************************************************************/
uint8 Alg_JudgeCValidity(uint16 *src, uint16 index, uint16 stepSize, uint16 INC)
{
#define ALG_JUDGE_POINTS (3)
	uint16 leftIndex[ALG_JUDGE_POINTS] = {0};
	uint16 rightIndex[ALG_JUDGE_POINTS] = {0};
	uint8 i = 0;
	uint8 status = 1;

	/* Left */
	for (i = 0; i < ALG_JUDGE_POINTS; i++)
		leftIndex[i] = index - (ALG_JUDGE_POINTS - 1 - i) * stepSize;

	for (i = 0; i < (ALG_JUDGE_POINTS - 1); i++)
	{
		if (src[leftIndex[i]] < src[leftIndex[i + 1]])
		{
			status = 0;
			break;
		}

		if ((src[leftIndex[i]] - src[leftIndex[i + 1]]) < INC)
		{
			status = 0;
			break;
		}
	}

	if (0 == status)
		return status;

	/* Right */
	for (i = 0; i < ALG_JUDGE_POINTS; i++)
		rightIndex[i] = index + (ALG_JUDGE_POINTS - 1 - i) * stepSize;

	for (i = 0; i < (ALG_JUDGE_POINTS - 1); i++)
	{
		if (src[rightIndex[i]] < src[rightIndex[i + 1]])
		{
			status = 0;
			break;
		}

		if ((src[rightIndex[i]] - src[rightIndex[i + 1]]) < INC)
		{
			status = 0;
			break;
		}
	}

	return status;
}

/******************************************************************************/
void Alg_FittingLine(uint16 arrayX[], uint16 arrayY[], uint16 size,
		float *coefA, float *coefB)
{
	uint16 index;
	uint32 sumXX, sumXY, sumX, sumY;

	/* Calculate per formula */
	/* Initialize */
	sumX = 0;
	sumY = 0;
	sumXX = 0;
	sumXY = 0;

	/* Calculate sums */
	for (index = 0; index < size; index++)
	{
		sumX += arrayX[index];
		sumY += arrayY[index];
		sumXX += arrayX[index] * arrayX[index];
		sumXY += arrayX[index] * arrayY[index];
	}

	/* Calculate coefA and coefB */
	*coefA = (float)((size * (double)sumXY - (double)sumX * (double)sumY)/
				(float)(size * (double)sumXX - (double)sumX * (double)sumX));
	*coefB = (float)(((double)sumY / size - (*coefA) * (double)sumX / size));
}


/******************************************************************************/
uint16 Alg_MoveToFitArray(uint16 dataArray[], uint16 midIndex, uint16 *xPtr,
		uint16 *yPtr, uint16 curSize)
{
#define BASE_SIZE 4
	uint8 index = 0;
	uint16 xValue = 0;

	xValue = midIndex > (BASE_SIZE >> 1)? (midIndex - (BASE_SIZE >> 1)) : 0;

	for (index = 0; index < BASE_SIZE; index++)
	{
		*xPtr++ = xValue;
		*yPtr++ = dataArray[xValue];
		xValue++;
	}

	curSize += BASE_SIZE;

	return curSize;
}


/******************************************************************************/
float Alg_CalcualteArea(uint16 src[], uint16 midIndex, uint16 halfWidth,
		float *coefA, float *coefB)
{
	float area = 0, area1 = 0, area2 = 0, diff = 0;
	float a, b;
	uint16 index, startIndex, endIndex;
	uint16 width = (halfWidth << 1);

	/* To avoid calculation error */
	if (midIndex >= halfWidth)
		startIndex = midIndex - halfWidth;
	else
		startIndex = 0;
	endIndex = startIndex + width;

	/* Corner case process */
	if (((*coefA) * midIndex + (*coefB)) > src[midIndex])
	{
		/* Straight line is above valley */
		if (((*coefA) * startIndex + (*coefB)) < src[startIndex])
		{
			/* Start point: straight line is below curve */
			/* Search real start index */
			for (index = startIndex; index < midIndex; index++)
			{
				if (((*coefA) * index + (*coefB)) >= src[index])
				{
					startIndex = index;
					break;
				}
			}
		}

		if (((*coefA) * endIndex + (*coefB)) < src[endIndex])
		{
			/* End point: straight line is below curve */
			/* Search real start index */
			for (index = midIndex; index < endIndex; index++)
			{
				if (((*coefA) * index + (*coefB)) <= src[index])
				{
					endIndex = index;
					break;
				}
			}
		}

		/* S = (a + b) * h / 2 */
		/* Correct width */
		width = endIndex - startIndex;

		a = (*coefA) * startIndex + (*coefB);
		b = (*coefA) * endIndex + (*coefB);
		area1 = (a + b) * width / 2;

		/* S = sum(RANGE) */
		area2 = 0;
		for (index = startIndex; index < endIndex; index++)
		{
			area2 += src[index];
		}

		/* Delta area */
		area = area1 - area2;

		return area;
	}
	else
	{
		/* No control/test line because straight line is below valley */
		return 0;
	}
}

/******************************************************************************/
uint16 Alg_GetMin(uint16 *src, uint16 count)
{
	uint16 value = 0;
	uint16 index = 0;

	/* Assign initial value */
	value = src[0];
	/* Search */
	for (index = 1; index < count; index++)
	{
		if (value > src[index])
			value = src[index];
	}

	return value;
}

/******************************************************************************/
uint16 Alg_GetMax(uint16 *src, uint16 count)
{
	uint16 value = 0;
	uint16 index = 0;

	/* Assign initial value */
	value = src[0];
	/* Search */
	for (index = 1; index < count; index++)
	{
		if (value < src[index])
			value = src[index];
	}

	return value;
}
