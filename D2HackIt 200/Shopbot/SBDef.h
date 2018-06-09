//////////////////////////////////////////////////////////////
// SBDef.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////

#ifndef __SBDEF_H__
#define __SBDEF_H__

#include <windows.h>

typedef struct tagShop
{
	BYTE iAct;				// Act 0-4
	char szNpcName[64];		// Npc name
	DWORD dwNpcClassID;		// Npc class ID
	BOOL bIsHealer;			// Npc is a healer
	BOOL bIsGambler;		// Npc is a gambler
	int x;					// Npc estimate position x
	int y;					// Npc estimate position x
} SHOP, *LPSHOP;
typedef const tagShop* LPCSHOP;

static const DWORD TOWNWPS[] = { 119, 156, 237, 398, 429, 0 }; // Town wp class ID's
static const SHOP SHOPS[] = {

	// Act1 Shops (act1 npc positions are relative to stash)
	{ 0, "Charis", 154, FALSE, FALSE, -33, -18 },
	{ 0, "Gheed", 147, FALSE, TRUE, -22, 46 },
	{ 0, "Akara", 148, TRUE, FALSE, 66, -20 },

	// Act2 Shops
	{ 1, "Elzix", 199, FALSE, TRUE, 5033, 5103 },
	{ 1, "Lysander", 202, FALSE, FALSE, 5115, 5103 },
	{ 1, "Fara", 178, TRUE, FALSE, 5116, 5079 },
	{ 1, "Drognan", 177, FALSE, FALSE, 5092, 5032 },

	// Act3 Shops
	{ 2, "Alkor", 254, FALSE, TRUE, 5084, 5017 },
	{ 2, "Asheara", 252, FALSE, FALSE, 5038, 5093 },
	{ 2, "Ormus", 255, TRUE, FALSE, 5124, 5092 },
	{ 2, "Hratil", 253, FALSE, FALSE, 5223, 5044 },

	// Act4 Shops	
	{ 3, "Jamella", 405, TRUE, TRUE, 5086, 5048 },
	{ 3, "Halbu", 257, FALSE, FALSE, 5087, 5032 },

	// Act5 Shops
	{ 4, "Larzuk", 511, FALSE, FALSE, 5143, 5042 },
	{ 4, "Malah", 513, TRUE, FALSE, 5075, 5026 },
	{ 4, "Anya", 512, FALSE, TRUE, 5108, 5117 },

	// the end
	{ -1 }
};

BOOL GetShop(DWORD dwClassID, LPSHOP lpBuffer, BOOL bNeedPosition);

#endif // __SBDEF_H__