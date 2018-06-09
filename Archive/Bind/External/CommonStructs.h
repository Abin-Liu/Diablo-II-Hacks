//////////////////////////////////////////////////////////////////////
// CommonStructs.h
// -------------------------------------------------------------------
// This include file contains all structs and typedefs used by both 
// the server and the clients.
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#pragma once

//////////////////////////////////////////////////////////////////////
// GAMECOMMANDSTRUCT
// -------------------------------------------------------------------
// Contains info about the game command line.
//////////////////////////////////////////////////////////////////////
typedef struct gamecommandstruct_t
{
	char*	szName;
	BOOL	(PRIVATE *pFunc)(char** argv, int argc);
	char*	szUsage;

} GAMECOMMANDSTRUCT;

//////////////////////////////////////////////////////////////////////
// FINGERPRINTSTRUCT
// -------------------------------------------------------------------
// Structure used when fingerprinting functions.
//////////////////////////////////////////////////////////////////////
#define MAX_FPS_NAME_LEN		0x40
#define MAX_FPS_MODULENAME_LEN	0x10
#define MAX_FPS_FINGERPRINT_LEN	0x80

typedef struct fingerprintstruct_t
{
	char		Name[MAX_FPS_NAME_LEN];
	char		ModuleName[MAX_FPS_MODULENAME_LEN];
	char		FingerPrint[MAX_FPS_FINGERPRINT_LEN];
	DWORD		Offset;
	DWORD		PatchSize;
	DWORD		AddressFound;

} FINGERPRINTSTRUCT;

//////////////////////////////////////////////////////////////////////
// FINGERPRINTINFO
// -------------------------------------------------------------------
// Contains information about fingerprints.
//////////////////////////////////////////////////////////////////////
typedef struct fingerprintinfo_t
{
	LPSTR		FingerprintName;
	LPSTR		DllName;
	LPSTR		Fingerprint;
	DWORD		PatchSize;
	DWORD		Offset;
	PBYTE		OriginalCode;
	DWORD		MemoryLocation;

} FINGERPRINTINFO;

//////////////////////////////////////////////////////////////////////
// Modified by sting.
// Two stuctures changed
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// PLAYERINFOSTUCT
// -------------------------------------------------------------------
// Contains info about the current player.
//////////////////////////////////////////////////////////////////////
typedef struct playerinfostruct_t_109
{
	BYTE		UnitType;		// +0x00
	DWORD		CharacterClass;	// +0x04
	DWORD		PlayerID;		// +0x08
	DWORD		PlayerLocation;	// +0x0c (defines location somehow in or out of town and maybe other locations (0x05=in town, 0x01=out of town)
	DWORD		unknown1;		// +0x10
	BYTE		Act;			// +0x14
	BYTE		unknown2[0x5b];	// +0x15
	LPCSTR		PlayerName;		// +0x70 pointer to LPSZ player name
	BYTE		unknown3[0x3c];	// +0x74
	DWORD		PlayerPositionX;// +0xb0
	DWORD		PlayerPositionY;// +0xb4

} PLAYERINFOSTRUCT_109, *PPLAYERINFOSTRUCT_109;

typedef struct playerinfostruct_t_110
{
	BYTE		UnitType;		// +0x00
	DWORD		CharacterClass;	// +0x04
	DWORD		unknown1;		// +0x08
	DWORD		PlayerID;		// +0x0c
	DWORD		PlayerLocation;	// +0x10 (defines location somehow in or out of town and maybe other locations (0x05=in town, 0x01=out of town)
	LPCSTR		PlayerName;		// +0x14 pointer to LPSZ player name
	BYTE		Act;			// +0x18
	BYTE		unknown2[0x73];	// +0x19
	WORD		PlayerPositionX;// +0x8c
	WORD		PlayerPositionY;// +0x8e
	
} PLAYERINFOSTRUCT_110, *PPLAYERINFOSTRUCT_110;


//////////////////////////////////////////////////////////////////////
// GAMESTRUCT
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
typedef struct gamestruct_t_109
{
	char	Unknown1[0x1a];
	char	GameName[0x10];
	char	Unknown2[0x08];
	char	ServerIP[0x10];
	char	Unknown3[0x46];
	char	AccountName[0x10];
	char	Unknown4[0x20];
	char	CharacterName[0x10];
	char	Unknown5[0x08];
	char	RealmName[0x10];
	char	Unknown6[0x145];
	char	RealmName2[0x10];
	char	Unknown7[0x08];
	char	GamePassword[0x10];

} GAMESTRUCT_109, *PGAMESTRUCT_109;

