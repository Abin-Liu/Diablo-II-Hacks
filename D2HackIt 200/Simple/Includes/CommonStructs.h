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
// ITEMMOD
//--------------------------------------------------------------------
// Translated item properties(mods) data.
//////////////////////////////////////////////////////////////////////
typedef struct tagItemMod
{
	DWORD dwModID;			// Mod ID, could be different from property ID
	DWORD dwOriginalID;		// Original property ID
	int aValues[4];			// Mod values
	int nValueCount;		// ValueCount
	BOOL bFloat;			// Is float value
	BOOL bBoolean;			// Is Boolean value, which means aValues[0] is constant 1
} ITEMMOD, *LPITEMMOD;
typedef const tagItemMod* LPCITEMMOD;

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


#endif // __COMMONSTRUCTS_H__

