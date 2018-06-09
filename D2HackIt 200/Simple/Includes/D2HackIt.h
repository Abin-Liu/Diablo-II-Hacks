//////////////////////////////////////////////////////////////////////
// D2HackIt.h
// -------------------------------------------------------------------
// Main incude file for D2HackIt Exports.
//
// <thohell@home.se>
//
//
// ## This file will define only Server/Client common used exports ##
// <Dan_Goon@diabloworld.com>
//////////////////////////////////////////////////////////////////////
#ifndef __D2HACKIT_H__
#define __D2HACKIT_H__

#define D2_VERSION 110
#include <windows.h>
#include "CommonStructs.h"

//////////////////////////////////////////////////////////////////////
// Version history.
// -------------------------------------------------------------------
// (See D2Hackit.txt for Version History)
//////////////////////////////////////////////////////////////////////
#define __SERVERVERSION__		MAKELONG(2, 0)
#define __SERVERBUILD__			MAKELONG(12, 20)

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#ifdef EXPORT
#undef EXPORT
#endif
#define	EXPORT			__declspec( dllexport ) __cdecl
#define PRIVATE			__cdecl

// Game ready check, added v2.00 by Abin
BOOL EXPORT VerifyUnit(LPCGAMEUNIT lpUnit);
POINT EXPORT GetUnitPosition(LPCGAMEUNIT lpUnit);
DWORD EXPORT GetUnitStat(LPCGAMEUNIT lpUnit, int nStatID);
DWORD EXPORT IsMercUnit(LPCGAMEUNIT lpUnit); // Check if an unit is a merc
int EXPORT GetMonsterType(LPCGAMEUNIT lpUnit); // normal/champion/boss/minion

// game attributes
LPCSTR EXPORT GetRealmName();
LPCSTR EXPORT GetAccountName();
BOOL EXPORT GetMyMercUnit(LPGAMEUNIT lpBuffer); // Get player's mercenary
DWORD EXPORT GetStashGoldLimit(); // stash gold limit
DWORD EXPORT GetInventoryGoldLimit(); // inventory gold limit
BOOL EXPORT IsHardcore(); // is hardcore player?
DWORD EXPORT GetCursorItem(); // returns ID of the item on cursor, if any
BOOL EXPORT EnumStorageItems(int nStorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam = 0);
BOOL EXPORT GetItemCode(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars);
DWORD EXPORT GetEquipItem(int nEquipLocation);
DWORD EXPORT GetStorageItem(int nStorageType, int x, int y);
int EXPORT GetItemQuality(DWORD dwItemID);
DWORD EXPORT GetItemPrice(DWORD dwItemID, DWORD dwNpcClassID, BOOL bBuyOrSell);
DWORD EXPORT GetItemAttributes(DWORD dwItemID);
BYTE EXPORT GetItemSockets(DWORD dwItemID);
DWORD EXPORT GetItemProperties(DWORD dwItemID, LPITEMPROPERTY lpBuffer, DWORD dwMaxCount);
BYTE EXPORT GetItemLevel(DWORD dwItemID);
DWORD EXPORT GetItemDescription(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor = TRUE);
DWORD EXPORT GetItemStrRequirement(DWORD dwItemID);
DWORD EXPORT GetItemDexRequirement(DWORD dwItemID);
DWORD EXPORT GetItemLevelRequirement(DWORD dwItemID);
BOOL EXPORT IsTwoHandItem(DWORD dwItemID);
BOOL EXPORT GetItemDamage(DWORD dwItemID, DWORD* pMinDamage, DWORD* pMaxDamage);
SIZE EXPORT GetItemSize2(DWORD dwItemID);
int EXPORT GetItemType(DWORD dwItemID);
int EXPORT GetItemLocationType(DWORD dwItemID);
DWORD EXPORT GetItemProperties2(DWORD dwItemID, LPITEMMOD lpBuffer, DWORD dwMaxCount);
int EXPORT GetItemImageIndex(DWORD dwItemID);
DWORD EXPORT GetMercEquip(int nEquipLocation);

#ifdef __cplusplus
}		             /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#endif // __D2HACKIT_H__

