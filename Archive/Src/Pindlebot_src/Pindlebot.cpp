///////////////////////////////////////////////////////////////////
// Pindlebot.cpp
//
// Global stuff and d2hackit template.
//
// 2003 Abin (abinn32@163.com)
///////////////////////////////////////////////////////////////////

#include "Pindlebot.h"
#pragma comment(lib, "..\\D2lib\\D2Lib.lib") // Import D2Lib
#include "..\\D2Lib\\ClientCore.cpp"

BOOL CALLBACK OnHotkey(BYTE iVkCode, LPCTSTR lpszCommand)
{
	if (lpszCommand && lpszCommand[0] != '\0')
		server->GameCommandLine((LPTSTR)lpszCommand);
	return TRUE;
}

// Message handler for player
void CALLBACK OnPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	switch (nMessage)
	{
	case PM_DEATH: // Player died
		FinishRun(FR_CHARDEAD, FALSE);
		break;

	case PM_HPDOWN: // HP decreased
		if (Me.GetHPPercent() < g_config.iQuitHP)
		{
			// quit
			FinishRun(FR_INDANGER, TRUE);
		}
		else if (Me.GetHPPercent() < g_config.iPotionHP)
		{
			// drink potioin
			if (Me.HealMe())
			{
				server->GamePrintInfo("ÿc2Drank a potion.");
				g_logIni.IncreaseInt("Log", "Potion");
			}
			else
			{
				server->GamePrintError("Unable to drink any potion!");
			}
		}
		break;

	case PM_MANADOWN: // mana decreased

		if (!Me.IsPlayerInTown())
		{
			if (Me.GetManaPercent() < g_config.iQuitMana)
			{
				// quit
				FinishRun(FR_INDANGER, TRUE);
			}
			else if (Me.GetManaPercent() < g_config.iPotionMana)
			{
				// drink potion
				if (Me.SupplyMana())
				{
					server->GamePrintInfo("ÿc2Drank a potion.");
					g_logIni.IncreaseInt("Log", "Potion");
				}
				else
				{
					server->GamePrintError("Unable to drink any potion!");
				}
			}
		}
		
		break;

	case PM_LEAVETOWN: // Player left town
		Me.SetRadius(5);
		break;

	case PM_MOVECOMPLETE: // move/npc interaction finished
	case PM_NPCEND:		
		if (wParam) // succeeded?
			UpdateBotState(g_nNextState);
		g_ticker.ResetTicker(0);
		break;

	case PM_EQUIPEVENT: // Item equiped/unequiped
		if (g_nCurState == MS_WAIT_AMMO && (wParam == ITEM_ACTION_TO_EQUIP || wParam == ITEM_ACTION_TO_STORAGE))
		{
			// if arrow/bolt equipped, means we bought arrow/bolt from Larzuk successfully
			LPCITEM lpItem = (LPCITEM)lParam;
			if (lpItem)
			{
				if (stricmp(lpItem->szItemCode, "aqv") == 0 || stricmp(lpItem->szItemCode, "cqv") == 0)
				{
					Me.OverHead("ÿc4Ammo refilled successfully");
					UpdateBotState(MS_LEAVE_LARZUK);
				}
			}
		}
		break;

	case PHM_ITEMDROP: // sent by CPickitPlayer
		if (!Me.IsPlayerInTown() && lParam)
			server->GamePrintInfo((LPCSTR)lParam);
		break;

	case PM_INVENTORYFULL: // Inventory full, next game IDBot
		g_bInventoryFull = TRUE;
		break;

	default:
		break;
	}
}

// Message handler for pet
void CALLBACK OnPetMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	switch (nMessage)
	{
	case MM_HPPERCENTDOWN: // pet HP down
		if ((BYTE)lParam < g_config.iPetPotionHP)
		{
			if (Me.HealMerc())
			{
				server->GamePrintInfo("ÿc2Fed your merc a potion.");
				g_logIni.IncreaseInt("Log", "Potion Merc");
			}
			else
			{
				server->GamePrintError("Unable to feed your merc any potion!");
			}
		}
		break;

	case MM_DEATH: // Pet died
		server->GamePrintInfo("Merc has been ÿc1killedÿc0.");
		if (g_config.bQuitOnPetDeath)
			FinishRun(FR_MERCDEAD, TRUE);
		g_logIni.IncreaseInt("Log", "Merc die");
		break;

	default:
		break;
	}
}

// Message handler for pindleskin
void CALLBACK OnPindleskinMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	char szMsg[256] = "";
	const WORD* pBonuses = NULL;

	switch (nMessage)
	{
	case PSM_BONUSES: // Pindleskin bonuses detected		
		CheckPindleAttrs();
		break;

	case MM_HPPERCENTUP: // Pindleskin HP changed
	case MM_HPPERCENTDOWN:
		sprintf(szMsg, "ÿc4Pindleskin HP: ÿc2%d%%", Pindleskin.GetHPPercent());
		Me.OverHead(szMsg);
		break;

	case MM_CORPSE: // Pindleskin's corpse appeared
		// pindle dead
		server->GamePrintInfo("ÿc2Pindleskinÿc0 has been killed.");
		UpdateBotState(MS_PINDLE_DEAD, 500);
		break;

	default:
		break;
	}
}

void UpdateBotState(int nNewState, DWORD dwTicker)
{
	g_nCurState = nNewState;
	g_ticker.ResetTicker(dwTicker);
}

void WaitForMove(int nNextState)
{
	g_nNextState = nNextState;
	UpdateBotState(MS_VERIFY_STEP, 30000);
}

void WaitForNPC(int nNextState)
{
	g_nNextState = nNextState;
	UpdateBotState(MS_VERIFY_NPC, 30000);
}

// Check if Malah is in our way
BOOL MalahNotInTheWay()
{ 
	return ((Malah.GetPositionY() <= 0x13a7)
		|| (Malah.GetPositionY() <= Me.GetPositionY() - 2));
}

DWORD ProcessCorpose()
{	
	POSITION pos = g_aCorposes.GetHeadPosition();
	if (g_config.iOnCorpseCount == 0 || pos == NULL)
		return 0;

	const MONSTERCORPSE& mc = g_aCorposes.GetNext(pos);

	// if there are more than 1 skills in this scheme, randomly pick 1
	int nIndex = rand() % g_config.iOnCorpseCount;
	WORD wSpellID = g_config.aOnCorpse[nIndex].wSpellID;

	if (!Me.HasSpell(wSpellID))
		return 0;

	// cast the skill on the first corpose(first die first service, LOL)
	if (Me.CastSpell(wSpellID, g_config.aOnCorpse[nIndex].iLeft, &mc))
	{
		g_aCorposes.RemoveFront();
		return g_config.aOnCorpse[nIndex].wDelay + HARDDELAY;
	}

	return 0;
}

BOOL NeedResMerc()
{
	return (g_config.bResurrectPet
		&& !Pet.IsValidObject()
		&& Me.GetPlayerStats(D2PS_GOLD) + Me.GetPlayerStats(D2PS_GOLDBANK) >= 50000);
}

BOOL NeedRefillAmmo()
{
	return (g_config.nAmmoType != PB_AMMO_NONE && Me.GetPlayerStats(D2PS_AMMOQUANTITY) == 0);
}

void RefillAmmo()
{
	// Buy arrow/bolt from Larzuk
	if (!Larzuk.IsValidObject())
		return;
	
	Me.OverHead("ÿc4Refilling ammo.");
	if (g_config.nAmmoType == PB_AMMO_ARROW)
	{
		Me.BuyItem(g_dwArrowID, &Larzuk);
	}
	else if (g_config.nAmmoType == PB_AMMO_BOLT)
	{
		Me.BuyItem(g_dwBoltID, &Larzuk);
	}
	else
	{
	}
}

