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
#include "PathAlgorithms\\Common.h"
#include "PathAlgorithms\\TeleportPath.h"
#include "PathAlgorithms\\CollisionMap.h"
#include "PathAlgorithms\\TeleportPath.h"
#include "PathAlgorithms\\WalkPath.h"

typedef struct tagPatchData
{
	LPVOID lpDest; // Destination
	int nCodeLen; // Code length, must be 1-4
	DWORD dwCode; // Patch code
} PATCHDATA, *LPPATCHDATA;
typedef const tagPatchData* LPCPATCHDATA;

BOOL g_bKeepD2WndOn = FALSE;
DWORD g_dwGameTime = 0; // total time elapsed since game joining
PacketQueue g_outgoingPackets; // Outgoing packets
DWORD g_dwLastOutgoing = 0; // Last outgoing packet sent time
BOOL g_bLoadedBeforeJoin = FALSE; // d2Hackit loaded before joining game?
CCollisionMap g_collisionMap;
CTickThread g_clientTickThread;
CTickThread g_serverTickThread;
CTickThread	g_messageTickThread;
CTickThread	g_jobTickThread;

// Our patchs
PATCHDATA g_KeepD2WndPatch1 = { 0, 1, SW_SHOWNOACTIVATE };
PATCHDATA g_KeepD2WndPatch2 = { 0, 1, SW_SHOWNOACTIVATE };
PATCHDATA g_KeepD2WndPatch3 = { 0, 1, SW_SHOWNOACTIVATE };
PATCHDATA g_GameOnDrawPatch = { 0, 4, 0 };

BOOL IsValidIndex(POINT pt, SIZE cz);
void CALLBACK ServerTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag); // Server20.cpp
void CALLBACK ClientTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag); // Server20.cpp
void CALLBACK MessageTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag); // GameMessage.cpp
void CALLBACK JobTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag); // MotionJob.cpp

BOOL Patch_Unpatch(LPPATCHDATA lpPatch);
BOOL Patch_Unpatch(LPPATCHDATA lpPatch)
{
	if (lpPatch == NULL)
		return FALSE;
	
	return PatchFill(lpPatch->lpDest, &lpPatch->dwCode, &lpPatch->dwCode, lpPatch->nCodeLen);
}

BOOL Server20Start()
{
	// v2.00

	// Create threads, v2.00
	g_serverTickThread.Start(ServerTimerTick, (LPVOID)&ClientModules);
	g_clientTickThread.Start(ClientTimerTick, (LPVOID)&ClientModules);
	g_messageTickThread.Start(MessageTimerTick, (LPVOID)&ClientModules);
	g_jobTickThread.Start(JobTimerTick, (LPVOID)&ClientModules);

	// Install KeepD2Wnd patchs
	g_KeepD2WndPatch1.lpDest = (LPVOID)D2GFX_KeepD2WindowPatch1;
	g_KeepD2WndPatch2.lpDest = (LPVOID)D2GFX_KeepD2WindowPatch2;
	g_KeepD2WndPatch3.lpDest = (LPVOID)D2GFX_KeepD2WindowPatch3;

	// Install GameOnDraw patch
	g_GameOnDrawPatch.lpDest = (LPVOID)(D2CLIENT_ScreenPrint + 1);
	g_GameOnDrawPatch.dwCode = (DWORD)&GameOnDrawIntercept - (D2CLIENT_ScreenPrint + 1 + 4); // offset from d2bs
	Patch_Unpatch(&g_GameOnDrawPatch);

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
	g_messageTickThread.Stop(2500);
	g_jobTickThread.Stop(2500);

	// Remove GameOnDraw Patch
	Patch_Unpatch(&g_GameOnDrawPatch);

	// Remove KeepD2Wnd Patchs
	if (g_bKeepD2WndOn)
	{
		Patch_Unpatch(&g_KeepD2WndPatch1);
		Patch_Unpatch(&g_KeepD2WndPatch2);
		Patch_Unpatch(&g_KeepD2WndPatch3);
	}
	
	return TRUE;
}

