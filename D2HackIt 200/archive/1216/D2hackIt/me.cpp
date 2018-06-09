//////////////////////////////////////////////////////////////////
// Me.cpp
//
// Implementation of the "me" API set functions
//
// Abin(abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////
#include "me.h"
#include "D2HackItServer.h"
#include "utilities\\ArrayEx.h"
#include "d2functions.h"
#include "definitions.h"
#include "D2HackIt.h"
#include "MapDef.h"
#include "Inventory.h"
#include "SpellDef.h"
#include "MotionJob.h"
#include "Server20.h"
#include "GameMessage.h"
#include "NpcSession.h"
#include "ItemCheck.h"
#include "EnterWayPoint.h"
#include "BackToTown.h"
#include "D2Stdio.h"

static const BYTE CLOSABLEUIS[] = { UI_INVENTORY, UI_CHARACTER, UI_MINISKILL, UI_SKILL, UI_CHAT, UI_NPCMENU, UI_MAINMENU, UI_CONFIG, UI_NPCSHOP, UI_ANVIL, UI_QUEST, UI_STATUSAREA, UI_WPMENU, UI_PARTY, UI_TRADE, UI_MSGS, UI_STASH, UI_CUBE, UI_HELP, UI_MERC, UI_SCROLL, 0x00 }; 
static const LPCSTR RSL[] = { "hp", "rvs", "rvl", NULL }; // red-rejuv-full rejuv
static const LPCSTR SRL[] = { "rvs", "hp", "rvl", NULL }; // rejuv-red-full rejuv
static const LPCSTR LSR[] = { "rvl", "rvs", "hp", NULL }; // full rejuv-rejuv-red 
static const LPCSTR SL[] = { "rvs", "rvl", NULL }; // rejuv-full rejuv 

DWORD g_dwPlayerID = 0; // Player ID(before OnGameJoin is called)
char g_szPlayerName[32] = ""; // Player name(before OnGameJoin is called)
BYTE g_iCurMapID = 0; // current map
BOOL g_bGameReady = FALSE; // Game ready flag
D2SPELLINFO g_dsi = {0}; // Most recently utilized spell
BOOL g_bClosingUI = FALSE;

void CheckPlayerMap();
BOOL CALLBACK FindCorpseProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2);
BOOL CALLBACK FindHoistilerProc(DWORD dwPlayerID, LPCSTR lpszPlayerName, DWORD dwPlayerClass, DWORD dwPvpFlags, BYTE iMapID, LPARAM lParam);

VOID Player_OnGameJoin(THISGAMESTRUCT* thisgame)
{	
	g_bGameReady = FALSE;	
	g_dwPlayerID = thisgame->player->PlayerID;
	::strcpy(g_szPlayerName, thisgame->player->PlayerName);
	g_iCurMapID = 0;
	::memset(&g_dsi, 0, sizeof(D2SPELLINFO));
	CleanJobs();
	g_bClosingUI = FALSE;	
	CleanEnterWayPointStats();
}

VOID Player_OnGameLeave()
{
	g_dwPlayerID = 0;
	::memset(g_szPlayerName, 0, sizeof(g_szPlayerName));
	CleanBackToTownStates();	
	CleanNpcSessionStates();
	CleanEnterWayPointStats();
}

void CALLBACK PlayerTimerTick(LPVOID lpParam, LPCVBYTE lpStopFlag)
{
	if (!g_bGameReady)
	{
		BOOL bReady = IsGameReady();
		if (bReady)
		{
			g_bGameReady = TRUE;			
			SendThisPlayerMessage(PM_GAMEREADY);
		}
	}	
	else if (g_bClosingUI)
	{
		// try to close UI every 100 ms
		if (GetOpenedUI()) // any UI opened?
		{
			// simply send "esc" key until no more UI's appear!
			// I really don't understand why others would make such thing so complicated.
			// Note that unlike other keys, the "esc" key is not configurable so it will
			// never get overwritten, this key is absolutely safe.
			SendKey(VK_ESCAPE);
		}
		else
		{
			// No more UI, stop the timing and send PM_UICLOSED
			g_bClosingUI = FALSE;
			SendThisPlayerMessage(PM_UICLOSED);
		}
	}

	// Map check
	if (!NEED_ABORT(lpStopFlag))
		CheckPlayerMap();
	
	////////////////////////////////////////////////////////////////
	// Steps below here require game ready
	////////////////////////////////////////////////////////////////
	if (!g_bGameReady)
		return;

	if (!NEED_ABORT(lpStopFlag))
		ProcessBackToTown(lpStopFlag);

	if (!NEED_ABORT(lpStopFlag))
		ProcessEnterWayPoint(lpStopFlag);

	if (!NEED_ABORT(lpStopFlag))
		ProcessNpcSession(lpStopFlag);
}

void SetPlayerInfoBeforeJoin(DWORD dwPlayerID, LPCSTR lpszPlayerName)
{
	if (!g_bGameJoined && !g_dwPlayerID && dwPlayerID && lpszPlayerName)
	{
		g_dwPlayerID = dwPlayerID;
		::memset(g_szPlayerName, 0, 32);
		::strncpy(g_szPlayerName, lpszPlayerName, 16);
	}
}

BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin)
{
	if (dwVal == 0 || dwMaxVal == 0)
		return 0;

	BYTE iRes = (BYTE)((double)dwVal / (double)dwMaxVal * 100.0);
	if (iRes > 100)
		iRes = 100;

	return max(iRes, iMin);
}

void MakePlayerData(LPGAMEUNIT lpBuffer)
{
	lpBuffer->dwUnitID = GetSafePlayerID();
	lpBuffer->dwUnitType = UNIT_TYPE_PLAYER;
}

