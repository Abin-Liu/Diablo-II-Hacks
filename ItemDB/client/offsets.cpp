#include "Offsets.h"
#include "d2ptrs.h"

const static LPCSTR NEEDED_DLLS[] = {"D2CLIENT.DLL",
									"D2COMMON.DLL",
									"D2GAME.DLL",
									"D2LAUNCH.DLL", 
									"BNCLIENT.DLL",
									NULL };

////////////////////////////////////////////////////////////
// Originally Mousepad & D2BS Code
////////////////////////////////////////////////////////////

void DefineOffsets()
{
	DWORD *p = (DWORD *)&D2COMMON_START;
	do {
		*p++ = GetDllOffset(*p);
	} while (p <= (DWORD *)&D2COMMON_END);

	p = (DWORD *)&D2CLIENT_START;
	do {
		*p++ = GetDllOffset(*p);
	} while (p <= (DWORD *)&D2CLIENT_END);

	p = (DWORD *)&D2OTHER_START;
	do {
		*p++ = GetDllOffset(*p);
	} while (p <= (DWORD *)&D2OTHER_END);
}

/************************************************************/
DWORD GetDllOffset(LPCSTR dll, int offset)
{
	HMODULE hmod = GetModuleHandle(dll);

	// restore mousepad's old code...
	//if (!hmod)
	//	hmod = LoadLibrary(dll);

	if (!hmod) return 0;
	
	if (offset < 0) {
		return (DWORD)GetProcAddress(hmod, (LPCSTR)-offset);
	}
	return ((DWORD)hmod)+offset;
}

DWORD GetDllOffset(int num)
{
	return GetDllOffset(NEEDED_DLLS[num&0xff], num>>8);
}

