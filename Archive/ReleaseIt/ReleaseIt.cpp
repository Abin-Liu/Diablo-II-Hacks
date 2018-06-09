//////////////////////////////////////////////////////////////////////
// ReleaseIt.CPP by Abin
// -------------------------------------------------------------------
// A D2HackIt module that auto releases Assassin charge-up skills.
//
// Copyright (c) 2002, Abin (Bin Liu)
// All rights reserved.
//
// Features:
// 
// 1: Auto releases Assassin Phoenix Strike and Claws of Thunder skills.
// 2: Configurable charge-up numbers for either skills.
//
// Great thanks to:
//
// Thohell: The Godfather of all D2Hackit stuff, this module would not
//          even exist without his splD2Hackit API. Also, some of source
//          code from his "Bind.cpp" are reused in this program for the
//          "shortcut key" feature.
//
// Ackmed: Some of source code from his "Packethelper.cpp" are reused in
//         This program for multiple functionalities.
//
//////////////////////////////////////////////////////////////////////
//
// Update History:
//
// v0.6 - Captures packet 0xa3 instead of 0x12 as "chargeup released" flag.
// v0.5 - Fixed a conflict with "Aura" skills.
// v0.4 - Renamed this module to "ReleaseIt"(originally was "Chargeup"),
//        some Minior fixes.
// v0.3 - Added commands ".ReleaseIt PS +/-" and ".ReleaseIt CoT +/-", for 
//        increasing/decreasing currently set charge-up numbers rather than 
//        assigning with arbitrary values.
// v0.2 - Fixed a bug when determines whether character is an Assassin.
// v0.1 - Beta release. (Abin)
// 
//////////////////////////////////////////////////////////////////////
#include "External/D2Hackit.h"
#include "External/D2Client.h"
#include <time.h>
#include <math.h>
#include "ReleaseIt.h"
#include "PacketHelper.h"
#include "D2Skills.h"

BOOL g_bDebug = FALSE;
BOOL g_bEnable = TRUE;

// current left skills
WORD g_wLastSkillID = 0x0000;

CIniFile g_ini; // Ini file

BYTE g_iCharge_PS = 0xff;
BYTE g_iCharge_CoT = 0xff;

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
//////////////////////////////////////////////////////////////////////
DWORD   ModuleVersion=MAKELONG(0, 11);
char    ModuleAuthor[]="Abin";
char    ModuleWebsite[]="http://www.bridgeport.edu/~binliu/d2x/";
char    ModuleDescription[]="ReleaseIt - Auto release assassin charge-up skills.";
char    ModuleEmail[]="abinn32@yahoo.com";

//////////////////////////////////////////////////////////////////////
// Function Definition
//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter);
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc);
BOOL PRIVATE OnGameCommandToggle(char** argv, int argc);
BOOL PRIVATE OnGameCommandPS(char** argv, int argc);
BOOL PRIVATE OnGameCommandCoT(char** argv, int argc);
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);
BOOL PRIVATE OnGameCommandShow(char** argv, int argc);

//////////////////////////////////////////////////////////////////////
// ClientCore.cpp stuff
//////////////////////////////////////////////////////////////////////
char ConfigPath[_MAX_PATH] = ""; // Dll path
FUNCTIONENTRYPOINTS	*server = NULL;
THISGAMESTRUCT *thisgame = NULL;

typedef struct thread_data_t
{
	HANDLE   Thread;
	BOOL  Active;  
} THREAD_DATA;

THREAD_DATA td;

//////////////////////////////////////////////////////////////////////
// Module Commands
//////////////////////////////////////////////////////////////////////
MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",  OnGameCommandHelp,
		"help ÿc0List commands available in this module.\n"
		"<command> help ÿc0 Shows detailed help for <command> in this module."
	},

	{
		"toggle",
		OnGameCommandToggle,
		"toggleÿc0 Enable/disable this module.\n"
	},
	
	{
		"ps",
		OnGameCommandPS,
		"PSÿc0 Set charge-up number(0/1/2/3/+/-) for Phoenix strike"
	},

	{
		"cot",
		OnGameCommandCoT,
		"CoTÿc0 Set charge-up number(0/1/2/3/+/-) for Claws of Thunder"
	},

	{
		"show",
		OnGameCommandShow,
		"Show current config"
	},

	{
		"debug",
		OnGameCommandDebug,
		"debugÿc0 For programmer use only."
	},

	{NULL}
};

