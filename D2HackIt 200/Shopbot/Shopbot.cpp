//////////////////////////////////////////////////////////////////////
// Shopbot.cpp
// -------------------------------------------------------------------
// <abinn32@yahoo.com>
//////////////////////////////////////////////////////////////////////
#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include "..\\Includes\\ClientCore.cpp"	  // Include the backend of the module
#include "..\\includes\\utilities\\stateticker.h"
#include "..\\includes\\utilities\\ini.h"
#include "..\\includes\\utilities\\ArrayEx.h"
#include "SBDef.h"

//////////////////////////////////////////////////////////////////////
// Global stuff.
// -------------------------------------------------------------------
// This is a very convenient place to your function declarations and
// global variables.
//////////////////////////////////////////////////////////////////////

enum {
	STATE_NONE = 0, // not started

	STATE_WAIT_MOVE, // waiting for PM_MOVECOMPLETE
	STATE_WAIT_WP, // waiting for "EnterWaypoint" ends
	STATE_WAIT_NPC, // waiting for "PM_NPCSESSION"

	STATE_CHANGE_ACT_TO_WP, // changing act
	STATE_CHANGE_ACT_ENTER_WP, // entering wp to shopping act
	STATE_CHANGE_ACT_OK, // act changed

	STATE_VERIFY, // verifying objects
	STATE_INIT_TO_WP, // first to shopping town wp

	STATE_RAND_TO_NPC, // anti-detection 1
	STATE_WP_TO_NPC, // walk from wp to npc
	STATE_OPEN_STORE, // open npc store
	STATE_WAIT_ITEMS, // wait for vendor items
	STATE_BUY_ITEM, // buy items!
	STATE_LEAVE_NPC, // close npc store
	STATE_AFTER_NPC, // check for acceleration spells
	STATE_RAND_TO_WP, // anti-detection 2
	STATE_NPC_TO_WP, // walk from npc to wp
	STATE_LEAVE_TOWN, // leave town
	STATE_BACK_TO_TOWN, // back to town

	STATE_BACK_TO_ORIGIN, // walk back to original position
	STATE_EXECUTE_CALLBACK, // execute callback commands
	STATE_END // ended
};

typedef struct tagSBConfig
{
	char szSection[256]; // ini section name
	char szCodes[512]; // item codes
	char szExp[512]; // expression
	BOOL bValid; // valid expression?
} SBCONFIG, *LPSBCONFIG;
typedef const tagSBConfig* LPCSBCONFIG;

// Persist data
char g_szCallback[512] = "";
char g_szWpName[64] = "";
BYTE g_iOutMapID = 0;
char g_szOutMapName[128] = "";
SHOP g_shop = { 0 };
BOOL g_bAutoStart = FALSE;
BYTE g_iOriginMapID = 0;
POINT g_ptOrigin = { 0 };
BYTE g_iHotkey = VK_PAUSE;

// Volatile data
BOOL g_bReturn = FALSE;
DWORD g_dwTimeRemain = 0;
DWORD g_dwShopVisited = 0;
DWORD g_dwItemChecked = 0;
DWORD g_dwItemBought = 0;
POINT g_ptTownWp = { 0 };
CStateTicker g_ticker;
CArrayEx<DWORD, DWORD> g_aGoodItems;
CArrayEx<SBCONFIG, const SBCONFIG&> g_aConfig;

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);
BOOL PRIVATE OnGameCommandStart(char** argv, int argc);
BOOL PRIVATE OnGameCommandStop(char** argv, int argc);
BOOL PRIVATE OnGameCommandPause(char** argv, int argc);
BOOL PRIVATE OnGameCommandReturn(char** argv, int argc);

