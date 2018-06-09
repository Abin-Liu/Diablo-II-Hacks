///////////////////////////////////////////////////////////
// Inventory.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <windows.h>

class CStorageBuffer
{
public:	
	
	CStorageBuffer(BYTE iType);
	~CStorageBuffer();

	BOOL IsValid() const;
	BOOL Dump(LPCSTR lpszFilePath) const;
	BYTE GetType() const;
	DWORD GetItem(int x, int y) const;
	BOOL FindFirstAvailableLocation(int cx, int cy, LPPOINT lpBuffer) const;

private:
	
	BOOL IsValidLocationSize(int x, int y, int cx, int cy) const;
	void FreeBuffer();
	BOOL RemoveItem(DWORD dwItemID);
	BOOL AddItem(DWORD dwItemID, int x, int y, int cx, int cy);
	DWORD** m_pStorage;
	SIZE m_size;
	BYTE m_iType;
};

BOOL FindStorageItem(DWORD dwItemID);
int FindEquipItem(DWORD dwItemID);
DWORD FindFirstBeltPotion(int nType, DWORD& dwClassID, BOOL bKeepBottomRow);
DWORD FindFirstInventoryPotion(int nType, DWORD& dwClassID);
DWORD GetCubeID();
DWORD GetInventoryTPBookCharges();
DWORD GetInventoryTPScrolls();
DWORD GetInventoryIDBookCharges();
DWORD GetInventoryIDScrolls();
DWORD GetInventoryItemCount(LPCSTR lpszItemCode);
DWORD GetInventoryItemCharges(LPCSTR lpszItemCode);

#endif // __INVENTORY_H__
