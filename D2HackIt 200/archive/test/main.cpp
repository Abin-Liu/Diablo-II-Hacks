
#include "..\\includes\\D2hackit.h"
#include "..\\includes\\D2client.h"
#include "..\\includes\\ClientCore.cpp"
#include "..\\includes\\utilities\\LinkedList.h"

CLinkedList<SIZE, SIZE, NULL> g_aList;

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
		"calc",
		OnCommandDebug,
		"debugÿc6 Internal usage.\n"
	},

	// Add your own commands here

	{NULL}	// No more commands
};

//////////////////////////////////////////////////////////////////////
// OnUnitMessage
// -------------------------------------------------------------------
// Executes when a game unit message is sent to the client
//
// Refer to the documentation for the format of unit messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == UM_ITEMEVENT && (wParam == ITEM_EVENT_DROP || wParam == ITEM_EVENT_NEW_GROUND))
	{
		char szCode[4] = "";
		server->GetItemCode(lpUnit->dwUnitID, szCode, 3);
		g_aList.InsertRear(server->GetItemSize(szCode));
	}
}

BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	int nTotal = g_aList.GetItemCount();
	if (nTotal == 0)
	{
		server->GamePrintInfo("No items recorded!");
		return TRUE;
	}
	
	int aCounts[2][4] = { 0 };	

	POSITION pos = g_aList.GetHeadPosition();
	while (pos)
	{
		const SIZE& c = g_aList.GetNext(pos);
		if (c.cx == 1 && c.cy == 1)
		{
			aCounts[0][0]++;
		}
		else if (c.cx == 1 && c.cy == 2)
		{
			aCounts[0][1]++;
		}
		else if (c.cx == 1 && c.cy == 3)
		{
			aCounts[0][2]++;
		}
		else if (c.cx == 1 && c.cy == 4)
		{
			aCounts[0][3]++;
		}
		else if (c.cx == 2 && c.cy == 1)
		{
			aCounts[1][0]++;
		}
		else if (c.cx == 2 && c.cy == 2)
		{
			aCounts[1][1]++;
		}
		else if (c.cx == 2 && c.cy == 3)
		{
			aCounts[1][2]++;
		}
		else if (c.cx == 2 && c.cy == 4)
		{
			aCounts[1][3]++;
		}
	}

	server->GamePrintInfo("---------------------------------------");
	server->GamePrintInfo("Counting Results:");
	server->GamePrintInfo("---------------------------------------");

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
			server->GameInfof("%d*%d - %d/%d (%.2f%%)", i + 1, j + 1, aCounts[i][j], nTotal, (double)aCounts[i][j] / (double)nTotal * 100.0);
	}

	server->GamePrintInfo("---------------------------------------");
	return TRUE;
}