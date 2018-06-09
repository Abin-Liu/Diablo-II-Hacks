/////////////////////////////////////////////////////////////////////
// ItemMod.h
//
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////////

#ifndef __ITEMMOD_H__
#define __ITEMMOD_H__

#include <windows.h>

typedef struct tagItemMod
{
	DWORD dwModID;			// Mod ID, could be different from property ID
	DWORD dwOriginalID;		// Original property ID
	int aValues[4];			// Mod values
	BOOL bFloat;			// Is float value
} ITEMMOD, *LPITEMMOD;
typedef const tagItemMod* LPCITEMMOD;

#endif // __ITEMMOD_H__