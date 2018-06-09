/////////////////////////////////////////////////////////////////////////
// Takeit.cpp
//
// Written by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////

#include "GetIt.h"
#include "..\\Includes\\ClientCore.cpp"

const static DWORD CAINS[5] = { NPC_A1_CAIN, NPC_A2_CAIN, NPC_A3_CAIN, NPC_A4_CAIN, NPC_A5_CAIN };
const static DWORD HEALERS[5] = { NPC_A1_AKARA, NPC_A2_FARA, NPC_A3_ORMUS, NPC_A4_JAMELLA, NPC_A5_MALAH };

void CALLBACK FreeConfig(LPIDCONFIG& pCfg)
{
	if (pCfg)
	{
		if (pCfg->pszSection)
			::free(pCfg->pszSection);

		for (int i = 0; pCfg->aExpressions[i]; i++)
		{
			if (pCfg->aExpressions[i])
				::free(pCfg->aExpressions[i]);
		}

		delete pCfg;
		pCfg = NULL;
	}
}

BOOL operator==(const IDITEM& lhs, const IDITEM& rhs)
{
	return ::memcmp(&lhs.itemSize, &rhs.itemSize, sizeof(SIZE)) == 0;
}

BOOL operator>(const IDITEM& lhs, const IDITEM& rhs)
{
	return lhs.itemSize.cy > rhs.itemSize.cy || lhs.itemSize.cx > rhs.itemSize.cx;
}

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
	
	if (nMessage == PM_STASHOPEN && g_ticker.GetState() == ID_WAIT_STASH)
		g_ticker.Update(ID_DEPOSIT);
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
	sprintf(szFile, "%s\\%s", server->GetModuleDirectory(), "GetIt.ini");
	g_picker.LoadConfig(szFile);

	g_ini.SetPathName(szFile);	

	g_picker.SetDelay(g_ini.GetUInt("Global", "Delay", 300));
	g_picker.SetRadius(g_ini.GetInt("Global", "Radius", 15));
	g_picker.ShowClientMove(g_ini.GetBool("Global", "Client Move", FALSE));
	g_picker.SetAllowTownPick(g_ini.GetBool("Global", "Town Pick", TRUE));

	char szMethod[1025] = "";
	g_ini.GetString("Global", "full cmds", szMethod, 1024);
	g_picker.SetFullCmds(szMethod);

	g_ini.GetString("Global", "Log", szMethod, 1024);
	g_picker.SetLogFilePath(szMethod);

	g_nLeftCol = g_ini.GetInt("Global", "Left Col", 1024);
	g_nRightCol = g_ini.GetInt("Global", "Right Col", -1);
	g_ini.GetString("Global", "Stash cmds", g_szCallback, 255);
	g_bUseCube = g_ini.GetBool("Global", "Use Cube", TRUE);

	g_ini.EnumSections(EnumSectionsProc, (LPVOID)&g_aConfigs);	
	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	g_picker.OnGameJoin();
	g_ticker.Clear();
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* aThisgame)
{
	g_picker.OnGameLeave();
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
		server->GamePrintError("ÿc4Syntax: .getit clientmove <0/1>");
		return TRUE;
	}

	int nShow = atoi(argv[2]);
	g_picker.ShowClientMove(nShow);
	g_ini.WriteBool("Global", "Client Move", !!nShow);
	ShowClientMoveSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandRadius(char **argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .getit radius <radius>");
		return TRUE;
	}

	g_picker.SetRadius(strtoul(argv[2], NULL, 10));
	g_ini.WriteInt("Global", "Radius", g_picker.GetRadius());
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
		server->GamePrintError("ÿc4Syntax: .getit load <file name>");
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
	ShowDelaySetting();
	ShowRadiusSetting();
	ShowTownPickSetting();
	ShowClientMoveSetting();
	
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
	server->GameInfof("ÿc4Enabled:    ÿc2%d", g_picker.IsEnabled());
}

void ShowRadiusSetting()
{
	server->GameInfof("ÿc4Radius:     ÿc2%d", g_picker.GetRadius());
}

void ShowTownPickSetting()
{
	server->GameInfof("ÿc4Town Pick:  ÿc2%d", g_picker.IsAllowTownPick());
}

void ShowClientMoveSetting()
{
	server->GameInfof("ÿc4Client Move: ÿc2%d", g_picker.IsShowClientMove());
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
	g_ini.WriteString("Global", "Log", g_picker.GetLogFilePath());
	ShowLogFileSettings();
	return TRUE;
}

