
#include "external\\D2hackit.h"
#include "external\\D2client.h"
#include "..\\D2Lib\\ClientCore.cpp"
#include "D2TextDraw.h"

CD2TextDraw draw;

CLIENTINFO
(		
	0,0,						// Module version (Major, Minor)
	"Abin",						// Author
	"www.wxjindu.com/abin/",	// url (http:// is appended)
	"Sample plugin",			// Short module description
	"abinn32@yahoo.com"			// Author eMail
)

BOOL PRIVATE OnCommandDebug(char **argv, int argc);
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
		OnCommandDebug,
		"debugÿc6 Internal usage.\n"
	},

	// Add your own commands here

	{NULL}	// No more commands
};


BOOL EXPORT OnClientStart()
{
	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	
	server->GamePrintInfo("Game Joined");

	return;
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* game)
{	
	draw.Undraw();
	return;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}


DWORD EXPORT OnGameTimerTick(void)
{
	return 0;
}

BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	/*
	int nFont = 0;
	if (argc > 2)
		nFont = atoi(argv[2]);

	//draw.DrawSingleText("fuck this", 300, 300, D2T_YELLOW, nFont);
	D2DRAWDATA dd[14] = {0};
	long y = 50;

	for (int i = 0; i < 14; i++)
	{
		dd[i].bRightAlign = 1;
		dd[i].nFont = i;
		dd[i].x = 300;
		dd[i].y = y + i * 30;
		sprintf(dd[i].szText, "Kasim, %d", i);
		//strcpy(dd[i].szText, "Kasim");
	}

	draw.DrawMultipleTexts(dd, 14);
	*/
	char sz[256] = "";
	SIZE cz = draw.GetScreenSize();
	sprintf(sz, "wnd %08X, screen size: %d * %d", (DWORD)draw.GetD2WndHandle(), cz.cx, cz.cy);
	server->GamePrintInfo(sz);

	return TRUE;
}