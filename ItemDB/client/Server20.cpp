///////////////////////////////////////////////////////////////////////
// Server20.cpp
//
// Abin(abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////
#include "server20.h"
#include "..\\Common\\LinkedList.h"
#include "..\\Common\\ArrayEx.h"
#include "StringEx.h"
#include "d2functions.h"
#include "definitions.h"
#include "Inventory.h"
#include "ItemCheck.h"
#include "Client.h"

const UnitPlayer* g_pPLayer = NULL;
CTickThread g_serverTickThread;

void CALLBACK ServerTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag);
DWORD GetItemRequirement(DWORD dwItemID, int nStatID);
DWORD GetItemReqRecursive(int nMin, int nMax, UnitItem* pItem, UnitAny* pUnit, Stats* pStats, DWORD* p1, DWORD* p2, DWORD* p3, DWORD* pMatch);
int ReplaceColorSpecs(LPWSTR lpwsz);

BOOL Server20Start()
{
	g_serverTickThread.Start(ServerTimerTick, NULL, 500);
	return TRUE;
}

BOOL Server20Stop()
{
	DisplayInfo("ÿc9ItemDB Removed.", FALSE);
	g_serverTickThread.Stop(3000);
	if (g_pPLayer)
		OnGameLeave();

	return TRUE;
}

void CALLBACK ServerTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag)
{
	const UnitPlayer* p = D2CLIENT_GetPlayerUnit();
	if (p != g_pPLayer)
	{
		g_pPLayer = p;
		if (p)
			OnGameJoin(p);
		else
			OnGameLeave();
	}

	if (p)
		OnGameTimerTick();
}

BOOL VerifyUnit(LPCGAMEUNIT lpUnit)
{
	if (lpUnit == NULL)
		return FALSE;

	return (BOOL)D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
}

LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen)
{
	::MultiByteToWideChar(CP_ACP, 0, lpszAnsiString, -1, lpUnicodeString, maxlen);
	return lpUnicodeString;
}

LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen)
{
	::WideCharToMultiByte(CP_ACP, 0, lpUnicodeString, -1, lpszAnsiString, maxlen, NULL, NULL);
	return lpszAnsiString;
}

POINT GetItemPosition(DWORD dwItemDB)
{
	POINT mp = { 0 };
	UnitAny* p = D2CLIENT_GetUnit(dwItemDB, UNIT_TYPE_ITEM);
	if (p && p->hOPath)
	{
		mp.x = p->hOPath->Xpos;
		mp.y = p->hOPath->Ypos;
	}

	return mp;
}

DWORD GetStat(int nStatID)
{
	return ServerGetUnitStat(D2CLIENT_GetPlayerUnit(), nStatID, FALSE);
}

DWORD GetUnitClassID(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return -1;
	return p->dwClassId;
}

DWORD GetUnitStat(LPCGAMEUNIT lpUnit, int nStatID)
{
	return ServerGetUnitStat((UnitAny*)VerifyUnit(lpUnit), nStatID, FALSE);
}

DWORD GetUnitName(LPCGAMEUNIT lpUnit, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpUnit == NULL || lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars);
	UnitAny* pUnit = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (pUnit == NULL)
		return 0;

	// If its a player unit then get the name from the player data structure
	// thanks to Henrik Noerregaard Joergensen<hnj@kalliope.org>
	if (pUnit->dwType == UNIT_TYPE_PLAYER)
	{
		if (pUnit->ptPlayerData == NULL)
			return 0;

		strncpy(lpszBuffer, pUnit->ptPlayerData->PlayerName, dwMaxChars);
		return strlen(lpszBuffer);
	}

	wchar_t* wszName = D2CLIENT_GetNameFromUnit(pUnit);
	if (wszName == NULL)
		return 0;

	ConvertFromUnicode(wszName, lpszBuffer, dwMaxChars);
	return strlen(lpszBuffer);
}

BOOL IsMercClassID(DWORD dwClassID)
{
	return dwClassID == MERC_A1 || dwClassID == MERC_A2 || dwClassID == MERC_A3 || dwClassID == MERC_A5;
}

LPCSTR GetRealmName()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).RealmName : NULL;
}

LPCSTR GetGameName()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).GameName : NULL;
}

LPCSTR GetGamePassword()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).GamePass : NULL;
}

BOOL GetItemCode(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL ||dwMaxChars == 0)
		return FALSE;

	::memset(lpszBuffer, 0, dwMaxChars + 1);

	GAMEUNIT gu;
	gu.dwUnitID = dwItemID;
	gu.dwUnitType = UNIT_TYPE_ITEM;
	UnitAny* pUnit = (UnitAny*)VerifyUnit(&gu);
	if (pUnit == NULL)
	{
		//ItemError(dwItemID, "GetItemCode");
		return FALSE;
	}

	ItemTxt* ptxt = D2COMMON_GetItemTxt(pUnit->dwClassId);
	if (ptxt == NULL)
		return FALSE;

	::strncpy(lpszBuffer, ptxt->szCode, min(dwMaxChars, 3));
	return strlen(lpszBuffer);
}

