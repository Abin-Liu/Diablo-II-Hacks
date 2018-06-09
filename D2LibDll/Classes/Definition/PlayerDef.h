#ifndef __PLAYERDEF_H__
#define __PLAYERDEF_H__

#include <windows.h>
#include <tchar.h>

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


/////////////////////////////////////////////////////////////////
// belt item type definitions
/////////////////////////////////////////////////////////////////

// Misc Potions
#define POTION_STAMINA				0x07370760
#define POTION_ANTIDOTE				0x07370790
#define POTION_THAWING				0x0736D770

// Healing Potions
#define POTION_MINOR_HEALING		0x03170680
#define POTION_LIGHT_HEALING		0x03270680
#define POTION_HEALING				0x03370680
#define POTION_GREATER_HEALING		0x03470680
#define POTION_SUPER_HEALING		0x03570680

// Mana Potions
#define POTION_MINOR_MANA			0x031706D0
#define POTION_LIGHT_MANA			0x032706D0
#define POTION_MANA					0x033706D0
#define POTION_GREATER_MANA			0x034706D0
#define POTION_SUPER_MANA			0x035706D0

// Rejuvenate Potions
#define POTION_REJUVENATE			0x07376720
#define POTION_FULL_REJUVENATE		0x06C76720

// Scrolls
#define SCROLL_IDENTIFY				0x06373690
#define SCROLL_TOWN_PORTAL			0x06373740

// Belt item categories
enum {  BELT_CATA_UNKNOWN = 0,  // None drinable items, scrolls or invalid items
		BELT_CATA_JUNKPOTION,	// Junky potions
		BELT_CATA_HEALING,		// Healing potions
		BELT_CATA_MANA,			// Mana potions
		BELT_CATA_REJUV };		// Rejuvenation potions

// Items that can be put on belt
typedef struct tagBeltItem
{	
	DWORD dwID; // item id
	DWORD dwType; // type
} BELTITEM, *LPBELTITEM;
typedef const tagBeltItem* LPCBELTITEM;

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
	BOOL bVerifySpell; // Verify spell before cast(teleport)
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
int D2GetBeltItemCategory(DWORD dwItemType);
DWORD D2GetBeltItemName(DWORD dwItemType, LPTSTR lpszBuffer, DWORD dwMaxChars);
DWORD D2FormatPacket(BOOL bReceived, const BYTE* aPacket, DWORD aLen, LPTSTR lpszBuffer, DWORD dwMaxChars);

#endif