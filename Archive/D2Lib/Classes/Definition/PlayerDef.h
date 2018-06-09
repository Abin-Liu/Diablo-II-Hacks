#ifndef __PLAYERDEF_H__
#define __PLAYERDEF_H__

#include <windows.h>
#include <tchar.h>

typedef struct tagD2Position
{
	WORD x;
	WORD y;
} D2POSITION, *LPD2POSITION;
typedef const tagD2Position* LPCD2POSITION;

/////////////////////////////////////////////////////////////////
// D2PLAYERDATA Struct Definition
/////////////////////////////////////////////////////////////////
typedef struct tagD2PlayerData
{
	DWORD dwPlayerID; // Current player's ID
	BYTE iCharClass; // Player's character class
	TCHAR szPlayerName[17]; // Player name
	const WORD* pPlayerXAddr; // Memory address where we find player's position x
	const WORD* pPlayerYAddr; // Memory address where we find player's position y
	const DWORD* pPlayerLocationAddr;  // Memory address where we find player's loaction type
	const BYTE* pPlayerActAddr; // Memory address where we find player's act
} D2PLAYERDATA, *LPD2PLAYERDATA;
typedef const tagD2PlayerData* LPCD2PLAYERDATA;

// Belt item categories
enum {  BELT_CATA_UNKNOWN = 0,  // None drinable items, scrolls or invalid items
		BELT_CATA_JUNKPOTION,	// Junky potions
		BELT_CATA_HEALING,		// Healing potions
		BELT_CATA_MANA,			// Mana potions
		BELT_CATA_REJUV };		// Rejuvenation potions

#define PLAYER_MAX_NAME		15 // Maximum character name length

///////////////////////////////////////////////////////////////////////
// MOVEJOB Struct Definition
///////////////////////////////////////////////////////////////////////
// Types of movement
enum { MJ_UNKNOWN = 0, MJ_POSITION, MJ_STATICOBJ, MJ_UNIT, MJ_PLAYER, MJ_ITEM, MJ_TELEPORT };
typedef struct tagMoveJob
{
	int nJobType; // Type of movement
	DWORD dwEntityID; // Destination object ID
	WORD x;	// Destination position x
	WORD y; // Destination position y
	BOOL bRun; // Run/walk
	BOOL bShowMove; // Show the movement on client screen?
} MOVEJOB, *LPMOVEJOB;
typedef const tagMoveJob* LPCMOVEJOB;

// Ground item
typedef struct tagGroundItem
{
	DWORD dwID; // Item ID
	WORD x; // position s
	WORD y; // position y
} GROUNDITEM, *LPGROUNDITEM;
typedef const tagGroundItem* LPCGROUNDITEM;

// Player related object
typedef struct tagPlayerObj
{
	DWORD dwID; // Player object ID
	WORD x;	// position x
	WORD y; // position y
} PLAYEROBJ, *LPPLAYEROBJ;
typedef const tagPlayerObj* LPCPLAYEROBJ;

BOOL operator==(const PLAYEROBJ& lhs, const PLAYEROBJ& rhs);
BOOL operator>(const PLAYEROBJ& lhs, const PLAYEROBJ& rhs);

int D2GetBeltItemCategory(LPCTSTR lpszItemCode);
DWORD D2GetBeltItemName(LPCTSTR lpszItemCode, LPTSTR lpszBuffer, DWORD dwMaxChars);
DWORD D2GetBeltItemName(DWORD dwCodeID, LPTSTR lpszBuffer, DWORD dwMaxChars);

#endif