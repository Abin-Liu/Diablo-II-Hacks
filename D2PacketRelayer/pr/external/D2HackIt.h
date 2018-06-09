//////////////////////////////////////////////////////////////////////
// D2HackIt.h
// -------------------------------------------------------------------
// Main incude file for D2HackIt Exports.
//
// <thohell@home.se>
//
//
// ## This file will define only Server/Client common used exports ##
// <Dan_Goon@diabloworld.com>
//////////////////////////////////////////////////////////////////////
#ifndef __D2HACKIT_H__
#define __D2HACKIT_H__

#include <windows.h>
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <tlhelp32.h>
//#include ".\\external\\LinkedList\\linkedlist.h"

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#ifdef EXPORT
#undef EXPORT
#endif
#define	EXPORT			__declspec( dllexport ) __cdecl
#define PRIVATE			__cdecl

#ifdef D2HACKIT_DEBUG
extern	char g_szDebug[512];
#define	DBGHDR			g_szDebug, "D2HACKIT : "
#define DGTRACE( _x_ )	sprintf _x_; OutputDebugString(g_szDebug);
#else
#define DGTRACE( _x_ )
#endif

#define D2_VERSION 110
#include "CommonStructs.h"

//////////////////////////////////////////////////////////////////////
// Version history.
// -------------------------------------------------------------------
// (See D2Hackit.txt for Version History)
//////////////////////////////////////////////////////////////////////
#define __SERVERVERSION__		MAKELONG(0, 81)

// Sane defaults for prompts
#define DEFAULTINFOPROMPT		"ÿc3::ÿc0"
#define DEFAULTERRORPROMPT		"ÿc1::ÿc0"
#define DEFAULTVERBOSEPROMPT	"ÿc2::ÿc0"

#define MAXPROMPTLENGTH			0x20

// ClientHandler.cpp
BOOL	EXPORT HookD2WndProc(HMODULE hModule, PF_HookProc pHookProc);
BOOL	EXPORT CleanMeUp(HMODULE hModule);

// GameSendPacketToServer.cpp
DWORD	EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len);
DWORD	EXPORT BnetSendPacketToServer(LPBYTE buf, DWORD len);
DWORD	EXPORT RealmSendPacketToServer(LPBYTE buf, DWORD len);
BOOL	EXPORT GameSendMessageToChat(LPSTR msg);

// GameSendPacketToGame.cpp
DWORD	EXPORT GameSendPacketToGame(LPBYTE buf, DWORD len);
BOOL	EXPORT GameInsertPacketToGame(LPBYTE buf, DWORD len);

// IniFileHandlers.cpp
LPSTR	EXPORT GetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName);
LPSTR	EXPORT GetHackProfileStringEx(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, DWORD& cSize);
LPSTR	EXPORT GetHackProfileSectionNames( LPCSTR lpHackName, DWORD& cSize );
BOOL	EXPORT SetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPCSTR lpValue);
BOOL	EXPORT FreeHackProfileString(LPCSTR lpString);
BOOL	EXPORT GetFingerprint(LPCSTR szModule, LPCSTR szFingerprintName, FINGERPRINTSTRUCT &fps);
//*****************************************************************************************************************
// New HackProfile related functions (Dan_Goon Versions)
//
// 1. These functions can be used to get the buffer size required, when NULL value is passed in lpOutBuffer.
//		In this case, the Function will only return the required buffer size without allocating the buffer.
// 2. And then caller should allocate the buffer as amount of return value in caller's routine.
// 3. Finally call those functions with caller-allocated buffer in lpOutBuffer and buffer size in nBufferSize.
// 4. Caller should free-up those buffers which are allocated by this method, when they are no more needed.
// 5. Or you can call these functions with enough sized in-stack buffers at once.
//		In this case, the returned data in buffer may not guarantee full information you required.
// 6. The output string is terminated with zero and the return value always reflects required buffer size
//		including this zero. 
// 7. When supplied buffer size is not enough to hold all results, the function returns larger value than nBufferSize.
//	   And the output string will be truncated. Caller should check the return value for this reson.
//*****************************************************************************************************************
DWORD	EXPORT GetIniFileName( LPCSTR lpHackName, LPSTR lpOutFileName, DWORD nBufferSize );
DWORD	EXPORT GetIniString( LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPSTR lpOutBuffer, DWORD nBufferSize);
DWORD	EXPORT GetIniSectionNames( LPCSTR lpHackName, LPSTR lpOutBuffer, DWORD nBufferSize );

// GamePrintFunctions.cpp
BOOL	EXPORT GamePrintInfo(LPCSTR buf);
BOOL	EXPORT GamePrintError(LPCSTR buf);
BOOL	EXPORT GamePrintVerbose(LPCSTR buf);
BOOL	EXPORT GamePrintString(LPCSTR buf);

// GameSendPacketToServer.cpp
DWORD	EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len);

// OtherExportedFunctions.cpp
PTHISGAMESTRUCT EXPORT GetThisgameStruct(void);
PSERVERINFO EXPORT GetServerInfo(DWORD dwVersion, LPCSTR szModule);
BOOL	EXPORT GameSaveAndExit();
HWND	EXPORT FindMyD2WndHandle();

// HelperFunctions.cpp
#define INST_NOP 0x90
#define INST_CALL 0xe8
#define INST_JMP 0xe9
BOOL	EXPORT Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len);
BOOL	EXPORT D2FindFile(LPSTR szDest, LPCSTR szFileName, int nDestSize = _MAX_PATH);

// GameCommandLine.cpp
BOOL	EXPORT GameCommandLine(char* buf);

LPCTSTR EXPORT GetModuleDirectory(); // retrieves module directory , Abin

#ifdef __cplusplus
}		             /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#endif // __D2HACKIT_H__

