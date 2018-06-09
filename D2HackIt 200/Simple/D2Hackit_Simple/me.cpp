//////////////////////////////////////////////////////////////////
// Me.cpp
//
// Implementation of the "me" API set functions
//
// Abin(abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////
#include "me.h"
#include "utilities\\ArrayEx.h"
#include "d2functions.h"
#include "definitions.h"
#include "D2HackIt.h"
#include "Inventory.h"
#include "Server20.h"
#include "ItemCheck.h"

THISGAMESTRUCT* thisgame = NULL;
DWORD g_dwPlayerID = 0; // Player ID(before OnGameJoin is called)
char g_szPlayerName[32] = ""; // Player name(before OnGameJoin is called)
BOOL g_bGameReady = FALSE; // Game ready flag

VOID Player_OnGameJoin(THISGAMESTRUCT* thisgame)
{	

}

VOID Player_OnGameLeave()
{

}

void CALLBACK PlayerTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag)
{

}

void MakePlayerData(LPGAMEUNIT lpBuffer)
{
	lpBuffer->dwUnitID = g_dwPlayerID;
	lpBuffer->dwUnitType = UNIT_TYPE_PLAYER;
}

BOOL EXPORT IsGameReady()
{
	return 0;
}

BOOL VerifyItem(DWORD dwItemID)
{
	if (dwItemID == 0)
		return NULL;

	UnitAny* p = D2CLIENT_GetUnit(dwItemID, UNIT_TYPE_ITEM);
	if (p == NULL || p->ptItemData == NULL)
		return NULL;

	return (BOOL)p;
}


DWORD EXPORT GetStashGoldLimit() // stash gold limit
{
	return D2COMMON_GetStashGoldLimit(D2CLIENT_GetPlayerUnit());
}

DWORD EXPORT GetInventoryGoldLimit() // inventory gold limit
{
	GAMEUNIT unit;
	MakePlayerData(&unit);
	DWORD dwLevel = GetUnitStat(&unit, STAT_LEVEL);
	return dwLevel * 10000;
}

BOOL EXPORT IsHardcore() // is hardcore player?
{
	BnetData BnData = **p_D2LAUNCH_bnData;
	return (BnData.CharFlags & 0x04) == 0x04;
}

DWORD EXPORT GetCursorItem() // returns ID of the item on cursor, if any
{
	UnitAny* p = D2COMMON_GetCursorItem(D2CLIENT_GetPlayerUnit()->ptInventory);
	return p ? p ->dwId : 0;
}

LPCSTR EXPORT GetAccountName()
{
	return (**p_D2LAUNCH_bnData).AccountName;
}

BOOL EXPORT IsWeaponSwitched()
{
	return (BOOL)*p_D2CLIENT_bWeapSwitch;
}

DWORD EXPORT GetClassID()
{
	UnitAny* p = D2CLIENT_GetPlayerUnit();
	return p ? p->dwClassId : 0;
}


BOOL EXPORT GetMyMercUnit(LPGAMEUNIT lpBuffer) // Get player's mercenary
{
	if (lpBuffer == NULL)
		return FALSE;
	
	::memset(lpBuffer, 0, sizeof(GAMEUNIT));
	
	for (RosterPet* p = *p_D2CLIENT_FirstRosterPet; p; p = p->PetNext)
	{
		if (IsMercClassID(p->ClassId) && p->OwnerId == 0)
		{
			lpBuffer->dwUnitID = p->Id;
			lpBuffer->dwUnitType = UNIT_TYPE_MONSTER;
		}
	}
	return lpBuffer->dwUnitID;
}

DWORD EXPORT GetMercEquip(int nEquipLocation)
{
	if (nEquipLocation != EQUIP_HEAD && nEquipLocation != EQUIP_BODY && nEquipLocation != EQUIP_RIGHT_PRIMARY)
		return 0;

	GAMEUNIT gu;
	if (!GetMyMercUnit(&gu))
		return 0;

	UnitAny* pUnit = (UnitAny*)VerifyUnit(&gu);
	if (pUnit == NULL || pUnit->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pUnit->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData == NULL || p->hOPath == NULL || p->hOPath->Xpos != (WORD)nEquipLocation)
			continue;

		return p->dwId;
	}

	return 0;
}

