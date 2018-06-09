/////////////////////////////////////////////////////////////////////////
// pickit.cpp
//
// Written by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////

#include "Pickit.h"
#pragma comment(lib, "..\\D2lib\\D2Lib.lib") // Import D2Lib
#include "..\\D2Lib\\ClientCore.cpp"

// D2 Message handler
void CALLBACK OnPickitMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	if (nMessage == PHM_ITEMDROP) // A desired item dropped
	{
		LPCTSTR lpszDescription = (LPCTSTR)lParam;
		if (lpszDescription && lpszDescription[0])
			server->GamePrintInfo(lpszDescription);
	}
}

// functions called by d2hackit
BOOL EXPORT OnClientStart()
{
	Me.SetD2MessageProc(OnPickitMessage);

	char szFile[_MAX_PATH + 1] = "";
	sprintf(szFile, "%s\\%s", GetModuleDirectory(), "pickitconfig.ini");
	pickitConfig_ini.SetPathName(szFile);

	sprintf(szFile, "%s\\%s", GetModuleDirectory(), "pickit.ini");
	Me.LoadConfig(szFile);

	Me.SetDelay(pickitConfig_ini.GetInt("Pickit", "Delay", PICKIT_DELAY_DEFAULT));
	Me.SetRadius(pickitConfig_ini.GetInt("Pickit", "Radius", PICKIT_RADIUS_DEFAULT));
	Me.ShowClientMove(pickitConfig_ini.GetBool("Pickit", "Client Move", TRUE));
	Me.SetAllowTownPick(pickitConfig_ini.GetBool("Pickit", "Town Pick", TRUE));
	Me.SetSpecialItemMethod(PICKIT_SPECIAL_GOLD, pickitConfig_ini.GetInt("Pickit", "Gold", PICKIT_PICK_AUTO));
	Me.SetSpecialItemMethod(PICKIT_SPECIAL_REJUV, pickitConfig_ini.GetInt("Pickit", "Rejuv", PICKIT_PICK_NO));
	Me.SetSpecialItemMethod(PICKIT_SPECIAL_FULLREJUV, pickitConfig_ini.GetInt("Pickit", "Full Rejuv", PICKIT_PICK_AUTO));

	char szMethod[1025] = "";
	pickitConfig_ini.GetString("Pickit", "Full Commands", szMethod, 1024);
	Me.SetFullCmds(szMethod);

	pickitConfig_ini.GetString("Pickit", "Log File", szMethod, 1024);
	Me.SetLogFilePath(szMethod);
	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	Me.OnGameJoin(game);

	// Pickit must be loaded before game join, otherwise some important features
	// will not be available until the player joins a new game.
	Me.DisplayNotLoadedBeforeJoinWarning("pickit");
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* aThisgame)
{
	Me.OnGameLeave();
}

// is run ~every 1/10th a second
DWORD EXPORT OnGameTimerTick(void)
{
	Me.OnGameTimerTick(); // call every time tick
	return 0;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	Me.OnGamePacketBeforeReceived(aPacket, aLen);
	return aLen;
}

// functions related to the above commands
BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	Me.SetDebug(!Me.IsDebug());
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

	Me.ShowClientMove(bShow);
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

	Me.SetRadius(strtoul(argv[2], NULL, 10));
	pickitConfig_ini.WriteInt("Pickit", "Radius", Me.GetRadius());
	ShowRadiusSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandToggle(char **argv, int argc)
{	
	if (Me.Toggle())
		server->GamePrintInfo("ÿc4Pickit enabled.");
	else
		server->GamePrintInfo("ÿc4Pickit disabled.");
	return TRUE;
}

BOOL PRIVATE OnCommandEnable(char **argv, int argc)
{
	Me.Enable(TRUE);
	server->GamePrintInfo("ÿc4Pickit enabled.");
	return TRUE;
}

BOOL PRIVATE OnCommandDisable(char** argv, int argc)
{
	Me.Enable(FALSE);
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
	sprintf(szFile, "%s\\", GetModuleDirectory());
	for (int i = 2; i < argc; i++)
		strcat(szFile, argv[i]);

	if (Me.LoadConfig(szFile))
		ShowIniFileSettings();
	else
		::D2Errorf("ÿc4Failed to load file ÿc0%s!", szFile);
		
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
	ShowDelaySetting();
	ShowRadiusSetting();
	ShowTownPickSetting();
	ShowClientMoveSetting();
	ShowGoldSetting();
	ShowRejuvSetting();
	ShowFullRejuvSetting();	
	
	server->GamePrintInfo("ÿc4---------------------------------------");
	return TRUE;
}