// display additional info when debug mode is enabled
void _DebugInfo(LPCTSTR lpMsg)
{
	if (g_bDebug)
		server->GamePrintInfo(lpMsg);
}

BOOL IsAssassin()
{
	return thisgame && thisgame->player && thisgame->player->CharacterClass == 6;
}

BOOL ShouldRelease(const BYTE* aPacket, DWORD aLen)
{
	if (aLen < 11)
		return FALSE;

	const BYTE icharge = aPacket[10]; // charge number
	if (icharge < 1 || icharge > 3)
		return FALSE;

	if (g_wLastSkillID == DSD_PHOENIXSTRIKE)
	{
		return (icharge >= g_iCharge_PS);
	}
	else if (g_wLastSkillID == DSD_CLAWSOFTHUNDER)
	{
		return (icharge >= g_iCharge_CoT);
	}
	else
	{
		return FALSE;
	}
}

void LoadConfig()
{
	g_iCharge_PS = g_ini.GetProfileInt("Charges", "PS", 0xff);
	g_iCharge_CoT = g_ini.GetProfileInt("Charges", "CoT", 0xff);
	if (g_iCharge_PS < 1 || g_iCharge_PS > 3)
		g_iCharge_PS = 0xff;
	if (g_iCharge_CoT < 1 || g_iCharge_CoT > 3)
		g_iCharge_CoT = 0xff;
}

void SaveConfig()
{
	g_ini.WriteProfileInt("Charges", "PS", g_iCharge_PS);
	g_ini.WriteProfileInt("Charges", "CoT", g_iCharge_CoT);
}

void ShowConfig()
{
	char sz[128] = "";
	char szPS[10] = "";
	char szCoT[10] = "";

	if (g_iCharge_PS > 3)
		strcpy(szPS, "ÿc2N/Aÿc0");
	else
		sprintf(szPS, "ÿc2%dÿc0", g_iCharge_PS);

	if (g_iCharge_CoT > 3)
		strcpy(szCoT, "ÿc2N/Aÿc0");
	else
		sprintf(szCoT, "ÿc2%dÿc0", g_iCharge_CoT);

	sprintf(sz, "PS: %s    CoT: %s", szPS, szCoT);
	server->GamePrintInfo("ÿc7Charge-up settings:");
	server->GamePrintInfo(sz);
}

BOOL EXPORT OnClientStart()
{
	char szPath[_MAX_PATH] = "";
	sprintf(szPath, "%s\\ReleaseIt.ini", ConfigPath);
	g_ini.SetPathName(szPath);
	LoadConfig();

	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	return TRUE;
}

void EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	thisgame = game;
	//g_wLastSkillID = 0x0000;

	if (IsAssassin())
		ShowConfig();
}

void EXPORT OnGameLeave(THISGAMESTRUCT* thisgame)
{
}

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	if (g_bDebug)
	{
		g_bDebug = FALSE;
		server->GamePrintInfo("Debug mode disabled.");
	}
	else
	{
		g_bDebug = TRUE;
		server->GamePrintInfo("Debug mode enabled.");
	}

	ShowConfig();
    return TRUE;
}

