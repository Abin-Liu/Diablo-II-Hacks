///////////////////////////////////////////////////////////////
// MotionJob.h
//
// Maintains a FIFO queue of motion jobs.
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////

#ifndef __MOTIONJOB_H__
#define __MOTIONJOB_H__
#include <windows.h>
#include "utilities\\LinkedList.h"
#include "..\\Includes\\CommonStructs.h"

// Types of movement
enum { MJ_UNKNOWN = 0, MJ_POSITION, MJ_UNIT, MJ_TELEPORT };

// Motion job struct
typedef struct tagMotionJob
{
	int nJobType; // Type of movement
	GAMEUNIT unit;	// target unit
	WORD x;	// Destination position x
	WORD y; // Destination position y
	//int nMaxRetry; // max retries allowed
	//int nRetried; // retried
	//DWORD dwMaxWait; // max wait time in milliseconds
	//DWORD dwElapsed; // milliseconds elapsed
} MOTIONJOB, *LPMOTIONJOB;
typedef const tagMotionJob* LPCMOTIONJOB;

void AddMotionJob(const MOTIONJOB& job);
void CompleteJobs(BOOL bSuccessful);
BOOL DoFirstJob(BOOL bRedo);
void StepDone();
BOOL IsStanding();

#endif // __MOTIONJOB_H__