//////////////////////////////////////////////////////////////////////
// Pindlebot.h
//
// Pindlebot client definition and implementation. Based on D2Lib.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// August 8th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __PINDLEBOT_H__
#define __PINDLEBOT_H__

#include "..\\D2Lib\\D2Lib.h"
#include "PickitPlayer.h"
#include <time.h>
#include "Pindleskin.h"
#include "PBotConfig.h"

// reason for finishing a game
enum {
	FR_UNKNOWN = 0,		// unknown
	FR_SUCCEEDED,		// succeeded
	FR_INDANGER,		// player in danger
	FR_DEADLYCOMBO,		// pindleskin has deadly combo
	FR_CHARDEAD,		// player dead
	FR_MERCDEAD,		// pet dead
	FR_FAILED,			// failed
	FR_REPAIR,			// item repair
	FR_OUTSKILL,		// insufficient skills
	FR_WARPWP,			// wp warps
	FR_IDBOT,			// idbot used
};

// Motion States
enum {  MS_NONE = 0,		// none		
		MS_IDLE,			// idle for xxx seconds
		MS_INIT_START,		// initial state OK
		MS_ADJUST_LOCATION, // first move
		MS_OPEN_WP,			// open act1 wp
		MS_WARP_ACT5,		// goto act5 through WP
		MS_AFTER_WP,		// after using WP
		MS_TAVEL_PORTAL,	// travel to portal
		MS_JUNCTION,		// where we determine if need to res merc
		MS_VERIFY_STEP,		// verify character position
		MS_VERIFY_NPC,		// verify npc interactions
		MS_CHECK_PORTAL,	// check if reached portal
		MS_TALK_PORTAL,		// take portal
		MS_CHECK_INPORTAL,	// check if entered portal successfully
		MS_TALK_MALAH,		// interact with entity - Malah	
		MS_LEAVE_MALAH,
		MS_AFTER_MALAH,		// back to portal track[0]
		MS_TALK_QUAL,		// interact with entity - Qual
		MS_RES_MERC,		// resurrect merc
		MS_LEAVE_QUAL,		// stop interact with Qual-kehk
		MS_AFTER_QUAL,		// keeps going towards red portal after visiting Qual-kehk
		MS_TRAVEL_LARZUK,	// travel to Larzuk
		MS_TALK_LARZUK,		// interact with Larzuk
		MS_OPEN_STORE,		// open larzuk store
		MS_VERIFY_STORE,
		MS_ASK_REPAIR,		// repair all
		MS_REFILL_AMMO,		// buy arrow/bolt
		MS_WAIT_AMMO,		// is arrow/bolt bought successfully?
		MS_LEAVE_LARZUK,	// ...
		MS_AFTER_LARZUK,	// 
		MS_TRAVEL_CAIN,
		MS_TALK_CAIN,
		MS_IDENTIFY,
		MS_LEAVE_CAIN,
		MS_AFTER_CAIN,		
		MS_LOAD_IDBOT,
		MS_USE_IDBOT,		// using idbot
		MS_IDBOT_END,
		MS_FINISHED,		// game finished
		MS_MAP_UPDATE,		// map update
		MS_CAST_FINDITEM,	// cast fint item skill(Barbarians)
		MS_PICK_ITEMS,		// enable pickit and radius
		MS_PICKITEMS_OK,	// ready to exit
		MS_APPROACH_PINDLESKIN, // approaching pindleskin
		MS_PREPARETOQUIT,		// prepare to quit game
		MS_CAST_BEFORERED,	// spells before entering red portal
		MS_CAST_AFTERRED,	// spells after entering red portal
		MS_CAST_ONPINDLESKIN,	// killing pindleskin
		MS_PINDLE_DEAD,		// Pindleskin is just killed		
		MS_DEBUG			// debug
	 };

#define HARDDELAY	250		// mandatory spell delay, will be added to every action

// Skill casting results
enum { SCR_SUCCEEDED = 0, SCR_NOMORE, SCR_INVALIDSKILL, SCR_OUTOFAMMO };
THISGAMESTRUCT* thisgame = NULL;

// D2Lib objects
CKeyBind g_keyBind;			// For hotkey intercept
CPickitPlayer Me;			// The player
CD2Merc Pet;				// Pet
CD2Unit Malah;				// Malah
CD2Unit Larzuk;				// Larzuk
CD2Unit Qual;				// Qual-Kehk
CD2Unit Cain;				// Cain in act5
CPindleskin Pindleskin;		// Pindleskin
CD2StaticObject RedPortal;	// Red portal
CD2StaticObject Act1Wp;		// Act1 WP
PBOTCONFIG g_config = { 0 }; // bot configuration