// command modules
BOOL PRIVATE OnGameCommandToggle(char** argv, int argc)
{
	if (!IsAssassin())
	{
		server->GamePrintError("ReleaseIt module only available to Assassins.");
		return TRUE;
	}

	if (g_bEnable)
	{
		g_bEnable = FALSE;
		server->GamePrintInfo("ReleaseIt disabled");
	}
	else
	{
		g_bEnable = TRUE;
		server->GamePrintInfo("ReleaseIt enabled");
	}
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandPS(char** argv, int argc)
{
	if (!IsAssassin())
	{
		server->GamePrintError("ReleaseIt module only available to Assassins.");
		return TRUE;
	}

	if (argc < 3)
	{
		server->GamePrintError("Syntax: .ReleaseIt <PS/CoT> <0/1/2/3/+/->");
		return TRUE;
	}

	if ((argv[2])[0] == '+')
	{
		if (g_iCharge_PS >= 3)
			g_iCharge_PS = 1;
		else
			g_iCharge_PS++;
	}
	else if ((argv[2])[0] == '-')
	{
		if (g_iCharge_PS <= 1 || g_iCharge_PS > 3)
			g_iCharge_PS = 3;
		else
			g_iCharge_PS--;
	}
	else
	{
		int n = atoi(argv[2]);
	
		if (n < 0 || n > 3)
		{
			server->GamePrintError("Syntax: .ReleaseIt <PS/CoT> <0/1/2/3/+/->");
			return TRUE;
		}

		g_iCharge_PS = (n == 0) ? 0xff : (BYTE)n;
	}

	SaveConfig();
	ShowConfig();
	return TRUE;
}

BOOL PRIVATE OnGameCommandCoT(char** argv, int argc)
{
	if (!IsAssassin())
	{
		server->GamePrintError("ReleaseIt module only available to Assassins.");
		return TRUE;
	}

	if (argc < 3)
	{
		server->GamePrintError("Syntax: .ReleaseIt <PS/CoT> <0/1/2/3/+/->");
		return TRUE;
	}

	if ((argv[2])[0] == '+')
	{
		if (g_iCharge_CoT >= 3)
			g_iCharge_CoT = 1;
		else
			g_iCharge_CoT++;
	}
	else if ((argv[2])[0] == '-')
	{
		if (g_iCharge_CoT <= 1 || g_iCharge_CoT > 3)
			g_iCharge_CoT = 3;
		else
			g_iCharge_CoT--;
	}
	else
	{
		int n = atoi(argv[2]);
	
		if (n < 0 || n > 3)
		{
			server->GamePrintError("Syntax: .ReleaseIt <PS/CoT> <0/1/2/3/+/->");
			return TRUE;
		}

		g_iCharge_CoT = (n == 0) ? 0xff : (BYTE)n;
	}

	SaveConfig();
	ShowConfig();
	return TRUE;
}

BOOL PRIVATE OnGameCommandShow(char** argv, int argc)
{
	if (IsAssassin())
		ShowConfig();
	else
		server->GamePrintError("ReleaseIt module only available to Assassins.");
	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	if (!IsAssassin())
		return aLen;

	// skill activation
	if (aLen == 13 && aPacket[0] == 0x23 && aPacket[6] != 0x00)
	{
		DWORD dwPlayer;
		::memcpy(&dwPlayer, aPacket + 2, 4);
		if (dwPlayer == thisgame->player->PlayerID)
		{
			// left side
			WORD wSkillID;
			::memcpy(&wSkillID, aPacket + 7, 2);
			if (wSkillID == DSD_PHOENIXSTRIKE || wSkillID == DSD_CLAWSOFTHUNDER)
			{
				//_DebugInfo("PS or CoT activated.");
				g_wLastSkillID = wSkillID;
			}
		}
	}

	// current charge up number (1-3)
	if (g_bEnable
		&& aLen == 18
		&& aPacket[0] == 0x14
		&& aPacket[10] > 0
		&& aPacket[10] < 4)
	{
		DWORD dwID;
		::memcpy(&dwID, aPacket + 2, 4);
		if (dwID == thisgame->player->PlayerID)
		{
			if (ShouldRelease(aPacket, aLen))
			{
				// so either PS or CoT has been charged up
				_DebugInfo("should release now.");
				SelectSkill(DSD_ATTACK, FALSE);
			}
		}
	}

	// charge up released
	/*
	if (g_bEnable
		&& aLen == 24
		&& aPacket[0] == 0xa3
		&& aPacket[1] >= 0x01
		&& aPacket[1] <= 0x03 )
	{
		DWORD dwID;
		memcpy(&dwID, aPacket + 7, 4);
		if (dwID == thisgame->player->PlayerID)
		{
			SelectSkill(g_wLastSkillID, FALSE);
		}
	}
	*/

	if (g_bEnable
		&& aLen == 26
		&& aPacket[0] == 0x12)
	{
		DWORD dwID;
		memcpy(&dwID, aPacket + 2, 4);
		if (dwID == thisgame->player->PlayerID)
		{
			SelectSkill(g_wLastSkillID, FALSE);
		}
	}

    return aLen;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	return 0;	
}

//////////////////////////////////////////////////////////////////////
// stuff below here is from ClientCore.cpp
//////////////////////////////////////////////////////////////////////
// Dll entry/exit
//////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	BOOL hResult = TRUE;
	char *t;
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:

			// Create server struct
			server = new FUNCTIONENTRYPOINTS;

			// Bind exported functions from server
			HMODULE hModule;
			hModule = (HMODULE)GetModuleHandle("D2HackIt");

			// Macros make this look a lot nicer :)
			BIND_TO_SERVER(GamePrintInfo);
			BIND_TO_SERVER(GamePrintError);
			BIND_TO_SERVER(GamePrintVerbose);
			BIND_TO_SERVER(GamePrintString);
			BIND_TO_SERVER(GameCommandLine);
			BIND_TO_SERVER(GameSendPacketToServer);
			BIND_TO_SERVER(GameSendPacketToGame);
			BIND_TO_SERVER(GetFingerprint);
			BIND_TO_SERVER(Intercept);
			BIND_TO_SERVER(GetHackProfileString);
			BIND_TO_SERVER(SetHackProfileString);
			BIND_TO_SERVER(GetThisgameStruct);

			// Get plugin path
			t = ConfigPath;
			if (GetModuleFileName((HINSTANCE)hModule, t, _MAX_PATH)) {
				int p=strlen(ConfigPath);
				while (p) {
						if (ConfigPath[p] == '\\')
							{ ConfigPath[p] = 0; p=0;}
						else
					p--;
				}
			}
			// initiate client
			// hResult = OnClientStart();
			break;

		case DLL_PROCESS_DETACH:
			// kill client

			// hResult = OnClientStop();

			delete server;
			break;
    }
    return hResult;
}

