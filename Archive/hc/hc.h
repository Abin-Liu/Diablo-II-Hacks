////////////////////////////////////////////////////////////
// hc.h
//
// A D2HackIt module that auto reacts when character is threatened.
//
// Copyright (c) 2002, Abin (abinn32@yahoo.com)
// All rights reserved.
//
// Version History:
//
// v1.5 - Fixed a bug on HP/mana detection.
// v1.4 - Fixed some minor bugs.
// v1.3 - Almost a complete rewrite, now code are based on "D2Lib" library.
//        Added command "mana".
//        Syntax of some commands have been changed.
// v1.2 - Added command "merc"
//        Now is able to automatically heal the player and merc
// v1.1 - Interacts with bind.d2h properly.
// v1.0 - Fixed a hotkey glitch.
// v0.8 - Now is 1.10 compatible.
// v0.7 - Fixed a bug so now character TP to town properly when low on HP.
// v0.6 - HP being low will no longer causes character to quit immediately after joining game.
// v0.5 - Official release, it's no longer a beta~!
// v0.4 - Guaranteed to enter portal after casting it.
// v0.3 - Added "TP to town" feature.
// v0.2 - Fixed a shortcut key setting problem.
// v0.1 - Beta release.
//
////////////////////////////////////////////////////////////

#ifndef __HC_H__
#define __HC_H__

#include "..\\D2Lib\\D2Lib.h" 

////////////////////////////////////////////////////////////
// Reaction Type Definition
////////////////////////////////////////////////////////////
enum {	TR_NONE = 0,			// do nothing
		TR_QUIT,				// quit to channel
		TR_TOWN,				// go to town
		TR_POTION };			// drink a potion

////////////////////////////////////////////////////////////
// Alert Type Definition
////////////////////////////////////////////////////////////
enum {	ALERT_PLAYER_HP = 0,	// player's hp low
		ALERT_PLAYER_MANA,		// player's mana low
		ALERT_MERC };			// merc hp percent low

////////////////////////////////////////////////////////////
// Search Method Types for Bind.ini
////////////////////////////////////////////////////////////
enum {	KB_UNKNOWN = 0,			// Should never happen
		KB_QUIT,				// quit key
		KB_SYNC,				// sync key
		KB_TOWN	};				// town key

////////////////////////////////////////////////////////////
// Global Variable Definition
////////////////////////////////////////////////////////////
CD2Player Me;						// D2lib object: The player, Me
CD2Merc Pet;						// D2lib object: My pet
CIni g_ini;							// hc.ini
CIni g_bind_ini;					// bind.ini
BYTE g_iQuitHP = 0;					// Quit game when HP below this percent
BYTE g_iTownHP = 0;					// TP to town when HP below this percent
BYTE g_iPotionHP = 0;				// Drink potion when HP below this percent
BYTE g_iQuitMana = 0;				// Quit game when mana below this percent
BYTE g_iTownMana = 0;				// TP to town when mana below this percent
BYTE g_iPotionMana = 0;				// Drink potion when mana below this percent
BYTE g_iMercQuitHP = 0;				// Quit game when merc's HP below this percent
BYTE g_iMercTownHP = 0;				// TP to town when merc's HP below this percent
BYTE g_iMercPotionHP = 0;			// Drink potion when merc's HP below this percent
int g_nOnHostile = TR_NONE;			// Reaction when hostiled by other players
BOOL g_bQuitNow;					// SHould quit game now?
char g_szPlayerName[PLAYER_MAX_NAME + 1] = ""; // Player name
BOOL g_bMemChecked = FALSE;

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
//////////////////////////////////////////////////////////////////////
DWORD   ModuleVersion=MAKELONG(1, 5);
char    ModuleAuthor[]="Abin";
char    ModuleWebsite[]="http://www.wxjindu.com/abin/";
char    ModuleDescription[]="HC - Auto-react when in danger";
char    ModuleEmail[]="abinn32@yahoo.com";

