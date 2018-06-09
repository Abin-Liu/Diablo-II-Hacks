///////////////////////////////////////////////////////////////
// Main.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////

#include <windows.h>
#include <TLHELP32.H>
#include "..\\Includes\\CommonDef.h"

#define MAX_TARGETWNDS	32

#pragma data_seg ("SHARED")
static HHOOK g_hHook = NULL; // Hook handle.
static int g_nRefCount = 0; // Reference count
static HWND g_aTargetWnds[MAX_TARGETWNDS] = { 0 };
#pragma data_seg ()
#pragma comment(linker, "/section:SHARED,RWS")

#define MUTEX_UUID	"{F6608AF3-52C8-4EF0-83B0-BC160CFB1BA2}"
HINSTANCE g_hModInstance = NULL; // Instance handle of this utility DLL.
char szClass[32] = "";

int FindTargetWnd(HWND hwnd)
{
	for (int i = 0; i < MAX_TARGETWNDS; i++)
	{
		if (g_aTargetWnds[i] == hwnd)
			return i;
	}

	return -1;
}

BOOL AddTargetWnd(HWND hwnd)
{
	int nIdx = FindTargetWnd(hwnd);
	if (nIdx != -1)
		return TRUE;

	for (int i = 0; i < MAX_TARGETWNDS; i++)
	{
		if (g_aTargetWnds[i] == NULL)
		{
			g_aTargetWnds[i] = hwnd;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL RemoveTargetWnd(HWND hwnd)
{
	int nIdx = FindTargetWnd(hwnd);
	if (nIdx == -1)
		return FALSE;

	g_aTargetWnds[nIdx] = NULL;
	return TRUE;
}

void NotifyTargetWnds(HWND hWnd)
{
	char szClass[32] = "";
	::GetClassName(hWnd, szClass, 31);
	if (::stricmp(szClass, DIABLOII_CLASS))
		return;		

	for (int i = 0; i < MAX_TARGETWNDS; i++)
		::PostMessageA(g_aTargetWnds[i], WM_D2WND_EVENT, (WPARAM)hWnd, 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		g_hModInstance = hinstDLL;

	lpvReserved = lpvReserved;
	return TRUE;
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_CREATEWND)
		NotifyTargetWnds((HWND)wParam);

	return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

BOOL __declspec(dllexport) StartHookGameWnds(HWND hMsgWnd)
{
	if (hMsgWnd == NULL)
		return FALSE;

	HANDLE hMutex = ::CreateMutexA(NULL, FALSE, MUTEX_UUID);
	if (hMutex == NULL)
		return FALSE;

	if (::WaitForSingleObject(hMutex, 3000) != WAIT_OBJECT_0)
	{
		::CloseHandle(hMutex);
		return FALSE;
	}

	BOOL bOK = FALSE;

	if (AddTargetWnd(hMsgWnd))
	{
		if (g_hHook == NULL)
			g_hHook = ::SetWindowsHookEx(WH_CBT, CBTProc, g_hModInstance, 0);

		if (g_hHook)
			g_nRefCount++;

		bOK =  g_hHook != NULL;
	}

	::ReleaseMutex(hMutex);
	::CloseHandle(hMutex);
	return bOK;
}

BOOL __declspec(dllexport) StopHookGameWnds(HWND hMsgWnd)
{
	if (hMsgWnd == NULL)
		return FALSE;

	HANDLE hMutex = ::CreateMutexA(NULL, FALSE, MUTEX_UUID);
	if (hMutex == NULL)
		return FALSE;

	if (::WaitForSingleObject(hMutex, 3000) != WAIT_OBJECT_0)
	{
		::CloseHandle(hMutex);
		return FALSE;
	}

	if (RemoveTargetWnd(hMsgWnd))
	{
		if (g_nRefCount > 0)
		g_nRefCount--;

		if (g_nRefCount == 0 && g_hHook)
		{
			::UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}
	}	

	::ReleaseMutex(hMutex);
	::CloseHandle(hMutex);
	return TRUE;
}