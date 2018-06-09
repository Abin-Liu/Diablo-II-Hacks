///////////////////////////////////////////////////////////////////////
// Server20.cpp
//
// Implememtation of the "server" API set functions that were added in
// D2Hackit 2.0
//
// Abin(abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////
#include "server20.h"
#include "D2HackItServer.h"
#include "utilities\\LinkedList.h"
#include "utilities\\ArrayEx.h"
#include "utilities\\StringEx.h"
#include "d2functions.h"
#include "definitions.h"
#include "D2HackIt.h"
#include "MapDef.h"
#include "AffectionDef.h"
#include "me.h"
#include <math.h>
#include <stdlib.h>
#include "SpellDef.h"
#include "KeyDef.h"
#include "Inventory.h"
#include "Packet.h"
#include "ItemCheck.h"
#include "GameMessage.h"
#include "NpcSession.h"
#include "ServerDraw.h"
#include "D2Stdio.h"
#include "PathAlgorithms\\Common.h"
#include "PathAlgorithms\\TeleportPath.h"
#include "PathAlgorithms\\CollisionMap.h"
#include "PathAlgorithms\\TeleportPath.h"
#include "PathAlgorithms\\WalkPath.h"
#include "DrawImage.h"

typedef struct tagPatchData
{
	LPVOID lpDest; // Destination
	int nCodeLen; // Code length, must be 1-4
	union {
		DWORD dwCode; // Patch code
		LPBYTE lpOldCode;
	};
} PATCHDATA, *LPPATCHDATA;
typedef const tagPatchData* LPCPATCHDATA;

//BOOL g_bKeepD2WndOn = FALSE;
DWORD g_dwGameTime = 0; // total time elapsed since game joining
PacketQueue g_outgoingPackets; // Outgoing packets
DWORD g_dwLastOutgoing = 0; // Last outgoing packet sent time
BOOL g_bLoadedBeforeJoin = FALSE; // d2Hackit loaded before joining game?
CCollisionMap g_collisionMap;
CTickThread g_clientTickThread;
CTickThread g_serverTickThread;
CTickThread	g_jobTickThread;

BOOL WINAPI MySetCursorPos(int x, int y);

// Our patchs
//PATCHDATA g_KeepD2WndPatch1 = { 0, 1, SW_SHOWNOACTIVATE };
//PATCHDATA g_KeepD2WndPatch2 = { 0, 1, SW_SHOWNOACTIVATE };
//PATCHDATA g_KeepD2WndPatch3 = { 0, 1, SW_SHOWNOACTIVATE };
PATCHDATA g_GameOnDrawPatch = { 0, 4, 0 };
PATCHDATA g_ChangeActPatch1 = { 0, 5, 0 };
PATCHDATA g_ChangeActPatch2 = { 0, 5, 0 };
PATCHDATA g_SetCursorPosPatch1 = { 0, 4, (DWORD)MySetCursorPos };
PATCHDATA g_SetCursorPosPatch2 = { 0, 4, (DWORD)MySetCursorPos };

BOOL Patch_Unpatch(LPPATCHDATA lpPatch);
BOOL IsValidIndex(POINT pt, SIZE cz);
void LoadTypeModDefinition(LPCSTR lpszIniFile); // ExamItemProperties.cpp
void CALLBACK ServerTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag); // Server20.cpp
void CALLBACK ClientTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag); // Server20.cpp
void CALLBACK JobTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag); // MotionJob.cpp
BOOL CALLBACK AppEnumUnitsProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2);
BOOL CALLBACK FindUnitNameProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2);
BOOL CALLBACK FindPlayerNameProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2);
BOOL CALLBACK FindUnitClassProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2);
BOOL CALLBACK AppEnumGroundItemProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2);

void ChangeActPatch1();
void ChangeActPatch2();
DWORD GetItemColorHeader(DWORD dwItemID, LPSTR lpszBuffer);
DWORD GetItemRequirement(DWORD dwItemID, int nStatID);
DWORD GetItemReqRecursive(int nMin, int nMax, UnitItem* pItem, UnitAny* pUnit, Stats* pStats, DWORD* p1, DWORD* p2, DWORD* p3, DWORD* pMatch);

BOOL CALLBACK AppEnumUnitsProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2)
{
	if (pUnit == NULL)
		return FALSE;

	fnEnumUnitProc lpfn = (fnEnumUnitProc)lParam1;
	if (lpfn == NULL)
		return FALSE;

	GAMEUNIT gu;
	gu.dwUnitID = pUnit->dwId;
	gu.dwUnitType = pUnit->dwType;
	return lpfn(&gu, lParam2);
}

BOOL CALLBACK AppEnumGroundItemProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2)
{
	if (pUnit == NULL)
		return FALSE;
	
	fnEnumItemProc lpfn = (fnEnumItemProc)lParam1;
	if (lpfn == NULL)
		return FALSE;

	if (pUnit->dwMode != ITEM_MODE_ON_GROUND || pUnit->hOPath == NULL)
		return TRUE; // not a ground item

	ItemTxt* ptxt = D2COMMON_GetItemTxt(pUnit->dwClassId);
	if (ptxt == NULL)
		return TRUE; // should never happen but just in case

	char szCode[4];
	::memcpy(szCode, ptxt->szCode, 3);
	szCode[3] = '\0';
	return lpfn(pUnit->dwId, szCode, (WORD)pUnit->hOPath->Xpos, (WORD)pUnit->hOPath->Ypos, lParam2);
}

