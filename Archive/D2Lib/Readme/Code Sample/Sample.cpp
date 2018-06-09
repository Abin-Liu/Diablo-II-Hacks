//////////////////////////////////////////////////////////////////////
// D2Lib_Test.cpp
// -------------------------------------------------------------------
// This is a simple D2H module for demonstrating some basic features
// of the D2Lib library.
//
// In this module, I will be maintaining two D2Lib objects: a CD2Player
// object which represent "me", and a CD2Merc object which represents
// "my pet". D2 message handlers for those two objects are "OnPlayerMessage"
// and "OnPetMessage", as you will see below. 
//
// D2 messages will be sent to this module and be processed by our message
// handlers, I use simply display some brief descriptions for each message
// we receive, except those frequently-sent messages such as MM_ATTACK, MM_STARTMOVE,
// PM_MANA_UP, etc, that the screen will be fullfilled if we display them.
//
// In this module, when the player's life drops down below 70%, it will call
// "Me.HealMe();" to drink a life potion; When the pet's life drops down
// below 50%, it will call "Me.HealMerc()" to feed the pet a life potion.  
//
// Also, if you invoke the "OnGameCommandDebug" function, it will call
// "Me.BackToTown();" so the player will cast a town portal and back to town. 
//
// That's all this module does, however, the D2Lib provides far more
// features and functionalities than what can be ever shown here, so please
// refer to my D2Lib documents to learn more about this library.
//
// Written by Abin (abinn32@yahoo.com)
//
// 7/26/04
//////////////////////////////////////////////////////////////////////

#include "..\\D2lib\\D2Lib.h"
#pragma comment(lib, "..\\D2lib\\D2Lib.lib") // Import D2Lib
#include "..\\D2Lib\\ClientCore.cpp"

CLIENTINFO
(		
	0,0,						// Module version (Major, Minor)
	"Abin",						// Author
	"www.wxjindu.com/abin/",	// url (http:// is appended)
	"Sample plugin",			// Short module description
	"abinn32@yahoo.com"			// Author eMail
)

////////////////////////////////////////////////////////////////
// D2Lib Objects
////////////////////////////////////////////////////////////////
CD2Player Me; // Represents the player
CD2Merc Pet; // Represents the player's pet

BOOL PRIVATE OnCommandDebug(char **argv, int argc);
MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc6 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	{
		"debug",
		OnCommandDebug,
		"debugÿc6 Internal usage.\n"
	},

	// Add your own commands here

	{NULL}	// No more commands
};


////////////////////////////////////////////////////////////////////////////
// D2 Message Handler for Me (The CD2Player Object)
////////////////////////////////////////////////////////////////////////////
void CALLBACK OnPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	char sz1[256] = "";
	char sz2[256] = "";
	D2SPELLINFO dsi = { 0 };

	// Process all received messages
	switch (nMessage)
	{
	case PM_SPELLCHARGES:

		// Spell charges change
		::D2GetSpellInfo(wParam, &dsi);
		::D2Infof("Spell \"%s\", %d charges left.", dsi.szSpellName, lParam);
		break;

	case PM_LSPELL:

		// Left-hand spell change
		::D2GetSpellInfo(lParam, &dsi);
		::D2Infof("Left-hand spell: \"%s\".", dsi.szSpellName);
		break;

	case PM_RSPELL:

		// Right-hand spell change
		::D2GetSpellInfo(lParam, &dsi);
		::D2Infof("Right-hand spell: \"%s\".", dsi.szSpellName);
		break;

	case PM_LEVEL:

		// Player level changed
		::D2Infof("Player level: %d", lParam);
		break;

	case PM_HPUP:

		// Player life up
		::D2Infof("Player HP up %d %d/%d (%d%%)",
			lParam,
			Me.GetPlayerStats(D2PS_HP),
			Me.GetPlayerStats(D2PS_MAXHP),
			Me.GetHPPercent());
		break;

	case PM_HPDOWN:

		// Player life down
		::D2Infof("Player HP down %d %d/%d (%d%%)",
			lParam,
			Me.GetPlayerStats(D2PS_HP),
			Me.GetPlayerStats(D2PS_MAXHP),
			Me.GetHPPercent());

		// if life down below 70%, drink a poition		
		if (Me.GetHPPercent() < 70)
		{
			DWORD dwResult = Me.HealMe();
			if (dwResult != 0)
			{
				::D2GetBeltItemName(dwResult, sz1, 255);
				::D2Infof("Player drank a \"%s\" successfully.", sz1);
			}
			else
			{
				server->GamePrintError("Unable to drink any potion");
			}
		}
		break;

	case PM_MANAUP:

		// Player mana up
		::D2Infof("Player mana up %d %d/%d (%d%%)",
			lParam,
			Me.GetPlayerStats(D2PS_MANA),
			Me.GetPlayerStats(D2PS_MAXMANA),
			Me.GetManaPercent());
		break;

	case PM_MANADOWN:

		// Player mana down
		::D2Infof("Player mana down %d %d/%d (%d%%)",
			lParam,
			Me.GetPlayerStats(D2PS_MANA),
			Me.GetPlayerStats(D2PS_MAXMANA),
			Me.GetManaPercent());
		break;

	case PM_DEATH:

		// Player die
		::D2Infof("Player is killed in act %d, %04X, %04X", wParam + 1, LOWORD(lParam), HIWORD(lParam));
		break;

	case PM_CORPSE:
	
		// Player's corpse appeared
		::D2Infof("Player corpse %08X appeared at %04X, %04X", wParam + 1, LOWORD(lParam), HIWORD(lParam));
		break;

	case PM_CORPSEPICKEDUP:

		// Player picked up corpse
		server->GamePrintInfo("Player picked up corpse");
		break;

	case PM_HOSTILE:

		// Hostiled by other player
		::D2Infof("Hostiled by %08X, Current hostilers %d", wParam, lParam);
		break;

	case PM_UNHOSTILE:

		// Unhostiled by other player
		::D2Infof("Unhostiled by %08X, Current hostilers %d", wParam, lParam);
		break;

	case PM_MAPBLINK:

		// Player's map blinked
		break;

	case PM_MAPCHANGE:

		// Player entered a new map
		::D2GetMapName(wParam, sz1, 255);
		::D2GetMapName(lParam, sz2, 255);
		::D2Infof("Player map changed: \"%s\" - \"%s\"", sz1, sz2);
		break;

	case PM_ENTERTOWN:

		// Player entered town
		server->GamePrintInfo("Player entered town");
		break;

	case PM_LEAVETOWN:

		// Player left town
		server->GamePrintInfo("Player left town");
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

	case PM_EQUIPEVENT:

		// Player equipments changed
		if (lParam)
			::D2Infof("Equipment event: %d, %s", wParam, ((LPCITEM)lParam)->szItemCode);
		break;

	case OM_AFFECT:

		// Player affected by some affections.
		if (::D2GetAffectionName(lParam, sz1, 255))
			::D2Infof("Player affected by \"%s\"", sz1);
		break;

	case OM_DISAFFECT:

		// Player no longer affected by some affections
		if (::D2GetAffectionName(lParam, sz1, 255))
			::D2Infof("Player no longer affected by \"%s\"", sz1);
		break;

	case PM_WPOPEN:
		::D2GetMapName(lParam, sz1, 255);
		::D2Infof("WP opened at act %d, %s", wParam + 1, sz1);
		break;

	case PM_STASHOPEN:
		::D2Infof("Stash opened at act %d", wParam + 1);
		break;

	default:
		break;
	}
}


