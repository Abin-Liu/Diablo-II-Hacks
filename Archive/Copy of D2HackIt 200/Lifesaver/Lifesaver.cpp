//////////////////////////////////////////////////////////////////////
// LifeSaver.cpp
// -------------------------------------------------------------------
// Automatically reacts when player is in danger.
//
// Abin (abinn32@yahoo.com)
//
// Version History:
//
// 1.00 2004-09-20 Abin - Initial release for D2Hackit 2.00
//////////////////////////////////////////////////////////////////////
#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include "Utilities\\Ini.h"
#include "..\\Includes\\ClientCore.cpp"	  // Include the backend of the module


//////////////////////////////////////////////////////////////////////
// Global stuff.
// -------------------------------------------------------------------
// This is a very convenient place to your function declarations and
// global variables.
//////////////////////////////////////////////////////////////////////
CIni g_ini;
GAMEUNIT g_pet = {0};
BYTE g_iPetHP = 0;
BOOL g_bGameReady = FALSE;

// Reactions on hostility
enum { ACTION_NONE = 0, ACTION_QUIT, ACTION_TOWN, ACTION_POTION };

// Hotkey types
enum { KEY_UNKNOWN = 0, KEY_QUIT, KEY_TOWN, KEY_SYNC };

////////////////////////////////////////////////////////////
// Alert Type Definition
////////////////////////////////////////////////////////////
enum {	ALERT_PLAYER_HP = 0,	// player's hp low
		ALERT_PLAYER_MANA,		// player's mana low
		ALERT_MERC };			// merc hp percent low

BYTE g_iQuitHP = 0;					// Quit game when HP below this percent
BYTE g_iTownHP = 0;					// TP to town when HP below this percent
BYTE g_iPotionHP = 0;				// Drink potion when HP below this percent
BYTE g_iQuitMana = 0;				// Quit game when mana below this percent
BYTE g_iTownMana = 0;				// TP to town when mana below this percent
BYTE g_iPotionMana = 0;				// Drink potion when mana below this percent
BYTE g_iMercQuitHP = 0;				// Quit game when merc's HP below this percent
BYTE g_iMercTownHP = 0;				// TP to town when merc's HP below this percent
BYTE g_iMercPotionHP = 0;			// Drink potion when merc's HP below this percent
int g_nOnHostile = ACTION_NONE;	// Reaction when hostiled by other players
BOOL g_bQuitNow = FALSE;			// Should quit game now?

// hotkeys
BYTE g_iTownKey = 0;
BYTE g_iQuitKey = 0;
BYTE g_iSyncKey = 0;

/////////////////////////////////////////////////////////////////////
// Module Commands Prototypes
/////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
// -------------------------------------------------------------------
// Tell us a little about this module.
//////////////////////////////////////////////////////////////////////
CLIENTINFO
(		
	1,0,							// Module version (Major, Minor)
	"Abin",						// Author
	"www.wxjindu.com/abin/",		// url (http:// is appended)
	"Life Saver",				// Short module description
	"abinn32@yahoo.com"				// Author eMail
)

