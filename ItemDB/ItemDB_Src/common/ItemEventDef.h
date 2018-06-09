////////////////////////////////////////////////////////////////////
// ItemEventDef.h
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////

#ifndef __ITEMEVENT_H__
#define __ITEMEVENT_H__

#include <windows.h>

#define WM_ITEMDB_IDENTIFIER			0x193488F3 // ITEM
#define	WM_ITEMDB_CLIENT_UPDATE			(WM_APP + 1225)

#define ITEMDB_WND_SERVER	"ItemDB_Msg_Wnd" // Receiver app HWND class

#define STO_INVALID		-1	// Invalid
#define STO_INVENTORY	0	// Inventory
#define STO_EQUIP		1	// Equip
#define STO_CUBE		3	// cube
#define STO_STASH		4	// stash
#define STO_CURSOR		10	// cursor
#define STO_MERC		11	// merc equip

#define ITEM_MAX_MODCOUNT	16

typedef struct tagCharInfo
{
	BYTE iActiveSwitch;
	WORD wMercClass;
	char szMercName[32];
	DWORD dwInvGold;
	DWORD dwStashGold;
	DWORD dwGoldLimit;
	char szRealm[65];
	char szAccount[65];
	char szCharName[16];
	BYTE iCharClass;
	BYTE iHardcore;	
} CHARINFO, *LPCHARINFO;
typedef const tagCharInfo* LPCCHARINFO;

typedef struct tagModInfo
{
	DWORD dwModID;
	int aValues[4];
} MODINFO, *LPMODINFO;
typedef const MODINFO* LPCMODINFO;

typedef struct tagItemInfo
{	
	//DWORD dwPlayerID; // server side database player id
	BYTE iStorageType;
	BYTE x;
	BYTE y;
	BYTE cx;
	BYTE cy;
	BYTE iQuality;
	BYTE iSockets;
	BYTE iLevel;
	BYTE iType;
	BYTE iArtwork;
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
	char szName[128];
	char szCode[4];
	MODINFO aMods[ITEM_MAX_MODCOUNT];
	BYTE iModCount;
} ITEMINFO, *LPITEMINFO;
typedef const tagItemInfo* LPCITEMINFO;

#endif // __ITEMEVENT_H__