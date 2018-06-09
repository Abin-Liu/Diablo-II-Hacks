///////////////////////////////////////////////////////////////
// LoaderDLL.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////

#ifndef __LOADERDLL_H__
#define __LOADERDLL_H__

#include <windows.h>

BOOL __declspec(dllexport) StartHookGameWnds(HWND hMsgWnd);
BOOL __declspec(dllexport) StopHookGameWnds(HWND hMsgWnd);

#endif // __LOADERDLL_H__