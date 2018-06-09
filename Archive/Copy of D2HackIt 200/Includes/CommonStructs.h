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
// D2 Game Map Position
////////////////////////////////////////////////////////////////////
typedef struct tagMapPos
{
	WORD x;				// x
	WORD y;				// y
} MAPPOS, *LPMAPPOS;
typedef const tagMapPos* LPCMAPPOS;

////////////////////////////////////////////////////////////////////
// Stuff for Screen Text Drawing
// Added v2.00 by Abin
////////////////////////////////////////////////////////////////////

// Drawing Capacities
#define D2DRAW_MAX_TEXT		255 // Maximum text length that can be drawn in one element
#define D2DRAW_MAX_DATA		64  // Maximum elements can be drawn on the screen at a time

// Drawing Element Data Definition
typedef struct tagD2DrawData
{
	char szText[D2DRAW_MAX_TEXT + 1]; // Text to be drawn
	BOOL bRightAlign;				  // Right alignment
	long x;							  // Screen coordinates x
	long y;							  // Screen coordinates y
	int nFont;						  // Text font(0-13)
} D2DRAWDATA, *LPD2DRAWDATA;
typedef const tagD2DrawData* LPCD2DRAWDATA;


// Miscs
#define	ITEM_CODE_LEN					3
#define	ITEM_DESC_LEN					128
#define ITEM_MAX_MOD	25
typedef struct tagItemMod
{
	DWORD dwModID;			// mod id
	BYTE iCount;			// number of values
	DWORD aValues[4];		// mod values
} ITEMMOD, *LPITEMMOD;
typedef const tagItemMod* LPCITEMMOD;

// The ITEM struct
typedef struct tagItem
{
	DWORD dwItemID;							// item ID
	BYTE iMessageID;						// 0x9c/0x9d
	BYTE iAction;							// action
	BYTE iType;								// type: weapon, armor, etc
	BYTE iIsSocketFull;						// is sockets full?
	BYTE iSocketNumber;						// number of total sockets on this item
	BYTE iSwtichIn;							// switched in?
	BYTE iSwitchOut;						// switched out?
	BYTE iBroken;							// item is broken
	BYTE iIdentified;						// identified?
	BYTE iEthereal;							// ethereal?
	BYTE iLevel;							// item level
	BYTE iLocation;							// ground, belt, inventory etc
	BYTE iPersonalized;						// personalized
	BYTE iRuneword;							// is runeword item
	BYTE iGamble;							// gamble
	BYTE iStorageID;						// storage id
	BYTE iQuality;							// unique, rare, set, superior, etc
	WORD wPositionX;						// location x
	WORD wPositionY;						// location y
	DWORD dwGoldAmount;						// Gold amount, "gld" only
	char szItemCode[ITEM_CODE_LEN + 1];		// item code, 3 letters
	WORD wDefense;							// defense
	WORD wDurability;						// durability
	WORD wMaxDurability;					// maximum durability
	BYTE iModCount;							// number of total mods
	BYTE iAtBeltRow;						// belt row index(0-4)
	BYTE iAtBeltColumn;						// belt column index(0-4)
	BYTE iGemmedNumber;						// gemmed number
	WORD wMagicPrefix;						// magic prefix
	WORD wMagicSuffix;						// magic suffix
	WORD wRarePrefix;						// rare prefix
	WORD wRareSuffix;						// rare suffix
	WORD wUniqueID;							// unique id
	WORD wSetID;							// set id
	//WORD wRuneWordID;						// runeword id
	ITEMMOD aMods[ITEM_MAX_MOD];			// mods
} ITEM, *LPITEM;
typedef const tagItem* LPCITEM;

//////////////////////////////////////////////////////////////////////
// Item Enumeration Callback Function def
//////////////////////////////////////////////////////////////////////
typedef BOOL (CALLBACK *fnEnumItemProc)(LPCITEM lpItem, LPARAM lParam); 

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
// Game Unit
//////////////////////////////////////////////////////////////////////
typedef struct tagGameUnit
{
	DWORD dwUnitID;					// Unit ID
	DWORD dwUnitType;				// Unit type(player, monster, object, etc)
} GAMEUNIT, *LPGAMEUNIT;
typedef const tagGameUnit* LPCGAMEUNIT;

// Unit enumeration callback definition
typedef BOOL (CALLBACK *fnEnumUnitProc)(LPCGAMEUNIT lpUnit, LPARAM lParam);

