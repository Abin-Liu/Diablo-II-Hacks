//////////////////////////////////////////////////////////////////////
// CommonStructs.h
// -------------------------------------------------------------------
// This include file contains all structs and typedefs used by both 
// the server and the clients.
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#ifndef __COMMONSTRUCTS_H__
#define __COMMONSTRUCTS_H__

#include <windows.h>
#include "Constants.h"

////////////////////////////////////////////////////////////////////
// Screen Drawing API (v2.00, Abin)
//------------------------------------------------------------------
// Drawing on game screen.
////////////////////////////////////////////////////////////////////
typedef LPVOID HD2BITMAP; // handle to bitmap drawn on screen
typedef BOOL (__cdecl *fnD2TextOut)(LPCSTR lpszText, long x, long y);
typedef BOOL (__cdecl *fnD2DrawText)(LPCSTR lpszText, LPCRECT lpRect, UINT nAlign);
typedef POINT (__cdecl *fnD2MoveTo)(long x, long y);
typedef BOOL (__cdecl *fnD2LineTo)(long x, long y);
typedef BOOL (__cdecl *fnD2Rectangle)(LPCRECT lpRect);
typedef BOOL (__cdecl *fnD2FrameRect)(LPCRECT lpRect);
typedef BOOL (__cdecl *fnD2FillRect)(LPCRECT lpRect);
typedef BYTE (__cdecl *fnD2SetFont)(BYTE iFont);
typedef BYTE (__cdecl *fnD2GetFont)();
typedef BYTE (__cdecl *fnD2SetTextColor)(BYTE iColor);
typedef BYTE (__cdecl *fnD2GetTextColor)();
typedef int (__cdecl *fnD2SetBkMode)(int nBkMode);
typedef int (__cdecl *fnD2GetBkMode)();
typedef SIZE (__cdecl *fnD2GetTextExtent)(LPCSTR lpszText);
typedef BYTE (__cdecl *fnD2SetPenColor)(BYTE iColor);
typedef BYTE (__cdecl *fnD2GetPenColor)();
typedef int (__cdecl *fnD2SetPenWidth)(int nWidth);
typedef int (__cdecl *fnD2GetPenWidth)();
typedef BYTE (__cdecl *fnD2SetBrushColor)(BYTE iColor);
typedef BYTE (__cdecl *fnD2GetBrushColor)();
typedef BYTE (__cdecl *fnD2SetBrushTransparency)(BYTE iTransparency);
typedef BYTE (__cdecl *fnD2GetBrushTransparency)();
typedef POINT (__cdecl *fnD2GetCurrentPosition)();
typedef BOOL (__cdecl *fnD2DrawD2Bitmap)(HD2BITMAP hBitmap, long x, long y, BYTE iMaskColor);

////////////////////////////////////////////////////////////////////
// CGameDC (v2.00, Abin)
//------------------------------------------------------------------
// Will be passed to "OnDraw" functions in client modules. Use members
// of this struct to draw customized stuff on the game screen.
////////////////////////////////////////////////////////////////////
typedef struct tagGameDC 
{
	// Drawing methods
	fnD2TextOut					TextOut;	// Draw simple text
	fnD2DrawText				DrawText;	// Draw aligned text
	fnD2MoveTo					MoveTo;		// Change DC start position
	fnD2LineTo					LineTo;		// Draw line
	fnD2Rectangle				Rectangle;	// Draw a solid rectangle outlined by a frame
	fnD2FrameRect				FrameRect;	// Draw empty frame
	fnD2FillRect				FillRect;	// Draw a solid rectangle	
	fnD2DrawD2Bitmap			DrawD2Bitmap; // Draw a bmp image to the screen

	// Drawing data access
	fnD2GetCurrentPosition		GetCurrentPosition; // Current pen position
	fnD2SetFont					SetFont;	// Set font
	fnD2GetFont					GetFont;	// Get font
	fnD2SetTextColor			SetTextColor; // Set text color
	fnD2GetTextColor			GetTextColor; // Get text color
	fnD2SetBkMode				SetBkMode; // Set text background mode
	fnD2GetBkMode				GetBkMode; // Get text background mode
	fnD2GetTextExtent			GetTextExtent; // Get text drawing size
	fnD2SetPenColor				SetPenColor; // Set pen color
	fnD2GetPenColor				GetPenColor; // Get pen color
	fnD2SetPenWidth				SetPenWidth; // Set pen width
	fnD2GetPenWidth				GetPenWidth; // Get pen width
	fnD2SetBrushColor			SetBrushColor; // Set brush color
	fnD2GetBrushColor			GetBrushColor; // Get brush color
	fnD2SetBrushTransparency	SetBrushTransparency; // Set brush transparency
	fnD2GetBrushTransparency	GetBrushTransparency; // Get brush transparency
	
} GAMEDC, *LPGAMEDC;
typedef const tagGameDC* LPCGAMEDC;
typedef tagGameDC CGameDC; // make the name more MFC GDI'ish :)

//////////////////////////////////////////////////////////////////////
// GAMEUNIT
//--------------------------------------------------------------------
// The basic game unit info.
//////////////////////////////////////////////////////////////////////
typedef struct tagGameUnit
{
	DWORD dwUnitID;					// Unit ID
	DWORD dwUnitType;				// Unit type(player, monster, object, etc)
} GAMEUNIT, *LPGAMEUNIT;
typedef const tagGameUnit* LPCGAMEUNIT;

// Unit enumeration callback definition
typedef BOOL (CALLBACK *fnEnumUnitProc)(LPCGAMEUNIT lpUnit, LPARAM lParam);

