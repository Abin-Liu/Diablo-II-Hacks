//////////////////////////////////////////////////////////////////////
// Bind.cpp
// -------------------------------------------------------------------
// The bind module used to bind keystrokes to commands. 
// Compatible with D2 1.10.
//
// Written by Abin (abinn32@yahoo.com)
//
//////////////////////////////////////////////////////////////////////
#include ".\\External\\ClientCore.cpp"
#include "KeyBind.h"
#include "Ini.h"

CKeyBind g_keyBind; // the CKeyBind object
CIni g_ini;			// bind.ini object
BOOL g_bGameJoined = FALSE;

#define DEFAULT_SECTION "Default"

#define MAX_COMMANDS_LEN	4096 // maximum command line 

// Functions as they appear in the source...
BOOL PRIVATE OnGameCommandSet(char** argv, int argc);
BOOL PRIVATE OnGameCommandUnset(char** argv, int argc);
BOOL PRIVATE OnGameCommandKeycode(char** argv, int argc);
BOOL PRIVATE OnGameCommandList(char** argv, int argc);
BOOL PRIVATE OnGameCommandClear(char** argv, int argc);
BOOL PRIVATE OnGameCommandReload(char** argv, int argc);

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
// -------------------------------------------------------------------
// 3.00 Abin - Completely rewritten, made 1.10 compatible.
// 3.01 Abin - Some minor fixes.
// 3.02 Abin - Return codes are again supported.
//////////////////////////////////////////////////////////////////////

DWORD	ModuleVersion=MAKELONG(3,2);
char	ModuleAuthor[]="Abin";
char	ModuleWebsite[]="http://wxjindu.com/abin/";
char	ModuleDescription[]="Bind module - Binds keystrokes to commands";
char	ModuleEmail[]="abinn32@yahoo.com";

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
// Functions should use following convention...
// BOOL PRIVATE OnGameCommandName(char** argv, int argc)
// ...and return TRUE on success or FALSE to show usage of the command.
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
		"set",
		OnGameCommandSet,
		"ÿc7set <keycode> <return-code> [command1;command2;...]"
	},
	
	{
		"unset",
		OnGameCommandUnset,
		"ÿc7unset <keycode>"
	},
	{
		"keycode",
		OnGameCommandKeycode,
		"ÿc7keycode"
	},
	{
		"list",
		OnGameCommandList,
		"ÿc7list"
	},
	
	{	"clear",
		OnGameCommandClear,
		"ÿc7clear"
	},

	{	"reload",
		OnGameCommandReload,
		"ÿc7reload"
	},
	
	{NULL}	// No more commands
};

BOOL CALLBACK KeyBindProc(BYTE iVkCode, LPCTSTR lpszCommand)
{
	// do nothing if not even in a game
	if (!g_bGameJoined)
		return FALSE; // abort if not in a game

	if (lpszCommand == NULL)
	{
		// just display the key code
		char szCode[32] = "";
		sprintf(szCode, "Key Code: ÿc2%02Xÿc0", iVkCode);
		server->GamePrintInfo(szCode);
	}
	else
	{
		// invoke the command
		server->GameCommandLine((LPTSTR)lpszCommand);
	}

	g_keyBind.SetShowCode(FALSE);
	return TRUE; // continue processing
}

void LoadConfig()
{
	// load settings from bind.ini
	g_keyBind.ClearAll();
	char szKey[10] = "";
	char szRetCode[4] = "";
	char szCommands[MAX_COMMANDS_LEN + 1] = "";

	for (int i = 0; i < BIND_ENTRY_COUNT; i++)
	{
		CKeyBindEntry::GetEntryNameStr((BYTE)i, szKey);
		g_ini.GetString(DEFAULT_SECTION, szKey, szCommands, MAX_COMMANDS_LEN);
		CKeyBindEntry* pEntry = g_keyBind.GetEntry((BYTE)i);
		if (pEntry != NULL)
			pEntry->ParseLineStr(szCommands);
	}	
}

BOOL EXPORT OnClientStart()
{
	// determines path of bind.ini
	char szFile[MAX_PATH + 1] = "";
	sprintf(szFile, "%s\\bind.ini", g_szDir);
	g_ini.SetPathName(szFile);

	// initialize our CKeyBind object
	g_keyBind.InstallKeyBind();
	g_keyBind.SetKeyBindProc(KeyBindProc);
	g_keyBind.SetTargetWnd(server->FindMyD2WndHandle());

	// load settings
	LoadConfig();
	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	// uninitialize our CKeyBind object
	g_keyBind.UninstallKeyBind();
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	g_keyBind.OnGameJoin();
	// set the in-game flag
	g_bGameJoined = TRUE;
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* thisgame)
{
	g_keyBind.OnGameLeave();
	// unset the in-game flag
	g_bGameJoined = FALSE;
}

