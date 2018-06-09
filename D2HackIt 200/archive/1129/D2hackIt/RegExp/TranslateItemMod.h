////////////////////////////////////////////////////////////////////////////
// TranslateItemMod.h
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////////////

#ifndef __TRANSLATEITEMMOD_H__
#define __TRANSLATEITEMMOD_H__

#include <windows.h>
#include "CommonStructs.h"

typedef struct tagItemMod
{
	DWORD dwModID;			// Mod ID, could be different from property ID
	DWORD dwOriginalID;		// Original property ID
	int aValues[4];			// Mod values
	BOOL bFloat;			// Is float value
} ITEMMOD, *LPITEMMOD;
typedef const tagItemMod* LPCITEMMOD;

BOOL TranslateItemMod(LPCITEMPROPERTY lpProperty, LPCITEMPROPERTY aList, DWORD dwCount, LPITEMMOD lpBuffer);

#endif // __TRANSLATEITEMMOD_H__
