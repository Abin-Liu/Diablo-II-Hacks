#include "countess.h"
#include "..\\includes\\ClientCore.cpp"

DWORD FindStairs()
{
	DWORD dwClassID = 0;
	::memset(&g_stairs, 0, sizeof(GAMEUNIT));

	switch (me->GetCurrentMapID())
	{
	case MAP_A1_BLACK_MARSH:

		dwClassID = 10;
		break;
		
	case MAP_A1_FORGOTTEN_TOWER:

		dwClassID = 12;
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_1:
	case MAP_A1_TOWER_CELLAR_LEVEL_2:
	case MAP_A1_TOWER_CELLAR_LEVEL_3:
	case MAP_A1_TOWER_CELLAR_LEVEL_4:

		dwClassID = 9;
		break;

	default:
		break;
	}

	if (!server->FindUnitByClassID(dwClassID, UNIT_TYPE_ROOMTILE, &g_stairs))
		return 0;

	return g_stairs.dwUnitID;
}

DWORD FindCountess()
{
	::memset(&g_countess, 0, sizeof(GAMEUNIT));
	//server->EnumUnits(UNIT_TYPE_MONSTER, EnumUnitProc, 0);
	server->FindUnitByName("the countess", UNIT_TYPE_MONSTER, &g_countess);
	return g_countess.dwUnitID;
}

BOOL CALLBACK EnumPresetUnitProc(LPCPRESETUNIT lpPresetUnit, LPARAM lParam)
{
	switch (me->GetCurrentMapID())
	{
	case MAP_A1_BLACK_MARSH:

		// look for tower
		if (lpPresetUnit->dwID == 10 && lpPresetUnit->dwType == UNIT_TYPE_ROOMTILE)
			::memcpy(&g_destination, lpPresetUnit, sizeof(PRESETUNIT));
		break;
		
	case MAP_A1_FORGOTTEN_TOWER:

		// look for stairs to lvl 1
		if (lpPresetUnit->dwID == 12 && lpPresetUnit->dwType == UNIT_TYPE_ROOMTILE)
			::memcpy(&g_destination, lpPresetUnit, sizeof(PRESETUNIT));
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_1:
	case MAP_A1_TOWER_CELLAR_LEVEL_2:
	case MAP_A1_TOWER_CELLAR_LEVEL_3:
	case MAP_A1_TOWER_CELLAR_LEVEL_4:

		// look for stairs to next level
		if (lpPresetUnit->dwID == 9 && lpPresetUnit->dwType == UNIT_TYPE_ROOMTILE)
			::memcpy(&g_destination, lpPresetUnit, sizeof(PRESETUNIT));
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_5:

		// look for countess chest
		if (lpPresetUnit->dwID == 580 && lpPresetUnit->dwType == UNIT_TYPE_OBJECT)
			::memcpy(&g_destination, lpPresetUnit, sizeof(PRESETUNIT));
		break;

	default:
		break;
	}

	return TRUE;
}