// Player enumeration callback definition
typedef BOOL (CALLBACK *fnEnumPlayerProc)(DWORD dwPlayerID, LPCSTR lpszPlayerName, DWORD dwPlayerClass, DWORD dwPvpFlags, BYTE iMapID, LPARAM lParam);

//////////////////////////////////////////////////////////////////////
// PRESETUNIT
//--------------------------------------------------------------------
// Game preset-unit(Caves, Stairs, Dens, Towers, Entrances, Izual,
// Hephasto, Countess Chest, etc...).
//////////////////////////////////////////////////////////////////////
typedef struct tagPresetUnit
{
	DWORD dwID;			// Preset unit ID 
	DWORD dwType;		// Preset unit type, monster, object, room tile, etc
	long x;				// Map position x
	long y;				// Map position y
} PRESETUNIT, *LPPRESETUNIT;
typedef const tagPresetUnit* LPCPRESETUNIT;

//////////////////////////////////////////////////////////////////////
// PresetUnit enumeration callback definition
//////////////////////////////////////////////////////////////////////
typedef BOOL (CALLBACK *fnEnumPresetUnitProc)(LPCPRESETUNIT lpPresetUnit, LPARAM lParam);


//////////////////////////////////////////////////////////////////////
// ITEMPROPERTY
//--------------------------------------------------------------------
// Item properties(mods) data. Thanks to Mousepad for the help.
//////////////////////////////////////////////////////////////////////
typedef struct tagItemProperty
{
    WORD wSubProperty; // Sub property value
    WORD wPropertyID; // Property ID
    int nValue; // Property value
} ITEMPROPERTY, *LPITEMPROPERTY;
typedef const tagItemProperty* LPCITEMPROPERTY;

//////////////////////////////////////////////////////////////////////
// Item Enumeration Callback Function def
//////////////////////////////////////////////////////////////////////
typedef BOOL (CALLBACK *fnEnumItemProc)(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam); 

//////////////////////////////////////////////////////////////////////
// GAMECOMMANDSTRUCT
// -------------------------------------------------------------------
// Contains info about the game command line.
//////////////////////////////////////////////////////////////////////
#ifndef PRIVATE
#define PRIVATE			__cdecl
#endif

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
typedef struct playerinfostruct_t
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
	
} PLAYERINFOSTRUCT, *PPLAYERINFOSTRUCT;

//////////////////////////////////////////////////////////////////////
// GAMESTRUCT
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
typedef struct gamestruct_t
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

} GAMESTRUCT, *PGAMESTRUCT;


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

// Added by Abin from v2.00
typedef BOOL (__cdecl *fnPatchFill)(LPVOID lpDest, LPCVOID lpSource, LPVOID lpOldCode, int len); // 2.00
typedef LPCSTR (__cdecl *fnGetModuleDirectory)(); // Get plugin directory
typedef BOOL (__cdecl *fnIsClientLoaded)(LPCSTR lpszClientName); // Check if a module was loaded(*.d2h, *.dll)
typedef BOOL (__cdecl *fnGameInfof)(LPCSTR lpszFormat, ...); // Convenient string route
typedef BOOL (__cdecl *fnGameErrorf)(LPCSTR lpszFormat, ...);
typedef BOOL (__cdecl *fnGameStringf)(LPCSTR lpszFormat, ...);
typedef BOOL (__cdecl *fnGameCommandf)(LPCSTR lpszFormat, ...);
typedef BOOL (__cdecl *fnGamePrintPacket)(BOOL bReceived, const BYTE *aPacket, DWORD aLen);
typedef BOOL (__cdecl *fnIsGameReady)();
typedef BOOL (__cdecl *fnWasD2HackitLoadedBeforeJoinGame)();
typedef BOOL (__cdecl *fnVerifyUnit)(LPCGAMEUNIT lpUnit);
typedef DWORD (__cdecl *fnFindUnitByClassID)(DWORD dwUnitClassID, DWORD dwUnitType, LPGAMEUNIT lpBuffer);
typedef DWORD (__cdecl *fnFindUnitByName)(LPCSTR lpszUnitName, DWORD dwUnitType, LPGAMEUNIT lpBuffer);
typedef POINT (__cdecl *fnGetUnitPosition)(LPCGAMEUNIT lpUnit);
typedef DWORD (__cdecl *fnGetUnitName)(LPCGAMEUNIT lpUnit, LPSTR lpszBuffer, DWORD dwMaxChars);
typedef DWORD (__cdecl *fnGetUnitOwnerID)(LPCGAMEUNIT lpUnit);
typedef DWORD (__cdecl *fnGetUnitClassID)(LPCGAMEUNIT lpUnit);
typedef BYTE (__cdecl *fnGetUnitHPPercent)(LPCGAMEUNIT lpUnit);
typedef DWORD (__cdecl *fnGetUnitStat)(LPCGAMEUNIT lpUnit, int nStatID);
typedef BOOL (__cdecl *fnGetUnitAffection)(LPCGAMEUNIT lpUnit, int nAffectionID);
typedef int (__cdecl *fnGetMonsterType)(LPCGAMEUNIT lpUnit); // normal/champion/boss/minion
typedef DWORD (__cdecl *fnGetMonsterAttributes)(LPCGAMEUNIT lpUnit); // monster attributes
typedef DWORD (__cdecl *fnGetDistance)(long x1, long y1, long x2, long y2); // distance between two positions, in yards
typedef DWORD (__cdecl *fnIsMercUnit)(LPCGAMEUNIT lpUnit); // Check if an unit is a merc
typedef DWORD (__cdecl *fnGetPing)(); // Get ping value (lag indicator)
typedef BOOL (__cdecl *fnEnumUnits)(DWORD dwUnitType, fnEnumUnitProc lpfnEnumUnitProc, LPARAM lParam);
typedef BOOL (__cdecl *fnSendKey)(BYTE iVKCode); // Send a key-down message to d2 window
typedef DWORD (__cdecl *fnGetSpellName)(WORD wSpell, LPSTR lpszBuffer, DWORD dwMaxChars);
typedef DWORD (__cdecl *fnGetAffectionName)(BYTE iAffectionID, LPTSTR lpszBuffer, DWORD dwMaxChars);
typedef BOOL (__cdecl *fnAffectionIsAura)(BYTE iAffectionID);
typedef DWORD (__cdecl *fnGetMapName)(BYTE iMapID, LPTSTR lpszBuffer, DWORD dwMaxChars); // Name of the map
typedef BOOL (__cdecl *fnMapIsTown)(BYTE iMapID); // Is town?
typedef BYTE (__cdecl *fnGetTownMap)(BYTE iAct);
typedef BYTE (__cdecl *fnGetActFromMap)(BYTE iMapID);
typedef SIZE (__cdecl *fnGetItemSize)(LPCSTR lpszItemCode);
typedef int (__cdecl *fnGetGameDifficulty)(); // current game difficulty, normal/nm/hell
typedef DWORD (__cdecl *fnGetD2WindowTitle)(LPSTR lpszBuffer, DWORD dwMaxChars);
typedef DWORD (__cdecl *fnGetInGameTime)(); // time elapsed since joining
typedef LPCSTR (__cdecl *fnGetRealmName)();
typedef LPCSTR (__cdecl *fnGetGameName)();
typedef LPCSTR (__cdecl *fnGetGamePassword)();
typedef LPCSTR (__cdecl *fnGetGameIP)();

