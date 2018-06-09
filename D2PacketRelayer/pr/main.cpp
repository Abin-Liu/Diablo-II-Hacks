//////////////////////////////////////////////////////////////////////
// ClientImplementation.cpp
// -------------------------------------------------------------------
// This is the file to change when writing new modules. No other code
// should be needed!
//
// The modules are event based, and the functions they export will be
// called by the D2HackIt! dll at appropriate times. Any events you do
// not need can safely be deleted from this file, as D2HackIt! only
// calls the events it can bind.
//
// 
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#include "external\\D2Hackit.h"
#include "external\\D2Client.h"
#include "external\\ClientCore.cpp"	  // Include the backend of the module
#include "..\\MessageDef.h"
#include "SharedMem.h"
#include "D2PacketFilter.h"

//////////////////////////////////////////////////////////////////////
// Global stuff.
// -------------------------------------------------------------------
// This is a very convenient place to your function declarations and
// global variables.
//////////////////////////////////////////////////////////////////////
#define MEM_FILTER_SIZE				256
#define MEM_SHOW_NONE				0x00
#define MEM_SHOW_RCV				0x01
#define MEM_SHOW_SNT				0x02

THISGAMESTRUCT* thisgame = NULL; // The game struct
CSharedMem g_mem;

DWORD g_dwPacketNo = 0;
CD2PacketFilter g_filter;
BOOL g_bPaused = FALSE;
BOOL g_bInGame = FALSE;

BYTE g_iAct = 0;
DWORD g_dwPlayerLocation = 0;
WORD g_wPlayerX = 0;
WORD g_wPlayerY = 0;

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
	"Packet Relayer",				// Short module description
	"abinn32@yahoo.com"				// Author eMail
)

BOOL PRIVATE OnGameCommandShowSend(char** argv, int argc);
BOOL PRIVATE OnGameCommandShowReceive(char** argv, int argc);
BOOL PRIVATE OnGameCommandHideSend(char** argv, int argc);
BOOL PRIVATE OnGameCommandHideReceive(char** argv, int argc);
BOOL PRIVATE OnGameCommandClear(char** argv, int argc);
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);
BOOL PRIVATE OnGameCommandToggle(char** argv, int argc);
BOOL PRIVATE OnGameCommandInfo(char** argv, int argc);

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
		"helpÿc6 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	{
		"show.s",
		OnGameCommandShowSend,
		"show.s ÿc0Show packets that are being sent.\nSyntax: pr show.s <all>\npr show.s head-byte <feature-byte1 feature-byte2 ...>."
	},

	{
		"show.r",
		OnGameCommandShowReceive,
		"show.r ÿc0Show packets that are being received.\nSyntax: pr show.r <all>\npr show.r head-byte <feature-byte1 feature-byte2 ...>."
	},

	{
		"hide.s",
		OnGameCommandHideSend,
		"hide.s ÿc0Hide packets that are being sent.\nSyntax: pr hide.s <all>\npr hide.s head-byte <feature-byte1 feature-byte2 ...>."
	},

	{
		"hide.r",
		OnGameCommandHideReceive,
		"hide.r ÿc0Hide packets that are being received.\nSyntax: pr hide.r <all>\npr hide.r head-byte <feature-byte1 feature-byte2 ...>."
	},

	{
		"clear",
		OnGameCommandClear,
		"clear ÿc0Clear all defined packets.\nSyntax: pr clear."
	},

	{
		"toggle",
		OnGameCommandToggle,
		"toggle."
	},

	{
		"info",
		OnGameCommandInfo,
		"info."
	},

	{
		"debug",
		OnGameCommandDebug,
		"debug ÿc0for debug purpose only."
	},

	// Add your own commands here

	{NULL}	// No more commands
};