//////////////////////////////////////////////////////////////////////
// Function Definition
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc);
BOOL PRIVATE OnGameCommandHostile(char** argv, int argc);
BOOL PRIVATE OnGameCommandHP(char** argv, int argc);
BOOL PRIVATE OnGameCommandMana(char** argv, int argc);
BOOL PRIVATE OnGameCommandMerc(char** argv, int argc);
BOOL PRIVATE OnGameCommandSync(char** argv, int argc);
BOOL PRIVATE OnGameCommandQuit(char** argv, int argc);
BOOL PRIVATE OnGameCommandSummary(char** argv, int argc);
BOOL PRIVATE OnGameCommandSetQuitKey(char** argv, int argc);
BOOL PRIVATE OnGameCommandSetSyncKey(char** argv, int argc);
BOOL PRIVATE OnGameCommandSetTownKey(char** argv, int argc);
BOOL PRIVATE OnGameCommandTown(char** argv, int argc);
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);
DWORD EXPORT OnGameTimerTick(void);

// D2Lib Message Handlers
void CALLBACK OnPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData);
void CALLBACK OnPetMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData);

void LoadConfig();									// Loads settings from "hc.ini"
BOOL SetShortcutKey(BYTE iKeyCode, BOOL bQuitKey);	// Bind sa key code to a command
void ShowHostilePolicy();							// Displays hostility reactions
void ShowHPPolicy();								// Displays low-HP reactions
void ShowManaPolicy();								// Displays low-HP reactions
void ShowMercPolicy();								// Displays merc low-HP reactions
void ShowShortcutKeys();							// Displays shortcut key settings
BOOL IsBindPresent();								// Check if "bind" is present
void SearchBindKeys(BYTE& iQuitKey, BYTE& iSyncKey, BYTE& iTownKey); // Search hotkey settings in "bind.ini"
BOOL CheckAlert(int nAlertType, BYTE iValue);		// Checks alert types
int Str2Method(LPCTSTR lpsz, BOOL bAllowPotion);	// Gets a method from its string representation

//////////////////////////////////////////////////////////////////////
// Module Commands
//////////////////////////////////////////////////////////////////////
MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",  OnGameCommandHelp,
		"help ÿc0List commands available in this module.\n"
		"<command> help ÿc0 Shows detailed help for <command> in this module."
	},
	{
		"hostile", OnGameCommandHostile,
		"hostile ÿc0Specify your action when hostiled.\nSyntax: .hc hostile <nothing/quit/town>"
	},
	{
		"hp", OnGameCommandHP,
		"hp ÿc0Specify your action when HP is low.\nSyntax: .hc hp <quit/town/potion> <Percent>"
	},
	{
		"mana", OnGameCommandMana,
		"mana ÿc0Specify your action when mana is low.\nSyntax: .hc mana <quit/town/potion> <Percent>"
	},
	{
		"merc", OnGameCommandMerc,
		"merc ÿc0Specify your action when merc's HP is low.\nSyntax: .hc merc <quit/town/potion> <Percent>"
	},	
	{
		"sync", OnGameCommandSync,
		"sync ÿc0Force client view to synchronize char location.\nSyntax: .hc sync"
	},
	{
		"quit", OnGameCommandQuit,
		"quit ÿc0Save and exit.\nSyntax: .hc quit"
	},
	{
		"debug", OnGameCommandDebug,
		"debug ÿc0For debug purpose only\n"
	},	
	{
		"summary", OnGameCommandSummary,
		"summary ÿc0Displays all settings for current character.\nSyntax: .hc summary"
	},
	{
		"key.quit", OnGameCommandSetQuitKey,
		"key.quit ÿc0Set shortcut key for \"Save And Exit\".\nSyntax: .hc key.quit <key_code>"
	},
	{
		"key.sync", OnGameCommandSetSyncKey,
		"key.sync ÿc0Set shortcut key for \"Sync\".\nSyntax: .hc key.sync <key_code>"
	},
	{
		"key.town", OnGameCommandSetTownKey,
		"key.town ÿc0Set shortcut key for \"Town\".\nSyntax: .hc key.town <key_code>"
	},
	{
		"town", OnGameCommandTown,
		"town ÿc0Cast a town portal and enter it.\nSyntax: .hc town"
	},
	{NULL}
};

#endif // __HC_H__