VOID Server20GameJoin(THISGAMESTRUCT* thisgame)
{
	g_dwGameTime = 0;	
	g_dwLastOutgoing = 0;
	Player_OnGameJoin(thisgame);
	ServerDrawOnGameJoin();
}

VOID Server20GameLeave()
{		
	g_collisionMap.DestroyMap();
	g_dwGameTime = 0;
	g_bLoadedBeforeJoin = FALSE;
	Player_OnGameLeave();

	g_outgoingPackets.Lock(500);
	g_outgoingPackets.RemoveAll();
	g_outgoingPackets.Unlock();
	g_dwLastOutgoing = 0;
	CleanMessageQueue();
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
	if (!g_outgoingPackets.Lock(500))
		return FALSE;

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

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return FALSE; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (dwUnitType == pUnit->dwType && dwUnitClassID == pUnit->dwClassId)
			{
				lpBuffer->dwUnitID = pUnit->dwId;
				lpBuffer->dwUnitType = pUnit->dwType;
				return TRUE;
			}			
		}
	}

	return FALSE;
}

BOOL EXPORT FindUnitByName(LPCSTR lpszUnitName, DWORD dwUnitType, LPGAMEUNIT lpBuffer)
{
	if (lpszUnitName == NULL || !lpszUnitName[0] || lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(GAMEUNIT));

	// convert to unicode first
	wchar_t wszName[256] = {0};
	ConvertToUnicode(lpszUnitName, wszName, 255);

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return FALSE; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (dwUnitType == pUnit->dwType)
			{
				wchar_t* pwsz = D2CLIENT_GetNameFromUnit(pUnit);
				if (pwsz && !wcsicmp(wszName, pwsz))
				{
					lpBuffer->dwUnitID = pUnit->dwId;
					lpBuffer->dwUnitType = pUnit->dwType;
					return TRUE;
				}				
			}			
		}
	}

	return FALSE;
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
		return mp;

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
		return -1;

	if (lpUnit->dwUnitType == UNIT_TYPE_MONSTER)
		return D2CLIENT_GetMonsterOwner(lpUnit->dwUnitID);

	if (lpUnit->dwUnitType == UNIT_TYPE_OBJECT)
	{
		if (!::stricmp(GetSafePlayerName(), p->ptObjectData->parent))
			return GetSafePlayerID();
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
		return 0;

	return p->dwClassId;
}

DWORD EXPORT GetUnitStat(LPCGAMEUNIT lpUnit, int nStatID)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
		return 0;

	return D2COMMON_GetBaseStat(p, nStatID, 0);
}

BYTE EXPORT GetUnitHPPercent(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
		return 0;

	return CalcPercent(D2COMMON_GetBaseStat(p, STAT_HP, 0), D2COMMON_GetBaseStat(p, STAT_MAXHP, 0), 1);
}

BOOL EXPORT GetUnitAffection(LPCGAMEUNIT lpUnit, int nAffectionID)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
		return 0;

	return D2COMMON_GetUnitState(p, nAffectionID);
}

int EXPORT GetMonsterType(LPCGAMEUNIT lpUnit) // normal/champion/boss/minion
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL || p->ptMonsterData == NULL)
		return MONSTER_UNKNOWN;	

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
		return 0; // not a monster

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
	if (D2COMMON_GetBaseStat(p, 36, 0) >= 100)
		dwResult |= MA_IMMUNE_PHYSICAL;

	if (D2COMMON_GetBaseStat(p, 37, 0) >= 100)
		dwResult |= MA_IMMUNE_MAGIC;

	if (D2COMMON_GetBaseStat(p, 39, 0) >= 100)
		dwResult |= MA_IMMUNE_FIRE;

	if (D2COMMON_GetBaseStat(p, 41, 0) >= 100)
		dwResult |= MA_IMMUNE_LIGHTNING;

	if (D2COMMON_GetBaseStat(p, 43, 0) >= 100)
		dwResult |= MA_IMMUNE_COLD;

	if (D2COMMON_GetBaseStat(p, 45, 0) >= 100)
		dwResult |= MA_IMMUNE_POISON;

	return dwResult;
}