DWORD GetSafePlayerID()
{
	return thisgame->player ? thisgame->player->PlayerID : g_dwPlayerID;
}

LPCSTR GetSafePlayerName()
{
	return thisgame->player ? thisgame->player->PlayerName : g_szPlayerName;
}

BOOL EXPORT IsGameReady()
{
	return g_bGameJoined
		&& thisgame
		&& thisgame->player
		&& thisgame->player->PlayerPositionX
		&& thisgame->player->PlayerPositionY;
}

BOOL EXPORT HaveSpell(WORD wSpellID)
{
	// First check chargeable spells
	if (GetSpellCharges(wSpellID) > 0)
		return TRUE;

	// Check normal spells
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	for (Skill* pSkill = D2COMMON_GetStartSkill(pPlayer); pSkill && pSkill->pSkillInfo; pSkill = pSkill->pNextSkill)
	{
		if (wSpellID == pSkill->pSkillInfo->Id)
			return TRUE;
	}

	return FALSE;
}

BYTE EXPORT GetCurrentAct()
{
	return D2GetActFromMap(GetCurrentMapID());
}

DWORD EXPORT GetID() // player ID
{
	return GetSafePlayerID();
}

LPCSTR EXPORT GetName() // Player name
{
	return GetSafePlayerName();
}

BOOL EXPORT LeaveGame()
{
	return GameSaveAndExit();
}

BOOL EXPORT IsInTown()
{
	return IsGameReady() ? D2MapIsTown(GetCurrentMapID()) : TRUE;
}

BOOL IsThisPlayerID(const BYTE* ptr)
{
	DWORD dwPlayerID = GetSafePlayerID();
	if (ptr == NULL || dwPlayerID == 0)
		return FALSE;

	return ::memcmp(&dwPlayerID, ptr, 4) == 0;
}

DWORD EXPORT GetMode()	// Get player's current mode(walking/running/attacking etc)
{
	UnitAny* p = D2CLIENT_GetPlayerUnit();
	if (p == NULL)
		return 0;
	return p->dwMode;
}

WORD EXPORT GetSelectedSpell(BOOL bLeft) // Get the selected spell
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return D2S_INVALID;

	Skill* pSkill = bLeft ? D2COMMON_GetLeftSkill(pPlayer) : D2COMMON_GetRightSkill(pPlayer);
	if (pSkill == NULL || pSkill->pSkillInfo == NULL)
		return D2S_INVALID;

	return pSkill->pSkillInfo->Id;
}

BYTE EXPORT GetHPPercent()	// Player HP percent 0-100
{
	return CalcPercent(GetStat(STAT_HP), GetStat(STAT_MAXHP));
}

BYTE EXPORT GetManaPercent()	// Player mana percent 0-100
{
	return CalcPercent(GetStat(STAT_MANA), GetStat(STAT_MAXMANA));
}

BYTE EXPORT GetItemDurabilityPercent()	// Player item durability percent 0-100
{
	return CalcPercent(GetStat(STAT_DURABILITY), GetStat(STAT_MAXDURABILITY));
}

DWORD EXPORT GetStat(int nStatID)	// General stat access, hp/maxhp/mana/maxmana/mf/ias etc
{
	return ServerGetUnitStat((UnitAny*)D2CLIENT_GetPlayerUnit(), nStatID, FALSE);
}

BOOL EXPORT GetAffection(int nAffectionID)
{
	GAMEUNIT gu;
	MakePlayerData(&gu);
	return GetUnitAffection(&gu, nAffectionID);
}

BOOL EXPORT SelectSpell(WORD wSpell, BOOL bLeftHand)
{
	if (GetSelectedSpell(bLeftHand) == wSpell)
		return TRUE;

	if (!HaveSpell(wSpell) || !D2GetSpellInfo(wSpell, &g_dsi))
		return FALSE;

	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x3c;
	memcpy(aPacket + 1, &wSpell, 2);
	aPacket[4] = bLeftHand ? 0x80 : 0x00;
	aPacket[5] = aPacket[6] =aPacket[7] = aPacket[8] = 0xff;

	return GameSendPacketToServer(aPacket, 9);
}

POINT EXPORT GetPosition() // player position
{
	POINT pt = { 0 };
	UnitAny* p = (UnitAny*)D2CLIENT_GetPlayerUnit();
	if (p && p->hPath)
	{
		pt.x = D2COMMON_GetUnitX(p->hPath);
		pt.y = D2COMMON_GetUnitY(p->hPath);	
	}

	return pt;
}

DWORD EXPORT GetDistanceFrom(long x, long y) // Player's distance from a position
{
	POINT mp = GetPosition();
	return GetDistance(mp.x, mp.y, x, y);
}

BOOL EXPORT Interact(LPCGAMEUNIT lpUnit)
{
	if (!VerifyUnit(lpUnit))
	{
		UnitError(lpUnit, "Interact");
		return FALSE;
	}

	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x13;
	::memcpy(aPacket + 1, &lpUnit->dwUnitType, 4);
	::memcpy(aPacket + 5, &lpUnit->dwUnitID, 4);

	if (!GameSendPacketToServer(aPacket, 9))
		return FALSE;

	if (lpUnit->dwUnitType == 0x01)
	{
		aPacket[0] = 0x2f;
		if (!GameSendPacketToServer(aPacket, 9))
			return FALSE;
	}

	return TRUE;
}