////////////////////////////////////////////////////////////////////////////
// D2 Message Handler for Pet (The CD2Merc Object)
////////////////////////////////////////////////////////////////////////////
void CALLBACK OnPetMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	char szMessage[256] = "";
	char sz1[256] = "";
	char sz2[256] = "";

	switch (nMessage)
	{
	case OM_SPAWN:

		// Pet spawned
		sprintf(szMessage, "Pet spawned, ID %08X, location %04X, %04X", wParam, LOWORD(lParam), HIWORD(lParam));
		server->GamePrintInfo(szMessage);
		break;

	case MM_HPPERCENTUP:

		// Pet HP up
		sprintf(szMessage, "Pet life up to %d%%", lParam);
		server->GamePrintInfo(szMessage);
		break;

	case MM_HPPERCENTDOWN:

		// Pet HP down
		sprintf(szMessage, "Pet life down to %d%%", lParam);
		server->GamePrintInfo(szMessage);

		// if pet's life down below 50%, feed him a poition
		if (lParam < 50)
		{
			DWORD dwResult = Me.HealMerc();
			if (dwResult != 0)
			{
				::D2GetBeltItemName(dwResult, sz1, 255);
				sprintf(szMessage, "Fed the pet a \"%s\" successfully.", sz1);
				server->GamePrintInfo(szMessage);
			}
			else
			{
				server->GamePrintError("Unable to feed the pet any potion");
			}
		}

		break;

	case MM_ATTACK:

		// Pet attacks some object
		break;

	case MM_UNDERATTACK:

		// Pet is attacked by some object
		break;

	case MM_DEATH:

		// Pet dies
		server->GamePrintInfo("Pet died.");
		break;

	case MM_CORPSE:

		// Pet's corpse appeared
		sprintf(szMessage, "Pet's corpse appeared at %04X, %04X", LOWORD(lParam), HIWORD(lParam));
		server->GamePrintInfo(szMessage);
		break;

	case OM_DESTROY:

		// Pet is gone
		break;

	case UM_STARTMOVE:

		// Pet started to move
		break;

	case UM_ENDMOVE:

		// Pet ended moving
		break;

	case OM_AFFECT:

		// Pet is affected by some affections
		if (::D2GetAffectionName(lParam, sz1, 255))
			::D2Infof("Pet affected by \"%s\"", sz1);
		break;

	case OM_DISAFFECT:

		// Pet is no longer affected by some affections
		if (::D2GetAffectionName(lParam, sz1, 255))
			::D2Infof("Pet no longer affected by \"%s\"", sz1);
		break;

	default:
		break;
	}
}

BOOL EXPORT OnClientStart()
{
	// Set the message handlers
	Me.SetD2MessageProc(OnPlayerMessage);
	Pet.SetD2MessageProc(OnPetMessage);
	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	// Just let D2Lib process the event
	Me.OnGameJoin(game);
	Pet.OnGameJoin(game);

	// Display a warning message if the module was not loaded before joining game
	Me.DisplayNotLoadedBeforeJoinWarning("D2Lib_Sample");
	return;
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* game)
{
	// Just let D2Lib process the event
	Me.OnGameLeave();
	Pet.OnGameLeave();
	return;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	// Just let D2Lib process the event
	Me.OnGamePacketBeforeSent(aPacket, aLen);
	Pet.OnGamePacketBeforeSent(aPacket, aLen);
	return aLen;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	// Just let D2Lib process the event
	Me.OnGamePacketBeforeReceived(aPacket, aLen);
	Pet.OnGamePacketBeforeReceived(aPacket, aLen);
	return aLen;
}


DWORD EXPORT OnGameTimerTick(void)
{
	// Just let D2Lib process the event
	Me.OnGameTimerTick();
	Pet.OnGameTimerTick();
	return 0;
}

BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	Me.BackToTown(); // Cast a TP and back to town
	return TRUE;
}