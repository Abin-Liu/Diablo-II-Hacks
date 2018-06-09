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
#include "Utilities\\StateTicker.h"
#include <math.h>
#include "Inventory.h"
#include "npc.h"
#include "SpellDef.h"
#include "MotionJob.h"

typedef struct tagChargeable
{
	WORD wSpellID; // spell id
	DWORD dwCharges; // remaining charges
} CHARGEABLE, *LPCHARGEABLE;
typedef const tagChargeable* LPCCHARGEAABLE;

BOOL operator==(const CHARGEABLE& lhs, const CHARGEABLE& rhs)
{
	return lhs.wSpellID == rhs.wSpellID;
}

BOOL operator>(const CHARGEABLE& lhs, const CHARGEABLE& rhs)
{
	return lhs.wSpellID > rhs.wSpellID;
}

typedef struct tagNpcStore
{
	GAMEUNIT npc;
	int nInteractType;
	BOOL bSessionStarted;
	
	// used for compare potion levels
	char szHealingPotionCode[4]; // hp1/hp2/hp3/hp4/hp5
	char szManaPotionCode[4]; // mp1/mp2/mp3/mp4/mp5
	
	// scrolls
	DWORD dwTownScrollID;
	DWORD dwIdentifyScrollID;

	// potions
	DWORD dwHealingPotionID;
	DWORD dwManaPotionID;
	DWORD dwAntidotePotionID;

	// ammo
	DWORD dwArrowID;
	DWORD dwBoltID;

	// gamble
	DWORD aAmulets[5]; // actually there will be only 1 amulet
	DWORD aRings[5]; // actually there will be only 1-2 rings
	DWORD aCirclets[5];
	DWORD aGloves[5];
	DWORD aBoots[5];
	DWORD aBelts[5];

} NPCSTORE, *LPNPCSTORE;
typedef const tagNpcStore* LPCNPCSTORE;

enum { NPC_NONE = 0, NPC_RUNTO, NPC_INTERACT, NPC_WAIT_INTERACT, NPC_OPENSTORE, NPC_WAIT_STORE, NPC_STORE_OK };

static const BYTE CLOSABLEUIS[] = { UI_INVENTORY, UI_CHARACTER, UI_MINISKILL, UI_SKILL, UI_CHAT, UI_NPCMENU, UI_MAINMENU, UI_CONFIG, UI_NPCSHOP, UI_ANVIL, UI_QUEST, UI_STATUSAREA, UI_WPMENU, UI_PARTY, UI_TRADE, UI_MSGS, UI_STASH, UI_CUBE, UI_HELP, UI_MERC, UI_SCROLL, 0x00 }; 

WORD g_wPrevHP = 0; // Previous HP
WORD g_wPrevMana = 0; // Previous mana
BOOL g_bGameJoined = FALSE; // Is OnGameJoin called?
DWORD g_dwPlayerID = 0; // Player ID(before OnGameJoin is called)
char g_szPlayerName[32] = ""; // Player name(before OnGameJoin is called)
BYTE g_iCurMapID = 0; // current map
BOOL g_bGameReady = FALSE; // Game ready flag
ITEM g_aBeltItems[4][4];	// Items in belt
BYTE g_iBeltRows; // Current belt row number
D2SPELLINFO g_dsi = {0}; // Most recently utilized spell
BOOL g_bPaused = FALSE; // Paused?

WORD g_wLSpell = 0xffff; // Left-hand spell
WORD g_wRSpell = 0xffff; // Right-hand spell

#define TP_CAST_TICK		1000	// Town portal casting checking interval
#define TP_ENTER_TICK		500		// Town portal interaction checking interval

enum {	TS_NONE = 0,			// nothing
		TS_CAST_PORTAL,			// open a town portal
		TS_WAIT_PORTAL,			// wait for portal to appear
		TS_VERIFY_PORTAL,		// Verify portal mode, thanks to Mousepad & Ninjai
		TS_ENTER_PORTAL,		// enter the town portal
		TS_VERIFY_INTOWN };		// verify in town

int g_nTPFail = 0;					// Number of town portal failing
int g_nNpcFail = 0;					// Number of npc interaction failing

CStateTicker g_npcState;				// Ticker counter
CStateTicker g_townState;
CStateTicker g_wpState;

GAMEUNIT g_myPortal = {0};		// My town portal unit

CArrayEx<CHARGEABLE, const CHARGEABLE&> g_aSkillCharges;	// Chargable spells(spell+charges)
CArrayEx<DWORD, DWORD> g_aHostilers;	// Hostilers

CArrayEx<DWORD, DWORD> g_aBuys; // items to be bought
CArrayEx<DWORD, DWORD> g_aSells; // items to be sold
CArrayEx<DWORD, DWORD> g_aGambles; // items to be gambled

CInventory g_inventory; // inventory space tracer
BOOL g_bClosingUI = FALSE;

NPCSTORE g_npcStore = { 0 };

// wp state
enum { WP_NONE = 0, WP_INTERACT, WP_WAITUI, WP_ENTER, WP_VERIFYDEST, WP_FINISH };
GAMEUNIT g_waypoint = { 0 };
BYTE g_iWPdest = 0;
int g_nWPRetries = 0;
BOOL g_bWpOpened = FALSE;

VOID Player_OnGameJoin(THISGAMESTRUCT* thisgame)
{	
	g_bGameReady = FALSE;	
	g_dwPlayerID = thisgame->player->PlayerID;
	::strcpy(g_szPlayerName, thisgame->player->PlayerName);
	g_iCurMapID = 0;
	g_wPrevHP = 0;
	g_wPrevMana = 0;
	::memset(g_aBeltItems, 0, sizeof(ITEM) * 16);
	g_iBeltRows = 1;
	::memset(&g_dsi, 0, sizeof(D2SPELLINFO));
	g_wLSpell = 0xffff;
	g_wRSpell = 0xffff;
	g_bPaused = FALSE;
	CleanJobs();
	g_nTPFail = 0;
	g_nNpcFail = 0;
	::memset(&g_myPortal, 0, sizeof(GAMEUNIT));
	g_bClosingUI = FALSE;
	g_nWPRetries = 0;
	::memset(&g_waypoint, 0, sizeof(GAMEUNIT));
	g_iWPdest = 0;
	g_bWpOpened = IsUIOpened(UI_WPMENU);
	::memset(&g_npcStore, 0, sizeof(NPCSTORE));
	g_inventory.SetValid(WasD2HackitLoadedBeforeJoinGame()); // cannot be used if not loaded before joining

	g_npcState.Clear();
	g_townState.Clear();
	g_wpState.Clear();

	g_aBuys.RemoveAll();
	g_aSells.RemoveAll();
	g_aGambles.RemoveAll();

	g_bGameJoined = TRUE;
}

VOID Player_OnGameLeave()
{
	g_bGameJoined = FALSE;	
	g_dwPlayerID = 0;
	::memset(g_szPlayerName, 0, sizeof(g_szPlayerName));
	g_aSkillCharges.RemoveAll();
	g_aHostilers.RemoveAll();
	g_inventory.ClearAll();
}

DWORD Player_OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if (aPacket[0] == 0x49)
	{
		if (!g_bWpOpened && GetGameIP() != NULL && strlen(GetGameIP()) > 7)
			return 0; // block invalid wp packet unless it's single player game
		
		g_bWpOpened = FALSE;
	}

	
	if (aLen == 9
		&& aPacket[0] == 0x30
		&& aPacket[1] == 0x01
		&& !::memcmp(aPacket + 5, &g_npcStore.npc.dwUnitID, 4))
	{
		g_aBuys.RemoveAll();
		g_aSells.RemoveAll();
		g_aGambles.RemoveAll();
		g_npcStore.bSessionStarted = FALSE;
	}

	return aLen;
}

DWORD Player_TimerTick()
{
	g_npcState.TickIt(100);
	g_townState.TickIt(100);
	g_wpState.TickIt(100);
	OnJobTicker();
	
	////////////////////////////////////////////////////////////////
	// Steps below here are checked as soon as joining game
	////////////////////////////////////////////////////////////////
	if (!g_bGameReady)
	{
		BOOL bReady = IsGameReady();
		if (bReady)
		{
			g_bGameReady = TRUE;			
			NotifyThisPlayerMessage(PM_GAMEREADY);
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
			NotifyThisPlayerMessage(PM_UICLOSED);
		}
	}

	// Map check
	CheckPlayerMap();
	
	////////////////////////////////////////////////////////////////
	// Steps below here require game ready & not paused
	////////////////////////////////////////////////////////////////
	if (!g_bGameReady || g_bPaused)
		return 0;	

	DoTPProc();
	DoWPProc();
	DoNpcProc();
	DoJobProc();
	DoGambleProc(); // multiple gamble packets cannot be sent altogether or items won't identify, lol

	return 0;
}