BYTE g_iActionIndex = 0;	// Current action index
BYTE g_iSpellCast = 0;		// Current spell casting counter
int g_nTotalGame = 0;			// total game count
BOOL g_bUseSig = FALSE;			// use sig?
CIni g_logIni; // log files		// pindlebot.txt
int g_nCurState = MS_NONE;		// current state
int g_nNextState = MS_NONE;		// next state
BOOL g_bPaused = FALSE;			// game paused
DWORD g_dwTotalTime = 0;		// total in-game milliseconds
CTicker g_ticker; // ticker		// time ticker
CLinkedList<MONSTERCORPSE, const MONSTERCORPSE&, NULL> g_aCorposes; // Monster corposes list
BOOL g_bStop = TRUE;			// stopped?
DWORD g_dwArrowID = 0x00;		// store arrow ID
DWORD g_dwBoltID = 0x00;		// store bolt ID
int g_nPortalRetry = 0;			// red portal retries
WORD g_wOldX = 0;				// old position
WORD g_wOldY = 0;		
BOOL g_bInventoryFull = FALSE;	// Is TRUE, next game goto Cain and use IDBot
DWORD g_dwIdleLength = 0;		// Idle length before running, anti-cdkey-banning
BOOL g_bUseShopbot = FALSE;		// Use shopbot?

// from red portal to pindleskin, map coordinates of Nihlathak's temple vary from
// game to game, we must calculate the route in each game
CArrayEx<D2POSITION, D2POSITION> g_aPindleRoute; 

// D2LIB Message handlers
void CALLBACK OnPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData);
void CALLBACK OnPetMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData);
void CALLBACK OnPindleskinMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData);
BOOL CALLBACK OnHotkey(BYTE iVkCode, LPCTSTR lpszCommand);

// Module Commands
BOOL PRIVATE OnGameCommandPause(char** argv, int argc);
BOOL PRIVATE OnGameCommandResume(char** argv, int argc);
BOOL PRIVATE OnGameCommandStatistics(char** argv, int argc);
BOOL PRIVATE OnGameCommandStart(char** argv, int argc);
BOOL PRIVATE OnGameCommandStop(char** argv, int argc);
BOOL PRIVATE OnGameCommandIDOK(char** argv, int argc);
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);

// Misc Functions
void UpdateBotState(int nNewState, DWORD dwTicker = 0);
void WaitForMove(int nNextState);
void WaitForNPC(int nNextState);
void CalcPindleRoute();
void FinishRun(int nReason, BOOL bRunAway); // finish current run immediately
void ProcessActions(int nType);
DWORD ProcessCorpose();
BOOL PerformAction(const PBOTACTION& action, BYTE iTime);
BOOL CheckPindleAttrs();
void DisplayIdleTime();

// SIG functions
void SIGPostString(LPCTSTR lpsz);

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
//////////////////////////////////////////////////////////////////////
DWORD   ModuleVersion=MAKELONG(5, 1);
char    ModuleAuthor[]="Abin";
char    ModuleDescription[]="Pindlebot for SIG";
char    ModuleWebsite[]="http://www.wxjindu.com/abin/";
char    ModuleEmail[]="abinn32@yahoo.com";

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",  OnGameCommandHelp,
		"help ÿc0 List commands available in this module.\n"
		"<command> help ÿc0 Shows detailed help for <command> in this module."
	},

	{
		"pause", OnGameCommandPause,
		"pause ÿc0 Pause the bot."
	},

	{
		"resume", OnGameCommandResume,
		"resume ÿc0 Resume the bot."
	},

	{
		"statistics", OnGameCommandStatistics,
		"statistics ÿc0 Display current running results."
	},

	{
		"start", OnGameCommandStart,
		"start ÿc0 Start current run."
	},

	{
		"stop", OnGameCommandStop,
		"stop ÿc0 Stop pindlebot."
	},

	{
		"idok", OnGameCommandIDOK,
		"idok ÿc0 ID items ok."
	},

	{
		"debug", OnGameCommandDebug,
		"Debug ÿc0 For debug purpose, users should ÿc1notÿc0 use this."
	},

	{NULL}
};

#endif