//////////////////////////////////////////////////////////////////////
// MODULECOMMANDSTRUCT ModuleCommands[]
// -------------------------------------------------------------------
// To add your own commands, just add them to this list like:
//
//	{
//		"name"			// Name of the command
//		pFunction		// The function that handles command
//		"usage"			// Help text for the command
//	}
// 
// ...where pFunction is the name of the function that handles the
// command. The function must be declared as...
//
// BOOL PRIVATE OnGameCommandName(char** argv, int argc)
//
// ...and return TRUE on success or FALSE to show usage of the command.
//
// The "usage" line can contain "\n" as linebreaks and should be in 
// the following format for consistency:
//
// "commandname <required parameter> [optional parameter]ÿc0 Description"
//
//
// To use your commands in the game, just type 
// .modulename <command> [arguments]
//////////////////////////////////////////////////////////////////////

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	{
		"hostile", OnGameCommandHostile,
		"hostile ÿc0Specify your action when hostiled.\nSyntax: .lifesaver hostile <nothing/quit/town>"
	},
	{
		"hp", OnGameCommandHP,
		"hp ÿc0Specify your action when HP is low.\nSyntax: .lifesaver hp <quit/town/potion> <Percent>"
	},
	{
		"mana", OnGameCommandMana,
		"mana ÿc0Specify your action when mana is low.\nSyntax: .lifesaver mana <quit/town/potion> <Percent>"
	},
	{
		"merc", OnGameCommandMerc,
		"merc ÿc0Specify your action when merc's HP is low.\nSyntax: .lifesaver merc <quit/town/potion> <Percent>"
	},	
	{
		"sync", OnGameCommandSync,
		"sync ÿc0Force client view to synchronize char location.\nSyntax: .lifesaver sync"
	},
	{
		"quit", OnGameCommandQuit,
		"quit ÿc0Save and exit.\nSyntax: .lifesaver quit"
	},
	{
		"debug", OnGameCommandDebug,
		"debug ÿc0For debug purpose only\n"
	},	
	{
		"summary", OnGameCommandSummary,
		"summary ÿc0Displays all settings for current character.\nSyntax: .lifesaver summary"
	},
	{
		"key.quit", OnGameCommandSetQuitKey,
		"key.quit ÿc0Set shortcut key for \"Save And Exit\".\nSyntax: .lifesaver key.quit <key_code>"
	},
	{
		"key.sync", OnGameCommandSetSyncKey,
		"key.sync ÿc0Set shortcut key for \"Sync\".\nSyntax: .lifesaver key.sync <key_code>"
	},
	{
		"key.town", OnGameCommandSetTownKey,
		"key.town ÿc0Set shortcut key for \"Town\".\nSyntax: .lifesaver key.town <key_code>"
	},
	{
		"town", OnGameCommandTown,
		"town ÿc0Cast a town portal and enter it.\nSyntax: .lifesaver town"
	},

	{NULL}	// No more commands
};

/////////////////////////////////////////////////////////////////////
// Other Function Prototypes
/////////////////////////////////////////////////////////////////////
void LoadConfig(LPCSTR lpszSection);				// Loads settings from "LifeSaver.ini"
BOOL SetShortcutKey(BYTE iKeyCode, int nType);		// Bind a key code to a command
void ShowHostilePolicy();							// Displays hostility reactions
void ShowHPPolicy();								// Displays low-HP reactions
void ShowManaPolicy();								// Displays low-HP reactions
void ShowMercPolicy();								// Displays merc low-HP reactions
void ShowShortcutKeys();							// Displays shortcut key settings
BOOL CheckAlert(int nAlertType, BYTE iValue);		// Checks alert types
DWORD GetPotionName(DWORD dwClassID, LPSTR lpszBuffer, DWORD dwBufferLen);
int Str2Method(LPCTSTR lpsz, BOOL bAllowPotion);	// Gets a method from its string representation
void ShowHostilePolicy();							// Displays hostility reactions
void ShowHPPolicy();								// Displays low-HP reactions
void ShowManaPolicy();								// Displays low-HP reactions
void ShowMercPolicy();								// Displays merc low-HP reactions
void ShowShortcutKeys();							// Displays shortcut key settings

