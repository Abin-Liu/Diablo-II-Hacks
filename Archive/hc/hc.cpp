//////////////////////////////////////////////////////////////////////
// HC.CPP
//
// Copyright (c) 2002, Abin (Bin Liu)
// All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#include "hc.h"
#pragma comment(lib, "..\\D2lib\\D2Lib.lib") // Import D2Lib
#include "..\\D2Lib\\ClientCore.cpp"

//////////////////////////////////////////////////////////////////////
// Message Handler of the CD2Player Object
//////////////////////////////////////////////////////////////////////
void CALLBACK OnPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	if (nMessage == PM_HPDOWN) // Player HP Drop Down
	{
		const BYTE HP = Me.GetHPPercent();
		if (HP == 0)
			return; // player already dead

		// HP is lower than alert level, do something
		if (HP < g_iQuitHP && CheckAlert(ALERT_PLAYER_HP, HP))
		{
			g_bQuitNow = TRUE;
			return;
		}	
		
		if (HP < g_iTownHP && CheckAlert(ALERT_PLAYER_HP, HP))
		{
			// TP to town
			Me.BackToTown();
			return;
		}

		if (HP < g_iPotionHP && CheckAlert(ALERT_PLAYER_HP, HP))
		{			
			// Drink a potion, full rejuv > rejuv > super healing > greater healing > healing > light healing > minor healing
			DWORD dwType = Me.HealMe(); // Drink a potion

			if (dwType == 0)
			{
				server->GamePrintError("ÿc4Unable to drink any potion!");
			}
			else
			{
				char szPoiton[65] = "";
				::D2GetBeltItemName(dwType, szPoiton, 64);
				::D2Infof("ÿc4Drank a %s successfully.", szPoiton);
			}			
		
			return;
		}

		return;
	}

	if (nMessage == PM_MANADOWN) // Player Mana Drop Down
	{
		const BYTE MANA = Me.GetManaPercent();

		// Mana is lower than alert level, do something
		if (MANA < g_iQuitMana && CheckAlert(ALERT_PLAYER_MANA, MANA))
		{
			g_bQuitNow = TRUE;
			return;
		}	
		
		if (MANA < g_iTownMana && CheckAlert(ALERT_PLAYER_MANA, MANA))
		{
			// TP to town
			Me.BackToTown();
			return;
		}

		if (MANA < g_iPotionMana && CheckAlert(ALERT_PLAYER_MANA, MANA))
		{
			DWORD dwType = Me.SupplyMana(); // Drink a mana potion

			if (dwType == 0)
			{
				server->GamePrintError("ÿc4Unable to drink any potion for mana!");
			}
			else
			{
				char szPoiton[65] = "";
				::D2GetBeltItemName(dwType, szPoiton, 64);
				::D2Infof("ÿc4Drank a %s successfully.", szPoiton);
			}			
		}

		return;
	}

	if (nMessage == PM_HOSTILE) // Player is Hostiled by Others
	{
		server->GamePrintInfo("ÿc1Hostility Detected!");
		if (Me.IsPlayerInTown())
		{
			server->GamePrintInfo("ÿc4Player in town, no danger.");
			return;
		}

		if (g_nOnHostile == TR_QUIT)
		{
			g_bQuitNow = TRUE;
		}
		else if (g_nOnHostile == TR_TOWN)
		{
			Me.BackToTown();
		}

		return;
	}
}

