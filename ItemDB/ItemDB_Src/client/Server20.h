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
#include "TickThread.h"
#include "d2functions.h"

extern HWND g_hD2Wnd;

#define UNIT_MAX_NAME 64

BOOL Server20Start();
BOOL Server20Stop();

// Game ready check, added v2.00 by Abin
BOOL VerifyUnit(LPCGAMEUNIT lpUnit);
POINT GetItemPosition(DWORD dwItemDB);
DWORD GetUnitStat(LPCGAMEUNIT lpUnit, int nStatID);

// game attributes
LPCSTR GetRealmName();
LPCSTR GetAccountName();
BOOL GetMyMercUnit(LPGAMEUNIT lpBuffer); // Get player's mercenary
DWORD GetStashGoldLimit(); // stash gold limit
DWORD GetInventoryGoldLimit(); // inventory gold limit
DWORD GetStat(int nStatID);
DWORD GetUnitClassID(LPCGAMEUNIT lpUnit);
DWORD GetUnitName(LPCGAMEUNIT lpUnit, LPSTR lpszBuffer, DWORD dwMaxChars);
BOOL IsWeaponSwitched();
BOOL IsHardcore(); // is hardcore player?
DWORD GetCursorItem(); // returns ID of the item on cursor, if any
BOOL EnumStorageItems(int nStorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam = 0);
BOOL GetItemCode(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars);
DWORD GetEquipItem(int nEquipLocation);
DWORD GetStorageItem(int nStorageType, int x, int y);
int GetItemQuality(DWORD dwItemID);
DWORD GetItemPrice(DWORD dwItemID, DWORD dwNpcClassID, BOOL bBuyOrSell);
DWORD GetItemAttributes(DWORD dwItemID);
BYTE GetItemSockets(DWORD dwItemID);
DWORD GetItemProperties(DWORD dwItemID, LPITEMPROPERTY lpBuffer, DWORD dwMaxCount);
BYTE GetItemLevel(DWORD dwItemID);
DWORD GetItemDescription(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor = TRUE);
DWORD GetItemStrRequirement(DWORD dwItemID);
DWORD GetItemDexRequirement(DWORD dwItemID);
DWORD GetItemLevelRequirement(DWORD dwItemID);
BOOL IsTwoHandItem(DWORD dwItemID);
BOOL GetItemDamage(DWORD dwItemID, DWORD* pMinDamage, DWORD* pMaxDamage);
SIZE GetItemSize2(DWORD dwItemID);
int GetItemType(DWORD dwItemID);
int GetItemLocationType(DWORD dwItemID);
DWORD GetItemProperties2(DWORD dwItemID, LPITEMMOD lpBuffer, DWORD dwMaxCount);
int GetItemImageIndex(DWORD dwItemID);
DWORD GetMercEquip(int nEquipLocation);

LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen);
LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen);
BOOL IsMercClassID(DWORD dwClassID);
DWORD ServerGetUnitStat(UnitAny* p, int nStatID, BOOL bBaseStatOnly);
BOOL VerifyItem(DWORD dwItemID);

void DisplayInfo(LPCSTR lpszInfo, BOOL bBottom);

void DebugTest(char** argv, int argc);

#endif // __SERVER20_H__