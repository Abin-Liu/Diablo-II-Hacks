///////////////////////////////////////////////////////////////////////////////
// D2Npc.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////////////

#ifndef __D2NPC_H__
#define __D2NPC_H__

#include <windows.h>
#include "..\\Includes\\CommonStructs.h"

// Npc class ID
#define NPC_A1_CHARIS	154
#define NPC_A1_GHEED	147
#define NPC_A1_AKARA	148
#define NPC_A1_KASHYA	150
#define NPC_A1_CAIN		265
#define NPC_A2_ELZIX	199
#define NPC_A2_LYSANDER	202
#define NPC_A2_FARA		178
#define NPC_A2_DROGNAN	177
#define NPC_A2_GREIZ	198
#define NPC_A2_CAIN		244
#define NPC_A3_ALKOR	254
#define NPC_A3_ASHEARA	252
#define NPC_A3_ORMUS	255
#define NPC_A3_HRATIL	253
#define NPC_A3_CAIN		245
#define NPC_A4_JAMELLA	405
#define NPC_A4_HALBU	257
#define NPC_A4_TYRAEL	367
#define NPC_A4_CAIN		246
#define NPC_A5_LARZUK	511
#define NPC_A5_MALAH	513
#define NPC_A5_ANYA		512
#define NPC_A5_QUAL		515
#define NPC_A5_CAIN		520

typedef struct tagD2Npc
{
	BYTE iAct;				// Act 0-4
	char szNpcName[64];		// Npc name
	DWORD dwNpcClassID;		// Npc class ID
	int x;					// Npc estimate position x
	int y;					// Npc estimate position x
	int x2;					// Npc alternative position x
	int y2;					// Npc alternative position y
} D2NPC, *LPD2NPC;
typedef const tagD2Npc* LPCD2NPC;

BOOL GetTownWpPosition(LPPOINT lpBuffer);
BOOL GetTownWp(LPGAMEUNIT lpBuffer);
BOOL GetStashPosition(LPPOINT lpBuffer);
BOOL GetStash(LPGAMEUNIT lpBuffer);
BOOL GetNpc(DWORD dwNpcClassID, LPD2NPC lpBuffer);
int CalcPathAndGo(int x, int y);

#endif // __D2NPC_H__
