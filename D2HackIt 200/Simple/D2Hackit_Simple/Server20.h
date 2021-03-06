///////////////////////////////////////////////////////////
// Server20.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __SERVER20_H__
#define __SERVER20_H__

#include <windows.h>
#include "CommonStructs.h"
#include "definitions.h"
#include "utilities\\TickThread.h"
#include "d2functions.h"

#define UNIT_MAX_NAME 64

BOOL Server20Start();
BOOL Server20Stop();
VOID Server20GameJoin(THISGAMESTRUCT* thisgame);
VOID Server20GameLeave();

typedef BOOL (CALLBACK *fnInternalEnumUnitProc)(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2);
BOOL InternalEnumUnits(DWORD dwUnitType, fnInternalEnumUnitProc lpfn, LPARAM lParam1, LPARAM lParam2);

LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen);
LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen);
BOOL IsMercClassID(DWORD dwClassID);
DWORD ServerGetUnitStat(UnitAny* p, int nStatID, BOOL bBaseStatOnly);
void DebugTest(char** argv, int argc);

#endif // __SERVER20_H__