BOOL CALLBACK EnumSectionsProc(LPCTSTR lpSection, LPVOID lpParam);
BOOL UpdateWpName(LPCGAMEUNIT lpUnit);
DWORD AntiDetectMove(int x, int y);
void StateMessage(LPCSTR lpszMsg);
BOOL ExamItem(DWORD dwItemID);
BOOL AnnounceItem(DWORD dwItemID);
BOOL CheckBoughtItem(DWORD dwItemID);
int CalcPathAndGo(int x, int y);
DWORD Accelerate();
void ExecuteCallback();
BOOL GetTimeFormat(LPSTR lpszBuffer, BYTE& iColor);

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
// -------------------------------------------------------------------
// Tell us a little about this module.
//////////////////////////////////////////////////////////////////////
CLIENTINFO
(		
	2,0,							// Module version (Major, Minor)
	"Abin",						// Author
	"www.wxjindu.com/abin/",		// url (http:// is appended)
	"Shopbot",				// Short module description
	"abinn32@yahoo.com"				// Author eMail
)

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	{
		// Debug
		"debug",
		OnGameCommandDebug,
		"debugÿc0 Debug your module."
	},

	{
		"start",
		OnGameCommandStart,
		"startÿc0 .shopbot start <milliseconds>"
	},

	{
		"stop",
		OnGameCommandStop,
		"stopÿc0 .shopbot stop"
	},

	{
		"pause",
		OnGameCommandPause,
		"pauseÿc0 .shopbot pause"
	},

	{
		"return",
		OnGameCommandReturn,
		"returnÿc0 .shopbot return"
	},

	// Add your own commands here

	{NULL}	// No more commands
};

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	// Put debug code here...
	server->GameInfof("state %d, paused %d, remain %d, waiting %d, return %d",
		g_ticker.GetState(), g_ticker.IsPaused(), g_ticker.GetRemainer(), g_ticker.IsWaiting(), g_bReturn);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// OnClientStart
// -------------------------------------------------------------------
// Runs *once* when the module loads. 
// This function can be removed.
//
// Put any initialization code here.
//
// Return FALSE to prevent module from loading.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{
	::memset(&g_shop, 0, sizeof(SHOP));
	char szPath[MAX_PATH + 1] = "";
	::sprintf(szPath, "%s\\Shopbot.ini", server->GetModuleDirectory());
	CIni ini(szPath);

	GetShop(ini.GetUInt("Global", "npc", 511), &g_shop, FALSE);
	g_iOutMapID = ini.GetUInt("Global", "out step", MAP_A5_WORLDSTONE_KEEP_LEVEL_2);
	server->GetMapName(g_iOutMapID, g_szOutMapName, 127);
	g_bAutoStart = ini.GetBool("Global", "auto start", FALSE);
	g_iHotkey = ini.GetUInt("Global", "hotkey", VK_PAUSE);
	ini.GetString("Global", "callback", g_szCallback, 511);

	g_aConfig.RemoveAll();
	ini.EnumSections(EnumSectionsProc, (LPVOID)&ini);
	return TRUE;
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
	if (nMessage != UM_ITEMEVENT/* || g_ticker.GetState() == STATE_NONE*/)
		return;

	if (wParam == ITEM_EVENT_TO_STORE)
	{
		// exam the item
		g_dwItemChecked++;
		if (ExamItem(lpUnit->dwUnitID))
		{
			AnnounceItem(lpUnit->dwUnitID);
			g_aGoodItems.Lock();
			g_aGoodItems.Add(lpUnit->dwUnitID);
			g_aGoodItems.Unlock();
		}
	}
	//else if (wParam == ITEM_EVENT_FROM_STORE)
	//{
		// commented out for backward compatibility
		// for those who are using earlier than build 1123, before which
		// the UM_ITEMEVENT (wParam==ITEM_EVENT_FROM_STORE) was not sent.
		//CheckBoughtItem(lpUnit->dwUnitID);
	//}
}

//////////////////////////////////////////////////////////////////////
// OnThisPlayerMessage - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when a this-player message is sent to the client
//
// Refer to the documentation for the format of this-player messages.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (g_ticker.GetState() == STATE_NONE)
		return;
	
	switch (nMessage)
	{
	case PM_MOVECOMPLETE:
		if (g_ticker.GetState() == STATE_WAIT_MOVE)
			g_ticker.EndWait(wParam);
		break;
		
	case PM_NPCSESSION:
		if (g_ticker.GetState() == STATE_WAIT_NPC)
			g_ticker.EndWait(wParam);
		break;

	case PM_ACTCHANGE:
	case PM_LEAVETOWN:
	case PM_ENTERTOWN:
		if (g_ticker.GetState() == STATE_WAIT_WP)
			g_ticker.EndWait(TRUE, Accelerate());
		break;

	case PM_WPFAIL:
		me->CloseAllUIs();
		g_ticker.EndWait(FALSE, 1000);
		break;

	default:
		break;
	}
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	// backward compatible
	// for those who are using earlier than build 1123, before which
	// the UM_ITEMEVENT (wParam==ITEM_EVENT_FROM_STORE) was not sent.
	if (aPacket[0] == 0x9c && aPacket[1] == ITEM_EVENT_FROM_STORE)
	{
		CheckBoughtItem(*((DWORD*)(aPacket + 4)));
	}
	return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGameKeyDown - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when a key is pressed.
