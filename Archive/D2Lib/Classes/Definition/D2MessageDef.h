//////////////////////////////////////////////////////////////////////
// D2MessageDef.h
//
// This file defines common Diablo II event messages. Messages are generated
// when something meaningful happens in a Diablo II game, and are sent to the
// applications via callback functions, along with a WPARAM, a LPARAM, and a
// 32-bit application defined data LPVOID.
//
// Values of the WPARAM and the LPARAM depends on the message types.
// 
// This mechanism provides an "event-driving" interface to your applications,
// and your applications should handle the messages sent to them appropriately.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//
//////////////////////////////////////////////////////////////////////

#ifndef __D2MESSAGEDEF_H__
#define __D2MESSAGEDEF_H__

///////////////////////////////////////////////////////////////////////
// D2 Message Handler Function Definition
///////////////////////////////////////////////////////////////////////
typedef void (CALLBACK *D2MESSAGEPROC)(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData); // Notify a D2 message


///////////////////////////////////////////////////////////////////////
// D2 Message Definition
///////////////////////////////////////////////////////////////////////
enum {

	///////////////////////////////////////////////////////////////////////
	// CD2Abstract Messages
	///////////////////////////////////////////////////////////////////////
	DM_VOID	= 0,		// Placeholder

	///////////////////////////////////////////////////////////////////////
	// CD2Object Messages
	///////////////////////////////////////////////////////////////////////
	OM_SPAWN,			// Object spawns, wParam: object ID, lParam: lower-word position x, higher-word position y
	OM_DESTROY,			// Object is gone from current player's vision, wParam: object ID, lParam: 0
	OM_AFFECT,			// Object is affected by some states, wParam: 0-initiative, 1-passive, lParam: affection id
	OM_DISAFFECT,		// Object is disaffected by some states, wParam: 0-initiative, 1-passive, lParam: affection id

	///////////////////////////////////////////////////////////////////////
	// CD2Uint Message Definition
	///////////////////////////////////////////////////////////////////////
	UM_STARTMOVE,		// Unit starts to move, wParam: lower-word original x, higher-word original y, lParam: lower-word destination x, higher-word destination y
	UM_ENDMOVE,			// Unit ends moving, wParam: lower-word original x, higher-word original y, lParam: lower-word current x, higher-word current y

	///////////////////////////////////////////////////////////////////////
	// CD2Monster Message Definition
	///////////////////////////////////////////////////////////////////////
	MM_HPPERCENTUP,		// Monster HP percentage increases, wParam: original HP percent, lParam: current HP percent
	MM_HPPERCENTDOWN,	// Monster HP percentage decreases, wParam: original HP percent, lParam: current HP percent
	MM_ATTACK,			// Monster attacks, wParam: victim ID, lParam: lower-word position x, higher-word position y
	MM_UNDERATTACK,		// Monster under attack, wParam: attacker ID, lParam: lower-word position x, higher-word position y
	MM_DEATH,			// Monster dies, wParam: object ID, lParam: 0
	MM_CORPSE,			// Monster corpse appears, wParam: object ID, lParam: lower-word position x, higher-word position y
	MM_REVIVED,			// Monster is revived(e.g. fallen by shaman), wParam: 1-revived by others, 0-revived by itself,  lParam: lower-word position x, higher-word position y

	///////////////////////////////////////////////////////////////////////
	// CD2Player Message Definition
	///////////////////////////////////////////////////////////////////////
	PM_SPELLCHARGES,	// Player's spell charges changed (spells those can only be used for a particular times, e.g., Tome of town portal 20 charges), wParam: spell ID, lParam: charges left
	PM_LSPELL,			// Player's left-hand spell changed, wParam: old spell ID, lParam: new spell ID
	PM_RSPELL,			// Player's right-hand spell changed, wParam: old spell ID, lParam: new spell ID
	PM_LEVEL,			// Player's character level retrieved, wParam: 0, lParam: character level
	PM_HPUP,			// Player HP increases, wParam: original HP, lParam: current HP
	PM_HPDOWN,			// Player HP decreases, wParam: original HP, lParam: current HP
	PM_MANAUP,			// Player mana increases, wParam: original mana, lParam: current mana
	PM_MANADOWN,		// Player mana decreases, wParam: original mana, lParam: current mana
	PM_DEATH,			// Player dies, wParam: act, lParam: lower-word position x, higher-word position y
	PM_CORPSE,			// Player's corpse appears, wParam: corpse ID, lParam: lower-word position x, higher-word position y
	PM_CORPSEPICKEDUP,	// Player's corpse picked up, wParam: 0, lParam: 0
	PM_HOSTILE,			// Player is hostiled by other players, wParam: hostiler ID, lParam: hostiler count
	PM_UNHOSTILE,		// Player is unhostiled by other players, wParam: un-hostiler ID, lParam: hostiler count
	PM_NPCEND,			// Player ended interacting with an npc, wParam: 1-successful, 0-failed, lParam: npc ID
	PM_MAPBLINK,		// The map blinked (because of WP, TP, Teleport, etc), wParam: act, lParam: lower-word position x, higher-word position y
	PM_MAPCHANGE,		// The player has entered a new area, wParam: old area ID, lParam: new area ID 
	PM_ENTERTOWN,		// The player has entered town (safe areas), wParam: old area ID, lParam: new area ID
	PM_LEAVETOWN,		// The player has left town (unsafe areas), wParam: old area ID, lParam: new area ID
	PM_ACTCHANGE,		// Current act changed, wParam: original act, lParam: current act
	PM_MOVECOMPLETE,	// The player has completed a moving action, wParam: 1 if successful, 0 if failed, lParam: lower-word position x, higher-word position y if successful, 0 if failed
	PM_WEAPONSWITCH,	// The player's weapon switched
	PM_TOWNFAIL,		// The "Back to town" operation has failed
	PM_INVENTORYFULL,	// Player's inventory is full, wParam: 0, lParam: 0
	PM_EQUIPEVENT,		// Item event, wParam: event type, lParam: address of an ITEM struct
	PM_WPOPEN,			// The WP has been opened, wParam: act, lParam: map ID
	PM_STASHOPEN,		// The stash is opened, wParam: act, lParam: 0
	PM_GAMEREADY,		// Game is ready for operations, wParam: act, lParam: lower-word position x, higher-word position y

	///////////////////////////////////////////////////////////////////////
	// Available for Application Defined Messages
	///////////////////////////////////////////////////////////////////////
	DM_APP
};

#endif