/////////////////////////////////////////////////////////////////////////
// Main.cpp
//-----------------------------------------------------------------------
// Client implementation for GetIt.d2h, a module for D2Hackit v2.00 that
// automatically picks up items and moves items from inventory to stash
// or cube.
//-----------------------------------------------------------------------
// History:
//
// 1.0 - Initial public release.
// 2.0 - Added ".getit stash" command.
// 3.0 - Changed many formats...Removed gold, rejuv, fullrejuv,goldamount commands.
// 4.0 - Added support to "common" expression entries.
//-----------------------------------------------------------------------
// Written by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////

#include "ItemPicker.h"
#include "ItemMover.h"
#include "..\\Includes\\ClientCore.cpp"

// globals
CGetItConfigLib g_lib; // Configuration library
CItemPicker g_picker; // For picking up
CItemMover g_mover; // For moving items from inventory to stash/cube

// module info
DWORD	ModuleVersion=MAKELONG(4,0);
char	ModuleAuthor[]="Abin";
char	ModuleWebsite[]="www.wxjindu.com/abin";
char	ModuleDescription[]="GetIt for D2Hackit 2.00";
char	ModuleEmail[]="abinn32@yahoo.com";

// function definations
BOOL PRIVATE OnCommandDebug(char **argv, int argc);
BOOL PRIVATE OnCommandRadius(char **argv, int argc);
BOOL PRIVATE OnCommandToggle(char **argv, int argc);
BOOL PRIVATE OnCommandClientMove(char **argv, int argc);
BOOL PRIVATE OnCommandLoad(char** argv, int argc);
BOOL PRIVATE OnCommandEnable(char** argv, int argc);
BOOL PRIVATE OnCommandDisable(char** argv, int argc);
BOOL PRIVATE OnCommandSummary(char** argv, int argc);
BOOL PRIVATE OnCommandLog(char** argv, int argc);
BOOL PRIVATE OnCommandFullCmds(char** argv, int argc);
BOOL PRIVATE OnCommandTownPick(char** argv, int argc);
BOOL PRIVATE OnCommandDelay(char** argv, int argc);
BOOL PRIVATE OnCommandStash(char** argv, int argc);
BOOL PRIVATE OnCommandUseCube(char** argv, int argc);
BOOL PRIVATE OnCommandMoveColumns(char** argv, int argc);
BOOL PRIVATE OnCommandMoveCmds(char** argv, int argc);

// module commands
MODULECOMMANDSTRUCT ModuleCommands[]= {

	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	{	
		"debug",
		OnCommandDebug,
		"ÿc4debug ÿc0 toggle debug mode.\n"
	},
	{
		"radius",
		OnCommandRadius,
		"ÿc4radius ÿc0<radius>"
	},
	{
		"toggle",
		OnCommandToggle,
		"ÿc4toggle"
	},
	{
		"enable",
		OnCommandEnable,
		"ÿc4enable"
	},	
	{
		"disable",
		OnCommandDisable,
		"ÿc4disable"
	},	
	{
		"townpick",
		OnCommandTownPick,
		"ÿc4townpick disable"
	},
	{
		"clientmove",
		OnCommandClientMove,
		"ÿc4clientmove"
	},	
	{
		"summary",
		OnCommandSummary,
		"ÿc4summary"
	},
	{
		"load",
		OnCommandLoad,
		"ÿc4loadÿc0 <file-name>"
	},
	{
		"log",
		OnCommandLog,
		"ÿc4logÿc0 [file-name]"
	},

	{
		"fullcmds",
		OnCommandFullCmds,
		"ÿc4fullcmdsÿc0 [command1;command2;command3;...;commandn]"
	},

	{
		"delay",
		OnCommandDelay,
		"ÿc4delayÿc0 <milliseconds>"
	},

	{
		"stash",
		OnCommandStash,
		"ÿc4stash"
	},

	{
		"movecmds",
		OnCommandMoveCmds,
		"ÿc4movecmdsÿc0 [command1;command2;command3;...;commandn]"
	},

	{
		"usecube",
		OnCommandUseCube,
		"ÿc4usecube <0/1>"
	},

	{
		"movecol",
		OnCommandMoveColumns,
		"ÿc4movecol leftcol rightcol"
	},


	{NULL}
};


//////////////////////////////////////////////////////////////////////
// OnUnitMessage - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when a game unit message is sent to the client
//
// Refer to the documentation for the format of unit messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	g_picker.OnUnitMessage(nMessage, lpUnit, wParam, lParam);
	g_mover.OnUnitMessage(nMessage, lpUnit, wParam, lParam);
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
	g_picker.OnThisPlayerMessage(nMessage, wParam, lParam);
	g_mover.OnThisPlayerMessage(nMessage, wParam, lParam);	
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
	// We define data ID "1" for querying number of items that need to be pickied
	// lParam 0: Query number of all items (Valuable items + junky items).
	// lParam 1: Query number of valuable items only.
	if (dwDataID != 1 || !lpBuffer || !dwMaxLength)
		return 0;

	DWORD dwCount = g_picker.GetQueueItemCount(lParam, TRUE);
	DWORD dwCopied = min(4, dwMaxLength);
	::memcpy(lpBuffer, &dwCount, dwCopied);
	return dwCopied;
}

