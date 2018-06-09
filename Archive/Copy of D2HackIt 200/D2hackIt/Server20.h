///////////////////////////////////////////////////////////
// Server20.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __SERVER20_H__
#define __SERVER20_H__

#include <windows.h>
#include "D2Hackit.h"
#include "me.h"

#define UNIT_MAX_NAME 64

extern DWORD g_dwBannerHook; // banner text hook id
extern BOOL g_bDrawBanner; // draw banner text or not
extern BOOL g_bQuitOnVersionCheckDll;
extern BOOL g_bQuitOnExtraworkDll;
extern BOOL g_bIntegrityScanOnReload;
struct PKT
{
	BYTE* aPacket;
	DWORD aLen;
};

VOID Proc_OnGameJoin(THISGAMESTRUCT* thisgame);
VOID Proc_OnGameLeave();
DWORD Proc_OnGameTimerTick();
DWORD Proc_OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen);
DWORD Proc_OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen);
void OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen);

void CALLBACK FreePktData(PKT& pkt);

LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen);
LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen);
BOOL IsMercClassID(DWORD dwClassID);
void TrimString(LPSTR lpszString);
BOOL DrawBanner();

#endif // __SERVER20_H__