#ifndef __WNDPROCHOOK_H__
#define __WNDPROCHOOK_H__

BOOL __declspec(dllexport) InstallHook(HINSTANCE hInstance);
BOOL __declspec(dllexport) AddNotify(HWND hTarget, HWND hNotify, UINT nMsg);
BOOL __declspec(dllexport) UninstallHook();

#endif // __WNDPROCHOOK_H__