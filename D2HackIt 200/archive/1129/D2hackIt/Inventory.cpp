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

#define INV_ROW		10
#define INV_COL		4
#define STASH_ROW	6
#define STASH_COL	8
#define CUBE_ROW	3
#define CUBE_COL	4

#define ITEM_INVALID	-1

#define LOCATION2_INV		0
#define LOCATION2_CUBE		3
#define LOCATION2_STASH		4

#define NODEPAGE_STORAGE	1
#define NODEPAGE_BELTSLOTS	2
#define NODEPAGE_EQUIP		3

CStorageBuffer::CStorageBuffer(BYTE iType)
{
	m_iType = iType;
	SIZE sz = { 0 };
	switch (iType)
	{
	case LOCATION2_INV:
		sz.cx = INV_ROW;
		sz.cy = INV_COL;
		break;

	case LOCATION2_CUBE:
		sz.cx = CUBE_ROW;
		sz.cy = CUBE_COL;
		break;

	case LOCATION2_STASH:
		sz.cx = STASH_ROW;
		sz.cy = STASH_COL;
		break;

	default:
		break;
	}

	if (!m_spaces.Create(sz.cx, sz.cy, 0))
		return;

	// now build up the storage
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData && p->ptItemData->NodePage == NODEPAGE_STORAGE && p->ptItemData->ItemLocation2 == m_iType)
		{
			char szItemCode[4] = "";
			GetItemCode(p->dwId, szItemCode, 3);
			SIZE cz = GetItemSize(szItemCode);
			int x = (int)p->hOPath->Xpos;
			int y = (int)p->hOPath->Ypos;
			AddItem(p->dwId, x, y, cz.cx, cz.cy);
		}
	}
}

CStorageBuffer::~CStorageBuffer()
{
}

BOOL CStorageBuffer::AddItem(DWORD dwItemID, int x, int y, int cx, int cy)
{
	if (dwItemID == 0 || !IsValidLocationSize(x, y, cx, cy))
		return FALSE;

	RemoveItem(dwItemID);
	for (int i = 0; i < cx; i++)
	{
		for (int j = 0; j < cy; j++)
			m_spaces.SetAt(i + x, j + y, dwItemID);
	}

	return TRUE;
}

BOOL CStorageBuffer::IsValid() const
{
	return m_spaces.IsCreated();
}

BOOL CStorageBuffer::RemoveItem(DWORD dwItemID)
{
	if (!IsValid() || dwItemID == 0)
		return FALSE;

	int nCount = 0;
	for (int i = 0; i < m_spaces.GetCX(); i++)
	{
		for (int j = 0; j < m_spaces.GetCX(); j++)
		{
			if (m_spaces[i][j] == dwItemID)
			{
				m_spaces[i][j] = 0;
				nCount++;
			}
		}
	}

	return nCount;
}

BOOL CStorageBuffer::IsValidLocationSize(int x, int y, int cx, int cy) const
{
	if (!IsValid() || cx < 1 || cy < 1)
		return FALSE;

	return m_spaces.IsValidIndex(x, y) &&  m_spaces.IsValidIndex(x + cx - 1, y + cy - 1);
}

DWORD CStorageBuffer::GetItem(int x, int y) const
{
	return IsValidLocationSize(x, y, 1, 1) ? m_spaces.GetAt(x, y) : ITEM_INVALID;
}

BYTE CStorageBuffer::GetType() const
{
	return m_iType;
}

BOOL CStorageBuffer::Dump(LPCSTR lpszFilePath) const
{
	if (!IsValid())
		return FALSE;

	FILE* file = fopen(lpszFilePath, "w+");
	if (file == NULL)
		return FALSE;

	for (int j = 0; j < m_spaces.GetCY(); j++)
	{		
		for (int i = 0; i < m_spaces.GetCX(); i++)
			fprintf(file, "%d ", !!m_spaces[i][j]);
		
		fprintf(file, "\n");
	}

	fclose(file);
	return TRUE;
}

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
			if (!lpfnEnumItemProc(p->dwId, szCode, p->hOPath->Xpos, p->hOPath->Ypos, lParam))
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
			&& (p->ptItemData->ItemLocation2 == LOCATION2_INV || p->ptItemData->ItemLocation2 == LOCATION2_CUBE || p->ptItemData->ItemLocation2 == LOCATION2_STASH))
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
		if (p->ptItemData && p->ptItemData->NodePage == NODEPAGE_STORAGE && p->ptItemData->ItemLocation2 == LOCATION2_INV)
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

DWORD GetCubeID()
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == NODEPAGE_STORAGE
			&& (p->ptItemData->ItemLocation2 == LOCATION2_INV || p->ptItemData->ItemLocation2 == LOCATION2_STASH))
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (!::stricmp(szCode, "box"))
				return p->dwId;
		}
	}

	return 0;
}

