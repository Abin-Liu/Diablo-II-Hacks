/////////////////////////////////////////////////////////////////////////
// pickit.cpp
//
// Written by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////

#include "Pickit.h"
#include "..\\Includes\\ClientCore.cpp"

//////////////////////////////////////////////////////////////////////
// OnUnitMessage - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when a game unit message is sent to the client
//
// Refer to the documentation for the format of unit messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	g_picker.OnUnitMessage(nMessage, lpUnit, wParam, lParam);
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
	g_picker.OnThisPlayerMessage(nMessage, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////
// OnQueryModuleData
// -------------------------------------------------------------------
// Executes when other modules want to query data from this module
//
// Refer to the documentation for the format of module data querying.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnQueryModuleData(DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam)
{
	// We define data ID "1" for querying number of items that need to be pickied
	// lParam 0: Query number of all items (Valuable items + junky items).
	// lParam 1: Query number of valuable items only.
	if (dwDataID != 1 || !lpBuffer || !dwMaxLength)
		return 0;

	DWORD dwCount = g_picker.GetQueueItemCount(lParam, TRUE);
	DWORD dwCopied = min(4, dwMaxLength);
	::memcpy(lpBuffer, &dwCount, dwCopied);
	return dwCopied;
}


// functions called by d2hackit
BOOL EXPORT OnClientStart()
{
	char szFile[_MAX_PATH + 1] = "";
	sprintf(szFile, "%s\\%s", server->GetModuleDirectory(), "pickitconfig.ini");
	pickitConfig_ini.SetPathName(szFile);

	sprintf(szFile, "%s\\%s", server->GetModuleDirectory(), "pickit.ini");
	g_picker.LoadConfig(szFile);

	g_picker.SetRadius(pickitConfig_ini.GetInt("Pickit", "Radius", PICKIT_RADIUS_DEFAULT));
	g_picker.ShowClientMove(pickitConfig_ini.GetBool("Pickit", "Client Move", TRUE));
	g_picker.SetAllowTownPick(pickitConfig_ini.GetBool("Pickit", "Town Pick", TRUE));
	g_picker.SetSpecialItemMethod(PICKIT_SPECIAL_GOLD, pickitConfig_ini.GetInt("Pickit", "Gold", PICKIT_PICK_AUTO));
	g_picker.SetSpecialItemMethod(PICKIT_SPECIAL_REJUV, pickitConfig_ini.GetInt("Pickit", "Rejuv", PICKIT_PICK_NO));
	g_picker.SetSpecialItemMethod(PICKIT_SPECIAL_FULLREJUV, pickitConfig_ini.GetInt("Pickit", "Full Rejuv", PICKIT_PICK_AUTO));

	char szMethod[1025] = "";
	pickitConfig_ini.GetString("Pickit", "Full Commands", szMethod, 1024);
	g_picker.SetFullCmds(szMethod);

	pickitConfig_ini.GetString("Pickit", "Log File", szMethod, 1024);
	g_picker.SetLogFilePath(szMethod);

	g_picker.SetMinGoldAmount(pickitConfig_ini.GetUInt("Pickit", "Gold Amount", 0));
	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	g_picker.OnGameJoin();
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* aThisgame)
{
	g_picker.OnGameLeave();
}

DWORD EXPORT OnGameTimerTick()
{
	g_picker.OnGameTimerTick();
	return 0;
}

// functions related to the above commands
BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	g_picker.DebugQueue();
	return TRUE;
}

BOOL PRIVATE OnCommandClientMove(char **argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit clientmove <on/off>");
		return TRUE;
	}

	BOOL bShow = TRUE;
	if (stricmp(argv[2], "on") == 0)
	{
		bShow = TRUE;
	}
	else if (stricmp(argv[2], "off") == 0)
	{
		bShow = FALSE;
	}
	else
	{
		server->GamePrintError("ÿc4Syntax: .pickit clientmove <on/off>");
		return TRUE;
	}

	g_picker.ShowClientMove(bShow);
	pickitConfig_ini.WriteBool("Pickit", "Client Move", bShow);
	ShowClientMoveSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandRadius(char **argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit radius <radius>");
		return TRUE;
	}

	g_picker.SetRadius(strtoul(argv[2], NULL, 10));
	pickitConfig_ini.WriteInt("Pickit", "Radius", g_picker.GetRadius());
	ShowRadiusSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandToggle(char **argv, int argc)
{	
	if (g_picker.Toggle())
		server->GamePrintInfo("ÿc4Pickit enabled.");
	else
		server->GamePrintInfo("ÿc4Pickit disabled.");
	return TRUE;
}