//////////////////////////////////////////////////////////////////////
// PresetUnit(Caves, Stairs, Dens, Towers, Entrances, Izual, Hephasto, Countess Chest, etc...)
//////////////////////////////////////////////////////////////////////
typedef struct tagPresetUnit
{
	DWORD dwID;			// Preset unit ID 
	DWORD dwType;		// Preset unit type, monster, object, room tile, etc
	WORD x;				// Map position x
	WORD y;				// Map position y
} PRESETUNIT, *LPPRESETUNIT;
typedef const tagPresetUnit* LPCPRESETUNIT;

// PresetUnit enumeration callback definition
typedef BOOL (CALLBACK *fnEnumPresetUnitProc)(LPCPRESETUNIT lpPresetUnit, LPARAM lParam);


/////////////////////////////////////////////////////////////////////
// Path (path from one position to an other)
/////////////////////////////////////////////////////////////////////
typedef struct tagPath
{
	MAPPOS posStart;					// Start position
	MAPPOS posEnd;						// Destination position
	MAPPOS aPathNodes[255];				// Path nodes between posStart and posEnd
	BYTE iNodeCount;					// Number of path nodes (0-255)
} PATH, *LPPATH;
typedef const tagPath* LPCPATH;


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
typedef MAPPOS (__cdecl *fnGetUnitPosition)(LPCGAMEUNIT lpUnit);
typedef DWORD (__cdecl *fnGetUnitName)(LPCGAMEUNIT lpUnit, LPSTR lpszBuffer, DWORD dwMaxChars);
typedef DWORD (__cdecl *fnGetUnitOwnerID)(LPCGAMEUNIT lpUnit);
typedef DWORD (__cdecl *fnGetUnitClassID)(LPCGAMEUNIT lpUnit);
typedef BYTE (__cdecl *fnGetUnitHPPercent)(LPCGAMEUNIT lpUnit);
typedef DWORD (__cdecl *fnGetUnitStat)(LPCGAMEUNIT lpUnit, int nStatID);
typedef BOOL (__cdecl *fnGetUnitAffection)(LPCGAMEUNIT lpUnit, int nAffectionID);
typedef int (__cdecl *fnGetMonsterType)(LPCGAMEUNIT lpUnit); // normal/champion/boss/minion
typedef DWORD (__cdecl *fnGetMonsterAttributes)(LPCGAMEUNIT lpUnit); // monster attributes
typedef DWORD (__cdecl *fnGetDistance)(WORD x1, WORD y1, WORD x2, WORD y2); // distance between two positions, in yards
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
typedef BOOL (__cdecl *fnParseItem)(const BYTE* aPacket, DWORD aLen, LPITEM lpItem); // Parses item info from a packet
typedef SIZE (__cdecl *fnGetItemSize)(LPCSTR lpszItemCode);
typedef int (__cdecl *fnGetGameDifficulty)(); // current game difficulty, normal/nm/hell
typedef DWORD (__cdecl *fnGetD2WindowTitle)(LPSTR lpszBuffer, DWORD dwMaxChars);
typedef DWORD (__cdecl *fnGetInGameTime)(); // time elapsed since joining
typedef LPCSTR (__cdecl *fnGetRealmName)();
typedef LPCSTR (__cdecl *fnGetGameName)();
typedef LPCSTR (__cdecl *fnGetGamePassword)();
typedef LPCSTR (__cdecl *fnGetGameIP)();

typedef BOOL (__cdecl *fnEnumPresetUnits)(fnEnumPresetUnitProc lpfnEnumPresetUnitProc, LPARAM lParam);
typedef BYTE (__cdecl *fnCalculatePath)(WORD x, WORD y, LPPATH lpBuffer, int nAdjust);
typedef BOOL (__cdecl *fnGetItemCode)(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars);
typedef BOOL (__cdecl *fnLeftClick)(long x, long y);
typedef BOOL (__cdecl *fnRightClick)(long x, long y);
typedef DWORD (__cdecl *fnQueryModuleData)(LPCSTR lpszModuleName, DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam);
typedef DWORD (__cdecl *fnGetKeyName)(BYTE iKCode, LPSTR lpszBuffer, DWORD dwMaxChars);
typedef BOOL (__cdecl *fnGetItemMod)(LPCITEM lpItem, DWORD dwModID, LPITEMMOD pMod);

