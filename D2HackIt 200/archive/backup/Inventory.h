///////////////////////////////////////////////////////////
// Inventory.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <windows.h>
#include "CommonStructs.h"
#include "utilities\\ArrayEx.h"
#include "..\INCLUDES\CommonStructs.h"	// Added by ClassView

#define INV_ROW		10
#define INV_COL		4
#define STASH_ROW	6
#define STASH_COL	8
#define CUBE_ROW	3
#define CUBE_COL	4
#define BELT_ROW	4
#define BELT_COL	4
#define BELT_SLOTS	16

#define INV_TYPE_INVENTORY	0x01
#define INV_TYPE_STASH		0x05
#define INV_TYPE_CUBE		0x04

#define ITEM_INVALID	-1

typedef CArrayEx<ITEM, const ITEM&> ItemArray;

class CStorageBuffer
{
public:	
	BOOL FindFirstAvailableLocation(int cx, int cy, LPPOINT lpBuffer) const;
	
	CStorageBuffer(BYTE iType);
	~CStorageBuffer();

	BOOL IsValid() const;
	BOOL Dump(LPCSTR lpszFilePath) const;
	BYTE GetType() const;
	DWORD GetItem(int x, int y) const;

private:
	
	BOOL IsValidLocationSize(int x, int y, int cx, int cy) const;
	void FreeBuffer();
	BOOL RemoveItem(DWORD dwItemID);
	BOOL AddItem(DWORD dwItemID, int x, int y, int cx, int cy);
	DWORD** m_pStorage;
	SIZE m_size;
	BYTE m_iType;
};

class CInventory  
{
public:
	LPCITEM GetStorageItem(int nSTorageType, int nRow, int nColumn) const;
	LPCITEM GetEquipItem(int nEquipLocation) const;

	BYTE GetLowestEquipItemDurability(BOOL bIncludeSecondarySlots) const;
	void CheckEquip(const ITEM& item);
	void SetValid(BOOL bValid);
	BOOL EnumStorageItems(int nSTorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam) const;
	DWORD GetCubeID() const;
	int FindStorageItem(DWORD dwItemID) const;
	
	CInventory();
	virtual ~CInventory();

	BOOL FindInventoryPosition(int cx, int cy, LPPOINT lpBuffer) const;
	BOOL FindStashPosition(int cx, int cy, LPPOINT lpBuffer) const;
	BOOL FindCubePosition(int cx, int cy, LPPOINT lpBuffer) const;
	
	void OnGamePacketAfterReceived(const BYTE* aPacket, DWORD aLen);
	void ClearAll();	
	BOOL Dump(LPCSTR lpszFile) const;

private:
	static void Free1DBuffer(DWORD*& pBuffer);
	static void Free2DBuffer(DWORD**& pBuffer, int x);
	static DWORD* Allocate1DBuffer(int x);
	static DWORD** Allocate2DBuffer(int x, int y);
	BOOL IsOnSecondary(DWORD dwItemID) const;
	int FindFromEquip(DWORD dwItemID) const;
	void RemoveItem(DWORD dwItemID);	
	BOOL RemoveFromInventory(DWORD dwItemID);
	BOOL RemoveFromStash(DWORD dwItemID);
	BOOL RemoveFromCube(DWORD dwItemID);
	BOOL AddToCube(DWORD dwItemID, int x, int y, int cx, int cy);
	BOOL AddToStash(DWORD dwItemID, int x, int y, int cx, int cy);
	BOOL AddToInventory(DWORD dwItemID, int x, int y, int cx, int cy);
		
	DWORD m_aInventory[INV_ROW][INV_COL]; // Inventory 4*10
	DWORD m_aStash[STASH_ROW][STASH_COL]; // Stash 8*6
	DWORD m_aCube[CUBE_ROW][CUBE_COL];    // Cube 4*3
	DWORD m_dwCubeID; // cube id

	ItemArray m_aInventoryItems;
	ItemArray m_aStashItems;
	ItemArray m_aCubeItems;
	ItemArray m_aEquipItems;

	BOOL m_bValid;
	BOOL m_bSwitching;
};

#endif // __INVENTORY_H__
