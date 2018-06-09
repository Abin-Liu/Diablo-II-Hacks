#ifndef __SERVERITEM_H__
#define __SERVERITEM_H__

typedef struct tagItemMod
{
	DWORD dwModID;			// Mod ID, could be different from property ID
	DWORD dwOriginalID;		// Original property ID
	int aValues[4];			// Mod values
	int nValueCount;		// ValueCount
	BOOL bFloat;			// Is float value
	BOOL bBoolean;			// Is Boolean value, which means aValues[0] is constant 1
} ITEMMOD, *LPITEMMOD;
typedef const tagItemMod* LPCITEMMOD;

typedef struct tagServerItem
{	
	BYTE iQuality;
	BYTE iSockets;
	BYTE iLevel;
	BYTE iType;
	BYTE cx;
	BYTE cy;
	DWORD dwFlags;
	WORD wStrReq;
	WORD wDexReq;
	BYTE iLevelreq;
	BYTE i2Hand;
	WORD wMinDamage;
	WORD wMaxDamage;
	WORD wMinDura;
	WORD wMaxDura;
	WORD wdefense;
	WORD wQuantity;
	DWORD dwValue;
	ITEMMOD aMods[16];
	BYTE iModCount;
} SERVERITEM, *LPSERVERITEM;
typedef const tagServerItem* LPCSERVERITEM;

#endif // __SERVERITEM_H__