//////////////////////////////////////////////////////////////////////
// OnClientStart
// -------------------------------------------------------------------
// Runs *once* when the module loads. 
// This function can be removed.
//
// Put any initialization code here.
//
// Return FALSE to prevent module from loading.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{
	// Initialize ini file
	char szPathName[MAX_PATH + 1] = "";
	sprintf(szPathName, "%s\\%s", server->GetModuleDirectory(), "LifeSaver.ini");
	g_ini.SetPathName(szPathName);

	// Load hotkey settings	
	g_iQuitKey = g_ini.GetUInt("Hotkey", "Quit", VK_F9);
	g_iSyncKey = g_ini.GetUInt("Hotkey", "Sync", VK_F10);
	g_iTownKey = g_ini.GetUInt("Hotkey", "Town", VK_F11);
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStop
// -------------------------------------------------------------------
// Runs *once* before client is unloaded. 
// This function can be removed.
// 
// Put any cleanup code here.
//
// Return value should be TRUE.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStop()
{	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnUnitMessage
// -------------------------------------------------------------------
// Executes when a game unit message is sent to the client
//
// Refer to the documentation for the format of unit messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	// We only check the merc's hp percent
	if (nMessage == UM_HPPERCENT && !::memcmp(&g_pet, lpUnit, sizeof(GAMEUNIT)))
	{
		// my merc's life percent changed
		BOOL bHPDown = wParam < g_iPetHP;
		g_iPetHP = wParam;

		if (g_iPetHP && bHPDown && CheckAlert(ALERT_MERC, g_iPetHP))
		{
			// its hp going down, do some check
			if (g_iPetHP < g_iMercQuitHP)
			{
				// quit game
				g_bQuitNow = TRUE;
			}
			else if (g_iPetHP < g_iMercTownHP)
			{
				// back to town
				me->BackToTown();
				return;
			}
			else if (g_iPetHP < g_iMercPotionHP)
			{
				// feed the dude a potion
				char szPotion[128] = "";
				if (GetPotionName(me->HealMerc(), szPotion, 127))
					server->GameInfof("Fed your merc a %s successfully.", szPotion);
				else
					server->GamePrintError("Unable to feed the merc any potion!");
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// OnThisPlayerMessage
// -------------------------------------------------------------------
// Executes when a this-player message is sent to the client
//
// Refer to the documentation for the format of this-player messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == PM_GAMEREADY)
	{
		// retrieve my merc unit
		g_bGameReady = TRUE;
		me->GetMyMercUnit(&g_pet);
	}
	else if (nMessage == PM_HPDOWN)
	{
		// player's hp going down
		BYTE iPercent = me->GetHPPercent();
		if (iPercent && CheckAlert(ALERT_PLAYER_HP, iPercent))
		{
			// player's HP percent lower than alert level, do something
			if (iPercent < g_iQuitHP)
			{
				// quit game
				g_bQuitNow = TRUE;
			}
			else if (iPercent < g_iTownHP)
			{
				// back to town
				me->BackToTown();
				return;
			}
			else if (iPercent < g_iPotionHP)
			{
				// drink a potion
				char szPotion[128] = "";
				if (GetPotionName(me->HealMe(), szPotion, 127))
					server->GameInfof("Drank a %s successfully.", szPotion);
				else
					server->GamePrintError("Unable to drink any potion!");
			}
		}
	}
	else if (nMessage == PM_MANADOWN)
	{
		// player's mana going down
		BYTE iPercent = me->GetManaPercent();
		if (CheckAlert(ALERT_PLAYER_MANA, iPercent))
		{
			// player's mana percent lower than alert level, do something
			if (iPercent < g_iQuitMana)
			{
				// quit game
				g_bQuitNow = TRUE;
			}
			else if (iPercent < g_iTownMana)
			{
				// back to town
				me->BackToTown();
				return;
			}
			else if (iPercent < g_iPotionMana)
			{
				// drink a potion
				char szPotion[128] = "";
				if (GetPotionName(me->SupplyMana(TRUE), szPotion, 127))
					server->GameInfof("Drank a %s successfully.", szPotion);
				else
					server->GamePrintError("Unable to drink any potion!");
			}
		}
	}
	else if (nMessage == PM_HOSTILE)
	{
		// we just got hostiled by some mother fucker
		server->GamePrintInfo("ÿc1Hostility Detected!");
		if (me->IsInTown())
		{
			// player is in townm, do nothing
			server->GamePrintInfo("Player in town, no danger.");
		}
		else if (g_nOnHostile == ACTION_QUIT)
		{
			// quit game
			g_bQuitNow = TRUE;
		}
		else if (g_nOnHostile == ACTION_TOWN)
		{
			// back to town
			me->BackToTown();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// OnQueryModuleData
// -------------------------------------------------------------------
// Executes when other modules want to query data from this module
//
// Refer to the documentation for the format of module data querying.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnQueryModuleData(DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam)
{
	// TODO:
	// If dwDataID was undefined or you do not allow other modules to query this
	// data, simply return 0.
	// If dwDataID was defined and you are willing to allow other modules to query
	// this data, copy desired data to lpBuffer and return length of data actually
	// copied to lpBuffer.
	return 0;
}

//////////////////////////////////////////////////////////////////////
// OnGameKeyDown
// -------------------------------------------------------------------
// Executes when a key is pressed.
//
// Return zero to prevent Diablo II from receiving this key press. Other
// modules will receive the key press regardless of the return values.
//////////////////////////////////////////////////////////////////////
BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{	
	if (!g_bGameReady)
		return iKeyCode;

	// Check our hotkey settings
	if (g_iQuitKey == iKeyCode)
	{
		g_bQuitNow = TRUE;
		return 0;
	}

	if (g_iTownKey == iKeyCode)
	{
		me->BackToTown();
		return 0;
	}

	if (g_iSyncKey == iKeyCode)
	{
		me->RedrawClient(TRUE);
		return 0;
	}

	return iKeyCode;
}


//////////////////////////////////////////////////////////////////////
// OnGameJoin
// -------------------------------------------------------------------
// Executes when joining a game, and if loading the module while 
// inside a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	g_bQuitNow = FALSE;
	g_iPetHP = 0;
	g_bGameReady = FALSE;
	::memset(&g_pet, 0, sizeof(GAMEUNIT));
	LoadConfig(me->GetName());
	return;
}

//////////////////////////////////////////////////////////////////////
// OnGameLeave
// -------------------------------------------------------------------
// Executes when leaving a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameLeave(THISGAMESTRUCT* thisgame)
{
	g_bQuitNow = FALSE;
	g_bGameReady = FALSE;
	return;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeSent
// -------------------------------------------------------------------
// Executes before packets are sent from the game to the server.
// 
// If you rewrite the packet, you must make sure to return the length
// of the new packet!
//
// NOTE: This event also works in single player games.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeReceived
// -------------------------------------------------------------------
// Executes before packets are received to the game from the server.
// 
// If you rewrite the packet, you must make sure to return the length
// of the new packet!
//
// NOTE: This event also works in single player games.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}


//////////////////////////////////////////////////////////////////////
// OnGameTimerTick
// -------------------------------------------------------------------
// This gets executed apx. every 1/10th of a second when in a game.
//
// You can use this to create custom timers.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGameTimerTick(void)
{
	if (g_bQuitNow)
	{
		g_bQuitNow = FALSE;
		me->LeaveGame();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
// Module Commands
////////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	return TRUE;
}

BOOL PRIVATE OnGameCommandSync(char** argv, int argc)
{
	if (me->RedrawClient(TRUE))
		server->GamePrintInfo("Client view synchronized.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandQuit(char** argv, int argc)
{
	g_bQuitNow = TRUE;
	return TRUE;
}

BOOL PRIVATE OnGameCommandSetQuitKey(char** argv, int argc)
{
	if (argc != 3)
	{
		server->GamePrintError("Syntax: .lifesaver key.quit <key_code>");
		return TRUE;
	}

	if (!SetShortcutKey((BYTE)strtoul(argv[2], NULL, 16), KEY_QUIT))
		server->GamePrintError("Syntax: .lifesaver key.quit <key_code>");
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandSetSyncKey(char** argv, int argc)
{
	if (argc != 3)
	{
		server->GamePrintError("Syntax: .lifesaver key.sync <key_code>");
		return TRUE;
	}

	if (!SetShortcutKey((BYTE)strtoul(argv[2], NULL, 16), KEY_SYNC))
		server->GamePrintError("Syntax: .lifesaver key.quit <key_code>");

	return TRUE;
}

BOOL PRIVATE OnGameCommandSetTownKey(char** argv, int argc)
{
	if (argc != 3)
	{
		server->GamePrintError("Syntax: .lifesaver key.town <key_code>");
		return TRUE;
	}

	if (!SetShortcutKey((BYTE)strtoul(argv[2], NULL, 16), KEY_TOWN))
		server->GamePrintError("Syntax: .lifesaver key.quit <key_code>");

	return TRUE;
}

BOOL PRIVATE OnGameCommandHostile(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: .lifesaver hostile <nothing/quit/town>");
		return TRUE;
	}

	if (stricmp(argv[2], "nothing")
		&& stricmp(argv[2], "quit")
		&& stricmp(argv[2], "town"))
	{
		server->GamePrintError("Syntax: .lifesaver hostile <nothing/quit/town>");
		return TRUE;
	}

	g_nOnHostile = Str2Method(argv[2], FALSE);
	g_ini.WriteInt(me->GetName(), "On Hostile", g_nOnHostile);
    ShowHostilePolicy();
	return TRUE;
}

BOOL PRIVATE OnGameCommandHP(char** argv, int argc)
{
	if (argc < 4)
	{
		server->GamePrintError("Syntax: .lifesaver hp <quit/town/potion> <Percent>");
		return TRUE;
	}
	
	BYTE* p = NULL;
	char szEntry[32] = "";
	switch (Str2Method(argv[2], TRUE))
	{
	case ACTION_QUIT:
		p = &g_iQuitHP;
		strcpy(szEntry, "Quit HP");
		break;

	case ACTION_TOWN:
		p = &g_iTownHP;
		strcpy(szEntry, "Town HP");
		break;

	case ACTION_POTION:
		p = &g_iPotionHP;
		strcpy(szEntry, "Potion HP");
		break;

	default:
		p = NULL;
		break;
	}

	if (p == NULL)
	{
		server->GamePrintError("Syntax: .lifesaver hp <quit/town/potion> <Percent>");
		return TRUE;
	}

	*p = (BYTE)strtoul(argv[3], NULL, 10);
	g_ini.WriteUInt(me->GetName(), szEntry, *p);
	ShowHPPolicy();
	return TRUE;
}

BOOL PRIVATE OnGameCommandMana(char** argv, int argc)
{
	if (argc < 4)
	{
		server->GamePrintError("Syntax: .lifesaver mana <quit/town/potion> <Percent>");
		return TRUE;
	}
	
	BYTE* p = NULL;
	char szEntry[32] = "";
	switch (Str2Method(argv[2], TRUE))
	{
	case ACTION_QUIT:
		p = &g_iQuitMana;
		strcpy(szEntry, "Quit Mana");
		break;

	case ACTION_TOWN:
		p = &g_iTownMana;
		strcpy(szEntry, "Town Mana");
		break;

	case ACTION_POTION:
		p = &g_iPotionMana;
		strcpy(szEntry, "Potion Mana");
		break;

	default:
		p = NULL;
		break;
	}

	if (p == NULL)
	{
		server->GamePrintError("Syntax: .lifesaver mana <quit/town/potion> <Percent>");
		return TRUE;
	}

	*p = (BYTE)strtoul(argv[3], NULL, 10);
	g_ini.WriteUInt(me->GetName(), szEntry, *p);
	ShowManaPolicy();
	return TRUE;
}

BOOL PRIVATE OnGameCommandMerc(char** argv, int argc)
{
	if (argc < 4)
	{
		server->GamePrintError("Syntax: .lifesaver merc <quit/town/potion> <Percent>");
		return TRUE;
	}
	
	BYTE* p = NULL;
	char szEntry[32] = "";
	switch (Str2Method(argv[2], TRUE))
	{
	case ACTION_QUIT:
		p = &g_iMercQuitHP;
		strcpy(szEntry, "Quit Merc");
		break;

	case ACTION_TOWN:
		p = &g_iMercTownHP;
		strcpy(szEntry, "Town Merc");
		break;

	case ACTION_POTION:
		p = &g_iMercPotionHP;
		strcpy(szEntry, "Potion Merc");
		break;

	default:
		p = NULL;
		break;
	}

	if (p == NULL)
	{
		server->GamePrintError("Syntax: .lifesaver merc <quit/town/potion> <Percent>");
		return TRUE;
	}

	*p = (BYTE)strtoul(argv[3], NULL, 10);
	g_ini.WriteUInt(me->GetName(), szEntry, *p);
	ShowMercPolicy();
	return TRUE;
}

BOOL PRIVATE OnGameCommandSummary(char** argv, int argc)
{
	char sz[128] = "";
	sprintf(sz, "Settings for ÿc7%sÿc0:", me->GetName());
	server->GamePrintInfo(sz);
	server->GamePrintInfo("ÿc7-----------------------------------------------------");
	ShowHPPolicy();
	ShowManaPolicy();
	ShowHostilePolicy();
	ShowMercPolicy();
	ShowShortcutKeys();
	server->GamePrintInfo("ÿc7-----------------------------------------------------");
	return TRUE;
}

BOOL PRIVATE OnGameCommandTown(char** argv, int argc)
{
	me->BackToTown();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Other Functions
////////////////////////////////////////////////////////////////////////
void LoadConfig(LPCSTR lpszSection)
{
	if (lpszSection == NULL || !lpszSection[0])
		return;

	// Character specific Config

	// Reaction on life low
	g_iQuitHP = (BYTE)g_ini.GetUInt(lpszSection, "Quit HP", 0);
	g_iQuitHP = min(g_iQuitHP, 100);
	g_iTownHP = (BYTE)g_ini.GetUInt(lpszSection, "Town HP", 0);
	g_iTownHP = min(g_iTownHP, 100);
	g_iPotionHP = (BYTE)g_ini.GetUInt(lpszSection, "Potion HP", 0);
	g_iPotionHP = min(g_iPotionHP, 100);

	// Reaction on mana low
	g_iQuitMana = (BYTE)g_ini.GetUInt(lpszSection, "Quit Mana", 0);
	g_iQuitMana = min(g_iQuitMana, 100);
	g_iTownMana = (BYTE)g_ini.GetUInt(lpszSection, "Town Mana", 0);
	g_iTownMana = min(g_iTownMana, 100);
	g_iPotionMana = (BYTE)g_ini.GetUInt(lpszSection, "Potion Mana", 0);
	g_iPotionMana = min(g_iPotionMana, 100);

	// Reaction on merc life low
	g_iMercQuitHP = (BYTE)g_ini.GetUInt(lpszSection, "Quit Merc", 0);
	g_iMercQuitHP = min(100, g_iMercQuitHP);
	g_iMercTownHP = (BYTE)g_ini.GetUInt(lpszSection, "Town Merc", 0);
	g_iMercTownHP = min(100, g_iMercTownHP);	
	g_iMercPotionHP = (BYTE)g_ini.GetUInt(lpszSection, "Potion Merc", 0);
	g_iMercPotionHP = min(100, g_iMercPotionHP);

	// Reaction on hostility
	g_nOnHostile = g_ini.GetInt(lpszSection, "On Hostile", ACTION_NONE);
	if (g_nOnHostile != ACTION_TOWN && g_nOnHostile != ACTION_QUIT)
		g_nOnHostile = ACTION_NONE;
}

BOOL SetShortcutKey(BYTE iKeyCode, int nType)
{
	if (iKeyCode == VK_ESCAPE)
	{
		server->GamePrintError("The \"esc\" key must not be overridden!");
		return FALSE;
	}

	char szKeyType[32] = "";
	switch (nType)
	{
	case KEY_TOWN:
		g_iTownKey = iKeyCode;
		strcpy(szKeyType, "Town");
		break;

	case KEY_QUIT:
		g_iQuitKey = iKeyCode;
		strcpy(szKeyType, "Quit");
		break;

	case KEY_SYNC:
		g_iSyncKey = iKeyCode;
		strcpy(szKeyType, "Sync");
		break;

	default:
		return FALSE;
	}

	// save the new hotkey
	g_ini.WriteUInt("Hotkey", szKeyType, iKeyCode);

	// announcement
	server->GameInfof("Hotkey ÿc4%s ÿc0has been set to ÿc2%02X", szKeyType, iKeyCode);
	return TRUE;
}

BOOL CheckAlert(int nAlertType, BYTE iValue)
{
	BOOL bAlert = TRUE;
	
	if (nAlertType == ALERT_PLAYER_HP) // player HP
	{	
		if (iValue < g_iQuitHP || iValue < g_iTownHP || iValue < g_iPotionHP)
			server->GameInfof("ÿc8Player ÿc1lifeÿc8 down to ÿc1%d%%ÿc8!", iValue);
		else
			return FALSE;
	}
	else if (nAlertType == ALERT_MERC) // merc
	{
		if (iValue < g_iMercQuitHP || iValue < g_iMercTownHP || iValue < g_iMercPotionHP)
			server->GameInfof("ÿc8Merc ÿc1lifeÿc8 down to ÿc1%d%%ÿc8!", iValue);
		else
			return FALSE;
	}
	else if (nAlertType == ALERT_PLAYER_MANA) // mana
	{
		if (iValue < g_iQuitMana || iValue < g_iTownMana || iValue < g_iPotionMana)
			server->GameInfof("ÿc8Player ÿc3manaÿc8 down to ÿc1%d%%ÿc8!", iValue);
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}

	if (me->IsInTown())
	{
		server->GamePrintInfo("ÿc4Player in town, no danger.");
		bAlert = FALSE;
	}

	return bAlert;
}

DWORD GetPotionName(DWORD dwClassID, LPSTR lpszBuffer, DWORD dwBufferLen)
{	
	if (lpszBuffer == NULL || dwBufferLen == 0)
		return 0;

	::memset(lpszBuffer, 0, dwBufferLen);

	switch (dwClassID)
	{
	// Healing potions
	case 587:
		strncpy(lpszBuffer, "ÿc1Minor Healing Potionÿc0", dwBufferLen);
		break;

	case 588:
		strncpy(lpszBuffer, "ÿc1Light Healing Potionÿc0", dwBufferLen);
		break;

	case 589:
		strncpy(lpszBuffer, "ÿc1Healing Potionÿc0", dwBufferLen);
		break;

	case 590:
		strncpy(lpszBuffer, "Greater Healing Potionÿc0", dwBufferLen);
		break;

	case 591:
		strncpy(lpszBuffer, "ÿc1Super Healing Potionÿc0", dwBufferLen);
		break;

	// Mana potions
	case 592:
		strncpy(lpszBuffer, "ÿc3Minor Mana Potionÿc0", dwBufferLen);
		break;

	case 593:
		strncpy(lpszBuffer, "ÿc3Light Mana Potionÿc0", dwBufferLen);
		break;

	case 594:
		strncpy(lpszBuffer, "ÿc3Mana Potionÿc0", dwBufferLen);
		break;

	case 595:
		strncpy(lpszBuffer, "ÿc3Greater Mana Potionÿc0", dwBufferLen);
		break;

	case 596:
		strncpy(lpszBuffer, "ÿc3Super Mana Potionÿc0", dwBufferLen);
		break;

	// Rejuvenation potions
	case 515:
		strncpy(lpszBuffer, "ÿc;Rejuvenation Potionÿc0", dwBufferLen);
		break;

	case 516:
		strncpy(lpszBuffer, "ÿc;Full Rejuvenation Potionÿc0", dwBufferLen);
		break;

	default:
		return 0;
	}

	return strlen(lpszBuffer);
}

int Str2Method(LPCTSTR lpsz, BOOL bAllowPotion)
{
	if (lpsz == NULL)
		return ACTION_NONE;

	if (stricmp(lpsz, "quit") == 0)
		return ACTION_QUIT;

	if (stricmp(lpsz, "town") == 0)
		return ACTION_TOWN;

	if (bAllowPotion && stricmp(lpsz, "potion") == 0)
		return ACTION_POTION;

	return ACTION_NONE;
}

void ShowHPPolicy()
{
	if (g_iPotionHP > 0)
		server->GameInfof("HP < %d%%:    ÿc2Drink Potion", g_iPotionHP);

	if (g_iTownHP > 0)
		server->GameInfof("HP < %d%%:    ÿc2Back to Town", g_iTownHP);

	if (g_iQuitHP > 0)
		server->GameInfof("HP < %d%%:    ÿc2Quit Game", g_iQuitHP);
	
	if (g_iQuitHP ==0 && g_iTownHP == 0 && g_iPotionHP == 0)
		server->GamePrintInfo("HP Low:    ÿc1Do Nothing");
}

void ShowMercPolicy()
{
	if (g_iMercPotionHP > 0)
		server->GameInfof("Merc HP < %d%%:    ÿc2Drink Potion", g_iMercPotionHP);

	if (g_iMercTownHP > 0)
		server->GameInfof("Merc HP < %d%%:    ÿc2Back to Town", g_iMercTownHP);

	if (g_iMercQuitHP > 0)
		server->GameInfof("Merc HP < %d%%:    ÿc2Quit Game", g_iMercQuitHP);
	
	if (g_iMercQuitHP == 0 && g_iMercTownHP == 0 && g_iMercPotionHP == 0)
		server->GamePrintInfo("Merc HP Low:    ÿc1Do Nothing");
}

void ShowHostilePolicy()
{
	if (g_nOnHostile == ACTION_QUIT)
		server->GamePrintInfo("Hotiled:    ÿc2Quit game");
	else if (g_nOnHostile == ACTION_TOWN)
		server->GamePrintInfo("Hotiled:    ÿc2Back to Town");
	else
		server->GamePrintInfo("Hotiled:    ÿc1Do Nothing");
}

void ShowManaPolicy()
{
	if (g_iPotionMana > 0)
		server->GameInfof("Mana < %d%%:    ÿc2Drink Potion", g_iPotionMana);

	if (g_iTownMana > 0)
		server->GameInfof("Mana < %d%%:    ÿc2Back to Town", g_iTownMana);

	if (g_iQuitMana > 0)
		server->GameInfof("Mana < %d%%:    ÿc2Quit Game", g_iQuitMana);
	
	if (g_iQuitMana ==0 && g_iTownMana == 0 && g_iPotionMana == 0)
		server->GamePrintInfo("Mana Low:    ÿc1Do Nothing");
}

void ShowShortcutKeys()
{	
	char szKey[64] = "";
	if (server->GetKeyName(g_iQuitKey, szKey, 63))
		server->GameInfof("ÿc7Quitÿc0 shortcut key:    ÿc2%s", szKey);

	if (server->GetKeyName(g_iSyncKey, szKey, 63))
		server->GameInfof("ÿc7Syncÿc0 shortcut key:    ÿc2%s", szKey);

	if (server->GetKeyName(g_iTownKey, szKey, 63))
		server->GameInfof("ÿc7Townÿc0 shortcut key:    ÿc2%s", szKey);
}