//////////////////////////////////////////////////////////////////////
// MsgTest.cpp
// 
// Demonstrating the D2 Message System.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "..\\includes\\D2hackit.h"
#include "..\\includes\\D2client.h"
#include "..\\includes\\ClientCore.cpp"

CLIENTINFO
(		
	0,1,						// Module version (Major, Minor)
	"Abin",						// Author
	"www.wxjindu.com/abin/",	// url (http:// is appended)
	"MsgTest",			// Short module description
	"abinn32@yahoo.com"			// Author eMail
)

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc6 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	// Add your own commands here

	{NULL}	// No more commands
};

//////////////////////////////////////////////////////////////////////
// OnUnitMessage
// -------------------------------------------------------------------
// Executes when a game unit message is sent to the client
//
// Refer to the documentation for the format of unit messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	char szUnit[128] = "";
	sprintf(szUnit, "Unit %08X/%d ", lpUnit->dwUnitID, lpUnit->dwUnitType);

	switch (nMessage)
	{
	case UM_HPPERCENT:
		break;

	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// OnThisPlayerMessage
// -------------------------------------------------------------------
// Executes when a this-player message is sent to the client
//
// Refer to the documentation for the format of this-player messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	char sz1[256] = "";
	char sz2[256] = "";

	// Process all received messages
	switch (nMessage)
	{
	case PM_AFFECT:

		// Player affected by some affections.
		if (server->GetAffectionName(lParam, sz1, 255))
			server->GameInfof("Player affected by \"%s\"", sz1);
		break;

	case PM_DISAFFECT:

		// Player no longer affected by some affections
		if (server->GetAffectionName(lParam, sz1, 255))
			server->GameInfof("Player no longer affected by \"%s\"", sz1);
		break;

	case PM_SPELLSELECT:		

		// Left-hand spell change
		server->GetSpellName(lParam, sz1, 255);
		server->GameInfof("%s-hand spell: \"%s\".", wParam ? "Left" : "Right", sz1);
		break;

	case PM_LEVEL:

		// Player level changed
		server->GameInfof("Player level: %d", lParam);
		break;

	case PM_HPUP:

		// Player life up
		server->GameInfof("Player HP up %d-%d", wParam, lParam);
		break;

	case PM_HPDOWN:

		// Player life down
		server->GameInfof("Player HP down %d-%d", wParam, lParam);
		break;

	case PM_MANAUP:

		// Player mana up
		server->GameInfof("Player mana up %d-%d", wParam, lParam);
		break;

	case PM_MANADOWN:

		// Player mana down
		server->GameInfof("Player mana down %d-%d", wParam, lParam);
		break;
		
	case PM_DEATH:

		// Player die
		server->GameInfof("Player is killed");
		break;

	case PM_CORPSE:
	
		// Player's corpse appeared
		server->GameInfof("Player corpse %08X appeared at %04X, %04X", wParam, LOWORD(lParam), HIWORD(lParam));
		break;

	case PM_CORPSEPICKEDUP:

		// Player picked up corpse
		server->GamePrintInfo("Player picked up corpse");
		break;

	case PM_HOSTILE:

		// Hostiled by other player
		server->GameInfof("Hostiled by %08X, Current hostilers %d", wParam, lParam);
		break;

	case PM_UNHOSTILE:

		// Unhostiled by other player
		server->GameInfof("Unhostiled by %08X, Current hostilers %d", wParam, lParam);
		break;

	case PM_MAPBLINK:

		// Player's map blinked
		server->GameInfof("Map blinked at act %d, %04X, %04X", wParam, LOWORD(lParam), HIWORD(lParam));
		break;

	case PM_NPCSESSION:

		// Player's map blinked
		server->GameInfof("Npc session with %08X %s.", lParam, wParam ? "started" : "failed");
		break;

	case PM_MAPCHANGE:

		// Player entered a new map
		server->GetMapName(wParam, sz1, 255);
		server->GetMapName(lParam, sz2, 255);
		server->GameInfof("Player map changed: \"%s\" - \"%s\"", sz1, sz2);
		break;

	case PM_ENTERTOWN:

		// Player entered town
		server->GamePrintInfo("Player entered town");
		break;

	case PM_LEAVETOWN:

		// Player left town
		server->GamePrintInfo("Player left town");
		break;

	case PM_ACTCHANGE:

		// Player left town
		server->GameInfof("Act changed: %d-%d", wParam, lParam);
		break;

	case PM_MOVECOMPLETE:

		// Player finished moving
		if (wParam)
			server->GamePrintInfo("Player move completed");
		else
			server->GamePrintError("Player move failed.");
		break;

	case PM_WEAPONSWITCH:

		// Player weapon switched
		server->GamePrintInfo("Player weapon switched");
		break;

	case PM_TOWNFAIL:
		
		// The previous "back to town" operation has failed
		server->GamePrintError("Back-to-town failed!");
		break;

	case PM_INVENTORYFULL:

		// Player's inventory is full
		server->GamePrintInfo("Player's inventory is full.");
		break;

	case PM_WPOPEN:
		server->GetMapName(lParam, sz1, 255);
		server->GameInfof("WP opened at act %d, %s", wParam, sz1);
		break;

	case PM_STASHOPEN:
		server->GameInfof("Stash opened at act %d", wParam);
		break;

	case PM_GAMEREADY:
		server->GameInfof("Game is ready");
		break;

	case PM_INVITATION:
		server->GameInfof("Player %08X invites you to join a party.", wParam);
		break;

	case PM_JOINPARTY:
		server->GameInfof("Joined player %08X's party.", wParam);
		break;

	case PM_UICLOSED:
		server->GameInfof("UI closed.");
		break;

	default:
		break;
	}
}