//////////////////////////////////////////////////////////////
// SBDef.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////

#include "SBDef.h"
#include "..\\includes\\d2hackit.h"
#include "..\\Includes\\D2Client.h"

#define STASH_CLASS		267

BOOL GetShop(DWORD dwClassID, LPSHOP lpBuffer, BOOL bNeedPosition)
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(SHOP));
	
	for (int i = 0; SHOPS[i].szNpcName; i++)
	{
		if (SHOPS[i].dwNpcClassID == dwClassID)
		{
			::memcpy(lpBuffer, &SHOPS[i], sizeof(SHOP));
			if (bNeedPosition && lpBuffer->iAct == 0)
			{
				// its act1, positions are relative to stash
				POINT pt = server->FindPresetUnit(STASH_CLASS, UNIT_TYPE_OBJECT);
				lpBuffer->x += pt.x;
				lpBuffer->y += pt.y;
			}
			return TRUE;
		}
	}

	return FALSE;
}