BOOL GoToEntity(LPCGAMEUNIT lpUnit, WORD x, WORD y, BOOL bShowClientMove)
{
	BYTE aPacket[16] = { 0 };
	aPacket[0] = GetAlwaysRun() ? 0x04 : 0x02;
	::memcpy(aPacket + 1, &lpUnit->dwUnitType, 4);
	::memcpy(aPacket + 5, &lpUnit->dwUnitID, 4);

	if (!GameSendPacketToServer(aPacket, 9))
		return FALSE;

	if (bShowClientMove && IsGameReady())
	{
		DWORD dwPlayerID = GetSafePlayerID();
		::memset(aPacket, 0, sizeof(aPacket));
		aPacket[0] = 0x10;
		aPacket[1] = 0x00;
		::memcpy(aPacket + 2, &dwPlayerID, 4);
		aPacket[6] = GetAlwaysRun() ? 0x18 : 0x00;
		aPacket[7] = (BYTE)lpUnit->dwUnitType;
		::memcpy(aPacket + 8, &lpUnit->dwUnitID, 4);
		::memcpy(aPacket + 12, &x, 2);
		::memcpy(aPacket + 14, &y, 2);
		GameSendPacketToGame(aPacket, 16);
	}

	return TRUE;		
}

int EXPORT GetHostilerCount() // number of players that are hostiled to me
{	
	int nCount = 0;
	EnumPlayers(FindHoistilerProc, (LPARAM)&nCount);
	return nCount;
}

BYTE EXPORT GetCurrentMapID()
{
	UnitAny* pPlayer = D2CLIENT_GetPlayerUnit();
	if (!pPlayer)
		return MAP_UNKNOWN;
	
	Room* pRoom = D2COMMON_GetRoomFromUnit(pPlayer);
	if (!pRoom)
		return MAP_UNKNOWN;
	
	return D2COMMON_GetLevelIdFromRoom(pRoom);
}

BOOL GoToPosition(WORD x, WORD y, BOOL bShowClientMove)
{
	if (GetDistanceFrom(x, y) > 99)
	{
		PositionError(x, y, "GoToPosition");
		return FALSE;
	}

	BYTE aPacket[16] = { 0 };
	aPacket[0] = GetAlwaysRun() ? 0x03 : 0x01;
	::memcpy(&aPacket[1], &x, 2);
	::memcpy(&aPacket[3], &y, 2);

	if (!GameSendPacketToServer(aPacket, 5))
		return FALSE;

	if (bShowClientMove && IsGameReady())
	{
		DWORD dwPlayerID = GetSafePlayerID();
		POINT mp = GetPosition();

		::memset(aPacket, 0, sizeof(aPacket));
		aPacket[0] = 0x0f;
		::memcpy(aPacket + 2, &dwPlayerID, 4);
		aPacket[6] = GetAlwaysRun() ? 0x17 : 0x01;
		::memcpy(aPacket + 7, &x, 2); // New position x
		::memcpy(aPacket + 9, &y, 2); // New position y
		::memcpy(aPacket + 12, &mp.x, 2); // Old position X
		::memcpy(aPacket + 14, &mp.y, 2); // Old position Y	
		GameSendPacketToGame(aPacket, 16);
	}
	
	return TRUE;
}

BOOL EXPORT CastNoTarget(WORD wSpell, BOOL bLeftHand)
{
	POINT mp = GetPosition();
	return CastOnMap(wSpell, mp.x, mp.y, bLeftHand);
}

BOOL EXPORT CastOnMap(WORD wSpell, long x, long y, BOOL bLeftHand)
{
	if (GetDistanceFrom(x, y) > 99)
	{
		PositionError(x, y, "CastOnMap");
		return FALSE;
	}
	
	if (!SelectSpell(wSpell, bLeftHand))
	{
		SpellError(wSpell);
		return FALSE;
	}

	WORD wx = (WORD)x;
	WORD wy = (WORD)y;
	
	BYTE aPacket[17] = { 0 };
	aPacket[0] = bLeftHand ? 0x05 : 0x0c;
	memcpy(aPacket + 1, &wx, 2);
	memcpy(aPacket + 3, &wy, 2);	

	if (!GameSendPacketToServer(aPacket, 5))
		return FALSE;

	if (IsGameReady())
	{
		DWORD dwPlayerID = GetSafePlayerID();
		::memset(aPacket, 0, sizeof(aPacket));
		aPacket[0] = 0x4d;
		::memcpy(aPacket + 2, &dwPlayerID, 4);
		::memcpy(aPacket + 6, &wSpell, 2);
		aPacket[10] = 0x01;
		::memcpy(aPacket + 11, &wx, 2);
		::memcpy(aPacket + 13, &wy, 2);
		GameSendPacketToGame(aPacket, 17);
	}

	return TRUE;	
}

BOOL EXPORT CastOnUnit(WORD wSpell, LPCGAMEUNIT lpUnit, BOOL bLeftHand)
{
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
	{
		UnitError(lpUnit, "CastOnUnit");
		return FALSE;
	}

	if (!SelectSpell(wSpell, bLeftHand))
	{
		SpellError(wSpell);
		return FALSE;
	}

	POINT mp = GetUnitPosition(lpUnit);
	
	if (wSpell == g_dsi.wSpellID
		&& GetDistanceFrom(mp.x, mp.y) > 5
		&& (g_dsi.dwSpellInfoFlag & DSI_MELEE))
	{
		// cast a melee-only spell on an entity requires approaching the entity first!
		GoToEntity(lpUnit, (WORD)mp.x, (WORD)mp.y, TRUE);
	}

	BYTE aPacket[16] = { 0 };
	aPacket[0] = bLeftHand ? 0x06 : 0x0d;
	::memcpy(aPacket + 1, &lpUnit->dwUnitType, 4);
	::memcpy(aPacket + 5, &lpUnit->dwUnitID, 4);

	if (!GameSendPacketToServer(aPacket, 9))
		return FALSE;

	if (IsGameReady())
	{
		DWORD dwPlayerID = GetSafePlayerID();
		::memset(aPacket, 0, sizeof(aPacket));
		aPacket[0] = 0x4c;
		::memcpy(aPacket + 2, &dwPlayerID, 4);
		::memcpy(aPacket + 6, &wSpell, 2);
		aPacket[8] = 0x01;
		aPacket[9] = (BYTE)lpUnit->dwUnitType;
		::memcpy(aPacket + 10, &lpUnit->dwUnitID, 4);	
		GameSendPacketToGame(aPacket, 16);
	}

	return TRUE;	
}

