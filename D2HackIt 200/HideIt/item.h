#ifndef __ITEM_H__
#define __ITEM_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#define	ITEM_MESSAGEID_DROP				0x9C // drop/pick from belt/ground
#define	ITEM_MESSAGEID_OPERATE			0x9D // pick/equip
	
#define	ITEM_ACTION_NEW_GROUND			0x00 //9c
#define	ITEM_ACTION_PICK_UP				0x01 //9c
#define	ITEM_ACTION_DROP				0x02 //9c
#define	ITEM_ACTION_OLD_GROUND			0x03 //9c
#define	ITEM_ACTION_TO_STORAGE			0x04 //9c
#define	ITEM_ACTION_FROM_STORAGE		0x05 //9d
#define	ITEM_ACTION_TO_EQUIP			0x06 //9d
#define	ITEM_ACTION_FROM_EQUIP			0x08 //9d
#define	ITEM_ACTION_SWITCH_EQUIP		0x09 //9d
#define	ITEM_ACTION_TO_STORE			0x0B //9c
#define	ITEM_ACTION_FROM_STORE			0x0C //9c
#define	ITEM_ACTION_SWITCH_STORAGE		0x0d //9c
#define	ITEM_ACTION_TO_BELTSLOT			0x0E //9c
#define	ITEM_ACTION_FROM_BELTSLOT		0x0F //9c
#define	ITEM_ACTION_SWITCH_BELTSLOT		0x10 //9c
#define ITEM_ACTION_TO_CURSOR			0x12 //9c
#define ITEM_ACTION_SHIFT_BELTSLOT		0x15 //9d
	
#define	ITEM_ITEMTYPE_HELM				0x00
#define	ITEM_ITEMTYPE_ARMOR				0x01
#define	ITEM_ITEMTYPE_WEAPON			0x05
#define	ITEM_ITEMTYPE_BOW				0x06
#define	ITEM_ITEMTYPE_SHIELD			0x07
#define	ITEM_ITEMTYPE_EXPANSION			0x0A // heads
#define	ITEM_ITEMTYPE_OTHER				0x10

// Drop locations
#define	ITEM_LOCAIION_STORED			0x00
#define	ITEM_LOCATION_WORN				0x01
#define	ITEM_LOCATION_BELT				0x02
#define	ITEM_LOCATION_GROUND			0x03
#define	ITEM_LOCATION_TOHAND			0x04

// Item levels
#define	ITEM_LEVEL_INFERIOR				0x01
#define	ITEM_LEVEL_NORMAL				0x02
#define	ITEM_LEVEL_SUPERIOR				0x03
#define	ITEM_LEVEL_MAGIC				0x04
#define	ITEM_LEVEL_SET					0x05
#define	ITEM_LEVEL_RARE					0x06
#define	ITEM_LEVEL_UNIQUE				0x07
#define	ITEM_LEVEL_CRAFT				0x08
#define	ITEM_INFERIORTYPE_CRUDE			0x00
#define	ITEM_INFERIORTYPE_CRACKED		0x01
#define	ITEM_INFERIORTYPE_DAMAGED		0x02
#define	ITEM_INFERIORTYPE_LOWQUALITY	0x04
#define	ITEM_SUPERIOR_AR				0x00
#define	ITEM_SUPERIOR_MAXDMG			0x01
#define	ITEM_SUPERIOR_AC				0x02
#define	ITEM_SUPERIOR_AR_MAXDMG			0x03
#define	ITEM_SUPERIOR_DUR				0x04
#define	ITEM_SUPERIOR_DUR_AR			0x05
#define	ITEM_SUPERIOR_DUR_MAXDMG		0x06
#define	ITEM_SUPERIOR_DUR_AC			0x07

// Gold sizes
#define	ITEM_GOLDSIZE_12BIT				0x00
#define	ITEM_GOLDSIZE_32BIT				0x01
#define	ITEM_UNSET						0xFF

// Miscs
#define	ITEM_CODE_LEN					3
#define	ITEM_DESC_LEN					128

#define ITEM_MAX_MOD	25
typedef struct tagItemMod1
{
	DWORD dwModID;
	BYTE iCount;
	DWORD aValues[4];
} ITEMMOD1, *LPITEMMOD1;
typedef const tagItemMod1* LPCITEMMOD1;

// The ITEM struct
typedef struct tagItem
{
	DWORD dwItemID; // item ID
	BYTE iMessageID; // 0x9c/0x9d
	BYTE iAction; // action
	BYTE iType;		// type: weapon, armor, etc
	BYTE iIsSocketFull; // is sockets full?
	BYTE iSocketNumber; // number of total sockets on this item
	BYTE iSwtichIn; // switched in?
	BYTE iSwitchOut; // switched out?
	BYTE iBroken; // item is broken
	BYTE iIdentified; // identified?
	BYTE iEthereal; // ethereal?
	BYTE iLevel; // item level
	BYTE iLocation; // ground, belt, inventory etc
	BYTE iPersonalized; // personalized
	BYTE iRuneword; // is runeword item
	BYTE iGamble; // gamble
	BYTE iStorageID; // storage id
	BYTE iQuality; // unique, rare, set, superior, etc
	WORD wPositionX; // location x
	WORD wPositionY; // location y
	DWORD dwGoldAmount; // Gold amount, "gld" only
	TCHAR szItemCode[ITEM_CODE_LEN + 1]; // item code, 3 letters
	WORD wDefense; // defense
	WORD wDurability; // durability
	WORD wMaxDurability; // maximum durability
	BYTE iModCount; // number of total mods
	BYTE iAtBeltRow; // belt row index(0-4)
	BYTE iAtBeltColumn; // belt column index(0-4)
	ITEMMOD1 aMods[ITEM_MAX_MOD]; // mods
} ITEM, *LPITEM;
typedef const tagItem* LPCITEM;

BOOL D2ParseItem(const BYTE* aPacket, DWORD aLen, ITEM& item); // Parses item info from a packet
BOOL D2GetItemCode(const BYTE* aPacket, DWORD aLen, LPTSTR szBuffer); // Extracts item code from a packet
BOOL D2GetItemMod(const ITEM& item, DWORD dwModID, LPITEMMOD1 pMod);
SIZE D2GetItemSize(LPCTSTR lpszItemCode);
BYTE D2GetBeltRows(LPCTSTR lpszItemCode); // if it's a belt, get the row count(1-4)
DWORD D2ItemCode2Dword(LPCTSTR lpszItemCode); // convert a string into dword

#endif // __ITEM_H__
