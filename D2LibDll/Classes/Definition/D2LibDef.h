#ifndef __D2LIBDEF_H__
#define __D2LIBDEF_H__

#include "Definition\\D2MessageDef.h"
#include "Definition\\SpellDef.h"
#include "Definition\\MapDef.h"
#include "Definition\\AuraDef.h"
#include "Definition\\KeyDef.h"
#include "Definition\\PlayerDef.h"

#define D2LIB_MAX_SPELLS			128
#define D2LIB_MAX_HOSTILERS			8
#define D2LIB_MAX_CORPSES			16
#define D2LIB_MAX_BELTROWCOL		4
#define D2LIB_MAX_MAPDATA			32
#define D2LIB_MAX_MOTIONJOBS		255

typedef struct tagD2BasicObj
{
	DWORD dwObjID;
	WORD x;
	WORD y;
} D2BASICOBJ, *LPD2BASICOBJ;
typedef tagD2BasicObj* LPCD2BASICOBJ;

typedef struct tagD2AbstractData
{
	BOOL m_bDebug; // Whether to display debug messages
	D2PLAYERDATA m_thisPlayer; // Data for current game player
	D2MESSAGEPROC m_lpfnMessageProc; // The message handler
	LPVOID m_lpMessageData; // 32-bit application data, sent along with object messages
	BOOL m_bLoadedBeforeJoin; // is module loaded before joining a game?
} D2ABSTRACTDATA, *LPD2ABSTRACTDATA;
typedef const tagD2AbstractData* LPCD2ABSTRACTDATA;


typedef struct tagD2ObjectData
{
	DWORD m_dwObjectID; // Object ID, unique in a game
	WORD m_wType; // Object's type
	WORD m_wPositionX; // Object's current location X
	WORD m_wPositionY; // Object's current location X
	BOOL m_bLockSpawn; // Prevent the object from spawning
	BYTE m_iAuraActivated; // Actived aura
	BYTE m_iAuraAffected; // Affected by aura
	D2OBJFINGERPRINTER m_fingerprinter; // Fingerprinter
} D2OBJECTDATA, *LPD2OBJECTDATA;
typedef const tagD2ObjectData* LPCD2OBJECTDATA;



typedef tagD2UnitData
{
	BOOL m_bIsMoving; // Is the unit moving?
} D2UNITDATA, *LPD2UNITDATA;
typedef const tagD2UnitData* LPCD2UNITDATA;


typedef tagD2MonsterData
{
	BOOL m_bIsAlive; // Whether the Monster is alive
	BOOL m_bIsCorpse; // Whether the Monster's corpse has appeared
	BYTE m_iHPPercent; // Monster HP remaining percent
} D2MONSTERDATA, *LPD2MONSTERDATA;
typedef const tagD2MonsterData* LPCD2MONSTERDATA;


typedef tagD2PlayerData
{
	WORD aBaseSpells[D2LIB_MAX_SPELLS]; // Player spells
	WORD aChargeableSpells[D2LIB_MAX_SPELLS]; // Charged spells
	WORD aHostilers[D2LIB_MAX_HOSTILERS]; // Hostiler IDs
	WORD aCorpses[D2LIB_MAX_CORPSES]; // Player corpses, up to 16 in 1.10
	MOVEJOB aJobs[D2LIB_MAX_MOTIONJOBS]; // Motion jobs
	MOVEJOB m_prevJob; // Previous job
	BYTE m_iCurMap; // Current map ID
	WORD m_wHP; // Current HP amount
	WORD m_wMana; // Current mana amount
	WORD m_wMaxHP; // Base HP amount
	WORD m_wMaxMana; // Base mana amount
	DWORD m_dwInventoryGold; // Inventory gold
	DWORD m_dwStashGold; // Stash gold
	WORD m_wLeftSpell; // Left-hand spell
	WORD m_wRightSpell; // Right-hand spell
	D2SPELLINFO m_lastD2s; // Last spell info
	BYTE m_iCharLevel; // Character level
	BYTE m_iAuraActivated; // Actived aura
	BYTE m_iAuraAffected; // Affected by aura
	BELTITEM m_items[D2LIB_MAX_BELTROWCOL][D2LIB_MAX_BELTROWCOL];	// Items in belt
	BYTE m_iBeltRows; // Current belt row number
	CTicker m_ticker; // Timer Ticker
	BOOL m_bMapBlinked; // Was previous teleport OK?
	int m_nTeleportFail; // How many times the previous teleport has failed
	int m_nTPState; // "Back to town" state
	int m_nTPMaxRetry; // Maximum retry times
	int m_nTPCastfail; // TP casting failure count
	int m_nTPEnterFail; // TP entering failure count
	PLAYEROBJ m_portal; // For "back to town" usage
	DWORD m_dwLastSync; // The last time when "RedrawClient" is called
	MAPDATA aMapData[D2LIB_MAX_MAPDATA]; // Map data list
	MAPCOUNT aMapCounts[D2LIB_MAX_MAPDATA]; // Map area list
} D2PLAYERDATA, *LPD2PLAYERDATA;
typedef const tagD2PlayerData* LPCD2PLAYERDATA;


#endif