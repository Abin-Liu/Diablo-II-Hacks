#ifndef _MATHEXT_H
#define _MATHEXT_H

// random number functions

// reset the random generator seed
void SRand();

// generate a random integer n such that nLow <= n < nHigh
int IRand(int nLow, int nHigh);

// generate a random float f such that 0.0 <= f < 1.0, and
// f has accuracy of nDecimal (e.g. accuracy of 1.25 is 2,
// accuracy of 1.250 is 3)
float FRand(unsigned nDecimal);


// decimal functions

// calculates the round off value of a float
int RoundOff(float f);

// calculates the round up value of a float
int RoundUp(float f);

// calculates the round down value of a float
int RoundDown(float f);

#endif