BOOL SendPacketMessage(const BYTE* aPacket, DWORD aLen, BOOL bReceived)
{
	if (!g_mem.IsCreated() || !aPacket || !aLen)
		return FALSE;

	// Notify each window
	for (DWORD i = 0; i < MAX_HWND; i++)
	{
		HWND hWnd = (HWND)g_mem.GetLong(MEM_POS_HWND + i * 4);
		if (!::IsWindow(hWnd))
			continue;

		::PostMessage(hWnd, WM_PACKET_START, (WPARAM)g_dwPacketNo, MAKELPARAM((WORD)bReceived, (WORD)aLen));
		
		DWORD dwBytesRemain = aLen;
		const BYTE* p = aPacket;
		while (dwBytesRemain > 0)
		{
			WPARAM wParam = 0;
			LPARAM lParam = 0;

			DWORD dwCopied = min(dwBytesRemain, 4);
			if (dwCopied > 0)
			{
				::memcpy(&wParam, p, dwCopied);
				p += dwCopied;
				dwBytesRemain -= dwCopied;
			}

			dwCopied = min(dwBytesRemain, 4);
			if (dwCopied > 0)
			{
				::memcpy(&lParam, p, dwCopied);
				p += dwCopied;
				dwBytesRemain -= dwCopied;
			}

			::PostMessage(hWnd, WM_PACKET_BODY, wParam, lParam);
		}

		::PostMessage(hWnd, WM_PACKET_END, (WPARAM)g_dwPacketNo, MAKELPARAM((WORD)bReceived, (WORD)aLen));
	}

	return TRUE;
}

void SendGameJoinLeaveMessage(BOOL bIsJoin, DWORD dwPlayerID = 0, BYTE iCharClass = 0, LPCTSTR lpszPlayerName = NULL)
{
	if (!g_mem.IsCreated())
		return;

	g_mem.SetLong(0, MEM_POS_MERCID);
	g_mem.SetByte(0, MEM_POS_MERCALIVE);

	g_mem.SetLong((long)dwPlayerID, MEM_POS_PLAYERID);
	g_mem.SetByte(iCharClass, MEM_POS_CHARCLASS);

	BYTE aDemmy[MAX_PLAYERNAME + 1] = { 0 };
	g_mem.SetData((const char*)aDemmy, MAX_PLAYERNAME + 1, MEM_POS_PLAYERNAME);

	if (lpszPlayerName)
		g_mem.SetData((const char*)lpszPlayerName, min(MAX_PLAYERNAME, strlen(lpszPlayerName)), MEM_POS_PLAYERNAME);

	for (DWORD i = 0; i < MAX_HWND; i++)
	{
		HWND hWnd = (HWND)g_mem.GetLong(MEM_POS_HWND + i * 4);
		if (!::IsWindow(hWnd))
			continue;

		::PostMessage(hWnd, bIsJoin ? WM_GAMEJOIN : WM_GAMELEAVE, (WPARAM)iCharClass, (LPARAM)dwPlayerID);
	}

	g_mem.SetByte(bIsJoin ? 1 : 0, MEM_POS_GAME);
}

void PrintPacket(BYTE* aPacket, int aLen, BOOL bReceived)
{
	char sz[1024] = "";
	sprintf(sz, "ÿc4%s ÿc2%d:ÿc0", bReceived? "R" : "S", aLen);

	for (int i = 0; i < aLen; i++)
	{
		char szByte[5] = "";
		sprintf(szByte, " %02X ", aPacket[i]);
		strcat(sz, szByte);
	}
	server->GamePrintInfo(sz);
}

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
	g_mem.Create(MEM_ID, MEM_SIZE, 100);
	g_filter.BlockAll(TRUE);
	g_filter.BlockAll(FALSE);
	
	if (g_mem.IsCreated())
	{
		for (DWORD i = 0; i < MAX_HWND; i++)
		{
			HWND hWnd = (HWND)g_mem.GetLong(MEM_POS_HWND + i * 4);
			if (!::IsWindow(hWnd))
				continue;

			::PostMessage(hWnd, WM_CLIENT_START, (WPARAM)0, (LPARAM)0);
		}

		g_mem.SetByte(1, MEM_POS_CLIENT);
	}
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
	if (g_mem.IsCreated())
	{
		for (DWORD i = 0; i < MAX_HWND; i++)
		{
			HWND hWnd = (HWND)g_mem.GetLong(MEM_POS_HWND + i * 4);
			if (!::IsWindow(hWnd))
				continue;

			::PostMessage(hWnd, WM_CLIENT_STOP, (WPARAM)0, (LPARAM)0);
		}

		g_mem.SetByte(0, MEM_POS_CLIENT);
	}

	g_mem.Close();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameJoin
// -------------------------------------------------------------------
// Executes when joining a game, and if loading the module while 
// inside a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	thisgame = game;
	g_bInGame = TRUE;
	g_iAct = 0;
	g_dwPlayerLocation = 0;
	g_wPlayerX = 0;
	g_wPlayerY = 0;
	SendGameJoinLeaveMessage(TRUE, game->player->PlayerID, game->player->CharacterClass, game->player->PlayerName);
	return;
}

//////////////////////////////////////////////////////////////////////
// OnGameLeave
// -------------------------------------------------------------------
// Executes when leaving a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameLeave(THISGAMESTRUCT* game)
{
	g_bInGame = FALSE;
	SendGameJoinLeaveMessage(FALSE);
	g_dwPacketNo = 0;
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
	g_dwPacketNo++;
	SendPacketMessage(aPacket, aLen, FALSE);
	if (g_filter.IsAllowed(aPacket[0], FALSE))
		PrintPacket(aPacket, aLen, FALSE);	
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
	g_dwPacketNo++;
	SendPacketMessage(aPacket, aLen, TRUE);
	if (g_filter.IsAllowed(aPacket[0], TRUE))
		PrintPacket(aPacket, aLen, TRUE);	
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
	if (!g_bInGame || !thisgame || !thisgame->player || !g_mem.IsCreated())
		return 0;

	if (g_iAct == thisgame->player->Act && g_dwPlayerLocation == thisgame->player->PlayerLocation && g_wPlayerX == thisgame->player->PlayerPositionX && g_wPlayerY == thisgame->player->PlayerPositionY)
		return 0; // no need to update

	g_iAct = thisgame->player->Act;
	g_dwPlayerLocation = thisgame->player->PlayerLocation;
	g_wPlayerX = thisgame->player->PlayerPositionX;
	g_wPlayerY = thisgame->player->PlayerPositionY;
	for (DWORD i = 0; i < MAX_HWND; i++)
	{
		HWND hWnd = (HWND)g_mem.GetLong(MEM_POS_HWND + i * 4);
		if (!::IsWindow(hWnd))
			continue;

		::PostMessage(hWnd, WM_PLAYERLOCATION, MAKEWPARAM(g_iAct, (WORD)g_dwPlayerLocation), MAKELPARAM(g_wPlayerX, g_wPlayerY));
	}

	return 0;
}

BOOL PRIVATE OnGameCommandShowSend(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc1Syntax: ÿc4.pr show.s <message-id/all>");
		return TRUE;
	}
	
	if (stricmp(argv[2], "all") == 0)
	{
		g_filter.AllowAll(FALSE);
		server->GamePrintInfo("ÿc4All sent messages will be shown.");
		return TRUE;
	}

	BYTE iID = (BYTE)strtoul(argv[2], NULL, 16);
	g_filter.Allow(iID, FALSE);

	char szMessage[128] = "";
	sprintf(szMessage, "ÿc4(S)Message ÿc2%02Xÿc4 will be shown.", iID);
	server->GamePrintInfo(szMessage);

	return TRUE;
}

