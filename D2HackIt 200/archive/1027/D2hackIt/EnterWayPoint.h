///////////////////////////////////////////////////////////
// EnterWayPoint.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __ENTERWAYPOINT__
#define __ENTERWAYPOINT__

#include <windows.h>

void CleanEnterWayPointStats();
BOOL ProcessEnterWayPoint(const volatile BYTE* lpStopFlag);
void OnWpOpen(BOOL bOpened);
BOOL IsWpOpened();
void OnMapChangeCheckWayPoint(BYTE iMapID);

#endif // __ENTERWAYPOINT__
