//////////////////////////////////////////////////////////////////////
// test.CPP
//
// Copyright (c) 2002, Abin (Bin Liu)
// All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#include "D2hackit\\D2hackit.h"
#include "D2hackit\\D2client.h"

DWORD g_dw = 0;
RECT g_rect;

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
	::memset(&g_rect, 0, sizeof(RECT));
	WINDOWPLACEMENT wp = {0};
	GetWindowPlacement(server->FindMyD2WndHandle(), &wp);
	char sz[256] = "";
	g_rect = wp.rcNormalPosition;
	sprintf(sz, "%d %d %d %d", g_rect.left, g_rect.top, g_rect.right, g_rect.bottom);
	server->GamePrintInfo(sz);
	g_dw = 0;
}

//////////////////////////////////////////////////////////////////////
// OnGameLeave
//////////////////////////////////////////////////////////////////////
void EXPORT OnGameLeave(THISGAMESTRUCT* game)
{	
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeReceived
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{	
    return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeSent
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
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
	g_dw += 100;
	if (g_dw == 10000)
	{		
		server->GamePrintInfo("clicked");
		server->GameCommandLine("click 114 208");
		//::ShowWindow(server->FindMyD2WndHandle(), SW_SHOWNOACTIVATE);
		//::ShowWindow(server->FindMyD2WndHandle(), SW_HIDE);
		//::MoveWindow(server->FindMyD2WndHandle(), -100, -100, 1, 1, 0);
		//::ShowWindow(server->FindMyD2WndHandle(), SW_SHOW);
	}

	if (g_dw == 15000)
	{
		//::MoveWindow(server->FindMyD2WndHandle(), g_rect.left, g_rect.top, g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, 1);
		//::ShowWindow(server->FindMyD2WndHandle(), SW_SHOW);
	}
	return 0;
}
