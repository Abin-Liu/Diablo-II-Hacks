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
#include "D2Hackit.h"
#include "me.h"
#include "D2HackIt.h"
#include "..\\Includes\\Utilities\\ArrayEx.h"
#include "ItemCheck.h"
#include "Server20.h"

#define NODEPAGE_STORAGE	1
#define NODEPAGE_BELTSLOTS	2
#define NODEPAGE_EQUIP		3

BYTE EXPORT GetBeltRows() // Row number of the player's belt(1-4)
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 1;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData && p->ptItemData->NodePage == NODEPAGE_EQUIP && p->ptItemData->ItemLocation == 8)
		{
			char szItemCode[4] = "";
			GetItemCode(p->dwId, szItemCode, 3);
			return max(1, D2IsBelt(szItemCode));		
		}
	}

	return 1;
}

BOOL EXPORT EnumStorageItems(int nSTorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam)
{
	if (lpfnEnumItemProc == NULL)
		return FALSE;
	
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return FALSE;

	char szCode[4] = "";

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData && p->ptItemData->NodePage == NODEPAGE_STORAGE && p->ptItemData->ItemLocation2 == (BYTE)nSTorageType)
		{
			ItemTxt* ptxt = D2COMMON_GetItemTxt(p->dwClassId);
			if (ptxt == NULL || p->hOPath == NULL)
				continue;
			
			::memcpy(szCode, ptxt->szCode, 3);
			szCode[3] = '\0';
			if (!lpfnEnumItemProc(p->dwId, szCode, (WORD)p->hOPath->Xpos, (WORD)p->hOPath->Ypos, lParam))
				return FALSE;
		}
	}

	return TRUE;
}

DWORD EXPORT GetBeltItem(int nSlot) // Belt item access
{
	// slot limit: belt_row * 4 - 1
	int nBeltRows = GetBeltRows();
	if (nSlot >= nBeltRows * 4)
		return -1; // invalid slot

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return -1;

	UnitItem* p = D2COMMON_GetBeltItem(pPlayer->ptInventory, nSlot);
	if (p == NULL)
		return 0; // slot is empty

	return p->dwId;
}

DWORD EXPORT GetEquipItem(int nEquipLocation)
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData && p->ptItemData->NodePage == NODEPAGE_EQUIP && p->ptItemData->ItemLocation == (BYTE)nEquipLocation)
			return p->dwId;
	}

	return 0; // not found
}

DWORD EXPORT GetStorageItem(int nStorageType, int x, int y)
{
	CStorageBuffer sb((BYTE)nStorageType);
	return sb.GetItem(x, y);
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

int FindEquipItem(DWORD dwItemID)
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData && p->ptItemData->NodePage == NODEPAGE_EQUIP && p->dwId == dwItemID)
			return p->ptItemData->ItemLocation;
	}

	return 0; // not found
}

BOOL EXPORT PickStorageItemToCursor(DWORD dwItemID)
{
	if (!FindStorageItem(dwItemID))
		return FALSE;

	BYTE aPacket[5]= { 0 };
	aPacket[0] = 0x19;
	::memcpy(aPacket + 1, &dwItemID, 4);
	return GameSendPacketToServer(aPacket, 5);
}

DWORD FindFirstBeltPotion(LPCSTR lpszCode, BOOL bKeepBottomRow)
{
	if (lpszCode == NULL)
		return 0;

	const int LEN = strlen(lpszCode);
	if (LEN == 0)
		return 0;

	int nStart = bKeepBottomRow ? 4 : 0;
	for (int i = 15; i >= nStart; i--)
	{
		DWORD dwItemID = GetBeltItem(i);

		if (dwItemID <= 0)
			continue;

		char szCode[4] = "";
		GetItemCode(dwItemID, szCode, 3);
		if (LEN == 2 && (szCode[2] < '1' || szCode[2] > '5'))
			continue;

		if (!::memcmp(szCode, lpszCode, LEN))
			return dwItemID;
	}

	return 0;
}

DWORD FindFirstInventoryPotion(LPCSTR lpszCode)
{
	if (lpszCode == NULL)
		return 0;

	const int LEN = strlen(lpszCode);
	if (LEN == 0)
		return 0;

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData && p->ptItemData->NodePage == NODEPAGE_STORAGE && p->ptItemData->ItemLocation2 == STORAGE_INVENTORY)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);			
			if (LEN == 2 && (szCode[2] < '1' || szCode[2] > '5'))
				continue;

			//GameInfof(szCode);
			if (!::memcmp(szCode, lpszCode, LEN))
				return p->dwId;
		}
	}

	return 0; // not found
}

BOOL EXPORT FindFirstStorageSpace(int nStorageType, SIZE minSize, LPPOINT lpBuffer)
{
	CStorageBuffer sb((BYTE)nStorageType);
	return sb.GetBestFit(minSize.cx, minSize.cy, lpBuffer);
}