void TrimString(LPSTR lpszString)
{
	if (lpszString == NULL)
		return;

	int nLen = strlen(lpszString);

	// '\n' and '\r' are actually not possible in this case, but anyway...
	
	// Trim right side
	while (nLen > 0
		&& (lpszString[nLen - 1] == ' '
			|| lpszString[nLen - 1] == '\t'
			|| lpszString[nLen - 1] == '\r'
			|| lpszString[nLen - 1] == '\n'))
	{
		lpszString[--nLen] = 0;
	}

	// Trim left side
	LPCTSTR p = lpszString; 
	while (*p ==' '
			|| *p == '\t'
			|| *p == '\r'
			|| *p == '\n')
	{
		p = &p[1];
	}

	if (p != lpszString)
	{
		LPTSTR psz = strdup(p);
		strcpy(lpszString, psz);
		delete [] psz;
	}
}

DWORD EXPORT GetDistance(long x1, long y1, long x2, long y2) // distance between two positions, in yards
{
	return (DWORD)CalculateDistance(x1, y1, x2, y2);
}

BOOL EXPORT GetMyMercUnit(LPGAMEUNIT lpBuffer) // Get player's mercenary
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(GAMEUNIT));

	const DWORD PID = GetSafePlayerID();
	if (PID == 0)
		return FALSE;

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return FALSE; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (pUnit->dwType == UNIT_TYPE_MONSTER // type check
				&& IsMercClassID(pUnit->dwClassId) // class id check
				&& D2CLIENT_GetMonsterOwner(pUnit->dwId) == PID) // owner check
			{
				lpBuffer->dwUnitID = pUnit->dwId;
				lpBuffer->dwUnitType = pUnit->dwType;
				return TRUE;
			}			
		}
	}

	return FALSE;
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

BOOL EXPORT EnumUnits(DWORD dwUnitType, fnEnumUnitProc lpfnEnumUnitProc, LPARAM lParam)
{
	if (lpfnEnumUnitProc == NULL)
		return FALSE;

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return FALSE; // not in a game?

	GAMEUNIT gu;
	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (dwUnitType == pUnit->dwType)
			{
				gu.dwUnitID = pUnit->dwId;
				gu.dwUnitType = pUnit->dwType;

				if (!lpfnEnumUnitProc(&gu, lParam))
					return FALSE;
			}			
		}
	}

	return TRUE;
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
	return D2GetItemSize(lpszItemCode);
}

int EXPORT GetMyCorpseCount()
{
	int nCount = 0;

	const DWORD PID = GetSafePlayerID();
	if (PID == 0)
		return 0;

	// convert to unicode first
	wchar_t wszName[256] = {0};
	ConvertToUnicode(GetSafePlayerName(), wszName, 255);

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return 0; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (pUnit->dwType == UNIT_TYPE_PLAYER && pUnit->dwId != PID)
			{
				wchar_t* pwsz = D2CLIENT_GetNameFromUnit(pUnit);
				if (pwsz && !wcsicmp(wszName, pwsz))
				{
					nCount++;
				}				
			}			
		}
	}

	return nCount;
}