void CalcPindleRoute()
{
	// Calculate the route from red portal to pindleskin
	g_aPindleRoute.RemoveAll();
	D2POSITION lc = {0};

	if (Me.HasSpell(D2S_TELEPORT))
	{		
		lc.x = RedPortal.GetPositionX() - 5;
		lc.y = RedPortal.GetPositionY() - 30;
		g_aPindleRoute.Add(lc);

		lc.x = RedPortal.GetPositionX() - 7;
		lc.y = RedPortal.GetPositionY() - 57;
		g_aPindleRoute.Add(lc);

	}
	else
	{
		lc.x = RedPortal.GetPositionX() - 7;
		lc.y = RedPortal.GetPositionY() - 14;
		g_aPindleRoute.Add(lc);

		lc.x = RedPortal.GetPositionX() - 7;
		lc.y = RedPortal.GetPositionY() - 34;
		g_aPindleRoute.Add(lc);

		lc.x = RedPortal.GetPositionX() - 7;
		lc.y = RedPortal.GetPositionY() - 51;
		g_aPindleRoute.Add(lc);
	}

	// adjust the last step	
	if (g_config.nLocationType == PB_LOCATION_CLOSE)
	{
		lc.x = RedPortal.GetPositionX() - 11;
		lc.y = RedPortal.GetPositionY() - 76;
	}
	else if (g_config.nLocationType == PB_LOCATION_FAR)
	{
		lc.x = RedPortal.GetPositionX() - 11;
		lc.y = RedPortal.GetPositionY() - 68;
	}
	else // normal
	{
		lc.x = RedPortal.GetPositionX() - 11;
		lc.y = RedPortal.GetPositionY() - 72;
	}
	g_aPindleRoute.Add(lc);	
}

// perform an action
int PerformAction(const PBOTACTION& action, BYTE iTime)
{
	if (iTime >= action.iTimes)
		return SCR_NOMORE;

	D2SPELLINFO dsi = { 0 };
	if (!::D2GetSpellInfo(action.wSpellID, &dsi))
	{
		server->GamePrintError("invalid spell");
		return SCR_INVALIDSKILL;
	}

	if (!Me.HasSpell(action.wSpellID))
	{
		::D2Errorf("Skill ÿc7%sÿc0 is unavailable to this player.", dsi.szSpellName);
		SIGPostString("·Ç·¨¼¼ÄÜ");
		return SCR_INVALIDSKILL;
	}
	
	if (g_config.nAmmoType != PB_AMMO_NONE && Me.GetPlayerStats(D2PS_AMMOQUANTITY) == 0)
	{
		server->GamePrintError("Out of ammo");
		SIGPostString("µ¯Ò©ÓÃÍê");
		return SCR_OUTOFAMMO;
	}

	char szSpell[256] = "";
	sprintf(szSpell, "ÿc4Casting: ÿc2%s(%d)", dsi.szSpellName, iTime + 1);
	Me.OverHead(szSpell);
	sprintf(szSpell, "Ê¹ÓÃ¼¼ÄÜ: %s(%d)", dsi.szSpellName, iTime + 1);
	SIGPostString(szSpell);

	WORD DIFX = (Me.GetPositionX() > Pindleskin.GetPositionX()) ? (Me.GetPositionX() - Pindleskin.GetPositionX()) : (Pindleskin.GetPositionX() - Me.GetPositionX());
	WORD DIFY = (Me.GetPositionY() > Pindleskin.GetPositionY()) ? (Me.GetPositionY() - Pindleskin.GetPositionY()) : (Pindleskin.GetPositionY() - Me.GetPositionY());
	WORD wTargetX = 0;
	WORD wTargetY = 0;
	
	if (action.iTarget == PB_TARGET_NONE)
	{
		Me.CastSpell(action.wSpellID, action.iLeft);
	}
	else if (action.iTarget == PB_TARGET_FRONT)
	{
		wTargetX = (Me.GetPositionX() > Pindleskin.GetPositionX()) ? (Pindleskin.GetPositionX() + DIFX / 2) : (Pindleskin.GetPositionX() - DIFX / 2);
		wTargetY = (Me.GetPositionY() > Pindleskin.GetPositionY()) ? (Pindleskin.GetPositionY() + DIFY / 2) : (Pindleskin.GetPositionY() - DIFY / 2);
		Me.CastSpell(action.wSpellID, action.iLeft, wTargetX, wTargetY);
	}
	else if (action.iTarget == PB_TARGET_BEHIND)
	{
		DIFX = max(5, DIFX);
		DIFX = min(15, DIFX);
		DIFY = max(5, DIFY);
		DIFY = min(15, DIFY);
		wTargetX = (Me.GetPositionX() > Pindleskin.GetPositionX()) ? (Pindleskin.GetPositionX() - DIFX) : (Pindleskin.GetPositionX() + DIFX);
		wTargetY = (Me.GetPositionY() > Pindleskin.GetPositionY()) ? (Pindleskin.GetPositionY() - DIFY) : (Pindleskin.GetPositionY() + DIFY);
		Me.CastSpell(action.wSpellID, action.iLeft, wTargetX, wTargetY);
	}
	else
	{
		// by default target directly on pindleskin
		Me.CastSpell(action.wSpellID, action.iLeft, &Pindleskin);
	}

	return SCR_SUCCEEDED;
}

void ProcessActions(int nType)
{
	int nNextStep = 0;	
	LPCPBOTACTION pActions = NULL;
	BYTE iActionCount = 0;

	switch (nType)
	{
	case PB_ACTION_BEFORERED:
		nNextStep = MS_TALK_PORTAL;
		iActionCount = g_config.iBeforeRedCount;
		pActions = g_config.aBeforeRed;
		break;

	case PB_ACTION_INRED:
		nNextStep = MS_APPROACH_PINDLESKIN;
		iActionCount = g_config.iInRedCount;
		pActions = g_config.aInRed;
		break;

	case PB_ACTION_ONPINDLE:
		nNextStep = MS_FINISHED;
		iActionCount = g_config.iOnPindleCount;
		pActions = g_config.aOnPindle;
		break;

	default:
		return;
		break;
	}

	if (g_iActionIndex < iActionCount)
	{
		int nRes = PerformAction(pActions[g_iActionIndex], g_iSpellCast);
		if (nRes == SCR_NOMORE)
		{
			g_iSpellCast = 0;
			g_iActionIndex++;
		}
		else if (nRes == SCR_SUCCEEDED)
		{
			g_iSpellCast++;
			g_ticker.ResetTicker(pActions[g_iActionIndex].wDelay + HARDDELAY);
		}
		else if (nRes == SCR_OUTOFAMMO)
		{			
			FinishRun(FR_FAILED, FALSE);
		}
		else
		{
			g_iSpellCast = 0xff;
			g_iActionIndex++;
		}
	}
	else
	{		
		if (nType == PB_ACTION_ONPINDLE)
			FinishRun(FR_OUTSKILL, TRUE);
		else
			g_nCurState = nNextStep;
	}
}

BOOL CheckPindleAttrs()
{
	// Check for pindleskin's bonuses, if dedly combo found, we quit
	DWORD dwBonuses = ::PBotFindCombo(Pindleskin.GetBonuses(), &g_config);
	if (dwBonuses)
	{
		char szMessage[256] = "";
		char szBonus1[128] = "";
		char szBonus2[128] = "";
		WORD w1 = LOWORD(dwBonuses);
		WORD w2 = HIWORD(dwBonuses);
		::PBotGetBonusName(w1, szBonus1, 127, FALSE);
		::PBotGetBonusName(w2, szBonus2, 127, FALSE);
		if (w1 == 0)
		{
			::D2Infof("Deadly Combo: ÿc1%s", szBonus2);
			sprintf(szMessage, "%s", szBonus2);
		}
		else if (w2 == 0)
		{
			::D2Infof("Deadly Combo: ÿc1%s", szBonus1);
			sprintf(szMessage, "%s", szBonus1);
		}
		else
		{
			::D2Infof("Deadly Combo: ÿc1%s ÿc0+ ÿc1%s", szBonus1, szBonus2);
			sprintf(szMessage, "%s + %s", szBonus1, szBonus2);
		}
		
		g_logIni.IncreaseInt("Deadly Combos", szMessage);
		FinishRun(FR_DEADLYCOMBO, FALSE);
		return TRUE;
	}
	return FALSE;
}