void DoNpcProc()
{
	if (g_npcState.GetState() == NPC_NONE || !g_npcState.IsTickEnded())
		return;
	
	if (g_npcStore.npc.dwUnitID == 0)
	{
		NotifyNpcSession(FALSE);
		return;
	}

	switch (g_npcState.GetState())
	{
	case NPC_RUNTO:
		if (!MoveToUnit(&g_npcStore.npc))
		{
			NotifyNpcSession(FALSE);
		}
		else
		{
			g_npcState.Update(NPC_INTERACT, 1000);
		}
		break;

	case NPC_INTERACT:
		if (!PrepareInteractWithNPC(&g_npcStore.npc) || !Interact(&g_npcStore.npc))
		{
			NotifyNpcSession(FALSE);
		}
		else
		{
			g_npcState.Update(NPC_WAIT_INTERACT, 2000);
		}
		break;

	case NPC_WAIT_INTERACT:
		if (IsUIOpened(UI_NPCMENU))
		{
			g_nNpcFail = 0;
			if (g_npcStore.nInteractType)
			{
				g_npcState.Update(NPC_OPENSTORE, 500);
			}
			else
			{
				NotifyNpcSession(TRUE);
			}			
		}
		else
		{			
			g_nNpcFail++;
			GameInfof("npc fails: %d", g_nNpcFail);
			if (g_nNpcFail < MAX_RETRY)
			{
				//if (IsNpcBullShitting())
				//	SendKey(VK_ESCAPE); // close the npc bull-shitting screen
				CloseAllUIs();
				g_npcState.Update(NPC_INTERACT, 500);
			}
			else
			{
				NotifyNpcSession(FALSE);
			}
		}
		break;

	case NPC_OPENSTORE:
		if (!IsUIOpened(UI_NPCMENU) || !OpenNpcStore(&g_npcStore.npc, g_npcStore.nInteractType == NPC_GAMBLE))
		{
			NotifyNpcSession(FALSE);
		}
		else
		{
			g_npcState.Update(NPC_WAIT_STORE, 5000);
		}
		break;

	case NPC_WAIT_STORE:
		if (!IsUIOpened(UI_NPCMENU) || g_nNpcFail)
		{
			NotifyNpcSession(FALSE);
		}
		else
		{
			g_nNpcFail++;
			g_npcState.Update(NPC_OPENSTORE, 0);
		}
		break;

	case NPC_STORE_OK:
		NotifyNpcSession(TRUE);
		break;

	default:
		break;
	}
}

// OnGamePacketAfterReceived
void Player_OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{
	g_inventory.OnGamePacketAfterReceived(aPacket, aLen);

	//////////////////////////////////////////////////////////
	// This Let Us Have a Player ID Before OnGameJoin is Called
	//////////////////////////////////////////////////////////
	if (!g_bGameJoined && !g_dwPlayerID && aLen == 26 && aPacket[0] == 0x59)
	{
		::memcpy(&g_dwPlayerID, aPacket + 1, 4);
		::memcpy(g_szPlayerName, aPacket + 6, 17);
	}

	//////////////////////////////////////////////////////////
	// Charge-able Spells 
	//////////////////////////////////////////////////////////
	if (aLen == 12 && aPacket[0] == 0x22)
	{
		CHARGEABLE ca;
		::memcpy(&ca.wSpellID, aPacket + 7, 2);
		ca.dwCharges = aPacket[9];

		int nIndex = g_aSkillCharges.Find(ca);
		if (nIndex == -1)
			g_aSkillCharges.Add(ca);
		else
			g_aSkillCharges[nIndex].dwCharges = aPacket[9];		

		NotifyThisPlayerMessage(PM_SPELLCHARGES, (WPARAM)ca.wSpellID, (LPARAM)aPacket[9]);
	}

	//////////////////////////////////////////////////////////
	// Stash Opened
	//////////////////////////////////////////////////////////
	if (aLen == 2 && aPacket[0] == 0x77 && aPacket[1] == 0x10)
	{		
		NotifyThisPlayerMessage(PM_STASHOPEN, (WPARAM)GetCurrentAct());
	}

	//////////////////////////////////////////////////////////
	// Waypoint Opened
	//////////////////////////////////////////////////////////
	if (aLen == 21 && aPacket[0] == 0x63)
	{
		g_bWpOpened = TRUE;
		NotifyThisPlayerMessage(PM_WPOPEN, (WPARAM)GetCurrentAct(), (LPARAM)GetCurrentMapID());
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Player's Corpse is Picked Up 
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 6 && aPacket[0] == 0x8e && aPacket[1] == 0x00 && IsThisPlayerID(aPacket + 2))
	{
		NotifyThisPlayerMessage(PM_CORPSEPICKEDUP);
	}	

	//////////////////////////////////////////////////////////
	// Player Character Level Changed
	//////////////////////////////////////////////////////////
	if (aLen == 3 && aPacket[0] == 0x1d && aPacket[1] == 0x0c)
	{
		NotifyThisPlayerMessage(PM_LEVEL, 0, GetStat(STAT_LEVEL));
	}

	//////////////////////////////////////////////////////////
	// Player HP/Mana Updates
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x95)
	{
		CheckPlayerHPMana(*((WORD *)(aPacket + 1)) & 0x0FFF, (*((WORD *)(aPacket + 3)) & 0x0FFF) << 1);
	}

	//////////////////////////////////////////////////////////
	// Player Death
	//////////////////////////////////////////////////////////
	if (aLen >= 6 && aPacket[0] == 0x8e && aPacket[1] == 0x01)
	{
		DWORD dwPlayerID;
		::memcpy(&dwPlayerID, aPacket + 2, 4);
		if (dwPlayerID == GetSafePlayerID())
		{
			if (!IsInTown())
			{
				// i'm dead
				NotifyThisPlayerMessage(PM_DEATH);
			}
		}
		else
		{
			// other player dead
			GAMEUNIT gu;
			gu.dwUnitID = dwPlayerID;
			gu.dwUnitType = UNIT_TYPE_PLAYER;
			NotifyUnitMessage(UM_DEATH, &gu);
		}
	}

	//////////////////////////////////////////////////////////
	// Party invitation/Joining
	//////////////////////////////////////////////////////////
	if (aLen == 6 && aPacket[0] == 0x8b)
	{
		DWORD dwPlayerID;
		::memcpy(&dwPlayerID, aPacket + 1, 4);

		if (aPacket[5] == 0x01)
		{
			// joined party			
			NotifyThisPlayerMessage(PM_JOINPARTY, dwPlayerID);
		}
		else if (aPacket[5] == 0x02)
		{
			// other player invited me to join his party
			NotifyThisPlayerMessage(PM_INVITATION, dwPlayerID);
		}		
	}

	//////////////////////////////////////////////////////////
	// Hostility/Unhostility
	//////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x8c && IsThisPlayerID(aPacket + 5))		
	{
		DWORD dwSrcID;
		::memcpy(&dwSrcID, aPacket + 1, 4);
		
		if (aPacket[9] == 0x08)
		{
			// Hostility
			if (g_aHostilers.Find(dwSrcID) == -1)
			{
				g_aHostilers.Add(dwSrcID);
				NotifyThisPlayerMessage(PM_HOSTILE, (WPARAM)dwSrcID, (LPARAM)g_aHostilers.GetSize());
			}
		}
		else if (aPacket[9] == 0x00)
		{
			// Unhostility
			int n = g_aHostilers.Find(dwSrcID);
			if (n != -1)
			{
				g_aHostilers.RemoveAt(n);
				NotifyThisPlayerMessage(PM_UNHOSTILE, (WPARAM)dwSrcID, (LPARAM)g_aHostilers.GetSize());
			}
		}
	}

	//////////////////////////////////////////////////////////
	// Other Player Quit
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x5c && aLen == 5)
	{
		DWORD dwQuitterID;
		::memcpy(&dwQuitterID, aPacket + 1, 4);
		
		int n = g_aHostilers.Find(dwQuitterID);
		if (n >= 0)
		{
			g_aHostilers.RemoveAt(n);		
			NotifyThisPlayerMessage(PM_UNHOSTILE, (WPARAM)dwQuitterID, (LPARAM)g_aHostilers.GetSize());
		}
	}
	
	//////////////////////////////////////////////////////////
	// Player Left/Right Spell Selected
	//////////////////////////////////////////////////////////
	if (aLen == 13 && aPacket[0] == 0x23 && aPacket[1] == 0x00 && IsThisPlayerID(aPacket + 2))
	{
		WORD wSpell;
		::memcpy(&wSpell, aPacket + 7, 2);

		if (aPacket[6])
		{
			// left-hand
			if (g_wLSpell != wSpell)
			{
				g_wLSpell = wSpell;
				NotifyThisPlayerMessage(PM_SPELLSELECT, aPacket[6], wSpell);
			}
		}
		else
		{
			// right-hand
			if (g_wRSpell != wSpell)
			{
				g_wRSpell = wSpell;
				NotifyThisPlayerMessage(PM_SPELLSELECT, aPacket[6], wSpell);
			}
		}		
	}

	//////////////////////////////////////////////////////////
	// Map Blink (Teleport, WP, TP, etc)
	//////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x15 && IsThisPlayerID(aPacket + 2))
	{
		WORD x, y;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		NotifyThisPlayerMessage(PM_MAPBLINK, (WPARAM)GetCurrentAct(), MAKELPARAM(x, y));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Player's Inventory is Full
	//////////////////////////////////////////////////////////////////////
	if (aLen == 8
		&& aPacket[0] == 0x2c
		&& aPacket[1] == 0x00
		&& aPacket[6] == 0x17
		&& aPacket[7] == 0x00
		 && IsThisPlayerID(aPacket + 2))
	{
		NotifyThisPlayerMessage(PM_INVENTORYFULL);
	}

	//////////////////////////////////////////////////////////////////////
	// Town Portal Appeared
	//////////////////////////////////////////////////////////////////////
	if (g_townState.GetState() == TS_WAIT_PORTAL
		&& aLen > 12
		&& aPacket[0] == 0x51
		&& aPacket[6] == 0x3b
		&& aPacket[7] == 0x00)
	{
		GAMEUNIT gu;
		::memcpy(&gu.dwUnitID, aPacket + 2, 4);
		gu.dwUnitType = UNIT_TYPE_OBJECT;
		UnitAny* p = D2CLIENT_GetUnit(gu.dwUnitID, UNIT_TYPE_OBJECT);
		if (p && !::stricmp(p->ptObjectData->parent, GetSafePlayerName()))
		{
			OnMyPortalAppeared(&gu); // it's my tp
		}
	}	
	
	//////////////////////////////////////////////////////////////////////
	// Weapon Switch
	//////////////////////////////////////////////////////////////////////
	if (aLen == 1 && aPacket[0] == 0x97)
	{
		NotifyThisPlayerMessage(PM_WEAPONSWITCH);
	}

	//////////////////////////////////////////////////////////
	// Item Equipped
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x9c || aPacket[0] == 0x9d)
	{
		if (aPacket[1] == ITEM_ACTION_TO_BELTSLOT
			|| aPacket[1] == ITEM_ACTION_SWITCH_BELTSLOT
			|| aPacket[1] == ITEM_ACTION_FROM_BELTSLOT
			|| aPacket[1] == ITEM_ACTION_SHIFT_BELTSLOT
			|| aPacket[1] == ITEM_ACTION_TO_EQUIP
			|| aPacket[1] == ITEM_ACTION_SWITCH_EQUIP
			|| aPacket[1] == ITEM_ACTION_FROM_EQUIP)
		{
			ITEM item = { 0 };
			if (::D2ParseItem(aPacket, aLen, &item))
			{
				AnalyzeItem(item);
				NotifyThisPlayerMessage(PM_EQUIPEVENT, aPacket[1], (LPARAM)&item);
			}
		}
	}
	
	//////////////////////////////////////////////////////////
	// Vendor Item Info
	//////////////////////////////////////////////////////////
	if (g_npcStore.npc.dwUnitID && aLen > 8 && aPacket[0] == 0x9c && aPacket[1] == 0x0b)
	{
		DWORD dwItemID;
		::memcpy(&dwItemID, aPacket + 4, 4);
		char szItemCode[4] = "";
		D2GetItemCode(aPacket, aLen, szItemCode);
		AnalyzeVendorItem(dwItemID, szItemCode);

		if (g_npcState.GetState() == NPC_WAIT_STORE || g_npcState.GetState() == NPC_STORE_OK)
		{
			g_npcState.Update(NPC_STORE_OK, 1000);
		}
	}
}

void NotifyUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{	
	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnUnitMessage)
			pCIS->OnUnitMessage(nMessage, lpUnit, wParam, lParam);
		//else
		//	GamePrintError("OnUnitMessage error!");

		li = ClientModules.GetNextItem(li);
	}
}

void NotifyThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{	
	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnThisPlayerMessage)
			pCIS->OnThisPlayerMessage(nMessage, wParam, lParam);
		//else
		//	GamePrintError("OnThisPlayerMessage error!");

		li = ClientModules.GetNextItem(li);
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

void CheckPlayerMap()
{
	if (g_iCurMapID == 0)
	{
		g_iCurMapID = GetCurrentMapID();
		return;
	}

	// do map comparing every 300 milliseconds
	if (GetInGameTime() % 300)
		return;

	BYTE iMap = GetCurrentMapID();
	if (iMap == 0 || iMap == g_iCurMapID)
		return;

	BYTE iOldMap = g_iCurMapID;
	g_iCurMapID = iMap;

	// was in a wp proc?
	if (g_iCurMapID == g_iWPdest && g_wpState.GetState() == WP_VERIFYDEST)
	{
		// the "EnterWaypoint" process succeeded
		CloseAllUIs();		
		g_wpState.Update(WP_FINISH, 1000);
	}

	// Map changed
	NotifyThisPlayerMessage(PM_MAPCHANGE, iOldMap, g_iCurMapID);

	// entered/left town?
	BOOL bWasTown = D2MapIsTown(iOldMap);
	BOOL bIsTown = D2MapIsTown(g_iCurMapID);
	if (bWasTown && !bIsTown)
	{
		// leave town
		NotifyThisPlayerMessage(PM_LEAVETOWN, (WPARAM)iOldMap, (LPARAM)g_iCurMapID);
	}
	else if (!bWasTown && bIsTown)
	{
		// enter town
		NotifyThisPlayerMessage(PM_ENTERTOWN, (WPARAM)iOldMap, (LPARAM)g_iCurMapID);
	}
	
	// act changed also?
	BYTE iOldAct = D2GetActFromMap(iOldMap);
	BYTE iNewAct = D2GetActFromMap(g_iCurMapID);

	if (iOldAct != iNewAct)
		NotifyThisPlayerMessage(PM_ACTCHANGE, iOldAct, iNewAct);		
}

BOOL EXPORT IsGameReady()
{
	return IsGameJoined()
		&& thisgame
		&& thisgame->player
		&& thisgame->player->PlayerPositionX
		&& thisgame->player->PlayerPositionY;
}