BOOL EXPORT DrinkBeltPotion(DWORD dwItemID, BOOL bFeedMerc)
{
	if (dwItemID == 0)
	{
		ItemError(dwItemID, "DrinkBeltPotion");
		return 0;
	}

	BYTE aPacket[13] = { 0x26 };
	memcpy(aPacket + 1, &dwItemID, 4);
	aPacket[5] = bFeedMerc ? 0x01 : 0x00;
	memset(aPacket + 6, 0x00, 7);
	return GameSendPacketToServer(aPacket, 13) ? dwItemID : 0;
}

BOOL EXPORT DrinkInventoryPotion(DWORD dwItemID)
{
	if (dwItemID == 0)
	{
		ItemError(dwItemID, "DrinkInventoryPotion");
		return 0;
	}

	POINT mp = GetPosition();
	BYTE aPacket[13] = {0};
	aPacket[0] = 0x20;
	::memcpy(aPacket + 1, &dwItemID, 4);
	::memcpy(aPacket + 5, &mp.x, 2);
	::memcpy(aPacket + 9, &mp.y, 2);
	return GameSendPacketToServer(aPacket, 13) ? dwItemID : 0;
}

DWORD EXPORT HealMe(BOOL bKeepBottomRow) // Drink a purple/red potion
{
	// first check life percent
	const BYTE HP = GetHPPercent();
	const LPCSTR* ORDER = NULL;
	
	if (HP > 75)
	{
		// red -> rejuv -> full rejuv
		ORDER = RSL;
	}
	else if (HP > 50)
	{
		// rejuv -> red -> full rejuv
		ORDER = SRL;
	}
	else
	{
		// full rejuv -> rejuv -> red
		ORDER = LSR;
	}

	for (int i = 0; ORDER && ORDER[i]; i++)
	{
		DWORD dwItemID = FindFirstInventoryPotion(ORDER[i]);
		if (dwItemID)
			return DrinkInventoryPotion(dwItemID);

		dwItemID = FindFirstBeltPotion(ORDER[i], bKeepBottomRow);
		if (dwItemID)
			return DrinkBeltPotion(dwItemID, FALSE);		
	}
	
	return 0;
}

DWORD EXPORT HealMerc(BOOL bKeepBottomRow) // Feed the merc a purple/red potion
{
	// first check life percent
	GAMEUNIT gu;
	if (!GetMyMercUnit(&gu))
		return 0;
	
	const BYTE HP = GetUnitHPPercent(&gu);
	const LPCSTR* ORDER = NULL;
	
	if (HP > 75)
	{
		// red -> rejuv -> full rejuv
		ORDER = RSL;
	}
	else if (HP > 50)
	{
		// rejuv -> red -> full rejuv
		ORDER = SRL;
	}
	else
	{
		// full rejuv -> rejuv -> red
		ORDER = LSR;
	}

	for (int i = 0; ORDER && ORDER[i]; i++)
	{
		DWORD dwItemID = FindFirstBeltPotion(ORDER[i], bKeepBottomRow);
		if (dwItemID)
			return DrinkBeltPotion(dwItemID, TRUE);		
	}

	return 0;
}

DWORD EXPORT SupplyMana(BOOL bDrinkPurpleIfNoBlue, BOOL bKeepBottomRow) // Drink a blue/purple potion for mana
{
	DWORD dwItemID = FindFirstInventoryPotion("mp");
	if (dwItemID)
		return DrinkInventoryPotion(dwItemID);

	dwItemID = FindFirstBeltPotion("mp", bKeepBottomRow);
	if (dwItemID)
		return DrinkBeltPotion(dwItemID, FALSE);

	if (!bDrinkPurpleIfNoBlue)
		return 0;

	for (int i = 0; SL[i]; i++)
	{
		DWORD dwItemID = FindFirstInventoryPotion(SL[i]);
		if (dwItemID)
			return DrinkInventoryPotion(dwItemID);

		dwItemID = FindFirstBeltPotion(SL[i], bKeepBottomRow);
		if (dwItemID)
			return DrinkBeltPotion(dwItemID, FALSE);
	}
	
	return 0;
}

BOOL EXPORT MoveTo(long x, long y, BOOL bQueueJob) // Run to a position
{
	// We do not drive the player to run immediately, instead, we 
	// put the destination positions in to a FIFO queue and run to
	// those positions at each OnGameTimerTick call.
	if (!bQueueJob)
		CleanJobs();	

	// queue this new job
	MOTIONJOB job = { 0 };
	job.nJobType = MJ_POSITION;
	job.x = (WORD)x;
	job.y = (WORD)y;
	AddMotionJob(job);
	return TRUE;
}

BOOL EXPORT MoveToUnit(LPCGAMEUNIT lpUnit, BOOL bQueueJob) // Run to a static object
{
	if (lpUnit == NULL)
		return FALSE;
	
	if (!bQueueJob)
		CleanJobs();

	// queue this new job
	MOTIONJOB job = { 0 };
	job.nJobType = MJ_UNIT;
	memcpy(&job.unit, lpUnit, sizeof(GAMEUNIT));
	AddMotionJob(job);
	return TRUE;
}

