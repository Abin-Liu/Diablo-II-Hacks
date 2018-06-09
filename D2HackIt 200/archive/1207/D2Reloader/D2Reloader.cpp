// d2reloader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModuleWalker.h"
#include "IntegrityWalker.h"
#include "ModuleLoader.h"

DWORD	g_dwD2ProcessId;
HWND	g_hwndD2;
BOOL	g_bIntegrityScan;
LPCSTR	g_lpcszTargetDllPath;

static BOOL SecurityCheck(DWORD processId, BOOL bIntegrityScan)
{
	CModuleWalker dllWalker;
	if(!dllWalker.Walk(processId))
	{
		PostQuitMessage(0);
		return FALSE;
	}
	if(dllWalker.FindModule(_T("Extrawork"), TRUE, FALSE) != NULL )
	{
		return FALSE;
	}
	if(dllWalker.FindModule(_T("IX86ver"), TRUE, FALSE) != NULL)
	{
		return FALSE;
	}
	if(bIntegrityScan)
	{
		static int retry = 0;
		if (++retry > 30)
		{
			PostQuitMessage(0);
			return FALSE;
		}
		CIntegrityWalker iscan;
		if(!iscan.Walk(processId, FALSE) || !iscan.m_mods.empty())
		{
			return FALSE;
		}
	}
	return TRUE;
}

static VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (SecurityCheck(g_dwD2ProcessId, g_bIntegrityScan))
	{
		CModuleLoader::LoadModule(g_dwD2ProcessId, g_hwndD2, g_lpcszTargetDllPath);
		PostQuitMessage(0);
	}
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char classname[20];

	GetClassName(hwnd, classname, sizeof(classname));

	if(!strcmp(classname, "Diablo II"))
	{
		DWORD pid, tid;
		tid = GetWindowThreadProcessId( hwnd, &pid );
		if( pid == lparam )
		{
			g_hwndD2 = hwnd;
			return FALSE;	// Stop enum
		}
	}

	return TRUE;	// Continue enum
}


BOOL FindD2Window()
{
	EnumWindows(EnumWindowsProc, (LPARAM)g_dwD2ProcessId);
	return g_hwndD2 != NULL;
}

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   PSTR szCmdLine,
				   int iCmdShow)
{
//	__asm int 3;
	char seps[]   = " \t";
	char *token = strtok( szCmdLine, seps );
	char szTargetDllPath[MAX_PATH];
	GetModuleFileName(NULL, szTargetDllPath, MAX_PATH);
	char *p = strrchr(szTargetDllPath, '\\')+1;
	*p = '\0';
	
	char* pend = NULL;
	for (int i = 0; token != NULL; i++ )
	{
		switch(i) {
		case 0:
			g_dwD2ProcessId = (DWORD)atoi(token);
			if (!g_dwD2ProcessId || !FindD2Window())
				return -2;
			break;
		case 1:
			g_bIntegrityScan = atoi(token);
			break;
		case 2:
			for (pend = szTargetDllPath+MAX_PATH-1; *token && p < pend; token++)
			{
				if (*token != '\"')
					*p++ = *token;
			}
			*p = '\0';
			break;
		default:
			break;
		}

		token = strtok( NULL, seps );
	}

	HANDLE h = CreateFile(szTargetDllPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return -3;
	CloseHandle(h);
	g_lpcszTargetDllPath = szTargetDllPath;

	SetTimer(NULL, 0, 3000, TimerProc);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return 0;
}

