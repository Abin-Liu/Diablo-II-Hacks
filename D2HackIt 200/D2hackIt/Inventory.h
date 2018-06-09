///////////////////////////////////////////////////////////
// Inventory.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <windows.h>
#include "StorageBuffer.h"

DWORD GetInventoryTPBookCharges();
DWORD GetInventoryTPScrolls();
DWORD GetInventoryIDBookCharges();
DWORD GetInventoryIDScrolls();
DWORD FindFirstBeltPotion(LPCSTR lpszCode, BOOL bKeepBottomRow);
DWORD FindFirstInventoryPotion(LPCSTR lpszCode);
BOOL IsMyInventoryItem(DWORD dwItemID);
int GetAvailableColumnFor(int nPotionType);
DWORD FindFirstTPScroll();
DWORD FindFirstUseableTPBook();

#endif // __INVENTORY_H__
