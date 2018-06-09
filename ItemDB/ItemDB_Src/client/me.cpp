//////////////////////////////////////////////////////////////////
// Me.cpp
//
// Implementation of the "me" API set functions
//
// Abin(abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////
#include "..\\Common\\ArrayEx.h"
#include "d2functions.h"
#include "definitions.h"
#include "Inventory.h"
#include "Server20.h"
#include "ItemCheck.h"

BOOL VerifyItem(DWORD dwItemID)
{
	if (dwItemID == 0)
		return NULL;

	UnitAny* p = D2CLIENT_GetUnit(dwItemID, UNIT_TYPE_ITEM);
	if (p == NULL || p->ptItemData == NULL)
		return NULL;

	return (BOOL)p;
}


DWORD GetStashGoldLimit() // stash gold limit
{
	return D2COMMON_GetStashGoldLimit(D2CLIENT_GetPlayerUnit());
}

DWORD GetInventoryGoldLimit() // inventory gold limit
{
	UnitAny* p = D2CLIENT_GetPlayerUnit();
	if (p == NULL)
		return 0;

	DWORD dwLevel = ServerGetUnitStat(p, STAT_LEVEL, FALSE);
	return dwLevel * 10000;
}

BOOL IsHardcore() // is hardcore player?
{
	BnetData BnData = **p_D2LAUNCH_bnData;
	return (BnData.CharFlags & 0x04) == 0x04;
}

DWORD GetCursorItem() // returns ID of the item on cursor, if any
{
	UnitAny* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;
	
	UnitAny* p = D2COMMON_GetCursorItem(pPlayer->ptInventory);
	return p ? p ->dwId : 0;
}

LPCSTR GetAccountName()
{
	return (**p_D2LAUNCH_bnData).AccountName;
}

BOOL IsWeaponSwitched()
{
	return (BOOL)*p_D2CLIENT_bWeapSwitch;
}

DWORD GetClassID()
{
	UnitAny* p = D2CLIENT_GetPlayerUnit();
	return p ? p->dwClassId : 0;
}


BOOL GetMyMercUnit(LPGAMEUNIT lpBuffer) // Get player's mercenary
{
	if (lpBuffer == NULL)
		return FALSE;
	
	::memset(lpBuffer, 0, sizeof(GAMEUNIT));
	UnitAny* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return FALSE;
	
	for (RosterPet* p = *p_D2CLIENT_FirstRosterPet; p; p = p->PetNext)
	{
		if (IsMercClassID(p->ClassId) && p->OwnerId == pPlayer->dwId)
		{
			lpBuffer->dwUnitID = p->Id;
			lpBuffer->dwUnitType = UNIT_TYPE_MONSTER;
		}
	}
	return lpBuffer->dwUnitID;
}

DWORD GetMercEquip(int nEquipLocation)
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

