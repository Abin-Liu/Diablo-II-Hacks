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
#include "Utilities\\ArrayEx.h"
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
	m_pStorage = NULL;
	::memset(&m_size, 0, sizeof(SIZE));

	switch (iType)
	{
	case LOCATION2_INV:
		m_size.cx = INV_ROW;
		m_size.cy = INV_COL;
		break;

	case LOCATION2_CUBE:
		m_size.cx = CUBE_ROW;
		m_size.cy = CUBE_COL;
		break;

	case LOCATION2_STASH:
		m_size.cx = STASH_ROW;
		m_size.cy = STASH_COL;
		break;

	default:
		break;
	}

	if (m_size.cx == 0 || m_size.cy == 0)
		return; // invalid type

	m_pStorage = new DWORD*[m_size.cx];
	if (m_pStorage == NULL)
		return;

	::memset(m_pStorage, 0, sizeof(DWORD*) * m_size.cx);
	for (int i = 0; i < m_size.cx; i++)
	{
		m_pStorage[i] = new DWORD[m_size.cy];

		if (m_pStorage[i] == NULL)
		{
			FreeBuffer();
			return;
		}

		::memset(m_pStorage[i], 0, sizeof(DWORD) * m_size.cy);
	}

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
	FreeBuffer();
}

void CStorageBuffer::FreeBuffer()
{
	if (m_pStorage == NULL)
		return;

	for (int i = 0; i < m_size.cx; i++)
	{
		if (m_pStorage[i])
		{
			delete [] m_pStorage[i];
			m_pStorage[i] = NULL;
		}
	}

	delete [] m_pStorage;
	m_pStorage = NULL;
}

BOOL CStorageBuffer::AddItem(DWORD dwItemID, int x, int y, int cx, int cy)
{
	if (dwItemID == 0 || !IsValidLocationSize(x, y, cx, cy))
		return FALSE;

	RemoveItem(dwItemID);
	for (int i = 0; i < cx; i++)
	{
		for (int j = 0; j < cy; j++)
		{
			m_pStorage[i + x][j + y] = dwItemID;
		}
	}

	return TRUE;
}

BOOL CStorageBuffer::IsValid() const
{
	return m_pStorage != NULL;
}

BOOL CStorageBuffer::RemoveItem(DWORD dwItemID)
{
	if (!IsValid() || dwItemID == 0)
		return FALSE;

	int nCount = 0;
	for (int i = 0; i < m_size.cx; i++)
	{
		for (int j = 0; j < m_size.cy; j++)
		{
			if (m_pStorage[i][j] == dwItemID)
			{
				m_pStorage[i][j] = 0;
				nCount++;
			}
		}
	}

	return nCount;
}

BOOL CStorageBuffer::IsValidLocationSize(int x, int y, int cx, int cy) const
{
	if (!IsValid())
		return FALSE;

	return (x >= 0 && y >= 0 && x + cx <= m_size.cx && y + cy <= m_size.cy);
}

DWORD CStorageBuffer::GetItem(int x, int y) const
{
	return IsValidLocationSize(x, y, 1, 1) ? m_pStorage[x][y] : ITEM_INVALID;
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

	for (int j = 0; j < m_size.cy; j++)
	{		
		for (int i = 0; i < m_size.cx; i++)
			fprintf(file, "%d ", !!m_pStorage[i][j]);
		
		fprintf(file, "\n");
	}

	fclose(file);
	return TRUE;
}

BOOL CStorageBuffer::FindFirstAvailableLocation(int cx, int cy, LPPOINT lpBuffer) const
{
	if (!IsValid() || cx == 0 || cy == 0)
		return FALSE;

	for (int j = 0; j < m_size.cy; j++)
	{
		for (int i = 0; i < m_size.cx; i++)
		{
			if (m_pStorage[i][j])
				continue;

			BOOL bOK = TRUE;
			
			// check cx, cy
			for (int x = i; x < i + cx; x++)
			{
				for (int y = j; y < j + cy; y++)
				{
					if (!IsValidLocationSize(x, y, 1, 1) || m_pStorage[x][y])
					{
						bOK = FALSE;
						break;
					}
				}				
			}

			if (!bOK)
				continue;
			
			// OK, we found
			if (lpBuffer)
			{
				lpBuffer->x = i;
				lpBuffer->y = j;
			}

			return TRUE;
		}
	}

	return FALSE;
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

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData && p->ptItemData->NodePage == NODEPAGE_STORAGE && p->ptItemData->ItemLocation2 == (BYTE)nSTorageType)
		{
			if (!lpfnEnumItemProc(p->dwId, lParam))
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

DWORD FindFirstBeltPotion(int nType, DWORD& dwClassID, BOOL bKeepBottomRow)
{
	dwClassID = 0;

	if (nType < 0 || nType > POTION_PURPLE)
		return 0;

	int nStart = bKeepBottomRow ? 4 : 0;
	for (int i = 15; i >= nStart; i--)
	{
		DWORD dwItemID = GetBeltItem(i);

		if (dwItemID <= 0)
			continue;

		char szCode[4] = "";
		GetItemCode(dwItemID, szCode, 3);
		if (D2IsPotion(szCode) == nType)
		{
			GAMEUNIT gu;
			gu.dwUnitID = dwItemID;
			gu.dwUnitType = UNIT_TYPE_ITEM;
			dwClassID = GetUnitClassID(&gu);
			return dwItemID;
		}
	}

	return 0;
}

DWORD FindFirstInventoryPotion(int nType, DWORD& dwClassID)
{
	dwClassID = 0;

	if (nType <= 0 || nType > POTION_JUNK)
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
			if (D2IsPotion(szCode) == nType)
			{
				dwClassID = p->dwClassId;
				return p->dwId;
			}
		}
	}

	return 0; // not found
}

BOOL EXPORT FindFirstStorageSpace(int nStorageType, SIZE minSize, LPPOINT lpBuffer)
{
	CStorageBuffer sb((BYTE)nStorageType);
	return sb.FindFirstAvailableLocation(minSize.cx, minSize.cy, lpBuffer);
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