BOOL IsQuestItem(LPCSTR lpszItemCode)
{
	return D2IsQuestItem(lpszItemCode);
}

int GetItemQuality(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL || pItem->ptItemData == NULL)
		return 0;

	return pItem->ptItemData->Quality1;
}

DWORD GetItemPrice(DWORD dwItemID, DWORD dwNpcClassID, BOOL bBuyOrSell)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
	{
		return 0;
	}

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return 0;
	
	return (DWORD)D2COMMON_GetUnitPrice(pPlayer, pItem, *p_D2CLIENT_nDiff, *p_D2CLIENT_ItemPriceGameInfoUnk, dwNpcClassID, !bBuyOrSell);
}

DWORD GetItemAttributes(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
	{
		return 0;
	}

	return pItem->ptItemData->dwFlags;
}

BYTE GetItemSockets(DWORD dwItemID)
{
	GAMEUNIT gu;
	gu.dwUnitID = dwItemID;
	gu.dwUnitType = UNIT_TYPE_ITEM;
	return (BYTE)GetUnitStat(&gu, 0xc2);
}

DWORD GetItemProperties(DWORD dwItemID, LPITEMPROPERTY lpBuffer, DWORD dwMaxCount)
{
	if (dwItemID == 0 || lpBuffer == NULL || dwMaxCount == 0)
		return 0;

	UnitItem* p = (UnitItem*)VerifyItem(dwItemID);
	if (p == NULL)
	{
		return 0;
	}

	UnitStats *pStats = D2COMMON_GetUnitPtrStats3((UnitAny*)p, 0, 0x40);
	if (pStats == NULL)
		return 0;

	return (DWORD)D2COMMON_ListUnitStats(pStats, (ListStats*)lpBuffer, dwMaxCount);
}

BYTE GetItemLevel(DWORD dwItemID)
{
	UnitAny* p = (UnitAny*)VerifyItem(dwItemID);
	return p ? (BYTE)p->ptItemData->_14[0] : 0;	
}

DWORD GetItemDescription(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor)
{
	if (lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars);
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return 0;

	wchar_t wsz[256] = { 0 };
	char sz1[256] = "";	

	D2CLIENT_GetItemNameString(pItem, wsz, 255);
	ConvertFromUnicode(wsz, sz1, 255);
	CStringEx sTemp(sz1);	

	int nQuestion = sTemp.Find('?');
	if (nQuestion != -1)
		sTemp.Delete(nQuestion, 3);

	CArrayEx<CStringEx, const CStringEx&> aList;	
	CStringEx sSeg;
	char sz0A[] = { 0x0a, 0x00 };
	BOOL b = sTemp.FindToken(sz0A);
	while (b)
	{
		b = sTemp.FindNextToken(sSeg);
		sSeg.Trim();
		if (!sSeg.IsEmpty())
			aList.Add(sSeg);
	}

	CStringEx str;
	for (int i = aList.GetSize() - 1; i >= 0; i--)
	{
		str += aList[i];
		str += '|';
	}
	
	str.TrimRight('|');
	str.Trim();
	if (str.IsEmpty())
		return 0;
	
	DWORD dwCopy = min(dwMaxChars, (DWORD)str.GetLength());
	::memcpy(lpszBuffer, (LPCSTR)str, dwCopy);
	lpszBuffer[dwCopy] = 0;
	return dwCopy;
}

DWORD ServerGetUnitStat(UnitAny* p, int nStatID, BOOL bBaseStatOnly)
{
	if (p == NULL)
		return 0;

	DWORD dwResult = 0;
	if (bBaseStatOnly)
		dwResult = D2COMMON_GetUnitCleanBaseStat(p, nStatID, 0);
	else
		dwResult = D2COMMON_GetUnitModifiedStat(p, nStatID, 0);

	if (nStatID == STAT_HP || nStatID == STAT_MAXHP
		|| nStatID == STAT_MANA || nStatID == STAT_MAXMANA
		|| nStatID == STAT_STAMINA || nStatID == STAT_MAXSTAMINA)
	{
		dwResult /= 256;
	}

	return dwResult;
}

///////////////////////////////////////////////////////////////////////
// GetItemReqRecursive
//---------------------------------------------------------------------
// Searching for requirements could be time consuming if we start from 0
// and perform a set of linear comparison. If the item require 232 strength,
// e.g., Sacred Armor, we will have to call D2COMMON_TestItemMeetReq 232
// times, which would be ugly.
//
// This recursive function implements a binary searching algorithm to get the
// job done, hence reduces the complexity to O(log(n)).
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////
DWORD GetItemReqRecursive(int nMin, int nMax, UnitItem* pItem, UnitAny* pUnit, Stats* pStats, DWORD* p1, DWORD* p2, DWORD* p3, DWORD* pMatch)
{
	const int DIFF = nMax - nMin;

	if (DIFF < 4)
	{
		// do a linear search
		for (pStats->dwValue = (DWORD)nMin; (int)pStats->dwValue <= nMax; pStats->dwValue++)
		{
			D2COMMON_TestItemMeetReq(pItem, pUnit, 0, p1, p2, p3);
			if (*pMatch)
				return pStats->dwValue;
		}

		return nMax + 1;
	}

	pStats->dwValue = nMin;
	D2COMMON_TestItemMeetReq(pItem, pUnit, 0, p1, p2, p3);
	if (*pMatch)
		return nMin;

	pStats->dwValue = nMin + DIFF / 2;
	D2COMMON_TestItemMeetReq(pItem, pUnit, 0, p1, p2, p3);
	if (*pMatch)
		return GetItemReqRecursive(nMin, nMin + DIFF / 2, pItem, pUnit, pStats, p1, p2, p3, pMatch);
	else
		return GetItemReqRecursive(nMin + DIFF / 2 + 1, nMax, pItem, pUnit, pStats, p1, p2, p3, pMatch);
}

