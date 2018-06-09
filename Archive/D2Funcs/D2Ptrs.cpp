#include "D2Ptrs.h"
#include <windows.h>
#include "D2Structs.h"

/************************************************************/
DWORD GetDllOffset(char *dll, int offset)
{
	HMODULE hmod = GetModuleHandle(dll);
	if (!hmod) return 0;
	if (offset < 0) {
		return (DWORD)GetProcAddress(hmod, (LPCSTR)-offset);
	}
	return ((DWORD)hmod)+offset;
}

DWORD GetDllOffset(int num)
{
	static char *dlls[] = {"D2CLIENT.DLL", "D2COMMON.DLL", "D2GFX.DLL", "D2LANG.DLL", 
			       "D2WIN.DLL", "D2NET.DLL", "D2GAME.DLL", "D2LAUNCH.DLL", "FOG.DLL", "BNCLIENT.DLL",
					"STORM.DLL"};
	return GetDllOffset(dlls[num&0xff], num>>8);
}

void AssignOffset(void * Patch)
{
	*(DWORD *)Patch = GetDllOffset(*(int *)Patch);
}

void InitializeD2Ptrs()
{
	DWORD *p = (DWORD*)&pD2PtrsListStart;
	do {
		*p++ = GetDllOffset(*p);
	} while (p <= (DWORD*)&pD2PtrsListEnd);	
}