//////////////////////////////////////////////////////////////////////
// Stubfunctions for 'property get' functions.
//////////////////////////////////////////////////////////////////////
LPCSTR	EXPORT GetModuleAuthor()		{return ModuleAuthor;}
LPCSTR	EXPORT GetModuleWebsite()		{return ModuleWebsite;}
DWORD	EXPORT GetModuleVersion()		{return ModuleVersion;}
LPCSTR	EXPORT GetModuleEmail()			{return ModuleEmail;}
LPCSTR	EXPORT GetModuleDescription()	{return ModuleDescription;}

//////////////////////////////////////////////////////////////////////
// OnClientCommandLine
// -------------------------------------------------------------------
// The modules own extension of the command line interface. Any custom
// commands you add are parsed here.
//
// Return value should be TRUE, but it is not used at this
// time.
//
// Arguments when we get here:
// argv[0]			Name of module
// argv[1]			Name of command (If supplied)
// argv[2 ... n]	The rest
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnGameCommandLine(char* argv[], int argc)
{
	// Check if user supplied anything at all, if not assume help...
	if (argc==1)
		argv[argc++]="help";


	MODULECOMMANDSTRUCT* mcs=ModuleCommands;

	while (mcs->szName) {
		if (!stricmp(mcs->szName, argv[1]))
			break;
		mcs++;
	}

	char *p,*t,*msg,*fMsg;
	fMsg=new char[256];
	//
	// Is this a built-in function ?
	if (mcs->szName) {
		//
		// If functions returns false, show usage help
		if (!mcs->pFunc(argv, argc)) {
			t=new char[strlen(mcs->szUsage)+1];
			server->GamePrintInfo("Usage:");
			sprintf((char*)t, "%s", mcs->szUsage);
			if (strlen((char*)t))
			{
				msg=p=t;
				while (*p != 0) {
					if (*p == '\n')
					{
						*(p++) = 0;
						sprintf(fMsg, "ÿc4.%s %s", argv[0], msg);
						server->GamePrintInfo((char*)fMsg);
					if (*p != 0)
						msg = p;
					} else
						p++;
					}
				sprintf(fMsg, "ÿc4.%s %s", argv[0], msg);
				server->GamePrintInfo((char*)fMsg);
			}
			delete t;
		}
	}
	else {
	// Unknown command, show catch-all help phraze.
	t=new char[128];
	sprintf(t, "Unknown command ÿc4'%s %s'ÿc0 - try ÿc4'.%s help'ÿc0 to get help.",
		argv[0], argv[1], argv[0]);
	server->GamePrintError(t);
	delete t;
	}
	delete fMsg;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameCommandHelp
// -------------------------------------------------------------------
// Our default help function.
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc)
{
	// If command line is longer than 2, show usage for 'help'
	if (argc>2) return FALSE;

	char t[1024];
	sprintf(t, "Available commands for %s:", argv[0]);
	server->GamePrintInfo(t);

	// Loop through ModuleCommands[] and print their names
	for (int i=0; ModuleCommands[i].szName != NULL; i++)
	{
		sprintf(t, "ÿc4.%s %s", argv[0], ModuleCommands[i].szName);
		server->GamePrintInfo(t);
	}

	sprintf(t, "For help on a specific command, type ÿc4.%s <command> help", argv[0]);
	server->GamePrintInfo(t);
	return TRUE;
}
