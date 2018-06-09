/////////////////////////////////////////////////////////////////////////
// pickit.h
//
// Written by Abin (abinn32@yahoo.com)
//
// History:
//
// 0.90 - Compatible to D2 1.10 (Abin)
// 0.91 - Fixed a gold-pick problem, maximum pickup radius changed to 50.
// 0.92 - Special items are always added even without a config file. 
//        Fixed a problem where the callback commands were not properly saved.
// 0.93 - Improved picking speed. Summary information are no longer displayed upon entering  games.
// 0.94 - Code are now based on the D2Lib library, faster and stabler.
//        Added command "clientmove" to show/hide client move when picking items.
// 0.95 - Added command "townpick" to allow/disallow pickup in towns.
//////////////////////////////////////////////////////////////////////////

#ifndef __PICKIT_H__
#define __PICKIT_H__

#include "PickitPlayer.h"

// globals
CPickitPlayer Me;
CIni pickitConfig_ini;

// module info
DWORD	ModuleVersion=MAKELONG(0,95);
char	ModuleAuthor[]="Abin";
char	ModuleWebsite[]="www.wxjindu.com/abin";
char	ModuleDescription[]="pickit for Diablo2 LOD 1.10";
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

// D2Lib Message Handler
void CALLBACK OnPickitMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData);

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
		"�c4.hc radius �c0<radius>"
	},
	{
		"toggle",
		OnCommandToggle,
		"�c4.hc toggle"
	},
	{
		"enable",
		OnCommandEnable,
		"�c4.hc enable"
	},	
	{
		"disable",
		OnCommandDisable,
		"�c4.hc disable"
	},	
	{
		"townpick",
		OnCommandTownPick,
		"�c4.townpick disable"
	},
	{
		"clientmove",
		OnCommandClientMove,
		"�c4.hc clientmove"
	},	
	{
		"delay",
		OnCommandDelay,
		"�c4.hc delay�c0 <milliseconds>"
	},
	{
		"summary",
		OnCommandSummary,
		"�c4.hc summary"
	},
	{
		"load",
		OnCommandLoad,
		"�c4.hc load�c0 <file-name>"
	},
	{
		"gold",
		OnCommandGold,
		"�c4.hc gold�c0 <yes/no/auto>"
	},	
	{
		"rejuv",
		OnCommandRejuv,
		"�c4.hc rejuv�c0 <yes/no/auto>"
	},	
	{
		"fullrejuv",
		OnCommandFullRejuv,
		"�c4.hc fullrejuv�c0 <yes/no/auto>"
	},	

	{
		"log",
		OnCommandLog,
		"�c4.hc log�c0 [file-name]"
	},

	{
		"fullcmds",
		OnCommandFullCmds,
		"�c4.hc fullcmds�c0 [command1;command2;command3;...;commandn]"
	},

	{NULL}
};


#endif // __PICKIT_H__