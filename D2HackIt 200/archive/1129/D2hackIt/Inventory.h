///////////////////////////////////////////////////////////
// Inventory.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <windows.h>
#include "..\\Includes\\utilities\\matrix.h"

class CStorageBuffer
{
public:	
	
	CStorageBuffer(BYTE iType);
	~CStorageBuffer();

	BOOL IsValid() const;
	BOOL Dump(LPCSTR lpszFilePath) const;
	BYTE GetType() const;
	DWORD GetItem(int x, int y) const;
	BOOL GetBestFit(int cx, int cy, LPPOINT lpBuffer) const;

private:
	
	BOOL IsValidLocationSize(int x, int y, int cx, int cy) const;
	BOOL GetAdjacent(int x, int y, int cx, int cy, int& rAdj, int& rIntens) const;
	BOOL RemoveItem(DWORD dwItemID);
	BOOL AddItem(DWORD dwItemID, int x, int y, int cx, int cy);
	BYTE m_iType;
	CMatrix<DWORD, DWORD> m_spaces;	
};

BOOL FindStorageItem(DWORD dwItemID);
int FindEquipItem(DWORD dwItemID);
DWORD GetCubeID();
DWORD GetInventoryTPBookCharges();
DWORD GetInventoryTPScrolls();
DWORD GetInventoryIDBookCharges();
DWORD GetInventoryIDScrolls();
DWORD GetInventoryItemCount(LPCSTR lpszItemCode);
DWORD GetInventoryItemCharges(LPCSTR lpszItemCode);
BOOL IsMyInventoryItem(DWORD dwItemID);
int GetAvailableColumnFor(int nPotionType);
DWORD FindFirstTPScroll();
DWORD FindFirstUseableTPBook();

DWORD FindFirstBeltPotion(LPCSTR lpszCode, BOOL bKeepBottomRow);
DWORD FindFirstInventoryPotion(LPCSTR lpszCode);

#endif // __INVENTORY_H__