//
// Return zero to prevent Diablo II from receiving this key press. Other
// modules will receive the key press regardless of the return values.
//////////////////////////////////////////////////////////////////////
BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if (iKeyCode == g_iHotkey)
	{
		OnGameCommandPause(NULL, 0);
		return 0;
	}

	return iKeyCode;
}

//////////////////////////////////////////////////////////////////////
// OnDraw - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when the game screen needs to be drawn.
//
// Parameters:
// 
// pDC - Pointer to a game device context structure, use member functions
//       of this structure to draw your own items, for example:
//       pDC->MoveTo(0, 0); pDC->LineTo(400, 300, 0x62);
//       will draw a red line from (0,0) to (400,300).
// lpScreenRect - Pointer to the client screen rect boundary.
//
// Check D2Hackit API document for more details.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnDraw(CGameDC* pDC, LPCRECT lpScreenRect)
{
	// TODO: Draw your own texts/graphs on the screen.
	if (g_ticker.GetState() == STATE_NONE)
		return;

	// we will draw at bottom-left corner
	const int LINE_GAP = 15;
	int x1 = 20, x2 = 120;
	int y = lpScreenRect->bottom - 180;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetFont(D2FONT_THIN);	

	static char szTemp[128] = "";
	static BYTE iColor = 0;

	if (GetTimeFormat(szTemp, iColor))
	{
		pDC->SetTextColor(9);
		pDC->TextOut("Time Remain", x1, y);	
		pDC->SetTextColor(iColor);
		pDC->TextOut(szTemp, x2, y);
		y += LINE_GAP;
	}

	pDC->SetTextColor(9);
	pDC->TextOut("NPC Name", x1, y);	
	pDC->SetTextColor(0);
	pDC->TextOut(g_shop.szNpcName, x2, y);

	y += LINE_GAP;
	pDC->SetTextColor(9);
	pDC->TextOut("Out Step", x1, y);	
	pDC->SetTextColor(0);
	pDC->TextOut(g_szOutMapName, x2, y);

	y += LINE_GAP;
	pDC->SetTextColor(9);
	pDC->TextOut("Shop Visited", x1, y);	
	::sprintf(szTemp, "%d", g_dwShopVisited);
	pDC->SetTextColor(0);
	pDC->TextOut(szTemp, x2, y);

	y += LINE_GAP;
	pDC->SetTextColor(9);
	pDC->TextOut("Items Checked", x1, y);
	::sprintf(szTemp, "%d", g_dwItemChecked);
	pDC->SetTextColor(0);
	pDC->TextOut(szTemp, x2, y);

	y += LINE_GAP;
	pDC->SetTextColor(9);
	pDC->TextOut("Items Bought", x1, y);
	::sprintf(szTemp, "%d", g_dwItemBought);
	pDC->SetTextColor(0);
	pDC->TextOut(szTemp, x2, y);
}

//////////////////////////////////////////////////////////////////////
// OnGameJoin
// -------------------------------------------------------------------
// Executes when joining a game, and if loading the module while 
// inside a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	g_dwTimeRemain = 0;
	g_bReturn = FALSE;
	g_ticker.Clear();
	::memset(&g_ptTownWp, 0, sizeof(POINT));
	g_aGoodItems.Lock();
	g_aGoodItems.RemoveAll();
	g_aGoodItems.Unlock();
	return;
}

