///////////////////////////////////////////////////////////
// Me.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __ME_H__
#define __ME_H__

#include <windows.h>
#include "D2HackIt.h"
#include "AffectionDef.h"
#include "MapDef.h"
#include "item.h"

#define MAX_RETRY			5		// Maximum retries allowed for an action

DWORD Player_TimerTick();
void Player_OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen);
DWORD Player_OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen);
VOID Player_OnGameJoin(THISGAMESTRUCT* thisgame);
VOID Player_OnGameLeave();

void AnalyzeVendorItem(DWORD dwItemID, LPCSTR lpszItemCode);
BOOL IsThisPlayerID(const BYTE* ptr);
int CompareHealingPotion(LPCSTR lpszCode1, LPCSTR lpszCode2);
int CompareManaPotion(LPCSTR lpszCode1, LPCSTR lpszCode2);
BOOL BuyMiscItem(DWORD dwItemID, LPCGAMEUNIT lpNpcUnit, int nQuantity);

void NotifyUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam = 0, LPARAM lParam = 0);
void NotifyThisPlayerMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);

BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = 0);
void MakePlayerData(LPGAMEUNIT lpBuffer);
void CheckPlayerMap();
void CheckPlayerHPMana(WORD wNewHP, WORD wNewMana);
DWORD GetSafePlayerID();
LPCSTR GetSafePlayerName();
BYTE GetCurrentAct();
BOOL IsGameJoined();
void AnalyzeItem(const ITEM &item);

BOOL PrepareInteractWithNPC(LPCGAMEUNIT lpNpcUnit);
BOOL OpenNpcStore(LPCGAMEUNIT lpNpcUnit, BOOL bGamble); // Open an npc's store
BOOL GoToEntity(LPCGAMEUNIT lpUnit, BOOL bShowClientMove = TRUE);
BOOL GoToPosition(WORD x, WORD y, BOOL bShowClientMove = TRUE);
LPCITEM FindFirstBeltItem(LPCSTR lpszItemCode);
BOOL IsBeltColFull(int nCol);
void DoTPProc();
void DoNpcProc();
void DoGambleProc();
void OnMyPortalAppeared(LPCGAMEUNIT lpUnit);
void NotifyNpcSession(BOOL bSuccessful);

DWORD FindFirstBeltPotion(int nType, DWORD& dwClassID);
DWORD FindFirstInventoryPotion(int nType, DWORD& dwClassID);

BOOL VerifyItem(DWORD dwItemID);
void DoWPProc();
void FinishWPProc(BOOL bSucceeded);
BOOL IsNpcActive(int nInteractType = 0);
BOOL IsNpcBullShitting();
void InsertGambleItem(DWORD dwItemID, DWORD* aItems, DWORD dwArrayLen);
int BatchGamble(DWORD* aItems, DWORD dwArrayLen);

#endif // __ME_H__