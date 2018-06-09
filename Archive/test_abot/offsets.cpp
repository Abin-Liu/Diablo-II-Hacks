/*
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//define the functions that call ingame funcs
//util functions found in utilfuncs.cpp
#include "funclib.h"

// d2 function libraries:
#include "d2ptrs.h"
//#include "d2client.h"

#define DllExport   __declspec(dllexport)
#define DllImport   __declspec( dllimport )


// ASM pointers:

//has to be called before UtilGetExport works(main dll calls this)
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
/*
DllExport void WinDrawText(char * pszText, int x, int y, int nColor, int nFont, int align)
{
	wchar_t buf[1024];
	int len = strlen(pszText)+1;
	DWORD oldsize, width;
	ConvertToUnicode(pszText, buf, 1024);
	//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszText, len, buf, 1024);
	if(nFont != -1)
		oldsize = D2WIN_DrawTextFont(nFont);
	if(align == ALIGN_LEFT)
		width = 0;
	else if(align == ALIGN_CENTER)
		width = (D2WIN_GetTextWidth(buf))/2;
	else
		width = D2WIN_GetTextWidth(buf);
	D2WIN_DrawText(buf, x-width, y, nColor, 0);
	if(nFont != -1)
		D2WIN_DrawTextFont(oldsize);
	return;
}
*/
void GameSendPacketToChatServer(LPBYTE lpBuf, DWORD len)
{
	FOG_Send(*p_BNCLIENT_bnClient, lpBuf, len);
}

void GameSendPacketToServer(LPBYTE lpBuf, DWORD len)
{
	D2NET_Send(1, lpBuf, len);
}

void GameSendPacketToGame(LPBYTE lpBuf, DWORD len)
{
//taken from d2hackit .60 source
__asm {
	call FakeEntrypoint
	jmp  EndOfFunction

FakeEntrypoint:
	// This code is an approximate copy of the code
	// at 0x6fc015e0 (1.09). Thanks to Techwarrior 
	// for the info. We use this to stub the 
	// function and pass our own packet to the game.
	sub esp, 0x10c
	push ebx
	mov ebx, lpBuf
	push ebp
	push esi
	mov esi, len

	// Jump to original code
	mov eax, dword ptr [D2NET_Recv]
	push eax
	ret					
EndOfFunction:
	}
}

void WeaponSwitch()
{
	D2CLIENT_WeaponSwitch();
}

BOOL IsSwitched()
{
	UnitPlayer *player = D2CLIENT_GetPlayerUnit();
	return D2GAME_IsSwitched(player->ptPlayerData->ptPlrMsg);
}


//DWORD GetScreenOffset(DWORD baseaddress)
//{
//	if(!baseaddress)
//		return D2CLIENT_ScreenPrint;
//	if(baseaddress == -1)
//		return D2CLIENT_ReturnScreenPrint;
//	return baseaddress - (D2CLIENT_ScreenPrint+1+4);
//}


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


