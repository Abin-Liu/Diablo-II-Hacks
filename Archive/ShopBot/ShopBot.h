//////////////////////////////////////////////////////////////////////
// ShopBot.h
//
// Copyright (c) 2004, Abin (Bin Liu)
// All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SHOPBOT_H__
#define __SHOPBOT_H__

#include "..\\D2Lib\\D2Lib.h"
#include "D2Town.h"
#include "SBItemConfig.h"
#include "ShopbotPathDef.h"

/////////////////////////////////////////////////////
// D2Lib Objects
/////////////////////////////////////////////////////
CD2Player Me;			// Player
CD2Unit Npc;			// NPC
CD2Town Town;			// Town stuff, wps, healers
CD2StaticObject g_outWP; // The outside WP
CKeyBind g_hotkey;		// Hotkey
CD2TextDraw g_text;		// For drawing texts on game screen

typedef CArrayEx<D2POSITION, D2POSITION> D2PATH;

CIni g_log; // Log file Shopbot.log
CTicker g_ticker; // Ticker
SBGLOBAL g_global = { 0 }; // Global setting
CArrayEx<SBITEMCONFIG, const SBITEMCONFIG&> g_aConfig; // Item settings
CLinkedList<ITEM, const ITEM&, NULL> g_aGoodItems; // Good items from NPC
D2PATH g_aStartToWP; // From start position to WP
D2PATH g_aWPToNpc; // From WP to NPC
D2PATH g_aWPToHealer; // From WP to healer

BOOL g_bWasHealing = FALSE; // Was visiting a healer
BYTE g_iTownMapID = 0; // Town map
DWORD g_dwTotalTime = 0; // Total in-game time
BOOL g_bPaused = FALSE; // Bot paused?
int g_nGameState = 0; // Game state
DWORD g_dwTotalShopping = 0; // Total shopping
DWORD g_dwTotalBought = 0; // Total items bought
DWORD g_dwTownCheckCount = 0; // Town check retry counter
DWORD g_dwTotalNPCFail = 0; // NPC fail counter
DWORD g_dwItemChecked = 0; // Item examed from npc store
BOOL g_bUseSig = FALSE; // using sig?
BOOL g_bAutoStop = FALSE; // auto stop?
DWORD g_dwRemainMS = 0; // Remaining milliseconds before auto stop
DWORD g_dwTotalExamined = 0; // Number of items that have been examined

int g_nNextState = 0; // Next game state
char g_szNpcName[64] = ""; // NPC name
BOOL g_bNeedChangeAct = FALSE; // Need to change act?
BOOL g_bBoSOn = FALSE; // Burst of speed, assassin only

WORD g_wQuicknessSpell = D2S_INVALID; // Spell for quicker motion

#define MAX_RETRY	5
int g_nRetry = 0; // Action retry counter

char g_szStopCallback[1025] = ""; // Callback commands after stopped
BOOL g_bNeedStop = FALSE; // Need to stop?
BYTE g_iEndMap = MAP_A1_ROGUE_ENCAMPMENT; // where to go after shopbot stops
BOOL g_bStartAfterGameReady = FALSE; // start after game ready?

// Game State Definition
enum {
	GS_NONE = 0,			// No actions
	GS_CALCSTARTTOWPPATH,	// Calculate "Start to WP" path
	GS_ACTSWAPOK,			// Act swapped
	GS_CALCWPTONPCPATH	,	// Calculate "WP to npc" path
	GS_STARTTOWP,			// From start location to wp
	GS_FIRSTWPREACHED,		// First time we reached wp
	GS_WAITINTOWN,			// Wait for player location update
	GS_WPTONPC,				// From wp to npc
	GS_BOOSTSPEED,			// Case Burst of speed or Vigor
	GS_TALKNPC,				// Talk to npc
	GS_OPENSTORE,			// Open npc store
	GS_WAITNPCITEMS,		// Wait for npc items
	GS_STOREOK,				// NPC store opened successfully
	GS_BUYITEM,				// Buy an item
	GS_CHECKBUYITEM,		// Check if item is bought successfully
	GS_CLOSESTORE,			// Close npc store
	GS_NPCTOWP,				// From npc to wp
	GS_OPENWP,				// Open wp
	GS_WAITOPENWP,			// Wait for open WP operation reasults
	GS_USEWP,				// Use wp to swap areas
	GS_WAITUSEWP,			// Wait for use wp results
	GS_WAITMOVE,			// Wait for move completes
	GS_WAITNPC,				// Wait for npc interaction
	GS_WPTOSTART,			// WP to start location
	GS_READYSTOP,			// Ready to stop
};

