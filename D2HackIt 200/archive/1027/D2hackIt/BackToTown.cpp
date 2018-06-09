///////////////////////////////////////////////////////////
// BackToTown.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "BackToTown.h"
#include "Utilities\\StateTicker.h"
#include "Inventory.h"
#include "D2Hackit.h"
#include "d2functions.h"
#include "me.h"
#include "GameMessage.h"

#define TP_CAST_TICK		1000	// Town portal casting checking interval
#define TP_ENTER_TICK		500		// Town portal interaction checking interval
#define MAX_PORTAL_FAIL		5

enum {	TS_NONE = 0,			// nothing
		TS_CAST_PORTAL,			// open a town portal
		TS_WAIT_PORTAL,			// wait for portal to appear
		TS_VERIFY_PORTAL,		// Verify portal mode, thanks to Mousepad & Ninjai
		TS_ENTER_PORTAL,		// enter the town portal
		TS_VERIFY_INTOWN };		// verify in town

int g_nTPRetry = 0;
CStateTicker g_tpTicker;
GAMEUNIT g_portal = {0};

BOOL CALLBACK FindPortalProc(LPCGAMEUNIT lpUnit, LPARAM lParam);
DWORD FindMyPortal();

BOOL EXPORT BackToTown() // Cast a town portal and go back to town immediately
{
	if (IsInTown())
	{
		GamePrintInfo("You are in town.");
		return TRUE;
	}

	if (g_tpTicker.GetState() != TS_NONE)
	{
		GamePrintError("Already in a portal Process");
		return FALSE; // already in middle of a tp process
	}

	if (!GetInventoryTPScrolls() && !GetInventoryTPBookCharges())
	{
		GamePrintError("�c8Out of inventory town portal charges!");
		return FALSE;
	}

	CleanJobs(); // Action "Back to town" has higher priority than any other jobs
	g_nTPRetry = 0;
	g_tpTicker.Update(TS_CAST_PORTAL, 0);
	return TRUE;
}

void CleanBackToTownStates()
{	
	g_tpTicker.Clear();
	g_nTPRetry = 0;
	::memset(&g_portal, 0, sizeof(GAMEUNIT));
}

BOOL IsBackToTownInProcess()
{
	return g_tpTicker.GetState();
}

BOOL CALLBACK FindPortalProc(LPCGAMEUNIT lpUnit, LPARAM lParam)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, UNIT_TYPE_OBJECT);
	if (p 
		&& p->dwClassId == 0x3b // town portal
		&& p->ptObjectData
		&& !::stricmp(p->ptObjectData->parent, GetSafePlayerName()))
	{
		::memcpy(&g_portal, lpUnit, sizeof(GAMEUNIT));		
		return FALSE;
	}

	return TRUE;
}

DWORD FindMyPortal()
{
	g_portal.dwUnitID = 0;
	g_portal.dwUnitType = UNIT_TYPE_OBJECT;
	EnumUnits(UNIT_TYPE_OBJECT, FindPortalProc);
	return g_portal.dwUnitID;
}

BOOL ProcessBackToTown(const volatile BYTE* lpStopFlag)
{
	if (g_tpTicker.GetState() == 0)
		return FALSE;

	if (IsInTown())
	{
		// already in town
		g_tpTicker.Clear();
		return FALSE;
	}

	g_tpTicker.TickIt(100);
	if (!g_tpTicker.IsTickEnded())
		return TRUE;

	WORD wSpell = 0;
	UnitAny* pUnit = NULL;

	switch (g_tpTicker.GetState())
	{
	case TS_CAST_PORTAL:

		// use scroll first
		if (GetInventoryTPScrolls())
			wSpell = D2S_SCROLLOFTOWNPORTAL;
		else if (GetInventoryTPBookCharges())
			wSpell = D2S_TOMEOFTOWNPORTAL;

		if (wSpell == 0)
		{
			GamePrintError("�c8Out of inventory town portal charges!");
			g_tpTicker.Clear();
			QueueThisPlayerMessage(PM_TOWNFAIL);
			return FALSE;
		}

		// cast it
		::memset(&g_portal, 0, sizeof(GAMEUNIT));
		CastNoTarget(wSpell, FALSE);
		g_tpTicker.Update(TS_WAIT_PORTAL, TP_CAST_TICK);		
		break;

	case TS_WAIT_PORTAL:

		// portal didn't appear
		if (g_nTPRetry >= MAX_PORTAL_FAIL)
		{
			// maximum retries reached and portal still did not appear
			g_tpTicker.Clear();
			GamePrintError("�c8Failed to cast town portal.");
			QueueThisPlayerMessage(PM_TOWNFAIL);
			return FALSE;
		}
		else
		{
			if (FindMyPortal())
			{
				g_nTPRetry = 0;
				g_tpTicker.Update(TS_ENTER_PORTAL, 0);
			}
			else
			{
				// retry and prompt every seconds
				g_nTPRetry++;
				GamePrintInfo("Trying to cast town portal...");
				g_tpTicker.Update(TS_CAST_PORTAL, 0);
			}			
		}
		break;

		/*
	case TS_VERIFY_PORTAL:
		pUnit = (UnitAny*)VerifyUnit(&g_myPortal);
		if (pUnit && pUnit->dwMode == 2) // mode 2 indicates portal clickable (Thanks Ninjai)
			g_townState.Update(TS_ENTER_PORTAL, 0);
		break;
		*/

	case TS_ENTER_PORTAL:
		// Walk in.
		Interact(&g_portal);
		g_tpTicker.Update(TS_VERIFY_INTOWN, TP_ENTER_TICK);
		break;

	case TS_VERIFY_INTOWN:
		if (g_nTPRetry >= MAX_PORTAL_FAIL * 2)
		{
			GamePrintError("Failed to enter TP");
			g_tpTicker.Clear();
			QueueThisPlayerMessage(PM_TOWNFAIL);
			return FALSE;
		}
		else
		{
			g_nTPRetry++;
			g_tpTicker.Update(TS_ENTER_PORTAL, 0);
			if (g_nTPRetry % 2)
				GamePrintInfo("Trying to enter town portal...");
		}

		break;

	default:
		break;
	}

	return TRUE;
}