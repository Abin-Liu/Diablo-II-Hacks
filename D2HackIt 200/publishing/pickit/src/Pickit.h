/////////////////////////////////////////////////////////////////////////
// pickit.h
//
// Written by Abin (abinn32@yahoo.com)
//
// History:
//
// 2.0 - Released for D2Hackit v2.00.
// 2.1 - Fixed a problem where the module would stop working if the user specified pickit delay values that could not be exactly devided by 100. 
// 2.2 - Fixed some synchronization problems.
// 2.3 - Added "goldamount" command.
// 2.4 - Some minor fixes.
//////////////////////////////////////////////////////////////////////////

#ifndef __PICKIT_H__
#define __PICKIT_H__

#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include "Utilities\\Ini.h"
#include "ItemPicker.h"

// globals
CIni pickitConfig_ini;
CItemPicker g_picker;

// module info
DWORD	ModuleVersion=MAKELONG(2,4);
char	ModuleAuthor[]="Abin";
char	ModuleWebsite[]="www.wxjindu.com/abin";
char	ModuleDescription[]="pickit for D2Hackit 2.00";
char	ModuleEmail[]="abinn32@yahoo.com";

// function definations
BOOL PRIVATE OnCommandDebug(char **argv, int argc);
BOOL PRIVATE OnCommandRadius(char **argv, int argc);
BOOL PRIVATE OnCommandToggle(char **argv, int argc);
BOOL PRIVATE OnCommandClientMove(char **argv, int argc);
BOOL PRIVATE OnCommandLoad(char** argv, int argc);
BOOL PRIVATE OnCommandEnable(char** argv, int argc);
BOOL PRIVATE OnCommandDisable(char** argv, int argc);
BOOL PRIVATE OnCommandGold(char** argv, int argc);
BOOL PRIVATE OnCommandRejuv(char** argv, int argc);
BOOL PRIVATE OnCommandFullRejuv(char** argv, int argc);
BOOL PRIVATE OnCommandDelay(char** argv, int argc);
BOOL PRIVATE OnCommandSummary(char** argv, int argc);
BOOL PRIVATE OnCommandLog(char** argv, int argc);
BOOL PRIVATE OnCommandFullCmds(char** argv, int argc);
BOOL PRIVATE OnCommandTownPick(char** argv, int argc);
BOOL PRIVATE OnCommandGoldAmount(char** argv, int argc);

int Str2Method(LPCTSTR lpsz);
BOOL Method2Str(int nMethod, LPTSTR lpszBuffer);
void ShowIniFileSettings();
void ShowLogFileSettings();
void ShowFullCmdSetting();
void ShowEnableDisableSetting();
void ShowDelaySetting();
void ShowRadiusSetting();
void ShowTownPickSetting();
void ShowClientMoveSetting();
void ShowGoldSetting();
void ShowRejuvSetting();
void ShowFullRejuvSetting();
void ShowGoldAmountSetting();
BOOL LoadConfig(LPCSTR lpszFile);

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
		"�c4.pickit radius �c0<radius>"
	},
	{
		"toggle",
		OnCommandToggle,
		"�c4.pickit toggle"
	},
	{
		"enable",
		OnCommandEnable,
		"�c4.pickit enable"
	},	
	{
		"disable",
		OnCommandDisable,
		"�c4.pickit disable"
	},	
	{
		"townpick",
		OnCommandTownPick,
		"�c4.townpick disable"
	},
	{
		"clientmove",
		OnCommandClientMove,
		"�c4.pickit clientmove"
	},	
	{
		"delay",
		OnCommandDelay,
		"�c4.pickit delay�c0 <milliseconds>"
	},
	{
		"summary",
		OnCommandSummary,
		"�c4.pickit summary"
	},
	{
		"load",
		OnCommandLoad,
		"�c4.pickit load�c0 <file-name>"
	},
	{
		"gold",
		OnCommandGold,
		"�c4.pickit gold�c0 <yes/no/auto>"
	},	
	{
		"rejuv",
		OnCommandRejuv,
		"�c4.pickit rejuv�c0 <yes/no/auto>"
	},	
	{
		"fullrejuv",
		OnCommandFullRejuv,
		"�c4.pickit fullrejuv�c0 <yes/no/auto>"
	},	

	{
		"log",
		OnCommandLog,
		"�c4.pickit log�c0 [file-name]"
	},

	{
		"fullcmds",
		OnCommandFullCmds,
		"�c4.pickit fullcmds�c0 [command1;command2;command3;...;commandn]"
	},

	{
		"goldamount",
		OnCommandGoldAmount,
		"�c4.pickit goldamount�c0 <amount>"
	},

	{NULL}
};


#endif // __PICKIT_H__