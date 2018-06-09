//funclib.h centralized source to declare functions
#pragma once
//#include "botNET.core.h"
#include <stdio.h>
#include "offsets.h"

// include d2 function libraries
#include "d2functions.h"
//#include "botNET.core.export.h"


extern bool script_abort;
extern char lpszDllPath[MAX_PATH];
extern bool MouseEnabled;


//D2Hackit related stuff

void * __cdecl d2memcpy(DWORD lpDest, DWORD lpSource, int len);
BOOL	__declspec( dllexport ) __cdecl Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len);
// GamePacketReceived.cpp
DWORD __fastcall GamePacketReceivedIntercept(BYTE* aPacket, DWORD aLength);
void	GamePacketReceivedInterceptSTUB();
BOOL __fastcall GamePacketReceivedIntercept2();
void	GamePacketReceivedIntercept2STUB();
void __fastcall GamePlayerInfoIntercept(UnitPlayer* pis);
void			GamePlayerInfoInterceptSTUB();


///////////////////////////////////////////////////////////////////////////////
//                             Misc_Funcs.cpp                                //
//                                                                           //
//  Section with script handling funcs/misc items                            //
///////////////////////////////////////////////////////////////////////////////
WORD GetSkillByName(char* skillname);

///////////////////////////////////////////////////////////////////////////////
//                           Misc Functions                                  //
//                                                                           //
//						                                               //
///////////////////////////////////////////////////////////////////////////////

//utilfuncs.cpp -- UTIL funcs (not accessible by scripts)
//necessary to load dlls defined in offsets.cpp
//displays using drawtext
//void InitScreenhooks(cbcoreScreenPrint * sp);
//void InitChat(cbcoreCommandLine * cb);
//void InitKeys(cbcoreKeyClick * kc);
//void InitMouse(cbcoreMouseClick * mc);
//void InitGameStuff(cbcoreStartGame * sgc, cbcoreEndGame * egc);
//void InitPacketReceive(cbcorePacketReceive * cb);

void CleanupImages();
void __fastcall ScreenPrint();	// this is the shell


DWORD WINAPI LoadScript(LPVOID lpszFileName);
DWORD GetScriptFileSize(const char *lpszFileName);

LPSTR GetCurrentDllDirectory();
LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString);
DllExport LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen);


/**********************************************************


					Global Functions


 **********************************************************/

