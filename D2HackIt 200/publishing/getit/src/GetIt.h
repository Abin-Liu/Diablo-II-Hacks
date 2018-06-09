/////////////////////////////////////////////////////////////////////////
// GetIt.h
//
// Written by Abin (abinn32@yahoo.com)
//
// History:
//
//////////////////////////////////////////////////////////////////////////

#ifndef __GETIT_H__
#define __GETIT_H__

#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include "..\\Includes\\Utilities\\Ini.h"
#include "..\\Includes\\Utilities\\StateTicker.h"
#include "..\\Includes\\Utilities\\ArrayEx.h"
#include "..\\Includes\\Utilities\\LinkedList.h"
#include "ItemPicker.h"
#include "D2Npc.h"

typedef struct tagIDConfig
{
	LPSTR pszSection;
	char szCodes[256];
	LPSTR aExpressions[64];
} IDCONFIG, *LPIDCONFIG;
typedef const tagIDConfig* LPCIDCONFIG;
void CALLBACK FreeConfig(LPIDCONFIG& pCfg);
typedef CLinkedList<LPIDCONFIG, LPIDCONFIG, FreeConfig> IDConfigList;

typedef struct tagIDItem
{
	DWORD dwItemID;
	SIZE itemSize;
} IDITEM, *LPIDITEM;
typedef const tagIDItem* LPCIDITEM;

enum { ID_NONE = 0,
	 ID_CHECK_STASH,
	 ID_OPEN_STASH,
	 ID_WAIT_STASH,
	 ID_DEPOSIT,
	 ID_PICK_UP, 
	 ID_WAIT_PICK,
	 ID_DROP_DOWN, 
	 ID_WAIT_DOWN,	 
	 ID_END
};


// globals
CItemPicker g_picker;
CStateTicker g_ticker;
CIni g_ini;
int g_nLeftCol = 1024;
int g_nRightCol = -1;
IDConfigList g_aConfigs;
BOOL g_bFull = FALSE;
DWORD g_dwCurItem = 0;
CArrayEx<IDITEM, const IDITEM&> g_aItems;
char g_szCallback[256] = "";
BOOL g_bUseCube = TRUE;
int g_nStashRetry = 0;

// module info
DWORD	ModuleVersion=MAKELONG(3,0);
char	ModuleAuthor[]="Abin";
char	ModuleWebsite[]="www.wxjindu.com/abin";
char	ModuleDescription[]="GetIt for D2Hackit 2.00";
char	ModuleEmail[]="abinn32@yahoo.com";

// function definations
BOOL PRIVATE OnCommandDebug(char **argv, int argc);
BOOL PRIVATE OnCommandRadius(char **argv, int argc);
BOOL PRIVATE OnCommandToggle(char **argv, int argc);
BOOL PRIVATE OnCommandClientMove(char **argv, int argc);
BOOL PRIVATE OnCommandLoad(char** argv, int argc);
BOOL PRIVATE OnCommandEnable(char** argv, int argc);
BOOL PRIVATE OnCommandDisable(char** argv, int argc);
BOOL PRIVATE OnCommandSummary(char** argv, int argc);
BOOL PRIVATE OnCommandLog(char** argv, int argc);
BOOL PRIVATE OnCommandFullCmds(char** argv, int argc);
BOOL PRIVATE OnCommandTownPick(char** argv, int argc);
BOOL PRIVATE OnCommandDelay(char** argv, int argc);
BOOL PRIVATE OnCommandStash(char** argv, int argc);

void ShowIniFileSettings();
void ShowLogFileSettings();
void ShowFullCmdSetting();
void ShowEnableDisableSetting();
void ShowRadiusSetting();
void ShowTownPickSetting();
void ShowClientMoveSetting();
void ShowDelaySetting();
BOOL IsGoodItem(DWORD dwItemID);
//DWORD SellFirstJunk();
int GetGoodItems();
void RemoveItem(DWORD dwItemID);
void StateMessage(LPCSTR lpszMsg);
BOOL operator==(const IDITEM& lhs, const IDITEM& rhs);
BOOL operator>(const IDITEM& lhs, const IDITEM& rhs);

BOOL CALLBACK EnumSectionsProc(LPCTSTR lpSection, LPVOID lpParam);
BOOL CALLBACK EnumKeysProc(LPCTSTR lpKeyName, LPCTSTR lpValue, LPVOID lpParam);
BOOL CALLBACK EnumItemProc(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam);

// module commands
MODULECOMMANDSTRUCT ModuleCommands[]= {

	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"help�c0 List commands available in this module.\n"
		"<command> help�c0 Shows detailed help for <command> in this module."
	},

	{	
		"debug",
		OnCommandDebug,
		"�c4debug �c0 toggle debug mode.\n"
	},
	{
		"radius",
		OnCommandRadius,
		"�c4radius �c0<radius>"
	},
	{
		"toggle",
		OnCommandToggle,
		"�c4toggle"
	},
	{
		"enable",
		OnCommandEnable,
		"�c4enable"
	},	
	{
		"disable",
		OnCommandDisable,
		"�c4disable"
	},	
	{
		"townpick",
		OnCommandTownPick,
		"�c4townpick disable"
	},
	{
		"clientmove",
		OnCommandClientMove,
		"�c4clientmove"
	},	
	{
		"summary",
		OnCommandSummary,
		"�c4summary"
	},
	{
		"load",
		OnCommandLoad,
		"�c4load�c0 <file-name>"
	},
	{
		"log",
		OnCommandLog,
		"�c4log�c0 [file-name]"
	},

	{
		"callback",
		OnCommandFullCmds,
		"�c4fullcmds�c0 [command1;command2;command3;...;commandn]"
	},

	{
		"delay",
		OnCommandDelay,
		"�c4delay�c0 <milliseconds>"
	},

	{
		"stash",
		OnCommandStash,
		"�c4id"
	},

	{NULL}
};


#endif // __TAKEIT_H__