BOOL CALLBACK FindUnitNameProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2)
{
	if (pUnit == NULL)
		return FALSE;

	LPCWSTR lpwszName = (LPCWSTR)lParam1;
	if (lpwszName == NULL || *lpwszName == 0)
		return FALSE;

	LPGAMEUNIT pBuffer = (LPGAMEUNIT)lParam2;
	if (pBuffer == NULL)
		return FALSE;

	LPCWSTR lpwsz = D2CLIENT_GetNameFromUnit(pUnit);
	if (lpwsz == NULL)
		return TRUE;

	if (::wcsicmp(lpwszName, lpwsz) == 0)
	{
		pBuffer->dwUnitID = pUnit->dwId;
		pBuffer->dwUnitType = pUnit->dwType;
		return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK FindUnitClassProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2)
{
	if (pUnit == NULL)
		return FALSE;

	LPGAMEUNIT pBuffer = (LPGAMEUNIT)lParam2;
	if (pBuffer == NULL)
		return FALSE;

	if (pUnit->dwClassId == (DWORD)lParam1)
	{
		pBuffer->dwUnitID = pUnit->dwId;
		pBuffer->dwUnitType = pUnit->dwType;
		return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK FindPlayerNameProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2)
{
	if (pUnit == NULL)
		return FALSE;

	LPGAMEUNIT pBuffer = (LPGAMEUNIT)lParam2;
	if (pBuffer == NULL)
		return FALSE;

	LPCSTR lpszName = (LPCSTR)lParam1;
	if (lpszName == NULL || *lpszName == 0)
		return FALSE;

	// If its a player unit then get the name from the player data structure
	// thanks to Henrik Noerregaard Joergensen<hnj@kalliope.org>
	if (pUnit->ptPlayerData == NULL)
		return TRUE;

	if (::stricmp(lpszName, pUnit->ptPlayerData->PlayerName) == 0)
	{
		pBuffer->dwUnitID = pUnit->dwId;
		pBuffer->dwUnitType = pUnit->dwType;
		return FALSE;
	}

	return TRUE;
}

BOOL Patch_Unpatch(LPPATCHDATA lpPatch)
{
	if (lpPatch == NULL)
		return FALSE;
	
	return PatchFill(lpPatch->lpDest, &lpPatch->dwCode, &lpPatch->dwCode, lpPatch->nCodeLen);
}

BOOL Patch_Unpatch2(LPPATCHDATA lpPatch, int instruction=0, DWORD lpSource=0)
{
	//	DGTRACE((DBGHDR "Intercept(%X, %X, %d)...\n", lpSource, lpDest, len));

	if (!lpPatch)
		return FALSE;
	if (instruction && lpSource)
	{
		lpPatch->lpOldCode = new BYTE[lpPatch->nCodeLen];
		lpPatch->lpOldCode[0] = instruction;
		*(DWORD*) (lpPatch->lpOldCode + 1) = lpSource - ((DWORD)lpPatch->lpDest + 5);
		PatchFill(lpPatch->lpDest, lpPatch->lpOldCode, lpPatch->lpOldCode, lpPatch->nCodeLen);
	}
	else
	{
		PatchFill(lpPatch->lpDest, lpPatch->lpOldCode, lpPatch->lpOldCode, lpPatch->nCodeLen);
		delete [] lpPatch->lpOldCode;
	}

	return TRUE;
}

BOOL Server20Start()
{
	// v2.00

	// Create threads, v2.00
	g_serverTickThread.Start(ServerTimerTick, (LPVOID)&ClientModules);
	g_clientTickThread.Start(ClientTimerTick, (LPVOID)&ClientModules);
	g_jobTickThread.Start(JobTimerTick, (LPVOID)&ClientModules);

	// Install KeepD2Wnd patchs
	//g_KeepD2WndPatch1.lpDest = (LPVOID)D2GFX_KeepD2WindowPatch1;
	//g_KeepD2WndPatch2.lpDest = (LPVOID)D2GFX_KeepD2WindowPatch2;
	//g_KeepD2WndPatch3.lpDest = (LPVOID)D2GFX_KeepD2WindowPatch3;

	// Install Change Act patchs
	g_ChangeActPatch1.lpDest = (LPVOID)D2CLIENT_ChangeActPatch1;
	g_ChangeActPatch2.lpDest = (LPVOID)D2CLIENT_ChangeActPatch2;
	Patch_Unpatch2(&g_ChangeActPatch1, INST_CALL, (DWORD)ChangeActPatch1);
	Patch_Unpatch2(&g_ChangeActPatch2, INST_JMP, (DWORD)ChangeActPatch2);

	// Install SetCursorPosPatch
	g_SetCursorPosPatch1.lpDest = (LPVOID)D2CLIENT_SetCursorPosPatch;
	g_SetCursorPosPatch2.lpDest = (LPVOID)D2WIN_SetCursorPosPatch;
	Patch_Unpatch(&g_SetCursorPosPatch1);
	Patch_Unpatch(&g_SetCursorPosPatch2);

	// Install GameOnDraw patch
	g_GameOnDrawPatch.lpDest = (LPVOID)(D2CLIENT_ScreenPrint + 1);
	g_GameOnDrawPatch.dwCode = (DWORD)&GameOnDrawIntercept - (D2CLIENT_ScreenPrint + 1 + 4); // offset from d2bs
	Patch_Unpatch(&g_GameOnDrawPatch);

	// load item mod definition from "D2Items.ini"
	char szPath[MAX_PATH + 1] = "";
	sprintf(szPath, "%s\\D2Items.ini", GetModuleDirectory());
	LoadTypeModDefinition(szPath);

	if (thisgame->player)
		Server20GameJoin(thisgame);

	return TRUE;
}

BOOL Server20Stop()
{
	Server20GameLeave();	 // v2.00
	
	// End threads first!
	g_serverTickThread.Stop(2500);
	g_clientTickThread.Stop(2500);
	g_jobTickThread.Stop(2500);

	Patch_Unpatch(&g_SetCursorPosPatch1);
	Patch_Unpatch(&g_SetCursorPosPatch2);

	Patch_Unpatch2(&g_ChangeActPatch1);
	Patch_Unpatch2(&g_ChangeActPatch2);

	// Remove GameOnDraw Patch
	Patch_Unpatch(&g_GameOnDrawPatch);

	// Remove KeepD2Wnd Patchs
	//if (g_bKeepD2WndOn)
	//{
	//	Patch_Unpatch(&g_KeepD2WndPatch1);
	//	Patch_Unpatch(&g_KeepD2WndPatch2);
	//	Patch_Unpatch(&g_KeepD2WndPatch3);
	//}
	
	return TRUE;
}

VOID Server20GameJoin(THISGAMESTRUCT* thisgame)
{
	g_dwGameTime = 0;	
	g_dwLastOutgoing = 0;
	Player_OnGameJoin(thisgame);
	ServerDrawOnGameJoin();
	MessageReset();
}

VOID Server20GameLeave()
{		
	g_collisionMap.DestroyMap();
	g_dwGameTime = 0;
	g_bLoadedBeforeJoin = FALSE;
	Player_OnGameLeave();

	g_outgoingPackets.Lock();
	g_outgoingPackets.RemoveAll();
	g_outgoingPackets.Unlock();
	g_dwLastOutgoing = 0;
	MessageReset();
	ServerDrawOnGameLeave();
}

void CALLBACK ServerTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag)
{
	if (!IsGameJoined())
		return;

	g_dwGameTime += 100;	

	if (!NEED_ABORT(lpStopFlag))
		PlayerTimerTick(lpParam, lpStopFlag);	

	if (!NEED_ABORT(lpStopFlag))
		ProcessOutgoingPackets(lpStopFlag);	
}

void CALLBACK ClientTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag)
{
	if (!IsGameJoined())
		return;
	
	CClients* cl = (CClients*)lpParam;
	if (cl && IsGameJoined())
	{
		LinkedItem *li = cl->GetFirstItem();
		while(li && !NEED_ABORT(lpStopFlag))
		{
			PCIS pCIS = cl->GetCIS(li);
			if (pCIS && pCIS->OnGameTimerTick)
				pCIS->OnGameTimerTick(); // old timer tick function

			li = cl->GetNextItem(li);
		}
	}	
}

BOOL ProcessOutgoingPackets(const volatile BYTE* lpStopFlag)
{
	if (NEED_ABORT(lpStopFlag))
		return FALSE;

	//////////////////////////////////////////////////////
	// Send "batch packets" one at a time every 100 ms.
	// This will avoid getting connection interrupted due to
	// sending too many packets simultaneously.
	// Used for batch selling, batch buying, batch gambling
	// and batch potion drinking, etc.
	//////////////////////////////////////////////////////
	g_outgoingPackets.Lock();

	const DWORD NOW = ::GetTickCount();
	POSITION pos = g_outgoingPackets.GetHeadPosition();
	while (pos && !NEED_ABORT(lpStopFlag))
	{	
		POSITION cur = pos;
		CPacket* pkt = g_outgoingPackets.GetNext(pos);		

		if (pkt == NULL || !pkt->IsValidPacket())
		{
			g_outgoingPackets.RemoveAt(cur);
			continue; // bad packet, throw away
		}		
		
		if (NOW >= pkt->GetDelay() + g_dwLastOutgoing)
		{
			if (pkt->SendToServer())			
				g_dwLastOutgoing = NOW; // send successfully, record the time

			g_outgoingPackets.RemoveAt(cur);
		}

		break;		
	}
	
	g_outgoingPackets.Unlock();
	return TRUE;
}