BOOL PRIVATE OnGameCommandSet(char** argv, int argc)
{
	// If command line is shorter than 4 tokens, something is wrong
	if (argc < 4)
	{
		server->GamePrintError("Syntax: .bind set <key-code> <return-code> [command1[;command2;...]]");
		return TRUE;
	}
	
	// argv[2] is the vk code
	BYTE iVKCode;
	if (!CKeyBindEntry::VerifyVKCode(argv[2], 0, &iVKCode))
	{
		server->GamePrintError("Invalid key code!");
		return TRUE;
	}

	// argv[3] is the return code
	BYTE iRetCode;
	if (!CKeyBindEntry::VerifyVKCode(argv[3], 0, &iRetCode))
	{
		server->GamePrintError("Invalid return code!");
		return TRUE;
	}

	char* pszCommands = NULL;
	if (argc > 4)
	{
		// argv[4-x] are the commands
		pszCommands = new char[MAX_COMMANDS_LEN + 1];
		pszCommands[0] = 0;
		for (int i = 4; i < argc; i++)
			sprintf(pszCommands, "%s %s", pszCommands, argv[i]);
	}
	
	g_keyBind.SetEntry(iVKCode, iRetCode, pszCommands);
	if (pszCommands != NULL)
		delete [] pszCommands;
	
	const CKeyBindEntry* pEntry = g_keyBind.GetEntry(iVKCode);
	if (pEntry != NULL)
	{
		char szKey[32] = "";
		pEntry->GetEntryNameStr(szKey);

		char szInfo[MAX_COMMANDS_LEN + 100] = "";
		pEntry->GetLineStr(szInfo);	
	
		// save it	
		g_ini.WriteString(DEFAULT_SECTION, szKey, szInfo);
		pEntry->GetDisplayStr(szInfo);
		server->GamePrintInfo(szInfo);
	}	

	return TRUE;
}

BOOL PRIVATE OnGameCommandUnset(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: .bind unset <key-code>");
		return TRUE;
	}

	BOOL bSilent = argc >= 4 && !stricmp(argv[3], "silent");
	
	// argv[2] is the vk code
	BYTE iVKCode;
	if (!CKeyBindEntry::VerifyVKCode(argv[2], 0, &iVKCode))
	{
		if (!bSilent)
			server->GamePrintError("Invalid key code!");
		return TRUE;
	}

	g_keyBind.Clear(iVKCode);
	char szKey[16] = "";
	char szLine[256] = "";

	CKeyBindEntry::GetEntryNameStr(iVKCode, szKey);
	CKeyBindEntry::GetLineStr(iVKCode, NULL, szLine);	
	g_ini.WriteString(DEFAULT_SECTION, szKey, szLine);

	if (!bSilent)
	{
		sprintf(szLine, "Key code ÿc2%02Xÿc0 has been unmapped successfully.", iVKCode);
		server->GamePrintInfo(szLine);
	}

	return TRUE;
}

BOOL PRIVATE OnGameCommandKeycode(char** argv, int argc)
{
	server->GamePrintInfo("ÿc7Press a key to get its keycode...");
	g_keyBind.SetShowCode();
	return TRUE;
}

BOOL PRIVATE OnGameCommandList(char** argv, int argc)
{
	if (g_keyBind.IsAllCleared())
	{
		server->GamePrintInfo("ÿc7There are no keys currently mapped.");
		return TRUE;
	}

	server->GamePrintInfo("ÿc7Keys currently mapped by bindÿc0:");
	
	for (int i = 0; i < BIND_ENTRY_COUNT; i++)
	{
		const CKeyBindEntry* pEntry = g_keyBind.GetEntry((BYTE)i);
		if (pEntry == NULL || !pEntry->IsMapped())
			continue;

		char szLine[MAX_COMMANDS_LEN + 20] = "";
		pEntry->GetDisplayStr(szLine);
		server->GamePrintInfo(szLine);	
	}
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandClear(char** argv, int argc)
{
	g_keyBind.ClearAll();
	char szKey[16] = "";
	char szLine[16] = "";
	for (int i = 0; i < BIND_ENTRY_COUNT; i++)
	{	
		CKeyBindEntry::GetEntryNameStr((BYTE)i, szKey);
		CKeyBindEntry::GetLineStr((BYTE)i, NULL, szLine);
		g_ini.WriteString(DEFAULT_SECTION, szKey, szLine);
	}	

	server->GamePrintInfo("ÿc7All key-bindings has been removed.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandReload(char** argv, int argc)
{
	LoadConfig();
	if (argc < 3 || stricmp(argv[2], "silent"))
		server->GamePrintInfo("ÿc7Key definitions have been reloaded!");
	return TRUE;
}