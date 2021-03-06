///////////////////////////////////////////////////////////
// EnterWayPoint.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "EnterWayPoint.h"
#include "Utilities\\StateTicker.h"
#include "CommonStructs.h"
#include "GameMessage.h"
#include "D2HackIt.h"
#include "MapDef.h"
#include "d2functions.h"
#include "D2Stdio.h"

// wp state
enum { WP_NONE = 0, WP_CLEAN, WP_INTERACT, WP_WAITUI, WP_ENTER, WP_VERIFYDEST, WP_FINISH };
#define MAX_WP_RETRY	5

int g_nWpRetry = 0;
CStateTicker g_wpTicker;
GAMEUNIT g_waypoint = { 0 };
BYTE g_iWPdest = 0;
BOOL g_bWpOpened = FALSE;

BOOL IsWaypointAvailable(BYTE iMapID);
BOOL IsWaypoint(BYTE iMapID);
void FinishWPProc(BOOL bSucceeded);

BOOL EXPORT EnterWaypoint(LPCGAMEUNIT lpWpUnit, BYTE iDestinationMapID) // take WP to another area
{
	if (g_wpTicker.GetState())
	{
		ServerErrorf("Already in a waypoint process.");
		return FALSE;
	}

	if (lpWpUnit == NULL || !VerifyUnit(lpWpUnit))
	{
		UnitError(lpWpUnit, "Waypoint");
		return FALSE;
	}

	if (GetCurrentMapID() == iDestinationMapID)
	{
		ServerErrorf("Player already on map %02X", iDestinationMapID);
		return FALSE;
	}
	
	if (!IsWaypoint(iDestinationMapID))
	{
		ServerErrorf("There is no waypoint on map %02X", iDestinationMapID);
		return FALSE;
	}

	POINT mp = GetUnitPosition(lpWpUnit);
	DWORD dwDistance = GetDistanceFrom(mp.x, mp.y);
	if (dwDistance > 50)
	{
		PositionError(&mp, "Waypoint");
		return FALSE;	
	}

	g_iWPdest = iDestinationMapID;
	::memcpy(&g_waypoint, lpWpUnit, sizeof(GAMEUNIT));

	if (dwDistance > 5)
	{
		MoveToUnit(lpWpUnit, FALSE);
		g_wpTicker.Update(WP_CLEAN, dwDistance * 50);
	}
	else
	{
		g_wpTicker.Update(WP_CLEAN, 0);
	}
	return TRUE;
}

void CleanEnterWayPointStats()
{
	g_wpTicker.Clear();
	g_nWpRetry = 0;
	g_iWPdest = 0;
	::memset(&g_waypoint, 0, sizeof(GAMEUNIT));
	g_bWpOpened = IsUIOpened(UI_WPMENU);
}