BOOL EXPORT VerifyUnit(LPCGAMEUNIT lpUnit)
{
	if (lpUnit == NULL)
		return FALSE;

	return (BOOL)D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
}

BOOL EXPORT FindUnitByClassID(DWORD dwUnitClassID, DWORD dwUnitType, LPGAMEUNIT lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(GAMEUNIT));
	InternalEnumUnits(dwUnitType, FindUnitClassProc, dwUnitClassID, (LPARAM)lpBuffer);
	return lpBuffer->dwUnitID;
}

BOOL EXPORT FindUnitByName(LPCSTR lpszUnitName, DWORD dwUnitType, LPGAMEUNIT lpBuffer)
{
	if (lpszUnitName == NULL || !lpszUnitName[0] || lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(GAMEUNIT));

	// If its a player unit then get the name from the player data structure
	// thanks to Henrik Noerregaard Joergensen<hnj@kalliope.org>
	if (dwUnitType == UNIT_TYPE_PLAYER)
	{
		InternalEnumUnits(dwUnitType, FindPlayerNameProc, (LPARAM)lpszUnitName, (LPARAM)lpBuffer);
	}
	else
	{
		// convert to unicode first
		wchar_t wszName[256] = {0};
		ConvertToUnicode(lpszUnitName, wszName, 255);
		InternalEnumUnits(dwUnitType, FindUnitNameProc, (LPARAM)wszName, (LPARAM)lpBuffer);
	}
	
	return lpBuffer->dwUnitID;
}

LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen)
{
	::MultiByteToWideChar(CP_ACP, 0, lpszAnsiString, -1, lpUnicodeString, maxlen);
	return lpUnicodeString;
}

LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen)
{
	::WideCharToMultiByte(CP_ACP, 0, lpUnicodeString, -1, lpszAnsiString, maxlen, NULL, NULL);
	return lpszAnsiString;
}

DWORD EXPORT GetUnitName(LPCGAMEUNIT lpUnit, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpUnit == NULL || lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars);
	UnitAny* pUnit = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (pUnit == NULL)
		return 0;

	// If its a player unit then get the name from the player data structure
	// thanks to Henrik Noerregaard Joergensen<hnj@kalliope.org>
	if (pUnit->dwType == UNIT_TYPE_PLAYER)
	{
		if (pUnit->ptPlayerData == NULL)
			return 0;

		strncpy(lpszBuffer, pUnit->ptPlayerData->PlayerName, dwMaxChars);
		return strlen(lpszBuffer);
	}

	wchar_t* wszName = D2CLIENT_GetNameFromUnit(pUnit);
	if (wszName == NULL)
		return 0;

	ConvertFromUnicode(wszName, lpszBuffer, dwMaxChars);
	return strlen(lpszBuffer);
}

POINT EXPORT GetUnitPosition(LPCGAMEUNIT lpUnit)
{
	POINT mp = { 0 };
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
	{
		UnitError(lpUnit, "GetUnitPosition");
		return mp;
	}

	if (lpUnit->dwUnitType == UNIT_TYPE_PLAYER
		|| lpUnit->dwUnitType == UNIT_TYPE_MONSTER
		|| lpUnit->dwUnitType == UNIT_TYPE_MISSILE)
	{
		mp.x = D2COMMON_GetUnitX(p->hPath);
		mp.y = D2COMMON_GetUnitY(p->hPath);		
	}
	else
	{
		mp.x = p->hOPath->Xpos;
		mp.y = p->hOPath->Ypos;
	}

	return mp;
}

DWORD EXPORT GetUnitOwnerID(LPCGAMEUNIT lpUnit)
{
	if (lpUnit == NULL)
		return -1;

	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
	{
		UnitError(lpUnit, "GetUnitOwnerID");
		return -1;
	}

	if (lpUnit->dwUnitType == UNIT_TYPE_MONSTER)
		return D2CLIENT_GetMonsterOwner(lpUnit->dwUnitID);

	if (lpUnit->dwUnitType == UNIT_TYPE_OBJECT)
	{
		for (Roster* pRoster = *p_D2CLIENT_FirstRosterUnit; pRoster; pRoster = pRoster->ptNext)
		{
			if (!::stricmp(pRoster->Name, p->ptObjectData->parent))
				return pRoster->PlayerId;
		}
	}

	if (lpUnit->dwUnitType == UNIT_TYPE_ITEM)
	{
		if (p->ptItemData->ptOwnerInv && p->ptItemData->ptOwnerInv->ptOwner)
			return p->ptItemData->ptOwnerInv->ptOwner->dwId;
	}

	return -1;
}

DWORD EXPORT GetUnitClassID(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
	{
		UnitError(lpUnit, "GetUnitClassID");
		return 0;
	}

	return p->dwClassId;
}

DWORD EXPORT GetUnitStat(LPCGAMEUNIT lpUnit, int nStatID)
{
	return ServerGetUnitStat((UnitAny*)VerifyUnit(lpUnit), nStatID, FALSE);
}

BYTE EXPORT GetUnitHPPercent(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
	{
		UnitError(lpUnit, "GetUnitHPPercent");
		return 0;
	}

	if (p == D2CLIENT_GetPlayerUnit())
		return CalcPercent(ServerGetUnitStat(p, STAT_HP, FALSE), ServerGetUnitStat(p, STAT_MAXHP, FALSE), 1);
	else
		return CalcPercent(ServerGetUnitStat(p, STAT_HP, FALSE) / 256, 128, 1);
}

BOOL EXPORT GetUnitAffection(LPCGAMEUNIT lpUnit, int nAffectionID)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
	{
		UnitError(lpUnit, "GetUnitAffection");
		return 0;
	}

	return D2COMMON_GetUnitState(p, nAffectionID);
}

int EXPORT GetMonsterType(LPCGAMEUNIT lpUnit) // normal/champion/boss/minion
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL || p->ptMonsterData == NULL)
	{
		UnitError(lpUnit, "GetMonsterType");
		return MONSTER_UNKNOWN;	
	}

	if (p->ptMonsterData->fNormal & 1)
		return MONSTER_NORMAL;

	if (p->ptMonsterData->fChamp & 1)
		return MONSTER_CHAMPION;

	if (p->ptMonsterData->fBoss & 1)
		return MONSTER_BOSS;

	if (p->ptMonsterData->fMinion & 1)
		return MONSTER_MINION;

	return MONSTER_UNKNOWN;
}