BOOL CALLBACK EnumUnitProc(LPCGAMEUNIT lpUnit, LPARAM lParam)
{
	if (server->GetMonsterType(lpUnit) == MONSTER_BOSS && server->GetUnitClassID(lpUnit) == 45)
	{
		::memcpy(&g_countess, lpUnit, sizeof(GAMEUNIT));
		return FALSE;
	}

	return TRUE;
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (g_ticker.GetState() == 0 || g_ticker.GetState() == STOP)
		return;

	if (nMessage == UM_CORPSE)
	{
		if (!::memcmp(lpUnit, &g_countess, sizeof(GAMEUNIT)) && g_ticker.GetState() == KILL_COUNTESS)
		{
			g_ticker.Update(COUNTESS_DEAD);
			server->GamePrintInfo("Hah! The bitch got raped.");
		}
	}
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{	
	if (g_ticker.GetState() == 0)
		return;
	
	if (nMessage == PM_MOVECOMPLETE && g_ticker.IsWaiting())
	{
		g_ticker.EndWait(wParam, 100);
	}
	else if (nMessage == PM_ENTERTOWN)
	{
		g_ticker.Update(STOP);
		if (lParam == MAP_A1_ROGUE_ENCAMPMENT)
		{
			// we've portaled back to rogue encampment
			// call next bot here...
		}		
	}
	else if (nMessage == PM_LEAVETOWN)
	{
		if (g_ticker.GetState() == CHECK_WP && lParam == MAP_A1_BLACK_MARSH)
		{
			// we've wp'ed to black marsh successfully
			g_ticker.Update(ENTER_BLACK_MARSH);
		}
	}
	else if (nMessage == PM_MAPCHANGE)
	{
		if (g_ticker.GetState() == ENTER_DEST && lParam >= MAP_A1_FORGOTTEN_TOWER && lParam <= MAP_A1_TOWER_CELLAR_LEVEL_5)
		{
			// we've entered the stairs successfully
			g_ticker.Update(FINDCAVE, 15000, 100);
		}
	}
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	ResetBot();
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* game)
{	
	ResetBot();
}

BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{		
	//GAMEUNIT gu = {0};
	//server->FindUnitByName("the countess", 1, &gu);
	//server->GameInfof("countess class %d", server->GetUnitClassID(&gu));
	//GAMEUNIT gu = {0};
	//server->FindUnitByClassID(0x0077, UNIT_TYPE_OBJECT, &gu);
	//me->EnterWaypoint(&gu, MAP_A1_BLACK_MARSH);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeReceived
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{	
	return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeSent
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}

DWORD EXPORT OnGameTimerTick(void)
{
	g_ticker.TickIt();
	if (g_ticker.IsWaiting())
		return 0;

	static int i = 0;
	switch(g_ticker.GetState())
	{
	case WAIT_TIMEOUT:
		Abort("Waiting state timed out");
		break;

	case GO_TO_WP:
		me->OverHead("Entering waypoint", TRUE);
		me->EnterWaypoint(&g_waypoint, MAP_A1_BLACK_MARSH);
		g_ticker.Update(CHECK_WP, 10000);			
		break;

	case CHECK_WP:
		
		if (g_ticker.IsTickerInPeriod(1000))
			me->OverHead("Entering waypoint", TRUE);
		if (g_ticker.IsTickEnded())
			Abort("Failed to enter waypoint");
		break;
		
	case ENTER_BLACK_MARSH:

		me->OverHead("Reached Black marsh", TRUE);
		if (g_ticker.IsTickEnded())
			g_ticker.Update(CAST_PROTECTION1, 5000, 100);
		break;
		
	case CAST_PROTECTION1:

		if (g_ticker.IsTickEnded() || !me->HaveSpell(D2S_SHIVERARMOR))
		{
			me->OverHead("Casting Shiver armor", TRUE);
			g_ticker.Update(CAST_PROTECTION2, 5000, 100);
			break;
		}

		if (g_ticker.IsTickerInPeriod(1000))
		{
			if (!me->GetAffection(AFFECT_SHIVERARMOR))
				me->CastNoTarget(D2S_SHIVERARMOR, FALSE);				
			else
				g_ticker.Update(CAST_PROTECTION2, 5000, 100);			
		}

		break;

	case CAST_PROTECTION2:

		if (g_ticker.IsTickEnded() || !me->HaveSpell(D2S_ENERGYSHIELD))
		{
			me->OverHead("Casting Energy shield", TRUE);
			g_ticker.Update(CAST_PROTECTION3, 5000, 100);
			break;
		}

		if (g_ticker.IsTickerInPeriod(1000))
		{
			if (!me->GetAffection(AFFECT_ENERGYSHIELD))
				me->CastNoTarget(D2S_ENERGYSHIELD, FALSE);				
			else
				g_ticker.Update(CAST_PROTECTION3, 5000, 100);			
		}

		break;
		
	case CAST_PROTECTION3:
		
		// we need to check cta for later
		g_ticker.Update(FINDCAVE, 15000, 100);
		
		break;

	case FINDCAVE:		
		
		if (g_ticker.IsTickerInPeriod(3000))
		{
			me->OverHead("Searching for destination", TRUE);
			::memset(&g_destination, 0, sizeof(PRESETUNIT));
			server->EnumPresetUnits(EnumPresetUnitProc, 0);

			if (g_destination.dwID)
			{
				g_ticker.Update(CALCPATH);
				break;
			}
		}
		
		else if (g_ticker.IsTickEnded())
			Abort("Cannot find preset unit");
		
		break;

	case CALCPATH:

		me->OverHead("Calculating path", TRUE);
		g_dwPathCount = server->CalculateTeleportPath(g_destination.x, g_destination.y, g_path, 255);

		if (g_dwPathCount< 2)
			Abort("Failed to calculate path");
		else
			g_ticker.Update(TELEPORT_TO_DEST);
		break;

	case TELEPORT_TO_DEST:

		me->OverHead("Teleporting to destination", TRUE);
		for (i = 1; i < (int)g_dwPathCount; i++)
			me->TeleportTo(g_path[i].x, g_path[i].y, TRUE);

		if (me->GetCurrentMapID() == MAP_A1_TOWER_CELLAR_LEVEL_5)
			g_ticker.StartWait(30000, FIND_COUNTESS, WAIT_TIME_OUT);
		else
			g_ticker.StartWait(30000, FIND_STAIRS, WAIT_TIME_OUT);
		break;

	case FIND_STAIRS:

		me->OverHead("Searching for entrance", TRUE);
		if (!FindStairs())
			Abort("Cannot find entrance");
		else
			g_ticker.Update(ENTER_DEST, 15000, 100);
		break;
		
	case ENTER_DEST:
		
		me->OverHead("Entering entrance", TRUE);
		if (g_ticker.IsTickerInPeriod(5000))
			me->Interact(&g_stairs); // wait for PM_MAPCHANGE
		else if (g_ticker.IsTickEnded())
			Abort("Failed to enter entrance");

		break;

	case FIND_COUNTESS:

		me->OverHead("Searching for the Countess", TRUE);
		if (!FindCountess())
			Abort("Cannot find the Countess");
		else
			g_ticker.Update(TELE_TO_COUNTESS);
		break;

	case TELE_TO_COUNTESS:
		me->OverHead("Approaching the Countess", TRUE);
		me->CastOnUnit(D2S_TELEPORT, &g_countess, FALSE);
		g_ticker.Update(KILL_COUNTESS, 37000, 100);
		break;

	case KILL_COUNTESS:

		me->OverHead("Killing the Countess", TRUE);
		if (g_ticker.IsTickerInPeriod(1200))
			me->CastOnUnit(D2S_CHAINLIGHTNING, &g_countess, FALSE);
		else if (g_ticker.IsTickEnded())
			Abort("Failed to kill the Countess");
		break;

	case COUNTESS_DEAD:

		if (me->HaveSpell(D2S_FINDITEM))
		{
			me->OverHead("Using Find item", TRUE);
			me->CastOnUnit(D2S_FINDITEM, &g_countess, FALSE);
		}

		server->GameCommandLine("pickit enable");
		server->GameCommandLine("pickit radius 15");
		g_ticker.Update(PICK_UP_ITEMS, 10000, 100);
		break;

	case PICK_UP_ITEMS:
		if (g_ticker.IsTickEnded())
			Abort("Countess bot finished successfully", FALSE);

		if (g_ticker.IsTickerInPeriod(1000))
		{
			me->OverHead("Picking up items", TRUE);
			if (!FindValuableItems())
				Abort("Countess bot finished successfully", FALSE);
		}

		break;

	default:
		break;
	}

	return 0;
}

BOOL PRIVATE OnCommandStart(char **argv, int argc)
{
	// to start countess bot, the following conditions must be all true:
	// 1, bot must not be started already
	// 2, game must be ready
	// 3, player must be at act 1 town
	// 4, player must be no farther than 10 yards, to the town waypoint

	if (g_ticker.GetState() != STOP)
	{
		server->GamePrintError("Countess bot already started!");
		return TRUE;
	}

	if (!server->IsGameReady())
	{
		server->GamePrintError("Game is not ready!");
		return TRUE;
	}

	if (me->GetCurrentMapID() != MAP_A1_ROGUE_ENCAMPMENT)
	{
		server->GamePrintError("Player is not at Rogue Encampment!");
		return TRUE;
	}

	if (!server->FindUnitByClassID(0x0077, UNIT_TYPE_OBJECT, &g_waypoint)) // act1 wp class id 0x0077
	{
		server->GamePrintError("Act1 waypoint not found!");
		return TRUE;
	}
	
	POINT mp = server->GetUnitPosition(&g_waypoint);
	if (me->GetDistanceFrom(mp.x, mp.y) > 10)
	{
		server->GamePrintError("Waypoint is more than 10 yards away!");
		return TRUE;
	}

	// everything is OK, we can start now
	g_ticker.Update(GO_TO_WP);
	server->GamePrintInfo("Countess bot started.");
	return TRUE;
}

BOOL PRIVATE OnCommandStop(char **argv, int argc)
{
	ResetBot();
	server->GamePrintInfo("Countess bot stoped.");
	return TRUE;
}

BOOL PRIVATE OnCommandPause(char **argv, int argc)
{
	g_ticker.Pause();
	server->GamePrintInfo("Countess bot paused.");
	return TRUE;
}

BOOL PRIVATE OnCommandResume(char **argv, int argc)
{
	g_ticker.Resume();
	server->GamePrintInfo("Countess bot resumed.");
	return TRUE;
}

void ResetBot()
{
	g_ticker.Clear();
	::memset(&g_waypoint, 0, sizeof(GAMEUNIT));
	::memset(&g_countess, 0, sizeof(GAMEUNIT));
	::memset(&g_stairs, 0, sizeof(GAMEUNIT));	
	::memset(&g_destination, 0, sizeof(PRESETUNIT));
	g_dwPathCount = 0;
}

BOOL FindValuableItems()
{
	DWORD dwVal = 0;
	server->QueryModuleData("pickit", 1, &dwVal, 4, 0);
	server->GameInfof("left %d", dwVal);
	return dwVal;
}

void Abort(LPCSTR lpszReason, BOOL bIsFailed)
{
	if (bIsFailed)
		server->GameErrorf(lpszReason);
	else
		server->GameInfof(lpszReason);

	me->OverHead(" ", TRUE);
	ResetBot();
	me->BackToTown();
}