BOOL PRIVATE OnCommandFullCmds(char** argv, int argc)
{
	if (argc < 3)
	{
		g_picker.SetFullCmds(NULL);
		g_ini.WriteString("Global", "Callback", "");
		server->GamePrintInfo("ÿc4Inventory full commands disabled.");
		return TRUE;
	}
	
	char szCmds[1025] = "";
	for (int i = 2; i < argc; i++)
		sprintf(szCmds, "%s%s ", szCmds, argv[i]);

	g_picker.SetFullCmds(szCmds);
	g_ini.WriteString("Global", "Callback", szCmds);
	ShowFullCmdSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandTownPick(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .getit townpick <0/1>");
		return TRUE;
	}

	BOOL bAllow = atoi(argv[2]);
	g_picker.SetAllowTownPick(bAllow);
	g_ini.WriteBool("Global", "Town Pick", bAllow);
	ShowTownPickSetting();
	return TRUE;
}

BOOL PRIVATE OnCommandDelay(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: .getit delay <delay-milliseconds>");
		return TRUE;
	}

	g_picker.SetDelay(strtoul(argv[2], NULL, 10));
	g_ini.WriteInt("Global", "Delay", g_picker.GetDelay());
	ShowDelaySetting();
	return TRUE;
}

BOOL PRIVATE OnCommandStash(char** argv, int argc)
{
	if (g_ticker.GetState() != ID_NONE)
	{
		server->GamePrintError("GetIt is already in an \"stash\" command session!");
		return TRUE;
	}

	if (!me->IsInTown())
	{
		server->GamePrintError("Player must be in town to start an \"stash\" command session!");
		CItemPicker::InvokeCallbackCommands(g_szCallback);
		return TRUE;
	}
	
	int nCount = GetGoodItems();
	if (nCount == 0 && me->GetStat(STAT_GOLD) == 0)
	{
		server->GameInfof("There is no valuable items between column %d - %d.", g_nLeftCol, g_nRightCol);
		CItemPicker::InvokeCallbackCommands(g_szCallback);
		return TRUE;
	}	
	
	me->CloseAllUIs();
	server->GamePrintInfo("GetIt is starting an \"stash\" command session.");
	g_bFull = FALSE;
	server->GameInfof("Found ÿc2%dÿc0 valuable items between column %d - %d", nCount, g_nLeftCol, g_nRightCol);
	g_ticker.Clear();
	g_ticker.Update(ID_CHECK_STASH, 1000);	
	return TRUE;
}

void ShowDelaySetting()
{
	server->GameInfof("ÿc4Delay:    ÿc2%d", g_picker.GetDelay());
}

DWORD EXPORT OnGameTimerTick(void)
{
	if (!server->IsGameReady() || !me->IsInTown())
	{
		if (g_ticker.GetState() != ID_NONE)
			g_ticker.Clear();
		return 0;
	}

	g_ticker.TickIt(100);
	if (g_ticker.IsWaiting() || !g_ticker.IsTickEnded())
		return 0;

	static int nCount = 0;
	static GAMEUNIT stash;
	static POINT pt;

	switch (g_ticker.GetState())
	{
	case ID_CHECK_STASH:
		g_nStashRetry = 0;

		if (me->GetStat(STAT_GOLD) == 0)
		{
			g_ticker.Update(ID_PICK_UP);
			break;
		}
		
		if (!server->FindUnitByClassID(267, UNIT_TYPE_OBJECT, &stash))
		{
			g_ticker.Update(ID_PICK_UP);
			break;
		}

		pt = server->GetUnitPosition(&stash);
		if (me->GetDistanceFrom(pt.x, pt.y) > 20)
		{
			g_ticker.Update(ID_PICK_UP);
			break;
		}

		g_ticker.Update(ID_OPEN_STASH);
		break;

	case ID_OPEN_STASH:
		StateMessage("Opening stash");
		me->Interact(&stash);
		g_ticker.Update(ID_WAIT_STASH, 5000);
		break;

	case ID_WAIT_STASH:
		g_nStashRetry++;
		if (g_nStashRetry < 8)
			g_ticker.Update(ID_OPEN_STASH);
		else
			g_ticker.Update(ID_PICK_UP);
		
		break;

	case ID_DEPOSIT:
		StateMessage("Depositting gold");
		me->DepositGoldToStash(0);
		g_ticker.Update(ID_PICK_UP);
		break;
		
	case ID_PICK_UP:
		if (g_aItems.IsEmpty())
		{
			g_ticker.Update(ID_END);
			break;
		}
		
		StateMessage("Picking items");
		me->PickStorageItemToCursor(g_aItems[0].dwItemID);
		g_ticker.Update(ID_WAIT_PICK, 300);
		break;

	case ID_WAIT_PICK:
		g_dwCurItem = me->GetCursorItem();
		if (g_dwCurItem)
		{
			RemoveItem(g_dwCurItem);
			g_dwCurItem = 0;
			g_ticker.Update(ID_DROP_DOWN);
		}
		else
		{
			g_ticker.Update(ID_PICK_UP);			
		}
		break;

	case ID_DROP_DOWN:
		StateMessage("Moving items");
		if (!g_bFull)
		{
			g_bFull = !me->DropCursorItemToStorage(STORAGE_STASH);
			if (g_bFull && g_bUseCube)
				g_bFull = !me->DropCursorItemToStorage(STORAGE_CUBE);
		}
		
		if (g_bFull)
			me->DropCursorItemToStorage(STORAGE_INVENTORY);

		g_ticker.Update(ID_WAIT_DOWN);
		break;

	case ID_WAIT_DOWN:
		if (me->GetCursorItem())
		{
			g_ticker.Update(ID_DROP_DOWN);
		}
		else
		{
			if (g_bFull)
				g_ticker.Update(ID_END);
			else
				g_ticker.Update(ID_PICK_UP);
		}

		break;

	case ID_END:
		me->CloseAllUIs();
		server->GamePrintInfo("GetIt completed the \"stash\" command session successfully.");
		g_ticker.Clear();
		StateMessage("Stash command finished");
		CItemPicker::InvokeCallbackCommands(g_szCallback);
		break;

	default:
		break;
	}
	return 0;
}

