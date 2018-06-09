// WndProcHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#pragma data_seg("SHARED")
static HWND g_hTarget = NULL;
static HWND g_hNotify = NULL;
static UINT nNotifyMsg = 0;
static HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:SHARED,RWS")

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//if (nCode == HCBT_MINMAX)
	//{
		if (wParam == (WPARAM)g_hTarget)
		{
			::PostMessage(g_hNotify, nNotifyMsg, wParam, lParam);
		}
	//}
	/*
	if (nCode >= HC_ACTION && g_hTarget && g_hNotify && nNotifyMsg)
	{
		PCWPSTRUCT p = (PCWPSTRUCT)lParam;
		if (p && p->hwnd == g_hTarget)
		{
			::PostMessage(g_hNotify, nNotifyMsg, wParam, lParam);
		}		
	}*/
	return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

BOOL __declspec(dllexport) InstallHook(HINSTANCE hInstance)
{
	g_hHook = ::SetWindowsHookEx(WH_CBT, CBTProc, hInstance, 0);
	return g_hHook != NULL;
}

BOOL __declspec(dllexport) AddNotify(HWND hTarget, HWND hNotify, UINT nMsg)
{
	g_hTarget = hTarget;
	g_hNotify = hNotify;
	nNotifyMsg = nMsg;
	return TRUE;
}

BOOL __declspec(dllexport) UninstallHook()
{
	return ::UnhookWindowsHookEx(g_hHook);
}