DWORD EXPORT GetMonsterAttributes(LPCGAMEUNIT lpUnit) // monster attributes
{
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL || p->ptMonsterData == NULL)
	{
		UnitError(lpUnit, "GetMonsterAttributes");
		return 0; // not a monster
	}

	DWORD dwResult = 0x80000000; // non-zero even if the unit has no matched attributes

	// get boss enchants
	for (int i = 0; i < 9 && p->ptMonsterData->anEnchants[i]; i++)
	{
		switch (p->ptMonsterData->anEnchants[i])
		{
		case 0x05: // Extra Strong
			dwResult |= MA_EXTRA_STRONG;
			break;

		case 0x06: // Extra Fast
			dwResult |= MA_EXTRA_FAST;
			break;

		case 0x1a: // Teleport
			dwResult |= MA_TELEPORT;
			break;

		case 0x1c: // Stone Skin
			dwResult |= MA_STONE_SKIN;
			break;

		case 0x1b: // Spectral Hit
			dwResult |= MA_SPECTRAL_HIT;
			break;

		case 0x09: // Fire Enchanted
			dwResult |= MA_FIRE_ENCHANTED;
			break;

		case 0x11: // Lightning Enchanted
			dwResult |= MA_LIGHTNING_ENCHANTED;
			break;

		case 0x12: // Cold Enchanted
			dwResult |= MA_COLD_ENCHANTED;
			break;

		case 0x1d: // Multishot
			dwResult |= MA_MULTI_SHOT;
			break;

		case 0x1e: // Aura Enchanted
			dwResult |= MA_AURA_ENCHANTED;
			break;

		case 0x08: // Magic resistant
			dwResult |= MA_MAGIC_RESISTANT;
			break;

		case 0x19: // Mana Burn
			dwResult |= MA_MANA_BURN;
			break;

		case 0x07: // Cursed
			dwResult |= MA_CURSED;
			break;

		default:
			break;
		}
	}

	// get auras
	if (D2COMMON_GetUnitState(p, AFFECT_MIGHT))
		dwResult |= MA_MIGHT;

	if (D2COMMON_GetUnitState(p, AFFECT_HOLYFIRE))
		dwResult |= MA_HOLY_FIRE;

	if (D2COMMON_GetUnitState(p, AFFECT_HOLYFREEZE))
		dwResult |= MA_HOLY_FREEZE;

	if (D2COMMON_GetUnitState(p, AFFECT_HOLYSHOCK))
		dwResult |= MA_HOLY_SHOCK;

	if (D2COMMON_GetUnitState(p, AFFECT_CONVICTION))
		dwResult |= MA_CONVICTION;

	if (D2COMMON_GetUnitState(p, AFFECT_FANATICISM))
		dwResult |= MA_FANATICISM;

	if (D2COMMON_GetUnitState(p, AFFECT_BLESSEDAIM))
		dwResult |= MA_BLESSED_AIM;

	// get immunities
	if (ServerGetUnitStat(p, 36, FALSE) >= 100)
		dwResult |= MA_IMMUNE_PHYSICAL;

	if (ServerGetUnitStat(p, 37, FALSE) >= 100)
		dwResult |= MA_IMMUNE_MAGIC;

	if (ServerGetUnitStat(p, 39, FALSE) >= 100)
		dwResult |= MA_IMMUNE_FIRE;

	if (ServerGetUnitStat(p, 41, FALSE) >= 100)
		dwResult |= MA_IMMUNE_LIGHTNING;

	if (ServerGetUnitStat(p, 43, FALSE) >= 100)
		dwResult |= MA_IMMUNE_COLD;

	if (ServerGetUnitStat(p, 45, FALSE) >= 100)
		dwResult |= MA_IMMUNE_POISON;

	return dwResult;
}

DWORD EXPORT GetDistance(long x1, long y1, long x2, long y2) // distance between two positions, in yards
{
	return (DWORD)CalculateDistance(x1, y1, x2, y2);
}

DWORD EXPORT IsMercUnit(LPCGAMEUNIT lpUnit) // Check if an unit is a merc
{
	if (lpUnit == NULL || lpUnit->dwUnitType != UNIT_TYPE_MONSTER || !IsMercClassID(GetUnitClassID(lpUnit)))
		return 0;

	DWORD dwOwnerID = GetUnitOwnerID(lpUnit);
	return dwOwnerID == -1 ? 0 : dwOwnerID;
}

BOOL IsMercClassID(DWORD dwClassID)
{
	return dwClassID == MERC_A1 || dwClassID == MERC_A2 || dwClassID == MERC_A3 || dwClassID == MERC_A5;
}

DWORD EXPORT GetPing()
{
	WORD wPing = 0;
	if (!::ReadProcessMemory(::GetCurrentProcess(), (LPCVOID)0x6FBA7774, &wPing, 2, NULL))
		return 0;
	return wPing;	
}

BOOL InternalEnumUnits(DWORD dwUnitType, fnInternalEnumUnitProc lpfn, LPARAM lParam1, LPARAM lParam2)
{
	if (lpfn == NULL)
		return FALSE;

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return FALSE;

	Room* pRoom = D2COMMON_GetRoomFromUnit(pPlayer);
	
	if (pRoom == NULL || pRoom->ptAct == NULL)
		return FALSE; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (dwUnitType == pUnit->dwType)
			{
				if (!lpfn(pPlayer, pRoom, pUnit, lParam1, lParam2))
					return FALSE;
			}			
		}
	}

	return TRUE;
}

BOOL EXPORT EnumUnits(DWORD dwUnitType, fnEnumUnitProc lpfnEnumUnitProc, LPARAM lParam)
{
	if (lpfnEnumUnitProc == NULL)
		return FALSE;

	return InternalEnumUnits(dwUnitType, AppEnumUnitsProc, (LPARAM)lpfnEnumUnitProc, lParam);
}

BOOL EXPORT WasD2HackitLoadedBeforeJoinGame()
{
	return g_bLoadedBeforeJoin;
}

DWORD EXPORT GetAffectionName(BYTE iAffectionID, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	return D2GetAffectionName(iAffectionID, lpszBuffer, dwMaxChars);
}

BOOL EXPORT AffectionIsAura(BYTE iAffectionID)
{
	return D2IsAura(iAffectionID);
}

DWORD EXPORT GetMapName(BYTE iMapID, LPTSTR lpszBuffer, DWORD dwMaxChars) // Name of the map
{
	return D2GetMapName(iMapID, lpszBuffer, dwMaxChars);
}

BOOL EXPORT MapIsTown(BYTE iMapID) // Is town?
{
	return D2MapIsTown(iMapID);
}

BYTE EXPORT GetTownMap(BYTE iAct)
{
	return D2GetTownMap(iAct);
}

BYTE EXPORT GetActFromMap(BYTE iMapID)
{
	return D2GetActFromMap(iMapID);
}

SIZE EXPORT GetItemSize(LPCSTR lpszItemCode)
{
	// This function is obsolete, do not use!
	GameErrorf("Function obsolete: ÿc8SIZE GetItemSize(LPCSTR lpszItemCode)");
	GameErrorf("Please use: ÿc9SIZE GetItemSize2(DWORD dwItemID)");
	SIZE sz = { 0 };
	return sz;
}

