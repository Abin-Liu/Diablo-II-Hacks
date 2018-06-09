//////////////////////////////////////////////////////////////////////
// DllMain.cpp
// -------------------------------------------------------------------
// Default Dll entrypoint.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Server20.h"
#include "Offsets.h"

HWND g_hD2Wnd = NULL;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam);

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		g_hD2Wnd = NULL;
		::EnumWindows(EnumWindowsProc, 0);

		if (g_hD2Wnd)
		{
			DefineOffsets();
			Server20Start();
		}

		break;

	case DLL_PROCESS_DETACH:

		if (g_hD2Wnd)
		{
			Server20Stop();		
		}

		break;
    } 

    return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char classname[20];

	GetClassName(hwnd, classname, sizeof(classname));

	if(!stricmp(classname, "Diablo II"))
	{
		DWORD pid, tid;
		tid = GetWindowThreadProcessId( hwnd, &pid );
		if( pid == GetCurrentProcessId() )
		{
			g_hD2Wnd = hwnd;
			return FALSE;	// Stop enum
		}
	}

	return TRUE;	// Continue enum
}