BOOL EXPORT DropCursorItemToStorage(int nStorageType)
{
	DWORD dwItemID = GetCursorItem();
	if (dwItemID == 0)
		return FALSE;

	char szCode[4] = "";
	if (!GetItemCode(dwItemID, szCode, 3))
		return FALSE;	

	SIZE cs = GetItemSize2(dwItemID);
	if (cs.cx == 0 || cs.cy == 0)
		return FALSE;

	POINT pt;

	if (!FindFirstStorageSpace(nStorageType, cs, &pt))
		return FALSE; // no space
	
	if (nStorageType == STORAGE_CUBE)
	{
		if (!::stricmp(szCode, "box"))
			return FALSE; // cannot put cube into cube

		DWORD dwCubeID = CStorageBuffer::GetCubeID();
		if (dwCubeID == 0)
			return FALSE; // cannot put anything into cube if doesn't even have a cube

		BYTE aPacket[9] = { 0 };
		aPacket[0] = 0x2a;
		::memcpy(aPacket + 1, &dwItemID, 4);
		::memcpy(aPacket + 5, &dwCubeID, 4);
		return GameSendPacketToServer(aPacket, 9);
	}

	if (nStorageType == STORAGE_INVENTORY || nStorageType == STORAGE_STASH)
	{
		BYTE aPacket[17] = { 0 };
		aPacket[0] = 0x18;
		::memcpy(aPacket + 1, &dwItemID, 4);
		::memcpy(aPacket + 5, &pt.x, 4);
		::memcpy(aPacket + 9, &pt.y, 4);
		aPacket[13] = nStorageType == STORAGE_INVENTORY ? 0x00 : 0x04;
		return GameSendPacketToServer(aPacket, 17);
	}

	return FALSE;	
}

int EXPORT GetUnidItemCount()
{
	UnitPlayer* p = D2CLIENT_GetPlayerUnit();
	if (!p)
		return 0;

	return D2COMMON_GetNumberOfUnidItems(p);
}

int EXPORT GetAvailableBeltSlots(int nColumn)
{
	if (nColumn < 0 || nColumn > 3)
		return 0;

	int nRows = GetBeltRows();

	int nCount = 0;
	for (int i = nColumn; i < nRows * 4; i += 4)
	{
		if (GetBeltItem(i) == 0)
			nCount++;
	}

	return nCount;
}

int EXPORT DrinkAllInventoryPotions(BOOL bKeepFullRejuv, DWORD dwDelay /*= 500*/)
{
	CArrayEx<DWORD, DWORD> aPotions;

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == NODEPAGE_STORAGE
			&& p->ptItemData->ItemLocation2 == STORAGE_INVENTORY
			&& aPotions.Find(p->dwId) == -1)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (D2IsPotion(szCode))
			{
				if (!bKeepFullRejuv || stricmp(szCode, "rvl"))
					aPotions.Add(p->dwId);
			}
		}
	}

	if (aPotions.IsEmpty())
		return 0;

	POINT mp = GetPosition();
	BYTE aPacket[13] = {0};
	aPacket[0] = 0x20;
	::memcpy(aPacket + 5, &mp.x, 2);
	::memcpy(aPacket + 9, &mp.y, 2);

	int nCount = 0;
	for (int i = 0; i < aPotions.GetSize(); i++)
	{
		::memcpy(aPacket + 1, &aPotions[i], 4);
		GameSendPacketToServer2(aPacket, 13, dwDelay);
	}

	return aPotions.GetSize();
}

DWORD GetInventoryItemCount(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return 0;

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	DWORD dwCount = 0;
	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == NODEPAGE_STORAGE
			&& p->ptItemData->ItemLocation2 == STORAGE_INVENTORY)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (!::stricmp(szCode, lpszItemCode))
				dwCount++;
		}
	}

	return dwCount;
}

DWORD GetInventoryItemCharges(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return 0;

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	DWORD dwCount = 0;
	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == NODEPAGE_STORAGE
			&& p->ptItemData->ItemLocation2 == STORAGE_INVENTORY)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (!::stricmp(szCode, lpszItemCode))
				dwCount += ServerGetUnitStat(p, STAT_AMMOQUANTITY, FALSE);
		}
	}

	return dwCount;
}

DWORD GetInventoryIDScrolls()
{
	return GetInventoryItemCount("isc");
}

DWORD GetInventoryTPScrolls()
{
	return GetInventoryItemCount("tsc");
}

DWORD GetInventoryIDBookCharges()
{
	return GetInventoryItemCharges("ibk");
}

DWORD GetInventoryTPBookCharges()
{
	return GetInventoryItemCharges("tbk");
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

int GetAvailableColumnFor(int nPotionType)
{
	if (nPotionType != POTION_RED && nPotionType != POTION_BLUE && nPotionType != POTION_PURPLE)
		return -1;

	for (int i = 0; i < 4; i++)
	{
		DWORD dwID = GetBeltItem(i);
		if (dwID == 0)
			return i;

		if (GetAvailableBeltSlots(i) == 0)
			continue;

		// check first item
		char szCode[4] = "";
		GetItemCode(dwID, szCode, 3);
		if (D2IsPotion(szCode) == nPotionType)
			return i;
	}

	return -1;
}

DWORD FindFirstTPScroll()
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == NODEPAGE_STORAGE
			&& p->ptItemData->ItemLocation2 == STORAGE_INVENTORY)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (!::stricmp(szCode, "tsc"))
				return p->dwId;
		}
	}

	return 0;
}

DWORD FindFirstUseableTPBook()
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == NODEPAGE_STORAGE
			&& p->ptItemData->ItemLocation2 == STORAGE_INVENTORY)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (!::stricmp(szCode, "tbk") && ServerGetUnitStat(p, STAT_AMMOQUANTITY, FALSE) > 0)
				return p->dwId;
		}
	}

	return 0;
}