void ShowIniFileSettings()
{
	::D2Infof("ÿc4Config File:     ÿc0%s", Me.GetConfigFile());
}

void ShowFullCmdSetting()
{
	LPCTSTR lpszCmds = Me.GetFullCmds();
	if (lpszCmds != NULL)
		::D2Infof("ÿc4Full Commands:   ÿc0%s", lpszCmds);
	else
		server->GamePrintInfo("ÿc4Full Commands:");
}

void ShowLogFileSettings()
{
	::D2Infof("ÿc4Log File:   ÿc0%s", Me.GetLogFilePath());
}

void ShowEnableDisableSetting()
{
	::D2Infof("ÿc4Enabled:         ÿc2%s", Me.IsEnabled() ? "Yes" : "No");
}

void ShowDelaySetting()
{
	::D2Infof("ÿc4Delay:           ÿc2%d", Me.GetDelay());
}

void ShowRadiusSetting()
{
	::D2Infof("ÿc4Radius:          ÿc2%d", Me.GetRadius());
}

void ShowTownPickSetting()
{
	if (Me.IsAllowTownPick())
		server->GamePrintInfo("ÿc4Town Pick:  ÿc2On");
	else
		server->GamePrintInfo("ÿc4Town Pick:  ÿc1Off");
}

void ShowClientMoveSetting()
{
	if (Me.IsShowClientMove())
		server->GamePrintInfo("ÿc4Client Move:  ÿc2On");
	else
		server->GamePrintInfo("ÿc4Client Move:  ÿc1Off");
}

void ShowGoldSetting()
{
	char szMethod[16] = "";
	Method2Str(Me.GetSpecialItemMethod(PICKIT_SPECIAL_GOLD), szMethod);
	::D2Infof("ÿc4Gold:            ÿc2%s", szMethod);
}

void ShowRejuvSetting()
{
	char szMethod[16] = "";
	Method2Str(Me.GetSpecialItemMethod(PICKIT_SPECIAL_REJUV), szMethod);
	::D2Infof("ÿc4Rejuv:           ÿc2%s", szMethod);
}

void ShowFullRejuvSetting()
{
	char szMethod[16] = "";
	Method2Str(Me.GetSpecialItemMethod(PICKIT_SPECIAL_FULLREJUV), szMethod);
	::D2Infof("ÿc4Full Rejuv:      ÿc2%s", szMethod);
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

	Me.SetSpecialItemMethod(PICKIT_SPECIAL_GOLD, nMethod);
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

	Me.SetSpecialItemMethod(PICKIT_SPECIAL_REJUV, nMethod);
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

	Me.SetSpecialItemMethod(PICKIT_SPECIAL_FULLREJUV, nMethod);
	pickitConfig_ini.WriteInt("Pickit", "Full Rejuv", nMethod);	
	ShowFullRejuvSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandLog(char** argv, int argc)
{
	Me.SetLogFilePath(NULL);
	if (argc < 3)
	{
		ShowLogFileSettings();
		return TRUE;
	}

	char szLogFile[MAX_PATH + 1] = "";
	sprintf(szLogFile, "%s\\", GetModuleDirectory());
	for (int i = 2; i < argc; i++)
		sprintf(szLogFile, "%s%s ", szLogFile, argv[i]);
	Me.SetLogFilePath(szLogFile);
	pickitConfig_ini.WriteString("Pickit", "Log File", Me.GetLogFilePath());
	ShowLogFileSettings();
	return TRUE;
}

BOOL PRIVATE OnCommandFullCmds(char** argv, int argc)
{
	if (argc < 3)
	{
		Me.SetFullCmds(NULL);
		pickitConfig_ini.WriteString("Pickit", "Full Commands", "");
		server->GamePrintInfo("ÿc4Inventory full commands disabled.");
		return TRUE;
	}
	
	char szCmds[1025] = "";
	for (int i = 2; i < argc; i++)
		sprintf(szCmds, "%s%s ", szCmds, argv[i]);

	Me.SetFullCmds(szCmds);
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

	Me.SetAllowTownPick(bAllow);
	pickitConfig_ini.WriteBool("Pickit", "Town Pick", bAllow);
	ShowTownPickSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandDelay(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .pickit delay <delay-milliseconds>");
		return TRUE;
	}

	Me.SetDelay(strtoul(argv[2], NULL, 10));
	pickitConfig_ini.WriteInt("Pickit", "Delay", Me.GetDelay());
	ShowDelaySetting();
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