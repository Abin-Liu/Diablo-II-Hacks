//////////////////////////////////////////////////////////////////////
// DllMain.cpp
// -------------------------------------------------------------------
// Default Dll entrypoint.
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include "Server20.h"
#include "Offsets.h"

static const LPCSTR	NeededDlls[] = { "D2Common.dll", "D2Game.dll", "D2Client.dll", "D2Win.dll", "D2Gfx.dll", NULL };

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	int i = 0;

    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		//////////////////////////////////////////////////////////////////
		// Force-load needed dll's so we can patch their memory space.
		// We should unload these in DLL_PROCESS_DETACH.
		//////////////////////////////////////////////////////////////////
		for (i = 0; NeededDlls[i] != NULL; i++)
			::LoadLibrary(NeededDlls[i]);

		DefineOffsets();
		Server20Start();		
		break;

	case DLL_PROCESS_DETACH:

		Server20Stop();		
		// Release dll's that we loaded upon entry.
		for (i = 0; NeededDlls[i] != NULL; i++)
			::FreeLibrary(::GetModuleHandle(NeededDlls[i]));

		break;
    } 

    return TRUE;
}
