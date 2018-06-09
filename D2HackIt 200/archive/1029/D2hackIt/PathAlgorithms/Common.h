//////////////////////////////////////////////////////////
// Common.h
//--------------------------------------------------------
// Common algorithms.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////

#include <windows.h>

long CalculateDistance(POINT pt1, POINT pt2);
long CalculateDistance(long x1, long y1, long x2, long y2);
long CalculateAngle(POINT pt1, POINT pt2);
long CalculateAngle(long x1, long y1, long x2, long y2);
void NormalizeAngle(int& rAngle);
void NormalizeRect(RECT& rRect);
void NormalizeRect(LPRECT lpRect);
POINT CalculatePointOnTrack(POINT ptOrigin, int nRadius, int nAngle);
POINT CalculateRandomPosition(POINT ptOrigin, int nRadiusMin, int nRadiusMax, int nAngleMin = 0, int nAngleMax = 360);
BOOL PtInCircle(POINT pt, POINT ptOrigin, int nRadius);