BOOL EXPORT TeleportTo(long x, long y, BOOL bQueueJob) // teleport to a position
{
	if (!HaveSpell(D2S_TELEPORT))
	{
		ServerErrorf("Player does not have Teleport spell");
		return FALSE;
	}

	if (!bQueueJob)
		CleanJobs();

	// queue this new job
	MOTIONJOB job = { 0 };
	job.nJobType = MJ_TELEPORT;
	job.x = (WORD)x;
	job.y = (WORD)y;
	AddMotionJob(job);
	return TRUE;
}

BOOL EXPORT Say(LPCSTR lpszMessage) // Say a message
{
	if (lpszMessage == NULL || lpszMessage[0] == '\0')
		return FALSE;

	const int LEN = strlen(lpszMessage);
	BYTE* aPacket = new BYTE[LEN + 6];
	memset(aPacket, 0, LEN + 6);

	aPacket[0] = 0x15;
	aPacket[1] = 0x01;
	::memcpy(aPacket + 3, lpszMessage, LEN);
	BOOL bOK = GameSendPacketToServer(aPacket, LEN + 6);
	delete [] aPacket;
	return bOK;
}

BOOL EXPORT OverHead(LPCSTR lpszMessage, BOOL bClientOnly) // Display an over-head message
{
	if (lpszMessage == NULL || lpszMessage[0] == '\0')
		return FALSE;

	const int MSGLEN = strlen(lpszMessage);
	BYTE* aPacket = new BYTE[MSGLEN + 12];
	::memset(aPacket, 0, MSGLEN + 12);

	BOOL bOK = FALSE;
	if (bClientOnly)
	{
		DWORD dwPlayerID = GetSafePlayerID();
		aPacket[0] = 0x26;
		aPacket[1] = 0x05;
		::memcpy(aPacket + 4, &dwPlayerID, 4);
		WORD wRand = (rand() % 65535) + 1;
		::memcpy(aPacket + 8, &wRand, 2);
		::memcpy(aPacket + 11, lpszMessage, MSGLEN); // copy whole string including the tail-null
		bOK = GameSendPacketToGame(aPacket, MSGLEN + 12);
	}
	else
	{
		aPacket[0] = 0x14;
		::memcpy(aPacket + 3, lpszMessage, MSGLEN);
		bOK = GameSendPacketToServer(aPacket, MSGLEN + 6);
	}

	delete [] aPacket;
	return bOK;
}

BOOL EXPORT SendSound(BYTE iSound) // Send a sound
{
	// 0x19 - 0x20
	iSound = max(0x19, iSound);
	iSound = min(0x20, iSound);

	BYTE aPacket[3] = { 0 };
	aPacket[0] = 0x3f;
	aPacket[1] = iSound;

	return GameSendPacketToServer(aPacket, 3);	
}

BOOL EXPORT AntiIdle() // Anti-idle
{
	DWORD dwTick = ::GetTickCount();
	if (dwTick % 2)
	{
		return OverHead("Anti-idle", FALSE);
	}
	else
	{
		// send a random sound
		BYTE i = BYTE(dwTick % 0x08) + 0x19;
		return SendSound(i);
	}
}

BOOL EXPORT WeaponSwitch() // Do weapon switch
{
	BYTE aPacket[1] = { 0x60 };
	return GameSendPacketToServer(aPacket, 1);
}

BOOL EXPORT PickGroundItem(DWORD dwItemID, BOOL bShowClientMove) // Pick up a ground item
{
	UnitItem* p = (UnitItem*)VerifyItem(dwItemID);
	if (p == NULL)
	{
		//UnitError(lpUnit, "PickGroundItem");
		return FALSE;
	}

	if (p->dwMode != ITEM_MODE_ON_GROUND && p->dwMode != ITEM_MODE_BEING_DROPPED)
	{
		//ServerErrorf("0x%08X is not a ground item");
		return FALSE; // not a ground item
	}

	if (bShowClientMove && GetDistanceFrom(p->hOPath->Xpos, p->hOPath->Ypos) > 5)
	{
		GAMEUNIT gu = {	dwItemID, UNIT_TYPE_ITEM };
		GoToEntity(&gu, (WORD)p->hOPath->Xpos, (WORD)p->hOPath->Ypos, bShowClientMove);
	}

	BYTE aPacket[13] = { 0 };
	aPacket[0] = 0x16;
	aPacket[1] = 0x04;
	::memcpy(aPacket + 5, &dwItemID, 4);

	return GameSendPacketToServer(aPacket, 13);
}

BOOL EXPORT ResurrectMerc() // Resurrect merc
{
	// must be a resurrector!
	if (!IsCurrentNpcResurrector())
	{
		ServerErrorf("Current npc is not a resurrector.");
		return FALSE;
	}
	
	if (!IsNpcActive() || IsMercAlive()) // do not resurrect if the merc is alive
		return FALSE;

	DWORD dwNpcID = GetCurrentNpcID();
	BYTE aPacket[5] = { 0 };
	aPacket[0] = 0x62;
	::memcpy(aPacket + 1, &dwNpcID, 4);
	return GameSendPacketToServer(aPacket, 5);
}