BOOL EXPORT OnClientStart()
{
	g_lib.Load(NULL);
	g_picker.SetConfigLib(&g_lib);
	g_mover.SetConfigLib(&g_lib);	
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	g_picker.OnGameJoin();
	g_mover.OnGameJoin();
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* aThisgame)
{
	g_picker.OnGameLeave();
	g_mover.OnGameLeave();
}

// functions related to the above commands
BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	g_picker.DebugQueue();
	return TRUE;
}

BOOL PRIVATE OnCommandClientMove(char **argv, int argc)
{
	if (argc < 3)
		server->GamePrintError("ÿc4Syntax: .getit clientmove <0/1>");
	else
		g_lib.SetClientMove(atoi(argv[2]));
	return TRUE;
}

BOOL PRIVATE OnCommandRadius(char **argv, int argc)
{
	if (argc < 3)
		server->GamePrintError("ÿc4Syntax: .getit radius <radius>");
	else
		g_lib.SetRadius(atoi(argv[2]));
	return TRUE;
}

BOOL PRIVATE OnCommandToggle(char **argv, int argc)
{	
	if (g_picker.Toggle())
		server->GamePrintInfo("ÿc4Pickit enabled.");
	else
		server->GamePrintInfo("ÿc4Pickit disabled.");
	return TRUE;
}

BOOL PRIVATE OnCommandEnable(char **argv, int argc)
{
	g_picker.Enable(TRUE);
	server->GamePrintInfo("ÿc4Pickit enabled.");
	return TRUE;
}

BOOL PRIVATE OnCommandDisable(char** argv, int argc)
{
	g_picker.Enable(FALSE);
	server->GamePrintInfo("ÿc4Pickit disabled.");
	return TRUE;
}

BOOL PRIVATE OnCommandLoad(char** argv, int argc) {

	// get arguments 2-x as the target file
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .getit load <file name>");
		return TRUE;
	}

	char szFile[_MAX_PATH + 1] = "";
	sprintf(szFile, "%s\\", server->GetModuleDirectory());
	for (int i = 2; i < argc; i++)
		strcat(szFile, argv[i]);

	g_lib.Load(szFile);		
	return TRUE;
}

BOOL PRIVATE OnCommandSummary(char** argv, int argc)
{
	g_lib.DisplaySummary();
	return TRUE;
}

BOOL PRIVATE OnCommandLog(char** argv, int argc)
{
	char szLogFile[MAX_PATH + 1] = "";
	if (argc > 2)
	{
		sprintf(szLogFile, "%s\\", server->GetModuleDirectory());
		for (int i = 2; i < argc; i++)
			sprintf(szLogFile, "%s%s ", szLogFile, argv[i]);
	}

	g_lib.SetLogFile(szLogFile);
	return TRUE;
}

BOOL PRIVATE OnCommandFullCmds(char** argv, int argc)
{
	char szCmds[1025] = "";
	if (argc > 2)
	{
		for (int i = 2; i < argc; i++)
		sprintf(szCmds, "%s%s ", szCmds, argv[i]);
	}
	g_lib.SetFullCallback(szCmds);
	return TRUE;
}

BOOL PRIVATE OnCommandTownPick(char** argv, int argc)
{
	if (argc < 3)
		server->GamePrintError("ÿc4Syntax: .getit townpick <0/1>");
	else
		g_lib.SetTownPick(atoi(argv[2]));

	return TRUE;
}

BOOL PRIVATE OnCommandDelay(char** argv, int argc)
{
	if (argc < 3)
		server->GamePrintError("ÿc4Syntax: .getit delay <delay-milliseconds>");
	else
		g_lib.SetDelay(atoi(argv[2]));

	return TRUE;
}

BOOL PRIVATE OnCommandStash(char** argv, int argc)
{
	g_mover.Start();
	return TRUE;
}

BOOL PRIVATE OnCommandUseCube(char** argv, int argc)
{
	if (argc < 3)
		server->GamePrintError("ÿc4Syntax: .getit usecube <0/1>");
	else
		g_lib.SetUseCube(atoi(argv[2]));

	return TRUE;
}

BOOL PRIVATE OnCommandMoveColumns(char** argv, int argc)
{
	if (argc < 4)
		server->GamePrintError("ÿc4Syntax: .getit movecol <leftcol> <rightcol>");
	else
		g_lib.SetMoveColumns(atoi(argv[2]), atoi(argv[3]));

	return TRUE;
}

BOOL PRIVATE OnCommandMoveCmds(char** argv, int argc)
{
	char szCmds[1025] = "";
	if (argc > 2)
	{
		for (int i = 2; i < argc; i++)
		sprintf(szCmds, "%s%s ", szCmds, argv[i]);
	}
	g_lib.SetStashCallback(szCmds);
	return TRUE;
}

DWORD EXPORT OnGameTimerTick(void)
{
	if (server->IsGameReady())
	{
		g_picker.OnGameTimerTick();
		g_mover.OnGameTimerTick();
	}
	
	return 0;
}