//////////////////////////////////////////////////////////////////////
// Message Handler of the CD2Merc Object
//////////////////////////////////////////////////////////////////////
void CALLBACK OnPetMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	if (nMessage == MM_HPPERCENTDOWN) // Merc HP Drop Down
	{
		BYTE PER = (BYTE)lParam;
		if (PER == 0)
			return;

		// HP is lower than alert level, do something
		if (PER < g_iMercQuitHP && CheckAlert(ALERT_MERC, PER))
		{
			g_bQuitNow = TRUE;
			return;
		}	
		
		if (PER < g_iMercTownHP && CheckAlert(ALERT_MERC, PER))
		{
			// TP to town
			Me.BackToTown();
			return;
		}

		if (PER < g_iMercPotionHP && CheckAlert(ALERT_MERC, PER))
		{
			DWORD dwType = Me.HealMerc(); // Feed merc a potion

			if (dwType == 0)
			{
				server->GamePrintError("ÿc4Unable to feed the merc any potion!");
			}
			else
			{
				char szPoiton[65] = "";
				D2GetBeltItemName(dwType, szPoiton, 64);
				::D2Infof("ÿc4Fed the merc a %s successfully.", szPoiton);
			}		

			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// OnClientStart
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{
	char szPath[_MAX_PATH] = "";
	sprintf(szPath, "%s\\%s", GetModuleDirectory(), "hc.ini");
	g_ini.SetPathName(szPath);

	sprintf(szPath, "%s\\%s", GetModuleDirectory(), "bind.ini");
	g_bind_ini.SetPathName(szPath);

	// Set D2Lib Message Handlers 
	Me.SetD2MessageProc(OnPlayerMessage); // for player
	Pet.SetD2MessageProc(OnPetMessage); // for pet
	//Me.SetDebug(TRUE);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStop
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStop()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameJoin
//////////////////////////////////////////////////////////////////////
void EXPORT OnGameJoin(THISGAMESTRUCT* game)
{	
	Me.OnGameJoin(game);
	Pet.OnGameJoin(game);
	
	g_bMemChecked = FALSE;
	g_bQuitNow = FALSE;	
	Me.GetPlayerName(g_szPlayerName, PLAYER_MAX_NAME);

	if (IsBindPresent())
		server->GameCommandLine("load bind");

	LoadConfig();	
}

//////////////////////////////////////////////////////////////////////
// OnGameLeave
//////////////////////////////////////////////////////////////////////
void EXPORT OnGameLeave(THISGAMESTRUCT* game)
{	
	Me.OnGameLeave();
	Pet.OnGameLeave();
	g_bQuitNow = FALSE;
	::memset(g_szPlayerName, 0, sizeof(g_szPlayerName));	
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeReceived
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{	
	// Just let D2Lib process the packets
	Me.OnGamePacketBeforeReceived(aPacket, aLen);
	Pet.OnGamePacketBeforeReceived(aPacket, aLen);
    return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeSent
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	// Just let D2Lib process the packets
	Me.OnGamePacketBeforeSent(aPacket, aLen);
	Pet.OnGamePacketBeforeSent(aPacket, aLen);
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
	// quit game: highest priority
	if (g_bQuitNow)
	{
		g_bQuitNow = FALSE;
		Me.LeaveGame();
	}

	// Let D2Lib process the timer first
	Me.OnGameTimerTick();
	Pet.OnGameTimerTick();

	if (!g_bMemChecked && Me.IsGameReady())
	{
		g_bMemChecked = TRUE;

		// Check memory!
		if (Me.GetPlayerStats(D2PS_MAXHP) == 0)
		{
			server->GamePrintError("ÿc4-------------------------------------------------");
			server->GamePrintError("ÿc1        Warning:");
			server->GamePrintError(" ");
			server->GamePrintError("ÿc4HC failed to scan the memory for player stats address!");
			server->GamePrintError("ÿc4This may caused by use of unknown D2 loaders");
			server->GamePrintError("ÿc4As a result, HC will not work!");
			server->GamePrintError("ÿc4-------------------------------------------------");
		}
		else
		{
			if (!Me.DisplayNotLoadedBeforeJoinWarning("hc"))
				OnGameCommandSummary(NULL, 2); // display configs upon new game	
		}	
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////
// Module Commands
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	::D2Infof("HP: %d/%d (%d%%), Mana: %d/%d (%d%%)",
		Me.GetPlayerStats(D2PS_HP),
		Me.GetPlayerStats(D2PS_MAXHP),
		Me.GetHPPercent(),
		Me.GetPlayerStats(D2PS_MANA),
		Me.GetPlayerStats(D2PS_MAXMANA),
		Me.GetManaPercent());
	/*
	Me.SetDebug(!Me.IsDebug());
	if (Me.IsDebug())
		server->GamePrintInfo("Debug: On");
	else
		server->GamePrintInfo("Debug: Off");
	*/
    return TRUE;
}

BOOL PRIVATE OnGameCommandSync(char** argv, int argc)
{
	if (Me.RedrawClient(TRUE))
		server->GamePrintInfo("ÿc4Client view synchronized.ÿc0");
	return TRUE;
}

BOOL PRIVATE OnGameCommandQuit(char** argv, int argc)
{
	Me.LeaveGame();
	return TRUE;
}

BOOL PRIVATE OnGameCommandSetQuitKey(char** argv, int argc)
{
	if (argc != 3)
	{
		server->GamePrintError("Syntax: .hc key.quit <key_code>");
		return TRUE;
	}

	if (!SetShortcutKey((BYTE)strtoul(argv[2], NULL, 16), KB_QUIT))
		server->GamePrintError("Syntax: .hc key.quit <key_code>");
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandSetSyncKey(char** argv, int argc)
{
	if (argc != 3)
	{
		server->GamePrintError("Syntax: .hc key.sync <key_code>");
		return TRUE;
	}

	if (!SetShortcutKey((BYTE)strtoul(argv[2], NULL, 16), KB_SYNC))
		server->GamePrintError("Syntax: .hc key.quit <key_code>");

	return TRUE;
}

BOOL PRIVATE OnGameCommandSetTownKey(char** argv, int argc)
{
	if (argc != 3)
	{
		server->GamePrintError("Syntax: .hc key.town <key_code>");
		return TRUE;
	}

	if (!SetShortcutKey((BYTE)strtoul(argv[2], NULL, 16), KB_TOWN))
		server->GamePrintError("Syntax: .hc key.quit <key_code>");

	return TRUE;
}

BOOL PRIVATE OnGameCommandHostile(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: .hc hostile <nothing/quit/town>");
		return TRUE;
	}

	if (stricmp(argv[2], "nothing")
		&& stricmp(argv[2], "quit")
		&& stricmp(argv[2], "town"))
	{
		server->GamePrintError("Syntax: .hc hostile <nothing/quit/town>");
		return TRUE;
	}

	g_nOnHostile = Str2Method(argv[2], FALSE);
	g_ini.WriteInt(g_szPlayerName, "On Hostile", g_nOnHostile);
    ShowHostilePolicy();
	return TRUE;
}

BOOL PRIVATE OnGameCommandHP(char** argv, int argc)
{
	if (argc < 4)
	{
		server->GamePrintError("Syntax: .hc hp <quit/town/potion> <Percent>");
		return TRUE;
	}
	
	BYTE* p = NULL;
	char szEntry[32] = "";
	switch (Str2Method(argv[2], TRUE))
	{
	case TR_QUIT:
		p = &g_iQuitHP;
		strcpy(szEntry, "Quit HP");
		break;

	case TR_TOWN:
		p = &g_iTownHP;
		strcpy(szEntry, "Town HP");
		break;

	case TR_POTION:
		p = &g_iPotionHP;
		strcpy(szEntry, "Potion HP");
		break;

	default:
		p = NULL;
		break;
	}

	if (p == NULL)
	{
		server->GamePrintError("Syntax: .hc hp <quit/town/potion> <Percent>");
		return TRUE;
	}

	*p = (BYTE)strtoul(argv[3], NULL, 10);
	g_ini.WriteUInt(g_szPlayerName, szEntry, *p);
	ShowHPPolicy();
	return TRUE;
}

BOOL PRIVATE OnGameCommandMana(char** argv, int argc)
{
	if (argc < 4)
	{
		server->GamePrintError("Syntax: .hc mana <quit/town/potion> <Percent>");
		return TRUE;
	}
	
	BYTE* p = NULL;
	char szEntry[32] = "";
	switch (Str2Method(argv[2], TRUE))
	{
	case TR_QUIT:
		p = &g_iQuitMana;
		strcpy(szEntry, "Quit Mana");
		break;

	case TR_TOWN:
		p = &g_iTownMana;
		strcpy(szEntry, "Town Mana");
		break;

	case TR_POTION:
		p = &g_iPotionMana;
		strcpy(szEntry, "Potion Mana");
		break;

	default:
		p = NULL;
		break;
	}

	if (p == NULL)
	{
		server->GamePrintError("Syntax: .hc mana <quit/town/potion> <Percent>");
		return TRUE;
	}

	*p = (BYTE)strtoul(argv[3], NULL, 10);
	g_ini.WriteUInt(g_szPlayerName, szEntry, *p);
	ShowManaPolicy();
	return TRUE;
}

BOOL PRIVATE OnGameCommandMerc(char** argv, int argc)
{
	if (argc < 4)
	{
		server->GamePrintError("Syntax: .hc merc <quit/town/potion> <Percent>");
		return TRUE;
	}
	
	BYTE* p = NULL;
	char szEntry[32] = "";
	switch (Str2Method(argv[2], TRUE))
	{
	case TR_QUIT:
		p = &g_iMercQuitHP;
		strcpy(szEntry, "Quit Merc");
		break;

	case TR_TOWN:
		p = &g_iMercTownHP;
		strcpy(szEntry, "Town Merc");
		break;

	case TR_POTION:
		p = &g_iMercPotionHP;
		strcpy(szEntry, "Potion Merc");
		break;

	default:
		p = NULL;
		break;
	}

	if (p == NULL)
	{
		server->GamePrintError("Syntax: .hc merc <quit/town/potion> <Percent>");
		return TRUE;
	}

	*p = (BYTE)strtoul(argv[3], NULL, 10);
	g_ini.WriteUInt(g_szPlayerName, szEntry, *p);
	ShowMercPolicy();
	return TRUE;
}

BOOL PRIVATE OnGameCommandSummary(char** argv, int argc)
{
	char sz[128] = "";
	sprintf(sz, "Settings for ÿc7%sÿc0:", g_szPlayerName);
	server->GamePrintInfo(sz);
	server->GamePrintInfo("ÿc7-----------------------------------------------------");
	ShowHPPolicy();
	ShowManaPolicy();
	ShowHostilePolicy();
	ShowMercPolicy();
	ShowShortcutKeys();
	server->GamePrintInfo("ÿc7-----------------------------------------------------");
	return TRUE;
}

BOOL PRIVATE OnGameCommandTown(char** argv, int argc)
{
	Me.BackToTown();
	return TRUE;
}

void ShowHPPolicy()
{
	if (g_iPotionHP > 0)
		::D2Infof("HP < %d%%:    ÿc2Drink Potion", g_iPotionHP);

	if (g_iTownHP > 0)
		::D2Infof("HP < %d%%:    ÿc2Back to Town", g_iTownHP);

	if (g_iQuitHP > 0)
		::D2Infof("HP < %d%%:    ÿc2Quit Game", g_iQuitHP);
	
	if (g_iQuitHP ==0 && g_iTownHP == 0 && g_iPotionHP == 0)
		server->GamePrintInfo("HP Low:    ÿc1Do Nothing");
}

//////////////////////////////////////////////////////////////////////
// Application Functions
//////////////////////////////////////////////////////////////////////
void ShowMercPolicy()
{
	if (g_iMercPotionHP > 0)
		::D2Infof("Merc HP < %d%%:    ÿc2Drink Potion", g_iMercPotionHP);

	if (g_iMercTownHP > 0)
		::D2Infof("Merc HP < %d%%:    ÿc2Back to Town", g_iMercTownHP);

	if (g_iMercQuitHP > 0)
		::D2Infof("Merc HP < %d%%:    ÿc2Quit Game", g_iMercQuitHP);
	
	if (g_iMercQuitHP == 0 && g_iMercTownHP == 0 && g_iMercPotionHP == 0)
		server->GamePrintInfo("Merc HP Low:    ÿc1Do Nothing");
}

void ShowHostilePolicy()
{
	if (g_nOnHostile == TR_QUIT)
		server->GamePrintInfo("Hotiled:    ÿc2Quit game");
	else if (g_nOnHostile == TR_TOWN)
		server->GamePrintInfo("Hotiled:    ÿc2Back to Town");
	else
		server->GamePrintInfo("Hotiled:    ÿc1Do Nothing");
}

void ShowManaPolicy()
{
	if (g_iPotionMana > 0)
		::D2Infof("Mana < %d%%:    ÿc2Drink Potion", g_iPotionMana);

	if (g_iTownMana > 0)
		::D2Infof("Mana < %d%%:    ÿc2Back to Town", g_iTownMana);

	if (g_iQuitMana > 0)
		::D2Infof("Mana < %d%%:    ÿc2Quit Game", g_iQuitMana);
	
	if (g_iQuitMana ==0 && g_iTownMana == 0 && g_iPotionMana == 0)
		server->GamePrintInfo("Mana Low:    ÿc1Do Nothing");
}

void ShowShortcutKeys()
{	
	char szKey[64] = "";
	BYTE iQuitKey, iSyncKey, iTownKey;
	SearchBindKeys(iQuitKey, iSyncKey, iTownKey);

	::D2GetKeyName(iQuitKey, szKey, 63);
	::D2Infof("ÿc7Quitÿc0 shortcut key:    ÿc2%s", szKey);

	::D2GetKeyName(iSyncKey, szKey, 63);
	::D2Infof("ÿc7Syncÿc0 shortcut key:    ÿc2%s", szKey);

	::D2GetKeyName(iTownKey, szKey, 63);
	::D2Infof("ÿc7Townÿc0 shortcut key:    ÿc2%s", szKey);
}

void LoadConfig()
{
	// Character specific Config

	// Reaction on life low
	g_iQuitHP = (BYTE)g_ini.GetUInt(g_szPlayerName, "Quit HP", 0);
	g_iQuitHP = min(g_iQuitHP, 100);
	g_iTownHP = (BYTE)g_ini.GetUInt(g_szPlayerName, "Town HP", 0);
	g_iTownHP = min(g_iTownHP, 100);
	g_iPotionHP = (BYTE)g_ini.GetUInt(g_szPlayerName, "Potion HP", 0);
	g_iPotionHP = min(g_iPotionHP, 100);

	// Reaction on mana low
	g_iQuitMana = (BYTE)g_ini.GetUInt(g_szPlayerName, "Quit Mana", 0);
	g_iQuitMana = min(g_iQuitMana, 100);
	g_iTownMana = (BYTE)g_ini.GetUInt(g_szPlayerName, "Town Mana", 0);
	g_iTownMana = min(g_iTownMana, 100);
	g_iPotionMana = (BYTE)g_ini.GetUInt(g_szPlayerName, "Potion Mana", 0);
	g_iPotionMana = min(g_iPotionMana, 100);

	// Reaction on merc life low
	g_iMercQuitHP = (BYTE)g_ini.GetUInt(g_szPlayerName, "Quit Merc", 0);
	g_iMercQuitHP = min(100, g_iMercQuitHP);
	g_iMercTownHP = (BYTE)g_ini.GetUInt(g_szPlayerName, "Town Merc", 0);
	g_iMercTownHP = min(100, g_iMercTownHP);	
	g_iMercPotionHP = (BYTE)g_ini.GetUInt(g_szPlayerName, "Potion Merc", 0);
	g_iMercPotionHP = min(100, g_iMercPotionHP);

	// Reaction on hostility
	g_nOnHostile = g_ini.GetInt(g_szPlayerName, "On Hostile", TR_NONE);
	if (g_nOnHostile != TR_QUIT && g_nOnHostile != TR_TOWN)
		g_nOnHostile = TR_NONE;
}

BOOL SetShortcutKey(BYTE iKeyCode, int nKeyType)
{
	if (!IsBindPresent())
		return FALSE;

	if (iKeyCode == 0)
	{
		server->GamePrintError("Invalid key code. Must between 01 and FF");
		return FALSE;
	}

	char szKey[64] = "";	
	::D2GetKeyName(iKeyCode, szKey, 63);
	if (strlen(szKey) == 0)
	{
		server->GamePrintError("Invalid key, cannot be used.");
		return FALSE;
	}

	char szLine[128] = "";
	char szMsg[512] = "";
	if (nKeyType == KB_QUIT)
	{
		sprintf(szLine, "bind set %X 0 hc quit", iKeyCode);
		sprintf(szMsg, "Shortcut key for ÿc7Quit Gameÿc0 has been set to ÿc2%s", szKey);
	}
	else if (nKeyType == KB_SYNC)
	{
		sprintf(szLine, "bind set %X 0 hc sync", iKeyCode);		
		sprintf(szMsg, "Shortcut key for ÿc7Syncÿc0 has been set to ÿc2%s", szKey);
	}
	else if (nKeyType == KB_TOWN)
	{
		sprintf(szLine, "bind set %X 0 hc town", iKeyCode);
		sprintf(szMsg, "Shortcut key for ÿc7Townÿc0 has been set to ÿc2%s", szKey);
	}
	else
	{
		return FALSE;
	}

	server->GameCommandLine(szLine);
	server->GamePrintInfo(szMsg);	
	return TRUE;
}

BOOL CheckAlert(int nAlertType, BYTE iValue)
{
	BOOL bAlert = TRUE;
	char szAlert[128] = "";
	
	if (nAlertType == ALERT_PLAYER_HP) // player HP
	{
		sprintf(szAlert, "ÿc8Player ÿc1lifeÿc8 down to ÿc1%d%%ÿc8!", iValue);
	}
	else if (nAlertType == ALERT_MERC) // merc
	{
		sprintf(szAlert, "ÿc8Merc ÿc1lifeÿc8 down to ÿc1%d%%ÿc8!", iValue);
	}
	else if (nAlertType == ALERT_PLAYER_MANA) // mana
	{
		sprintf(szAlert, "ÿc8Player ÿc3manaÿc8 down to ÿc1%d%%ÿc8!", iValue);
	}
	else
	{
		return FALSE;
	}

	server->GamePrintInfo(szAlert);

	if (Me.IsPlayerInTown())
	{
		server->GamePrintInfo("ÿc4Player in town, no danger.");
		bAlert = FALSE;
	}

	return bAlert;
}

int Str2Method(LPCTSTR lpsz, BOOL bAllowPotion)
{
	if (lpsz == NULL)
		return TR_NONE;

	if (stricmp(lpsz, "quit") == 0)
		return TR_QUIT;

	if (stricmp(lpsz, "town") == 0)
		return TR_TOWN;

	if (bAllowPotion && stricmp(lpsz, "potion") == 0)
		return TR_POTION;

	return TR_NONE;
}

//////////////////////////////////////////////////////////////////////
// Determine if "bind.d2h" and "bind.ini" are present,
// Shortcut keys will not work without "bind"
//////////////////////////////////////////////////////////////////////
BOOL IsBindPresent()
{
	char szFile[_MAX_PATH] = "";
	sprintf(szFile, "%s\\%s", GetModuleDirectory(), "bind.d2h");
	DWORD dwRet = ::GetFileAttributes(szFile);
	if (dwRet == -1 || dwRet == FILE_ATTRIBUTE_DIRECTORY)
	{
		server->GamePrintError("Module 'Bind' isn't present, cannot assign shortcut keys.");
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Find the pre-defined shortcut keys in "bind.ini"
//////////////////////////////////////////////////////////////////////
void SearchBindKeys(BYTE& iQuitKey, BYTE& iSyncKey, BYTE& iTownKey)
{
	// reset whatsoever
	iQuitKey = 0x00;
	iSyncKey = 0x00;
	iTownKey = 0x00;

	// not good if bind isn't even present
	if (!IsBindPresent())
		return;

	// search through "bind.ini" and find the following
	// binded commands:
	// "000;hc quit" | "000;hc sync" | "000;hc town"
	char szEntry[32] = ""; // ini entry name
	char szVal[128] = ""; // string value read from the ini, 127 should be enough

	for (BYTE i = 0x01; i < 0xff; i++)
	{
		// all three keys have been found, search can stop now
		if (iQuitKey > 0x00 && iSyncKey > 0x00 && iTownKey > 0x00)
			return;
		
		sprintf(szEntry, "Keycode%02X", i);
		g_bind_ini.GetString("Default", szEntry, szVal, 127);

		// string compare
		// format of szVal should like "000;hc xxxx"
		if (iQuitKey == 0x00
			&& _stricmp(szVal, "000;hc quit") == 0)
		{
			iQuitKey = i; // the quit key found
		}
		else if (iSyncKey == 0x00
			&& _stricmp(szVal, "000;hc sync") == 0)
		{
			iSyncKey = i; // the sync key found
		}
		else if ( iTownKey == 0x00
			&& _stricmp(szVal, "000;hc town") == 0)
		{
			iTownKey = i; // the town key found
		}
		else
		{
			// nothing in this round
		}
	}
}