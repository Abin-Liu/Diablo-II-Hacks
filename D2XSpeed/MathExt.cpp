#include <stdafx.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "MathExt.h"

void SRand()
{
	srand((unsigned)time(NULL));
}

int IRand(int nLow, int nHigh)
{
	if (nLow == nHigh)
		return nLow;

	if (nLow > nHigh)
	{
		// swap
		int temp = nLow;
		nLow = nHigh;
		nHigh = temp;
	}

	int nResult = rand() % (nHigh - nLow);
	return nResult + nLow;
}

float FRand(unsigned nDecimal)
{
	if (nDecimal > 8)
		nDecimal = 8;

	unsigned long l = long(pow(10, nDecimal));
	int nResult = rand() % l;
	return ((float)nResult) / ((float)l);
}

int RoundOff(float f)
{
	float usf = (f >= 0? f: -f);

	int n = (int)usf + 1;
	int nResult;

	if (usf + 0.5 >= (float)n)
		nResult = (f >= 0? n: -n);
	else
		nResult = (f >= 0? n - 1: 1 - n);

	return nResult;
}

int RoundUp(float f)
{
	int n = (int)f;
	if ((float)n == f)
		return n;
	else
		return n + 1;
}

int RoundDown(float f)
{
	return (int)f;
}