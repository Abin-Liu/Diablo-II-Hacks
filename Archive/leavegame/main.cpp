
#include "..\\D2lib\\D2Lib.h"
#include "..\\D2Lib\\ClientCore.cpp"
#include <shellapi.h>

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
	server->GameSaveAndExit();	
	return;
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* game)
{
	char sz[256] = "";
	sprintf(sz, "%s\\loader.exe", GetModuleDirectory());
	::ShellExecute(NULL, "open", sz, NULL, GetModuleDirectory(), SW_HIDE);
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
	return TRUE;
}