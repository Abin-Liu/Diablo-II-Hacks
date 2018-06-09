//////////////////////////////////////////////////////////////
// D2MiscItem.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////

#include "D2MiscItem.h"
#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"

static const char GOLD[] = "gld";
static const char PURPLES[] = "rvs,rvl";
static const char REDS[] = "hp1,hp2,hp3,hp4,hp5";
static const char BLUES[] = "mp1,mp2,mp3,mp4,mp5";
static const char ANTIDOTE[] = "yps";
static const char STAMINA[] = "vps";
static const char THAWING[] = "wms";
static const char TOWNSCR[] = "tsc";
static const char IDSCR[] = "isc";
static const LPCSTR MISCITEMS[] = { GOLD, PURPLES, REDS, BLUES, ANTIDOTE, STAMINA, THAWING, TOWNSCR, IDSCR };

void CopyItemCode(char szBuffer[4], LPCSTR lpszSrc);
BOOL CheckBeltStats(DWORD dwType, BOOL bMustMatch);
BOOL FindNonFullTome(DWORD dwType);
int CheckMiscItemNeeded(DWORD dwType);
BOOL GoldNeeded(DWORD dwAmount, DWORD dwSafetyFactor = 3000);
BOOL CALLBACK EnumNonEmptyTomeProc(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam);

void CopyItemCode(char szBuffer[4], LPCSTR lpszSrc)
{
	for (int i = 0; i < 3; i++)
		szBuffer[i] = ::tolower(lpszSrc[i]);
	szBuffer[3] = 0;
}

BOOL CheckBeltStats(DWORD dwType, BOOL bMustMatch)
{	
	for (int i = 0; i < 4; i++)
	{
		char szCode[4] = "";
		server->GetItemCode(me->GetBeltItem(i), szCode, 3);
		DWORD dwBelt = GetMiscType(szCode);
		if ((!bMustMatch && dwBelt == 0)
			|| (dwBelt == dwType && me->GetAvailableBeltSlots((BYTE)i)))
			return TRUE;
	}

	return FALSE;
}

DWORD GetMiscType(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL || ::strlen(lpszItemCode) != 3)
		return 0;

	char szCode[4];
	CopyItemCode(szCode, lpszItemCode);

	for (int i = 0; MISCITEMS[i]; i++)
	{
		if (::strstr(MISCITEMS[i], szCode))
			return (DWORD)(MISCITEMS[i]);
	}
	return 0;
}

int CheckMiscItemNeeded(DWORD dwType)
{
	if (dwType == 0)
		return MISC_NOT_MISC;
	
	BOOL bNeeded = FALSE;
	if (dwType == (DWORD)PURPLES || dwType == (DWORD)REDS || dwType == (DWORD)BLUES)
	{
		// normal potions
		bNeeded = CheckBeltStats(dwType, FALSE);
	}
	else if (dwType == (DWORD)ANTIDOTE || dwType == (DWORD)STAMINA || dwType == (DWORD)THAWING)
	{
		// special potions
		bNeeded = CheckBeltStats(dwType, TRUE);
	}
	else if (dwType == (DWORD)TOWNSCR || dwType == (DWORD)IDSCR)
	{
		// town scroll/id scroll
		// first check belt, then check inventory tome
		bNeeded = CheckBeltStats(dwType, TRUE) || FindNonFullTome(dwType);
	}
	else
	{
		return MISC_NOT_MISC;
	}

	return bNeeded ? MISC_NEED : MISC_NOT_NEED;
}

int MiscItemNeeded(DWORD dwType, DWORD dwGoldAmount)
{
	if (dwType == 0)
		return MISC_NOT_MISC;

	if (dwType == (DWORD)GOLD && dwGoldAmount)
		return GoldNeeded(dwGoldAmount) ? MISC_NEED : MISC_NOT_NEED;

	return CheckMiscItemNeeded(dwType);
}

BOOL GoldNeeded(DWORD dwAmount, DWORD dwSafetyFactor)
{
	return dwAmount + dwSafetyFactor < me->GetInventoryGoldLimit() - me->GetStat(STAT_GOLD);
}

BOOL FindNonFullTome(DWORD dwType)
{
	return !me->EnumStorageItems(STORAGE_INVENTORY, EnumNonEmptyTomeProc, (LPARAM)dwType);
}

BOOL CALLBACK EnumNonEmptyTomeProc(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam)
{
	GAMEUNIT unit = { dwItemID, UNIT_TYPE_ITEM };
	if ((lParam == (LPARAM)TOWNSCR && !::stricmp(lpszItemCode, "tbk"))
		|| (lParam == (LPARAM)IDSCR && !::stricmp(lpszItemCode, "ibk")))
	{
		if (server->GetUnitStat(&unit, STAT_AMMOQUANTITY) < 20)
			return FALSE;
	}
	
	return TRUE;
}

BOOL IsGold(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return FALSE;

	return ::stricmp(lpszItemCode, "gld") == 0;
}


