///////////////////////////////////////////////////////////
// BackToTown.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __BACKTOTOWN_H__
#define __BACKTOTOWN_H__

#include <windows.h>

void CleanBackToTownStates();
BOOL IsBackToTownInProcess();
BOOL ProcessBackToTown(const volatile BYTE* lpStopFlag);

#endif // __BACKTOTOWN_H__