typedef void (__cdecl *fnDebugTest)(); // For debug only

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

	fnDebugTest						DebugTest;

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
	fnParseItem						ParseItem;				// 2.00 (Abin)
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
	fnCalculatePath					CalculatePath;			// 2.00 (Abin)
	fnGetItemCode					GetItemCode;			// 2.00 (Abin)
	fnLeftClick						LeftClick;				// 2.00 (Abin)
	fnRightClick					RightClick;				// 2.00 (Abin)
	fnQueryModuleData				QueryModuleData;		// 2.00 (Abin)
	fnGetKeyName					GetKeyName;				// 2.00 (Abin)
	fnGetItemMod					GetItemMod;				// 2.00 (Abin)
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
typedef int (__cdecl *fnGetMode)();	// Get player's current mode)(walking/running/attacking etc)
typedef WORD (__cdecl *fnGetSelectedSpell)(BOOL bLeftHand); // Get the selected spell
typedef BYTE (__cdecl *fnGetHPPercent)();	// Player HP percent 0-100
typedef BYTE (__cdecl *fnGetManaPercent)();	// Player mana percent 0-100
typedef BYTE (__cdecl *fnGetItemDurabilityPercent)();	// Player item durability percent 0-100
typedef DWORD (__cdecl *fnGetStat)(int nStatID);	// General stat access, hp/maxhp/mana/maxmana/mf/ias etc
typedef BOOL (__cdecl *fnGetAffection)(int nAffectionID);
typedef BOOL (__cdecl *fnSelectSpell)(WORD wSpell, BOOL bLeftHand);
typedef MAPPOS (__cdecl *fnGetPosition)(); // player position
typedef int (__cdecl *fnGetMyCorpseCount)();
typedef BOOL (__cdecl *fnPickMyCorpse)();
typedef int (__cdecl *fnGetHostilerCount)(); // number of players that are hostiled to me
typedef BYTE (__cdecl *fnGetCurrentMapID)();
typedef DWORD (__cdecl *fnGetDistanceFrom)(WORD x, WORD y); // Player's distance from a position
typedef DWORD (__cdecl *fnHealMe)(); // Drink a purple/red potion
typedef DWORD (__cdecl *fnHealMerc)(); // Feed the merc a purple/red potion
typedef DWORD (__cdecl *fnSupplyMana)(BOOL bDrinkPurpleIfNoBlue); // Drink a blue/purple potion for mana
typedef BOOL (__cdecl *fnDrinkBeltPotion)(DWORD dwItemID, BOOL bFeedMerc);
typedef BOOL (__cdecl *fnDrinkInventoryPotion)(DWORD dwItemID);
typedef BOOL (__cdecl *fnIsBeltFull)();
typedef BYTE (__cdecl *fnGetBeltRows)(); // Row number of the player's belt)(1-4)
typedef LPCITEM (__cdecl *fnGetBeltItem)(int x, int y); // Belt item access
typedef BOOL (__cdecl *fnCastNoTarget)(WORD wSpell, BOOL bLeftHand);
typedef BOOL (__cdecl *fnCastOnMap)(WORD wSpell, WORD x, WORD y, BOOL bLeftHand);
typedef BOOL (__cdecl *fnCastOnUnit)(WORD wSpell, LPCGAMEUNIT lpUnit, BOOL bLeftHand);
typedef BOOL (__cdecl *fnLeaveGame)(); // save & exit
typedef void (__cdecl *fnPauseJobs)(BOOL bPause);
typedef void (__cdecl *fnCleanJobs)();
typedef BOOL (__cdecl *fnBackToTown)(); // Cast a town portal and go back to town immediately
typedef BOOL (__cdecl *fnMoveTo)(WORD x, WORD y, BOOL bQueueJob); // Run to a position
typedef BOOL (__cdecl *fnMoveToUnit)(LPCGAMEUNIT lpUnit, BOOL bQueueJob); // Run to a static object
typedef BOOL (__cdecl *fnTeleportTo)(WORD x, WORD y, BOOL bQueueJob); // teleport to a position
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
typedef BOOL (__cdecl *fnBuyHealingPotion)(int nQuantity);
typedef BOOL (__cdecl *fnBuyManaPotion)(int nQuantity);
typedef BOOL (__cdecl *fnBuyTownScroll)(int nQuantity);
typedef BOOL (__cdecl *fnBuyIdentifyScroll)(int nQuantity);
typedef BOOL (__cdecl *fnBuyArrows)(int nQuantity);
typedef BOOL (__cdecl *fnBuyBolts)(int nQuantity);
typedef BOOL (__cdecl *fnSellItem)(DWORD dwItemID);
typedef BOOL (__cdecl *fnStartNpcSession)(LPCGAMEUNIT lpNpcUnit, int nInteractType);
typedef void (__cdecl *fnEndNpcSession)();
typedef int (__cdecl *fnDrinkAllInventoryPotions)(BOOL bKeepFullRejuv);
typedef BOOL (__cdecl *fnIsMercAlive)();
typedef BOOL (__cdecl *fnDropGold)(DWORD dwGoldAmount);
typedef BOOL (__cdecl *fnGamble)(DWORD dwItemID); // gamble an item
typedef DWORD (__cdecl *fnGetSpellCharges)(WORD wSpellID);
typedef int (__cdecl *fnGetUnidItemCount)();
typedef BYTE (__cdecl *fnGetLowestEquipDurabilityPercent)(BOOL bIncludeSecondarySlots);
typedef LPCITEM (__cdecl *fnGetEquipItem)(int nEquipLocation);
typedef int (__cdecl *fnGetAvailableBeltSlots)(int nColumn);
typedef BYTE (__cdecl *fnGetCurrentAct)();
typedef LPCITEM (__cdecl *fnGetStorageItem)(int nStorageType, int x, int y);
typedef int (__cdecl *fnSellInventoryItems)(int nLeftColumn, int nRightColumn);
typedef int (__cdecl *fnGambleAmulets)();
typedef int (__cdecl *fnGambleRings)();
typedef int (__cdecl *fnGambleCirclets)();
typedef int (__cdecl *fnGambleGloves)();
typedef int (__cdecl *fnGambleBoots)();
typedef int (__cdecl *fnGambleBelts)();
typedef BOOL (__cdecl *fnIsNpcSessionStarted)(DWORD dwNpcID, int nInteractType);

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
	fnIsBeltFull					IsBeltFull;				// 2.00 (Abin)
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
	fnGetLowestEquipDurabilityPercent	GetLowestEquipDurabilityPercent; // 2.00 (Abin)
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

} PLAYERENTRYPOINTS, *PPLAYERENTRYPOINTS;