BOOL PRIVATE OnGameCommandShowReceive(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc1Syntax: ÿc4.pr show.r <message-id/all>");
		return TRUE;
	}
	
	if (stricmp(argv[2], "all") == 0)
	{
		g_filter.AllowAll(TRUE);
		server->GamePrintInfo("ÿc4All received messages will be shown.");
		return TRUE;
	}

	BYTE iID = (BYTE)strtoul(argv[2], NULL, 16);
	g_filter.Allow(iID, TRUE);

	char szMessage[128] = "";
	sprintf(szMessage, "ÿc4(R)Message ÿc2%02Xÿc4 will be shown.", iID);
	server->GamePrintInfo(szMessage);
	return TRUE;
}

BOOL PRIVATE OnGameCommandHideSend(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc1Syntax: ÿc4.pr hide.s <message-id/all>");
		return TRUE;
	}
	
	if (stricmp(argv[2], "all") == 0)
	{
		g_filter.BlockAll(FALSE);
		server->GamePrintInfo("ÿc4All sent messages will be hidden.");
		return TRUE;
	}

	BYTE iID = (BYTE)strtoul(argv[2], NULL, 16);
	g_filter.Block(iID, FALSE);

	char szMessage[128] = "";
	sprintf(szMessage, "ÿc4(S)Message ÿc2%02Xÿc4 will be hidden.", iID);
	server->GamePrintInfo(szMessage);
	return TRUE;
}

BOOL PRIVATE OnGameCommandHideReceive(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc1Syntax: ÿc4.pr hide.r <message-id/all>");
		return TRUE;
	}
	
	if (stricmp(argv[2], "all") == 0)
	{
		g_filter.BlockAll(TRUE);
		server->GamePrintInfo("ÿc4All received messages will be hidden.");
		return TRUE;
	}

	BYTE iID = (BYTE)strtoul(argv[2], NULL, 16);
	g_filter.Block(iID, TRUE);

	char szMessage[128] = "";
	sprintf(szMessage, "ÿc4(R)Message ÿc2%02Xÿc4 will be hidden.", iID);
	server->GamePrintInfo(szMessage);
	return TRUE;
}

BOOL PRIVATE OnGameCommandClear(char** argv, int argc)
{
	g_filter.BlockAll(TRUE);
	g_filter.BlockAll(FALSE);
	server->GamePrintInfo("ÿc4Filter cleared, no message will be shown.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandToggle(char** argv, int argc)
{
	g_bPaused = !g_bPaused;
	
	if (g_bPaused)
		server->GamePrintInfo("ÿc4pr paused.");
	else
		server->GamePrintInfo("ÿc4pr resumed.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandInfo(char** argv, int argc)
{
	char szMessage[256] = "";
	sprintf(szMessage, "PID %08X, XY %04X,%04X, Act %d, Location %d, UTYP %d, UNK1 %d",
		thisgame->player->PlayerID,
		thisgame->player->PlayerPositionX,
		thisgame->player->PlayerPositionY,
		thisgame->player->Act,
		thisgame->player->PlayerLocation,
		thisgame->player->UnitType,
		thisgame->player->unknown1
		);
	server->GamePrintInfo(szMessage);
	//PrintPacket(thisgame->player->unknown2, 0x73, 1);
	return TRUE;
}

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	if (argc < 6)
		return TRUE;

	//WORD wPlayerX = GetPositionX();
	//WORD wPlayerY = GetPositionY();
	
	BYTE aPacket[11] = { 0 };
	//aPacket[10] = 0x01;
	aPacket[0] = 0x15;
	::memcpy(aPacket + 2, &thisgame->player->PlayerID, 4);
	for (int i = 0; i < 4; i++)
		aPacket[6 + i] = (BYTE)strtoul(argv[2 + i], NULL, 16);
	//::memcpy(aPacket + 6, &wPlayerX, 2);
	//::memcpy(aPacket + 8, &wPlayerY, 2);

	server->GameSendPacketToGame(aPacket, 11);
	return TRUE;
}