BOOL EXPORT SendKey(BYTE iVKCode) // Send a key-down message to d2 window
{
	//CMsgSender sender;
//	sender.SetTargetHWnd(g_hD2);
//	return sender.SendSysKey(iVKCode);

	char keybuf[5] = "";
	char oemchar[3] = "  ";	 // 2 character buffer
	::CharToOem(keybuf, &oemchar[0]);
	DWORD scan = ::OemKeyScan(oemchar[0]) & 0xFF;
	DWORD scancode = (::MapVirtualKey(iVKCode, 0) << 16);

	return ::PostMessage(g_hD2, WM_KEYDOWN, (WPARAM)iVKCode, (LPARAM)(scan | 0x00000001))
		&& ::PostMessage(g_hD2, WM_KEYUP, (WPARAM)iVKCode, (LPARAM)(scan | 0xC0000001));

}

BOOL EXPORT LeftClick(long x, long y)
{
	LPARAM pos = ((short)y << 16) + (short)x;  
	HWND hFG = ::GetForegroundWindow();
	::SetForegroundWindow(g_hD2);
	POINT pt;
	::GetCursorPos(&pt);
	::PostMessage(g_hD2, WM_MOUSEMOVE, NULL, pos);  
	::PostMessage(g_hD2, WM_LBUTTONDOWN, MK_LBUTTON, pos);  
	::PostMessage(g_hD2, WM_LBUTTONUP, MK_LBUTTON, pos);
	::SetCursorPos(pt.x, pt.y);
	::SetForegroundWindow(hFG);
	return TRUE;
}

BOOL EXPORT RightClick(long x, long y)
{
	LPARAM pos = ((short)y << 16) + (short)x;  
	HWND hFG = GetForegroundWindow();
	SetForegroundWindow(g_hD2);
	POINT pt;
	::GetCursorPos(&pt);
	SendMessage(g_hD2, WM_MOUSEMOVE, NULL, pos);  
	SendMessage(g_hD2, WM_RBUTTONDOWN, MK_RBUTTON, pos);  
	SendMessage(g_hD2, WM_RBUTTONUP, MK_RBUTTON, pos);
	::SetCursorPos(pt.x, pt.y);
	SetForegroundWindow(hFG);
	return TRUE;
}

DWORD EXPORT GetSpellName(WORD wSpell, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL || dwMaxChars == 0)
		return FALSE;

	::memset(lpszBuffer, 0, dwMaxChars);

	D2SPELLINFO dsi = {0};
	if (!D2GetSpellInfo(wSpell, &dsi))
		return 0;

	strncpy(lpszBuffer, dsi.szSpellName, dwMaxChars);
	return strlen(lpszBuffer);
}

int EXPORT GetGameDifficulty() // current game difficulty, normal/nm/hell
{
	return *p_D2CLIENT_nDiff + 1;
}

DWORD EXPORT GetD2WindowTitle(LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars);
	return ::GetWindowText(FindMyD2WndHandle(), lpszBuffer, dwMaxChars);
}

DWORD EXPORT GetInGameTime() // time elapsed since joining
{
	return g_dwGameTime;
}

POINT EXPORT GetMousePosition()
{
	POINT pt = { 0 };
	pt.x = D2CLIENT_GetMouseX();
	pt.y = D2CLIENT_GetMouseY();
	return pt;
}

LPCSTR EXPORT GetRealmName()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).RealmName : NULL;
}

LPCSTR EXPORT GetGameName()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).GameName : NULL;
}

LPCSTR EXPORT GetGamePassword()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).GamePass : NULL;
}

LPCSTR EXPORT GetGameIP()
{
	if (!thisgame || !thisgame->CurrentGame)
		return NULL;

	return thisgame->CurrentGame->ServerIP;
}

BOOL EXPORT GetItemCode(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL ||dwMaxChars == 0)
		return FALSE;

	::memset(lpszBuffer, 0, dwMaxChars);

	GAMEUNIT gu;
	gu.dwUnitID = dwItemID;
	gu.dwUnitType = UNIT_TYPE_ITEM;
	UnitAny* pUnit = (UnitAny*)VerifyUnit(&gu);
	if (pUnit == NULL)
	{
		//ItemError(dwItemID, "GetItemCode");
		return FALSE;
	}

	ItemTxt* ptxt = D2COMMON_GetItemTxt(pUnit->dwClassId);
	if (ptxt == NULL)
		return FALSE;

	::strncpy(lpszBuffer, ptxt->szCode, min(dwMaxChars, 3));
	return strlen(lpszBuffer);
}

DWORD EXPORT QueryModuleData(LPCSTR lpszModuleName, DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam)
{
	if (lpszModuleName == NULL || !lpszModuleName[0])
		return 0;

	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && !stricmp(pCIS->Name, lpszModuleName))
		{
			if (pCIS->OnQueryModuleData)
			{
				return pCIS->OnQueryModuleData(dwDataID, lpBuffer, dwMaxLength, lParam);
			}
			else
			{
				GameErrorf("\"OnQueryModuleData\" not exported by module \"%s\"!", lpszModuleName);
				return 0;
			}
		}

		li = ClientModules.GetNextItem(li);
	}

	GameErrorf("QueryModuleData Error: module \"%s\" not exists!", lpszModuleName);
	return 0; // module not found
}

DWORD EXPORT GetKeyName(BYTE iKCode, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	return D2GetKeyName(iKCode, lpszBuffer, dwMaxChars);
}

void OnMapChanged(BYTE iNewMapID)
{
	g_collisionMap.OnMapChanged(iNewMapID);
}

BOOL EXPORT IsQuestItem(LPCSTR lpszItemCode)
{
	return D2IsQuestItem(lpszItemCode);
}

BOOL IsGameJoined()
{
	return g_bGameJoined;
}

int EXPORT GetItemQuality(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL || pItem->ptItemData == NULL)
		return 0;

	return pItem->ptItemData->Quality1;
}

BOOL EXPORT GetItemPrefixSuffix(DWORD dwItemID, WORD* pPrefix, WORD* pSuffix)
{
	if (pPrefix)
		*pPrefix = 0;

	if (pSuffix)
		*pSuffix = 0;

	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL || pItem->ptItemData == NULL)
		return FALSE;

	if (pPrefix)
		*pPrefix = pItem->ptItemData->Prefix;

	if (pSuffix)
		*pSuffix = pItem->ptItemData->Suffix;
	return TRUE;
}

DWORD EXPORT GetPrefixSuffixName(WORD wPrefixOrSuffix, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	lpszBuffer[0] = '\0';
	char* p = D2COMMON_ItemGetMagicalMods(wPrefixOrSuffix);
	if (p)
		::strncpy(lpszBuffer, p, dwMaxChars);
	return strlen(lpszBuffer);
}

VOID GameSendPacketToServer2(LPBYTE buf, DWORD len, DWORD dwDelay/*=100*/)
{
	if (buf == NULL || len == 0)
		return;

	g_outgoingPackets.Lock();
	
	if (dwDelay >= 100)
		dwDelay -= 100; // minus the hard-coded 100 ms delay between every 2 thread ticks

	CPacket* pkt = new CPacket(buf, len, dwDelay);
	g_outgoingPackets.InsertRear(pkt);
	g_outgoingPackets.Unlock();
}

void CleanOutgoingPacketQueue()
{
	g_outgoingPackets.Lock();
	g_outgoingPackets.RemoveAll();
	g_outgoingPackets.Unlock();
}

SIZE EXPORT GetScreenSize()
{
	SIZE cz;
	cz.cx = min(800, *p_D2CLIENT_ClientScreenWidth);
	cz.cy = cz.cx == 800 ? 600 : 480;
	return cz;
}

