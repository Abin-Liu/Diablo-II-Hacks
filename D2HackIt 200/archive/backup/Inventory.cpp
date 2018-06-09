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
#include "Item.h"
#include "D2Hackit.h"
#include "me.h"
#include "item.h"

#define LOCATION2_INV		0
#define LOCATION2_CUBE		3
#define LOCATION2_STASH		4

#define NODEPAGE_STORAGE	1
#define NODEPAGE_BELTSLOTS	2
#define NODEPAGE_EQUIP		3

struct ITEM_SIGNATURE
{
	DWORD dwLocation2;
	DWORD dwNodePage;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInventory::CInventory()
{
	m_bValid = FALSE;
	ClearAll();
}

CInventory::~CInventory()
{
	
}

void CInventory::ClearAll()
{
	m_dwCubeID = 0;
	::memset(m_aInventory, 0, sizeof(m_aInventory));
	::memset(m_aStash, 0, sizeof(m_aStash));
	::memset(m_aCube, 0, sizeof(m_aCube));
	m_aInventoryItems.RemoveAll();
	m_aStashItems.RemoveAll();
	m_aCubeItems.RemoveAll();
	m_aEquipItems.RemoveAll();
	m_bSwitching = FALSE;
}

void CInventory::OnGamePacketAfterReceived(const BYTE *aPacket, DWORD aLen)
{	
	if (aPacket[0] == 0x9c
		&& (aPacket[1] == ITEM_ACTION_TO_STORAGE || aPacket[1] == ITEM_ACTION_SWITCH_STORAGE))
	{
		// to storage
		ITEM item;
		if (!D2ParseItem(aPacket, aLen, &item))
			return;

		SIZE cs = D2GetItemSize(item.szItemCode);
		switch (item.iStorageID)
		{
		case INV_TYPE_INVENTORY:
			if (stricmp(item.szItemCode, "box") == 0)
				m_dwCubeID = item.dwItemID;
			m_aInventoryItems.Add(item);
			AddToInventory(item.dwItemID, item.wPositionX, item.wPositionY, cs.cx, cs.cy);
			break;

		case INV_TYPE_STASH:
			if (stricmp(item.szItemCode, "box") == 0)
				m_dwCubeID = item.dwItemID;
			m_aStashItems.Add(item);
			AddToStash(item.dwItemID, item.wPositionX, item.wPositionY, cs.cx, cs.cy);
			break;

		case INV_TYPE_CUBE:
			m_aCubeItems.Add(item);
			AddToCube(item.dwItemID, item.wPositionX, item.wPositionY, cs.cx, cs.cy);
			break;

		default:
			break;
		}
	}

	if (aPacket[0] == 0x9d && aPacket[1] == ITEM_ACTION_FROM_STORAGE)
	{
		// from storage
		ITEM item;
		if (!D2ParseItem(aPacket, aLen, &item))
			return;

		RemoveItem(item.dwItemID);
		switch (item.iStorageID)
		{
		case INV_TYPE_INVENTORY:
			if (stricmp(item.szItemCode, "box") == 0)
				m_dwCubeID = 0;			
			RemoveFromInventory(item.dwItemID);
			break;

		case INV_TYPE_STASH:
			if (stricmp(item.szItemCode, "box") == 0)
				m_dwCubeID = 0;
			RemoveFromStash(item.dwItemID);
			break;

		case INV_TYPE_CUBE:
			RemoveFromCube(item.dwItemID);
			break;

		default:
			break;
		}
	}	
}

BOOL CInventory::Dump(LPCSTR lpszFile) const
{
	FILE* file = fopen(lpszFile, "w");
	if (file == NULL)
		return FALSE;

	int i, j;

	fprintf(file, "%s\n\n", "---------- Inventory ----------");
	for (j = 0; j < INV_COL; j++)
	{		
		for (i = 0; i < INV_ROW; i++)
			fprintf(file, "%d ", !!m_aInventory[i][j]);
		
		fprintf(file, "\n");
	}

	fprintf(file, "\n\n%s\n\n", "---------- Stash ----------");
	for (j = 0; j < STASH_COL; j++)	
	{
		for (i = 0; i < STASH_ROW; i++)
			fprintf(file, "%d ", !!m_aStash[i][j]);
		
		fprintf(file, "\n");
	}

	fprintf(file, "\n\n%s\n\n", "---------- Cube ----------");
	for (j = 0; j < CUBE_COL; j++)	
	{
		for (i = 0; i < CUBE_ROW; i++)
			fprintf(file, "%d ", !!m_aCube[i][j]);
		
		fprintf(file, "\n");
	}

	fclose(file);
	return TRUE;
}

BOOL CInventory::AddToInventory(DWORD dwItemID, int x, int y, int cx, int cy)
{
	if (dwItemID == 0 // verify item id
		|| x < 0 || y < 0 // verify location
		|| cx == 0 || cx > 2 || cy == 0 || cy > 4 // verify size
		|| x + cx > INV_ROW || y + cy > INV_COL) // verify relative
		return FALSE;

	for (int j = 0; j < cy; j++)	
	{
		for (int i = 0; i < cx; i++)
		{
			if (m_aInventory[i + x][j + y] != 0) // there's an item, remove it
				RemoveFromInventory(m_aInventory[i + x][j + y]);

			m_aInventory[i + x][j + y] = dwItemID; // add in
		}
	}

	return TRUE;
}

BOOL CInventory::AddToStash(DWORD dwItemID, int x, int y, int cx, int cy)
{
	if (dwItemID == 0 // verify item id
		|| x < 0 || y < 0 // verify location
		|| cx == 0 || cx > 2 || cy == 0 || cy > 4 // verify size
		|| x + cx > STASH_ROW || y + cy > STASH_COL) // verify relative
		return FALSE;

	for (int j = 0; j < cy; j++)	
	{
		for (int i = 0; i < cx; i++)
		{
			if (m_aStash[i + x][j + y] != 0) // there's an item, remove it
				RemoveFromStash(m_aStash[i + x][j + y]);

			m_aStash[i + x][j + y] = dwItemID; // add in
		}
	}

	return TRUE;
}

BOOL CInventory::AddToCube(DWORD dwItemID, int x, int y, int cx, int cy)
{
	if (dwItemID == 0 // verify item id
		|| x < 0 || y < 0 // verify location
		|| cx == 0 || cx > 2 || cy == 0 || cy > 4 // verify size
		|| x + cx > CUBE_ROW || y + cy > CUBE_COL) // verify relative
		return FALSE;

	for (int j = 0; j < cy; j++)	
	{
		for (int i = 0; i < cx; i++)
		{
			if (m_aCube[i + x][j + y] != 0) // there's an item, remove it
				RemoveFromCube(m_aCube[i + x][j + y]);

			m_aCube[i + x][j + y] = dwItemID; // add in
		}
	}

	return TRUE;
}

BOOL CInventory::RemoveFromInventory(DWORD dwItemID)
{
	if (dwItemID == 0)
		return FALSE;

	for (int j = 0; j < INV_COL; j++)	
	{
		for (int i = 0; i < INV_ROW; i++)
		{
			if (m_aInventory[i][j] == dwItemID)
				m_aInventory[i][j] = 0; // removed
		}
	}

	return TRUE;
}

BOOL CInventory::RemoveFromStash(DWORD dwItemID)
{
	if (dwItemID == 0)
		return FALSE;

	for (int j = 0; j < STASH_COL; j++)	
	{
		for (int i = 0; i < STASH_ROW; i++)
		{
			if (m_aStash[i][j] == dwItemID)
				m_aStash[i][j] = 0; // removed
		}
	}

	return TRUE;
}

BOOL CInventory::RemoveFromCube(DWORD dwItemID)
{
	if (dwItemID == 0)
		return FALSE;

	for (int j = 0; j < CUBE_COL; j++)	
	{
		for (int i = 0; i < CUBE_ROW; i++)
		{
			if (m_aCube[i][j] == dwItemID)
				m_aCube[i][j] = 0; // removed
		}
	}

	return TRUE;
}

BOOL CInventory::FindInventoryPosition(int cx, int cy, LPPOINT lpBuffer) const
{
	if (cx == 0 || cy == 0)
		return FALSE;

	for (int j = 0; j < INV_COL; j++)
	{
		for (int i = 0; i < INV_ROW; i++)
		{
			if (m_aInventory[i][j])
				continue;

			BOOL bOK = TRUE;
			
			// check cx, cy
			for (int x = i; x < i + cx; x++)
			{
				for (int y = j; y < j + cy; y++)
				{
					if (m_aInventory[x][y] || x >= INV_ROW || y >= INV_COL)
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

BOOL CInventory::FindStashPosition(int cx, int cy, LPPOINT lpBuffer) const
{
	if (!m_bValid || cx == 0 || cy == 0)
		return FALSE;

	for (int j = 0; j < STASH_COL; j++)
	{
		for (int i = 0; i < STASH_ROW; i++)
		{
			if (m_aStash[i][j])
				continue;

			BOOL bOK = TRUE;
			
			// check cx, cy
			for (int x = i; x < i + cx; x++)
			{
				for (int y = j; y < j + cy; y++)
				{
					if (m_aStash[x][y] || x >= STASH_ROW || y >= STASH_COL)
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

BOOL CInventory::FindCubePosition(int cx, int cy, LPPOINT lpBuffer) const
{
	if (!m_bValid || !m_dwCubeID || cx == 0 || cy == 0)
		return FALSE;

	for (int j = 0; j < CUBE_COL; j++)
	{
		for (int i = 0; i < CUBE_ROW; i++)
		{
			if (m_aCube[i][j])
				continue;

			BOOL bOK = TRUE;
			
			// check cx, cy
			for (int x = i; x < i + cx; x++)
			{
				for (int y = j; y < j + cy; y++)
				{
					if (m_aCube[x][y] || x >= CUBE_ROW || y >= CUBE_COL)
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

int CInventory::FindStorageItem(DWORD dwItemID) const
{
	if (!m_bValid || dwItemID == 0)
		return FALSE;

	int i = 0;
	for (i = 0; i < m_aCubeItems.GetSize(); i++)
	{
		if (m_aCubeItems[i].dwItemID == dwItemID)
			return STORAGE_CUBE;
	}

	for (i = 0; i < m_aStashItems.GetSize(); i++)
	{
		if (m_aStashItems[i].dwItemID == dwItemID)
			return STORAGE_STASH;
	}

	for (i = 0; i < m_aInventoryItems.GetSize(); i++)
	{
		if (m_aInventoryItems[i].dwItemID == dwItemID)
			return STORAGE_INVENTORY;
	}

	return STORAGE_NONE;
}

DWORD CInventory::GetCubeID() const
{
	return m_dwCubeID;
}

void CInventory::RemoveItem(DWORD dwItemID)
{
	int i;
	for (i = m_aInventoryItems.GetSize() - 1; i >= 0; i--)
	{
		if (m_aInventoryItems[i].dwItemID == dwItemID)
		{
			m_aInventoryItems.RemoveAt(i);
			//return; // don't return, in case of multiple receive
		}
	}

	for (i = m_aStashItems.GetSize() - 1; i >= 0; i--)
	{
		if (m_aStashItems[i].dwItemID == dwItemID)
		{
			m_aStashItems.RemoveAt(i);
			//return; // don't return, in case of multiple receive
		}
	}

	for (i = m_aCubeItems.GetSize() - 1; i >= 0; i--)
	{
		if (m_aCubeItems[i].dwItemID == dwItemID)
		{
			m_aCubeItems.RemoveAt(i);
			//return; // don't return, in case of multiple receive
		}
	}
}

BOOL CInventory::EnumStorageItems(int nSTorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam) const
{
	if (lpfnEnumItemProc == NULL)
		return FALSE;

	int i;
	switch (nSTorageType)
	{
	case STORAGE_INVENTORY:
		for (i = 0; i < m_aInventoryItems.GetSize(); i++)
		{
			if (!lpfnEnumItemProc(&m_aInventoryItems[i], lParam))
				return FALSE;
		}
		break;
	
	case STORAGE_STASH:
		for (i = 0; i < m_aStashItems.GetSize(); i++)
		{
			if (!lpfnEnumItemProc(&m_aStashItems[i], lParam))
				return FALSE;
		}
		break;

	case STORAGE_CUBE:
		for (i = 0; i < m_aCubeItems.GetSize(); i++)
		{
			if (!lpfnEnumItemProc(&m_aCubeItems[i], lParam))
				return FALSE;
		}
		break;

	default:
		return FALSE;
		break;
	}

	return TRUE;
}

void CInventory::SetValid(BOOL bValid)
{
	m_bValid = bValid;
}

void CInventory::CheckEquip(const ITEM& item)
{
	if (item.iAction == ITEM_ACTION_TO_EQUIP)
	{
		// add equip
		m_aEquipItems.Add(item);
	}
	else if (item.iAction == ITEM_ACTION_SWITCH_EQUIP)
	{
		// switching equip
		if (!m_bSwitching)
		{
			// removing
			int nIdx = FindFromEquip(item.dwItemID);
			if (nIdx != -1)
			{
				m_aEquipItems.RemoveAt(nIdx);
				m_bSwitching = TRUE;
			}
		}
		else
		{
			// adding
			m_aEquipItems.Add(item);
			m_bSwitching = FALSE;
		}
	}
	else if (item.iAction == ITEM_ACTION_FROM_EQUIP)
	{
		// removing equip
		int nIdx = FindFromEquip(item.dwItemID);
		if (nIdx != -1)
			m_aEquipItems.RemoveAt(nIdx);
	}
}

int CInventory::FindFromEquip(DWORD dwItemID) const
{
	if (dwItemID == 0)
		return -1;

	for (int i = 0; i < m_aEquipItems.GetSize(); i++)
	{
		if (m_aEquipItems[i].dwItemID == dwItemID)
			return i;
	}

	return -1;
}

BYTE CInventory::GetLowestEquipItemDurability(BOOL bIncludeSecondarySlots) const
{
	BYTE iMinDura = 100;
	for (int i = 0; i < m_aEquipItems.GetSize(); i++)
	{
		if (m_aEquipItems[i].wMaxDurability == 0)			
			continue; // this item has no durability

		if (!bIncludeSecondarySlots && IsOnSecondary(m_aEquipItems[i].dwItemID))
			continue; // on sercondary weapon switch

		BYTE iDura = CalcPercent(m_aEquipItems[i].wDurability, m_aEquipItems[i].wMaxDurability);
		if (iDura < iMinDura)
			iMinDura = iDura;
	}

	return iMinDura;
}

BOOL CInventory::IsOnSecondary(DWORD dwItemID) const
{
	if (dwItemID == 0)
		return FALSE;

	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (!pUnit)
		return FALSE;

	Inventory* pInv = pUnit->ptInventory;

	if (!pInv)
		return FALSE;

	for (UnitItem* p = pInv->pFirstItem; p && p->ptItemData; p = p->ptItemData->ptNextInvenItem)
	{
		if (p->dwId == dwItemID && (p->ptItemData->ItemLocation == EQUIP_RIGHT_SECONDARY || p->ptItemData->ItemLocation == EQUIP_LEFT_SECONDARY))
			return TRUE;
	}

	return FALSE;
}

LPCITEM CInventory::GetEquipItem(int nEquipLocation) const
{
	if (nEquipLocation == 0)
		return NULL;

	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (!pUnit)
		return NULL;

	Inventory* pInv = pUnit->ptInventory;

	if (!pInv)
		return NULL;

	for (UnitItem* p = pInv->pFirstItem; p && p->ptItemData; p = p->ptItemData->ptNextInvenItem)
	{
		if (p->ptItemData->ItemLocation == (BYTE)nEquipLocation)
		{
			int nIdx = FindFromEquip(p->dwId);
			if (nIdx != -1)
				return &m_aEquipItems[nIdx];
		}
	}

	return NULL; // not found
}

LPCITEM CInventory::GetStorageItem(int nSTorageType, int nRow, int nColumn) const
{
	if (nRow < 0 || nColumn < 0)
		return NULL;

	DWORD dwItemID = 0;
	int i = 0;

	switch (nSTorageType)
	{
	case STORAGE_INVENTORY:
		if (nRow < INV_ROW && nColumn < INV_COL)
			dwItemID = m_aInventory[nRow][nColumn];

		if (dwItemID == 0)
			return NULL;

		for (i = 0; i < m_aInventoryItems.GetSize(); i++)
		{
			if (m_aInventoryItems[i].dwItemID == dwItemID)
				return &m_aInventoryItems[i];
		}

		break;
	
	case STORAGE_STASH:
		if (nRow < STASH_ROW && nColumn < STASH_COL)
			dwItemID = m_aStash[nRow][nColumn];

		if (dwItemID == 0)
			return NULL;

		for (i = 0; i < m_aStashItems.GetSize(); i++)
		{
			if (m_aStashItems[i].dwItemID == dwItemID)
				return &m_aStashItems[i];
		}
		
		break;

	case STORAGE_CUBE:
		if (nRow < CUBE_ROW && nColumn < CUBE_COL)
			dwItemID = m_aCube[nRow][nColumn];

		if (dwItemID == 0)
			return NULL;

		for (i = 0; i < m_aCubeItems.GetSize(); i++)
		{
			if (m_aCubeItems[i].dwItemID == dwItemID)
				return &m_aCubeItems[i];
		}
		
		break;

	default:
		break;
	}

	return NULL;
}

DWORD** CInventory::Allocate2DBuffer(int x, int y)
{
	if (x == 0 || y == 0)
		return NULL;

	DWORD** p = new DWORD*[x];
	if (p == NULL)
		return NULL;

	::memset(p, 0, sizeof(DWORD*) * x);
	for (int i = 0; i < y; i++)
	{
		p[i] = new DWORD[y];

		if (p[i] == NULL)
		{
			Free2DBuffer(p, x);
			return NULL;
		}

		::memset(p[i], 0, sizeof(DWORD) * y);
	}
	return p; // require the caller to free this memory!!!
}

DWORD* CInventory::Allocate1DBuffer(int x)
{
	if (x == 0)
		return NULL;

	DWORD* p = new DWORD[x]; 
	if (p == NULL)
		return NULL;

	::memset(p, 0, sizeof(DWORD) * x);
	return p; // require the caller to free this memory!!!
}

void CInventory::Free2DBuffer(DWORD**& pBuffer, int x)
{
	if (pBuffer == NULL)
		return;

	for (int i = 0; i < x; i++)
	{
		if (pBuffer[i])
		{
			delete [] pBuffer[i];
			pBuffer[i] = NULL;
		}
	}

	delete [] pBuffer;
	pBuffer = NULL;
}

void CInventory::Free1DBuffer(DWORD *&pBuffer)
{
	if (pBuffer)
	{
		delete [] pBuffer;
		pBuffer = NULL;
	}
}



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
			return D2GetBeltRows(szItemCode);		
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
				return FALSE
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

	UnitItem* p = D2COMMON_GetBeltItem(nSlot);
	if (p == NULL)
		return -1;

	return p->dwId;
}

DWORD EXPORT GetEquipItem(int nEquipLocation)
{
	if (lpfnEnumItemProc == NULL)
		return 0;
	
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
