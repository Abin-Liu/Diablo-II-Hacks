/////////////////////////////////////////////////////////////////
// Client.cpp
//
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////

#include "Client.h"
#include "..\\Common\\ItemEventDef.h"
#include "..\\common\\LinkedList.h"
#include "Server20.h"

DWORD g_dwTickCount = 0;
HANDLE g_hFile = NULL;
char g_szFile[1024] = "";
CHARINFO g_char = { 0 };
BOOL g_bGatherOK = FALSE;
BOOL g_bGameReady = FALSE;
const UnitPlayer* g_pPlayer = NULL;
BOOL g_bFirstGather = FALSE;

void CALLBACK FreeItemInfo(LPITEMINFO& pData);
CLinkedList<LPITEMINFO, LPITEMINFO, FreeItemInfo> g_aItems;

BOOL GatherData();
HWND SaveData();
void AddItem(DWORD dwItemID, int nStorageType, int x, int y);

BOOL CALLBACK EnumItemProc(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam);

void OnGameJoin(const UnitPlayer* pPlayer)
{
	DisplayInfo("ÿc9ItemDB Installed.", FALSE);
	g_pPlayer = pPlayer;
	g_dwTickCount = 0;
	g_bGatherOK = FALSE;
	g_bGameReady = FALSE;
	g_bFirstGather = FALSE;
	
	::memset(&g_char, 0, sizeof(CHARINFO));
	LPCSTR p = GetRealmName();
	if (p)
		::strcpy(g_char.szRealm, p);

	p = GetAccountName();
	if (p)
		::strcpy(g_char.szAccount, p);

	::strcpy(g_char.szCharName, pPlayer->ptPlayerData->PlayerName);
	g_char.iCharClass = (BYTE)pPlayer->dwClassId;
	g_char.iHardcore = (BYTE)IsHardcore();

	if (pPlayer->ptPlayerData && pPlayer->wX && pPlayer->wY)
		GatherData();
}

void OnGameLeave()
{
	g_pPlayer = NULL;
	HWND hServerWnd = SaveData();
	if (hServerWnd)
		::PostMessage(hServerWnd, WM_ITEMDB_CLIENT_UPDATE, (WPARAM)WM_ITEMDB_IDENTIFIER, (LPARAM)g_hD2Wnd);
}

void OnGameTimerTick()
{
	g_dwTickCount++;
	BOOL bJustReady = FALSE;
	if (!g_bGameReady)
	{
		g_bGameReady = g_pPlayer && g_pPlayer->ptPlayerData && g_pPlayer->wX && g_pPlayer->wY;
		if (g_bGameReady)
		{
			bJustReady = TRUE;
			GatherData();
		}
	}

	if (!bJustReady && (g_dwTickCount % 10) == 0) // gathers data once every 5 seconds
		GatherData();
}

void CALLBACK FreeItemInfo(LPITEMINFO& pData)
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

BOOL GatherData()
{
	g_char.dwGoldLimit = GetStashGoldLimit();
	g_char.dwInvGold = GetStat(STAT_GOLD);
	g_char.dwStashGold = GetStat(STAT_GOLDBANK);
	g_char.iActiveSwitch = (BYTE)IsWeaponSwitched();
	
	GAMEUNIT gu;
	if (GetMyMercUnit(&gu))
	{
		g_char.wMercClass = (WORD)GetUnitClassID(&gu);
		GetUnitName(&gu, g_char.szMercName, 31);
	}

	g_aItems.Lock();
	
	g_aItems.RemoveAll();
	DWORD dwCursor = GetCursorItem();
	if (dwCursor)
		AddItem(dwCursor, STO_CURSOR, 0, 0);
	EnumStorageItems(STO_EQUIP, EnumItemProc, STO_EQUIP);
	EnumStorageItems(STO_INVENTORY, EnumItemProc, STO_INVENTORY);
	EnumStorageItems(STO_STASH, EnumItemProc, STO_STASH);
	EnumStorageItems(STO_CUBE, EnumItemProc, STO_CUBE);

	DWORD dwID = GetMercEquip(EQUIP_HEAD);
	if (dwID)
		AddItem(dwID, STO_MERC, EQUIP_HEAD, 0);

	dwID = GetMercEquip(EQUIP_BODY);
	if (dwID)
		AddItem(dwID, STO_MERC, EQUIP_BODY, 0);

	dwID = GetMercEquip(EQUIP_RIGHT_PRIMARY);
	if (dwID)
		AddItem(dwID, STO_MERC, EQUIP_RIGHT_PRIMARY, 0);

	g_aItems.Unlock();
	g_bGatherOK = TRUE;
	
	if (!g_bFirstGather)
		DisplayInfo("ÿc2ItemDB Data Gathered Successfully.", FALSE);
	//else
	//	DisplayInfo("ÿc2.", TRUE);

	g_bFirstGather = TRUE;
	return TRUE;
}

