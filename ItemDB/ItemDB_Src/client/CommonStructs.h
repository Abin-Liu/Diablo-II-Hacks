//////////////////////////////////////////////////////////////////////
// CommonStructs.h
// -------------------------------------------------------------------
// This include file contains all structs and typedefs used by both 
// the server and the clients.
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#ifndef __COMMONSTRUCTS_H__
#define __COMMONSTRUCTS_H__

#include <windows.h>
#include "Constants.h"

//////////////////////////////////////////////////////////////////////
// GAMEUNIT
//--------------------------------------------------------------------
// The basic game unit info.
//////////////////////////////////////////////////////////////////////
typedef struct tagGameUnit
{
	DWORD dwUnitID;					// Unit ID
	DWORD dwUnitType;				// Unit type(player, monster, object, etc)
} GAMEUNIT, *LPGAMEUNIT;
typedef const tagGameUnit* LPCGAMEUNIT;

// Unit enumeration callback definition
typedef BOOL (CALLBACK *fnEnumUnitProc)(LPCGAMEUNIT lpUnit, LPARAM lParam);

// Player enumeration callback definition
typedef BOOL (CALLBACK *fnEnumPlayerProc)(DWORD dwPlayerID, LPCSTR lpszPlayerName, DWORD dwPlayerClass, DWORD dwPvpFlags, BYTE iMapID, LPARAM lParam);

//////////////////////////////////////////////////////////////////////
// ITEMPROPERTY
//--------------------------------------------------------------------
// Item properties(mods) data. Thanks to Mousepad for the help.
//////////////////////////////////////////////////////////////////////
typedef struct tagItemProperty
{
    WORD wSubProperty; // Sub property value
    WORD wPropertyID; // Property ID
    int nValue; // Property value
} ITEMPROPERTY, *LPITEMPROPERTY;
typedef const tagItemProperty* LPCITEMPROPERTY;


//////////////////////////////////////////////////////////////////////
// ITEMMOD
//--------------------------------------------------------------------
// Translated item properties(mods) data.
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
// Item Enumeration Callback Function def
//////////////////////////////////////////////////////////////////////
typedef BOOL (CALLBACK *fnEnumItemProc)(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam); 

#endif // __COMMONSTRUCTS_H__