BOOL EXPORT HaveSpell(WORD wSpellID)
{
	// in case of id scroll and tp scroll, just check charged skills
	if (wSpellID == D2S_SCROLLOFIDENTIFY || wSpellID == D2S_SCROLLOFTOWNPORTAL)
		return GetSpellCharges(wSpellID) > 0;

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
	return thisgame->player ? thisgame->player->Act : D2GetActFromMap(g_iCurMapID);
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

BOOL IsGameJoined()
{
	return g_bGameJoined;
}

void CheckPlayerHPMana(WORD wNewHP, WORD wNewMana)
{
	WORD wOldHP = g_wPrevHP;
	WORD wOldMana = g_wPrevMana;

	// update the records
	g_wPrevHP = wNewHP;
	g_wPrevMana = wNewMana;

	if (wOldHP < wNewHP)
	{
		// hp up
		NotifyThisPlayerMessage(PM_HPUP, wOldHP, wNewHP);
	}
	else if (wOldHP > wNewHP)
	{
		// hp down
		NotifyThisPlayerMessage(PM_HPDOWN, wOldHP, wNewHP);
	}

	if (wOldMana < wNewMana)
	{
		// mana up
		NotifyThisPlayerMessage(PM_MANAUP, wOldMana, wNewMana);
	}
	else if (wOldMana > wNewMana)
	{
		// mana down
		NotifyThisPlayerMessage(PM_MANADOWN, wOldMana, wNewMana);
	}	
}

BOOL EXPORT IsInTown()
{
	return IsGameReady() ? D2MapIsTown(g_iCurMapID) : TRUE;
}

BOOL IsThisPlayerID(const BYTE* ptr)
{
	DWORD dwPlayerID = GetSafePlayerID();
	if (ptr == NULL || dwPlayerID == 0)
		return FALSE;

	return ::memcmp(&dwPlayerID, ptr, 4) == 0;
}

void AnalyzeItem(const ITEM &item)
{
	//////////////////////////////////////////////////////////////////////
	// Belt Items Added/Removed/Shifted
	//////////////////////////////////////////////////////////////////////
	if (item.iAction == ITEM_ACTION_TO_BELTSLOT
		|| item.iAction == ITEM_ACTION_SWITCH_BELTSLOT)
	{
		::memcpy(&g_aBeltItems[item.iAtBeltRow][item.iAtBeltColumn], &item, sizeof(ITEM));
	}

	if (item.iAction == ITEM_ACTION_FROM_BELTSLOT)
	{
		::memset(&g_aBeltItems[item.iAtBeltRow][item.iAtBeltColumn], 0, sizeof(ITEM));
	}

	if (item.iAction == ITEM_ACTION_SHIFT_BELTSLOT)
	{
		::memcpy(&g_aBeltItems[item.iAtBeltRow][item.iAtBeltColumn], &item, sizeof(ITEM));
		for (BYTE row = item.iAtBeltRow + 1; row < 4; row++)
			::memset(&g_aBeltItems[row][item.iAtBeltColumn], 0, sizeof(ITEM));
	}

	//////////////////////////////////////////////////////////////////////
	// Player Belt Equipped/Unequipped
	//////////////////////////////////////////////////////////////////////

	if (item.iAction == ITEM_ACTION_TO_EQUIP || item.iAction == ITEM_ACTION_SWITCH_EQUIP)
	{
		// is belt?
		BYTE iBeltRows = ::D2GetBeltRows(item.szItemCode);
		g_iBeltRows = max(1, iBeltRows);
	}

	if (item.iAction == ITEM_ACTION_FROM_EQUIP)
	{
		// is belt?
		if (::D2GetBeltRows(item.szItemCode))
			g_iBeltRows = 1;
	}

	g_inventory.CheckEquip(item);
}

int EXPORT GetMode()	// Get player's current mode(walking/running/attacking etc)
{
	UnitAny* p = D2CLIENT_GetPlayerUnit();
	return p ? (int)p->dwMode : 0;
}

WORD EXPORT GetSelectedSpell(BOOL bLeft) // Get the selected spell
{
	return bLeft ? g_wLSpell : g_wRSpell;
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
	GAMEUNIT gu;
	MakePlayerData(&gu);

	DWORD dwValue = GetUnitStat(&gu, nStatID);
	if (nStatID == STAT_HP || nStatID == STAT_MAXHP
		|| nStatID == STAT_MANA || nStatID == STAT_MAXMANA
		|| nStatID == STAT_STAMINA || nStatID == STAT_MAXSTAMINA)
		dwValue /= 256;
	return dwValue;
}

BOOL EXPORT GetAffection(int nAffectionID)
{
	GAMEUNIT gu;
	MakePlayerData(&gu);
	return GetUnitAffection(&gu, nAffectionID);
}

BOOL EXPORT SelectSpell(WORD wSpell, BOOL bLeftHand)
{
	if (!HaveSpell(wSpell) || !D2GetSpellInfo(wSpell, &g_dsi))
		return FALSE;

	if ((bLeftHand && wSpell == g_wLSpell) || (!bLeftHand && wSpell == g_wRSpell))
		return TRUE;

	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x3c;
	memcpy(aPacket + 1, &wSpell, 2);
	aPacket[4] = bLeftHand ? 0x80 : 0x00;
	aPacket[5] = aPacket[6] =aPacket[7] = aPacket[8] = 0xff;

	return GameSendPacketToServer(aPacket, 9);
}

MAPPOS EXPORT GetPosition() // player position
{
	GAMEUNIT gu;
	MakePlayerData(&gu);
	return GetUnitPosition(&gu);
}

DWORD EXPORT GetDistanceFrom(WORD x, WORD y) // Player's distance from a position
{
	MAPPOS mp = GetPosition();
	return GetDistance(mp.x, mp.y, x, y);
}

BOOL EXPORT Interact(LPCGAMEUNIT lpUnit)
{
	if (!VerifyUnit(lpUnit))
		return FALSE;

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

BOOL GoToEntity(LPCGAMEUNIT lpUnit, BOOL bShowClientMove)
{
	if (!VerifyUnit(lpUnit))
		return FALSE;

	MAPPOS mp = GetUnitPosition(lpUnit);

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
		::memcpy(aPacket + 12, &mp.x, 2);
		::memcpy(aPacket + 14, &mp.y, 2);
		GameSendPacketToGame(aPacket, 16);
	}

	return TRUE;		
}

int EXPORT GetHostilerCount() // number of players that are hostiled to me
{
	return g_aHostilers.GetSize();
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

BYTE EXPORT GetBeltRows() // Row number of the player's belt(1-4)
{
	return max(1, g_iBeltRows);
}

BOOL PrepareInteractWithNPC(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return FALSE;

	MAPPOS mp = GetUnitPosition(lpUnit);
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x59;
	aPacket[1] = 0x01;
	::memcpy(aPacket + 5, &lpUnit->dwUnitID, 4);
	::memcpy(aPacket + 9, &mp.x, 2);
	::memcpy(aPacket + 13, &mp.y, 2);
	return GameSendPacketToServer(aPacket, 17);
}

BOOL GoToPosition(WORD x, WORD y, BOOL bShowClientMove)
{
	if (GetDistanceFrom(x, y) > 99)
		return FALSE;

	BYTE aPacket[16] = { 0 };
	aPacket[0] = GetAlwaysRun() ? 0x03 : 0x01;
	::memcpy(&aPacket[1], &x, 2);
	::memcpy(&aPacket[3], &y, 2);

	if (!GameSendPacketToServer(aPacket, 5))
		return FALSE;

	if (bShowClientMove && IsGameReady())
	{
		DWORD dwPlayerID = GetSafePlayerID();
		MAPPOS mp = GetPosition();

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
	MAPPOS mp = GetPosition();
	return CastOnMap(wSpell, mp.x, mp.y, bLeftHand);
}

BOOL EXPORT CastOnMap(WORD wSpell, WORD x, WORD y, BOOL bLeftHand)
{
	if (GetDistanceFrom(x, y) > 99)
		return FALSE;
	
	if (!SelectSpell(wSpell, bLeftHand))
		return FALSE;
	
	BYTE aPacket[17] = { 0 };
	aPacket[0] = bLeftHand ? 0x05 : 0x0c;
	memcpy(aPacket + 1, &x, 2);
	memcpy(aPacket + 3, &y, 2);	

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
		::memcpy(aPacket + 11, &x, 2);
		::memcpy(aPacket + 13, &y, 2);
		GameSendPacketToGame(aPacket, 17);
	}

	return TRUE;	
}

BOOL EXPORT CastOnUnit(WORD wSpell, LPCGAMEUNIT lpUnit, BOOL bLeftHand)
{
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return FALSE;

	if (!SelectSpell(wSpell, bLeftHand))
		return FALSE;

	MAPPOS mp = GetUnitPosition(lpUnit);
	
	if (wSpell == g_dsi.wSpellID
		&& GetDistanceFrom(mp.x, mp.y) > 5
		&& (g_dsi.dwSpellInfoFlag & DSI_MELEE))
	{
		// cast a melee-only spell on an entity requires approaching the entity first!
		GoToEntity(lpUnit);
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

LPCITEM FindFirstBeltItem(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return NULL;

	for (BYTE row = 0; row < g_iBeltRows; row++)
	{
		for (BYTE col = 0; col < 4; col++)
		{
			if (stricmp(lpszItemCode, g_aBeltItems[row][col].szItemCode) == 0)
				return &g_aBeltItems[row][col];
		}
	}

	return NULL;
}

BOOL EXPORT IsBeltFull()
{
	for (BYTE row = 0; row < g_iBeltRows; row++)
	{
		for (BYTE col = 0; col < 4; col++)
		{
			if (g_aBeltItems[row][col].dwItemID == 0)
				return FALSE;
		}
	}

	return TRUE;
}

LPCITEM EXPORT GetBeltItem(int x, int y) // Belt item access
{
	y = (int)g_iBeltRows - y - 1;
	if (x < 0 || x > 3 || y < 0 || y >= (int)g_iBeltRows)
		return NULL;
	
	return g_aBeltItems[y][x].dwItemID ? &g_aBeltItems[y][x] : NULL;
}

BOOL IsBeltColFull(int nCol)
{
	for (BYTE row = 0; row < g_iBeltRows; row++)
	{
		for (BYTE col = 0; col < 4; col++)
		{
			if (g_aBeltItems[row][col].dwItemID == 0)
				return FALSE;
		}
	}

	return TRUE;
}

/*
LPCITEM FindBluePotion()
{
	LPCITEM p = FindFirstBeltItem("mp5");

	if (!p)
		p = FindFirstBeltItem("mp4");

	if (!p)
		p = FindFirstBeltItem("mp3");

	if (!p)
		p = FindFirstBeltItem("mp2");

	if (!p)
		p = FindFirstBeltItem("mp1");

	return p;
}

LPCITEM FindPurplePotion()
{
	LPCITEM p = FindFirstBeltItem("rvl");
	if (!p)
		p = FindFirstBeltItem("rvs");
	return p;
}

LPCITEM FindRedPotion()
{
	LPCITEM p = FindFirstBeltItem("hp5");

	if (!p)
		p = FindFirstBeltItem("hp4");

	if (!p)
		p = FindFirstBeltItem("hp3");

	if (!p)
		p = FindFirstBeltItem("hp2");

	if (!p)
		p = FindFirstBeltItem("hp1");

	return p;
}
*/

BOOL EXPORT DrinkBeltPotion(DWORD dwItemID, BOOL bFeedMerc)
{
	if (dwItemID == 0)
		return FALSE;

	BYTE aPacket[13] = { 0x26 };
	memcpy(aPacket + 1, &dwItemID, 4);
	aPacket[5] = bFeedMerc ? 0x01 : 0x00;
	memset(aPacket + 6, 0x00, 7);
	return GameSendPacketToServer(aPacket, 13);
}

BOOL EXPORT DrinkInventoryPotion(DWORD dwItemID)
{
	if (dwItemID == 0)
		return FALSE;

	MAPPOS mp = GetPosition();
	BYTE aPacket[13] = {0};
	aPacket[0] = 0x20;
	::memcpy(aPacket + 1, &dwItemID, 4);
	::memcpy(aPacket + 5, &mp.x, 2);
	::memcpy(aPacket + 9, &mp.y, 2);
	return GameSendPacketToServer(aPacket, 13);
}

DWORD EXPORT HealMe() // Drink a purple/red potion
{
	DWORD dwClassID = 0;

	DWORD dwItemID = FindFirstInventoryPotion(POTION_PURPLE, dwClassID);
	if (dwItemID)
		return DrinkInventoryPotion(dwItemID) ? dwClassID : 0;

	dwItemID = FindFirstBeltPotion(POTION_PURPLE, dwClassID);
	if (dwItemID)
		return DrinkBeltPotion(dwItemID, FALSE) ? dwClassID : 0;

	dwItemID = FindFirstInventoryPotion(POTION_RED, dwClassID);
	if (dwItemID)
		return DrinkInventoryPotion(dwItemID) ? dwClassID : 0;
	
	dwItemID = FindFirstBeltPotion(POTION_RED, dwClassID);
	if (dwItemID)
		return DrinkBeltPotion(dwItemID, FALSE) ? dwClassID : 0;
	
	return FALSE;
}

DWORD EXPORT HealMerc() // Feed the merc a purple/red potion
{
	DWORD dwClassID = 0;
	DWORD dwItemID = FindFirstBeltPotion(POTION_PURPLE, dwClassID);
	if (dwItemID == 0)
		dwItemID = FindFirstBeltPotion(POTION_RED, dwClassID);

	return DrinkBeltPotion(dwItemID, TRUE) ? dwClassID : 0;
}

DWORD EXPORT SupplyMana(BOOL bDrinkPurpleIfNoBlue) // Drink a blue/purple potion for mana
{
	DWORD dwClassID = 0;

	DWORD dwItemID = FindFirstInventoryPotion(POTION_BLUE, dwClassID);
	if (dwItemID)
		return DrinkInventoryPotion(dwItemID) ? dwClassID : 0;

	dwItemID = FindFirstBeltPotion(POTION_BLUE, dwClassID);
	if (dwItemID)
		return DrinkBeltPotion(dwItemID, FALSE) ? dwClassID : 0;

	if (!bDrinkPurpleIfNoBlue)
		return FALSE;

	dwItemID = FindFirstInventoryPotion(POTION_PURPLE, dwClassID);
	if (dwItemID)
		return DrinkInventoryPotion(dwItemID) ? dwClassID : 0;
	
	dwItemID = FindFirstBeltPotion(POTION_PURPLE, dwClassID);
	if (dwItemID)
		return DrinkBeltPotion(dwItemID, FALSE) ? dwClassID : 0;
	
	return FALSE;
}

BOOL EXPORT BackToTown() // Cast a town portal and go back to town immediately
{
	if (IsInTown())
	{
		GamePrintInfo("You are in town.");
		return TRUE;
	}

	if (g_townState.GetState() != TS_NONE)
	{
		GamePrintError("Already in a portal Process");
		return FALSE; // already in middle of a tp process
	}

	CleanJobs(); // Action "Back to town" has higher priority than any other jobs
	g_nTPFail = 0;
	g_townState.Update(TS_CAST_PORTAL, 0);
	return TRUE;
}

BOOL EXPORT MoveTo(WORD x, WORD y, BOOL bQueueJob) // Run to a position
{
	// We do not drive the player to run immediately, instead, we 
	// put the destination positions in to a FIFO queue and run to
	// those positions at each OnGameTimerTick call.
	if (!bQueueJob)
		CleanJobs();	

	// queue this new job
	MOTIONJOB job = { 0 };
	job.nJobType = MJ_POSITION;
	job.x = x;
	job.y = y;
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

BOOL EXPORT TeleportTo(WORD x, WORD y, BOOL bQueueJob) // teleport to a position
{
	if (!HaveSpell(D2S_TELEPORT))
		return FALSE;

	if (!bQueueJob)
		CleanJobs();

	// queue this new job
	MOTIONJOB job = { 0 };
	job.nJobType = MJ_TELEPORT;
	job.x = x;
	job.y = y;
	AddMotionJob(job);
	return TRUE;
}

void OnMyPortalAppeared(LPCGAMEUNIT lpUnit)
{
	if (lpUnit && g_townState.GetState() == TS_WAIT_PORTAL)
	{
		::memcpy(&g_myPortal, lpUnit, sizeof(GAMEUNIT));
		g_nTPFail = 0;
		g_townState.Update(TS_VERIFY_PORTAL, 0);
	}
}

void DoTPProc()
{
	if (!g_townState.IsTickEnded())
		return;

	if (IsInTown())
	{
		// already in town
		g_townState.Clear();
		return;
	}

	WORD wSpell = D2S_TOMEOFTOWNPORTAL;
	UnitAny* pUnit = NULL;

	switch (g_townState.GetState())
	{
	case TS_CAST_PORTAL:

		// cast it
		wSpell = GetSpellCharges(D2S_SCROLLOFTOWNPORTAL) ? D2S_SCROLLOFTOWNPORTAL : D2S_TOMEOFTOWNPORTAL;
		::memset(&g_myPortal, 0, sizeof(GAMEUNIT));
		CastNoTarget(wSpell, FALSE);
		g_townState.Update(TS_WAIT_PORTAL, TP_CAST_TICK);		
		break;

	case TS_WAIT_PORTAL:

		// portal didn't appear
		if (g_nTPFail >= MAX_RETRY)
		{
			// maximum retries reached and portal still did not appear,
			// assume char was already in town, or ran out of TP scrolls,
			// in either case, portal will never appear.
			g_townState.Clear();
			GamePrintError("Failed to cast town portal. Out of TP scrolls?");
			NotifyThisPlayerMessage(PM_TOWNFAIL);
		}
		else
		{
			// retry and prompt every seconds
			g_nTPFail++;
			GamePrintInfo("Trying to cast town portal...");
			g_townState.Update(TS_CAST_PORTAL, 0);
		}
		break;

	case TS_VERIFY_PORTAL:
		pUnit = (UnitAny*)VerifyUnit(&g_myPortal);
		if (pUnit && pUnit->dwMode == 2) // mode 2 indicates portal clickable (Thanks Ninjai)
			g_townState.Update(TS_ENTER_PORTAL, 0);
		break;

	case TS_ENTER_PORTAL:
		// Walk in.
		Interact(&g_myPortal);
		g_townState.Update(TS_VERIFY_INTOWN, TP_ENTER_TICK);
		break;

	case TS_VERIFY_INTOWN:
		if (g_nTPFail >= MAX_RETRY * 2)
		{
			GamePrintError("Failed to enter TP");
			g_townState.Clear();
			NotifyThisPlayerMessage(PM_TOWNFAIL);
		}
		else
		{
			g_nTPFail++;
			g_townState.Update(TS_ENTER_PORTAL, 0);
			if (g_nTPFail % 2)
				GamePrintInfo("Trying to enter town portal...");
		}

		break;

	default:
		break;
	}
}

void NotifyNpcSession(BOOL bSuccessful)
{
	g_npcState.Clear();
	DWORD NPCID = g_npcStore.npc.dwUnitID;	
	g_nNpcFail = 0;
	g_npcStore.bSessionStarted = bSuccessful;
	if (!bSuccessful)
		::memset(&g_npcStore, 0, sizeof(NPCSTORE));
	NotifyThisPlayerMessage(PM_NPCSESSION, (WPARAM)bSuccessful, (LPARAM)NPCID);
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

BOOL EXPORT EnterWaypoint(LPCGAMEUNIT lpWpUnit, BYTE iDestinationMapID) // take WP to another area
{
	if (lpWpUnit == NULL
		|| GetCurrentMapID() == iDestinationMapID
		|| !VerifyUnit(lpWpUnit))
		return FALSE;

	WPINFO wi = {0};
	if (!D2GetWPInfo(iDestinationMapID, &wi))
		return FALSE;

	MAPPOS mp = GetUnitPosition(lpWpUnit);
	DWORD dwDistance = GetDistanceFrom(mp.x, mp.y);
	if (dwDistance > 50)
		return FALSE;	
	
	g_nWPRetries = 0;
	g_iWPdest = iDestinationMapID;
	::memcpy(&g_waypoint, lpWpUnit, sizeof(GAMEUNIT));

	if (dwDistance > 5)
	{
		MoveToUnit(lpWpUnit, FALSE);
		g_wpState.Update(WP_INTERACT, dwDistance * 50);
	}
	else
	{
		g_wpState.Update(WP_INTERACT, 0);
	}
	return TRUE;
}

BOOL EXPORT PickGroundItem(DWORD dwItemID, BOOL bShowClientMove) // Pick up a ground item
{
	if (!VerifyItem(dwItemID))
		return FALSE;

	GAMEUNIT gu;
	gu.dwUnitID = dwItemID;
	gu.dwUnitType = UNIT_TYPE_ITEM;
	MAPPOS mp = GetUnitPosition(&gu);

	if (GetDistanceFrom(mp.x, mp.y) > 5)
		GoToEntity(&gu, bShowClientMove);

	BYTE aPacket[13] = { 0 };
	aPacket[0] = 0x16;
	aPacket[1] = 0x04;
	::memcpy(aPacket + 5, &dwItemID, 4);

	return GameSendPacketToServer(aPacket, 13);
}

BOOL EXPORT ResurrectMerc() // Resurrect merc
{
	// must be a resurrector!
	if (!IsResurrector(GetUnitClassID(&g_npcStore.npc)))
		return FALSE;
	
	if (!IsNpcActive() || IsMercAlive()) // do not resurrect if the merc is alive
		return FALSE;

	BYTE aPacket[5] = { 0 };
	aPacket[0] = 0x62;
	::memcpy(aPacket + 1, &g_npcStore.npc.dwUnitID, 4);
	return GameSendPacketToServer(aPacket, 5);
}

BOOL OpenNpcStore(LPCGAMEUNIT lpNpcUnit, BOOL bGamble) // Open an npc's store
{
	if (!VerifyUnit(lpNpcUnit))
		return FALSE;

	// verify npc capability
	DWORD dwClassID = GetUnitClassID(&g_npcStore.npc);
	int nMenuID = -1;
	if (bGamble)
		nMenuID = GetNpcGambleMenuID(dwClassID);
	else
		nMenuID = GetNpcTradeMenuID(dwClassID);
	if (nMenuID == -1)
		return FALSE;

	nMenuID++;

	// D2BS code for clicking npc menus
	D2MSG MouseXY = {0};
	d2_NpcMenu *NpcMenu = &(**p_D2CLIENT_NpcMenu);
	if (!NpcMenu)
		return FALSE;
	
	MouseXY.pt.X=(WORD)NpcMenu->X +32;
	MouseXY.pt.Y=(WORD)NpcMenu->Y +15 +15*(WORD)nMenuID;
	NpcMenu->clicked=1;
	NpcMenu->MenuId1=nMenuID;
	NpcMenu->MenuId2=nMenuID;
	D2CLIENT_ClickNpcMenuUI((DWORD)FindMyD2WndHandle(),(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
	return TRUE;
		/*
	BYTE aPacket[13] = { 0 };
	aPacket[0] = 0x38;
	aPacket[1] = bGamble ? 0x02 : 0x01;
	::memcpy(aPacket + 5, &lpNpcUnit->dwUnitID, 4);
	return GameSendPacketToServer(aPacket, 13);
	*/
}

BOOL EXPORT BuyItem(DWORD dwItemID) // Buy an item from npc
{
	if (!IsNpcActive(NPC_TRADE) || !VerifyItem(dwItemID))
		return FALSE;

	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x32;
	::memcpy(&aPacket[1], &g_npcStore.npc.dwUnitID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	return GameSendPacketToServer(aPacket, 17);
}

BOOL EXPORT Gamble(DWORD dwItemID) // gamble an item
{
	if (!IsNpcActive(NPC_GAMBLE) || !VerifyItem(dwItemID))
		return FALSE;

	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x32;
	::memcpy(&aPacket[1], &g_npcStore.npc.dwUnitID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	aPacket[9] = 0x02;
	return GameSendPacketToServer(aPacket, 17);
}

BOOL VerifyItem(DWORD dwItemID)
{
	GAMEUNIT gu;
	gu.dwUnitID = dwItemID;
	gu.dwUnitType = UNIT_TYPE_ITEM;
	UnitAny* p = (UnitAny*)VerifyUnit(&gu);
	if (p == NULL)
		return FALSE;

	return (BOOL)D2COMMON_IsItem(p);
}

BOOL EXPORT RepairItem(DWORD dwItemID) // Repair items
{
	if (!IsNpcActive(NPC_TRADE))
		return FALSE;

	if (dwItemID && !VerifyItem(dwItemID))
		return FALSE;	

	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x35;
	::memcpy(aPacket + 1, &g_npcStore.npc.dwUnitID, 4);

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

	BYTE aPacket[5] = { 0 };
	aPacket[0] = 0x34;
	::memcpy(aPacket + 1, &g_npcStore.npc.dwUnitID, 4);
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
	MAPPOS mp = GetPosition();

	BYTE aPacket[11] = { 0 };
	aPacket[0] = 0x15;
	::memcpy(aPacket + 2, &dwPlayerID, 4);
	::memcpy(aPacket + 6, &mp.x, 2);
	::memcpy(aPacket + 8, &mp.y, 2);

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

DWORD FindFirstBeltPotion(int nType, DWORD& dwClassID)
{
	dwClassID = 0;

	if (nType < 0 || nType > POTION_PURPLE)
		return 0;

	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (!pUnit)
		return 0;

	Inventory* pInv = pUnit->ptInventory;

	if (!pInv)
		return 0;

	for (UnitItem* p = pInv->pFirstItem; p && p->ptItemData; p = p->ptItemData->ptNextInvenItem)
	{
		if (D2COMMON_GetItemNodePage(p) == 2)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (D2IsPotion(szCode) == nType)
			{
				dwClassID = p->dwClassId;
				return p->dwId;
			}
		}	
	}

	return 0;
}

DWORD FindFirstInventoryPotion(int nType, DWORD& dwClassID)
{
	dwClassID = 0;

	if (nType <= 0 || nType > POTION_JUNK)
		return 0;

	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (!pUnit)
		return 0;

	Inventory* pInv = pUnit->ptInventory;

	if (!pInv)
		return 0;

	for (UnitItem* p = pInv->pFirstItem; p && p->ptItemData; p = p->ptItemData->ptNextInvenItem)
	{
		if (D2COMMON_GetItemNodePage(p) == 1)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (D2IsPotion(szCode) == nType)
			{
				dwClassID = p->dwClassId;
				return p->dwId;
			}
		}
	}

	return 0;
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

BOOL EXPORT PickStorageItemToCursor(DWORD dwItemID)
{
	if (g_inventory.FindStorageItem(dwItemID) == STORAGE_NONE)
		return FALSE;

	BYTE aPacket[5]= { 0 };
	aPacket[0] = 0x19;
	::memcpy(aPacket + 1, &dwItemID, 4);
	return GameSendPacketToServer(aPacket, 5);
}

BOOL EXPORT FindFirstStorageSpace(int nStorageType, SIZE minSize, LPPOINT lpBuffer)
{
	BOOL bResult = FALSE;
	switch (nStorageType)
	{
	case STORAGE_INVENTORY:
		bResult = g_inventory.FindInventoryPosition(minSize.cx, minSize.cy, lpBuffer);
		break;

	case STORAGE_STASH:
		bResult = g_inventory.FindStashPosition(minSize.cx, minSize.cy, lpBuffer);
		break;

	case STORAGE_CUBE:
		bResult = g_inventory.FindCubePosition(minSize.cx, minSize.cy, lpBuffer);
		break;
		
	default:
		break;
	}
	return bResult;
}

BOOL EXPORT DropCursorItemToStorage(int nStorageType)
{
	DWORD dwItemID = GetCursorItem();
	if (dwItemID == 0)
		return FALSE;

	char szCode[4] = "";
	if (!GetItemCode(dwItemID, szCode, 3))
		return FALSE;

	SIZE cs = GetItemSize(szCode);
	if (cs.cx == 0 || cs.cy == 0)
		return FALSE;

	POINT pt;

	if (!FindFirstStorageSpace(nStorageType, cs, &pt))
		return FALSE; // no space

	DWORD dwCubeID = g_inventory.GetCubeID();
	if (nStorageType == STORAGE_CUBE)
	{
		if (dwCubeID == 0)
			return FALSE;

		BYTE aPacket[9] = { 0 };
		aPacket[0] = 0x2a;
		::memcpy(aPacket + 1, &dwItemID, 4);
		::memcpy(aPacket + 5, &dwCubeID, 4);
		return GameSendPacketToServer(aPacket, 9);
	}

	if (nStorageType == STORAGE_INVENTORY || nStorageType == STORAGE_STASH)
	{
		BYTE aPacket[17] = { 0 };
		aPacket[0] = 0x18;
		::memcpy(aPacket + 1, &dwItemID, 4);
		::memcpy(aPacket + 5, &pt.x, 4);
		::memcpy(aPacket + 9, &pt.y, 4);
		aPacket[13] = nStorageType == STORAGE_INVENTORY ? 0x00 : 0x04;
		return GameSendPacketToServer(aPacket, 17);
	}

	return FALSE;	
}

BOOL EXPORT EnumStorageItems(int nSTorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam)
{
	return g_inventory.EnumStorageItems(nSTorageType, lpfnEnumItemProc, lParam);
}

/*
int GetWpButtonCoords(BYTE iMapID, POINT& rCorrds)
{
	WPINFO wi = {0};
	if (!D2GetWPInfo(iMapID, &wi))
		return 0;

	BOOL bRes800 = GetScreenSize().cx == 800;
	rCorrds.x = bRes800 ? 210 : 110;
	rCorrds.y = bRes800 ? 140 : 80;
	rCorrds.y += 35 * wi.nPos;
	return wi.nPos + 1;
}

BOOL GetWPPageCoords(BYTE iMapID, POINT& rCorrds)
{
	WPINFO wi = {0};
	if (!D2GetWPInfo(iMapID, &wi))
		return FALSE;

	BOOL bRes800 = GetScreenSize().cx == 800;
	rCorrds.x = bRes800 ? 110 : 40;
	rCorrds.x += 60 * wi.iAct;
	rCorrds.y = bRes800 ? 80 : 20;
	return TRUE;
}

BOOL ClickWPTab(BYTE iDestMap)
{
	POINT pt;
	if (!GetWPPageCoords(iDestMap, pt))
		return FALSE;

	D2MSG MouseXY;
	MouseXY.pt.X = pt.x;
	MouseXY.pt.Y = pt.y;
	D2CLIENT_ClickWaypointPageUI((DWORD)FindMyD2WndHandle(),(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
	return TRUE;
}

BOOL ClickWpButton(BYTE iDestMap)
{
	POINT pt;
	int nIndex = GetWpButtonCoords(iDestMap, pt);
	if (nIndex < 1 || nIndex > 5)
		return FALSE;

	D2MSG MouseXY;
	MouseXY.pt.X = pt.x;
	MouseXY.pt.Y = pt.y;
	*p_D2CLIENT_ClickWpId = nIndex - 1;
	D2CLIENT_ClickWaypointUI((DWORD)FindMyD2WndHandle(),(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
	return TRUE;	
}
*/

void DoWPProc()
{
	if (!g_wpState.IsTickEnded())
		return;

	switch (g_wpState.GetState())
	{
	case WP_INTERACT:
		if (g_bWpOpened)
		{
			g_wpState.Update(WP_ENTER, 0);
			break;
		}

		if (!VerifyUnit(&g_waypoint))
		{
			FinishWPProc(FALSE);
		}
		else
		{
			Interact(&g_waypoint);
			g_wpState.Update(WP_WAITUI, 1000);	
		}
		break;

	case WP_WAITUI:
		if (g_bWpOpened)
		{
			g_wpState.Update(WP_ENTER, 0);
			/*
			if (GetActFromMap(g_iWPdest) != GetActFromMap(g_iCurMap))
				g_nWPState = WP_CLICKTAB;
			else
				g_nWPState = WP_CLICKBTN;
				*/
		}
		else
		{
			g_nWPRetries++;
			if (g_nWPRetries > MAX_RETRY)
				FinishWPProc(FALSE);
			else
				g_wpState.Update(WP_INTERACT, 0);
		}
		
		break;
	
	case WP_ENTER:
		if (g_bWpOpened)
		{
			BYTE aPacket[9] = { 0 };
			aPacket[0] = 0x49;
			::memcpy(&aPacket[1], &g_waypoint.dwUnitID, 4);
			aPacket[5] = g_iWPdest;
			GameSendPacketToServer(aPacket, 9);
			g_nWPRetries = 0;
			g_wpState.Update(WP_VERIFYDEST, 10000);
		}
		break;

		/*
	case WP_CLICKTAB:
		if (!IsUIOpened(UI_WPMENU))
		{
			g_nWPState = WP_INTERACT;
			break;
		}

		if (!ClickWPTab(g_iWPdest))
		{
			FinishWPProc(FALSE);
		}
		else
		{
			g_ticker.Update(500);
			g_nWPState = WP_CLICKBTN;
			g_nWPRetries = 0;
		}
		break;

	case WP_CLICKBTN:
		if (!IsUIOpened(UI_WPMENU))
		{
			g_nWPState = WP_INTERACT;
			break;
		}
		
		if (!ClickWpButton(g_iWPdest))
		{
			FinishWPProc(FALSE);
		}
		else
		{	
			g_nWPState = WP_VERIFYDEST;
			g_nWPRetries = 0;
			g_ticker.Update(2000);
		}
		break;
		*/

	case WP_VERIFYDEST:
		FinishWPProc(FALSE);
		/*
		else
		{
			g_nWPRetries++;
			if (g_nWPRetries > MAX_RETRY)
			{
				FinishWPProc(FALSE);
			}
			else
			{
				if (!IsUIOpened(UI_WPMENU))
				{
					g_nWPState = WP_INTERACT;
					break;
				}
		
				if (!ClickWpButton(g_iWPdest))
				{
					FinishWPProc(FALSE);
				}
				else
				{		
					g_ticker.Update(2000);
				}
			}
		}
		*/
		
		break;

	case WP_FINISH:
		FinishWPProc(TRUE);
		break;

	default:
		break;
	}
}

void FinishWPProc(BOOL bSucceeded)
{
	g_waypoint.dwUnitID = 0;
	g_iWPdest = 0;
	g_wpState.Clear();
	g_nWPRetries = 0;

	if (!bSucceeded)
		NotifyThisPlayerMessage(PM_WPFAIL);
}

void AnalyzeVendorItem(DWORD dwItemID, LPCSTR lpszItemCode)
{
	if (dwItemID == 0 || lpszItemCode == NULL)
		return; // invalid item

	if (g_npcStore.nInteractType == NPC_TRADE)
	{
		// healing potion "hp1/2/3/4/5"
		if (lpszItemCode[0] == 'h' && lpszItemCode[1] == 'p')
		{
			if (lpszItemCode[2] > g_npcStore.szHealingPotionCode[2])
			{
				g_npcStore.dwHealingPotionID = dwItemID;
				strcpy(g_npcStore.szHealingPotionCode, lpszItemCode);
			}
		}	

		// mana potion "mp1/2/3/4/5"
		else if (lpszItemCode[0] == 'm' && lpszItemCode[1] == 'p')
		{		
			if (lpszItemCode[2] > g_npcStore.szManaPotionCode[2])
			{
				g_npcStore.dwManaPotionID = dwItemID;
				strcpy(g_npcStore.szManaPotionCode, lpszItemCode);
			}
		}

		// antidote potion
		else if (!stricmp(lpszItemCode, "yps"))
		{
			g_npcStore.dwAntidotePotionID = dwItemID;
		}

		// town scroll
		else if (!stricmp(lpszItemCode, "tsc"))
		{
			g_npcStore.dwTownScrollID = dwItemID;
		}

		// identify scroll
		else if (!stricmp(lpszItemCode, "isc"))
		{
			g_npcStore.dwIdentifyScrollID = dwItemID;
		}

		// arrow
		else if (!stricmp(lpszItemCode, "aqv"))
		{
			g_npcStore.dwArrowID = dwItemID;
		}

		// bolt
		else if (!stricmp(lpszItemCode, "cqv"))
		{
			g_npcStore.dwBoltID = dwItemID;
		}
	}
	else if (g_npcStore.nInteractType == NPC_GAMBLE)
	{
		// amulet
		if (!stricmp(lpszItemCode, "amu"))
		{
			InsertGambleItem(dwItemID, g_npcStore.aAmulets, 5);
		}

		// ring
		else if (!stricmp(lpszItemCode, "rin"))
		{
			InsertGambleItem(dwItemID, g_npcStore.aRings, 5);
		}

		// circlets
		else if (D2IsCirclets(lpszItemCode))
		{
			InsertGambleItem(dwItemID, g_npcStore.aCirclets, 5);
		}

		// gloves
		else if (D2IsGloves(lpszItemCode))
		{
			InsertGambleItem(dwItemID, g_npcStore.aGloves, 5);
		}

		// blets (4-row belts only)
		else if (D2GetBeltRows(lpszItemCode) == 4)
		{
			InsertGambleItem(dwItemID, g_npcStore.aBelts, 5);
		}

		// boots
		else if (D2IsBoots(lpszItemCode))
		{
			InsertGambleItem(dwItemID, g_npcStore.aBoots, 5);
		}
	}	
}

BOOL BuyMiscItem(DWORD dwItemID, LPCGAMEUNIT lpNpcUnit, int nQuantity)
{
	if (!dwItemID || !lpNpcUnit || !VerifyUnit(lpNpcUnit))
		return FALSE;

	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x32;
	::memcpy(&aPacket[1], &lpNpcUnit->dwUnitID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	
	if (nQuantity <= 0)
	{
		aPacket[12] = 0x80;
		return GameSendPacketToServer(aPacket, 17);
	}
	
	for (int i = 0; i < nQuantity && i < 16; i++)
	{
		if (!GameSendPacketToServer(aPacket, 17))
			return FALSE;
	}

	return TRUE;
}

BOOL EXPORT BuyHealingPotion(int nQuantity)
{
	if (!g_npcStore.dwHealingPotionID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No healing potion found!");
		return FALSE;	
	}	

	return BuyMiscItem(g_npcStore.dwHealingPotionID, &g_npcStore.npc, nQuantity);
}

BOOL EXPORT BuyManaPotion(int nQuantity)
{
	if (!g_npcStore.dwManaPotionID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No mana potion found!");
		return FALSE;	
	}

	return BuyMiscItem(g_npcStore.dwManaPotionID, &g_npcStore.npc, nQuantity);
}

BOOL EXPORT BuyTownScroll(int nQuantity)
{
	if (!g_npcStore.dwTownScrollID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No town scroll found!");
		return FALSE;	
	}

	return BuyMiscItem(g_npcStore.dwTownScrollID, &g_npcStore.npc, nQuantity);
}

BOOL EXPORT BuyIdentifyScroll(int nQuantity)
{
	if (!g_npcStore.dwIdentifyScrollID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No identify scroll found!");
		return FALSE;	
	}

	return BuyMiscItem(g_npcStore.dwIdentifyScrollID, &g_npcStore.npc, nQuantity);
}

BOOL EXPORT BuyArrows(int nQuantity)
{
	if (!g_npcStore.dwArrowID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No arrows found!");
		return FALSE;	
	}
	
	if (nQuantity < 1)
		return FALSE;	

	return BuyMiscItem(g_npcStore.dwArrowID, &g_npcStore.npc, nQuantity);
}

BOOL EXPORT BuyBolts(int nQuantity)
{
	if (!g_npcStore.dwBoltID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No bolts found!");
		return FALSE;	
	}
	
	if (nQuantity < 1)
		return FALSE;

	return BuyMiscItem(g_npcStore.dwBoltID, &g_npcStore.npc, nQuantity);
}

BOOL EXPORT StartNpcSession(LPCGAMEUNIT lpNpcUnit, int nInteractType)
{
	UnitAny* pUnit = (UnitAny*)VerifyUnit(lpNpcUnit);
	if (pUnit == NULL)
	{
		//GamePrintError("Invalid unit");
		return FALSE;
	}
	
	if (nInteractType == NPC_TRADE && GetNpcTradeMenuID(pUnit->dwClassId) == -1)
	{
		//GamePrintError("Not a vendor");
		return FALSE; // not a vendor
	}

	if (nInteractType == NPC_GAMBLE && GetNpcGambleMenuID(pUnit->dwClassId) == -1)
	{
		//GamePrintError("Not a Gambler");
		return FALSE; // not a gambler
	}

	g_nNpcFail = 0;
	::memset(&g_npcStore, 0, sizeof(NPCSTORE));
	::memcpy(&g_npcStore.npc, lpNpcUnit, sizeof(GAMEUNIT));
	g_npcStore.nInteractType = nInteractType;
	CleanJobs();
	CloseAllUIs();
	g_aBuys.RemoveAll();
	g_aSells.RemoveAll();
	g_aGambles.RemoveAll();
	g_npcState.Update(NPC_RUNTO, 500);
	return TRUE;
}

BOOL IsNpcActive(int nInteractType)
{
	return IsUIOpened(UI_NPCMENU)
		&& g_npcStore.npc.dwUnitID
		&& g_npcStore.nInteractType == nInteractType
		&& g_npcStore.bSessionStarted;
}

void EXPORT EndNpcSession()
{
	g_aBuys.RemoveAll();
	g_aSells.RemoveAll();
	g_aGambles.RemoveAll();
	::memset(&g_npcStore, 0, sizeof(NPCSTORE));
	CloseAllUIs();
}

BOOL EXPORT SellItem(DWORD dwItemID)
{
	if (!VerifyItem(dwItemID))
		return FALSE;

	if (!IsNpcActive(NPC_TRADE) && !IsNpcActive(NPC_GAMBLE))
		return FALSE;

	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x33;
	::memcpy(&aPacket[1], &g_npcStore.npc.dwUnitID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	aPacket[13] = 0x01;
	return GameSendPacketToServer(aPacket, 17);
}

int EXPORT DrinkAllInventoryPotions(BOOL bKeepFullRejuv)
{
	CArrayEx<DWORD, DWORD> aPotions;
	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (!pUnit)
		return 0;

	Inventory* pInv = pUnit->ptInventory;

	if (!pInv)
		return 0;

	for (UnitItem* p = pInv->pFirstItem; p && p->ptItemData; p = p->ptItemData->ptNextInvenItem)
	{
		if (D2COMMON_GetItemNodePage(p) == 1)
		{
			char szCode[4] = "";
			GetItemCode(p->dwId, szCode, 3);
			if (D2IsPotion(szCode))
			{
				if (!bKeepFullRejuv || stricmp(szCode, "rvl"))
					aPotions.Add(p->dwId);
			}
		}	
	}

	if (aPotions.IsEmpty())
		return 0;

	int nCount = 0;
	for (int i = 0; i < aPotions.GetSize(); i++)
	{
		if (DrinkInventoryPotion(aPotions[i]))
			nCount++;
	}

	return nCount;
}

BOOL EXPORT IsMercAlive()
{
	GAMEUNIT gu = {0};
	if (!GetMyMercUnit(&gu))
		return FALSE;

	return GetUnitStat(&gu, STAT_HP) != 0;
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

BOOL IsNpcBullShitting()
{
	// is the npc bull shitting with me? (quests assigning or information chatting)
	DWORD dwResult = 0;
	if (!::ReadProcessMemory(::GetCurrentProcess(), (LPCVOID)0x6FBB9EA8, &dwResult, 4, NULL))
		return FALSE;
	return dwResult;	
}

DWORD EXPORT GetSpellCharges(WORD wSpellID)
{
	CHARGEABLE ca = { 0 };
	ca.wSpellID = wSpellID;
	int nIndex = g_aSkillCharges.Find(ca);
	if (nIndex == -1)
		return 0;

	return g_aSkillCharges[nIndex].dwCharges;
}

int EXPORT GetUnidItemCount()
{
	UnitPlayer* p = D2CLIENT_GetPlayerUnit();
	if (!p)
		return 0;

	return D2COMMON_GetNumberOfUnidItems(p);
}

BYTE EXPORT GetLowestEquipDurabilityPercent(BOOL bIncludeSecondarySlots)
{
	return g_inventory.GetLowestEquipItemDurability(bIncludeSecondarySlots);
}

LPCITEM EXPORT GetEquipItem(int nEquipLocation)
{
	return g_inventory.GetEquipItem(nEquipLocation);
}

int EXPORT GetAvailableBeltSlots(int nColumn)
{
	if (nColumn > 3)
		return 0;

	int nCount = 0;
	for (BYTE row = 0; row < g_iBeltRows; row++)
	{
		if (g_aBeltItems[row][nColumn].dwItemID == 0)
			nCount++;
	}

	return nCount;
}

LPCITEM EXPORT GetStorageItem(int nStorageType, int x, int y)
{
	return g_inventory.GetStorageItem(nStorageType, x, y);
}

int EXPORT SellInventoryItems(int nLeftColumn, int nRightColumn)
{
	if (!IsNpcActive(NPC_TRADE) && !IsNpcActive(NPC_GAMBLE))
		return 0;

	CArrayEx<DWORD, DWORD> aSold;

	nRightColumn = min(nRightColumn, 9);
	for (int y = 0; y < 4; y++)
	{
		for (int x = nLeftColumn; x <= nRightColumn; x++)
		{
			LPCITEM p = g_inventory.GetStorageItem(STORAGE_INVENTORY, x, y);
			if (p && aSold.Find(p->dwItemID) == -1 && SellItem(p->dwItemID))
				aSold.Add(p->dwItemID); // aviod repetitive selling
		}
	}
	
	return aSold.GetSize();
}

void InsertGambleItem(DWORD dwItemID, DWORD* aItems, DWORD dwArrayLen)
{
	if (dwItemID == 0 || aItems == NULL || dwArrayLen == 0)
		return;

	for (DWORD i = 0; i < dwArrayLen; i++)
	{
		if (aItems[i] == 0)
		{
			aItems[i] = dwItemID;
			return;
		}
	}
}

int BatchGamble(DWORD* aItems, DWORD dwArrayLen)
{
	if (aItems == NULL || dwArrayLen == 0)
		return 0;

	int nCount = 0;
	for (DWORD i = 0; i < dwArrayLen; i++)
	{
		if (aItems[i])
		{
			g_aGambles.Add(aItems[i]);
			nCount++;
		}

		aItems[i] = 0;
	}

	return nCount;
}

int EXPORT GambleAmulets()
{
	return BatchGamble(g_npcStore.aAmulets, 5);
}

int EXPORT GambleRings()
{
	return BatchGamble(g_npcStore.aRings, 5);
}

int EXPORT GambleCirclets()
{
	return BatchGamble(g_npcStore.aCirclets, 5);
}

int EXPORT GambleGloves()
{
	return BatchGamble(g_npcStore.aGloves, 5);
}

int EXPORT GambleBoots()
{
	return BatchGamble(g_npcStore.aBoots, 5);
}

int EXPORT GambleBelts()
{
	return BatchGamble(g_npcStore.aBelts, 5);
}

void DoGambleProc()
{
	// gambling?
	if (!g_aGambles.IsEmpty())
	{
		Gamble(g_aGambles[0]);
		g_aGambles.RemoveAt(0);
	}
}

BOOL EXPORT IsNpcSessionStarted(DWORD dwNpcID, int nInteractType)
{
	if (!dwNpcID || g_npcStore.npc.dwUnitID != dwNpcID || !g_npcStore.bSessionStarted)
		return FALSE;

	if (!nInteractType)
		return TRUE;

	return g_npcStore.nInteractType == nInteractType;
}


void EXPORT DebugTest()
{
	//g_inventory.Dump("c:\\test.txt");
}