typedef BOOL (__cdecl *fnEnumPresetUnits)(fnEnumPresetUnitProc lpfnEnumPresetUnitProc, LPARAM lParam);
typedef POINT (__cdecl *fnFindPresetUnit)(DWORD dwUnitID, DWORD dwUnitType);
typedef BOOL (__cdecl *fnGetItemCode)(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars);
typedef BOOL (__cdecl *fnLeftClick)(long x, long y);
typedef BOOL (__cdecl *fnRightClick)(long x, long y);
typedef DWORD (__cdecl *fnQueryModuleData)(LPCSTR lpszModuleName, DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam);
typedef DWORD (__cdecl *fnGetKeyName)(BYTE iKCode, LPSTR lpszBuffer, DWORD dwMaxChars);
typedef VOID (__cdecl *fnGameSendPacketToServer2)(LPBYTE buf, DWORD len, DWORD dwDelay);
typedef BOOL (__cdecl *fnIsQuestItem)(LPCSTR lpszItemCode);
typedef int (__cdecl *fnGetItemQuality)(DWORD dwItemID);
typedef BOOL (__cdecl *fnGetItemPrefixSuffix)(DWORD dwItemID, WORD* pPrefix, WORD* pSuffix);
typedef DWORD (__cdecl *fnGetPrefixSuffixName)(WORD wPrefixOrSuffix, LPSTR lpszBuffer, DWORD dwMaxChars);

