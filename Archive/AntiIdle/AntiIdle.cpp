//////////////////////////////////////////////////////////////////////
// AntiIdle.CPP
//
// Copyright (c) 2002, Abin (Bin Liu)
// All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#include "..\\D2Lib\\D2Lib.h"
#pragma comment(lib, "..\\D2lib\\D2Lib.lib") // Import D2Lib
#include "..\\D2Lib\\ClientCore.cpp"

CD2Player Me;
DWORD g_dwTotalTime = 0;
BOOL g_bInGame = FALSE;

CLIENTINFO
(		
	0,0,						// Module version (Major, Minor)
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

	// Add your own commands here

	{NULL}	// No more commands
};

//////////////////////////////////////////////////////////////////////
// OnClientStart
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStop
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStop()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameJoin
//////////////////////////////////////////////////////////////////////
void EXPORT OnGameJoin(THISGAMESTRUCT* game)
{	
	Me.OnGameJoin(game);
	g_bInGame = TRUE;
	g_dwTotalTime = 0;
}

//////////////////////////////////////////////////////////////////////
// OnGameLeave
//////////////////////////////////////////////////////////////////////
void EXPORT OnGameLeave(THISGAMESTRUCT* game)
{	
	Me.OnGameLeave();
	g_bInGame = FALSE;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeReceived
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{	
	// Just let D2Lib process the packets
	//Me.OnGamePacketBeforeReceived(aPacket, aLen);
    return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeSent
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	// Just let D2Lib process the packets
	Me.OnGamePacketBeforeSent(aPacket, aLen);
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
	// Let D2Lib process the timer first
	//Me.OnGameTimerTick();

	if (!g_bInGame)
		return 0;

	g_dwTotalTime += 100;

	if ((g_dwTotalTime % 10000) == 0)
		Me.AntiIdle();

	return 0;
}