void EXPORT ScreenToMapCoords(POINT& rScreenCoords)
{
	// I thought these are Mousepad's code, but actually Ninjai's
	rScreenCoords.x += D2CLIENT_GetMouseXOffset();
	rScreenCoords.y += D2CLIENT_GetMouseYOffset();
	D2COMMON_AbsScreenToMapCoords(&rScreenCoords.x, &rScreenCoords.y);
}

void EXPORT MapToScreenCoords(POINT& rMapPosition)
{
	D2COMMON_MapToAbsScreenCoords(&rMapPosition.x, &rMapPosition.y);
	rMapPosition.x -= D2CLIENT_GetMouseXOffset();
	rMapPosition.y -= D2CLIENT_GetMouseYOffset();
}

POINT EXPORT GetRandomPosition(int nRadiusMin, int nRadiusMax, int nAngleMin, int nAngleMax, const LPPOINT lpOrigin)
{	
	// if lpOrigin is null, we use player's current position
	POINT origin;
	if (lpOrigin == NULL)
		origin = GetPosition();
	else
		::memcpy(&origin, lpOrigin, sizeof(POINT));	
	return CalculateRandomPosition(origin, nRadiusMin, nRadiusMax, nAngleMin, nAngleMax);
}

int EXPORT GetAngle(long x1, long y1, long x2, long y2)
{
	return (int)CalculateAngle(x1, y1, x2, y2);
}

BOOL EXPORT CreateCollisionMap() // 1000 * 1000
{
	return g_collisionMap.CreateMap();
}

WORD EXPORT GetCollisionData(long x, long y, BOOL bAbsolute)
{
	return g_collisionMap.GetMapData(x, y, bAbsolute);
}

BOOL EXPORT DumpCollisionMap(LPCSTR lpszFilePath, const LPPOINT lpTokens, DWORD dwTokenCount)
{
	return g_collisionMap.DumpMap(lpszFilePath, lpTokens, dwTokenCount);
}

DWORD EXPORT CalculateTeleportPath(long x, long y, LPPOINT lpBuffer, DWORD dwMaxCount)
{
	if (!g_collisionMap.CreateMap())
		return 0;

	POINT ptStart = GetPosition();
	POINT ptEnd = { x, y };
	
	if (!g_collisionMap.IsValidAbsLocation(ptStart.x, ptStart.y)
		|| !g_collisionMap.IsValidAbsLocation(ptEnd.x, ptEnd.y))
		return 0;

	g_collisionMap.AbsToRelative(ptStart);
	g_collisionMap.AbsToRelative(ptEnd);

	WordMatrix matrix;
	if (!g_collisionMap.CopyMapData(matrix))
		return 0;

	g_collisionMap.MakeBlank(matrix, ptStart);
	g_collisionMap.MakeBlank(matrix, ptEnd);
	
	CTeleportPath tf(matrix.GetData(), matrix.GetCX(), matrix.GetCY());
	DWORD dwCount = tf.FindTeleportPath(ptStart, ptEnd, lpBuffer, dwMaxCount);

	for (DWORD i = 0; i < dwCount; i++)
		g_collisionMap.RelativeToAbs(lpBuffer[i]);

	return dwCount;
}

DWORD EXPORT CalculateWalkPath(long x, long y, LPPOINT lpBuffer, DWORD dwMaxCount)
{	
	if (!g_collisionMap.CreateMap())
		return 0;

	POINT ptStart = GetPosition();
	POINT ptEnd = { x, y };
	
	if (!g_collisionMap.IsValidAbsLocation(ptStart.x, ptStart.y)
		|| !g_collisionMap.IsValidAbsLocation(ptEnd.x, ptEnd.y))
		return 0;

	g_collisionMap.AbsToRelative(ptStart);
	g_collisionMap.AbsToRelative(ptEnd);
	
	WordMatrix matrix;
	if (!g_collisionMap.CopyMapData(matrix))
		return 0;

	g_collisionMap.ThickenWalls(matrix, 1);

	g_collisionMap.MakeBlank(matrix, ptStart);
	g_collisionMap.MakeBlank(matrix, ptEnd);

	CWalkPath wp(matrix.GetData(), matrix.GetCX(), matrix.GetCY());

	DWORD dwCount = (DWORD)wp.FindWalkPath(ptStart, ptEnd, lpBuffer, dwMaxCount);
	
	for (DWORD i = 0; i < dwCount; i++)
		g_collisionMap.RelativeToAbs(lpBuffer[i]);

	return dwCount;
}

DWORD EXPORT GetItemPrice(DWORD dwItemID, DWORD dwNpcClassID, BOOL bBuyOrSell)
{
	if (!IsVendor(dwNpcClassID) && !IsGambler(dwNpcClassID))
	{
		ServerErrorf("GetItemPrice: Npc class %08X is not a vendor.", dwNpcClassID);
		return 0; // invalid npc
	}

	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
	{
		ItemError(dwItemID, "GetItemPrice");
		return 0;
	}

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return 0;
	
	return (DWORD)D2COMMON_GetUnitPrice(pPlayer, pItem, *p_D2CLIENT_nDiff, *p_D2CLIENT_ItemPriceGameInfoUnk, dwNpcClassID, !bBuyOrSell);
}

DWORD EXPORT GetItemAttributes(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
	{
		ItemError(dwItemID, "GetItemAttributes");
		return 0;
	}

	return pItem->ptItemData->dwFlags;
}

BOOL EXPORT GetMapRect(LPRECT lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(RECT));
	UnitPlayer *pPlayer = D2CLIENT_GetPlayerUnit();
	if(!pPlayer)
		return FALSE;
	
	Room *pRoom1 = D2COMMON_GetRoomFromUnit(pPlayer);
	if(!pRoom1)
		return FALSE;	

	RoomOther *pRoom2 = pRoom1->ptRoomOther;
	if(!pRoom2) 
		return FALSE;

	DrlgLevel *pLevel = pRoom2->ptDrlgLevel;
	if(!pLevel)
		return FALSE;

	lpBuffer->left = pLevel->LevelX * 5;
	lpBuffer->top = pLevel->LevelY * 5;
	lpBuffer->right = lpBuffer->left + pLevel->LevelCX * 5;
	lpBuffer->bottom = lpBuffer->top + pLevel->LevelCY * 5;
	return lpBuffer->right > lpBuffer->left && lpBuffer->bottom > lpBuffer->top;
}

BOOL IsValidIndex(POINT pt, SIZE cz)
{
	return pt.x >= 0 && pt.x < cz.cx && pt.y >= 0 && pt.y < cz.cy;
}

//void EXPORT KeepD2Wnd()
//{
//	g_bKeepD2WndOn = !g_bKeepD2WndOn;
//	Patch_Unpatch(&g_KeepD2WndPatch1);
//	Patch_Unpatch(&g_KeepD2WndPatch2);
//	Patch_Unpatch(&g_KeepD2WndPatch3);
//}

BYTE EXPORT GetItemSockets(DWORD dwItemID)
{
	GAMEUNIT gu;
	gu.dwUnitID = dwItemID;
	gu.dwUnitType = UNIT_TYPE_ITEM;
	return (BYTE)GetUnitStat(&gu, 0xc2);
}