//////////////////////////////////////////////////////////////////////
// SCREENENTRYPOINTS
//
// Pointers to screen-operation functions. Available since v2.00.
//
// Added by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////
typedef DWORD (__cdecl *fnCreateScreenHook)();
typedef BOOL (__cdecl *fnReleaseScreenHook)(DWORD dwHookID);
typedef BOOL (__cdecl *fnSendToFront)(DWORD dwHookID);
typedef BOOL (__cdecl *fnSendToBack)(DWORD dwHookID);
typedef BOOL (__cdecl *fnDrawSingleText)(DWORD dwHookID, LPCSTR lpszText, long x, long y, int nFont, BOOL bRightAlign); // Draw one text
typedef BOOL (__cdecl *fnDrawMultipleTexts)(DWORD dwHookID, LPCD2DRAWDATA aData, int nDataCount); // Draw multiple texts
typedef SIZE (__cdecl *fnGetTextSize)(LPCSTR lpszText, int nFont); // Retrieves text size
typedef SIZE (__cdecl *fnGetScreenSize)(); // Get the game screen size	
typedef MAPPOS (__cdecl *fnScreenToMapCoords)(POINT ptScreenCoords);
typedef POINT (__cdecl *fnMapToScreenCoords)(MAPPOS mapPosition);
typedef POINT (__cdecl *fnGetMousePosition)();
typedef void (__cdecl *fnSetScrollTitle)(LPCSTR lpszTitle);
typedef BOOL (__cdecl *fnAddScrollLine)(LPCSTR lpszText);
typedef BOOL (__cdecl *fnShowScroll)();
typedef void (__cdecl *fnClearScroll)();

typedef struct tagScreenEntryPoints
{
	fnCreateScreenHook				CreateScreenHook;		// 2.00 (Abin)
	fnReleaseScreenHook				ReleaseScreenHook;		// 2.00 (Abin)
	fnSendToFront					SendToFront;			// 2.00 (Abin)
	fnSendToBack					SendToBack;				// 2.00 (Abin)
	fnDrawSingleText				DrawSingleText;			// 2.00 (Abin)
	fnDrawMultipleTexts				DrawMultipleTexts;		// 2.00 (Abin)
	fnGetTextSize					GetTextSize;			// 2.00 (Abin)
	fnGetScreenSize					GetScreenSize;			// 2.00 (Abin)
	fnScreenToMapCoords				ScreenToMapCoords;		// 2.00 (Abin)
	fnMapToScreenCoords				MapToScreenCoords;		// 2.00 (Abin)	
	fnGetMousePosition				GetMousePosition;		// 2.00 (Abin)
	fnSetScrollTitle				SetScrollTitle;			// 2.00 (Abin)
	fnAddScrollLine					AddScrollLine;			// 2.00 (Abin)
	fnShowScroll					ShowScroll;				// 2.00 (Abin)
	fnClearScroll					ClearScroll;			// 2.00 (Abin)
} SCREENENTRYPOINTS, *PSCREENENTRYPOINTS;


#endif // __COMMONSTRUCTS_H__