////////////////////////////////////////////////////////////////////
// D2Lib Object Message Handlers
////////////////////////////////////////////////////////////////////
void CALLBACK OnPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData);
BOOL CALLBACK OnHotkey(BYTE iVkCode, LPCTSTR lpszCommand);

////////////////////////////////////////////////////////////////////
// Shopbot Module Commands
////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);
BOOL PRIVATE OnGameCommandPause(char** argv, int argc);
BOOL PRIVATE OnGameCommandCleanLog(char** argv, int argc);
BOOL PRIVATE OnGameCommandReload(char** argv, int argc);
BOOL PRIVATE OnGameCommandCallback(char** argv, int argc);
BOOL PRIVATE OnGameCommandStart(char** argv, int argc);
BOOL PRIVATE OnGameCommandStop(char** argv, int argc);
BOOL PRIVATE OnGameCommandAutoStart(char** argv, int argc);
BOOL PRIVATE OnGameCommandEndAct(char** argv, int argc);

////////////////////////////////////////////////////////////////////
// Application Functions
////////////////////////////////////////////////////////////////////
const CD2StaticObject* GetOutWP(); // Get outside WP id
int CalculateStartToWPPath(); // Calculate the path from start location to WP
int CalculateWPToNpcPath(); // Calculate path from WP to NPC
void UpdateGameState(int nNewState, DWORD dwDelay = 0); // Update game state
void WaitForMove(int nNextState); // Wait for move to complete
void WaitForNpc(int nNextState); // Wait for NPC interaction results
void UpdateScreenText(); // Update screen text
BOOL IsNeedHeal(); // Need to visit a healer?
void SigPostString(LPCTSTR lpszString); // post strings to sig gui
void LoadLogData(); // Loda data from shopbot.log
void LoadGlobalData(); // Load global data from shopbot.ini
void LoadConfigData(); // Load config data from shopbot.ini
void InitializeObjects(); // Initialize game objects abd attributes
BOOL CleanLog(); // Clean shopbot.log
void GetTimeFormat(DWORD dwMilliseconds, DWORD& dwHours, DWORD& dwMinutes, DWORD& dwSeconds); // format to hh:mm:ss
BOOL VerifyConditions(); // Verify conditions to determine whether shopbot can start
void StartShopping(); // Start shopping
WORD UseQuicknessSpell(); // Try to use vigor or BoS

CLIENTINFO
(		
	1,30,						// Module version (Major, Minor)
	"Abin",						// Author
	"www.wxjindu.com/abin/",	// url (http:// is appended)
	"Sample plugin",			// Short module description
	"abinn32@yahoo.com"			// Author eMail
)

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc6 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	{
		"debug",
		OnGameCommandDebug,
		"debug"
	},

	{
		"pause",
		OnGameCommandPause,
		"pause: Pause/Resume the bot."
	},

	{
		"cleanlog",
		OnGameCommandCleanLog,
		"cleanlog: Cleans shopbot.log."
	},

	{
		"reload",
		OnGameCommandReload,
		"reload: Reloads item config from Shopbot.ini."
	},

	{
		"callback",
		OnGameCommandCallback,
		"callback: Set the callback commands. Syntax: .shopbot callback [command1;command2;...;commandn]"
	},

	{
		"start",
		OnGameCommandStart,
		"start: Start shopbot. Syntax: .shopbot start [seconds]"
	},

	{
		"stop",
		OnGameCommandStop,
		"stop: Stop shopbot and back to start position."
	},

	{
		"autostart",
		OnGameCommandAutoStart,
		"autostart: .shopbot autostart 0/1."
	},

	{
		"endact",
		OnGameCommandEndAct,
		"endact: .shopbot endact 1/2/3/4/5."
	},

	// Add your own commands here

	{NULL}	// No more commands
};


#endif // __SHOPBOT_H__