DWORD EXPORT GetItemProperties(DWORD dwItemID, LPITEMPROPERTY lpBuffer, DWORD dwMaxCount)
{
	if (dwItemID == 0 || lpBuffer == NULL || dwMaxCount == 0)
		return 0;

	UnitItem* p = (UnitItem*)VerifyItem(dwItemID);
	if (p == NULL)
	{
		ItemError(dwItemID, "GetItemProperties");
		return 0;
	}

	UnitStats *pStats = D2COMMON_GetUnitPtrStats3((UnitAny*)p, 0, 0x40);
	if (pStats == NULL)
		return 0;

	return (DWORD)D2COMMON_ListUnitStats(pStats, (ListStats*)lpBuffer, dwMaxCount);
}

BOOL EXPORT GetHighlightUnit(LPGAMEUNIT lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(GAMEUNIT));
	if (!(D2CLIENT_HasHighlight))
		return FALSE;

	lpBuffer->dwUnitID = D2CLIENT_HighlightUnitId;
	lpBuffer->dwUnitType = D2CLIENT_HighlightUnitType;
	return (BOOL)lpBuffer->dwUnitID;
}

DWORD EXPORT GetUnitMode(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
	{
		UnitError(lpUnit, "GetUnitMode");
		return 0;
	}

	return p->dwMode;
}

BYTE fChangingAct;
void __declspec(naked) ChangeActPatch1()
{
	__asm {
		pop eax;
		push esi;
		xor esi, esi;
		cmp fChangingAct, 0;
		mov fChangingAct, 0;
		jmp eax;
	}
}

void __declspec(naked) ChangeActPatch2()
{
	__asm {
		mov fChangingAct, 1;
		ret;
	}
}

BOOL WINAPI MySetCursorPos(int x, int y)
{
	return IsIconic(D2GFX_GetWindowHandle()) ? TRUE : SetCursorPos(x, y);
}

BOOL EXPORT EnumGroundItems(fnEnumItemProc lpfn, LPARAM lParam)
{
	if (lpfn == NULL)
		return FALSE;

	return InternalEnumUnits(UNIT_TYPE_ITEM, AppEnumGroundItemProc, (LPARAM)lpfn, lParam);
}

BOOL EXPORT EnumPlayers(fnEnumPlayerProc lpfn, LPARAM lParam)
{
	if (lpfn == NULL)
		return FALSE;

	for (Roster* p = *p_D2CLIENT_FirstRosterUnit; p; p = p->ptNext)
	{
		if (p->PlayerId == GetSafePlayerID())
			continue;
		
		DWORD dwPvpFlags = GetPvpFlags(p->PlayerId);
		BYTE iMapID = (dwPvpFlags & PVP_ALLIED) ? (BYTE)p->LevelId : 0;
		if (!lpfn(p->PlayerId, p->Name, p->ClassId, dwPvpFlags, iMapID, lParam))
			return FALSE;
	}

	return TRUE;
}

BYTE EXPORT GetItemLevel(DWORD dwItemID)
{
	UnitAny* p = (UnitAny*)VerifyItem(dwItemID);
	return p ? (BYTE)p->ptItemData->_14[0] : 0;	
}

DWORD GetItemColorHeader(DWORD dwItemID, LPSTR lpszBuffer)
{
	if (lpszBuffer == NULL)
		return 0;	

	lpszBuffer[0] = 0;
	
	char szCode[4] = "";
	if (!GetItemCode(dwItemID, szCode, 3))
		return 0;
	
	char szColor[4] = "";
	if (szCode[0] == 'r' && isdigit(szCode[1]) && isdigit(szCode[2]))
	{
		// runes
		::strcpy(szColor, "ÿc1");
	}
	else if (!::memcmp(szCode, "hp", 2) && isdigit(szCode[2]))
	{
		// healing potions
		::strcpy(szColor, "ÿc1");
	}
	else if (!::memcmp(szCode, "mp", 2) && isdigit(szCode[2]))
	{
		// mana potions
		::strcpy(szColor, "ÿc3");
	}
	else if (!::memcmp(szCode, "rv", 2) && (szCode[2] == 's' || szCode[2] == 'l'))
	{
		// rejuve potions
		::strcpy(szColor, "ÿc;");
	}
	else
	{
		switch (GetItemQuality(dwItemID))
		{
		case 4:
			::strcpy(szColor, "ÿc3");
			break;

		case 5:
			::strcpy(szColor, "ÿc2");
			break;

		case 6:
			::strcpy(szColor, "ÿc9");
			break;

		case 7:
			::strcpy(szColor, "ÿc4");
			break;

		case 8:
			::strcpy(szColor, "ÿc8");
			break;

		default:
			break;
		}
	}

	if (*szColor == 0)
		return 0;

	::strcpy(lpszBuffer, szColor);
	return 3;
}

DWORD EXPORT GetItemDescription(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor)
{
	if (lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars);
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return 0;

	wchar_t wsz[256] = { 0 };
	char sz1[256] = "";
	CStringEx str;

	D2CLIENT_GetItemNameString(pItem, wsz, 255);
	ConvertFromUnicode(wsz, sz1, 255);

	LPSTR pBreak = ::strchr(sz1, 0x0a);
	if (pBreak == NULL)
	{
		// it's a simple string
		str = sz1;
	}
	else
	{
		str = pBreak + 1;
		// remove "?cx"
		int n63 = str.Find(63);
		if (n63 != -1)
			str.Delete(n63, 3);

		*pBreak = 0;
		str += " ";
		str += sz1;
	}

	str.Trim();
	if (str.IsEmpty())
		return 0;

	if (GetItemColorHeader(dwItemID, sz1))
		str = sz1 + str;
	
	int nSockets = GetItemSockets(dwItemID);
	if (nSockets > 0)
	{
		char szSockets[32] = "";
		::sprintf(szSockets, " (%d)", nSockets);
		str += szSockets;
	}

	DWORD dwCopy = min(dwMaxChars, (DWORD)str.GetLength());
	::memcpy(lpszBuffer, (LPCSTR)str, dwCopy);
	lpszBuffer[dwCopy] = 0;
	return dwCopy;
}

DWORD EXPORT GetD2HackitVersion()
{
	return __SERVERVERSION__;
}

DWORD EXPORT GetD2HackitBuild()
{
	return __SERVERBUILD__;
}

DWORD ServerGetUnitStat(UnitAny* p, int nStatID, BOOL bBaseStatOnly)
{
	if (p == NULL)
		return 0;

	DWORD dwResult = 0;
	if (bBaseStatOnly)
		dwResult = D2COMMON_GetUnitCleanBaseStat(p, nStatID, 0);
	else
		dwResult = D2COMMON_GetUnitModifiedStat(p, nStatID, 0);

	if (nStatID == STAT_HP || nStatID == STAT_MAXHP
		|| nStatID == STAT_MANA || nStatID == STAT_MAXMANA
		|| nStatID == STAT_STAMINA || nStatID == STAT_MAXSTAMINA)
	{
		dwResult /= 256;
	}

	return dwResult;
}

DWORD EXPORT GetUnitBaseStat(LPCGAMEUNIT lpUnit, int nStatID)
{
	return ServerGetUnitStat((UnitAny*)VerifyUnit(lpUnit), nStatID, TRUE);
}