void ShowCharSet()
{
	// displays some player info
	server->GamePrintInfo(" ");
	server->GamePrintInfo("ÿc7-------------------------------------------");
	server->GamePrintInfo(" ");
	::D2Infof("Player HP: ÿc2%d/%d (%d%%)", Me.GetPlayerStats(D2PS_HP), Me.GetPlayerStats(D2PS_MAXHP), Me.GetHPPercent());
	::D2Infof("Player Mana: ÿc2%d/%d (%d%%)", Me.GetPlayerStats(D2PS_MANA), Me.GetPlayerStats(D2PS_MAXMANA), Me.GetManaPercent());
	::D2Infof("Item Durability: ÿc2%d/%d (%d%%)", Me.GetPlayerStats(D2PS_DURABILITY), Me.GetPlayerStats(D2PS_MAXDURABILITY), Me.GetItemDurabilityPercent());
	if (g_config.nAmmoType != PB_AMMO_NONE)
		::D2Infof("Ammo Quantity: ÿc2%d", Me.GetPlayerStats(D2PS_AMMOQUANTITY));
/*
	if (g_config.iMalahHP)
		::D2Infof("Malah HP: ÿc2%d%%", g_config.iMalahHP);
	else
		server->GamePrintInfo("Malah HP: ÿc1Disabled");

	if (g_config.iMalahMana)
		::D2Infof("Malah Mana: ÿc2%d%%", g_config.iMalahMana);
	else
		server->GamePrintInfo("Malah Mana: ÿc1Disabled");

	if (g_config.iPotionHP)
		::D2Infof("Potion HP: ÿc2%d%%", g_config.iPotionHP);
	else
		server->GamePrintInfo("Potion HP: ÿc1Disabled");

	if (g_config.iPotionMana)
		::D2Infof("Potion MP: ÿc2%d%%", g_config.iPotionMana);
	else
		server->GamePrintInfo("Potion MP: ÿc1Disabled");

	if (g_config.iQuitHP)
		::D2Infof("Retreat HP: ÿc2%d%%", g_config.iQuitHP);
	else
		server->GamePrintInfo("Retreat HP: ÿc1Disabled");

	if (g_config.iQuitMana)
		::D2Infof("Retreat Mana: ÿc2%d%%", g_config.iQuitMana);
	else
		server->GamePrintInfo("Retreat Mana: ÿc1Disabled");

	if (g_config.iPetPotionHP)
		::D2Infof("Pet Potion HP: ÿc2%d%%", g_config.iPetPotionHP);
	else
		server->GamePrintInfo("Pet Potion HP: ÿc1Disabled");

	if (g_config.bQuitOnPetDeath)
		server->GamePrintInfo("Quit If Pet Dies: ÿc2Yes");
	else
		server->GamePrintInfo("Quit If Pet Dies: ÿc1No");

	if (g_config.iRepairPercent)
		::D2Infof("Repair Durability: ÿc2%d%%", g_config.iRepairPercent);
	else
		server->GamePrintInfo("Repair Durability: ÿc1Disabled");
*/

	server->GamePrintInfo("ÿc7-------------------------------------------");
	server->GamePrintInfo(" ");
}

void FinishRun(int nReason, BOOL bRunAway)
{	
	// prepare to quit current game
	g_bPaused = FALSE;
	UpdateBotState(MS_FINISHED);	
	switch (nReason)
	{
	case FR_SUCCEEDED:
		server->GamePrintInfo("Game finished by reason: ÿc2succeeded.");
		SIGPostString("ÓÎÏ·½áÊø£ºÁÔÉ±³É¹¦");
		g_logIni.IncreaseInt("Log", "Succeed");
		break;

	case FR_INDANGER:
		//if (bRunAway)
		//{
			// Immediately run away from Pindleskin
			//Me.RunTo(ADJ_RETREAT.x, ADJ_RETREAT.y, TRUE, TRUE);
		//}
		server->GamePrintInfo("Game finished by reason: ÿc1retreating.");
		SIGPostString("ÓÎÏ·½áÊø£º½ÇÉ«³·ÍË");
		g_logIni.IncreaseInt("Log", "Retreat");
	//	g_logIni.IncreaseInt("Retreat", szMsg);
		break;

	case FR_CHARDEAD:

		server->GamePrintInfo("Game finished by reason: ÿc1character slain.");
		SIGPostString("ÓÎÏ·½áÊø£º½ÇÉ«ËÀÍö :(");
		BYTE aPacket[1];
		aPacket[0] = 0x41;
		server->GameSendPacketToServer(aPacket, 1);
		g_logIni.IncreaseInt("Log", "Char die");
	//	g_logIni.IncreaseInt("Char Dead", szMsg);
		break;

	case FR_FAILED:
		server->GamePrintInfo("Game finished by reason: ÿc1failed.");
		SIGPostString("ÓÎÏ·½áÊø£ºÔËÐÐÔÝÊ±Ê§°Ü");
		g_logIni.IncreaseInt("Log", "Fail");
		break;

	case FR_REPAIR:
		server->GamePrintInfo("Game finished by reason: ÿc7repair.");
		SIGPostString("ÓÎÏ·½áÊø£ºÐÞÀíÎïÆ·");
		g_logIni.IncreaseInt("Log", "Repair");
		break;

	case FR_MERCDEAD:
		//if (bRunAway)
		//{
			// Immediately run away from Pindleskin
			//Me.RunTo(ADJ_RETREAT.x, ADJ_RETREAT.y, TRUE, TRUE);
		//}
		server->GamePrintInfo("Game finished by reason: ÿc1merc death.");
		SIGPostString("ÓÎÏ·½áÊø£º¹ÍÓ¶±øËÀÍö");	
		g_logIni.IncreaseInt("Log", "Retreat");
		break;

	case FR_DEADLYCOMBO:
		//server->GamePrintInfo("Deadly Combo:");
		//SIGPostString(szMsg);
		g_logIni.IncreaseInt("Log", "Avoid");
	//	g_logIni.IncreaseInt("Avoid", szMsg);
		break;

	case FR_OUTSKILL:
		server->GamePrintInfo("Game finished by reason: ÿc1insufficient skills.");
		SIGPostString("ÓÎÏ·½áÊø£º¼¼ÄÜ²»×ã");
		g_logIni.IncreaseInt("Log", "Ins Skill");
	//	g_logIni.IncreaseInt("Ins Skill", szMsg);
		break;

	case FR_WARPWP:
		server->GamePrintInfo("Game finished by reason: ÿc2WP warping.");
		SIGPostString("ÓÎÏ·½áÊø£º×ª»»³¡¾°");
		g_logIni.IncreaseInt("Log", "Warp");
		break;

	case FR_IDBOT:
		server->GamePrintInfo("Game finished by reason: ÿc2ID items.");
		SIGPostString("ÓÎÏ·½áÊø£ºÕûÀíÎïÆ·");
		g_logIni.IncreaseInt("Log", "ID items");
		break;

	default:		
		break;
	}

	UpdateBotState(MS_PREPARETOQUIT);
}

