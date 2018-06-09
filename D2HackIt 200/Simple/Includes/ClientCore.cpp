//////////////////////////////////////////////////////////////////////
// ClientCore.cpp
// -------------------------------------------------------------------
// Default Client Dll entrypoint.
//
// <thohell@home.se>
// <Dan_Goon@diabloworld.com>
//
// 0.7	: Updated by Dan_Goon
//////////////////////////////////////////////////////////////////////
#include "D2Client.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Global server struct holding pointers to exported functions
//////////////////////////////////////////////////////////////////////
HMODULE					g_hModule = NULL;			// New Global
PSERVERINFO				g_pSI = NULL;				// 0.7

BOOL PRIVATE SetupClient(HMODULE hModule, LPCSTR szModule);

LPCSTR					szLodErr = "D2Client Load Error";
LPSTR					m_szLargeBuffer = NULL;
DWORD					m_nLargeBufferSize = 0;
DWORD					m_nSectNameIndex = 0;

#ifdef	D2HACKIT_DEBUG
char	g_szDebug[512];
#endif

#ifdef _AFXDLL
static AFX_EXTENSION_MODULE D2H_CLIENT = { NULL, NULL };
#endif

//********************************************************************************************
// Important Notice on UNLOADING PROCESS!!
//
//	Client Module's DllMain(DLL_PROCESS_DETACH) can be called in two different sequences as follows! 
//
//	1. When FreeLibray() is called by D2HackIt during normal unloading process
//		(by unload command or during ServerStop() process).
//	2. Diablo II is exiting by user op (this is called prior to D2HackIt's ServerStop()!).
//		In this case, all client's thread memory is detached before OnClientStop() call!!!!
//		So, if client references any memory allocated in client's thread while OnClientStop() process,
//		it may casue memory exception, crash, hanging or remained-in-memory.
//		To prevent this situation, the unloading process is changed as follows:
//
//	New Unloading Process
//
//	1. When client's DllMain(DLL_PROCESS_DETACH) is called, calls server->ClenaMeUp()
//	2. server->CleanMeUp() handles actuall unloading process and calls client's OnGameLeave() and OnClientStop().
//	3. While unloading client by normal command or in ServerStop() process, just do as before.
//
//	So, the actual unload sequence for two different cases will be :
//
//	1. Normal Sequence (Unloading D2HackIt by second "D2Loader.exe" execution or by ".unload" command)
//
//		1) D2HackIt's DllMain(DLL_PROCESS_DETACH) : ServerStop() - this is D2HackIt's unloading process.
//		2) calls optional OnGameLeave() and OnClientStop() of client.
//		3) frees up client info structure and delete linked list.
//		4) calls FreeLibrary(hClient).
//		5) After D2HackIt is destructed, client's DllMain(DLL_PROCESS_DETACH) will be called.
//		6) server->CleanMeUp() is called : BUT THIS IS TREATED AS DUMMY CALL - DO NOTHING.
//
//	2. D2-fisrt Exiting Sequence (Exit from D2 by user op or accidental exiting)
//
//		1) Client's DllMain(DLL_PROCESS_DETACH) is called first.
//		2) server->CleanMeUp() is called.
//		3) then calls optional OnGameLeave() and OnClientStop() of client.
//		4) frees up client info structure and delete linked list.
//		5) then D2HackIt's DllMain(DLL_PROCESS_DETACH) : ServerStop() is called.
//		6) clients are already unloaded - DO NOTHING.
//
//********************************************************************************************


//////////////////////////////////////////////////////////////////////
// Dll entry/exit
//////////////////////////////////////////////////////////////////////
extern "C" BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	char szTemp[MAX_PATH], szFile[MAX_PATH];
	BOOL bResult = TRUE;

    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

#ifdef _AFXDLL
		if (!AfxInitExtensionModule(D2H_CLIENT, (HINSTANCE)hModule)) return FALSE;
#endif

		//DGTRACE(( CHDR "DLL_PROCESS_ATTACH\n" ));

		g_hModule = (HMODULE)hModule;
		GetModuleFileName( g_hModule, szTemp, sizeof(szTemp) );
		_splitpath(szTemp, NULL, NULL, szFile, NULL);		// Extract File Title

		bResult = FALSE;
		// Bind exported functions from server
		HMODULE hD2Hackit;
		hD2Hackit = (HMODULE)GetModuleHandle("D2HackIt");
		if(!hD2Hackit)
		{
			sprintf(szTemp, "[%s] : Failed to get D2HackIt!", szFile);
			MessageBox(NULL, szTemp, szLodErr, MB_OK);
			break;
		}

		// Fill in the server & g_pSI (server info)
		return SetupClient(hD2Hackit, szFile);

	case DLL_THREAD_DETACH:

		// Fall here first in case of accidental exit of Diablo II
		//DGTRACE(( CHDR "DLL_THREAD_DETACH\n" ));
		break;

	case DLL_PROCESS_DETACH:

		// This is normal call from FreeLibrary
		//DGTRACE(( CHDR "DLL_PROCESS_DETACH\n" ));

		// This call is available from V0.8
		if(!IsBadCodePtr((FARPROC)server->CleanMeUp)) server->CleanMeUp(g_hModule);

		if(m_szLargeBuffer) delete [] m_szLargeBuffer;
		m_szLargeBuffer = NULL;

