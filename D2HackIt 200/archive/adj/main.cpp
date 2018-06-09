
#include "..\\includes\\D2hackit.h"
#include "..\\includes\\D2client.h"
#include "..\\includes\\ClientCore.cpp"

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
		"test",
		OnCommandDebug,
		"debugÿc6 Internal usage.\n"
	},

	// Add your own commands here

	{NULL}	// No more commands
};

BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	if (argc < 4)
	{
		server->GamePrintError("syntax: .adj test <cx cy>");
		return TRUE;
	}

	SIZE cz = { atoi(argv[2]), atoi(argv[3]) };
	if (cz.cx < 1 || cz.cx > 2 || cz.cy < 1 || cz.cy > 4)
	{
		server->GameErrorf("Invalid size: %d, %d", cz.cx, cz.cy);
		return TRUE;
	}
	
	POINT pt;
	me->FindFirstStorageSpace(STORAGE_CUBE, cz, &pt);
	server->GameInfof("Best fit location: %d, %d", pt.x, pt.y);
	return TRUE;
}