HWND SaveData()
{
	if (!g_bGatherOK || g_hD2Wnd == NULL)
		return NULL;

	if (g_hFile)
	{
		//GamePrintError("ÿc1A previous update request has not been processed yet!");
		return NULL;
	}

	HWND hServerWnd = ::FindWindow(ITEMDB_WND_SERVER, NULL);
	if (!hServerWnd)
	{
		//GamePrintError("ÿc1Database application handle not found!");
		return NULL;
	}
	
	g_szFile[0] = 0;
	::GetWindowText(hServerWnd, g_szFile, 1000);
	::strcat(g_szFile, "\\Cache");
	::CreateDirectory(g_szFile, NULL);

	::sprintf(g_szFile + ::strlen(g_szFile), "\\ItemDB_%08X.tmp", (DWORD)g_hD2Wnd);

	g_hFile = ::CreateFile(g_szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (g_hFile == INVALID_HANDLE_VALUE)
	{
		//GamePrintError("ÿc1The database application cannot process this request now!");
		return NULL;
	}

	DWORD dwDummy;
	::WriteFile(g_hFile, &g_char, sizeof(g_char), &dwDummy, NULL);

	g_aItems.Lock();
	POSITION pos = g_aItems.GetHeadPosition();
	while (pos)
	{
		LPCITEMINFO p = g_aItems.GetNext(pos);
		if (p == NULL)
			continue;

		::WriteFile(g_hFile, p, sizeof(ITEMINFO), &dwDummy, NULL);
	}

	g_aItems.Unlock();

	::CloseHandle(g_hFile);
	g_hFile = NULL;
	g_bGatherOK = FALSE;

	return hServerWnd;
}

BOOL CALLBACK EnumItemProc(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam)
{
	AddItem(dwItemID, lParam, x, y);
	return TRUE;
}

void AddItem(DWORD dwItemID, int nStorageType, int x, int y)
{
	LPITEMINFO p = new ITEMINFO;
	if (p == NULL)
		return;
	::memset(p, 0, sizeof(ITEMINFO));
	
	p->iStorageType = (BYTE)nStorageType;
	p->x = (BYTE)x;
	p->y = (BYTE)y;
	SIZE cz = GetItemSize2(dwItemID);
	p->cx = (BYTE)cz.cx;
	p->cy = (BYTE)cz.cy;
	p->iQuality = GetItemQuality(dwItemID);
	p->iSockets = GetItemSockets(dwItemID);
	p->iLevel = GetItemLevel(dwItemID);
	p->iType = GetItemType(dwItemID);
	p->iArtwork = GetItemImageIndex(dwItemID);
	p->dwFlags = GetItemAttributes(dwItemID);
	p->wStrReq = (WORD)GetItemStrRequirement(dwItemID);
	p->wDexReq = (WORD)GetItemDexRequirement(dwItemID);
	p->iLevelreq = (BYTE)GetItemLevelRequirement(dwItemID);
	p->i2Hand = (BYTE)IsTwoHandItem(dwItemID);
	DWORD dwMinDmg = 0, dwMaxDmg = 0;
	GetItemDamage(dwItemID, &dwMinDmg, &dwMaxDmg);
	p->wMinDamage = (WORD)dwMinDmg;
	p->wMaxDamage = (WORD)dwMaxDmg;
	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	p->wdefense = (WORD)GetUnitStat(&gu, 31);
	p->wMinDura = (WORD)GetUnitStat(&gu, STAT_DURABILITY);
	p->wMaxDura = (WORD)GetUnitStat(&gu, STAT_MAXDURABILITY);
	p->wQuantity = (WORD)GetUnitStat(&gu, STAT_AMMOQUANTITY);
	p->dwValue = GetItemPrice(dwItemID, 513, FALSE);	
	GetItemCode(dwItemID, p->szCode, 3);	
	GetItemDescription(dwItemID, p->szName, 127, FALSE);

	ITEMMOD aTempMods[ITEM_MAX_MODCOUNT];
	p->iModCount = (BYTE)GetItemProperties2(dwItemID, aTempMods, ITEM_MAX_MODCOUNT);
	for (BYTE i = 0; i < p->iModCount; i++)
	{
		p->aMods[i].dwModID = aTempMods[i].dwModID;
		::memcpy(p->aMods[i].aValues, aTempMods[i].aValues, sizeof(int) * 4);
		if (aTempMods[i].dwModID == 152) // indestructible
		{
			p->wMinDura = -1;
			p->wMaxDura = -1;
		}
	}

	g_aItems.InsertRear(p);
}