BOOL EXPORT DropCursorItemToStorage(int nStorageType)
{
	DWORD dwItemID = GetCursorItem();
	if (dwItemID == 0)
		return FALSE;

	char szCode[4] = "";
	if (!GetItemCode(dwItemID, szCode, 3))
		return FALSE;	

	SIZE cs = GetItemSize(szCode);
	if (cs.cx == 0 || cs.cy == 0)
		return FALSE;

	POINT pt;

	if (!FindFirstStorageSpace(nStorageType, cs, &pt))
		return FALSE; // no space
	
	if (nStorageType == STORAGE_CUBE)
	{
		if (!::stricmp(szCode, "box"))
			return FALSE; // cannot put cube into cube

		DWORD dwCubeID = GetCubeID();
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
			&& p->ptItemData->ItemLocation2 == LOCATION2_INV
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
			&& p->ptItemData->ItemLocation2 == LOCATION2_INV)
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
			&& p->ptItemData->ItemLocation2 == LOCATION2_INV)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (!::stricmp(szCode, lpszItemCode))
				dwCount += D2COMMON_GetBaseStat(p, STAT_AMMOQUANTITY, 0);
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
			&& p->ptItemData->ItemLocation2 == LOCATION2_INV)
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
			&& p->ptItemData->ItemLocation2 == LOCATION2_INV)
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
			&& p->ptItemData->ItemLocation2 == LOCATION2_INV)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (!::stricmp(szCode, "tbk") && D2COMMON_GetBaseStat(p, STAT_AMMOQUANTITY, 0) > 0)
				return p->dwId;
		}
	}

	return 0;
}

BOOL CStorageBuffer::GetAdjacent(int x, int y, int cx, int cy, int& rAdj, int& rIntens) const
{
	// the "adjacent spaces" theroy for storage optimization, idea from netterhaufen
	rAdj = 0;
	rIntens = 0;

	if (!m_spaces.IsCreated() || cx < 1 || cy < 1)
		return FALSE;

	int i = 0, j = 0;
	for (i = x; i < x + cx; i++)
	{
		for (j = y; j < y + cy; j++)
		{
			if (!m_spaces.IsValidIndex(i, j) || m_spaces.GetAt(i, j))
				return FALSE; // cannot put in
		}
	}

	// check edges 1 (left)
	i = x - 1;
	for (j = y; j < y + cy; j++)
	{
		if (m_spaces.IsValidIndex(i, j) && m_spaces.GetAt(i, j) == 0)
			rAdj++;
	}

	// check edges 2 (right)
	i = x + cx;
	for (j = y; j < y + cy; j++)
	{
		if (m_spaces.IsValidIndex(i, j) && m_spaces.GetAt(i, j) == 0)
			rAdj++;
	}

	// check edges 3 (top)
	j = y - 1;
	for (i = x; i < x + cx; i++)
	{
		if (m_spaces.IsValidIndex(i, j) && m_spaces.GetAt(i, j) == 0)
			rAdj++;
	}

	// check edges 4 (bottom)
	j = y + cy;
	for (i = x; i < x + cx; i++)
	{
		if (m_spaces.IsValidIndex(i, j) && m_spaces.GetAt(i, j) == 0)
			rAdj++;
	}

	// calculate intensification
	rIntens = 1;
	for (i = 0; i < m_spaces.GetCX(); i++)
	{
		int nCount = 0;
		for (j = 0; j < m_spaces.GetCY(); j++)
		{
			if (m_spaces.GetAt(i, j) || (i >= x && i < x + cx && j >= y && j < y + cy))
				nCount++;
		}

		rIntens *= (nCount + 1);
	}

	return TRUE;
}

BOOL CStorageBuffer::GetBestFit(int cx, int cy, LPPOINT lpBuffer) const
{
	if (lpBuffer == NULL || !m_spaces.IsCreated())
		return FALSE;

	lpBuffer->x = -1;
	lpBuffer->y = -1;
	int nCurAdj = 65535;
	int nCurIntense = -1;
	
	for (int i = 0; i < m_spaces.GetCX(); i++)
	{
		for (int j = 0; j < m_spaces.GetCY(); j++)
		{
			int nAdj, nIntense;
			if (!GetAdjacent(i, j, cx, cy, nAdj, nIntense))
				continue;

			//GameInfof("%d %d - %d, %d", i, j, nAdj, nIntense);
			
			if (nAdj < nCurAdj || (nAdj == nCurAdj && nIntense < nCurIntense))
			{
				lpBuffer->x = i;
				lpBuffer->y = j;
				nCurAdj = nAdj;
				nCurIntense = nIntense;
			}
		}
	}
	
	return lpBuffer->x >= 0 && lpBuffer->y >= 0;
}
