///////////////////////////////////////////////////////////////////////////////
// D2Npc.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////////////

#include "D2Npc.h"
#include "..\\Includes\\d2Hackit.h"
#include "..\\Includes\\d2client.h"

static const DWORD TOWNWPS[] = { 119, 156, 237, 398, 429, 0 }; // Town wp class ID's
static const DWORD STASH_CLASS = 267;

static const D2NPC NPCS[] = {

	// Act1 npc (act1 npc positions are relative to stash)
	{ 0, "Charis", 154, -33, -18 },
	{ 0, "Gheed", 147, -22, 46 },
	{ 0, "Akara", 148, 66, -20 },
	{ 0, "Kashya", 150, 28, 7 },
	{ 0, "Deckard Cain", 265, 16, 6 },

	// Act2 npc
	{ 1, "Elzix", 199, 5033, 5103 },
	{ 1, "Lysander", 202, 5115, 5103 },
	{ 1, "Fara", 178, 5116, 5079 },
	{ 1, "Drognan", 177, 5092, 5032 },
	{ 1, "Deckard Cain", 244, 5124, 5082 },
	{ 1, "Greiz", 198, 5037, 5054 },

	// Act3 npc
	{ 2, "Alkor", 254, 5084, 5017 },
	{ 2, "Asheara", 252, 5038, 5093 },
	{ 2, "Ormus", 255, 5124, 5092 },
	{ 2, "Hratil", 253, 5223, 5044, 5118, 5163 },
	{ 2, "Deckard Cain", 245, 5145, 5063 },

	// Act4 npc	
	{ 3, "Jamella", 405, 5086, 5048 },
	{ 3, "Halbu", 257, 5087, 5032 },
	{ 3, "Deckard Cain", 246, 5024, 5025 },
	{ 3, "Tyrael", 367, 5021, 5021 },

	// Act5 npc
	{ 4, "Larzuk", 511, 5143, 5042 },
	{ 4, "Malah", 513, 5075, 5026 },
	{ 4, "Anya", 512, 5108, 5117 },
	{ 4, "Deckard Cain", 520, 5119, 5066, 5081, 5086 },
	{ 4, "Qual-Kehk", 515, 5073, 5076 },

	// the end
	{ -1 }
};

BOOL GetTownWpPosition(LPPOINT lpBuffer)
{
	if (lpBuffer == NULL || !server->IsGameReady())
		return FALSE;

	*lpBuffer = server->FindPresetUnit(TOWNWPS[me->GetCurrentAct()], UNIT_TYPE_OBJECT);
	return lpBuffer->x && lpBuffer->y;
}

BOOL GetTownWp(LPGAMEUNIT lpBuffer)
{
	if (!server->IsGameReady())
		return FALSE;
	
	return server->FindUnitByClassID(TOWNWPS[me->GetCurrentAct()], UNIT_TYPE_OBJECT, lpBuffer);
}

BOOL GetStashPosition(LPPOINT lpBuffer)
{
	if (lpBuffer == NULL || !server->IsGameReady())
		return FALSE;

	*lpBuffer = server->FindPresetUnit(STASH_CLASS, UNIT_TYPE_OBJECT);
	return lpBuffer->x && lpBuffer->y;
}

BOOL GetStash(LPGAMEUNIT lpBuffer)
{
	if (!server->IsGameReady())
		return FALSE;

	return server->FindUnitByClassID(STASH_CLASS, UNIT_TYPE_OBJECT, lpBuffer);
}

BOOL GetNpc(DWORD dwNpcClassID, LPD2NPC lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;

	for (int i = 0; NPCS[i].iAct >= 0; i++)
	{
		if (NPCS[i].dwNpcClassID == dwNpcClassID)
		{
			::memcpy(lpBuffer, &NPCS[i], sizeof(D2NPC));
			if (lpBuffer->iAct == 0 && server->IsGameReady() && me->GetCurrentMapID() == MAP_A1_ROGUE_ENCAMPMENT)
			{
				POINT pt = { 0 };
				GetStashPosition(&pt);

				lpBuffer->x += pt.x;
				lpBuffer->y += pt.y;

				if (lpBuffer->x2)
					lpBuffer->x2 += pt.x;

				if (lpBuffer->y2)
					lpBuffer->y2 += pt.y;				
			}

			return TRUE;
		}
	}

	return FALSE;
}

int CalcPathAndGo(int x, int y)
{
	if (me->GetDistanceFrom(x, y) < 5)
	{
		me->MoveTo(x, y, TRUE);
		return 1;
	}
	
	POINT aPath[256];
	int nCount = (int)server->CalculateWalkPath(x, y, aPath, 256);
	for (int i = 1; i < nCount; i++)
		me->MoveTo(aPath[i].x, aPath[i].y, TRUE);
	return nCount;
}