BOOL EXPORT BuyItem(DWORD dwItemID) // Buy an item from npc
{
	if (!IsNpcActive(NPC_TRADE) || !VerifyItem(dwItemID))
		return FALSE;

	DWORD dwNpcID = GetCurrentNpcID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x32;
	::memcpy(&aPacket[1], &dwNpcID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	return GameSendPacketToServer(aPacket, 17);
}

BOOL EXPORT Gamble(DWORD dwItemID) // gamble an item
{
	if (!IsNpcActive(NPC_GAMBLE) || !VerifyItem(dwItemID))
		return FALSE;

	DWORD dwNpcID = GetCurrentNpcID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x32;
	::memcpy(&aPacket[1], &dwNpcID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	aPacket[9] = 0x02;
	return GameSendPacketToServer(aPacket, 17);
}

BOOL VerifyItem(DWORD dwItemID)
{
	if (dwItemID == 0)
		return NULL;

	UnitAny* p = D2CLIENT_GetUnit(dwItemID, UNIT_TYPE_ITEM);
	if (p == NULL || p->ptItemData == NULL)
		return NULL;

	return (BOOL)p;
}

BOOL EXPORT RepairItem(DWORD dwItemID) // Repair items
{
	if (!IsNpcActive(NPC_TRADE))
		return FALSE;

	if (dwItemID && !VerifyItem(dwItemID))
		return FALSE;	

	DWORD dwNpcID = GetCurrentNpcID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x35;
	::memcpy(aPacket + 1, &dwNpcID, 4);

	if (dwItemID)
	{
		// repair a particular item
		::memcpy(aPacket + 5, &dwItemID, 4);
	}
	else
	{
		// repair all
		aPacket[16] = 0x80;
	}

	return GameSendPacketToServer(aPacket, 17);
}

BOOL EXPORT IdentifyItems()
{
	if (!IsNpcActive())
		return FALSE;

	DWORD dwNpcID = GetCurrentNpcID();
	BYTE aPacket[5] = { 0 };
	aPacket[0] = 0x34;
	::memcpy(aPacket + 1, &dwNpcID, 4);
	return GameSendPacketToServer(aPacket, 5);
}

BOOL EXPORT DepositGoldToStash(DWORD dwAmount) // Deposit inventory gold to stash
{
	if (dwAmount == 0)
		dwAmount = GetStat(STAT_GOLD);

	if (dwAmount == 0)
		return FALSE; // no gold in inventory

	BYTE aPacket[7] = { 0 };
	WORD g1 = 0, g2 = 0;

	// build deposit packet
	aPacket[0] = 0x4f;
	aPacket[1] = 0x14;

	g2 = WORD(dwAmount / 65536);
	g1 = WORD(dwAmount % 65536);
	
	::memcpy(&aPacket[3], &g2, 2);
	::memcpy(&aPacket[5], &g1, 2);

	return GameSendPacketToServer(aPacket, 7);
}

BOOL EXPORT RedrawClient(BOOL bFade) // Redraw client view
{
	DWORD dwPlayerID = GetSafePlayerID();
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return FALSE;

	BYTE aPacket[11] = { 0 };
	aPacket[0] = 0x15;
	::memcpy(aPacket + 2, &dwPlayerID, 4);
	::memcpy(aPacket + 6, &pPlayer->wX, 2);
	::memcpy(aPacket + 8, &pPlayer->wY, 2);

	if (bFade)
		aPacket[10] = 0x01;

	return GameSendPacketToGame(aPacket, 11);
}

BYTE EXPORT GetOpenedUI()
{
	for (int i = 0; CLOSABLEUIS[i]; i++)
	{
		if (IsUIOpened(CLOSABLEUIS[i]))
			return CLOSABLEUIS[i];
	}

	return 0;
}

void EXPORT CloseAllUIs()
{
	if (IsNpcBullShitting())
		SendKey(VK_ESCAPE);
	g_bClosingUI = TRUE;
}

DWORD EXPORT GetStashGoldLimit() // stash gold limit
{
	return D2COMMON_GetStashGoldLimit(D2CLIENT_GetPlayerUnit());
}

DWORD EXPORT GetInventoryGoldLimit() // inventory gold limit
{
	GAMEUNIT unit;
	MakePlayerData(&unit);
	DWORD dwLevel = GetUnitStat(&unit, STAT_LEVEL);
	return dwLevel * 10000;
}

int EXPORT GetMaxDifficulty()	// the max difficulty to which this player has access
{
	BnetData BnData = **p_D2LAUNCH_bnData;
	if((BnData.MaxDifficulty & 0x0E)==0x0E){ return 3; }
	else if((BnData.MaxDifficulty & 0x0C)==0x0C){ return 2; }
	else if((BnData.MaxDifficulty & 0x08)==0x08){ return 1; }
	else { return 0; }
}

BOOL EXPORT IsHardcore() // is hardcore player?
{
	BnetData BnData = **p_D2LAUNCH_bnData;
	return (BnData.CharFlags & 0x04) == 0x04;
}

BOOL EXPORT GetAlwaysRun() // is "always run" on?
{
	return (BOOL)*p_D2CLIENT_bAlwaysRun;
}

DWORD EXPORT GetCursorItem() // returns ID of the item on cursor, if any
{
	UnitAny* p = D2COMMON_GetCursorItem(D2CLIENT_GetPlayerUnit()->ptInventory);
	return p ? p ->dwId : 0;
}

LPCSTR EXPORT GetAccountName()
{
	return (**p_D2LAUNCH_bnData).AccountName;
}

BOOL EXPORT DropCursorItemToGround()
{
	DWORD dwItem = GetCursorItem();
	if (dwItem == 0)
		return FALSE;

	BYTE aPacket[5] = {0x17}; // 1.10 confirmed
	::memcpy(aPacket + 1,  &dwItem, 4);
	return GameSendPacketToServer(aPacket, 5);
}

BOOL EXPORT Transmute()
{
	BYTE aPacket[7] = {0};
	aPacket[0] = 0x4f;
	aPacket[1] = 0x18;
	return GameSendPacketToServer(aPacket, 7);
}

BOOL EXPORT IsUIOpened(int nUIType)
{
	if (nUIType == 0 || nUIType > 0x25)
		return FALSE;

	return D2CLIENT_GetUIState(nUIType);
}

BOOL EXPORT SellItem(DWORD dwItemID)
{
	// check if it's a quest item, selling a quest item causes game crash
	char szItemCode[4] = "";
	if (!GetItemCode(dwItemID, szItemCode, 3))
		return FALSE;
	
	if (IsQuestItem(szItemCode))
		return FALSE;
	
	if (!IsNpcActive(NPC_TRADE) && !IsNpcActive(NPC_GAMBLE))
		return FALSE;

	DWORD dwNpcID = GetCurrentNpcID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x33;
	::memcpy(&aPacket[1], &dwNpcID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	aPacket[13] = 0x01;
	return GameSendPacketToServer(aPacket, 17);
}

BOOL EXPORT IsMercAlive()
{
	GAMEUNIT gu = {0};
	if (!GetMyMercUnit(&gu))
		return FALSE;

	return GetUnitMode(&gu) != NPC_MODE_DEAD && GetUnitStat(&gu, STAT_HP);
}

BOOL EXPORT DropGold(DWORD dwGoldAmount)
{
	DWORD dwPlayerID = GetSafePlayerID();
	if (dwPlayerID == 0 || dwGoldAmount == 0 || dwGoldAmount > GetStat(STAT_GOLD))
		return FALSE;

	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x50;
	::memcpy(aPacket + 1, &dwPlayerID, 4);
	::memcpy(aPacket + 5, &dwGoldAmount, 4);
	return GameSendPacketToServer(aPacket, 9);
}

DWORD EXPORT GetSpellCharges(WORD wSpellID)
{
	if (wSpellID == D2S_SCROLLOFTOWNPORTAL)
		return GetInventoryTPScrolls();

	if (wSpellID == D2S_SCROLLOFIDENTIFY)
		return GetInventoryIDScrolls();

	if (wSpellID == D2S_TOMEOFTOWNPORTAL)
		return GetInventoryTPBookCharges();

	if (wSpellID == D2S_TOMEOFIDENTIFY)
		return GetInventoryIDBookCharges();
	
	return 0;
}

int EXPORT SellInventoryItems(int nLeftColumn, int nRightColumn, DWORD dwDelay)
{
	if (!IsNpcActive(NPC_TRADE) && !IsNpcActive(NPC_GAMBLE))
		return 0;

	CArrayEx<DWORD, DWORD> aSold;
	CStorageBuffer sb(STORAGE_INVENTORY);
	nLeftColumn = max(nLeftColumn, 0);
	nRightColumn = min(nRightColumn, 9);
	DWORD dwNpcID = GetCurrentNpcID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x33;
	::memcpy(&aPacket[1], &dwNpcID, 4);
	aPacket[13] = 0x01;

	for (int x = nLeftColumn; x <= nRightColumn; x++)	
	{
		for (int y = 0; y < 4; y++)
		{
			DWORD dwItemID = sb.GetItem(x, y);
			if (dwItemID && dwItemID != -1 && aSold.Find(dwItemID) == -1)
			{
				// check if it's a quest item, selling a quest item causes game crash
				char szItemCode[4] = "";
				if (!GetItemCode(dwItemID, szItemCode, 3))
					continue;
	
				if (IsQuestItem(szItemCode))
					continue;

				aSold.Add(dwItemID); // aviod repetitive selling

				//GameInfof("sell %08X, %d, %d", dwItemID, x, y);
				::memcpy(&aPacket[5], &dwItemID, 4);				
				GameSendPacketToServer2(aPacket, 17, dwDelay);
			}
		}
	}
	
	return aSold.GetSize();
}

BOOL EXPORT IsWeaponSwitched()
{
	return (BOOL)*p_D2CLIENT_bWeapSwitch;
}

void CheckPlayerMap()
{
	if (g_iCurMapID == 0)
	{
		g_iCurMapID = GetCurrentMapID();
		OnMapChanged(g_iCurMapID); // server20.cpp
		return;
	}

	// do map comparing every 300 milliseconds
	if (GetInGameTime() % 300)
		return;

	BYTE iMap = GetCurrentMapID();
	if (iMap == 0 || iMap == g_iCurMapID)
		return;

	OnMapChanged(iMap); // server20.cpp

	BYTE iOldMap = g_iCurMapID;
	g_iCurMapID = iMap;

	// was in a wp proc?
	OnMapChangeCheckWayPoint(g_iCurMapID);

	// Map changed
	SendThisPlayerMessage(PM_MAPCHANGE, iOldMap, g_iCurMapID);

	// entered/left town?
	BOOL bWasTown = D2MapIsTown(iOldMap);
	BOOL bIsTown = D2MapIsTown(g_iCurMapID);
	if (bWasTown && !bIsTown)
	{
		// leave town
		SendThisPlayerMessage(PM_LEAVETOWN, (WPARAM)iOldMap, (LPARAM)g_iCurMapID);
	}
	else if (!bWasTown && bIsTown)
	{
		// enter town
		SendThisPlayerMessage(PM_ENTERTOWN, (WPARAM)iOldMap, (LPARAM)g_iCurMapID);
	}
	
	// act changed also?
	BYTE iOldAct = D2GetActFromMap(iOldMap);
	BYTE iNewAct = D2GetActFromMap(g_iCurMapID);

	if (iOldAct != iNewAct)
		SendThisPlayerMessage(PM_ACTCHANGE, iOldAct, iNewAct);		
}

int EXPORT GetAngleFrom(long x, long y)
{
	POINT mp = GetPosition();
	return GetAngle(mp.x, mp.y, x, y);
}

BOOL EXPORT PutInventoryPotionToBelt(DWORD dwItemID)
{
	if (!IsMyInventoryItem(dwItemID))
		return FALSE;

	char szCode[4] = "";
	GetItemCode(dwItemID, szCode, 3);
	if (GetAvailableColumnFor(D2IsPotion(szCode)) == -1)
		return FALSE; // no available column

	BYTE aPacket[5];
	aPacket[0] = 0x63;
	::memcpy(aPacket + 1, &dwItemID, 4);
	return GameSendPacketToServer(aPacket, 5);
}

BOOL CALLBACK FindCorpseProc(UnitPlayer* pPlayer, Room* pRoom, UnitAny* pUnit, LPARAM lParam1, LPARAM lParam2)
{
	CArrayEx<DWORD, DWORD>* pList = (CArrayEx<DWORD, DWORD>*)lParam1;
	if (pList == NULL)
		return FALSE;

	// player's corpse: id!=playerid,name=playername
	if (pUnit->dwId == pPlayer->dwId)
		return TRUE;

	LPCWSTR wszPlayer = D2CLIENT_GetNameFromUnit(pPlayer);
	if (wszPlayer == NULL)
		return TRUE;

	LPCWSTR wszUnit = D2CLIENT_GetNameFromUnit(pUnit);
	if (wszUnit == NULL)
		return TRUE;

	if (::wcsicmp(wszPlayer, wszUnit) == 0)
	{
		if (pList->Find(pUnit->dwId) == -1)
		{
			pList->Add(pUnit->dwId);
			if (lParam2)
				return FALSE;
		}
	}

	return TRUE;
}

int EXPORT GetMyCorpseCount()
{
	CArrayEx<DWORD, DWORD> aList;
	InternalEnumUnits(UNIT_TYPE_PLAYER, FindCorpseProc, (LPARAM)&aList, 0);
	return aList.GetSize();
}

BOOL EXPORT PickMyCorpse()
{
	// just pick the first corpse
	CArrayEx<DWORD, DWORD> aList;
	InternalEnumUnits(UNIT_TYPE_PLAYER, FindCorpseProc, (LPARAM)&aList, 1);
	if (aList.IsEmpty())
		return FALSE; // no nearby corpse found

	GAMEUNIT gu;
	gu.dwUnitID = aList[0];
	gu.dwUnitType = UNIT_TYPE_PLAYER;
	POINT mp = GetUnitPosition(&gu);
	if (GetDistanceFrom(mp.x, mp.y) > 5)
		GoToEntity(&gu, (WORD)mp.x, (WORD)mp.y, TRUE);

	return Interact(&gu);
}

DWORD EXPORT GetClassID()
{
	UnitAny* p = D2CLIENT_GetPlayerUnit();
	return p ? p->dwClassId : 0;
}

BOOL CALLBACK FindHoistilerProc(DWORD dwPlayerID, LPCSTR lpszPlayerName, DWORD dwPlayerClass, DWORD dwPvpFlags, BYTE iMapID, LPARAM lParam)
{
	int* pCount = (int*)lParam;
	if (pCount == NULL)
		return FALSE;

	if (GetPvpFlags(dwPlayerID) & PVP_HOSTILED_YOU)
		*pCount += 1;

	return TRUE;
}

DWORD EXPORT GetPvpFlags(DWORD dwPlayerID)
{
	if (dwPlayerID == GetSafePlayerID())
		return 0;

	DWORD dwFlags = 0;
	for (Roster* p = *p_D2CLIENT_FirstRosterUnit; p; p = p->ptNext)
	{
		if (p->PlayerId != dwPlayerID)
			continue;

		// hostiled me?
		if (D2CLIENT_TestPvpFlag(dwPlayerID, GetSafePlayerID(), 8))
			dwFlags |= PVP_HOSTILED_YOU;

		// hostiled by me?
		if (D2CLIENT_TestPvpFlag(GetSafePlayerID(), dwPlayerID, 8))
			dwFlags |= PVP_HOSTILED_BY_YOU;

		// invited me?
		if (p->_4 == 2)
			dwFlags |= PVP_INVITED_YOU;

		// invited by me?
		if (p->_4 == 4)
			dwFlags |= PVP_INVITED_BY_YOU;

		// partied?
		if (p->_3 != 0xffff && p->_4 == 1)
			dwFlags |= PVP_ALLIED;
	}
	
	return dwFlags;
}

BOOL EXPORT GetMyMercUnit(LPGAMEUNIT lpBuffer) // Get player's mercenary
{
	if (lpBuffer == NULL)
		return FALSE;
	
	::memset(lpBuffer, 0, sizeof(GAMEUNIT));
	
	for (RosterPet* p = *p_D2CLIENT_FirstRosterPet; p; p = p->PetNext)
	{
		if (IsMercClassID(p->ClassId) && p->OwnerId == GetSafePlayerID())
		{
			lpBuffer->dwUnitID = p->Id;
			lpBuffer->dwUnitType = UNIT_TYPE_MONSTER;
		}
	}
	return lpBuffer->dwUnitID;
}

DWORD EXPORT GetBaseStat(int nStatID)
{
	return ServerGetUnitStat((UnitAny*)D2CLIENT_GetPlayerUnit(), nStatID, TRUE);
}

DWORD EXPORT GetMercEquip(int nEquipLocation)
{
	if (nEquipLocation != EQUIP_HEAD && nEquipLocation != EQUIP_BODY && nEquipLocation != EQUIP_RIGHT_PRIMARY)
		return 0;

	GAMEUNIT gu;
	if (!GetMyMercUnit(&gu))
		return 0;

	UnitAny* pUnit = (UnitAny*)VerifyUnit(&gu);
	if (pUnit == NULL || pUnit->ptInventory == NULL)
		return 0;

	for (UnitItem* p = D2COMMON_GetFirstInventoryItem(pUnit->ptInventory); p; p = D2COMMON_GetNextInventoryItem(p))
	{
		if (p->ptItemData == NULL || p->hOPath == NULL || p->hOPath->Xpos != (WORD)nEquipLocation)
			continue;

		return p->dwId;
	}

	return 0;
}