BOOL PRIVATE OnGameCommandStart(char** argv, int argc)
{
	if (g_ticker.GetState() != STATE_NONE)
	{
		server->GamePrintError("Shopbot already started!");
		return TRUE;
	}
	
	if (!me->IsInTown())
	{
		server->GamePrintError("Player is not in town!");
		return TRUE;
	}

	if (*g_shop.szNpcName == 0 || g_shop.iAct > 4)
	{
		server->GameErrorf("Invalid npc: %d!", g_shop.dwNpcClassID);
		return TRUE;
	}

	g_iOriginMapID = me->GetCurrentMapID();
	g_ptOrigin = me->GetPosition();

	g_dwTimeRemain = -1;
	if (argc > 2)
		g_dwTimeRemain = strtoul(argv[2], NULL, 10) * 1000;
	
	g_bReturn = FALSE;
	me->CloseAllUIs();
	me->PauseJobs(FALSE);
	g_ticker.Clear();
	g_ticker.Update(STATE_CHANGE_ACT_TO_WP, Accelerate());
	server->GamePrintInfo("Shopbot started.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandStop(char** argv, int argc)
{
	if (g_ticker.GetState() == STATE_NONE)
	{
		server->GamePrintError("Shopbot was not started!");
		return TRUE;
	}

	me->PauseJobs(FALSE);
	g_ticker.EndWait(FALSE);
	g_ticker.Resume();
	g_ticker.Update(STATE_EXECUTE_CALLBACK);
	return TRUE;
}

BOOL PRIVATE OnGameCommandPause(char** argv, int argc)
{
	if (g_ticker.GetState() == STATE_NONE)
	{
		server->GamePrintError("Shopbot was not started!");
		return TRUE;
	}
	
	if (!g_ticker.IsPaused())
	{
		g_ticker.Pause();
		server->GamePrintInfo("Shopbot paused.");
		me->PauseJobs(TRUE);
	}
	else
	{
		g_ticker.Resume();
		server->GamePrintInfo("Shopbot resumed.");
		me->PauseJobs(FALSE);
	}
	return TRUE;
}

BOOL PRIVATE OnGameCommandReturn(char** argv, int argc)
{
	if (g_ticker.GetState() == STATE_NONE)
	{
		server->GamePrintError("Shopbot was not started!");
		return TRUE;
	}

	if (g_bReturn)
	{
		server->GamePrintError("Shopbot is already returning!");
		return TRUE;
	}

	g_bReturn = TRUE;
	server->GamePrintInfo("Shopbot is returning to original position...");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameTimerTick
// -------------------------------------------------------------------
// This gets executed apx. every 1/10th of a second when in a game.
//
// You can use this to create custom timers.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGameTimerTick(void)
{
	if (g_bAutoStart && g_ticker.GetState() == STATE_NONE)
	{
		if (server->IsGameReady())
			server->GameCommandLine("shopbot start");
	}

	if (g_ticker.GetState() == STATE_NONE
		|| g_ticker.IsPaused())
		return 0;

	if (g_dwTimeRemain != -1)
	{
		if (g_dwTimeRemain >= 100)
			g_dwTimeRemain -= 100;
		else
			g_dwTimeRemain = 0;

		if (g_dwTimeRemain == 0)
			g_bReturn = TRUE;
	}	

	g_ticker.TickIt(100);
	if (g_ticker.IsWaiting() || !g_ticker.IsTickEnded())
		return 0;

	static GAMEUNIT unit;

	switch (g_ticker.GetState())
	{
	case STATE_CHANGE_ACT_TO_WP:
		if (me->GetCurrentMapID() == server->GetTownMap(g_shop.iAct))
		{
			g_ticker.Update(STATE_CHANGE_ACT_OK);
			break;
		}
		
		StateMessage("Walking to waypoint");
		g_ptTownWp = server->FindPresetUnit(TOWNWPS[me->GetCurrentAct()], UNIT_TYPE_OBJECT);
		CalcPathAndGo(g_ptTownWp.x, g_ptTownWp.y);
		g_ticker.Update(STATE_WAIT_MOVE);
		g_ticker.StartWait(30000, STATE_CHANGE_ACT_ENTER_WP, STATE_CHANGE_ACT_TO_WP);
		break;

	case STATE_CHANGE_ACT_ENTER_WP:
		
		if (!server->FindUnitByClassID(TOWNWPS[me->GetCurrentAct()], UNIT_TYPE_OBJECT, &unit))
		{
			g_ticker.Update(STATE_CHANGE_ACT_TO_WP, 3000);
			break;
		}
		
		StateMessage("Entering waypoint");
		me->EnterWaypoint(&unit, server->GetTownMap(g_shop.iAct));
		g_ticker.Update(STATE_WAIT_WP);
		g_ticker.StartWait(30000, STATE_CHANGE_ACT_OK, STATE_CHANGE_ACT_TO_WP, 1000);
		break;

	case STATE_CHANGE_ACT_OK:
		StateMessage("Checking objects");
		GetShop(g_shop.dwNpcClassID, &g_shop, TRUE);
		g_ptTownWp = server->FindPresetUnit(TOWNWPS[g_shop.iAct], UNIT_TYPE_OBJECT);
		g_ticker.Update(STATE_INIT_TO_WP);
		break;

	case STATE_INIT_TO_WP:
		StateMessage("Walking to waypoint");
		CalcPathAndGo(g_ptTownWp.x, g_ptTownWp.y);
		g_ticker.Update(STATE_WAIT_MOVE);
		g_ticker.StartWait(30000, STATE_RAND_TO_NPC, STATE_INIT_TO_WP);
		break;

	case STATE_RAND_TO_NPC:
		if (g_bReturn)
		{
			g_ticker.Update(STATE_BACK_TO_ORIGIN);
			break;
		}

		StateMessage("Walking to npc");		
		g_ticker.Update(STATE_WP_TO_NPC, AntiDetectMove(g_shop.x, g_shop.y), 100);
		break;

	case STATE_WP_TO_NPC:
		StateMessage("Walking to npc");
		CalcPathAndGo(g_shop.x, g_shop.y);
		g_ticker.Update(STATE_WAIT_MOVE);
		g_ticker.StartWait(30000, STATE_OPEN_STORE, STATE_WP_TO_NPC);		
		break;

	case STATE_OPEN_STORE:
		if (g_bReturn)
		{
			g_ticker.Update(STATE_AFTER_NPC);
			break;
		}
		
		StateMessage("opening npc store");
		g_aGoodItems.Lock();
		g_aGoodItems.RemoveAll();
		g_aGoodItems.Unlock();
		server->FindUnitByClassID(g_shop.dwNpcClassID, UNIT_TYPE_MONSTER, &unit);
		me->StartNpcSession(&unit, NPC_TRADE);
		g_ticker.Update(STATE_WAIT_NPC);
		g_ticker.StartWait(15000, STATE_WAIT_ITEMS, STATE_OPEN_STORE);
		//g_ticker.Clear();
		break;

	case STATE_WAIT_ITEMS:
		StateMessage("Checking items");
		g_ticker.Update(STATE_BUY_ITEM, 2000);
		break;

	case STATE_BUY_ITEM:

		if (g_aGoodItems.IsEmpty())
		{
			g_ticker.Update(STATE_LEAVE_NPC);
		}
		else
		{
			StateMessage("ÿc2Buying items...");		
			g_aGoodItems.Lock();		
			me->BuyItem(g_aGoodItems[0]);
			g_aGoodItems.Unlock();
			g_ticker.Update(STATE_BUY_ITEM, 3000);
		}
		
		break;

	case STATE_LEAVE_NPC:
		StateMessage("Closing npc store");
		g_dwShopVisited++;
		me->EndNpcSession();
		g_ticker.Update(STATE_AFTER_NPC, 1000);
		break;

	case STATE_AFTER_NPC:
		g_ticker.Update(STATE_RAND_TO_WP, Accelerate());
		break;

	case STATE_RAND_TO_WP:
		StateMessage("Walking to waypoint");		
		g_ticker.Update(STATE_NPC_TO_WP, AntiDetectMove(g_ptTownWp.x, g_ptTownWp.y));
		break;

	case STATE_NPC_TO_WP:
		StateMessage("Walking to waypoint");
		CalcPathAndGo(g_ptTownWp.x, g_ptTownWp.y);
		g_ticker.Update(STATE_WAIT_MOVE);
		g_ticker.StartWait(30000, STATE_LEAVE_TOWN, STATE_LEAVE_NPC);
		break;
		
	case STATE_LEAVE_TOWN:
		if (!server->FindUnitByClassID(TOWNWPS[g_shop.iAct], UNIT_TYPE_OBJECT, &unit))
		{
			g_ticker.Update(STATE_NPC_TO_WP, 1000);
			break;
		}

		UpdateWpName(&unit);		
		
		if (g_bReturn)
		{
			if (server->GetTownMap(g_shop.iAct) == g_iOriginMapID)
			{
				g_ticker.Update(STATE_BACK_TO_ORIGIN);
			}
			else
			{
				StateMessage("Entering waypoint");		
				me->EnterWaypoint(&unit, g_iOriginMapID);
				g_ticker.Update(STATE_WAIT_WP);
				g_ticker.StartWait(30000, STATE_BACK_TO_ORIGIN, STATE_LEAVE_TOWN, 1000);
			}			
		}
		else
		{
			StateMessage("Leaving town");		
			me->EnterWaypoint(&unit, g_iOutMapID);
			g_ticker.Update(STATE_WAIT_WP);
			g_ticker.StartWait(30000, STATE_BACK_TO_TOWN, STATE_LEAVE_TOWN, 2000);
		}		
		
		break;

	case STATE_BACK_TO_TOWN:
		StateMessage("Returning to town");
		server->FindUnitByName(g_szWpName, UNIT_TYPE_OBJECT, &unit);

		g_ticker.Update(STATE_WAIT_WP);
		if (g_bReturn)
		{
			if (!me->EnterWaypoint(&unit, g_iOriginMapID))
				g_ticker.Update(STATE_BACK_TO_TOWN, 1000);
			else
				g_ticker.StartWait(30000, STATE_BACK_TO_ORIGIN, STATE_BACK_TO_TOWN, 1000);
		}
		else
		{
			if (!me->EnterWaypoint(&unit, server->GetTownMap(g_shop.iAct)))
				g_ticker.Update(STATE_BACK_TO_TOWN, 1000);
			else
				g_ticker.StartWait(30000, STATE_RAND_TO_NPC, STATE_BACK_TO_TOWN, 1000);
		}	
		
		break;

	case STATE_BACK_TO_ORIGIN:
		StateMessage("Returning to original position");
		CalcPathAndGo(g_ptOrigin.x, g_ptOrigin.y);
		g_ticker.Update(STATE_WAIT_MOVE);
		g_ticker.StartWait(30000, STATE_EXECUTE_CALLBACK, STATE_BACK_TO_ORIGIN, 1000);
		break;

	case STATE_EXECUTE_CALLBACK:
		me->PauseJobs(FALSE);
		ExecuteCallback();
		g_ticker.Update(STATE_END);
		break;

	case STATE_END:
		g_bReturn = FALSE;
		g_dwTimeRemain = 0;
		g_ticker.Clear();
		server->GamePrintInfo("Shopbot stopped.");
		break;

	default:
		break;
	}

	return 0;
}

BOOL CALLBACK EnumSectionsProc(LPCTSTR lpSection, LPVOID lpParam)
{
	CIni* pIni = (CIni*)lpParam;
	if (pIni == NULL)
		return FALSE;

	if (::stricmp(lpSection, "Global") == 0)
		return TRUE;

	SBCONFIG cfg = { 0 };
	pIni->GetString(lpSection, "mod", cfg.szExp, 511);
	if (*cfg.szExp == 0)
		return TRUE;

	::strncpy(cfg.szSection, lpSection, 255);
	pIni->GetString(lpSection, "code", cfg.szCodes, 511);
	if (::strcmp(cfg.szCodes, "*") == 0)
		*cfg.szCodes = 0;
	cfg.bValid = TRUE;

	g_aConfig.Add(cfg);
	return TRUE;
}

BOOL UpdateWpName(LPCGAMEUNIT lpUnit)
{
	if (*g_szWpName)
		return TRUE;

	return server->GetUnitName(lpUnit, g_szWpName, 63);
}

void StateMessage(LPCSTR lpszMsg)
{
	if (lpszMsg && *lpszMsg)
		me->OverHead(lpszMsg, TRUE);
}

// randomize the path for anti-detection
DWORD AntiDetectMove(int x, int y)
{
	const long ANGLE = me->GetAngleFrom(x, y); 
	POINT pt = server->GetRandomPosition(6, 10, ANGLE - 45, ANGLE + 45, NULL);
	me->MoveTo(pt.x, pt.y, FALSE);
	return me->GetDistanceFrom(pt.x, pt.y) * 100;
}

BOOL ExamItem(DWORD dwItemID)
{
	BOOL bMatch = FALSE;
	for (int i = 0; i < g_aConfig.GetSize() && !bMatch; i++)
	{
		if (!g_aConfig[i].bValid)
			continue;

		BOOL bSyntaxError = FALSE;
		bMatch = server->ExamItemProperties(dwItemID, g_aConfig[i].szExp, g_aConfig[i].szCodes, &bSyntaxError);
		if (bSyntaxError)
		{
			g_aConfig[i].bValid = FALSE;
			server->GameErrorf("Syntax Error in [%s]!", g_aConfig[i].szSection);
			bMatch = FALSE;
		}
	}

	return bMatch;
}

BOOL AnnounceItem(DWORD dwItemID)
{
	WORD wPrefix = 0, wSuffix = 0;
	if (!server->GetItemPrefixSuffix(dwItemID, &wPrefix, &wSuffix))
		return FALSE;

	char szName[128] = "";
	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	server->GetUnitName(&gu, szName, 127);

	char szAnnounce[256] = "";
	if (wPrefix)
	{
		server->GetPrefixSuffixName(wPrefix, szAnnounce, 63);
		::strcat(szAnnounce, " ");
	}

	::strcat(szAnnounce, szName);

	if (wSuffix)
	{
		::strcat(szAnnounce, " ");
		server->GetPrefixSuffixName(wSuffix, szAnnounce + ::strlen(szAnnounce), 63);
	}

	server->GameInfof("ÿc3%s", szAnnounce);
	return TRUE;
}

BOOL CheckBoughtItem(DWORD dwItemID)
{
	// item bought successfully		
	g_aGoodItems.Lock();
	int nIdx = g_aGoodItems.Find(dwItemID);
	if (nIdx != -1)
	{
		g_dwItemBought++;
		g_aGoodItems.RemoveAt(nIdx);
	}
	g_aGoodItems.Unlock();

	return nIdx != -1;
}

int CalcPathAndGo(int x, int y)
{
	if (me->GetDistanceFrom(x, y) < 5)
	{
		me->MoveTo(x, y, TRUE);
		return 1;
	}

	POINT aPath[255];
	const int COUNT = server->CalculateWalkPath(x, y, aPath, 255);
	if (COUNT > 0)
	{
		for (int i = 1; i < COUNT; i++)
			me->MoveTo(aPath[i].x, aPath[i].y, TRUE);
	}
	return COUNT;
}

DWORD Accelerate()
{
	DWORD dwWait = 0;
	if (me->GetClassID() == 3 && me->HaveSpell(D2S_VIGOR) && !me->GetAffection(AFFECT_VIGOR))
	{
		// paladin & vigor
		me->SelectSpell(D2S_VIGOR, FALSE);
		dwWait = 300;
	}
	else if (me->GetClassID() == 6 && me->HaveSpell(D2S_BURSTOFSPEED) && !me->GetAffection(AFFECT_BURSTOFSPEED) && me->GetStat(STAT_MANA) > 10)
	{
		// assassin & burst of speed
		me->CastNoTarget(D2S_BURSTOFSPEED, FALSE);
		dwWait = 1000;
	}
	return dwWait;
}

void ExecuteCallback()
{
	// execute callback commands
	if (*g_szCallback == 0)
		return;

	const int LEN = ::strlen(g_szCallback);
	LPSTR psz = new char[LEN + 2];
	::strcpy(psz, g_szCallback);
	psz[LEN + 1] = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (psz[i] == ';')
			psz[i] = 0;
	}

	for (LPSTR p = psz; p && *p; p += ::strlen(p) + 1)
		server->GameCommandLine(p);

	delete [] psz;
}

BOOL GetTimeFormat(LPSTR lpszBuffer, BYTE& iColor)
{
	if (g_dwTimeRemain == -1 || lpszBuffer == NULL)
		return FALSE;
	
	// format a DWORD into hh:mm:ss
	DWORD dwSeconds = g_dwTimeRemain / 1000; // use seconds
	DWORD dwHours = dwSeconds / 3600;
	dwSeconds %= 3600;
	DWORD dwMinutes = dwSeconds / 60;
	dwSeconds %= 60;

	if (g_dwTimeRemain > 300 * 1000)
		iColor = 2;
	else if (g_dwTimeRemain > 60 * 1000)
		iColor = 8;
	else
		iColor = 1;

	::sprintf(lpszBuffer, "%02d : %02d : %02d", dwHours, dwMinutes, dwSeconds);
	return TRUE;
}