typedef SIZE (__cdecl *fnGetScreenSize)(); // Get the game screen size	
typedef void (__cdecl *fnScreenToMapCoords)(POINT& rScreenCoords);
typedef void (__cdecl *fnMapToScreenCoords)(POINT& rMapPosition);
typedef POINT (__cdecl *fnGetMousePosition)();
typedef POINT (__cdecl *fnGetRandomPosition)(int nRadiusMin, int nRadiusMax, int nAngleMin, int nAngleMax, const LPPOINT lpOrigin);
typedef int (__cdecl *fnGetAngle)(long x1, long y1, long x2, long y2);
typedef DWORD (__cdecl *fnCalculateTeleportPath)(long x, long y, LPPOINT lpBuffer, DWORD dwMaxCount);
typedef DWORD (__cdecl *fnCalculateWalkPath)(long x, long y, LPPOINT lpBuffer, DWORD dwMaxCount);
typedef BOOL (__cdecl *fnCreateCollisionMap)();
typedef WORD (__cdecl *fnGetCollisionData)(long x, long y, BOOL bAbsolute);
typedef BOOL (__cdecl *fnDumpCollisionMap)(LPCSTR lpszFilePath, const LPPOINT lpTokens, DWORD dwTokenCount);
typedef DWORD (__cdecl *fnGetItemPrice)(DWORD dwItemID, DWORD dwNpcClassID, BOOL bBuyOrSell);
typedef DWORD (__cdecl *fnGetItemAttributes)(DWORD dwItemID);
typedef BYTE (__cdecl *fnGetItemSockets)(DWORD dwItemID);
typedef BOOL (__cdecl *fnGetMapRect)(LPRECT lpBuffer);
typedef BOOL (__cdecl *fnUnitOverhead)(LPCGAMEUNIT lpUnit, LPCSTR lpszText, DWORD dwDuration);
typedef DWORD (__cdecl *fnGetItemProperties)(DWORD dwItemID, LPITEMPROPERTY lpBuffer, DWORD dwMaxCount);
typedef BOOL (__cdecl *fnGetHighlightUnit)(LPGAMEUNIT lpBuffer);
typedef DWORD (__cdecl *fnGetUnitMode)(LPCGAMEUNIT lpUnit);
typedef BOOL (__cdecl *fnEnumGroundItems)(fnEnumItemProc lpfn, LPARAM lParam);
typedef BOOL (__cdecl *fnExamItemProperties)(DWORD dwItemID, LPCSTR lpszExpression, LPCSTR lpszCodes, BOOL* pSyntaxError);
typedef BOOL (__cdecl *fnEnumPlayers)(fnEnumPlayerProc lpfn, LPARAM lParam);
typedef BYTE (__cdecl *fnGetItemLevel)(DWORD dwItemID);
typedef DWORD (__cdecl *fnGetItemDescription)(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor);
typedef DWORD (__cdecl *fnGetD2HackitVersion)();
typedef DWORD (__cdecl *fnGetD2HackitBuild)();
typedef HD2BITMAP (__cdecl *fnLoadD2Bitmap)(LPCSTR lpszFilePath);
typedef BOOL (__cdecl *fnDestroyD2Bitmap)(HD2BITMAP hBitmap);
typedef DWORD (__cdecl *fnGetUnitBaseStat)(LPCGAMEUNIT lpUnit, int nStatID);
typedef DWORD (__cdecl *fnGetItemLevelRequirement)(DWORD dwItemID);
typedef DWORD (__cdecl *fnGetItemStrRequirement)(DWORD dwItemID);
typedef DWORD (__cdecl *fnGetItemDexRequirement)(DWORD dwItemID);
typedef BOOL (__cdecl *fnGetItemDamage)(DWORD dwItemID, DWORD* pMinDamage, DWORD* pMaxDamage);
typedef BOOL (__cdecl *fnIsTwoHandItem)(DWORD dwItemID);
typedef SIZE (__cdecl *fnGetItemSize2)(DWORD dwItemID);
typedef int (__cdecl *fnGetItemType)(DWORD dwItemID);
typedef BOOL (__cdecl *fnIsObjectDoor)(LPCGAMEUNIT lpUnit);
typedef BOOL (__cdecl *fnIsObjectChest)(LPCGAMEUNIT lpUnit);
typedef BOOL (__cdecl *fnIsObjectShrine)(LPCGAMEUNIT lpUnit);

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

	fnPatchFill						PatchFill;				// 2.00 (Abin)
	fnGetModuleDirectory			GetModuleDirectory;		// 2.00 (Abin)
	fnIsClientLoaded				IsClientLoaded;			// 2.00 (Abin)
	fnGameInfof						GameInfof;				// 2.00 (Abin)
	fnGameErrorf					GameErrorf;				// 2.00 (Abin)
	fnGameStringf					GameStringf;			// 2.00 (Abin)
	fnGameCommandf					GameCommandf;			// 2.00 (Abin)
	fnGamePrintPacket				GamePrintPacket;		// 2.00 (Abin)
	fnIsGameReady					IsGameReady;			// 2.00 (Abin)
	fnWasD2HackitLoadedBeforeJoinGame WasD2HackitLoadedBeforeJoinGame; // 2.00 (Abin)
	fnEnumUnits						EnumUnits;				// 2.00 (Abin)
	fnVerifyUnit					VerifyUnit;				// 2.00 (Abin)
	fnFindUnitByClassID				FindUnitByClassID;		// 2.00 (Abin)
	fnFindUnitByName				FindUnitByName;			// 2.00 (Abin)
	fnGetUnitPosition				GetUnitPosition;		// 2.00 (Abin)
	fnGetUnitName					GetUnitName;			// 2.00 (Abin)
	fnGetUnitOwnerID				GetUnitOwnerID;			// 2.00 (Abin)
	fnGetUnitClassID				GetUnitClassID;			// 2.00 (Abin)
	fnGetUnitHPPercent				GetUnitHPPercent;		// 2.00 (Abin)
	fnGetUnitStat					GetUnitStat;			// 2.00 (Abin)
	fnGetUnitAffection				GetUnitAffection;		// 2.00 (Abin)
	fnGetMonsterType				GetMonsterType;			// 2.00 (Abin)
	fnGetMonsterAttributes			GetMonsterAttributes;	// 2.00 (Abin)
	fnGetDistance					GetDistance;			// 2.00 (Abin)
	fnIsMercUnit					IsMercUnit;				// 2.00 (Abin)
	fnGetPing						GetPing;				// 2.00 (Abin)
	fnGetAffectionName				GetAffectionName;		// 2.00 (Abin)
	fnAffectionIsAura				AffectionIsAura;		// 2.00 (Abin)
	fnGetMapName					GetMapName;				// 2.00 (Abin)
	fnMapIsTown						MapIsTown;				// 2.00 (Abin)
	fnGetTownMap					GetTownMap;				// 2.00 (Abin)
	fnGetActFromMap					GetActFromMap;			// 2.00 (Abin)
	fnGetItemSize					GetItemSize;			// 2.00 (Abin)
	fnSendKey						SendKey;				// 2.00 (Abin)
	fnGetSpellName					GetSpellName;			// 2.00 (Abin)
	fnGetGameDifficulty				GetGameDifficulty;		// 2.00 (Abin)
	fnGetD2WindowTitle				GetD2WindowTitle;		// 2.00 (Abin)
	fnGetInGameTime					GetInGameTime;			// 2.00 (Abin)
	fnGetRealmName					GetRealmName;			// 2.00 (Abin)
	fnGetGameName					GetGameName;			// 2.00 (Abin)
	fnGetGamePassword				GetGamePassword;		// 2.00 (Abin)
	fnGetGameIP						GetGameIP;				// 2.00 (Abin)
	fnEnumPresetUnits				EnumPresetUnits;		// 2.00 (Abin)
	fnCalculateTeleportPath			CalculateTeleportPath;	// 2.00 (Abin)
	fnGetItemCode					GetItemCode;			// 2.00 (Abin)
	fnLeftClick						LeftClick;				// 2.00 (Abin)
	fnRightClick					RightClick;				// 2.00 (Abin)
	fnQueryModuleData				QueryModuleData;		// 2.00 (Abin)
	fnGetKeyName					GetKeyName;				// 2.00 (Abin)
	fnIsQuestItem					IsQuestItem;			// 2.00 (Abin)
	fnGetItemQuality				GetItemQuality;			// 2.00 (Abin)
	fnGetItemPrefixSuffix			GetItemPrefixSuffix;	// 2.00 (Abin)
	fnGetPrefixSuffixName			GetPrefixSuffixName;	// 2.00 (Abin)
	fnGetScreenSize					GetScreenSize;			// 2.00 (Abin)
	fnScreenToMapCoords				ScreenToMapCoords;		// 2.00 (Abin)
	fnMapToScreenCoords				MapToScreenCoords;		// 2.00 (Abin)	
	fnGetMousePosition				GetMousePosition;		// 2.00 (Abin)
	fnGetRandomPosition				GetRandomPosition;		// 2.00 (Abin)
	fnGetAngle						GetAngle;				// 2.00 (Abin)
	fnCreateCollisionMap			CreateCollisionMap;		// 2.00 (Abin)
	fnGetCollisionData				GetCollisionData;		// 2.00 (Abin)
	fnCalculateWalkPath				CalculateWalkPath;		// 2.00 (ustc_tweeg)
	fnDumpCollisionMap				DumpCollisionMap;		// 2.00 (Abin)
	fnGetItemPrice					GetItemPrice;			// 2.00 (Abin)
	fnGetItemAttributes				GetItemAttributes;		// 2.00 (Abin)
	fnGetItemSockets				GetItemSockets;			// 2.00 (Abin)
	fnUnitOverhead					UnitOverhead;			// 2.00 (Abin)
	fnGetItemProperties				GetItemProperties;		// 2.00 (Abin)
	fnFindPresetUnit				FindPresetUnit;			// 2.00 (Abin)
	fnGetHighlightUnit				GetHighlightUnit;		// 2.00 (Abin)
	fnGetUnitMode					GetUnitMode;			// 2.00 (Abin)
	fnEnumGroundItems				EnumGroundItems;		// 2.00 (Abin)
	fnExamItemProperties			ExamItemProperties;		// 2.00 (Abin)
	fnEnumPlayers					EnumPlayers;			// 2.00 (Abin)
	fnGetItemLevel					GetItemLevel;			// 2.00 (Abin)
	fnGetItemDescription			GetItemDescription;		// 2.00 (Abin)
	fnGetMapRect					GetMapRect;				// 2.00 (Abin)
	fnGetD2HackitVersion			GetD2HackitVersion;		// 2.00 (Abin)
	fnGetD2HackitBuild				GetD2HackitBuild;		// 2.00 (Abin)
	fnLoadD2Bitmap					LoadD2Bitmap;			// 2.00 (Abin)
	fnDestroyD2Bitmap				DestroyD2Bitmap;		// 2.00 (Abin)
	fnGetUnitBaseStat				GetUnitBaseStat;		// 2.00 (Abin)
	fnGetItemLevelRequirement		GetItemLevelRequirement;// 2.00 (Abin)
	fnGetItemStrRequirement			GetItemStrRequirement;	// 2.00 (Abin)
	fnGetItemDexRequirement			GetItemDexRequirement;	// 2.00 (Abin)
	fnGetItemDamage					GetItemDamage;			// 2.00 (Abin)
	fnIsTwoHandItem					IsTwoHandItem;			// 2.00 (Abin)
	fnGetItemSize2					GetItemSize2;			// 2.00 (Abin)
	fnGetItemType					GetItemType;			// 2.00 (Abin)
	fnIsObjectDoor					IsObjectDoor;			// 2.00 (Abin)
	fnIsObjectChest					IsObjectChest;			// 2.00 (Abin)
	fnIsObjectShrine				IsObjectShrine;			// 2.00 (Abin)

} FUNCTIONENTRYPOINTS, *PFUNCTIONENTRYPOINTS;

