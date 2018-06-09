/////////////////////////////////////////////////////////////////
// StorageBuffer.cpp
//
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////

#include "StorageBuffer.h"
#include "Constants.h"
#include "definitions.h"
#include "d2functions.h"
#include "D2Hackit.h"
#include <iostream>
#include <io.h>
#include <stdio.h>

#define INV_ROW		10
#define INV_COL		4
#define STASH_ROW	6
#define STASH_COL	8
#define CUBE_ROW	3
#define CUBE_COL	4

CStorageBuffer::CStorageBuffer(BYTE iType)
{
	SetType(iType);
}

CStorageBuffer::~CStorageBuffer()
{
}

BOOL CStorageBuffer::AddToBuffer(DWORD dwItemID, int x, int y, int cx, int cy)
{
	if (dwItemID == 0 || !IsValidLocationSize(x, y, cx, cy))
		return FALSE;

	//GameInfof("%d, %d, %d, %d: %d, %d", x, y, cx, cy, m_spaces.GetCX(), m_spaces.GetCY());
	//return 1;
	for (int i = 0; i < cx; i++)
	{
		for (int j = 0; j < cy; j++)
			m_aBuffer[i + x][j + y] = dwItemID;
	}

	//GameInfof("%08X added", dwItemID);
	return TRUE;
}

BOOL CStorageBuffer::IsValid() const
{
	return m_cx> 0 && m_cy > 0;
}

BOOL CStorageBuffer::IsValidLocationSize(int x, int y, int cx, int cy) const
{
	if (cx < 1 || cy < 1)
		return FALSE;

	return IsValidIndex(x, y) &&  IsValidIndex(x + cx - 1, y + cy - 1);
}

DWORD CStorageBuffer::GetItem(int x, int y) const
{
	return IsValidIndex(x, y) ? m_aBuffer[x][y] : 0;
}

BOOL CStorageBuffer::Dump(LPCSTR lpszFilePath) const
{
	if (!IsValid())
		return FALSE;

	FILE* file = fopen(lpszFilePath, "w+");
	if (file == NULL)
		return FALSE;

	for (int j = 0; j < m_cy; j++)
	{		
		for (int i = 0; i < m_cx; i++)
			fprintf(file, "%d ", !!m_aBuffer[i][j]);
		
		fprintf(file, "\n");
	}

	fclose(file);
	return TRUE;
}

BOOL CStorageBuffer::GetAdjacent(int x, int y, int cx, int cy, int& rAdj, int& rIntens) const
{
	// the "adjacent spaces" theroy for storage optimization, idea from netterhaufen
	rAdj = 0;
	rIntens = 0;

	if (!IsValid() || cx < 1 || cy < 1)
		return FALSE;

	int i = 0, j = 0;
	for (i = x; i < x + cx; i++)
	{
		for (j = y; j < y + cy; j++)
		{
			if (!IsValidIndex(i, j) || m_aBuffer[i][j])
				return FALSE; // cannot put in
		}
	}

	// check edges 1 (left)
	i = x - 1;
	for (j = y; j < y + cy; j++)
	{
		if (IsValidIndex(i, j) && m_aBuffer[i][j] == 0)
			rAdj++;
	}

	// check edges 2 (right)
	i = x + cx;
	for (j = y; j < y + cy; j++)
	{
		if (IsValidIndex(i, j) && m_aBuffer[i][j] == 0)
			rAdj++;
	}

	// check edges 3 (top)
	j = y - 1;
	for (i = x; i < x + cx; i++)
	{
		if (IsValidIndex(i, j) && m_aBuffer[i][j] == 0)
			rAdj++;
	}

	// check edges 4 (bottom)
	j = y + cy;
	for (i = x; i < x + cx; i++)
	{
		if (IsValidIndex(i, j) && m_aBuffer[i][j] == 0)
			rAdj++;
	}

	// calculate intensification
	rIntens = 1;
	for (i = 0; i < m_cx; i++)
	{
		int nCount = 0;
		for (j = 0; j < m_cy; j++)
		{
			if (m_aBuffer[i][j] || (i >= x && i < x + cx && j >= y && j < y + cy))
				nCount++;
		}

		rIntens *= (nCount + 1);
	}

	return TRUE;
}

BOOL CStorageBuffer::GetBestFit(int cx, int cy, LPPOINT lpBuffer) const
{
	if (lpBuffer == NULL || !IsValid())
		return FALSE;

	lpBuffer->x = -1;
	lpBuffer->y = -1;
	int nCurAdj = 65535;
	int nCurIntense = -1;
	
	for (int i = 0; i < m_cx; i++)
	{
		for (int j = 0; j < m_cy; j++)
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

void CStorageBuffer::SetType(BYTE iType)
{
	m_cx = 0;
	m_cy = 0;
	::memset(m_aBuffer, 0, sizeof(m_aBuffer));
	
	switch (iType)
	{
	case STORAGE_INVENTORY:
		m_cx = INV_ROW;
		m_cy = INV_COL;
		break;

	case STORAGE_CUBE:
		if (GetCubeID())
		{
			m_cx = CUBE_ROW;
			m_cy = CUBE_COL;
		}
		
		break;

	case STORAGE_STASH:
		m_cx = STASH_ROW;
		m_cy = STASH_COL;
		break;

	default:
		break;
	}

	if (m_cx == 0 || m_cy == 0)
		return;
	
	// now build up the storage
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return;	

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData && p->ptItemData->NodePage == 1 && p->ptItemData->ItemLocation2 == iType)
		{
			char szItemCode[4] = "";
			GetItemCode(p->dwId, szItemCode, 3);
			SIZE cz = GetItemSize(szItemCode);
			int x = (int)p->hOPath->Xpos;
			int y = (int)p->hOPath->Ypos;
			AddToBuffer(p->dwId, x, y, cz.cx, cz.cy);
		}
	}
}

BOOL CStorageBuffer::IsValidIndex(int x, int y) const
{
	return x >= 0 && x < m_cx && y >= 0 && y < m_cy;
}

DWORD CStorageBuffer::GetCubeID()
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL || pPlayer->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pPlayer->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData
			&& p->ptItemData->NodePage == 1
			&& (p->ptItemData->ItemLocation2 == STORAGE_INVENTORY || p->ptItemData->ItemLocation2 == STORAGE_STASH))
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (!::stricmp(szCode, "box"))
				return p->dwId;
		}
	}

	return 0;
}
