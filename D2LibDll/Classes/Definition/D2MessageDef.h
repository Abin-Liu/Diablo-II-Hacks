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
	OM_AURAON,			// Object activates or is affected by an aura, wParam: 1-activate, 0-affected, lParam: aura ID
	OM_AURAOFF,			// Object deactivates or is disaffected by an aura, wParam: 1-deactivate, 0-disaffected, lParam: aura ID
	OM_DESTROY,			// Object is gone from current player's vision, wParam: object ID, lParam: lower-word position x, higher-word position y

	///////////////////////////////////////////////////////////////////////
	// CD2StaticObject Messages
	///////////////////////////////////////////////////////////////////////
	SM_SPAWN,			// Static Object spawns, wParam: object ID, lParam: lower-word position x, higher-word position y

	///////////////////////////////////////////////////////////////////////
	// CD2Uint Message Definition
	///////////////////////////////////////////////////////////////////////
	UM_SPAWN,			// Unit spawns, wParam: object ID, lParam: lower-word position x, higher-word position y
	UM_STARTMOVE,		// Unit starts to move, wParam: lower-word original x, higher-word original y, lParam: lower-word destination x, higher-word destination y
	UM_ENDMOVE,			// Unit ends moving, wParam: lower-word original x, higher-word original y, lParam: lower-word current x, higher-word current y

	///////////////////////////////////////////////////////////////////////
	// CD2Monster Message Definition
	///////////////////////////////////////////////////////////////////////
	MM_SPAWN,			// Monster spawns, wParam: object ID, lParam: lower-word position x, higher-word position y
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
	PM_GOLD,			// Player gold amount update, wParam: inventory gold, lParam: stash gold
	PM_NPCSUCCESSFUL,	// Player interacted with an npc successfully, wParam: 0, lParam: npc ID
	PM_MAPBLINK,		// The map blinked (because of WP, TP, Teleport, etc), wParam: act, lParam: lower-word position x, higher-word position y
	PM_MAPCHANGE,		// The player has entered a new area, wParam: old area ID, lParam: new area ID 
	PM_ENTERTOWN,		// The player has entered town (safe areas), wParam: old area ID, lParam: new area ID
	PM_LEAVETOWN,		// The player has left town (unsafe areas), wParam: old area ID, lParam: new area ID
	PM_MOVECOMPLETE,	// The player has completed a moving action, wParam: 1 if successful, 0 if failed, lParam: lower-word position x, higher-word position y if successful, 0 if failed
	PM_AURAON,			// The player activates or is affected by an aura, wParam: 1-activate, 0-affected, lParam: aura ID
	PM_AURAOFF,			// The player deactivates or is disaffected by an aura, wParam: 1-deactivate, 0-disaffected, lParam: aura ID
	PM_WEAPONSWITCH,	// The player's weapon switched
	PM_TOWNFAIL,		// The "Back to town" operation has failed
	PM_INVENTORYFULL,	// Player's inventory is full, wParam: 0, lParam: 0

	///////////////////////////////////////////////////////////////////////
	// Available for Application Defined Messages
	///////////////////////////////////////////////////////////////////////
	DM_APP
};

#endif