#define	SIZE_FUNCTIONENTRYPOINTS	sizeof(FUNCTIONENTRYPOINTS)

//////////////////////////////////////////////////////////////////////
// PLAYERENTRYPOINTS
//
// Pointers to player-operation functions. Available since v2.00.
//
// Added by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////
typedef BOOL (__cdecl *fnHaveSpell)(WORD wSpellID); // Whether the player has a particular spell
typedef BOOL (__cdecl *fnIsInTown)();
typedef BOOL (__cdecl *fnGetMyMercUnit)(LPGAMEUNIT lpBuffer); // Get player's mercenary
typedef DWORD (__cdecl *fnGetID)(); // player ID
typedef LPCSTR (__cdecl *fnGetName)(); // Player name
typedef DWORD (__cdecl *fnGetMode)();	// Get player's current mode)(walking/running/attacking etc)
typedef WORD (__cdecl *fnGetSelectedSpell)(BOOL bLeftHand); // Get the selected spell
typedef BYTE (__cdecl *fnGetHPPercent)();	// Player HP percent 0-100
typedef BYTE (__cdecl *fnGetManaPercent)();	// Player mana percent 0-100
typedef BYTE (__cdecl *fnGetItemDurabilityPercent)();	// Player item durability percent 0-100
typedef DWORD (__cdecl *fnGetStat)(int nStatID);	// General stat access, hp/maxhp/mana/maxmana/mf/ias etc
typedef BOOL (__cdecl *fnGetAffection)(int nAffectionID);
typedef BOOL (__cdecl *fnSelectSpell)(WORD wSpell, BOOL bLeftHand);
typedef POINT (__cdecl *fnGetPosition)(); // player position
typedef int (__cdecl *fnGetMyCorpseCount)();
typedef BOOL (__cdecl *fnPickMyCorpse)();
typedef int (__cdecl *fnGetHostilerCount)(); // number of players that are hostiled to me
typedef BYTE (__cdecl *fnGetCurrentMapID)();
typedef DWORD (__cdecl *fnGetDistanceFrom)(long x, long y); // Player's distance from a position
typedef DWORD (__cdecl *fnHealMe)(BOOL bKeepBottomRow); // Drink a purple/red potion
typedef DWORD (__cdecl *fnHealMerc)(BOOL bKeepBottomRow); // Feed the merc a purple/red potion
typedef DWORD (__cdecl *fnSupplyMana)(BOOL bDrinkPurpleIfNoBlue, BOOL bKeepBottomRow); // Drink a blue/purple potion for mana
typedef BOOL (__cdecl *fnDrinkBeltPotion)(DWORD dwItemID, BOOL bFeedMerc);
typedef BOOL (__cdecl *fnDrinkInventoryPotion)(DWORD dwItemID);
typedef BYTE (__cdecl *fnGetBeltRows)(); // Row number of the player's belt)(1-4)
typedef DWORD (__cdecl *fnGetBeltItem)(int nSlot); // Belt item access, slot: 0-15
typedef BOOL (__cdecl *fnCastNoTarget)(WORD wSpell, BOOL bLeftHand);
typedef BOOL (__cdecl *fnCastOnMap)(WORD wSpell, long x, long y, BOOL bLeftHand);
typedef BOOL (__cdecl *fnCastOnUnit)(WORD wSpell, LPCGAMEUNIT lpUnit, BOOL bLeftHand);
typedef BOOL (__cdecl *fnLeaveGame)(); // save & exit
typedef void (__cdecl *fnPauseJobs)(BOOL bPause);
typedef void (__cdecl *fnCleanJobs)();
typedef BOOL (__cdecl *fnBackToTown)(); // Cast a town portal and go back to town immediately
typedef BOOL (__cdecl *fnMoveTo)(long x, long y, BOOL bQueueJob); // Run to a position
typedef BOOL (__cdecl *fnMoveToUnit)(LPCGAMEUNIT lpUnit, BOOL bQueueJob); // Run to a static object
typedef BOOL (__cdecl *fnTeleportTo)(long x, long y, BOOL bQueueJob); // teleport to a position
typedef BOOL (__cdecl *fnSay)(LPCSTR lpszMessage); // Say a message
typedef BOOL (__cdecl *fnOverHead)(LPCSTR lpszMessage, BOOL bClientOnly); // Display an over-head message
typedef BOOL (__cdecl *fnSendSound)(BYTE iSound); // Send a sound
typedef BOOL (__cdecl *fnAntiIdle)(); // Anti-idle
typedef BOOL (__cdecl *fnWeaponSwitch)(); // Do weapon switch
typedef BOOL (__cdecl *fnEnterWaypoint)(LPCGAMEUNIT lpWPUnit, BYTE iDestinationMapID); // take WP to another area
typedef BOOL (__cdecl *fnInteract)(LPCGAMEUNIT lpUnit);
typedef BOOL (__cdecl *fnPickGroundItem)(DWORD dwItemID, BOOL bShowClientMove); // Pick up a ground item
typedef BOOL (__cdecl *fnResurrectMerc)(); // Resurrect merc
typedef BOOL (__cdecl *fnBuyItem)(DWORD dwItemID); // Buy an item from npc
typedef BOOL (__cdecl *fnRepairItem)(DWORD dwItemID); // Repair items
typedef BOOL (__cdecl *fnIdentifyItems)();
typedef BOOL (__cdecl *fnDepositGoldToStash)(DWORD dwAmount); // Deposit inventory gold to stash
typedef BOOL (__cdecl *fnRedrawClient)(BOOL bFade); // Redraw client view
typedef DWORD (__cdecl *fnGetStashGoldLimit)(); // stash gold limit
typedef DWORD (__cdecl *fnGetInventoryGoldLimit)(); // inventory gold limit
typedef int (__cdecl *fnGetMaxDifficulty)();	// the max difficulty to which this player has access
typedef BOOL (__cdecl *fnIsHardcore)(); // is hardcore player?
typedef BOOL (__cdecl *fnGetAlwaysRun)(); // is "always run" on?
typedef DWORD (__cdecl *fnGetCursorItem)();
typedef BOOL (__cdecl *fnDropCursorItemToGround)();
typedef LPCSTR (__cdecl *fnGetAccountName)();
typedef BOOL (__cdecl *fnTransmute)();
typedef BOOL (__cdecl *fnIsUIOpened)(int nUIType);
typedef BOOL (__cdecl *fnEnumStorageItems)(int nStorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam);
typedef BOOL (__cdecl *fnPickStorageItemToCursor)(DWORD dwItemID);
typedef BOOL (__cdecl *fnFindFirstStorageSpace)(int nStorageType, SIZE minSize, LPPOINT lpBuffer);
typedef BOOL (__cdecl *fnDropCursorItemToStorage)(int nStorageType);
typedef void (__cdecl *fnCloseAllUIs)();
typedef BYTE (__cdecl *fnGetOpenedUI)();
typedef BOOL (__cdecl *fnBuyHealingPotion)(int nQuantity, DWORD dwDelay);
typedef BOOL (__cdecl *fnBuyManaPotion)(int nQuantity, DWORD dwDelay);
typedef BOOL (__cdecl *fnBuyTownScroll)(int nQuantity, DWORD dwDelay);
typedef BOOL (__cdecl *fnBuyIdentifyScroll)(int nQuantity, DWORD dwDelay);
typedef BOOL (__cdecl *fnBuyArrows)(int nQuantity, DWORD dwDelay);
typedef BOOL (__cdecl *fnBuyBolts)(int nQuantity, DWORD dwDelay);
typedef BOOL (__cdecl *fnSellItem)(DWORD dwItemID);
typedef BOOL (__cdecl *fnStartNpcSession)(LPCGAMEUNIT lpNpcUnit, int nInteractType);
typedef void (__cdecl *fnEndNpcSession)();
typedef int (__cdecl *fnDrinkAllInventoryPotions)(BOOL bKeepFullRejuv, DWORD dwDelay);
typedef BOOL (__cdecl *fnIsMercAlive)();
typedef BOOL (__cdecl *fnDropGold)(DWORD dwGoldAmount);
typedef BOOL (__cdecl *fnGamble)(DWORD dwItemID); // gamble an item
typedef DWORD (__cdecl *fnGetSpellCharges)(WORD wSpellID);
typedef int (__cdecl *fnGetUnidItemCount)();
typedef DWORD (__cdecl *fnGetEquipItem)(int nEquipLocation);
typedef int (__cdecl *fnGetAvailableBeltSlots)(int nColumn);
typedef BYTE (__cdecl *fnGetCurrentAct)();
typedef DWORD (__cdecl *fnGetStorageItem)(int nStorageType, int x, int y);
typedef int (__cdecl *fnSellInventoryItems)(int nLeftColumn, int nRightColumn, DWORD dwDelay);
typedef int (__cdecl *fnGambleAmulets)(DWORD dwDelay);
typedef int (__cdecl *fnGambleRings)(DWORD dwDelay);
typedef int (__cdecl *fnGambleCirclets)(DWORD dwDelay);
typedef int (__cdecl *fnGambleGloves)(DWORD dwDelay);
typedef int (__cdecl *fnGambleBoots)(DWORD dwDelay);
typedef int (__cdecl *fnGambleBelts)(DWORD dwDelay);
typedef BOOL (__cdecl *fnIsNpcSessionStarted)(DWORD dwNpcID, int nInteractType);
typedef BOOL (__cdecl *fnIsWeaponSwitched)();
typedef int (__cdecl *fnGetAngleFrom)(long x, long y);
typedef BOOL (__cdecl *fnPutInventoryPotionToBelt)(DWORD dwItemID);
typedef DWORD (__cdecl *fnGetClassID)();
typedef DWORD (__cdecl *fnGetPvpFlags)(DWORD dwPlayerID);
typedef DWORD (__cdecl *fnGetBaseStat)(int nStatID);

