///////////////////////////////////////////////////////////
// Me.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __ME_H__
#define __ME_H__

#include <windows.h>
#include "CommonStructs.h"

VOID Player_OnGameJoin(THISGAMESTRUCT* thisgame);
VOID Player_OnGameLeave();
void CALLBACK PlayerTimerTick(LPVOID lpParam, const volatile BYTE* lpStopFlag);
BOOL IsThisPlayerID(const BYTE* ptr);
void SetPlayerInfoBeforeJoin(DWORD dwPlayerID, LPCSTR lpszPlayerName);
BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = 0);
void MakePlayerData(LPGAMEUNIT lpBuffer);
DWORD GetSafePlayerID();
LPCSTR GetSafePlayerName();
BOOL VerifyItem(DWORD dwItemID);
BOOL GoToEntity(LPCGAMEUNIT lpUnit, WORD x, WORD y, BOOL bShowClientMove = TRUE);
BOOL GoToPosition(WORD x, WORD y, BOOL bShowClientMove = TRUE);

#endif // __ME_H__