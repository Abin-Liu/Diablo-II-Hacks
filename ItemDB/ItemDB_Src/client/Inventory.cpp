///////////////////////////////////////////////////////////
// Inventory.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "Inventory.h"
#include "Constants.h"
#include "definitions.h"
#include "d2functions.h"
#include "CommonStructs.h"
#include "..\\Common\\ArrayEx.h"
#include "ItemCheck.h"
#include "Server20.h"

#define NODEPAGE_STORAGE	1
#define NODEPAGE_BELTSLOTS	2
#define NODEPAGE_EQUIP		3

BOOL FindStorageItem(DWORD dwItemID);

DWORD GetInventoryItemCount(LPCSTR lpszItemCode);
DWORD GetInventoryItemCharges(LPCSTR lpszItemCode);



BOOL StorageType2LocPage(int nStorageType, BYTE& rLoc, BYTE& rPage);
int LocPage2StorageType(BYTE nLoc, BYTE nPage);
BOOL IsItemEquipped(UnitItem* p);
BYTE GetItemEquipLocation(UnitItem* p);

BOOL StorageType2LocPage(int nStorageType, BYTE& rLoc, BYTE& rPage)
{
	switch (nStorageType)
	{
	case STORAGE_INVENTORY:
	case STORAGE_CUBE:
	case STORAGE_STASH:
		rLoc = (BYTE)nStorageType;
		rPage = NODEPAGE_STORAGE;
		break;

	case STORAGE_EQUIP:
		rLoc = 0xff;
		rPage = NODEPAGE_EQUIP;
		break;

	case STORAGE_BELTSLOT:
		rLoc = 0xff;
		rPage = NODEPAGE_BELTSLOTS;
		break;

	default:
		rLoc = 0xff;
		rPage = 0xff;
		break;
	}
	
	return rPage != 0xff;
}

int LocPage2StorageType(BYTE nLoc, BYTE nPage)
{
	//GameInfof("page %d, loc2 %d", nPage, nLoc);
	if (nPage == NODEPAGE_STORAGE)
		return nLoc;

	if (nLoc == 0xff)
		return nPage == NODEPAGE_EQUIP ? STORAGE_EQUIP : nPage;

	return STORAGE_INVALID;
}

BOOL EnumStorageItems(int nStorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam)
{
	if (lpfnEnumItemProc == NULL)
		return FALSE;
	
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return FALSE;

	char szCode[4] = "";
	BYTE iLoc2, iPage;
	
	if (nStorageType != STORAGE_EQUIP && !StorageType2LocPage(nStorageType, iLoc2, iPage))
		return FALSE;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData == NULL)
			continue;
		
		BOOL bMatch = FALSE;
		if (nStorageType == STORAGE_EQUIP)
			bMatch = IsItemEquipped(p);
		else
			bMatch = p->ptItemData->NodePage == iPage && p->ptItemData->ItemLocation2 == iLoc2;

		if (bMatch)
		{
			ItemTxt* ptxt = D2COMMON_GetItemTxt(p->dwClassId);
			if (ptxt == NULL || p->hOPath == NULL)
				continue;

			WORD x, y;
			if (nStorageType == STORAGE_EQUIP)
			{
				x = GetItemEquipLocation(p);
				y = 0;
			}
			else
			{
				x = (WORD)p->hOPath->Xpos;
				y = (WORD)p->hOPath->Ypos;
			}			
			
			::memcpy(szCode, ptxt->szCode, 3);
			szCode[3] = '\0';
			if (!lpfnEnumItemProc(p->dwId, szCode, x, y, lParam))
				return FALSE;
		}
	}

	return TRUE;
}

int GetItemLocationType(DWORD dwItemID)
{
	UnitItem* p = (UnitItem*)VerifyItem(dwItemID);
	if (p == NULL || p->ptItemData == NULL)
		return STORAGE_INVALID;

	if (IsItemEquipped(p))
		return GetItemEquipLocation(p);

	return LocPage2StorageType(p->ptItemData->ItemLocation2, p->ptItemData->NodePage);
}

DWORD GetEquipItem(int nEquipLocation)
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (IsItemEquipped(p) && (int)GetItemEquipLocation(p) == nEquipLocation)
			return p->dwId;
	}

	return 0; // not found
}

DWORD GetStorageItem(int nStorageType, int x, int y)
{
	if (nStorageType == STORAGE_EQUIP)
	{
		return GetEquipItem(x);
	}
	else if (nStorageType == STORAGE_BELTSLOT)
	{
		return 0;
	}
	else
	{
		CStorageBuffer sb((BYTE)nStorageType);
		return sb.GetItem(x, y);
	}	
}

BOOL FindStorageItem(DWORD dwItemID)
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return FALSE;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == NODEPAGE_STORAGE
			&& p->dwId == dwItemID
			&& (p->ptItemData->ItemLocation2 == STORAGE_INVENTORY || p->ptItemData->ItemLocation2 == STORAGE_CUBE || p->ptItemData->ItemLocation2 == STORAGE_STASH))
			return TRUE;
	}

	return FALSE; // not found
}

BOOL IsMyInventoryItem(DWORD dwItemID)
{
	if (dwItemID == NULL)
		return FALSE;
	
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return FALSE;

	DWORD dwCount = 0;
	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == NODEPAGE_STORAGE
			&& p->ptItemData->ItemLocation2 == STORAGE_INVENTORY)
		{
			if (p->dwId == dwItemID)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL IsItemEquipped(UnitItem* p)
{
	if (p == NULL || p->ptItemData == NULL)
		return FALSE;

	BYTE* ptr = (BYTE*)p->ptItemData;
	if (*(ptr + 69) != 0xff)
		return FALSE;

	ItemTxt* ptxt = D2COMMON_GetItemTxt(p->dwClassId);
	if (ptxt == NULL)
		return FALSE;

	char szCode[4] = "";
	::memcpy(szCode, ptxt->szCode, 3);
	return !D2IsBeltItem(szCode);
}

BYTE GetItemEquipLocation(UnitItem* p)
{
	if (p == NULL || p->ptItemData == NULL)
		return FALSE;

	BYTE* ptr = (BYTE*)p->ptItemData;
	return *(ptr + 68);
}