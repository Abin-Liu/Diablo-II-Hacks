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
// <abinn32@yahoo.com>
//////////////////////////////////////////////////////////////////////
#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include "..\\Includes\\ClientCore.cpp"	  // Include the backend of the module


//////////////////////////////////////////////////////////////////////
// Global stuff.
// -------------------------------------------------------------------
// This is a very convenient place to your function declarations and
// global variables.
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
// -------------------------------------------------------------------
// Tell us a little about this module.
//////////////////////////////////////////////////////////////////////
CLIENTINFO
(		
	0,0,							// Module version (Major, Minor)
	"thohell",						// Author
	"thohell.d2network.com",		// url (http:// is appended)
	"Sample plugin",				// Short module description
	"thohell@home.se"				// Author eMail
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
// "commandname <required parameter> [optional parameter]�c0 Description"
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
		"help�c0 List commands available in this module.\n"
		"<command> help�c0 Shows detailed help for <command> in this module."
	},

	{
		// Debug
		"debug",
		OnGameCommandDebug,
		"debug�c0 Debug your module."
	},

	// Add your own commands here

	{NULL}	// No more commands
};

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	// Put debug code here...
	return TRUE;
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
// EVENTS
// -------------------------------------------------------------------
// The rest of this file are event functions, and they can safely be
// removed if the module makes no use of them.
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// OnUnitMessage - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when a game unit message is sent to the client
//
// Refer to the documentation for the format of unit messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
}

//////////////////////////////////////////////////////////////////////
// OnThisPlayerMessage - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when a this-player message is sent to the client
//
// Refer to the documentation for the format of this-player messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
}

//////////////////////////////////////////////////////////////////////
// OnQueryModuleData - Added since v2.00 by Abin
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
// OnGameKeyDown - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when a key is pressed.
//
// Return zero to prevent Diablo II from receiving this key press. Other
// modules will receive the key press regardless of the return values.
//////////////////////////////////////////////////////////////////////
BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	return iKeyCode;
}

//////////////////////////////////////////////////////////////////////
// OnDraw - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when the game screen needs to be drawn.
//
// Parameters:
// 
// pDC - Pointer to a game device context structure, use member functions
//       of this structure to draw your own items, for example:
//       pDC->MoveTo(0, 0); pDC->LineTo(400, 300, 0x62);
//       will draw a red line from (0,0) to (400,300).
// lpScreenRect - Pointer to the client screen rect boundary.
//
// Check D2Hackit API document for more details.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnDraw(CGameDC* pDC, LPCRECT lpScreenRect)
{
	// TODO: Draw your own texts/graphs on the screen.
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
	return 0;
}
