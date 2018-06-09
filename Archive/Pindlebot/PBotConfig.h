//////////////////////////////////////////////////////////////////////
// PBotConfig.h
//
// Defines Pindlebot configuration structs and functions. 
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// August 8th, 2004
//////////////////////////////////////////////////////////////////////
#ifndef __PBOTCONFIG_H__
#define __PBOTCONFIG_H__

#include <windows.h>
#include <tchar.h>

#define PB_MAX_CHARNAME		15	// Maximum char name
#define PB_MAX_COMBO		64	// Maximum combo number
#define PB_MAX_ACTION		64	// Maximum actions
enum { PB_AMMO_NONE = 0, PB_AMMO_ARROW, PB_AMMO_BOLT, PB_AMMO_THROWING }; // ammo types
enum { PB_LOCATION_NORMAL = 0, PB_LOCATION_CLOSE, PB_LOCATION_FAR }; // Fight locations
enum { PB_TARGET_NONE = 0, PB_TARGET_PINDLE, PB_TARGET_FRONT, PB_TARGET_BEHIND }; // Spell casting target
enum { PB_ACTION_BEFORERED = 0, PB_ACTION_INRED, PB_ACTION_ONPINDLE, PB_ACTION_ONCORPSE }; // Action scheme types

// PBOTACTION struct, defines one action
typedef struct tagPBotAction
{
	WORD wSpellID;								// Spell ID
	BYTE iLeft;									// Left-hand/right-hand
	BYTE iTarget;								// Target type
	WORD wDelay;								// Cast delay
	BYTE iTimes;								// Repeat times
} PBOTACTION, *LPPBOTACTION;
typedef const tagPBotAction* LPCPBOTACTION;

// The bot configuration data
typedef struct tagPBotConfig
{
	TCHAR szCharName[PB_MAX_CHARNAME + 1];		// Character name
	BYTE iCharClass;							// Character class
	BYTE iMalahHP;								// HP % for visiting Malah
	BYTE iMalahMana;							// Mana % for visiting Malah
	BYTE iPotionHP;								// HP % for drinking potion
	BYTE iPotionMana;							// Mana % for drinking potion
	BYTE iQuitHP;								// HP % for quitting game
	BYTE iQuitMana;								// Mana % for quitting game
	BYTE iPetPotionHP;							// Pet HP % for feeding potion
	BOOL bQuitOnPetDeath;						// Quit if pet dies 
	BOOL bResurrectPet;							// Resurrect pet
	BYTE iRepairPercent;						// Repair when items durability percent < xx%
	int nLocationType;							// Position against Pindleskin, normal/close/far
	int nAmmoType;								// Ammo type, none/arrow/bolt
	DWORD aAvoidCombos[PB_MAX_COMBO];			// Deadly combos
	BYTE iComboCount;							// Deadly combo count
	PBOTACTION aBeforeRed[PB_MAX_ACTION];		// Actions before entering red portal
	BYTE iBeforeRedCount;						// Action count before entering red portal
	PBOTACTION aInRed[PB_MAX_ACTION];			// Actions after entering red portal
	BYTE iInRedCount;							// Action count after entering red portal
	PBOTACTION aOnPindle[PB_MAX_ACTION];		// Actiona for killing pindleskin
	BYTE iOnPindleCount;						// Actiona count for killing pindleskin
	PBOTACTION aOnCorpse[PB_MAX_ACTION];		// Actionas when there are corpses
	BYTE iOnCorpseCount;						// Action count when there are corpses
} PBOTCONFIG, *LPPBOTCONFIG;
typedef const tagPBotConfig* LPCPBOTCONFIG;

BOOL PBotLoadConfig(LPCTSTR lpszFile, LPPBOTCONFIG lpConfig);
BOOL PBotSaveConfig(LPCTSTR lpszFile, LPCPBOTCONFIG lpConfig);
DWORD PBotFindCombo(const WORD* aBonuses, LPCPBOTCONFIG lpConfig);

#endif // __PBOTCONFIG_H__