BOOL PRIVATE OnCommandEnable(char **argv, int argc)
{
	g_picker.Enable(TRUE);
	server->GamePrintInfo("ÿc4Pickit enabled.");
	return TRUE;
}

BOOL PRIVATE OnCommandDisable(char** argv, int argc)
{
	g_picker.Enable(FALSE);
	server->GamePrintInfo("ÿc4Pickit disabled.");
	return TRUE;
}

BOOL PRIVATE OnCommandLoad(char** argv, int argc) {

	// get arguments 2-x as the target file
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit load <file name>");
		return TRUE;
	}

	char szFile[_MAX_PATH + 1] = "";
	sprintf(szFile, "%s\\", server->GetModuleDirectory());
	for (int i = 2; i < argc; i++)
		strcat(szFile, argv[i]);

	if (g_picker.LoadConfig(szFile))
		ShowIniFileSettings();
	else
		server->GameErrorf("ÿc4Failed to load file ÿc0%s!", szFile);
		
	return TRUE;
}

BOOL PRIVATE OnCommandSummary(char** argv, int argc)
{
	server->GamePrintString("");
	server->GamePrintInfo("ÿc4Pickit Settings");
	server->GamePrintInfo("ÿc4---------------------------------------");

	ShowIniFileSettings();
	ShowLogFileSettings();
	ShowFullCmdSetting();
	ShowEnableDisableSetting();
	ShowRadiusSetting();
	ShowTownPickSetting();
	ShowClientMoveSetting();
	ShowGoldSetting();
	ShowGoldAmountSetting();
	ShowRejuvSetting();
	ShowFullRejuvSetting();	
	
	server->GamePrintInfo("ÿc4---------------------------------------");
	return TRUE;
}

void ShowIniFileSettings()
{
	server->GameInfof("ÿc4Config File:     ÿc0%s", g_picker.GetConfigFile());
}

void ShowFullCmdSetting()
{
	LPCTSTR lpszCmds = g_picker.GetFullCmds();
	if (lpszCmds != NULL)
		server->GameInfof("ÿc4Full Commands:   ÿc0%s", lpszCmds);
	else
		server->GamePrintInfo("ÿc4Full Commands:");
}

void ShowLogFileSettings()
{
	server->GameInfof("ÿc4Log File:   ÿc0%s", g_picker.GetLogFilePath());
}

void ShowEnableDisableSetting()
{
	server->GameInfof("ÿc4Enabled:         ÿc2%s", g_picker.IsEnabled() ? "Yes" : "No");
}

void ShowRadiusSetting()
{
	server->GameInfof("ÿc4Radius:          ÿc2%d", g_picker.GetRadius());
}

void ShowTownPickSetting()
{
	if (g_picker.IsAllowTownPick())
		server->GamePrintInfo("ÿc4Town Pick:  ÿc2On");
	else
		server->GamePrintInfo("ÿc4Town Pick:  ÿc1Off");
}

void ShowClientMoveSetting()
{
	if (g_picker.IsShowClientMove())
		server->GamePrintInfo("ÿc4Client Move:  ÿc2On");
	else
		server->GamePrintInfo("ÿc4Client Move:  ÿc1Off");
}

void ShowGoldSetting()
{
	char szMethod[16] = "";
	Method2Str(g_picker.GetSpecialItemMethod(PICKIT_SPECIAL_GOLD), szMethod);
	server->GameInfof("ÿc4Gold:            ÿc2%s", szMethod);
}

void ShowRejuvSetting()
{
	char szMethod[16] = "";
	Method2Str(g_picker.GetSpecialItemMethod(PICKIT_SPECIAL_REJUV), szMethod);
	server->GameInfof("ÿc4Rejuv:           ÿc2%s", szMethod);
}

void ShowFullRejuvSetting()
{
	char szMethod[16] = "";
	Method2Str(g_picker.GetSpecialItemMethod(PICKIT_SPECIAL_FULLREJUV), szMethod);
	server->GameInfof("ÿc4Full Rejuv:      ÿc2%s", szMethod);
}

