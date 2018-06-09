//////////////////////////////////////////////////////////////////////
// ShopBot.CPP
//
// Copyright (c) 2004, Abin (Bin Liu)
// All rights reserved.
//
//////////////////////////////////////////////////////////////////////
#include "ShopBot.h"
#pragma comment(lib, "..\\D2lib\\D2Lib.lib") // Import D2Lib
#include "..\\D2Lib\\ClientCore.cpp"

// CD2Player Message Handler
void CALLBACK OnPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{	
	if (nMessage == PM_GAMEREADY) // Game is ready for operations
	{
		if ((g_global.bAutoStart || g_bStartAfterGameReady) && VerifyConditions())
			StartShopping();

		return;
	}

	// All other messages are need to be processed only if shopbot had been started
	if (g_nGameState == GS_NONE || g_nGameState == GS_READYSTOP)
		return;	

	if (nMessage == OM_AFFECT) // Player affection
	{
		if (lParam == STATE_QUICKNESS) // burst of speed
			g_bBoSOn = TRUE;
		return;
	}

	if (nMessage == OM_DISAFFECT) // Player disaffection
	{
		if (lParam == STATE_QUICKNESS) // burst of speed
			g_bBoSOn = FALSE;
		return;
	}

	if (nMessage == PM_MOVECOMPLETE) // running completed
	{
		g_nRetry = 0;
		UpdateGameState(g_nNextState);
		return;
	}

	if (nMessage == PM_NPCEND) // NPC interaction completed		
	{
		if (wParam)
		{
			g_nRetry = 0;
			UpdateGameState(g_nNextState);
		}

		g_ticker.ResetTicker();
		return;
	}	

	if (nMessage == PM_WPOPEN) // WP opened successfully
	{
		g_nRetry = 0;
		if (g_nGameState == GS_WAITOPENWP)
			UpdateGameState(GS_USEWP);
		return;
	}

	if (nMessage == PM_ACTCHANGE) // Act changed
	{
		if (g_bNeedStop)
		{
			Me.OverHead("ÿc4Checking game objects...");
			SigPostString("Checking game objects...");
			UpdateGameState(GS_WPTOSTART, 5000);
		}
		else if (g_bNeedChangeAct)
		{
			g_bNeedChangeAct = FALSE;
			Me.OverHead("ÿc4Checking game objects...");
			SigPostString("Checking game objects...");
			UpdateGameState(GS_ACTSWAPOK, 5000);
		}

		return;
	}

	if (nMessage == PM_LEAVETOWN) // Player left town		
	{
		g_nRetry = 0;
		Me.OverHead("ÿc4Checking game objects...");
		SigPostString("Checking game objects...");
		UpdateGameState(GS_OPENWP, 1000);
		//UpdateGameState(0, 1000);
		//Me.SendChar(VK_ESCAPE);
		return;
	}

	if (nMessage == PM_ENTERTOWN) // Player entered town		
	{	g_nRetry = 0;
		Me.OverHead("ÿc4Checking game objects...");
		SigPostString("Checking game objects...");
		g_dwTownCheckCount = 0;
		UpdateGameState(GS_WAITINTOWN);
		return;
	}

	if (nMessage == PM_HPDOWN) // Player life down
	{
		if (Me.GetHPPercent() < g_global.iStopHP)
			Me.LeaveGame();
		return;
	}

	if (nMessage == PM_HOSTILE) // Player is hostiled
	{
		Me.LeaveGame();
		return;
	}
}