BOOL EXPORT OnClientStart()
{
	srand((unsigned)time(NULL));

	// Set D2Lib object fingerprinters
	Malah.SetFingerprinter(&FP_NPC_MALAH);
	Larzuk.SetFingerprinter(&FP_NPC_LARZUK);
	Qual.SetFingerprinter(&FP_NPC_QUAL);
	RedPortal.SetFingerprinter(&FP_OBJ_ACT5REDPORTAL);
	Act1Wp.SetFingerprinter(&FP_OBJ_ACT1WP);
	Cain.SetFingerprinter(&FP_NPC_ACT5CAIN);
	
	// Specifies D2 message handlers
	Me.SetD2MessageProc(OnPlayerMessage);
	Pet.SetD2MessageProc(OnPetMessage);	
	Pindleskin.SetD2MessageProc(OnPindleskinMessage);

	//debug
	//Me.SetDebug(TRUE);
	//Larzuk.SetDebug(TRUE);
	//Pindleskin.SetDebug(TRUE);
	//RedPortal.SetDebug(TRUE);
	//Qual.SetDebug(TRUE);
	//Cain.SetDebug(TRUE);

	// Intercept key presses
	g_keyBind.InstallKeyBind();
	g_keyBind.SetKeyBindProc(OnHotkey);
	g_keyBind.SetEntry(VK_F9, 0, "pindlebot statistics");
	g_keyBind.SetEntry(VK_F10, 0, "pindlebot stop");
	g_keyBind.SetEntry(VK_F11, 0, "pindlebot pause");
	g_keyBind.SetEntry(VK_F12, 0, "pindlebot resume");

	char szPath[_MAX_PATH + 1] = "";
	sprintf(szPath, "%s\\Pindlebot.txt", GetModuleDirectory());
	g_logIni.SetPathName(szPath);

	sprintf(szPath, "%s\\Pickit.log", GetModuleDirectory());
	Me.SetLogFilePath(szPath);

	sprintf(szPath, "%s\\pickit.ini", GetModuleDirectory());
	Me.LoadConfig(szPath);
	Me.SetAllowTownPick(FALSE);

	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	g_keyBind.UninstallKeyBind();
	return TRUE;
}

void EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	thisgame = game;
	g_dwTotalTime = 0;
	g_bPaused = FALSE;		
	g_nCurState = MS_NONE;
	g_nNextState = MS_NONE;
	g_nTotalGame++;
	g_aPindleRoute.RemoveAll();
	g_nPortalRetry = 0;
	g_bStop = TRUE;
	g_aCorposes.RemoveAll();
	g_logIni.IncreaseInt("Log", "Game");

	Me.OnGameJoin(game);
	Pet.OnGameJoin(game);
	Pindleskin.OnGameJoin(game);
	Cain.OnGameJoin(game);
	Malah.OnGameJoin(game);
	Larzuk.OnGameJoin(game);
	Qual.OnGameJoin(game);
	RedPortal.OnGameJoin(game);
	Act1Wp.OnGameJoin(game);	
	
	Me.SetRadius(0);
	Me.SetSpecialItemMethod(PICKIT_SPECIAL_GOLD, PICKIT_PICK_AUTO);
	Me.SetSpecialItemMethod(PICKIT_SPECIAL_REJUV, PICKIT_PICK_AUTO);
	Me.SetSpecialItemMethod(PICKIT_SPECIAL_FULLREJUV, PICKIT_PICK_AUTO);

	char szConfig[MAX_PATH + 1] = "";

	// is sig.dll there?
	sprintf(szConfig, "%s\\sig.dll", GetModuleDirectory());
	g_bUseSig = ::GetFileAttributes(szConfig) != -1; 

	// load bot configuration for current char	
	sprintf(szConfig, "%s\\pindlebot\\%s.bot", GetModuleDirectory(), game->player->PlayerName);
	::memset(&g_config, 0, sizeof(PBOTCONFIG));
	if (!::PBotLoadConfig(szConfig, &g_config))
	{
		char sMsg[128] = "";
		::D2Errorf("Cannot open file ÿc7%sÿc0, game stopped.", szConfig);
		server->GamePrintError(sMsg);
		OnGameCommandStop(NULL, 0);
		sprintf(sMsg, "ÎÞ·¨´ò¿ª%s", szConfig);
		SIGPostString(sMsg);
		return;
	}

	sprintf(szConfig, "%s\\Pindlebot.ini", GetModuleDirectory());
	CIni ini(szConfig);
	g_dwIdleLength = ini.GetUInt("Default", "Idle Length", 300) * 1000;
	
	server->GamePrintInfo("ÿc4Pindlebot Hotkeys");
	server->GamePrintInfo("Show Stats: ÿc2F9");
	server->GamePrintInfo("Stop: ÿc2F10");
	server->GamePrintInfo("Pause: ÿc2F11");
	server->GamePrintInfo("Resume: ÿc2F12");
	OnGameCommandStart(NULL, 0);
}

void EXPORT OnGameLeave(THISGAMESTRUCT* game)
{
	thisgame = NULL;
	Me.OnGameLeave();
	Pet.OnGameLeave();
	Pindleskin.OnGameLeave();
	Cain.OnGameLeave();
	Malah.OnGameLeave();
	Larzuk.OnGameLeave();
	Qual.OnGameLeave();
	RedPortal.OnGameLeave();
	Act1Wp.OnGameLeave();
	
	g_nCurState = MS_NONE;
	g_nNextState = MS_NONE;
	g_bStop = TRUE;	
	g_bPaused = FALSE;

	::memset(&g_config, 0, sizeof(PBOTCONFIG));
	g_logIni.WriteBool("Log", "Running", FALSE);
	if (g_dwTotalTime > 0)
	{
		g_logIni.IncreaseInt("Log", "Run Time", g_dwTotalTime / 1000);
		g_dwTotalTime = 0;
	}
}

BOOL PRIVATE OnGameCommandStart(char** argv, int argc)
{
	BOOL bOK = FALSE;
	if (g_nCurState != MS_NONE) // aleady started
	{
		server->GamePrintInfo("This run is started already.");
		return TRUE;
	}

	server->GamePrintInfo("Starting Pindlebot ...");
	SIGPostString("ÕýÔÚÆô¶¯Pindlebot...");	
	g_bStop = FALSE;
	g_bPaused = FALSE;
	Me.PauseJobs(FALSE);
	UpdateBotState(MS_INIT_START, 1500);
	return TRUE;
}