typedef struct tagPlayerEntryPoints
{
	fnHaveSpell						HaveSpell;				// 2.00 (Abin)
	fnIsInTown						IsInTown;				// 2.00 (Abin)
	fnGetMyMercUnit					GetMyMercUnit;			// 2.00 (Abin)
	fnGetID							GetID;					// 2.00 (Abin)
	fnGetName						GetName;				// 2.00 (Abin)
	fnGetMode						GetMode;				// 2.00 (Abin)
	fnGetSelectedSpell				GetSelectedSpell;		// 2.00 (Abin)
	fnGetHPPercent					GetHPPercent;			// 2.00 (Abin)
	fnGetManaPercent				GetManaPercent;			// 2.00 (Abin)
	fnGetItemDurabilityPercent		GetItemDurabilityPercent; // 2.00 (Abin)
	fnGetStat						GetStat;				// 2.00 (Abin)
	fnGetAffection					GetAffection;			// 2.00 (Abin)
	fnSelectSpell					SelectSpell;			// 2.00 (Abin)
	fnGetPosition					GetPosition;			// 2.00 (Abin)
	fnGetMyCorpseCount				GetMyCorpseCount;		// 2.00 (Abin)
	fnPickMyCorpse					PickMyCorpse;			// 2.00 (Abin)
	fnGetHostilerCount				GetHostilerCount;		// 2.00 (Abin)
	fnGetCurrentMapID				GetCurrentMapID;		// 2.00 (Abin)
	fnGetDistanceFrom				GetDistanceFrom;		// 2.00 (Abin)
	fnHealMe						HealMe;					// 2.00 (Abin)
	fnHealMerc						HealMerc;				// 2.00 (Abin)
	fnSupplyMana					SupplyMana;				// 2.00 (Abin)
	fnDrinkBeltPotion				DrinkBeltPotion;		// 2.00 (Abin)
	fnDrinkInventoryPotion			DrinkInventoryPotion;	// 2.00 (Abin)
	fnGetBeltRows					GetBeltRows;			// 2.00 (Abin)
	fnGetBeltItem					GetBeltItem;			// 2.00 (Abin)
	fnCastNoTarget					CastNoTarget;			// 2.00 (Abin)
	fnCastOnMap						CastOnMap;				// 2.00 (Abin)
	fnCastOnUnit					CastOnUnit;				// 2.00 (Abin)
	fnLeaveGame						LeaveGame;				// 2.00 (Abin)
	fnPauseJobs						PauseJobs;				// 2.00 (Abin)
	fnCleanJobs						CleanJobs;				// 2.00 (Abin)
	fnBackToTown					BackToTown;				// 2.00 (Abin)
	fnMoveTo						MoveTo;					// 2.00 (Abin)
	fnMoveToUnit					MoveToUnit;				// 2.00 (Abin)
	fnTeleportTo					TeleportTo;				// 2.00 (Abin)
	fnSay							Say;					// 2.00 (Abin)
	fnOverHead						OverHead;				// 2.00 (Abin)
	fnSendSound						SendSound;				// 2.00 (Abin)
	fnAntiIdle						AntiIdle;				// 2.00 (Abin)
	fnWeaponSwitch					WeaponSwitch;			// 2.00 (Abin)
	fnEnterWaypoint					EnterWaypoint;			// 2.00 (Abin)
	fnInteract						Interact;				// 2.00 (Abin)
	fnPickGroundItem				PickGroundItem;			// 2.00 (Abin)
	fnResurrectMerc					ResurrectMerc;			// 2.00 (Abin)
	fnBuyItem						BuyItem;				// 2.00 (Abin)
	fnRepairItem					RepairItem;				// 2.00 (Abin)
	fnIdentifyItems					IdentifyItems;			// 2.00 (Abin)
	fnDepositGoldToStash			DepositGoldToStash;		// 2.00 (Abin)
	fnRedrawClient					RedrawClient;			// 2.00 (Abin)
	fnGetStashGoldLimit				GetStashGoldLimit;		// 2.00 (Abin)
	fnGetInventoryGoldLimit			GetInventoryGoldLimit;	// 2.00 (Abin)
	fnGetMaxDifficulty				GetMaxDifficulty;		// 2.00 (Abin)
	fnIsHardcore					IsHardcore;				// 2.00 (Abin)
	fnGetAlwaysRun					GetAlwaysRun;			// 2.00 (Abin)
	fnGetCursorItem					GetCursorItem;			// 2.00 (Abin)
	fnGetAccountName				GetAccountName;			// 2.00 (Abin)
	fnDropCursorItemToGround		DropCursorItemToGround; // 2.00 (Abin)
	fnTransmute						Transmute;				// 2.00 (Abin)
	fnIsUIOpened					IsUIOpened;				// 2.00 (Abin)
	fnEnumStorageItems				EnumStorageItems;		// 2.00 (Abin)
	fnPickStorageItemToCursor		PickStorageItemToCursor; // 2.00 (Abin)
	fnFindFirstStorageSpace			FindFirstStorageSpace;	// 2.00 (Abin)
	fnDropCursorItemToStorage		DropCursorItemToStorage; // 2.00 (Abin)
	fnCloseAllUIs					CloseAllUIs;			 // 2.00 (Abin)
	fnGetOpenedUI					GetOpenedUI;			 // 2.00 (Abin)
	fnBuyHealingPotion				BuyHealingPotion;		 // 2.00 (Abin)
	fnBuyManaPotion					BuyManaPotion;			// 2.00 (Abin)
	fnBuyTownScroll					BuyTownScroll;			// 2.00 (Abin)
	fnBuyIdentifyScroll				BuyIdentifyScroll;		// 2.00 (Abin)
	fnBuyArrows						BuyArrows;				// 2.00 (Abin)
	fnBuyBolts						BuyBolts;				// 2.00 (Abin)
	fnSellItem						SellItem;				// 2.00 (Abin)
	fnStartNpcSession				StartNpcSession;		// 2.00 (Abin)
	fnEndNpcSession					EndNpcSession;			// 2.00 (Abin)
	fnDrinkAllInventoryPotions		DrinkAllInventoryPotions;	// 2.00 (Abin)
	fnIsMercAlive					IsMercAlive;			// 2.00 (Abin)
	fnDropGold						DropGold;				// 2.00 (Abin)
	fnGamble						Gamble;					// 2.00 (Abin)
	fnGetSpellCharges				GetSpellCharges;		// 2.00 (Abin)
	fnGetUnidItemCount				GetUnidItemCount;		// 2.00 (Abin)
	fnGetEquipItem					GetEquipItem;			// 2.00 (Abin)
	fnGetAvailableBeltSlots			GetAvailableBeltSlots;	// 2.00 (Abin)
	fnGetCurrentAct					GetCurrentAct;			// 2.00 (Abin)
	fnGetStorageItem				GetStorageItem;			// 2.00 (Abin)
	fnSellInventoryItems			SellInventoryItems;		// 2.00 (Abin)
	fnGambleAmulets					GambleAmulets;			// 2.00 (Abin)
	fnGambleRings					GambleRings;			// 2.00 (Abin)
	fnGambleCirclets				GambleCirclets;			// 2.00 (Abin)
	fnGambleGloves					GambleGloves;			// 2.00 (Abin)
	fnGambleBoots					GambleBoots;			// 2.00 (Abin)
	fnGambleBelts					GambleBelts;			// 2.00 (Abin)
	fnIsNpcSessionStarted			IsNpcSessionStarted;	// 2.00 (Abin)
	fnIsWeaponSwitched				IsWeaponSwitched;		// 2.00 (Abin)
	fnGetAngleFrom					GetAngleFrom;			// 2.00 (Abin)
	fnPutInventoryPotionToBelt		PutInventoryPotionToBelt;	// 2.00 (Abin)
	fnGetClassID					GetClassID;				// 2.00 (Abin)
	fnGetPvpFlags					GetPvpFlags;			// 2.00 (Abin)
	fnGetBaseStat					GetBaseStat;			// 2.00 (Abin)

} PLAYERENTRYPOINTS, *PPLAYERENTRYPOINTS;

#endif // __COMMONSTRUCTS_H__