#ifdef _AFXDLL
		AfxTermExtensionModule(D2H_CLIENT);
#endif
		break;
    } 

    return bResult;
}

// From V0.7 by Dan_Goon

BOOL PRIVATE SetupClient(HMODULE hModule, LPCSTR szModule)
{
	char tbf[256];

	BIND_TO_SERVER(GetServerInfo);			// This is available from V0.59

	// We need at least 0.59
	if(!server->GetServerInfo)
	{
		sprintf(tbf, "[%s] : Failed to bind server functions!\n\nPlease use D2HackIt! V0.59 or later.", szModule);
		MessageBox(NULL, tbf, szLodErr, MB_OK);
		return FALSE;
	}

	g_pSI = server->GetServerInfo(__SERVERVERSION__, szModule);

	if(!g_pSI)
	{
		server->GamePrintError("D2HackIt Server Info not available!");
	}
	else
	{
		_snprintf(tbf, sizeof(tbf), "D2HackIt directory = %s", g_pSI->PluginDirectory);
		server->GamePrintVerbose(tbf);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Stubfunctions for 'property get' functions.
//////////////////////////////////////////////////////////////////////
LPCSTR	EXPORT GetModuleAuthor()		{return ModuleAuthor;}
LPCSTR	EXPORT GetModuleWebsite()		{return ModuleWebsite;}
DWORD	EXPORT GetModuleVersion()		{return ModuleVersion;}
LPCSTR	EXPORT GetModuleEmail()			{return ModuleEmail;}
LPCSTR	EXPORT GetModuleDescription()	{return ModuleDescription;}

//////////////////////////////////////////////////////////////////////
// OnClientCommandLine
// -------------------------------------------------------------------
// The modules own extension of the command line interface. Any custom
// commands you add are parsed here.
//
// Return value should be TRUE, but it is not used at this 
// time.
//
// Arguments when we get here:
// argv[0]			Name of module
// argv[1]			Name of command (If supplied)
// argv[2 ... n]	The rest
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnGameCommandLine(char* argv[], int argc)
{
	//DGTRACE((DBGHDR "OnGameCommandLine : (%d) [%s]..\n", argc, argc > 0 ? argv[0] : "Mola"));

	// Check if user supplied anything at all, if not assume help...
	if (argc==1) argv[argc++] = "help";


	MODULECOMMANDSTRUCT* mcs = ModuleCommands;

	while (mcs->szName)
	{
		if (!stricmp(mcs->szName, argv[1])) break;
		mcs++;
	}

	char *p, fMsg[256];
	//
	// Is this a built-in function ?
	if (mcs->szName)
	{
		// If functions returns false, show usage help
		if (!mcs->pFunc(argv, argc))
		{
			server->GamePrintInfo("Usage:");

			p = strtok(mcs->szUsage, "\n");
			while(p)
			{
				sprintf(fMsg, "ÿc4.%s %s", argv[0], p);
				server->GamePrintInfo(fMsg);

				p = strtok(NULL, "\n");
			}
		}
	}
	else
	{
		// Unknown command, show catch-all help phraze.
		sprintf(fMsg, "Unknown command ÿc4'%s %s'ÿc0 - try ÿc4'.%s help'ÿc0 to get help.",
			argv[0], argv[1], argv[0]);
		server->GamePrintError(fMsg);
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// OnClientCommandHelp
// -------------------------------------------------------------------
// Our default help function.
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc)
{
	// If command line is longer than 2, show usage for 'help'
	if (argc>2) return FALSE;

	char t[1024];
	sprintf(t, "Available commands for %s:", argv[0]);
	server->GamePrintInfo(t);

	// Loop through ModuleCommands[] and print their names
	for (int i=0; ModuleCommands[i].szName != NULL; i++)
	{
		sprintf(t, "ÿc4.%s %s", argv[0], ModuleCommands[i].szName);
		server->GamePrintInfo(t);
	}

	sprintf(t, "For help on a specific command, type ÿc4.%s <command> help", argv[0]);
	server->GamePrintInfo(t);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// assert()
// -------------------------------------------------------------------
// Thanks to ClckWcher for contributing assert code
//
// Use assert(expression) or assert(expression, "Message") in your 
// module.
//
//////////////////////////////////////////////////////////////////////

// Added by Abin, to avoid warning C4005(macro redefinition)
#ifdef assert
#undef assert 
#endif

#define assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )
#define msg_assert(exp,msg) (void)( (exp) || (_assert(msg, __FILE__, __LINE__), 0) )
void _assert(char * str,char * file,int line)
{
	char t[1024];
	sprintf(t, "Assertion failed: ÿc4%sÿc0, file ÿc4%sÿc0, line ÿc4%d", str, file, line);
	server->GamePrintError(t);
}