///////////////////////////////////////////////////////////////////////
// GetItemReqRecursive
//---------------------------------------------------------------------
// Searching for requirements could be time consuming if we start from 0
// and perform a set of linear comparison. If the item require 232 strength,
// e.g., Sacred Armor, we will have to call D2COMMON_TestItemMeetReq 232
// times, which would be ugly.
//
// This recursive function implements a binary searching algorithm to get the
// job done, hence reduces the complexity to O(log(n)).
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////
DWORD GetItemReqRecursive(int nMin, int nMax, UnitItem* pItem, UnitAny* pUnit, Stats* pStats, DWORD* p1, DWORD* p2, DWORD* p3, DWORD* pMatch)
{
	const int DIFF = nMax - nMin;

	if (DIFF < 4)
	{
		// do a linear search
		for (pStats->dwValue = (DWORD)nMin; (int)pStats->dwValue <= nMax; pStats->dwValue++)
		{
			D2COMMON_TestItemMeetReq(pItem, pUnit, 0, p1, p2, p3);
			if (*pMatch)
				return pStats->dwValue;
		}

		return nMax + 1;
	}

	pStats->dwValue = nMin;
	D2COMMON_TestItemMeetReq(pItem, pUnit, 0, p1, p2, p3);
	if (*pMatch)
		return nMin;

	pStats->dwValue = nMin + DIFF / 2;
	D2COMMON_TestItemMeetReq(pItem, pUnit, 0, p1, p2, p3);
	if (*pMatch)
		return GetItemReqRecursive(nMin, nMin + DIFF / 2, pItem, pUnit, pStats, p1, p2, p3, pMatch);
	else
		return GetItemReqRecursive(nMin + DIFF / 2 + 1, nMax, pItem, pUnit, pStats, p1, p2, p3, pMatch);
}

DWORD GetItemRequirement(DWORD dwItemID, int nStatID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return -1;

	DWORD dwStr, dwDex, dwLvl;
	DWORD* pMatch = NULL;
	if (nStatID == STAT_STRENGTH)
		pMatch = &dwStr;
	else if (nStatID == STAT_DEXTERITY)
		pMatch = &dwDex;
	else if (nStatID == STAT_LEVEL)
		pMatch = &dwLvl;

	if (pMatch == NULL)
		return -1;

	Stats st = { MAKELONG(0, nStatID), 0 };	// Create a fake stats
	StatsList sl = { 0 }; // Create a fake stats list
	sl.ptStats = &st; // associate the fake stats to the fake stats list
	sl.StatCount = 1;	
	UnitAny unit = { 0 }; // Create a fake unit
	unit.ptStatsList = &sl; // Associate the fake stats list to the fake unit
	
	return GetItemReqRecursive(0, 500, pItem, &unit, &st, &dwStr, &dwDex, &dwLvl, pMatch);
}

DWORD EXPORT GetItemLevelRequirement(DWORD dwItemID)
{
	return GetItemRequirement(dwItemID, STAT_LEVEL);
}

DWORD EXPORT GetItemStrRequirement(DWORD dwItemID)
{
	return GetItemRequirement(dwItemID, STAT_STRENGTH);
}

DWORD EXPORT GetItemDexRequirement(DWORD dwItemID)
{
	return GetItemRequirement(dwItemID, STAT_DEXTERITY);
}

BOOL EXPORT GetItemDamage(DWORD dwItemID, DWORD* pMinDamage, DWORD* pMaxDamage)
{
	if (pMinDamage)
		*pMinDamage = 0;

	if (pMaxDamage)
		*pMaxDamage = 0;

	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	if (!VerifyUnit(&gu))
		return FALSE;

	DWORD dw1, dw2;
	if (pMinDamage)
	{
		dw1 = GetUnitStat(&gu, 21);
		dw2 = GetUnitStat(&gu, 23);
		*pMinDamage = max(dw1, dw2);
		//GameInfof("min %d", *pMinDamage);
	}

	if (pMaxDamage)
	{
		dw1 = GetUnitStat(&gu, 22);
		dw2 = GetUnitStat(&gu, 24);
		*pMaxDamage = max(dw1, dw2);
		//GameInfof("max %d", *pMaxDamage);
	}

	return TRUE;	
}

BOOL EXPORT IsTwoHandItem(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return -1;

	return D2COMMON_TwoHandedItemCheck(pItem);
}

SIZE EXPORT GetItemSize2(DWORD dwItemID)
{
	SIZE sz = { 0 };
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return sz;

	ItemBinData* p = D2COMMON_ItemGetBinFromClass(pItem->dwClassId);
	if (p == NULL)
		return sz;

	sz.cx = p->SizeX;
	sz.cy = p->SizeY;
	return sz;
}

int EXPORT GetItemType(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return -1;

	ItemBinData* p = D2COMMON_ItemGetBinFromClass(pItem->dwClassId);
	if (p == NULL)
		return -1;

	return p->Type;
}

BOOL EXPORT IsObjectDoor(LPCGAMEUNIT lpUnit)
{
	if (lpUnit == NULL || lpUnit->dwUnitType != UNIT_TYPE_OBJECT)
		return 0;

	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return 0;

	if (!D2COMMON_IsUnitObjectDoor((UnitObject*)p))
		return 0;

	return p->dwClassId;
}

BOOL EXPORT IsObjectChest(LPCGAMEUNIT lpUnit)
{
	if (lpUnit == NULL || lpUnit->dwUnitType != UNIT_TYPE_OBJECT)
		return 0;

	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return 0;

	ObjectTxt *obj = D2COMMON_GetObjectTxt(p->dwClassId);
	if (obj == NULL || !obj->nSelectable0)
		return 0;

	int nResult = obj->nOperateFn;
	switch (nResult)
	{
	case 1: //bed, undef grave, casket, sarc
	case 3: //basket, urn, rockpile, trapped soul
	case 4: //chest, corpse, wooden chest, buriel chest, skull and rocks, dead barb
	case 5: //barrel
	case 7: //exploding barrel
	case 14: //loose bolder etc....*
	case 19: //armor stand
	case 20: //weapon rack
	case 33: //writ
	case 48: //trapped soul
	case 51: //stash
	case 68: //evil urn
		break;

	default:
		nResult = 0;
		break;
	}

	return nResult;
}

BOOL EXPORT IsObjectShrine(LPCGAMEUNIT lpUnit)
{
	if (lpUnit == NULL || lpUnit->dwUnitType != UNIT_TYPE_OBJECT)
		return 0;

	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return 0;

	ObjectTxt *obj = D2COMMON_GetObjectTxt(p->dwClassId);
	if (obj == NULL || !obj->nSelectable0)
		return 0;

	return obj->nAutoMap == 310 ? p->dwClassId : 0;
}

void DebugTest(char** argv, int argc)
{
	/*
	DWORD dw;
	::GetWindowThreadProcessId(g_hD2, &dw);
	HANDLE h = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw);
	GameInfof("%08X - %08X = %08X", 
		(DWORD)D2COMMON_GetFirstInventoryItem,
		(DWORD)h,
		(DWORD)D2COMMON_GetFirstInventoryItem - (DWORD)h);
	::CloseHandle(h);
	*/
}