BOOL EXPORT PickMyCorpse()
{
	// just pick the first corpse
	const DWORD PID = GetSafePlayerID();
	if (PID == 0)
		return 0;

	// convert to unicode first
	wchar_t wszName[256] = {0};
	ConvertToUnicode(GetSafePlayerName(), wszName, 255);

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return 0; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (pUnit->dwType == UNIT_TYPE_PLAYER && pUnit->dwId != PID)
			{
				wchar_t* pwsz = D2CLIENT_GetNameFromUnit(pUnit);
				if (pwsz && !wcsicmp(wszName, pwsz))
				{
					// its my corpse
					GAMEUNIT gu;
					gu.dwUnitID = pUnit->dwId;
					gu.dwUnitType = UNIT_TYPE_PLAYER;
					POINT mp = GetUnitPosition(&gu);
					if (GetDistanceFrom(mp.x, mp.y) > 5)
						GoToEntity(&gu, mp.x, mp.y, TRUE);

					return Interact(&gu);
				}				
			}			
		}
	}

	return FALSE;
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
		return FALSE;

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

	if (!g_outgoingPackets.Lock(5000))
		return;
	
	if (dwDelay >= 100)
		dwDelay -= 100; // minus the hard-coded 100 ms delay between every 2 thread ticks

	CPacket* pkt = new CPacket(buf, len, dwDelay);
	g_outgoingPackets.InsertRear(pkt);
	g_outgoingPackets.Unlock();
}

void CleanOutgoingPacketQueue()
{
	g_outgoingPackets.Lock(5000);
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

	g_collisionMap.MakeBlank(matrix, ptStart);
	g_collisionMap.MakeBlank(matrix, ptEnd);

	CWalkPath wp(matrix.GetData(), matrix.GetCX(), matrix.GetCY());
	DWORD dwCount = wp.FindWalkPath(ptStart, ptEnd, lpBuffer, dwMaxCount);
	
	for (DWORD i = 0; i < dwCount; i++)
		g_collisionMap.RelativeToAbs(lpBuffer[i]);

	return dwCount;
}

DWORD EXPORT GetItemPrice(DWORD dwItemID, DWORD dwNpcClassID, BOOL bBuyOrSell)
{
	if (!IsVendor(dwNpcClassID) && !IsGambler(dwNpcClassID))
		return 0; // invalid npc

	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return 0;

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return 0;
	
	return (DWORD)D2COMMON_GetUnitPrice(pPlayer, pItem, *p_D2CLIENT_nDiff, *p_D2CLIENT_ItemPriceGameInfoUnk, dwNpcClassID, !bBuyOrSell);
}

DWORD EXPORT GetItemAttributes(DWORD dwItemID)
{
	UnitItem* pItem = (UnitItem*)VerifyItem(dwItemID);
	if (pItem == NULL)
		return 0;

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

void EXPORT KeepD2Wnd()
{
	g_bKeepD2WndOn = !g_bKeepD2WndOn;
	Patch_Unpatch(&g_KeepD2WndPatch1);
	Patch_Unpatch(&g_KeepD2WndPatch2);
	Patch_Unpatch(&g_KeepD2WndPatch3);
}

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
		return 0;

	UnitStats *pStats = D2COMMON_GetUnitPtrStats3((UnitAny*)p);
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
		return 0;

	return p->dwMode;
}

void DebugTest(char** argv, int argc)
{	
	//POINT pt = FindPresetUnit(10, UNIT_TYPE_ROOMTILE);
	//POINT aPath[100];
	//DWORD dwCount = CalculateTeleportPath(pt.x, pt.y, aPath, 100);
	//g_collisionMap.CreateMap();
	//g_collisionMap.DumpMap("c:\\dump.txt", aPath, dwCount);
	
	//GAMEUNIT gu = {0};
	//FindUnitByName("waypoint", UNIT_TYPE_OBJECT, &gu);
	//EnterWaypoint(&gu, MAP_A4_RIVER_OF_FLAME);
	//POINT pt = GetUnitPosition(&gu);

	//GameInfof("wp %d %d", pt.x, pt.y);

	//POINT aPath[255];
	//DWORD dwCount = CalculateWalkPath(pt.x, pt.y, aPath, 255);
	//GameInfof("%d nodes", dwCount);

	//DumpCollisionMap("c:\\dump.txt", aPath, dwCount);

	//for (DWORD i = 0; i < dwCount; i++)
	//	MoveTo(aPath[i].x, aPath[i].y, 1);
		
}