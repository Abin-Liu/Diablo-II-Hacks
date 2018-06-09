#ifndef __D2OFFSETS_H__
#define __D2OFFSETS_H__

#include "D2Structs.h"

typedef struct tagD2Modules
{
	HMODULE hD2Client; // D2Client.dll
	HMODULE hD2Common; // D2Common.dll
	FARPROC fpGetItemTxt;
	FARPROC fpGetItemNameString;
	FARPROC fpGetFirstInventoryItem;
	FARPROC fpGetNextInventoryItem;
} D2MODULES, *LPD2MODULES;
typedef const tagD2Modules* LPCD2MODULES;

enum DllBase {
	DLLBASE_D2CLIENT = 0x6FAA0000,
	DLLBASE_D2COMMON = 0x6FD40000,
	DLLBASE_D2GFX = 0x6FA70000,
	DLLBASE_D2WIN = 0x6F8A0000,
	DLLBASE_D2LAUNCH = 0x6FA10000,
	DLLBASE_D2LANG = 0x6FC10000,
	DLLBASE_D2CMP = 0x6FDF0000,
	DLLBASE_D2MULTI = 0x6F9A0000,
	DLLBASE_BNCLIENT = 0x6FF00000,
	DLLBASE_D2NET = 0x6FC00000,
	DLLBASE_STORM = 0x6FFB0000
};

#define OF_PLAYERDATA		0x6FBBC200
#define OF_BNDATA			0x6FA3CD44
#define OF_GETFIRSTINVITEM	0x00A1E7A0

BOOL LoadD2Modules(LPCSTR lpszDirectory, LPD2MODULES lpDM);
void FreeD2Modules(LPD2MODULES lpDM);

ItemBinData* GetItemNameString(HANDLE hProcess, UnitItem* pItem, LPSTR lpszBuffer, int nLength);
BOOL GetBNetData(HANDLE hProcess, BnetData* pBuffer);
BOOL GetPlayerUnit(HANDLE hProcess, UnitAny* pBuffer);
BOOL EnumInventoryItems(HANDLE hProcess, LPCD2MODULES lpDM);

#endif // __D2OFFSETS_H__