typedef struct gamestruct_t_110
{
	char	Unknown1[0x1a];
	char	GameName[0x10];
	char	Unknown2[0x08];
	char	ServerIP[0x10];
	char	Unknown3[0x46];
	char	AccountName[0x10];
	char	Unknown4[0x20];
	char	CharacterName[0x10];
	char	Unknown5[0x08];
	char	RealmName[0x10];
	char	Unknown6[0x147];
	char	RealmName2[0x10];
	char	Unknown7[0x08];
	char	GamePassword[0x10];

} GAMESTRUCT_110, *PGAMESTRUCT_110;


#if (D2_VERSION == 109)
typedef PLAYERINFOSTRUCT_109 PLAYERINFOSTRUCT;
typedef GAMESTRUCT_109 GAMESTRUCT;
#else if(D2_VERSION == 110)
typedef PLAYERINFOSTRUCT_110 PLAYERINFOSTRUCT;
typedef GAMESTRUCT_110 GAMESTRUCT;
#endif // D2_VERSION

typedef PLAYERINFOSTRUCT *PPLAYERINFOSTRUCT;
typedef GAMESTRUCT *PGAMESTRUCT;

//////////////////////////////////////////////////////////////////////
// End of Modification
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// GAMEINFOSTRUCT
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
typedef struct thisgamestruct_t
{
	PPLAYERINFOSTRUCT	player;
	PGAMESTRUCT			CurrentGame;

} THISGAMESTRUCT, *PTHISGAMESTRUCT;



//////////////////////////////////////////////////////////////////////
// SERVERINFO *si;
// -------------------------------------------------------------------
// Contains public information about the server.
//////////////////////////////////////////////////////////////////////
typedef struct serverinfo_t
{
	DWORD	Version;
	DWORD	PlayerSlot;
	LPSTR	PluginDirectory;
	HWND	hD2Wnd;

} SERVERINFO, *PSERVERINFO;



//////////////////////////////////////////////////////////////////////
// Constant for GetInfoStruct
// -------------------------------------------------------------------
//
//////////////////////////////////////////////////////////////////////
#define	GS_ClientList	0x00000001
#define	GS_D2Param		0x00000002


