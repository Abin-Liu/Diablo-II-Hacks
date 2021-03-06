///////////////////////////////////////////////////////////
// Server20.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __SERVER20_H__
#define __SERVER20_H__

#include <windows.h>
#include "CommonStructs.h"

#define UNIT_MAX_NAME 64

extern BOOL g_bDrawBanner; // draw banner text
extern BOOL g_bDrawMyStats; // draw my stats
extern BOOL g_bQuitOnVersionCheckDll;
extern BOOL g_bQuitOnExtraworkDll;
extern BOOL g_bIntegrityScanOnReload;

#define NEED_ABORT(ptr) (((ptr) == NULL) || *(ptr))

VOID Proc_OnGameJoin(THISGAMESTRUCT* thisgame);
VOID Proc_OnGameLeave();
LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen);
LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen);
BOOL IsMercClassID(DWORD dwClassID);
void TrimString(LPSTR lpszString);
void OnMapChanged(BYTE iNewMapID);
BOOL ProcessIncomingPackets(const volatile BYTE* lpStopFlag);
BOOL ProcessOutgoingPackets(const volatile BYTE* lpStopFlag);
void CleanOutgoingPacketQueue();
VOID GameSendPacketToServer2(LPBYTE buf, DWORD len, DWORD dwDelay = 100);
BOOL IsGameJoined();
void DebugTest(char** argv, int argc);

#endif // __SERVER20_H__