BOOL PRIVATE OnGameCommandStop(char** argv, int argc)
{
	SIGStop(); // stop sig
	g_logIni.WriteBool("Log", "Running", FALSE);
	if (g_dwTotalTime > 0)
	{
		g_logIni.IncreaseInt("Log", "Run Time", g_dwTotalTime / 1000);
		g_dwTotalTime = 0;
	}

	g_bStop = TRUE;
	g_bPaused = FALSE;
	Me.PauseJobs(FALSE);
	Me.CleanJobs();

	// show statistics before stopping
	OnGameCommandStatistics(NULL, 0);
	server->GamePrintInfo("Pindlebot ÿc2stopped.");
	SIGPostString("PindlebotÒÑÍ£Ö¹");
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
	Me.OnGameTimerTick();
	Pet.OnGameTimerTick();
	Pindleskin.OnGameTimerTick();
	Cain.OnGameTimerTick();
	Malah.OnGameTimerTick();
	Larzuk.OnGameTimerTick();
	Qual.OnGameTimerTick();
	RedPortal.OnGameTimerTick();
	Act1Wp.OnGameTimerTick();	

	DWORD dwTemp = 0;

	// quit game, highest priority
	if (g_nCurState == MS_PREPARETOQUIT)
	{
		// prepare to quit current game
		g_bPaused = FALSE;
		UpdateBotState(MS_FINISHED);
		Me.LeaveGame();
		return 0;
	}

	if (!Me.IsGameReady())
		return 0;
	
	if (g_nCurState == MS_PICKITEMS_OK && Me.GetQueueItemCount(TRUE) == 0)
	{
		server->GamePrintInfo("No more valuable items queued.");
		FinishRun(FR_SUCCEEDED, FALSE);
		return 0;
	}

	g_dwTotalTime += 100;
	g_ticker.TickIt(100);

	if(g_bPaused
		||g_nCurState == MS_NONE
		|| g_nCurState == MS_FINISHED
		|| !g_ticker.IsTickEnded()
		|| g_bStop)
	{
		return 0;
	}

	if (g_dwTotalTime > g_dwIdleLength && g_dwTotalTime - g_dwIdleLength > 180000)
	{
		FinishRun(FR_FAILED, FALSE);
		g_ticker.ResetTicker();
		return 0;
	}

	// check for states
	switch (g_nCurState)
	{
	case MS_VERIFY_STEP:
	case MS_VERIFY_NPC:
		server->GamePrintError("step failed");
		FinishRun(FR_FAILED, FALSE); // Move or NPC interaction failed, quit
		break;

	case MS_INIT_START:
		
		if (Me.GetCorpseCount())
		{
			// if there's a corpse, pick it up first
			Me.OverHead("ÿc4Picking up corpse");
			SIGPostString("ÕýÔÚÖØÐÂ×°±¸");
			server->GamePrintInfo("Player corpse detected!");
			Me.PickupCorpse();
			g_ticker.ResetTicker(5000);
			break;
		}		

		// if idling, display remaining seconds
		if (g_dwTotalTime < g_dwIdleLength)
		{
			if ((g_dwTotalTime % 1000) == 0)
			{
				char szMsg[128] = "";
				sprintf(szMsg, "ÿc4Pindlebot idles for ÿc2%u ÿc4seconds...", (g_dwIdleLength - g_dwTotalTime) / 1000);
				Me.OverHead(szMsg);
				SIGPostString("Pindlebot Idling...");
			}

			if ((g_dwTotalTime % 10000) == 0)
				Me.AntiIdle();

			return 0;
		}

		// if not at act5...
		if (Me.GetCurrentAct() != 0 && Me.GetCurrentAct() != 4) // neither act5 nor act1...
		{
			SIGStop();
			server->GamePrintError("Character must be at either ÿc2Act5 or act1 initial locationÿc0 to start.");
			OnGameCommandStop(NULL, 0);
			SIGPostString("½ÇÉ«±ØÐëÎ»ÓÚact1»òact5");
			return 0;	
		}

		// start the bot
		ShowCharSet();
		Me.OverHead("ÿc4Pindlebot ÿc2started");
		//ShowCharSet();
		server->GamePrintInfo("Pindlebot ÿc2started.");
		SIGPostString("PindlebotÒÑÆô¶¯");

		// if this is a Paladin, activate Vigor
		Me.SelectSpell(D2S_VIGOR, FALSE);

		// if this is an Assassin, activate Burst of speed
		Me.CastSpell(D2S_BURSTOFSPEED, FALSE);

		// if is at act1, goto act5
		if (Me.GetCurrentAct() == 0)
		{
			if (Act1Wp.IsValidObject())
			{
				Me.OverHead("ÿc4Detecting WP coordinates");
				SIGPostString("ÕýÔÚÑ°ÕÒWPÂ·¾¶");

				if (Act1Wp.GetPositionX() < Me.GetPositionX())
					Me.RunTo(Me.GetPositionX(), Me.GetPositionY() - 16);
				else
					Me.RunTo(Me.GetPositionX() + 26, Me.GetPositionY() - 6);

				Me.RunTo(&Act1Wp);
				WaitForMove(MS_OPEN_WP);
			}
			else
			{
				FinishRun(FR_FAILED, FALSE);
			}
		}

		// if need to repair items or refill ammo, travel to larzuk
		else if ((Me.FindPlayerStats(D2PS_DURABILITY) && Me.GetItemDurabilityPercent() < g_config.iRepairPercent) || NeedRefillAmmo())
		{
			// walk to Larzuk
			UpdateBotState(MS_TRAVEL_LARZUK);
		}
		else if (g_bInventoryFull)
		{
			// walk to cain for identifying			
			UpdateBotState(MS_TRAVEL_CAIN);
		}		
		else
		{
			// run as normal
			UpdateBotState(MS_ADJUST_LOCATION);
		}				
			
		break;

	case MS_OPEN_WP:
		// Open wp screen
		Me.OverHead("ÿc4Opening WP...");
		Me.OpenStaticObject(&Act1Wp);
		UpdateBotState(MS_WARP_ACT5, 3000);
		break;

	case MS_WARP_ACT5:
		// go in the wp to act5
		Me.OverHead("ÿc4Taking WP to ÿc2Act5...");
		SIGPostString("ÒÆ¶¯µ½act5...");
		Me.EnterWP(&Act1Wp, MAP_A5_HARROGATH);
		UpdateBotState(MS_AFTER_WP, 6000);
		break;

	case MS_AFTER_WP:
		Me.OverHead("ÿc4Warped to ÿc2Act5");
		SIGPostString("ÒÑ¾­µ½´ïact5");
		FinishRun(FR_WARPWP, FALSE);
		break;

	case MS_ADJUST_LOCATION:
		// to Malah?
		Me.OverHead("ÿc4Adjusting initial location");
		SIGPostString("È·¶¨ÆðÊ¼Â·¾¶");
		Me.RunTo(0x13d7, 0x13aa);
		if (Me.GetHPPercent() < g_config.iMalahHP || Me.GetManaPercent() < g_config.iMalahMana)
		{
			Me.RunTo(&Malah);
			WaitForMove(MS_TALK_MALAH);
		}
		else
		{
			WaitForMove(MS_AFTER_MALAH);				
		}
		break;

	case MS_TALK_MALAH:
		Me.OverHead("ÿc4Interacting with ÿc2Malah.");
		SIGPostString("ÕýÔÚÓëMalah½»Ì¸");
		Me.TalkToNpc(&Malah);
		WaitForNPC(MS_AFTER_MALAH);
		break;

	case MS_AFTER_MALAH:
		Me.LeaveNpc(&Malah);
		if (!MalahNotInTheWay())
		{
			// make a detour around malah			
			Me.OverHead("ÿc4Making a detour around ÿc2Malah...");
			SIGPostString("Malah×èµ²Â·¾¶£¬ÕýÔÚÈÆµÀ");
			Me.RunTo(0x13d2, 0x13a2);
		}

		Me.OverHead(_T("ÿc4Traveling to ÿc2Red Portal"));
		SIGPostString("×ßÏò´«ËÍÃÅ");
		Me.RunTo(0x13ce, 0x13aa); // after Malah
		Me.RunTo(0x13d9, 0x13c5); // downstairs
		Me.RunTo(0x13da, 0x13db); // T junction 
		WaitForMove(MS_JUNCTION);				
		break;

	case MS_JUNCTION:
		// do we need to res pet?
		Me.OverHead(_T("ÿc2Junction check"));
		if (NeedResMerc())
		{
			Me.OverHead("ÿc4Need to resurrect merc.");
			SIGPostString("ÐèÒª¸´»î¹ÍÓ¶±ø");
			Me.RunTo(0x13c8, 0x13d3); // approach Qual-Kehk
			Me.RunTo(&Qual);
			WaitForMove(MS_TALK_QUAL);
		}
		else
		{
			UpdateBotState(MS_AFTER_QUAL);
		}
		break;

	case MS_AFTER_QUAL:
		Me.OverHead(_T("ÿc4Traveling to ÿc2Red Portal"));
		SIGPostString("×ßÏò´«ËÍÃÅ");
		Me.RunTo(0x13da, 0x13db); // T junction;
		Me.RunTo(0x13e3, 0x1402); // ready to make turn
		Me.RunTo(0x13fe, 0x1400); // approach red portal
		//Me.RunTo(&RedPortal);
		WaitForMove(MS_CHECK_PORTAL);
		break;

	case MS_TALK_QUAL:
		Me.OverHead("ÿc4Interacting with ÿc2Qual-Kehk.");
		SIGPostString("ÓëQual-Kehk½»Ì¸");
		// code here to talk to Qual-Kehk
		Me.TalkToNpc(&Qual);
		WaitForNPC(MS_RES_MERC);
		break;

	case MS_RES_MERC:
		// ressurect pet
		if (NeedResMerc())
		{
			Me.OverHead("ÿc4Requesting to resurrect merc.");
			SIGPostString("ÇëÇó¸´»î¹ÍÓ¶±ø");
			Me.ResurrectMerc(&Qual);
		}
		UpdateBotState(MS_LEAVE_QUAL, 1000);
		break;

	case MS_LEAVE_QUAL:
		Me.OverHead("ÿc4Leaving ÿc2Qual-Kehk.");
		SIGPostString("Àë¿ªQual-Kehk");
		// code here to leave Qual-Kehk
		Me.LeaveNpc(&Qual);	
		UpdateBotState(MS_AFTER_QUAL, 1000);
		break;

	case MS_TRAVEL_LARZUK:
		// goto Larzuk for repairing or ammo refilling
		Me.OverHead("ÿc4Walking to ÿc2Larzuk.");
		SIGPostString("×ßÏòLarzuk");
		Me.RunTo(0x13f5, 0x13b4); // middle platform
		Me.RunTo(0x1407, 0x13b1); // down stairs
		Me.RunTo(0x1413, 0x13b5); // Larzuk's shop
		Me.RunTo(&Larzuk);
		WaitForMove(MS_TALK_LARZUK);			
		break;

	case MS_TALK_LARZUK:
		Me.OverHead("ÿc4Interacting with ÿc2Larzuk.");
		SIGPostString("ÓëLarzuk½»Ì¸");
		Me.TalkToNpc(&Larzuk);
		WaitForNPC(MS_ASK_REPAIR);
		break;	

	case MS_ASK_REPAIR:
		// repair all
		Me.OverHead("ÿc4Repair items");
		SIGPostString("ÐÞÀí×°±¸");
		Me.RepairItem(&Larzuk);

		if ((g_config.nAmmoType == PB_AMMO_ARROW || g_config.nAmmoType == PB_AMMO_BOLT)
			&& NeedRefillAmmo())
		{			
			UpdateBotState(MS_OPEN_STORE, 2500);
		}
		else
		{
			UpdateBotState(MS_LEAVE_LARZUK, 2500);
		}		
				
		break;

	case MS_OPEN_STORE:
		// open Larzuk store
		Me.OverHead("ÿc4Open NPC store");
		SIGPostString("´ò¿ªNPC»õ¹ñ");
		Me.OpenNpcStore(&Larzuk);
		Me.OpenNpcStore(&Larzuk);
		UpdateBotState(MS_VERIFY_STORE, 8000);
		break;

	case MS_VERIFY_STORE:
		// store opened?
		Me.OverHead("ÿc1Failed to open NPC store");
		FinishRun(FR_FAILED, FALSE);
		break;

	case MS_REFILL_AMMO:
		// refill ammo
		Me.OverHead("ÿc4Refill ammo");
		SIGPostString("¹ºÂòÏûºÄÐÔÎäÆ÷");
		RefillAmmo();
		UpdateBotState(MS_WAIT_AMMO, 8000);
		break;

	case MS_WAIT_AMMO:
		// ammo bought?
		if (Me.GetPlayerStats(D2PS_AMMOQUANTITY) == 0)
		{
			FinishRun(FR_FAILED, FALSE);
		}
		else
		{
			UpdateBotState(MS_LEAVE_LARZUK);
		}
		break;

	case MS_LEAVE_LARZUK:
		Me.LeaveNpc(&Larzuk);
		UpdateBotState(MS_AFTER_LARZUK, 1000);

		break;

	case MS_AFTER_LARZUK:

		if (NeedResMerc() || Me.GetHPPercent() < g_config.iMalahHP || Me.GetManaPercent() < g_config.iMalahMana)
		{
			// need to heal or res merc, so quit and do those in next game
			FinishRun(FR_REPAIR, FALSE);
			break;
		}

		Me.OverHead(_T("ÿc4Traveling to ÿc2Red Portal"));
		SIGPostString("×ßÏò´«ËÍÃÅ");
		Me.RunTo(0x140d, 0x13b4);
		Me.RunTo(0x140c, 0x13c9);
		Me.RunTo(0x140d, 0x13df);
		Me.RunTo(0x140e, 0x13f3);
		Me.RunTo(0x13fe, 0x1400);
		//Me.RunTo(&RedPortal);
		WaitForMove(MS_CHECK_PORTAL);
		break;

	case MS_TRAVEL_CAIN:
		// goto cain for identifying items
		Me.OverHead(_T("ÿc4Traveling to ÿc2Cain"));
		SIGPostString("×ßÏòCain");
		Me.RunTo(0x13f1, 0x13b5);
		Me.RunTo(0x13f7, 0x13c4);
		//Me.RunTo(0x1404, 0x13c3);
		//Me.RunTo(&Cain);
		WaitForMove(MS_TALK_CAIN);
		break;

	case MS_TALK_CAIN:
		Me.OverHead("ÿc4Interacting with ÿc2Cain");
		SIGPostString("ÓëCain½»Ì¸");
		Me.TalkToNpc(&Cain);
		WaitForNPC(MS_IDENTIFY);
		break;

	case MS_IDENTIFY:
		Me.OverHead("ÿc4Identifying items");
		SIGPostString("ÇëÇó¼ø¶¨ÎïÆ·");
		Me.IdentifyItems(&Cain);
		UpdateBotState(MS_LEAVE_CAIN, 3000);
		break;

	case MS_LEAVE_CAIN:
		Me.LeaveNpc(&Cain);
		UpdateBotState(MS_USE_IDBOT, 1000);
		break;

	case MS_USE_IDBOT:
		// use IDBot, time limit is 60 seconds
		g_bInventoryFull = FALSE;
		UpdateBotState(MS_IDBOT_END, 60000);
		Me.OverHead("ÿc4Using ÿc2IDBot");
		SIGPostString("Ê¹ÓÃIDBot");
		server->GameCommandLine("idbot id");		
		break;

	case MS_IDBOT_END:	
		// IDBot finished
		if (NeedResMerc() || Me.GetHPPercent() < g_config.iMalahHP || Me.GetManaPercent() < g_config.iMalahMana)
		{
			FinishRun(FR_IDBOT, FALSE);
			break;
		}
		
		UpdateBotState(MS_AFTER_CAIN);
		break;

	case MS_AFTER_CAIN:
		// continue moving to red portal
		Me.OverHead(_T("ÿc4Traveling to ÿc2Red Portal"));
		SIGPostString("×ßÏò´«ËÍÃÅ");
		Me.RunTo(0x1403, 0x13e0);
		Me.RunTo(0x140d, 0x13df);
		Me.RunTo(0x140e, 0x13f3);
		Me.RunTo(0x13fe, 0x1400);
		//Me.RunTo(&RedPortal);
		WaitForMove(MS_CHECK_PORTAL);
		
		break;

	case MS_CHECK_PORTAL:
		// Have we reached the red portal?
		if (!RedPortal.IsValidObject())
		{
			server->GamePrintError("Failed to reach red portal.");
			SIGPostString("ÎÞ·¨µ½´ïºìÉ«´«ËÍÃÅ");
			FinishRun(FR_FAILED, FALSE);
			break;
		}

		g_wOldX = RedPortal.GetPositionX();
		g_wOldY = RedPortal.GetPositionY();
		g_iActionIndex = 0;
		g_iSpellCast = 0;
		UpdateBotState(MS_CAST_BEFORERED);		
		break;

	case MS_CAST_BEFORERED:
		// Perform actions before red portal
		ProcessActions(PB_ACTION_BEFORERED);
		break;

	case MS_TALK_PORTAL:
		Me.OverHead("ÿc4Entering ÿc2Red Portal...");
		SIGPostString("ÕýÔÚ½øÈëºìÉ«´«ËÍÃÅ");
		// code here to talk to portal		
		Me.OpenStaticObject(&RedPortal);
		UpdateBotState(MS_CHECK_INPORTAL, 1500);
		break;

	case MS_CHECK_INPORTAL:
		if (Me.GetCurrentMapID() == MAP_A5_NIHLATHAKS_TEMPLE)
		{
			// entered red successfully
			Me.OverHead("ÿc4Entered red portal successfully.");
			SIGPostString("ÒÑ³É¹¦½øÈëºìÉ«´«ËÍÃÅ");
			Me.SetRadius(5);
			thisgame->player->PlayerPositionX = RedPortal.GetPositionX();
			thisgame->player->PlayerPositionY = RedPortal.GetPositionY();
			g_iActionIndex = 0;
			g_iSpellCast = 0;
			UpdateBotState(MS_MAP_UPDATE);			
		}
		else
		{
			if (g_nPortalRetry >= 5)
			{
				// totally failed, quit
				server->GamePrintError("Failed to enter red portal.");
				SIGPostString("ÎÞ·¨½øÈëºìÉ«´«ËÍÃÅ");
				FinishRun(FR_FAILED, FALSE);
			}
			else
			{
				// try more
				g_nPortalRetry++;
				UpdateBotState(MS_TALK_PORTAL, 500);
			}
		}
				
		break;

	case MS_MAP_UPDATE:
		if (Me.GetDistance(RedPortal.GetPositionX(), RedPortal.GetPositionY()) > 20)
		{
			Me.OverHead("ÿc4Waiting for location update.");
			SIGPostString("ÕýÔÚ½ÓÊÕÐÂµØÍ¼×ø±êÏµÍ³");
		}
		else
		{
			Me.OverHead("ÿc4Location updated.");
			CalcPindleRoute();
			// cast enchant on pet, if available
			if (Me.CastSpell(D2S_ENCHANT, FALSE, &Pet))
			{
				Me.OverHead("ÿc4Casting ÿc2Enchant ÿc4on pet.");
				g_ticker.ResetTicker(500);
			}
			UpdateBotState(MS_CAST_AFTERRED);
		}
		break;

	case MS_CAST_AFTERRED:

		// actions after entering red portal
		ProcessActions(PB_ACTION_INRED);
		break;

	case MS_APPROACH_PINDLESKIN:
		Me.OverHead("ÿc4Approaching ÿc2Pindleskin.");
		SIGPostString("ÕýÔÚ½Ó½üPindleskin");
		// code here to approach pindleskin

		if (Me.HasSpell(D2S_TELEPORT))
		{
			Me.OverHead("ÿc4Teleporting to ÿc2Pindleskin");
			for (int i = 0; i < g_aPindleRoute.GetSize(); i++)
				Me.TeleportTo(g_aPindleRoute[i].x, g_aPindleRoute[i].y);
		}
		else
		{
			Me.OverHead("ÿc4Running to ÿc2Pindleskin");
			for (int i = 0; i < g_aPindleRoute.GetSize(); i++)
				Me.RunTo(g_aPindleRoute[i].x, g_aPindleRoute[i].y);
		}	
		
		g_iActionIndex = 0;
		g_iSpellCast = 0;		
		WaitForMove(MS_CAST_ONPINDLESKIN);
		break;

	case MS_CAST_ONPINDLESKIN:
		// Perform actions to kill pindleskin
		if (!Pindleskin.IsValidObject())
		{
			server->GamePrintError("Failed to reach Pindleskin.");
			SIGPostString("ÎÞ·¨µ½´ïPindleskinÎ»ÖÃ");
			FinishRun(FR_FAILED, FALSE);
			break;
		}		

		// check corpose-related action
		dwTemp = ProcessCorpose();
		if (dwTemp)
		{
			g_ticker.ResetTicker(dwTemp);
			break;
		}

		ProcessActions(PB_ACTION_ONPINDLE);
		break;

	case MS_PINDLE_DEAD:

		// pindleskin is dead and his corpose appeared
		// for barbs, use Find item in a second, for others
		// enable pickit
		SIGPostString("³É¹¦É±ËÀPindleskin");
		Me.SetRadius(50);
		if (Me.HasSpell(D2S_FINDITEM))
			UpdateBotState(MS_CAST_FINDITEM);
		else
			UpdateBotState(MS_PICK_ITEMS, 2000);

		break;

	case MS_CAST_FINDITEM:
		// Cast find-item on pindleskin's corpse, Barbarian only
		Me.OverHead("ÿc4Using ÿc2Find Item ÿc4skill on Pindleskin's corpose.");
		SIGPostString("ÕýÔÚ·­ÀÏPµÄÊ¬");
		Me.CastSpell(D2S_FINDITEM, FALSE, &Pindleskin);
		UpdateBotState(MS_PICK_ITEMS, 2000);
		break;

	case MS_PICK_ITEMS:
		// check and pick items
		Me.OverHead("ÿc4Picking up items...");
		SIGPostString("ÕýÔÚ¼ì²é»òÊ°È¡Õ½ÀûÆ·");		
		UpdateBotState(MS_PICKITEMS_OK, 6000);
		break;

	case MS_PICKITEMS_OK:
		FinishRun(FR_SUCCEEDED, FALSE);

	default:
		break;

	} // switch (g_state.nState)

return 0;	
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{	
	Me.OnGamePacketBeforeSent(aPacket, aLen);
	Pet.OnGamePacketBeforeSent(aPacket, aLen);
	Pindleskin.OnGamePacketBeforeSent(aPacket, aLen);
	Cain.OnGamePacketBeforeSent(aPacket, aLen);
	Malah.OnGamePacketBeforeSent(aPacket, aLen);
	Larzuk.OnGamePacketBeforeSent(aPacket, aLen);
	Qual.OnGamePacketBeforeSent(aPacket, aLen);
	RedPortal.OnGamePacketBeforeSent(aPacket, aLen);
	Act1Wp.OnGamePacketBeforeSent(aPacket, aLen);

	// weapon switch must not be performed during process
	if (aLen == 1 && aPacket[0] == 0x60)
	{
		if (!g_bStop)
		{
			server->GamePrintError("Weapon-switch prohibited while Pindlebot is running.");
			return 0;
		}
	}	

	// resurrect merc packet, verify before send, make sure merc is really dead, 
	// otherwise merc may disappear permanently.
	if (aLen == 5 && aPacket[0] == 0x62)
	{
		// if merc is not dead, block the packet
		if (Pet.IsAlive())
			return 0;
	}

	/*
	if (aPacket[0] == 0x03)
	{
		WORD x, y;
		::memcpy(&x, aPacket + 1, 2);
		::memcpy(&y, aPacket + 3, 2);
		char sz[128] = "";
		sprintf(sz, "x %d, y %d", x - wPX, y - wPY);
		//server->GamePrintInfo(sz);
		return aLen;
	}
	*/

	return aLen;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	Me.OnGamePacketBeforeReceived(aPacket, aLen);
	Pet.OnGamePacketBeforeReceived(aPacket, aLen);
	Pindleskin.OnGamePacketBeforeReceived(aPacket, aLen);
	Cain.OnGamePacketBeforeReceived(aPacket, aLen);
	Malah.OnGamePacketBeforeReceived(aPacket, aLen);
	Larzuk.OnGamePacketBeforeReceived(aPacket, aLen);
	Qual.OnGamePacketBeforeReceived(aPacket, aLen);
	RedPortal.OnGamePacketBeforeReceived(aPacket, aLen);
	Act1Wp.OnGamePacketBeforeReceived(aPacket, aLen);

	// item sticking on cursor upon join game
	if (aPacket[0] == 0x9c && aPacket[1] == 0x12)
	{
		//PrintPacket(aPacket, aLen, 1);
		server->GamePrintInfo("Item stuck on cursor, dropping it.");
		BYTE a[5] = {0x17}; // 1.10 confirmed
		::memcpy(a + 1,  aPacket + 4, 4);
		server->GameSendPacketToServer(a, 5);
		return aLen;
	}

	// arrow/bolt info from NPC
	if (g_nCurState == MS_VERIFY_STORE
		&& aPacket[0] == ITEM_MESSAGEID_DROP
		&& aPacket[1] == ITEM_ACTION_TO_STORE) // in store
	{
		ITEM item = {0};
		::D2ParseItem(aPacket, aLen, item);
		if (stricmp(item.szItemCode, "aqv") == 0)
		{
			g_dwArrowID = item.dwItemID;
		}
		else if (stricmp(item.szItemCode, "cqv") == 0)
		{
			g_dwBoltID = item.dwItemID;
		}

		if (g_dwArrowID != 0 && g_dwBoltID != 0)
		{
			UpdateBotState(MS_REFILL_AMMO);
		}

		return aLen;		
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Revived by Itself (Act5 Zombies) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 17
		&& aPacket[0] == 0x4d)
	{
		MONSTERCORPSE mc = {0};
		::memcpy(&mc.dwCorpseID, aPacket + 2, 4);
		g_aCorposes.RemoveMatchedItems(mc);
		return aLen;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Corpse Appeared
	//////////////////////////////////////////////////////////////////////
	if (g_nCurState == MS_CAST_ONPINDLESKIN
		&& aLen == 12
		&& aPacket[0] == 0x69
		&& aPacket[5] == 0x09)
	{
		MONSTERCORPSE mc;
		::memcpy(&mc.dwCorpseID, aPacket + 1, 4);
		::memcpy(&mc.x, aPacket + 6, 2);
		::memcpy(&mc.y, aPacket + 8, 2);
		if (mc.dwCorpseID != Pet.GetObjectID() && mc.dwCorpseID != Pindleskin.GetObjectID())
			g_aCorposes.InsertRear(mc);
		return aLen;
	}

    return aLen;
}

void GetPercent(int n, int nTotal, LPTSTR lpBuffer)
{
	lpBuffer[0] = '\0';
	if (nTotal == 0)
		return;

	float f = (float)n / (float)nTotal * 100.0f;
	if (f > 100.0)
		f = 100.0;
	
	sprintf(lpBuffer, "(%.1f%%)", f);
}

BOOL PRIVATE OnGameCommandStatistics(char** argv, int argc)
{
	char szPerc[32] = "";
	int nData = 0;

	server->GamePrintInfo("ÿc7Pindlebot Statistics:");
	server->GamePrintInfo("ÿc7-------------------------------");

	int nTotalGames = g_logIni.GetInt("Log","Game", 0) - 1;
	DWORD dwRunTime = g_logIni.GetInt("Log", "Run Time", 0);

	::D2Infof("ÿc7Completed Games:ÿc0 %d", nTotalGames);
	::D2Infof("ÿc7In Game Time:ÿc0 %d Hours %d Minutes %d Seconds", dwRunTime / 3600, (dwRunTime % 3600) / 60, (dwRunTime % 3600) % 60);

	float f = 0.0;

	if (nTotalGames > 0)
		f = (float)dwRunTime / (float)nTotalGames;
	
	::D2Infof("ÿc7Average Speed:ÿc0 %u Seconds/Game", (long)f);

	nData = g_logIni.GetInt("Log", "Succeed", 0);
	GetPercent(nData, nTotalGames, szPerc);
	::D2Infof("ÿc7Succeeded:ÿc0 %4d %s", nData, szPerc);

	nData = g_logIni.GetInt("Log", "Merc Die", 0);
	GetPercent(nData, nTotalGames, szPerc);
	::D2Infof("ÿc7Merc Died:ÿc0 %4d %s", nData, szPerc);

	nData = g_logIni.GetInt("Log", "Char Die", 0);
	GetPercent(nData, nTotalGames, szPerc);
	::D2Infof("ÿc7Char Died:ÿc0 %4d %s", nData, szPerc);

	nData = g_logIni.GetInt("Log", "Retreat", 0);
	GetPercent(nData, nTotalGames, szPerc);
	::D2Infof("ÿc7Retreated:ÿc0 %4d %s", nData, szPerc);

	nData = g_logIni.GetInt("Log", "Avoid", 0);
	GetPercent(nData, nTotalGames, szPerc);
	::D2Infof("ÿc7Pre-Avoided:ÿc0 %4d %s", nData, szPerc);
	
	nData = g_logIni.GetInt("Log", "Ins Skill", 0);
	GetPercent(nData, nTotalGames, szPerc);
	::D2Infof("ÿc7Ins. Skill:ÿc0 %4d %s", nData, szPerc);

	nData = g_logIni.GetInt("Log", "Potion Drunk", 0);
	::D2Infof("ÿc7Potion Drunk:ÿc0 %d", nData);

	nData = g_logIni.GetInt("Log", "Potion Merc", 0);
	::D2Infof("ÿc7Merc Potion Fed:ÿc0 %d", nData);

	server->GamePrintInfo("ÿc7-------------------------------");
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandPause(char** argv, int argc)
{
	g_bPaused = TRUE;
	Me.PauseJobs(TRUE);
	server->GamePrintInfo("Pindlebot ÿc2paused.");
	SIGPause();
	return TRUE;
}

BOOL PRIVATE OnGameCommandResume(char** argv, int argc)
{
	g_bPaused = FALSE;
	Me.PauseJobs(FALSE);
	g_ticker.ResetTicker();
	server->GamePrintInfo("Pindlebot ÿc2resumed.");
	SIGResume();
	return TRUE;
}

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	//wPX = RedPortal.GetPositionX();
	//wPY = RedPortal.GetPositionY();
	//g_bDebug = !g_bDebug;	
	D2Infof("Bot state: %d - %d, Paused=%d", g_nCurState, g_nNextState, g_bPaused);
	return TRUE;
}

BOOL PRIVATE OnGameCommandIDOK(char** argv, int argc)
{
	server->GamePrintInfo("ÿc2ID items finished.");
	g_bPaused = FALSE;
	FinishRun(FR_IDBOT, FALSE);
	return TRUE;
}

// SIG functions
void SIGPause()
{
	if (g_bUseSig)
		server->GameCommandLine("sig pause");
}

void SIGResume()
{
	if (g_bUseSig)
		server->GameCommandLine("sig resume");
}

void SIGStop()
{
	if (g_bUseSig)
		server->GameCommandLine("sig stop");
}

void SIGQuitD2()
{
	if (g_bUseSig)
		server->GameCommandLine("sig quitd2");
}

void SIGPostString(LPCTSTR lpsz)
{
	if (g_bUseSig && lpsz != NULL)
	{
		char sz[256] = "";
		sprintf(sz, "sig poststring %s", lpsz);
		server->GameCommandLine(sz);
	}
}
