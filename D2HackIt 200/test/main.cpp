
#include "..\\includes\\D2hackit.h"
#include "..\\includes\\D2client.h"
#include "..\\includes\\ClientCore.cpp"
#include "..\\includes\\utilities\\LinkedList.h"

CLinkedList<SIZE, SIZE, NULL> g_aList;

BOOL CALLBACK HookProc(LRESULT* pResult, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_ACTIVATEAPP && wParam == 0)
	{
		*pResult = 1;
		return 1;
	}

	*pResult = 0;
	return 0;
}

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

//////////////////////////////////////////////////////////////////////
// OnUnitMessage
// -------------------------------------------------------------------
// Executes when a game unit message is sent to the client
//
// Refer to the documentation for the format of unit messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	//if (nMessage == UM_ITEMEVENT && (wParam == ITEM_EVENT_DROP || wParam == ITEM_EVENT_NEW_GROUND))
	//{
	//	char szCode[4] = "";
	//	server->GetItemCode(lpUnit->dwUnitID, szCode, 3);
	//	g_aList.InsertRear(server->GetItemSize(szCode));
	//}
}

VOID EXPORT OnDraw(CGameDC* pDC, LPCRECT lpRect)
{
	/*
	pDC->SetBrushTransparency(-1);
	pDC->SetBrushColor(0x20);
	RECT rect;
	rect.left = 50;
	rect.top = 200;
	rect.right = 150;
	rect.bottom = 400;
	pDC->Rectangle(&rect);
	*/

	/*
	pDC->SetBrushColor(0x00);
	rect.left = 300;
	rect.top = 200;
	rect.right = 400;
	rect.bottom = 300;
	pDC->Rectangle(&rect);
	*/
}

BOOL CALLBACK EnumPlayerProc(DWORD dwPlayerID, LPCSTR lpszPlayerName, DWORD dwPlayerClass, DWORD dwPvpFlags, BYTE iMapID, LPARAM lParam)
{
	return 1;
}

BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	server->EnumPlayers(EnumPlayerProc, 0);
	return TRUE;
}