BOOL CALLBACK EnumItemProc(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam)
{
	if ((int)x >= g_nLeftCol && (int)x <= g_nRightCol && IsGoodItem(dwItemID)) 
	{
		IDITEM ii;
		ii.dwItemID = dwItemID;
		ii.itemSize = server->GetItemSize(lpszItemCode);
		g_aItems.Add(ii);
	}
	return TRUE;
}

int GetGoodItems()
{
	g_aItems.RemoveAll();
	me->EnumStorageItems(STORAGE_INVENTORY, EnumItemProc, 0);
	g_aItems.Sort(FALSE); // sort ascending, we move bigger items first
	return g_aItems.GetSize();
}

BOOL CALLBACK EnumSectionsProc(LPCTSTR lpSection, LPVOID lpParam)
{
	IDConfigList* pList = (IDConfigList*)lpParam;
	if (pList == NULL)
		return FALSE;

	if (!::stricmp(lpSection, "global"))
		return TRUE;

	LPIDCONFIG p = new IDCONFIG;
	::memset(p, 0, sizeof(IDCONFIG));
	g_ini.GetString(lpSection, "Code", p->szCodes, 255);
	if (*p->szCodes == 0)
	{
		delete p;
		return TRUE;
	}

	if (::strcmp(p->szCodes, "*") == 0)
		*p->szCodes = 0;

	g_ini.EnumKeys(lpSection, EnumKeysProc, (LPVOID)p);
	if (p->aExpressions[0] == NULL)
	{		
		delete p;
		return TRUE;
	}
	
	p->pszSection = ::strdup(lpSection);
	pList->InsertRear(p);
	return TRUE;
}

BOOL CALLBACK EnumKeysProc(LPCTSTR lpKeyName, LPCTSTR lpValue, LPVOID lpParam)
{
	LPIDCONFIG p = (LPIDCONFIG)lpParam;
	if (p == NULL)
		return FALSE;

	if (!::stricmp(lpKeyName, "keep") && *lpValue)
	{
		for (int i = 0; i < 64; i++)
		{
			if (p->aExpressions[i] == NULL)
			{
				p->aExpressions[i] = ::strdup(lpValue);
				break;
			}
		}
	}

	return TRUE;
}

BOOL IsGoodItem(DWORD dwItemID)
{
	DWORD dwAttr = server->GetItemAttributes(dwItemID);
	if ((dwAttr & ITEM_IDENTIFIED) == 0 || (dwAttr & ITEM_RUNEWORD))
		return TRUE;

	POSITION pos = g_aConfigs.GetHeadPosition();
	while (pos)
	{
		LPIDCONFIG p = g_aConfigs.GetNext(pos);
		for (int i = 0; i < 64; i++)
		{
			if (p->aExpressions[i] == NULL)
				continue;

			BOOL bSyntaxError = FALSE;
			BOOL bMatch = server->ExamItemProperties(dwItemID, p->aExpressions[i], p->szCodes, &bSyntaxError);
			if (bSyntaxError)
			{
				::free(p->aExpressions[i]);
				p->aExpressions[i] = NULL;
				server->GameErrorf("Syntax Error in section: %s", p->pszSection);
				continue;
			}

			if (bMatch)
				return TRUE;
		}
	}

	return FALSE;
}

void StateMessage(LPCSTR lpszMsg)
{
	if (lpszMsg && *lpszMsg)
		me->OverHead(lpszMsg, TRUE);
}

void RemoveItem(DWORD dwItemID)
{
	for (int i = 0; i < g_aItems.GetSize(); i++)
	{
		if (g_aItems[i].dwItemID == dwItemID)
		{
			g_aItems.RemoveAt(i);
			return;
		}
	}
}