BOOL ProcessEnterWayPoint(const volatile BYTE* lpStopFlag)
{
	if (g_wpTicker.GetState() == 0)
		return FALSE;

	g_wpTicker.TickIt(100);
	if (!g_wpTicker.IsTickEnded())
		return TRUE;

	switch (g_wpTicker.GetState())
	{
	case WP_CLEAN:
		if (g_bWpOpened)
		{
			CloseAllUIs();
			g_wpTicker.Update(WP_INTERACT, 1000);
		}
		else
		{
			g_wpTicker.Update(WP_INTERACT, 0);
		}
		break;

	case WP_INTERACT:
		if (g_bWpOpened)
		{
			g_wpTicker.Update(WP_ENTER, 0);
			break;
		}

		if (!VerifyUnit(&g_waypoint))
		{
			FinishWPProc(FALSE);
		}
		else
		{
			Interact(&g_waypoint);
			g_wpTicker.Update(WP_WAITUI, 1000);	
		}
		break;

	case WP_WAITUI:
		if (g_bWpOpened)
		{			
			g_wpTicker.Update(WP_ENTER, 0);
			/*
			if (GetActFromMap(g_iWPdest) != GetActFromMap(g_iCurMap))
				g_nWPState = WP_CLICKTAB;
			else
				g_nWPState = WP_CLICKBTN;
				*/
		}
		else
		{
			g_nWpRetry++;
			if (g_nWpRetry > MAX_WP_RETRY)
				FinishWPProc(FALSE);
			else
				g_wpTicker.Update(WP_CLEAN, 0);
		}
		
		break;
	
	case WP_ENTER:
		if (g_bWpOpened)
		{
			if (!IsWaypointAvailable(g_iWPdest))
			{
				ServerErrorf("Player does not have waypoint 0x%02X.", g_iWPdest);
				FinishWPProc(FALSE);
				break;
			}
			
			BYTE aPacket[9] = { 0 };
			aPacket[0] = 0x49;
			::memcpy(&aPacket[1], &g_waypoint.dwUnitID, 4);
			aPacket[5] = g_iWPdest;
			GameSendPacketToServer(aPacket, 9);
			g_nWpRetry = 0;
			g_wpTicker.Update(WP_VERIFYDEST, 10000);
		}
		break;

		/*
	case WP_CLICKTAB:
		if (!IsUIOpened(UI_WPMENU))
		{
			g_nWPState = WP_INTERACT;
			break;
		}

		if (!ClickWPTab(g_iWPdest))
		{
			FinishWPProc(FALSE);
		}
		else
		{
			g_ticker.Update(500);
			g_nWPState = WP_CLICKBTN;
			g_nWPRetries = 0;
		}
		break;

	case WP_CLICKBTN:
		if (!IsUIOpened(UI_WPMENU))
		{
			g_nWPState = WP_INTERACT;
			break;
		}
		
		if (!ClickWpButton(g_iWPdest))
		{
			FinishWPProc(FALSE);
		}
		else
		{	
			g_nWPState = WP_VERIFYDEST;
			g_nWPRetries = 0;
			g_ticker.Update(2000);
		}
		break;
		*/

	case WP_VERIFYDEST:
		FinishWPProc(FALSE);
		/*
		else
		{
			g_nWPRetries++;
			if (g_nWPRetries > MAX_RETRY)
			{
				FinishWPProc(FALSE);
			}
			else
			{
				if (!IsUIOpened(UI_WPMENU))
				{
					g_nWPState = WP_INTERACT;
					break;
				}
		
				if (!ClickWpButton(g_iWPdest))
				{
					FinishWPProc(FALSE);
				}
				else
				{		
					g_ticker.Update(2000);
				}
			}
		}
		*/
		
		break;

	case WP_FINISH:
		FinishWPProc(TRUE);
		break;

	default:
		break;
	}

	return TRUE;
}

void FinishWPProc(BOOL bSucceeded)
{
	g_waypoint.dwUnitID = 0;
	g_iWPdest = 0;
	g_wpTicker.Clear();
	g_nWpRetry = 0;

	if (!bSucceeded)
	{
		if (g_bWpOpened)
		{
			CloseAllUIs();
			Sleep(300);
		}

		ServerErrorf("Failed to enter waypoint.");
		SendThisPlayerMessage(PM_WPFAIL);
	}
}

void OnWpOpen(BOOL bOpened)
{
	g_bWpOpened = bOpened;
}

BOOL IsWpOpened()
{
	return g_bWpOpened;
}

void OnMapChangeCheckWayPoint(BYTE iMapID)
{
	// was in a wp proc?
	if (iMapID == g_iWPdest && g_wpTicker.GetState() == WP_VERIFYDEST)
	{
		// the "EnterWaypoint" process succeeded
		CloseAllUIs();		
		g_wpTicker.Update(WP_FINISH, 1000);
	}
}

BOOL IsWaypointAvailable(BYTE iMapID)
{
	// Important: This function ONLY works if the player has activated ANY wp object in
	// current game at least once, otherwise it always returns 0, even if the player has 
	// the waypoint.
	WPINFO wp;
	if (!D2GetWPInfo(iMapID, &wp))
		return FALSE;

	int nWpNum = (wp.iAct + 1) * 9 + wp.nPos - 9; // This formula is from d2bs
	if (wp.iAct > 3)
		nWpNum -= 6;

	return D2COMMON_GetCheckWp(*p_D2CLIENT_WpTable, nWpNum);
}

BOOL IsWaypoint(BYTE iMapID)
{
	WPINFO wp;
	return D2GetWPInfo(iMapID, &wp);
}
