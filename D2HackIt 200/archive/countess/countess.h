////////////////////////////////////////////////////////////////////////////
#include "..\\includes\\D2hackit.h"
#include "..\\includes\\D2client.h"
#include "utilities\\StateTicker.h"

////////////////////////////////////////////////////////////////////////////
#define STOP					0
#define GO_TO_WP				29
#define ENTER_BLACK_MARSH		30
#define CAST_PROTECTION1		31
#define CAST_PROTECTION2		32
#define CAST_PROTECTION3		33
#define FIND_COUNTESS			34
#define KILL_COUNTESS			35
#define COUNTESS_DEAD			36
#define TELE_TO_COUNTESS		37
#define CAST_SKILL				47
#define PICK_UP_ITEMS			48
#define BACK_TO_TOWN			49
#define FINDCAVE				50
#define CALCPATH				51
#define TELEPORT_TO_DEST		52
#define FIND_STAIRS				53
#define ENTER_DEST				54
#define CHECK_WP				55
#define WAIT_TIME_OUT			99


/////////////////////////////////////////////////////////////////////////////
CLIENTINFO
(
	0,0,						// Module version (Major, Minor)
	"abin,ustc_tweeg",						// Author
	"",	// url (http:// is appended)
	"Sample plugin",			// Short module description
	""			// Author eMail
)

/////////////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnCommandDebug(char **argv, int argc);
BOOL PRIVATE OnCommandStart(char **argv, int argc);
BOOL PRIVATE OnCommandStop(char **argv, int argc);
BOOL PRIVATE OnCommandPause(char **argv, int argc);
BOOL PRIVATE OnCommandResume(char **argv, int argc);

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",	OnGameCommandHelp,
		"helpÿc6 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{
		"debug", OnCommandDebug,
		"debugÿc6 Internal usage.\n"
	},

	{
		"start", OnCommandStart,
		"startÿc6 Start the countess bot.\n"
	},

	{
		"stop", OnCommandStop,
		"stopÿc6 Stop the countess bot.\n"
	},

	{
		"pause", OnCommandPause,
		"pauseÿc6 Pause the countess bot.\n"
	},

	{
		"resume", OnCommandResume,
		"resumeÿc6 Resume the countess bot.\n"
	},

	{NULL}	// No more commands
};

///////////////////////////////////////////////////////////////////////////////
CStateTicker g_ticker; // game controling ticker

// required units
GAMEUNIT g_waypoint = {0}; // act1 wp
GAMEUNIT g_countess = {0}; // the woman
GAMEUNIT g_stairs = {0}; // the stairs
DWORD g_dwDistanceToStairs = 0; // distance to destination
PRESETUNIT g_destination = {0}; // destination stairs/caves/chest etc
POINT g_path[255] = { 0 }; // path to destination
DWORD g_dwPathCount = 0;

void ResetBot();
DWORD FindStairs();
DWORD FindCountess();
BOOL FindValuableItems();
void Abort(LPCSTR lpszReason, BOOL bIsFailed = TRUE);
BOOL CALLBACK EnumPresetUnitProc(LPCPRESETUNIT lpPresetUnit, LPARAM lParam);
BOOL CALLBACK EnumUnitProc(LPCGAMEUNIT lpUnit, LPARAM lParam);