DWORD GetItemRequirement(DWORD dwItemID, int nStatID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return -1;

	DWORD dwStr, dwDex, dwLvl;
	DWORD* pMatch = NULL;
	if (nStatID == STAT_STRENGTH)
		pMatch = &dwStr;
	else if (nStatID == STAT_DEXTERITY)
		pMatch = &dwDex;
	else if (nStatID == STAT_LEVEL)
		pMatch = &dwLvl;

	if (pMatch == NULL)
		return -1;

	Stats st = { MAKELONG(0, nStatID), 0 };	// Create a fake stats
	StatsList sl = { 0 }; // Create a fake stats list
	sl.ptStats = &st; // associate the fake stats to the fake stats list
	sl.StatCount = 1;	
	UnitAny unit = { 0 }; // Create a fake unit
	unit.ptStatsList = &sl; // Associate the fake stats list to the fake unit
	
	return GetItemReqRecursive(0, 500, pItem, &unit, &st, &dwStr, &dwDex, &dwLvl, pMatch);
}

DWORD GetItemLevelRequirement(DWORD dwItemID)
{
	return GetItemRequirement(dwItemID, STAT_LEVEL);
}

DWORD GetItemStrRequirement(DWORD dwItemID)
{
	return GetItemRequirement(dwItemID, STAT_STRENGTH);
}

DWORD GetItemDexRequirement(DWORD dwItemID)
{
	return GetItemRequirement(dwItemID, STAT_DEXTERITY);
}

BOOL GetItemDamage(DWORD dwItemID, DWORD* pMinDamage, DWORD* pMaxDamage)
{
	if (pMinDamage)
		*pMinDamage = 0;

	if (pMaxDamage)
		*pMaxDamage = 0;

	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	if (!VerifyUnit(&gu))
		return FALSE;

	DWORD dw1, dw2;
	if (pMinDamage)
	{
		dw1 = GetUnitStat(&gu, 21);
		dw2 = GetUnitStat(&gu, 23);
		*pMinDamage = max(dw1, dw2);
		//GameInfof("min %d", *pMinDamage);
	}

	if (pMaxDamage)
	{
		dw1 = GetUnitStat(&gu, 22);
		dw2 = GetUnitStat(&gu, 24);
		*pMaxDamage = max(dw1, dw2);
		//GameInfof("max %d", *pMaxDamage);
	}

	return TRUE;	
}

BOOL IsTwoHandItem(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return -1;

	return D2COMMON_TwoHandedItemCheck(pItem);
}

SIZE GetItemSize2(DWORD dwItemID)
{
	SIZE sz = { 0 };
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return sz;

	ItemBinData* p = D2COMMON_ItemGetBinFromClass(pItem->dwClassId);
	if (p == NULL)
		return sz;

	sz.cx = p->SizeX;
	sz.cy = p->SizeY;
	return sz;
}

int GetItemType(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return -1;

	ItemBinData* p = D2COMMON_ItemGetBinFromClass(pItem->dwClassId);
	if (p == NULL)
		return -1;

	return p->Type;
}

int GetItemImageIndex(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return -1;
	
	BYTE *pi = (BYTE*)pItem->ptItemData;
	return *(pi + 0x49);
}

void DisplayInfo(LPCSTR lpszInfo, BOOL bBottom)
{
	if (lpszInfo == NULL || lpszInfo[0] == 0 || D2CLIENT_PrintGameStringAtBottomLeft == NULL || D2CLIENT_PrintGameStringAtTopLeft == NULL || D2CLIENT_GetPlayerUnit() == NULL)
		return;

	wchar_t wsz[256] = { 0 };
	ConvertToUnicode(lpszInfo, wsz, 255);
	ReplaceColorSpecs(wsz);
	if (bBottom)
		D2CLIENT_PrintGameStringAtBottomLeft(wsz, 0);
	else
		D2CLIENT_PrintGameStringAtTopLeft(wsz, 0);
}

int ReplaceColorSpecs(LPWSTR lpwsz)
{
	// Prevent the "ÿc" symbol to be converted into wchar_t
	if (lpwsz == NULL)
		return 0;

	const int LEN = wcslen(lpwsz);
	int nCount = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xf8f5) // Unicode 'ÿ'
		{
			lpwsz[i] = 0xff; // Ansi 'ÿ'
			nCount++;		
		}
	}

	return nCount;
}


void DebugTest(char** argv, int argc)
{
}
