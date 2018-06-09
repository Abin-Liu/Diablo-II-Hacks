///////////////////////////////////////////////////////////
// Inventory.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <windows.h>
#include "StorageBuffer.h"

BOOL FindStorageItem(DWORD dwItemID);
int FindEquipItem(DWORD dwItemID);
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