BOOL PRIVATE OnCommandGold(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit gold <yes/no/auto>");
		return TRUE;
	}

	int nMethod = Str2Method(argv[2]);
	if (nMethod == PICKIT_PICK_UNKNWON)
	{
		server->GamePrintError("ÿc4Syntax: .pickit gold <yes/no/auto>");
		return TRUE;
	}

	g_picker.SetSpecialItemMethod(PICKIT_SPECIAL_GOLD, nMethod);
	pickitConfig_ini.WriteInt("Pickit", "Gold", nMethod);
	ShowGoldSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandRejuv(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit rejuv <yes/no/auto>");
		return TRUE;
	}

	int nMethod = Str2Method(argv[2]);
	if (nMethod == PICKIT_PICK_UNKNWON)
	{
		server->GamePrintError("ÿc4Syntax: .pickit rejuv <yes/no/auto>");
		return TRUE;
	}

	g_picker.SetSpecialItemMethod(PICKIT_SPECIAL_REJUV, nMethod);
	pickitConfig_ini.WriteInt("Pickit", "Rejuv", nMethod);
	ShowRejuvSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandFullRejuv(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit fullrejuv <yes/no/auto>");
		return TRUE;
	}

	int nMethod = Str2Method(argv[2]);
	if (nMethod == PICKIT_PICK_UNKNWON)
	{
		server->GamePrintError("ÿc4Syntax: .pickit fullrejuv <yes/no/auto>");
		return TRUE;
	}

	g_picker.SetSpecialItemMethod(PICKIT_SPECIAL_FULLREJUV, nMethod);
	pickitConfig_ini.WriteInt("Pickit", "Full Rejuv", nMethod);	
	ShowFullRejuvSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandLog(char** argv, int argc)
{
	g_picker.SetLogFilePath(NULL);
	if (argc < 3)
	{
		ShowLogFileSettings();
		return TRUE;
	}

	char szLogFile[MAX_PATH + 1] = "";
	sprintf(szLogFile, "%s\\", server->GetModuleDirectory());
	for (int i = 2; i < argc; i++)
		sprintf(szLogFile, "%s%s ", szLogFile, argv[i]);
	g_picker.SetLogFilePath(szLogFile);
	pickitConfig_ini.WriteString("Pickit", "Log File", g_picker.GetLogFilePath());
	ShowLogFileSettings();
	return TRUE;
}

BOOL PRIVATE OnCommandFullCmds(char** argv, int argc)
{
	if (argc < 3)
	{
		g_picker.SetFullCmds(NULL);
		pickitConfig_ini.WriteString("Pickit", "Full Commands", "");
		server->GamePrintInfo("ÿc4Inventory full commands disabled.");
		return TRUE;
	}
	
	char szCmds[1025] = "";
	for (int i = 2; i < argc; i++)
		sprintf(szCmds, "%s%s ", szCmds, argv[i]);

	g_picker.SetFullCmds(szCmds);
	pickitConfig_ini.WriteString("Pickit", "Full Commands", szCmds);
	ShowFullCmdSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandTownPick(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit townpick <on/off>");
		return TRUE;
	}

	BOOL bAllow = TRUE;
	if (stricmp(argv[2], "on") == 0)
	{
		bAllow = TRUE;
	}
	else if (stricmp(argv[2], "off") == 0)
	{
		bAllow = FALSE;
	}
	else
	{
		server->GamePrintError("ÿc4Syntax: .pickit townpick <on/off>");
		return TRUE;
	}

	g_picker.SetAllowTownPick(bAllow);
	pickitConfig_ini.WriteBool("Pickit", "Town Pick", bAllow);
	ShowTownPickSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandGoldAmount(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit goldamount <amount>");
		return TRUE;
	}

	g_picker.SetMinGoldAmount(strtoul(argv[2], NULL, 10));
	pickitConfig_ini.WriteUInt("Pickit", "Gold Amount", g_picker.GetMinGoldAmount());
	ShowGoldAmountSetting();
	return TRUE;
}

int Str2Method(LPCTSTR lpsz)
{
	if (lpsz == NULL)
		return PICKIT_PICK_AUTO;

	if (stricmp(lpsz, "no") == 0)
		return PICKIT_PICK_NO;

	if (stricmp(lpsz, "yes") == 0)
		return PICKIT_PICK_YES;

	if (stricmp(lpsz, "auto") == 0)
		return PICKIT_PICK_AUTO;

	return PICKIT_PICK_AUTO;
}

BOOL Method2Str(int nMethod, LPTSTR lpszBuffer)
{
	if (lpszBuffer == NULL)
		return FALSE;

	lpszBuffer[0] = '\0';
	switch (nMethod)
	{
	case PICKIT_PICK_NO:
		strcpy(lpszBuffer, "No");
		break;

	case PICKIT_PICK_YES:
		strcpy(lpszBuffer, "yes");
		break;

	case PICKIT_PICK_AUTO:
		strcpy(lpszBuffer, "Auto");
		break;

	default:
		break;
	}

	return strlen(lpszBuffer);
}

void ShowGoldAmountSetting()
{
	server->GameInfof("ÿc4Gold Amount:          ÿc2%u", g_picker.GetMinGoldAmount());
}
