//////////////////////////////////////////////////////////////////////
// DllMain.cpp
// -------------------------------------------------------------------
// Default Dll entrypoint for Diablo II hack dlls.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include <windows.h>

HWND g_hD2Wnd = NULL; // the global d2 window handle.
long g_lStyle = 0;
WNDPROC g_lpWndProc = NULL;
HHOOK g_hHook = NULL;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);


BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		g_hD2Wnd = NULL;
		g_lStyle = 0;
		g_lpWndProc = NULL;
		g_hHook = NULL;

		::EnumWindows(EnumWindowsProc, 0);

		if (g_hD2Wnd)
		{
			g_lStyle = ::GetWindowLong(g_hD2Wnd, GWL_STYLE);
			::SetWindowLong(g_hD2Wnd, GWL_STYLE, g_lStyle | WS_OVERLAPPEDWINDOW);
			//g_lpWndProc = (WNDPROC)::GetWindowLong(g_hD2Wnd, GWL_WNDPROC);
			//::SetWindowLong(g_hD2Wnd, GWL_WNDPROC, (long)WindowProc);
			g_hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)CallWndProc, (HINSTANCE)hModule, ::GetWindowThreadProcessId(g_hD2Wnd, NULL));
		}		

		break;

	case DLL_PROCESS_DETACH:

		if (g_lStyle)
			::SetWindowLong(g_hD2Wnd, GWL_STYLE, g_lStyle);

		//if (g_lpWndProc)
		//	::SetWindowLong(g_hD2Wnd, GWL_WNDPROC, (LONG)g_lpWndProc);

		if (g_hHook)
		{
			::UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}

		break;
    } 

    return TRUE;
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char classname[20] = "";

	::GetClassName(hwnd, classname, sizeof(classname) - 1);

	if(!::stricmp(classname, "Diablo II"))
	{
		DWORD pid, tid;
		tid = ::GetWindowThreadProcessId( hwnd, &pid );
		if( pid == ::GetCurrentProcessId())
		{
			g_hD2Wnd = hwnd;
			return FALSE;	// Stop enum
		}
	}

	return TRUE;	// Continue enum
}

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PCWPSTRUCT pws = (PCWPSTRUCT)lParam;
	if (pws->message == WM_ACTIVATEAPP && pws->wParam == 0)
		return 1;
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_ACTIVATEAPP && wParam == 0)
		return 1;

	return ::CallWindowProc(g_lpWndProc, hwnd, uMsg, wParam, lParam);
}