typedef BOOL	(CALLBACK *PF_HookProc)(LRESULT* pResult, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//////////////////////////////////////////////////////////////////////
// FUNCTIONENTRYPOINTS *fep;
// -------------------------------------------------------------------
// Pointers to functions inside the game code and helper functions
// inside D2HackIt. This is exposed to the clients.
//////////////////////////////////////////////////////////////////////
typedef BOOL	(__cdecl *fnGamePrintString)(LPCSTR buf);
typedef BOOL	(__cdecl *fnGamePrintInfo)(LPCSTR buf);
typedef BOOL	(__cdecl *fnGamePrintError)(LPCSTR buf);
typedef BOOL	(__cdecl *fnGamePrintVerbose)(LPCSTR buf);
typedef LPSTR   (__cdecl *fnGetHackProfileString)(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName);
typedef LPSTR   (__cdecl *fnGetHackProfileStringEx)(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, DWORD& cSize);
typedef LPSTR	(__cdecl *fnGetHackProfileSectionNames)(LPCSTR lpHackName, DWORD& cSize);
typedef BOOL	(__cdecl *fnFreeHackProfileString)(LPCSTR lpString);
typedef	BOOL	(__cdecl *fnGameSendPacketToServer)(LPBYTE buf, DWORD len);
typedef	BOOL	(__cdecl *fnBnetSendPacketToServer)(LPBYTE buf, DWORD len);
typedef	BOOL	(__cdecl *fnRealmSendPacketToServer)(LPBYTE buf, DWORD len);
typedef BOOL	(__cdecl *fnGameCommandLine)(char* buf);
typedef BOOL	(__cdecl *fnIntercept)(int instruction, DWORD lpSource, DWORD lpDest, int len);
typedef	BOOL	(__cdecl *fnGetFingerprint)(LPCSTR szHackName, LPCSTR szFingerprintName, FINGERPRINTSTRUCT &fps);
typedef BOOL	(__cdecl *fnSetHackProfileString)(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPCSTR lpValue);
typedef	BOOL	(__cdecl *fnGameSendPacketToGame)(LPBYTE buf, DWORD len);
typedef	BOOL	(__cdecl *fnGameInsertPacketToGame)(LPBYTE buf, DWORD len);
typedef	BOOL	(__cdecl *fnGameSendMessageToChat)(LPSTR msg);
typedef BOOL	(__cdecl *fnD2FindFile)(LPSTR szDest, LPCSTR szFileName, int nDestSize /*= _MAX_PATH*/);
//typedef LPVOID	(__cdecl *fnGetInfoStruct)(DWORD dwGetStruct, const char* szClientName);
typedef PTHISGAMESTRUCT (__cdecl *fnGetThisgameStruct)(void);
typedef PSERVERINFO (__cdecl *fnGetServerInfo)(DWORD dwVersion, LPCSTR szModule);	// 0.71 (pass some parameters)
typedef BOOL	(__cdecl *fnGameSaveAndExit)();										// 0.71 (returns BOOL)

// Added by Dan_Goon from V0.70
typedef DWORD	(__cdecl *fnGetIniFileName)( LPCSTR lpHackName, LPSTR lpOutFileName, DWORD nBufferSize );
typedef DWORD	(__cdecl *fnGetIniString)(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPSTR lpOutBuffer, DWORD nBufferSize);
typedef DWORD	(__cdecl *fnGetIniSectionNames)( LPCSTR lpHackName, LPSTR lpOutBuffer, DWORD nBufferSize );
typedef HWND	(__cdecl *fnFindMyD2WndHandle)();
typedef BOOL	(__cdecl *fnHookD2WndProc)(HANDLE hModule, PF_HookProc pHookProc);
typedef BOOL	(__cdecl *fnCleanMeUp)(HANDLE hModule);

typedef struct functionentrypoints_t
{
	fnGetServerInfo					GetServerInfo;				// 0.59 (Moved to top @0.7)

	fnGamePrintString				GamePrintString;
	fnGamePrintInfo					GamePrintError;
	fnGamePrintError				GamePrintInfo;
	fnGamePrintVerbose				GamePrintVerbose;
	fnGetHackProfileString			GetHackProfileString;		// This will be obsoleted. Don't use this after 0.7!
	fnGetHackProfileStringEx		GetHackProfileStringEx;		// This will be obsoleted. Don't use this after 0.7!
	fnGetHackProfileSectionNames	GetHackProfileSectionNames;	// This will be obsoleted. Don't use this after 0.7!
	fnSetHackProfileString			SetHackProfileString;
	fnGameSendPacketToServer		GameSendPacketToServer;
	fnGameCommandLine				GameCommandLine;
	fnIntercept						Intercept;
	fnGetFingerprint				GetFingerprint;
	fnGetThisgameStruct				GetThisgameStruct;
	fnGameSendPacketToGame			GameSendPacketToGame;
	fnGameInsertPacketToGame		GameInsertPacketToGame;	// 0.59
	fnGameSendMessageToChat			GameSendMessageToChat;	// 0.59
	fnD2FindFile					D2FindFile;
//	fnGetInfoStruct					GetInfoStruct;
	fnFreeHackProfileString			FreeHackProfileString;	// 0.62 This will be obsoleted. Don't use this after 0.7!
	fnGameSaveAndExit				GameSaveAndExit;		// 0.62
	fnBnetSendPacketToServer		BnetSendPacketToServer;	// 0.62 
	fnRealmSendPacketToServer		RealmSendPacketToServer;// 0.62

	fnGetIniFileName				GetIniFileName;			// 0.71 (DG)
	fnGetIniString					GetIniString;			// 0.71 (DG)
	fnGetIniSectionNames			GetIniSectionNames;		// 0.71 (DG)

	fnFindMyD2WndHandle				FindMyD2WndHandle;		// 0.73
	fnHookD2WndProc					HookD2WndProc;			// 0.75
	fnCleanMeUp						CleanMeUp;

} FUNCTIONENTRYPOINTS, *PFUNCTIONENTRYPOINTS;

#define	SIZE_FUNCTIONENTRYPOINTS	sizeof(FUNCTIONENTRYPOINTS)