// Hotkey Message Handler
BOOL CALLBACK OnHotkey(BYTE iVkCode, LPCTSTR lpszCommand)
{
	if (lpszCommand) // Hotkey message
		server->GameCommandLine((char*)lpszCommand);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStart
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{		
	LoadLogData(); // Log data
	LoadGlobalData(); // Global data
	LoadConfigData(); // Item config data
	InitializeObjects(); // Objects initialization
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStop
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStop()
{
	g_hotkey.UninstallKeyBind(); // uninstall the keyboard hook
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameJoin
//////////////////////////////////////////////////////////////////////
void EXPORT OnGameJoin(THISGAMESTRUCT* game)
{	
	g_hotkey.OnGameJoin();
	Me.OnGameJoin(game);
	Npc.OnGameJoin(game);
	Town.OnGameJoin(game);
	g_outWP.OnGameJoin(game);
	
	server->GamePrintInfo("ÿc4Shopbot 1.10 by Abin");

	char szConfig[MAX_PATH + 1] = "";	
	sprintf(szConfig, "%s\\sig.dll", GetModuleDirectory()); // is sig.dll there?
	g_bUseSig = ::GetFileAttributes(szConfig) != -1; 

	g_bBoSOn = FALSE;

	if (Me.HasSpell(D2S_VIGOR))
		g_wQuicknessSpell = D2S_VIGOR;
	else if (Me.HasSpell(D2S_BURSTOFSPEED))
		g_wQuicknessSpell = D2S_BURSTOFSPEED;
	else
		g_wQuicknessSpell = D2S_INVALID;
	
	if (g_global.bAutoStart)
		OnGameCommandStart(NULL, 0); // auto start
}

//////////////////////////////////////////////////////////////////////
// OnGameLeave
//////////////////////////////////////////////////////////////////////
void EXPORT OnGameLeave(THISGAMESTRUCT* game)
{	
	g_hotkey.OnGameLeave();
	Me.OnGameLeave();
	Npc.OnGameLeave();
	Town.OnGameLeave();
	g_outWP.OnGameLeave();
	g_text.Undraw();

	g_bStartAfterGameReady = FALSE;
	g_bAutoStop = FALSE;
	g_dwRemainMS = 0;
	g_bPaused = FALSE;
	g_dwTotalTime = 0;
	g_nGameState = 0;
	g_nNextState = 0;
	g_nRetry = 0;
	g_bBoSOn = FALSE;
	g_bNeedStop = FALSE;

	g_aStartToWP.RemoveAll();
	g_aWPToNpc.RemoveAll();
	g_aWPToHealer.RemoveAll();
	g_ticker.ResetTicker();
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeReceived
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{	
	// Just let D2Lib process the packets
	Me.OnGamePacketBeforeReceived(aPacket, aLen);
	Npc.OnGamePacketBeforeReceived(aPacket, aLen);
	Town.OnGamePacketBeforeReceived(aPacket, aLen);
	g_outWP.OnGamePacketBeforeReceived(aPacket, aLen);

	// item to npc store
	if (!g_bPaused && aPacket[0] == 0x9c && aPacket[1] == 0x0B)
	{
		// by receiving this message we have opened npc store successfully
		g_dwItemChecked++;
		g_dwTotalExamined++;
		char szMessage[256] = "";
		sprintf(szMessage, "ÿc4Items examined: ÿc2%u", g_dwItemChecked);
		Me.OverHead(szMessage);	
		UpdateScreenText();

		UpdateGameState(GS_STOREOK, 10000); // halt the timer first
		ITEM item = {0};
		D2ParseItem(aPacket, aLen, item); // Parse item data from packet
		for (int i = 0; i < g_aConfig.GetSize(); i++)
		{
			if (SBVerifyItemMods(&item, &g_aConfig[i])) // Compare item data with configuration
			{
				// Good item found!
				g_aGoodItems.InsertRear(item);
				D2Infof("%s", g_aConfig[i].szDescription);
				SigPostString(g_aConfig[i].szDescription);
				g_log.IncreaseUInt("Items", g_aConfig[i].szDescription);
			}
		}
		UpdateGameState(GS_STOREOK, 2000); // resume the timer		
	}

	// item dropped to equip or inventory, means the item is bought successfully
	if ((aPacket[0] == 0x9c && aPacket[1] == 0x04) || (aPacket[0] == 0x9d && aPacket[1] == 0x06))
	{
		ITEM item = { 0 };
		D2ParseItem(aPacket, aLen, item); // parse item info

		POSITION pos = g_aGoodItems.GetHeadPosition();
		while (pos)
		{
			// check the item
			POSITION temp = pos;
			const ITEM& goodItem = g_aGoodItems.GetNext(pos);
			if (!::memcmp(goodItem.aMods, item.aMods, sizeof(item.aMods)))
			{
				// Bought an item
				g_aGoodItems.RemoveAt(temp);
				server->GamePrintInfo("Item bought successfully!");
				g_log.IncreaseUInt("Log", "Bought");
				g_dwTotalBought++;
				UpdateScreenText();
			}
		}
	}

    return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeSent
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	// Just let D2Lib process the packets
	Me.OnGamePacketBeforeSent(aPacket, aLen);
	Npc.OnGamePacketBeforeSent(aPacket, aLen);
	Town.OnGamePacketBeforeSent(aPacket, aLen);
	g_outWP.OnGamePacketBeforeSent(aPacket, aLen);

	return aLen;
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
	// Let D2Lib process the timer first
	Me.OnGameTimerTick();
	Npc.OnGameTimerTick();
	Town.OnGameTimerTick();
	g_outWP.OnGameTimerTick();
	
	if (!Me.IsGameJoined() || !Me.IsGameReady())
		return 0; // don't do shit if the game isn't even ready	

	g_dwTotalTime += 100; // record total game length

	if (g_bAutoStop && g_nGameState)
	{
		// auto stop enabled, check if time is epalsed
		if (g_dwRemainMS > 0)
		{
			// not elapsed yet
			if (g_dwRemainMS >= 100)
				g_dwRemainMS -= 100;
			else
				g_dwRemainMS = 0;

			if ((g_dwRemainMS % 1000) == 0)
				UpdateScreenText();
		}
		else
		{
			// time elapsed, stop shopping now
			if (!g_bNeedStop)
				OnGameCommandStop(NULL, 0);
		}
	}

	g_ticker.TickIt(100);

	if (g_bPaused || !g_ticker.IsTickEnded())
		return 0; // shopbot paused or ticker not yet ended

	// some temporary variables
	int i = 0;
	POSITION pos = NULL;
	char szMessage[256] = "";
	
	// Exam game states and perform actions accordingly
	switch (g_nGameState)
	{
	case GS_WAITMOVE:
		// move failed if reach here, what we can do? retry
		break;

	case GS_WAITNPC:
		// npc interaction failed if reach here
		if (g_nRetry < MAX_RETRY)
		{
			g_nRetry++;
			UpdateGameState(GS_TALKNPC);
		}
		else
		{
			UpdateGameState(GS_WPTONPC);
			//FailGame();
		}
		break;

	case GS_CALCSTARTTOWPPATH:		

		Me.OverHead("ÿc4Calculating path");
		CalculateStartToWPPath();
		SigPostString("Calculating path");
		
		// boost speed first
		UseQuicknessSpell();

		UpdateGameState(GS_STARTTOWP, 1000);
		break;

	case GS_STARTTOWP:

		// run from start position to wp
		Me.OverHead("ÿc4Running to waypoint");
		SigPostString("Running to waypoint");
		for (i = 0; i < g_aStartToWP.GetSize(); i++)
			Me.RunTo(g_aStartToWP[i].x, g_aStartToWP[i].y);

		WaitForMove(GS_FIRSTWPREACHED);
		break;

	case GS_FIRSTWPREACHED:

		// this is the first time we reach the wp since current running
		// it's a good place to do some initial decisions
		Me.OverHead("ÿc4Reached WP first time");
		SigPostString("Reached WP first time");
		if (g_global.iAct != Me.GetCurrentAct())
		{
			// need to change act
			g_bNeedChangeAct = TRUE;
			Me.OverHead("ÿc4Need to change act");
			SigPostString("Need to change act");
			UpdateGameState(GS_OPENWP);
		}
		else
		{
			// no need to change act, calculate the npc path and start shopping
			g_bNeedChangeAct = FALSE;
			Me.OverHead("ÿc4Recalculating path");
			SigPostString("Recalculating path");
			CalculateWPToNpcPath();
			UpdateGameState(GS_WPTONPC, 1000);
		}
		break;

	case GS_ACTSWAPOK:
		// act changed ok, calculate the npc path and start shopping
		Me.CloseWP(Town.GetCurrentTownWP());
		CalculateWPToNpcPath();
		UpdateGameState(GS_WAITINTOWN, 1000);
		break;	

	case GS_WAITINTOWN:
		
		// just entered town, need to wait for map objects update
		if (g_dwTownCheckCount < 50
			&& Town.GetCurrentTownWP()
			&& Me.GetDistance(Town.GetCurrentTownWP()->GetPositionX(), Town.GetCurrentTownWP()->GetPositionY()) > 20)
		{
			Me.OverHead("ÿc4Waiting for location ÿc4update...");
			SigPostString("Waiting for location update...");
			g_dwTownCheckCount++; // preventing state starvation
		}
		else
		{
			UpdateGameState(GS_WPTONPC);
		}
		
		break;

	case GS_WPTONPC:

		// run to the chosen npc
		Me.CloseWP(Town.GetCurrentTownWP());

		if (g_bNeedStop)
		{
			if (g_iEndMap != Me.GetCurrentMapID())
				UpdateGameState(GS_OPENWP);
			else
				UpdateGameState(GS_WPTOSTART);
			break;
		}
		
		g_bWasHealing = IsNeedHeal(); // need to visit a healer first?
		if (g_bWasHealing)
		{
			Me.OverHead("ÿc1HP<95% ÿc4Running to healer");
			SigPostString("HP<95% Running to healer");
			for (i = 0; i < g_aWPToHealer.GetSize(); i++)
				Me.RunTo(g_aWPToHealer[i].x, g_aWPToHealer[i].y);
		}
		else
		{
			sprintf(szMessage, "ÿc4Running to ÿc2%s", g_szNpcName);
			Me.OverHead(szMessage);
			sprintf(szMessage, "Running to %s", g_szNpcName);
			SigPostString(szMessage);
				
			for (i = 0; i < g_aWPToNpc.GetSize(); i++)
				Me.RunTo(g_aWPToNpc[i].x, g_aWPToNpc[i].y);
		}
		
		WaitForMove(GS_BOOSTSPEED);
		break;

	case GS_BOOSTSPEED:
		// cast speed boosting spells, if applicable
		if (UseQuicknessSpell() == D2S_BURSTOFSPEED)
			UpdateGameState(GS_TALKNPC, 1000);
		else
			UpdateGameState(GS_TALKNPC);	
				
		break;

	case GS_TALKNPC:

		// talk to the chosen npc/healer
		if (g_bNeedStop)
		{
			UpdateGameState(GS_CLOSESTORE);
			break;
		}
		
		if (g_bWasHealing)
		{
			Me.OverHead("ÿc4Talking to healer");
			SigPostString("Talking to healer");
		}
		else
		{
			sprintf(szMessage, "ÿc4Talking to ÿc2%s", g_szNpcName);
			Me.OverHead(szMessage);
			sprintf(szMessage, "Talking to %s", g_szNpcName);
			SigPostString(szMessage);
		}		
		
		Me.LeaveNpc(g_bWasHealing ? Town.GetCurrentTownHealer() : &Npc);
		Me.TalkToNpc(g_bWasHealing ? Town.GetCurrentTownHealer() : &Npc);
		WaitForNpc(GS_OPENSTORE);
		break;

	case GS_OPENSTORE:

		if (g_bNeedStop)
		{
			UpdateGameState(GS_CLOSESTORE);
			break;
		}

		// try to open the npc store
		Me.OverHead("ÿc4Opening NPC store");
		SigPostString("Opening NPC store");
		g_dwItemChecked = 0;
		Me.OpenNpcStore(g_bWasHealing ? Town.GetCurrentTownHealer() : &Npc);
		UpdateGameState(GS_WAITNPCITEMS, 6000);
		break;

	case GS_WAITNPCITEMS:

		// wait for npc item information
		if (g_bNeedStop)
		{
			UpdateGameState(GS_CLOSESTORE);
			break;
		}

		if (g_nRetry < 5)
		{
			g_nRetry++;
			UpdateGameState(GS_OPENSTORE); // retry
		}
		else
		{
			g_dwTotalNPCFail++;
			g_log.IncreaseUInt("Log", "NPC Fail");
			UpdateScreenText();
			UpdateGameState(GS_CLOSESTORE); // we give up
		}
		
		break;

	case GS_STOREOK:
		
		if (g_bNeedStop)
		{
			UpdateGameState(GS_CLOSESTORE);
			break;
		}

		// npc item info ended
		//Me.OverHead("ÿc4Checking for items");
		UpdateGameState(GS_BUYITEM, 2000);
		break;

	case GS_BUYITEM:

		if (g_bNeedStop)
		{
			UpdateGameState(GS_CLOSESTORE);
			break;
		}

		// buy items, if anything good has been found
		sprintf(szMessage, "Items examined: %u", g_dwItemChecked);
		SigPostString(szMessage);
		g_log.WriteUInt("Log", "Examined", g_dwTotalExamined);

		pos = g_aGoodItems.GetHeadPosition();
		if (pos)
		{
			Me.OverHead("ÿc4Trying to buy items...");
			SigPostString("Trying to buy items...");
			const ITEM& item = g_aGoodItems.GetNext(pos);			
			Me.BuyItem(item.dwItemID, g_bWasHealing ? Town.GetCurrentTownHealer() : &Npc);
			g_ticker.ResetTicker(5000);

			// Try to buy the item every 5 seconds
			// this state could stick forever, so we need to do some anti-idle
			if ((g_dwTotalTime % 10000) == 0)
				Me.AntiIdle();
		}
		else
		{
			UpdateGameState(GS_CLOSESTORE);
		}
		break;

	case GS_CLOSESTORE:

		Me.OverHead("ÿc4Leaving NPC");
		Me.LeaveNpc(g_bWasHealing ? Town.GetCurrentTownHealer() : &Npc);

		// close the npc screen
		if (!g_bNeedStop)
		{
			g_dwTotalShopping++;
			g_log.IncreaseUInt("Log", "Shopping");
			UpdateScreenText();
		}

		UpdateGameState(GS_NPCTOWP, 500);
		break;

	case GS_NPCTOWP:
		
		// run from npc to wp
		Me.OverHead("ÿc4Running to waypoint");	
		SigPostString("Running to waypoint");
		if (g_bWasHealing)
		{
			for (i = g_aWPToHealer.GetSize() - 1; i >= 0; i--)
				Me.RunTo(g_aWPToHealer[i].x, g_aWPToHealer[i].y);
		}
		else
		{
			for (i = g_aWPToNpc.GetSize() - 1; i >= 0; i--)
				Me.RunTo(g_aWPToNpc[i].x, g_aWPToNpc[i].y);
		}
		WaitForMove(GS_OPENWP);
		break;

	case GS_OPENWP:

		// open the wp
		Me.OverHead("ÿc4Opening waypoint");	
		SigPostString("Opening waypoint");
		if (!Me.IsPlayerInTown())
		{
			if (g_nRetry)
				Me.CloseWP(GetOutWP());
			Me.OpenStaticObject(GetOutWP());
		}
		else
		{
			Me.CloseWP(Town.GetCurrentTownWP());

			if (g_bNeedStop && g_iEndMap == Me.GetCurrentMapID())
			{
				UpdateGameState(GS_WPTOSTART);
				break;
			}
			
			Me.OpenStaticObject(Town.GetCurrentTownWP());
		}
		UpdateGameState(GS_WAITOPENWP, 5000);
		break;

	case GS_WAITOPENWP:

		// wait for the wp screen
		if (g_nRetry < MAX_RETRY)
		{
			g_nRetry++;
			UpdateGameState(GS_OPENWP);
		}
		else
		{
			UpdateGameState(GS_NPCTOWP);
			//FailGame();
		}
		break;

	case GS_USEWP:

		// enter the wp
		Me.OverHead("ÿc4Entering waypoint");
		SigPostString("Entering waypoint");
		if (!Me.IsPlayerInTown())
		{
			// back to town
			//D2Infof("Using outside wp to %02X", g_iTownMapID);
			Me.EnterWP(GetOutWP(), g_iTownMapID);
		}
		else
		{
			if (g_bNeedStop)
			{
				if (D2GetActFromMap(g_iEndMap) == Me.GetCurrentAct())
				{
					// no need to use the wp
					Me.CloseWP(Town.GetCurrentTownWP());
					UpdateGameState(GS_WPTOSTART);
					break;
				}
				else
				{
					Me.EnterWP(Town.GetCurrentTownWP(), g_iEndMap);
				}
			}
			else
			{
				if (Me.GetCurrentAct() != g_global.iAct)
				{
					// swap act
					//D2Infof("Using town wp to other act map %02X", g_iTownMapID);
					Me.EnterWP(Town.GetCurrentTownWP(), g_iTownMapID);
				}
				else
				{
					// leave town
					//D2Infof("Using town wp to %02X", g_global.iWP);
					Me.EnterWP(Town.GetCurrentTownWP(), g_global.iWP);
				}
			}			
		}
		
		UpdateGameState(GS_WAITUSEWP, 8000);
		break;

	case GS_WAITUSEWP:	
		
		// failed to enter wp if reach here
		if (g_nRetry < MAX_RETRY)
		{
			g_nRetry++;
			UpdateGameState(GS_USEWP);
		}
		else
		{
			UpdateGameState(GS_OPENWP);
			//FailGame();
		}
		break;

	case GS_WPTOSTART:

		CalculateStartToWPPath(); // Recalculate the path

		// wp to start location
		Me.CloseWP(Town.GetCurrentTownWP());
		Me.OverHead("ÿc4Going back to start ÿc4location");
		SigPostString("Going back to start location");
		for (i = g_aStartToWP.GetSize() - 1; i >= 0; i--)
			Me.RunTo(g_aStartToWP[i].x, g_aStartToWP[i].y);

		WaitForMove(GS_READYSTOP);
		break;

	case GS_READYSTOP:

		// ready to stop, execute the callback commands now
		if (g_szStopCallback[0])
		{
			CStringParse sp(g_szStopCallback, ";");
			POSITION pos = sp.GetStartPosition();
			while (pos)
			{
				char szCmd[256] = "";
				sp.GetNextString(pos, szCmd, 255);
				server->GameCommandLine(szCmd);
			}
		}

		server->GamePrintInfo("ÿc4Shopbot stopped.");
		Me.OverHead("ÿc4Shopbot stopped.");
		g_text.Undraw();
		g_bNeedStop = FALSE;
		UpdateGameState(GS_NONE);
		break;

	default:
		break;
	} // switch (g_nGameState)

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Application Functions
//////////////////////////////////////////////////////////////////////
const CD2StaticObject* GetOutWP()
{
	// retrieve the outside wp pointer
	return g_outWP.IsValidObject() ? &g_outWP : Town.GetCurrentTownWP();
}

int CalculateStartToWPPath()
{	
	// claculate "start to wp" path
	g_aStartToWP.RemoveAll();
	LPCD2POSITION lpStartToWP = NULL;
	
	switch (Me.GetCurrentAct())
	{
	case 0: // act 1
		if (Town.IsAct1Visited())
			lpStartToWP = Town.IsAct1WPHigh() ? A1_START_TO_WP_TOP : A1_START_TO_WP_BOTTOM;
		break;

	case 1: // act 2
		lpStartToWP = A2_START_TO_WP;
		break;

	case 2: // act 3
		lpStartToWP = A3_START_TO_WP;
		break;

	case 3: // act 4
		lpStartToWP = A4_START_TO_WP;
		break;

	case 4: // act 5
		lpStartToWP = A5_START_TO_WP;
		break;

	default: // shall never happen 
		break;
	}

	if (lpStartToWP == NULL)
		return 0;

	g_aStartToWP.SetSize(SBGetPathCount(lpStartToWP));
	for (int i = 0; i < g_aStartToWP.GetSize(); i++)
	{
		if (Me.GetCurrentAct() == 0)
		{
			// act1 map fucking changes in every game
			g_aStartToWP[i] = Town.GetAct1RelativePosition(lpStartToWP[i].x, lpStartToWP[i].y);
		}
		else
		{
			g_aStartToWP[i].x = lpStartToWP[i].x;
			g_aStartToWP[i].y = lpStartToWP[i].y;
		}		
	}

	return g_aStartToWP.GetSize();
}

int CalculateWPToNpcPath()
{
	// calculate the "wp to npc" path
	g_aWPToNpc.RemoveAll();
	g_aWPToHealer.RemoveAll();
	
	LPCD2POSITION lpWpToNpc = NULL;
	LPCD2POSITION lpWpToHealer = NULL;

	switch (g_global.wNPC)
	{
	case CHARIS:
		if (Town.IsAct1Visited())
			lpWpToNpc = Town.IsAct1WPHigh() ? A1_WP_TOP_TO_CHARIS : A1_WP_BOTTOM_TO_CHARIS;
		break;

	case AKARA:
		if (Town.IsAct1Visited())
			lpWpToNpc = Town.IsAct1WPHigh() ? A1_WP_TOP_TO_AKARA : A1_WP_BOTTOM_TO_AKARA;
		break;

	case FARA:
		lpWpToNpc = A2_WP_TO_FARA;
		break;

	case GROGNAN:
		lpWpToNpc = A2_WP_TO_GROGNAN;
		break;

	case ORMUS:
		lpWpToNpc = A3_WP_TO_ORMUS;
		break;

	case JAMELLA:
		lpWpToNpc = A4_WP_TO_JAMELLA;
		break;

	case HALBU:
		lpWpToNpc = A4_WP_TO_HALBU;
		break;

	case LARZUK:
		lpWpToNpc = A5_WP_TO_LARZUK;
		break;

	case MALAH:
		lpWpToNpc = A5_WP_TO_MALAH;
		break;

	case ANYA:
		lpWpToNpc = A5_WP_TO_ANYA;
		break;

	default: // shall never happen
		break;
	}

	if (lpWpToNpc == NULL)
		return 0;

	g_aWPToNpc.SetSize(SBGetPathCount(lpWpToNpc));
	int i = 0;
	for (i = 0; i < g_aWPToNpc.GetSize(); i++)
	{
		if (Me.GetCurrentAct() == 0)
		{
			g_aWPToNpc[i] = Town.GetAct1RelativePosition(lpWpToNpc[i].x, lpWpToNpc[i].y);
		}
		else
		{
			g_aWPToNpc[i].x = lpWpToNpc[i].x;
			g_aWPToNpc[i].y = lpWpToNpc[i].y;
		}
	}

	// wp to healer path
	switch (Me.GetCurrentAct())
	{
	case 0:
		if (Town.IsAct1Visited())
			lpWpToHealer = Town.IsAct1WPHigh() ? A1_WP_TOP_TO_AKARA : A1_WP_BOTTOM_TO_AKARA;
		break;

	case 1:
		lpWpToHealer = A2_WP_TO_FARA;
		break;

	case 2:
		lpWpToHealer = A3_WP_TO_ORMUS;
		break;

	case 3:
		lpWpToHealer = A4_WP_TO_JAMELLA;
		break;

	case 4:
		lpWpToHealer = A5_WP_TO_MALAH;
		break;

	default:
		break;
	}

	g_aWPToHealer.SetSize(SBGetPathCount(lpWpToHealer));
	for (i = 0; i < g_aWPToHealer.GetSize(); i++)
	{
		if (Me.GetCurrentAct() == 0)
		{
			g_aWPToHealer[i] = Town.GetAct1RelativePosition(lpWpToHealer[i].x, lpWpToHealer[i].y);
		}
		else
		{
			g_aWPToHealer[i].x = lpWpToHealer[i].x;
			g_aWPToHealer[i].y = lpWpToHealer[i].y;
		}
	}
	
	return g_aWPToNpc.GetSize();
}

void UpdateGameState(int nNewState, DWORD dwDelay)
{
	g_nGameState = nNewState;
	g_ticker.ResetTicker(dwDelay);
}

void WaitForMove(int nNextState)
{
	g_nNextState = nNextState;
	UpdateGameState(GS_WAITMOVE, 30000);
}

void WaitForNpc(int nNextState)
{
	g_nNextState = nNextState;
	UpdateGameState(GS_WAITNPC, 4000);
}

WORD UseQuicknessSpell() // Try to use vigor or BoS
{
	if (g_wQuicknessSpell == D2S_VIGOR && Me.GetRightSpell() != D2S_VIGOR)
	{
		Me.SelectSpell(D2S_VIGOR, FALSE);
		return D2S_VIGOR;
	}

	if (g_wQuicknessSpell == D2S_BURSTOFSPEED && !Me.IsAffected(STATE_QUICKNESS))
	{
		Me.CastSpell(D2S_BURSTOFSPEED, FALSE);
		return D2S_BURSTOFSPEED;
	}

	return D2S_INVALID;
}


void UpdateScreenText()
{
	// draw texts on game screen
	const int TEXTLINES	= 9; // lines of text
	LPD2DRAWDATA aData = new D2DRAWDATA[TEXTLINES];
	::memset(aData, 0, sizeof(D2DRAWDATA) * TEXTLINES);

	long y = 370; // use this screen y

	for (int i = 0; i < TEXTLINES; i++)
	{
		aData[i].x = 17;
		aData[i].y = y + (i * 15);
		aData[i].nFont = D2FONT_NORMAL; // draw normal font
	}	

	int nLine = 0;
	char szTemp[256] = "";
	
	//sprintf(aData[nLine++].szText, "%s", "-----------------------------------------");
	if (g_bAutoStop)
	{
		DWORD dwHours = 0, dwMinutes = 0, dwSeconds = 0;
		GetTimeFormat(g_dwRemainMS, dwHours, dwMinutes, dwSeconds);
		if (g_dwRemainMS > 300 * 1000)
			sprintf(aData[nLine++].szText, "ÿc4Remain:       ÿc0%02u : %02u : %02u", dwHours, dwMinutes, dwSeconds);
		else if (g_dwRemainMS > 60 * 1000)
			sprintf(aData[nLine++].szText, "ÿc4Remain:       ÿc9%02u : %02u : %02u", dwHours, dwMinutes, dwSeconds);
		else
			sprintf(aData[nLine++].szText, "ÿc4Remain:       ÿc1%02u : %02u : %02u", dwHours, dwMinutes, dwSeconds);
	}	
	
	sprintf(aData[nLine++].szText, "ÿc4Vendor:       ÿc8%s ÿc9Act %d", g_szNpcName, g_global.iAct + 1);
	D2GetMapName(g_global.iWP, szTemp, 255);
	sprintf(aData[nLine++].szText, "ÿc4Out Step:    ÿc9%s", szTemp);
	D2GetMapName(g_iEndMap, szTemp, 255);
	sprintf(aData[nLine++].szText, "ÿc4End Map:    ÿc9%s", szTemp);
	sprintf(aData[nLine++].szText, "ÿc4Quit HP:      ÿc1%d%%", g_global.iStopHP);	
	D2GetKeyName(g_global.iHotkey, szTemp, 255);
	sprintf(aData[nLine++].szText, "ÿc4Hotkey:       ÿc0%s", szTemp);
	sprintf(aData[nLine++].szText, "ÿc4Visits:         ÿc2%u", g_dwTotalShopping);
	sprintf(aData[nLine++].szText, "ÿc4Examined:   ÿc2%u", g_dwTotalExamined);
	if (g_dwTotalBought == 0)
		sprintf(aData[nLine++].szText, "ÿc4Bought:        ÿc0%u", g_dwTotalBought);
	else
		sprintf(aData[nLine++].szText, "ÿc4Bought:        ÿc2%u", g_dwTotalBought);
	//sprintf(aData[nLine++].szText, "ÿc9Npc Fails:   %u", g_dwTotalNPCFail);
	//sprintf(aData[nLine++].szText, "%s", "-----------------------------------------");

	g_text.DrawMultipleTexts(aData, TEXTLINES);
	delete [] aData;
}

BOOL IsNeedHeal()
{
	// need to heal?
	return !Town.IsHealer(&Npc) && Town.GetCurrentTownHealer() && !g_aWPToHealer.IsEmpty() && Me.GetHPPercent() < 95;
}

void SigPostString(LPCTSTR lpszString)
{
	if (!g_bUseSig || lpszString == NULL || !lpszString[0])
		return;

	char sz[256] = "";
	sprintf(sz, "sig poststring %s", lpszString);
	server->GameCommandLine(sz);
}

void LoadLogData() // Loda data from shopbot.log
{
	char szFilePath[MAX_PATH + 1] = "";
	sprintf(szFilePath, "%s\\Shopbot.log", GetModuleDirectory());
	g_log.SetPathName(szFilePath);
	g_dwTotalShopping = g_log.GetUInt("Log", "Shopping", 0);
	g_dwTotalBought = g_log.GetUInt("Log", "Bought", 0);
	g_dwTotalNPCFail = g_log.GetUInt("Log", "NPC Fail", 0);
	g_dwTotalExamined = g_log.GetUInt("Log", "Examined", 0);
	if (Me.IsGameJoined())
		UpdateScreenText();
}

BOOL CleanLog() // Clean shopbot.log
{
	char szFilePath[MAX_PATH + 1] = "";
	sprintf(szFilePath, "%s\\Shopbot.log", GetModuleDirectory());	
	HANDLE hFile = ::CreateFile(szFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == NULL)
		return FALSE;
	
	::CloseHandle(hFile);

	g_dwTotalShopping = 0;
	g_dwTotalBought = 0;
	g_dwTotalNPCFail = 0;
	g_dwTotalExamined = 0;
	if (Me.IsGameJoined())
		UpdateScreenText();
	
	return TRUE;
}

void LoadGlobalData() // Load global data from shopbot.ini
{
	char szFilePath[MAX_PATH + 1] = "";
	sprintf(szFilePath, "%s\\Shopbot.ini", GetModuleDirectory());

	// load global settings
	::memset(&g_global, 0, sizeof(g_global));
	::SBLoadGlobal(szFilePath, &g_global);
}

void LoadConfigData() // Load config data from shopbot.ini
{
	char szFilePath[MAX_PATH + 1] = "";
	sprintf(szFilePath, "%s\\Shopbot.ini", GetModuleDirectory());

	// load item settings
	CIni ini(szFilePath);
	DWORD dwBufSize = 1024;
	char *pszBuffer = new char[dwBufSize];
	DWORD dwCopied = ini.GetSectionNames(pszBuffer, dwBufSize - 1);
	while (dwCopied + 128 >= dwBufSize)
	{
		// keey enlarging the buffer unless we make sure all section names
		// are loaded.
		dwBufSize += 1024;
		delete [] pszBuffer;
		pszBuffer = new char[dwBufSize];
		dwCopied = ini.GetSectionNames(pszBuffer, dwBufSize - 1);
	}
	
	// iterate through all section names except "Global"
	g_aConfig.RemoveAll();
	for (const char* pSection = pszBuffer; pSection[0] != _T('\0'); pSection = &pSection[strlen(pSection) + 1])
	{	
		SBITEMCONFIG cfg = {0};
		if (::SBLoadConfig(szFilePath, pSection, &cfg))
			g_aConfig.Add(cfg);
	}

	delete [] pszBuffer;
}

void InitializeObjects() // Initialize game objects abd attributes
{
	// install hotkey
	g_hotkey.InstallKeyBind();
	g_hotkey.SetEntry(g_global.iHotkey, 0, "shopbot pause");
	g_hotkey.SetKeyBindProc(OnHotkey);

	// instantiate npc
	D2OBJFINGERPRINTER fp = FP_NPC_AKARA; // just for the format
	::memcpy(fp.aData, &g_global.wNPC, 2); // now copy the real npc fingerprinter from g_global
	Npc.SetFingerprinter(&fp);

	Me.SetD2MessageProc(OnPlayerMessage);

	// Determine appropriate WP
	g_iTownMapID = D2GetTownMap(g_global.iAct);
	
	fp = FP_OBJ_ACT1WP; // just for the format;
	WORD wOtherWPFP = GetWaypointFingerprinterData(g_global.iWP);
	::memcpy(fp.aData, &wOtherWPFP, 2);
	g_outWP.SetFingerprinter(&fp);

	// NPC names
	SBGetNpcName(g_global.wNPC, g_szNpcName, 63);	
}

//////////////////////////////////////////////////////////////////////
// Module Commands
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	Me.SetDebug(!Me.IsDebug());
	if (Me.IsDebug())
		Me.DebugMapData();
	return TRUE;
}

BOOL PRIVATE OnGameCommandPause(char** argv, int argc)
{
	g_bPaused = !g_bPaused;
	Me.PauseJobs(g_bPaused);
	if (g_bPaused)
		server->GamePrintInfo("ÿc4Shopbot ÿc2pausedÿc4.");
	else
		server->GamePrintInfo("ÿc4Shopbot ÿc2resumedÿc4.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandCleanLog(char** argv, int argc)
{
	if (!CleanLog())
		server->GamePrintError("ÿc4Failed to clean log file!");
	else
		server->GamePrintInfo("ÿc4Shopbot.log contents cleared");
	return TRUE;
}

BOOL PRIVATE OnGameCommandReload(char** argv, int argc)
{
	LoadConfigData();
	server->GamePrintInfo("ÿc4Item config has been reloaded from \"Shopbot.ini\"");
	server->GamePrintInfo("ÿc4Global settings will not change until you restart Shopbot.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandCallback(char** argv, int argc)
{
	::memset(g_szStopCallback, 0, sizeof(g_szStopCallback));
	for (int i = 2; i < argc; i++)
		sprintf(g_szStopCallback, "%s%s ", g_szStopCallback, argv[i]);

	D2Infof("ÿc4Shopbot callback: ÿc0%s", g_szStopCallback);

	return TRUE;
}

BOOL PRIVATE OnGameCommandStop(char** argv, int argc)
{
	g_bStartAfterGameReady = FALSE;

	if (g_nGameState  == GS_NONE)
	{
		server->GamePrintInfo("ÿc4Shopbot already stopped.");
	}
	else if (g_bNeedStop)
	{
		server->GamePrintError("Shopbot is already at stopping stage.");
	}
	else
	{
		g_bNeedStop = TRUE;
		server->GamePrintInfo("ÿc4Shopbot stopping, going back to start location.");
	}

	return TRUE;
}

BOOL PRIVATE OnGameCommandStart(char** argv, int argc)
{
	if (!VerifyConditions())
		return TRUE;	

	g_dwTotalTime = 0;
	g_dwRemainMS = 0;	

	if (argc > 2)
		g_dwRemainMS = strtoul(argv[2], NULL, 10);
	g_dwRemainMS = min(g_dwRemainMS, 3600 * 100 - 1);
	g_dwRemainMS *= 1000; // to milliseconds
	g_bStartAfterGameReady = TRUE;
	
	Me.OverHead("ÿc4Starting Shopbot...");
	SigPostString("Shopbot 1.10 by Abin");
	
	if (Me.IsGameReady())
		StartShopping();	
	return TRUE;
}

BOOL PRIVATE OnGameCommandAutoStart(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: ÿc0.shopbot autostart 0/1");
		return TRUE;
	}
	
	g_global.bAutoStart = atoi(argv[2]);
	if (g_global.bAutoStart)
		server->GamePrintInfo("ÿc4Shop auto-start is ÿc2on");
	else
		server->GamePrintInfo("ÿc4Shop auto-start is ÿc2off");
	return TRUE;
}

BOOL PRIVATE OnGameCommandEndAct(char** argv, int argc)
{
	if (g_bNeedStop)
	{
		server->GamePrintError("Shopbot is already at stopping stage.");
		server->GamePrintError("Cannot change the ending act now.");
		return TRUE;
	}

	if (argc < 3)
	{
		server->GamePrintError("ÿc4Syntax: ÿc0.shopbot endact 1/2/3/4/5");
		return TRUE;
	}

	BYTE iAct = (BYTE)atoi(argv[2]);
	if (iAct < 1 || iAct > 5)
	{
		server->GamePrintError("ÿc4Syntax: ÿc0.shopbot endact 1/2/3/4/5");
		return TRUE;
	}

	g_iEndMap = ::D2GetTownMap(iAct - 1);
	D2Infof("ÿc4Shopbot end act: ÿc2Act %d", iAct);
	if (g_nGameState  != GS_NONE)
		UpdateScreenText();
	return TRUE;
}

void GetTimeFormat(DWORD dwMilliseconds, DWORD& dwHours, DWORD& dwMinutes, DWORD& dwSeconds)
{
	// format a DWORD into hh:mm:ss
	dwSeconds = dwMilliseconds / 1000; // use seconds
	dwHours = dwSeconds / 3600;
	dwSeconds %= 3600;
	dwMinutes = dwSeconds / 60;
	dwSeconds %= 60;
}

BOOL VerifyConditions()
{
	if (g_nGameState != GS_NONE)
	{
		// already started
		server->GamePrintError("ÿc4Shopbot already started.");
		return FALSE;
	}

	if (!Me.IsLoadedBeforeJoin())
	{
		server->GamePrintInfo("Remember you must load Shopbot before joining the game.");
		/*
		// shopbot was not loaded before joining this game
		server->GamePrintError("ÿc4-----------------------------------------------");
		server->GamePrintError("ÿc1Shopbot Error");
		server->GamePrintError("ÿc4Shopbot must be loaded before joining game");
		server->GamePrintError("ÿc4Please enter a new game");
		server->GamePrintError("ÿc4-----------------------------------------------");
		Me.OverHead("ÿc1Please join a new game");
		return FALSE;
		*/
	}

	if (Me.IsGameReady() && !Me.IsPlayerInTown())
	{
		// player is not in town!
		server->GamePrintError("ÿc4You must be in town to start Shopbot!");
		Me.OverHead("ÿc1Player not in town!");
		return FALSE;
	}

	if (Me.IsGameReady() && !Town.IsPlayerAtStartPosition(&Me, 20))
	{
		// player location is too far to the start location
		server->GamePrintError("ÿc4Please go to the start location first!");
		Me.OverHead("ÿc1Player location check ÿc1failed!");
		return FALSE;
	}

	return TRUE; // every thing seemed ok...
}

void StartShopping() // Start shopping
{
	g_bNeedStop = FALSE;
	g_bAutoStop = g_dwRemainMS > 0;

	if (g_bAutoStop)
		D2Infof("ÿc4Shopbot started. Time limit ÿc2%d ÿc4seconds.", g_dwRemainMS / 1000);
	else
		server->GamePrintInfo("ÿc4Shopbot started.");

	UpdateScreenText();

	if (Me.GetCorpseCount())
	{
		// if there's a corpse, pick it up first
		Me.OverHead("ÿc4Picking up corpse");
		server->GamePrintInfo("Player corpse detected!");
		Me.PickupCorpse();
		UpdateGameState(GS_CALCSTARTTOWPPATH, 5000);
	}
	else
	{
		UpdateGameState(GS_CALCSTARTTOWPPATH);
	}
}

// cannot believe such a small bot took me over 1000 lines of code, maybe that
// means I still suck on programming? oh well...