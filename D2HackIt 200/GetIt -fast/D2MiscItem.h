//////////////////////////////////////////////////////////////
// D2MiscItem.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////

#ifndef __D2MISCITEM_H__
#define __D2MISCITEM_H__

#include <windows.h>

///////////////////////////////////////////////////////////////
// Misc Items and Their Needed/Not-Needed Criteria:
//-------------------------------------------------------------
// Gold:					Inventory can store more gold.
// Healing potions:			Belt can store more same potions.
// Mana potions:
// Rejuvenation potions:
// Antidote potions:
// Stamina potions:
// Thawing potions:
// Scroll of town portal:	Belt can store more, or inventory has non-full tomes.
// Scroll of identify:
//////////////////////////////////////////////////////////////// 
#define MISC_NOT_MISC	-1 // not a misc item
#define MISC_NOT_NEED	0  // misc item, not needed
#define MISC_NEED		1  // misc item, needed

DWORD GetMiscType(LPCSTR lpszItemCode);
BOOL IsGold(LPCSTR lpszItemCode);
int MiscItemNeeded(DWORD dwType, DWORD dwGoldAmount = 0);

#endif // __D2MISCITEM_H__