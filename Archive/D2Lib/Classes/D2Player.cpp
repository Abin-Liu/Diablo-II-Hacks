//////////////////////////////////////////////////////////////////////
// D2Player.cpp: implementation of the CD2Player class.
//////////////////////////////////////////////////////////////////////

#include "D2Player.h"
#include <math.h>
#include <stdlib.h>

#define PLAYER_SYNC_LIMIT	3000
#define MAXGOLD(char_level)((char_level) * (DWORD)10000)
#define GOLDSPACE(char_level, inventory_gold)(MAXGOLD(char_level) > (inventory_gold) ? MAXGOLD(char_level) - (inventory_gold) : 0)
#define GUESSLEVEL(inventory_gold)(((inventory_gold) % 10000) ? (inventory_gold) / 10000 + 1 : (inventory_gold) / 10000)

#define TARGET_TYPE_PLAYER	0x00
#define TARGET_TYPE_UNIT	0x01
#define TARGET_TYPE_OBJ		0x02
#define TARGET_TYPE_ITEM	0x04

#define TELEPORT_TICK		600
#define TP_CAST_TICK		1000
#define TP_ENTER_TICK		500
#define MAX_MAPDATA			10
#define MAP_CHECKTICK		500

enum {	TS_NONE = 0,			// nothing
		TS_CAST_PORTAL,			// open a town portal
		TS_WAIT_PORTAL,			// wait for portal to appear
		TS_ENTER_PORTAL,		// enter the town portal
		TS_VERIFY_INTOWN };		// verify in town

CD2Player::CD2Player()
{
	InitAttributes(); // Initialization
}

CD2Player::~CD2Player()
{
}

void CD2Player::InitAttributes()
{
	CD2Object::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_aHostilerList.RemoveAll();
	m_aCorpseList.RemoveAll();	
	m_wLeftSpell = D2S_INVALID;
	m_wRightSpell = D2S_INVALID;
	::memset(&m_lastD2s, 0, sizeof(D2SPELLINFO));
	m_lastD2s.wSpellID = D2S_INVALID;
	m_aMapDataList.RemoveAll();
	m_iCurMap = MAP_A1_ROGUE_ENCAMPMENT;
	::memset(m_aBeltItems, 0, sizeof(ITEM) * 16);
	m_iBeltRows = 1;
	CleanJobs();
	m_nTeleportFail = 0;
	m_nTPState = TS_NONE;
	m_nTPMaxRetry = 0;
	m_nTPCastfail = 0;
	m_nTPEnterFail = 0;
	m_iLastSyncMap = MAP_A1_ROGUE_ENCAMPMENT;
	m_dwLastSyncTime = 0;	
	m_wHP = 0;
	m_wMana = 0;
	m_bLockSpawn = TRUE;
	m_bTeleported = FALSE;
	m_bPaused = FALSE;
	m_nNpcMaxRetry = 0;
	m_nNpcFails = 0;
	m_pNpc = NULL;
	m_iWatchAct = 0xff;
	m_msgSender.SetTargetHWnd(NULL);
	m_bGameReady = FALSE;
	m_mapTicker.ResetTicker();
}

BOOL CD2Player::LeaveGame() const
{
	return server->GameSaveAndExit();
}

void CD2Player::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Object::OnGameJoin(pGame); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	m_msgSender.SetTargetHWnd(server->FindMyD2WndHandle());
	m_dwObjectID = pGame->player->PlayerID;
	m_stats.SetD2Wnd(server->FindMyD2WndHandle());	
}

void CD2Player::OnGameLeave()
{
	CD2Object::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.
	
	// TODO: Add your implementation here.
	m_aSkillList.RemoveAll();
	m_aChargeableSkillList.RemoveAll();
}

void CD2Player::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Object::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	const DWORD PLAYERID = GetObjectID();

	//////////////////////////////////////////////////////////
	// Skill List, The Only Packet That is Received Before Game Join!
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x94
		&& !::memcmp(&PLAYERID, aPacket + 2, 4))
	{
		m_aSkillList.RemoveAll();
		WORD wSpell;
		for (BYTE i = 0; i < aPacket[1]; i++)
		{
			::memcpy(&wSpell, aPacket + i * 3 + 6, 2);

			// Ignore charged spells
			if (wSpell != D2S_SCROLLOFIDENTIFY
				&& wSpell != D2S_TOMEOFIDENTIFY
				&& wSpell != D2S_SCROLLOFTOWNPORTAL
				&& wSpell != D2S_TOMEOFTOWNPORTAL)
			{
				m_aSkillList.Add(wSpell);
			}			
		}

		m_aSkillList.Sort();
		return;
	}

	//////////////////////////////////////////////////////////
	// Charge-able Spells 
	//////////////////////////////////////////////////////////
	if (aLen == 12
		&& aPacket[0] == 0x22)
	{
		WORD wSpell;
		::memcpy(&wSpell, aPacket + 7, 2);

		if (aPacket[9] > 0) // Has some changes
		{
			if (m_aChargeableSkillList.Find(wSpell) == -1)
			{
				m_aChargeableSkillList.Add(wSpell);
				m_aChargeableSkillList.Sort();
			}
		}
		else // Charges used up
		{
			int nIdx = m_aChargeableSkillList.Find(wSpell);
			if (nIdx != -1)
				m_aChargeableSkillList.RemoveAt(nIdx);
		}

		OnSpellCharges(wSpell, aPacket[9]);
		FormatAndDebugMessage(_T("Spell %04X, %d charges left."), wSpell, aPacket[9]);
		NotifyD2Message(PM_SPELLCHARGES, (WPARAM)wSpell, (LPARAM)aPacket[9]);
		return;
	}	

	//////////////////////////////////////////////////////////
	// Act Changed
	//////////////////////////////////////////////////////////
	if (aLen == 1 && aPacket[0] == 0x05)
	{
		m_iWatchAct = GetCurrentAct();
		return;
	}

	//////////////////////////////////////////////////////////
	// Map Data spawned
	//////////////////////////////////////////////////////////
	if (aLen == 0x06 && aPacket[0] == 0x07)
	{
		MAPDATA md;
		md.iMapID = aPacket[5];
		::memcpy(&md.dwMapFlag, aPacket + 1, 4);
		
		if (m_aMapDataList.Find(md) == -1)
			m_aMapDataList.Add(md);

		return;
	}

	//////////////////////////////////////////////////////////
	// Map Data Destroyed
	//////////////////////////////////////////////////////////
	if (aLen == 0x06 && aPacket[0] == 0x08)
	{
		MAPDATA md;
		md.iMapID = aPacket[5];
		::memcpy(&md.dwMapFlag, aPacket + 1, 4);

		int nIdx = m_aMapDataList.Find(md);
		if (nIdx != -1)
			m_aMapDataList.RemoveAt(nIdx);

		return;
	}

	//////////////////////////////////////////////////////////
	// Equip Event
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x9c || aPacket[0] == 0x9d)
	{
		if (aPacket[1] == ITEM_ACTION_TO_BELTSLOT
			|| aPacket[1] == ITEM_ACTION_SWITCH_BELTSLOT
			|| aPacket[1] == ITEM_ACTION_FROM_BELTSLOT
			|| aPacket[1] == ITEM_ACTION_SHIFT_BELTSLOT
			|| aPacket[1] == ITEM_ACTION_TO_EQUIP
			|| aPacket[1] == ITEM_ACTION_SWITCH_EQUIP
			|| aPacket[1] == ITEM_ACTION_FROM_EQUIP)
		{
			ITEM item = { 0 };
			if (::D2ParseItem(aPacket, aLen, item))
			{
				AnalyzeItem(item);
				OnEquipEvent(aPacket[1], &item);
				FormatAndDebugMessage(_T("Equip event %d, %08X, %s"), aPacket[1], item.dwItemID, item.szItemCode);
				NotifyD2Message(PM_EQUIPEVENT, aPacket[1], (LPARAM)&item);
			}
		}
		return;
	}	

	//////////////////////////////////////////////////////////
	// Stash Opened
	//////////////////////////////////////////////////////////
	if (aLen == 2 && aPacket[0] == 0x77 && aPacket[1] == 0x10)
	{
		OnStashOpen(GetCurrentAct());
		FormatAndDebugMessage(_T("Stash opened at act %d"), GetCurrentAct());
		NotifyD2Message(PM_STASHOPEN, (WPARAM)GetCurrentAct(), (LPARAM)0);
		return;
	}

	//////////////////////////////////////////////////////////
	// Waypoint Opened
	//////////////////////////////////////////////////////////
	if (aLen == 21 && aPacket[0] == 0x63)
	{
		OnWPOpen(GetCurrentAct(), m_iCurMap);
		FormatAndDebugMessage(_T("WP opened at act %d map %d"), GetCurrentAct(), m_iCurMap);
		NotifyD2Message(PM_WPOPEN, (WPARAM)GetCurrentAct(), (LPARAM)m_iCurMap);
		return;
	}

	//////////////////////////////////////////////////////////
	// Player/Corpse spawned
	//////////////////////////////////////////////////////////
	if (aLen == 26
		&& aPacket[0] == 0x59)
	{
		DWORD dwID;
		::memcpy(&dwID, aPacket + 1, 4);
		if (PLAYERID != dwID)
		{
			// player's corpse appear? check the name
			TCHAR szName[PLAYER_MAX_NAME + 1] = _T("");
			::memcpy(szName, aPacket + 6, PLAYER_MAX_NAME + 1);
			if (!_tcsicmp(m_thisPlayer.szPlayerName, szName))
			{
				// it's my corpse
				PLAYEROBJ corpse;
				corpse.dwID = dwID;
				::memcpy(&corpse.x, aPacket + 22, 2);
				::memcpy(&corpse.y, aPacket + 24, 2);
				if (corpse.x > 0 && corpse.y > 0)
				{
					m_aCorpseList.Add(corpse);
					OnCorpse(corpse.dwID, corpse.x, corpse.y);
					FormatAndDebugMessage(_T("Player Corpse %08X, %04X, %04X"), corpse.dwID, corpse.x, corpse.y);
					NotifyD2Message(PM_CORPSE, (WPARAM)corpse.dwID, MAKELPARAM(corpse.x, corpse.y));
				}
			}
		}
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Player's Corpse is Destroyed by the Game (be Gone from Player's Vision) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 6
		&& aPacket[0] == 0x0a
		&& aPacket[1] == TARGET_TYPE_PLAYER)
	{
		PLAYEROBJ corpse;
		::memcpy(&corpse.dwID, aPacket + 2, 4);
		int n = m_aCorpseList.Find(corpse);
		if (n >= 0)
		{
			m_aCorpseList.RemoveAt(n);
		}
		return;
	}
	
	//////////////////////////////////////////////////////////////////////
	// Player's Corpse is Picked Up 
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 6
		&& aPacket[0] == 0x8e
		&& aPacket[1] == 0x00
		&& !::memcmp(&PLAYERID, aPacket + 2, 4))
	{
		OnCorpsePicked();
		DebugMessage(_T("Corpse Picked Up"));
		NotifyD2Message(PM_CORPSEPICKEDUP, 0, 0);
		return;
	}	

	//////////////////////////////////////////////////////////
	// Player Character Level Changed
	//////////////////////////////////////////////////////////
	if (aLen == 3
		&& aPacket[0] == 0x1d 
		&& aPacket[1] == 0x0c)
	{
		BYTE iLevel = GetPlayerStats(D2PS_LEVEL);
		OnCharLevel(iLevel);
		FormatAndDebugMessage(_T("Player level %d"), iLevel);
		NotifyD2Message(PM_LEVEL, (WPARAM)0, (LPARAM)iLevel);
		return;
	}

	//////////////////////////////////////////////////////////
	// Player HP/Mana Updates
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x95)
	{
		m_aHPManaCheckList.InsertRear(MAKELONG(m_wHP, m_wMana));		
		m_wHP = *((WORD *)(aPacket + 1)) & 0x0FFF;
		m_wMana = (*((WORD *)(aPacket + 3)) & 0x0FFF) << 1;
		return;
	}

	//////////////////////////////////////////////////////////
	// Player Death
	//////////////////////////////////////////////////////////
	if ( aLen >= 6
		&& aPacket[0] == 0x8e
		&& aPacket[1] == 0x01
		&& !::memcmp(&PLAYERID, aPacket + 2, 4)
		&& !IsPlayerInTown())
	{
		m_wHP = 0;
		OnPlayerDeath(GetCurrentAct(), GetPositionX(), GetPositionY());
		FormatAndDebugMessage(_T("Player Died: %d, %04X, %04X"), GetCurrentAct(), GetPositionX(), GetPositionY());
		NotifyD2Message(PM_DEATH, (WPARAM)GetCurrentAct(), MAKELPARAM(GetPositionX(), GetPositionY()));
		return;
	}

	//////////////////////////////////////////////////////////
	// Hostility/Unhostility
	//////////////////////////////////////////////////////////
	if (aLen == 11
		&& aPacket[0] == 0x8c		
		&& !::memcmp(&PLAYERID, aPacket + 5, 4))
	{
		DWORD dwOtherID;
		::memcpy(&dwOtherID, aPacket + 1, 4);

		if (aPacket[9] == 0x08)
		{
			// Hostility
			int n = m_aHostilerList.Find(dwOtherID);
			if (n == -1)
			{
				m_aHostilerList.Add(dwOtherID);
				OnHostile(dwOtherID, GetHostilerCount());
				FormatAndDebugMessage(_T("Hostiled by %08X, Hostilers %u"), dwOtherID, GetHostilerCount());
				NotifyD2Message(PM_HOSTILE, (WPARAM)dwOtherID, (LPARAM)GetHostilerCount());
			}
		}
		else if (aPacket[9] == 0x00)
		{
			// Unhostility
			int n = m_aHostilerList.Find(dwOtherID);
			if (n != -1)
			{
				m_aHostilerList.RemoveAt(n);
				OnUnhostile(dwOtherID, GetHostilerCount());
				FormatAndDebugMessage(_T("Unhostiled by %08X, Hostilers %u"), dwOtherID, GetHostilerCount());
				NotifyD2Message(PM_UNHOSTILE, (WPARAM)dwOtherID, (LPARAM)GetHostilerCount());
			}
		}
		return;
	}

	//////////////////////////////////////////////////////////
	// Other Player Quit
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x5c
		&& aLen == 5)
	{
		DWORD dwQuitterID;
		::memcpy(&dwQuitterID, aPacket + 1, 4);
		
		int n = m_aHostilerList.Find(dwQuitterID);
		if (n >= 0)
		{
			m_aHostilerList.RemoveAt(n);		
			OnUnhostile(dwQuitterID, GetHostilerCount());
			FormatAndDebugMessage(_T("Unhostiled by %08X, Hostilers %u"), dwQuitterID, GetHostilerCount());
			NotifyD2Message(PM_UNHOSTILE, (WPARAM)dwQuitterID, (LPARAM)GetHostilerCount());
		}
		return;
	}	

	//////////////////////////////////////////////////////////
	// Player Left/Right Spell Selected
	//////////////////////////////////////////////////////////
	if (aLen == 13
		&& aPacket[0] == 0x23
		&& aPacket[1] == 0x00
		&& !::memcmp(&PLAYERID, aPacket + 2, 4))
	{
		WORD wSpell;
		::memcpy(&wSpell, aPacket + 7, 2);
		WORD wOldSpell = 0;

		if (aPacket[6] == 0x01)
		{
			// Left spell
			if (m_wLeftSpell != wSpell)
			{
				wOldSpell = m_wLeftSpell;
				m_wLeftSpell = wSpell;
				D2GetSpellInfo(wSpell, &m_lastD2s);
				OnLSpell(wOldSpell, wSpell);
				FormatAndDebugMessage(_T("Left Spell: %04X - %04X"), wOldSpell, wSpell);
				NotifyD2Message(PM_LSPELL, (WPARAM)wOldSpell, (LPARAM)wSpell);
			}
		}
		else if (aPacket[6] == 0x00)
		{
			// Right spell
			if (m_wRightSpell != wSpell)
			{
				wOldSpell = m_wRightSpell;
				m_wRightSpell = wSpell;
				D2GetSpellInfo(wSpell, &m_lastD2s);
				OnRSpell(wOldSpell, wSpell);
				FormatAndDebugMessage(_T("Right Spell: %04X - %04X"), wOldSpell, wSpell);
				NotifyD2Message(PM_RSPELL, (WPARAM)wOldSpell, (LPARAM)wSpell);
			}
		}
		return;
	}

	//////////////////////////////////////////////////////////
	// Teleported (Teleport spell, WP, TP, etc)
	//////////////////////////////////////////////////////////
	if (aLen == 11
		&& aPacket[0] == 0x15
		&& !::memcmp(&PLAYERID, aPacket + 2, 4)) 
	{
		m_bTeleported = TRUE;		
		m_dwLastSyncTime = ::GetTickCount();
		WORD x, y;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		OnTeleport(GetCurrentAct(), x, y);
		FormatAndDebugMessage(_T("Teleport: Act %d, %04X, %04X"), GetCurrentAct(), x, y);
		NotifyD2Message(PM_MAPBLINK, (WPARAM)GetCurrentAct(), MAKELPARAM(x, y));
		return;
	}

	//////////////////////////////////////////////////////////
	// NPC Interact Successfully
	//////////////////////////////////////////////////////////
	if (aLen == 40
		&& aPacket[0] == 0x27
		&& aPacket[1] == 0x01)
	{
		DWORD dwNpcID;
		::memcpy(&dwNpcID, aPacket + 2, 4);
		if (m_pNpc && m_pNpc->GetObjectID() == dwNpcID)
			NPCEnd(TRUE);
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Weapon Switch
	//////////////////////////////////////////////////////////////////////
	if (aLen == 1 && aPacket[0] == 0x97)
	{
		OnWeaponSwitch();
		DebugMessage(_T("Weapon switched."));
		NotifyD2Message(PM_WEAPONSWITCH, (WPARAM)0, (LPARAM)0);
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Town Portal Appeared
	//////////////////////////////////////////////////////////////////////
	if (m_nTPState == TS_WAIT_PORTAL
		&& aLen > 12
		&& aPacket[0] == 0x51
		&& aPacket[6] == 0x3b
		&& aPacket[7] == 0x00)
	{
		::memcpy(&m_portal.dwID, aPacket + 2, 4);
		::memcpy(&m_portal.x, aPacket + 8, 2);
		::memcpy(&m_portal.y, aPacket + 10, 2);
		m_nTPState = TS_ENTER_PORTAL;
		m_nTPCastfail = 0;
		m_nTPEnterFail = 0;
		m_ticker.ResetTicker(0);
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Player's Inventory is Full
	//////////////////////////////////////////////////////////////////////
	if (aLen == 8
		&& aPacket[0] == 0x2c
		&& aPacket[1] == 0x00
		&& aPacket[6] == 0x17
		&& aPacket[7] == 0x00
		&& !::memcmp(aPacket + 2, &PLAYERID, 4))
	{
		 // notify the client
		OnInventoryFull();
		DebugMessage(_T("Player inventory is full."));
		NotifyD2Message(PM_INVENTORYFULL, (WPARAM)0, (LPARAM)0);
		return;
	}
}

void CD2Player::OnGameTimerTick()
{
	CD2Object::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	if (!IsGameReady())
		return;

	m_mapTicker.TickIt(100);
	AnalyzeMapData();

	if (!m_bGameReady)
	{
		m_bGameReady = TRUE;
		BYTE iAct = GetCurrentAct();
		WORD x = GetPositionX();
		WORD y = GetPositionY();
		OnGameReady(iAct, x, y);
		FormatAndDebugMessage(_T("Game ready at act %d, %04X, %04X"), iAct, x, y);
		NotifyD2Message(PM_GAMEREADY, (WPARAM)iAct, MAKELPARAM(x, y));
	}

	AnalyzeHPMana();

	// check act changes
	if (m_iWatchAct != 0xff)
	{
		if (GetCurrentAct() != m_iWatchAct)
		{
			BYTE iOldAct = m_iWatchAct;
			m_iWatchAct = 0xff;
			OnActChange(iOldAct, GetCurrentAct());
			FormatAndDebugMessage(_T("Act change: %d - %d"), iOldAct, GetCurrentAct());
			NotifyD2Message(PM_ACTCHANGE, (WPARAM)iOldAct, (LPARAM)GetCurrentAct());
		}
	}

	m_ticker.TickIt(100);
	if (!m_ticker.IsTickEnded())
		return;

	if (m_nTPState != TS_NONE)
	{
		DoTPProc();
		return;
	}

	if (m_bPaused)
		return;

	if (m_pNpc)
	{
		if (m_nNpcFails < m_nNpcMaxRetry)
		{
			m_nNpcFails++;
			InteractWithCurrentNpc();
			m_ticker.ResetTicker(2000);	
		}
		else
		{
			// npc failed
			NPCEnd(FALSE);
		}
	}
	
	switch (m_prevJob.nJobType)
	{
	case MJ_TELEPORT:
		if (GetDistance(m_prevJob.x, m_prevJob.y) <= 5
			|| GetDistance(m_wXBeforeTele, m_wYBeforeTele) > 0)
		{
			StepDone();	
		}
		else
		{
			if (m_nTeleportFail < 5)
			{
				m_nTeleportFail++;
				CastSpell(D2S_TELEPORT, FALSE, m_prevJob.x, m_prevJob.y, FALSE);
				m_ticker.ResetTicker(TELEPORT_TICK);				
			}
			else
			{
				FailJobs();
			}
		}
		
		break;

	case MJ_POSITION:
	case MJ_STATICOBJ:
	case MJ_UNIT:
	case MJ_PLAYER:
	case MJ_ITEM:
		if (!IsMoving())
			StepDone();
		break;

	default:
		DoFirstJob();
		break;
	}
}

DWORD CD2Player::GetHostilerCount() const
{
	return (DWORD)m_aHostilerList.GetSize();
}

void CD2Player::OnCharLevel(BYTE iLevel)
{

}

void CD2Player::OnHPUp(WORD wOldHP, WORD wNewHP)
{

}

void CD2Player::OnHPDown(WORD wOldHP, WORD wNewHP)
{

}

void CD2Player::OnManaUp(WORD wOldMana, WORD wNewMana)
{

}

void CD2Player::OnManaDown(WORD wOldMana, WORD wNewMana)
{

}

void CD2Player::OnPlayerDeath(BYTE iAct, WORD x, WORD y)
{

}

void CD2Player::OnCorpsePicked()
{

}

void CD2Player::OnHostile(DWORD dwHostilerID, DWORD dwHostilerCount)
{

}

void CD2Player::OnUnhostile(DWORD dwUnhostilerID, DWORD dwHostilerCount)
{

}

BOOL CD2Player::HasSpell(WORD wSpellID) const
{
	return m_aSkillList.Find(wSpellID) != -1 || m_aChargeableSkillList.Find(wSpellID) != -1;
}

DWORD CD2Player::GetCorpseCount() const
{
	return (DWORD)m_aCorpseList.GetSize();
}

BOOL CD2Player::PickupCorpse() const
{
	if (m_aCorpseList.IsEmpty())
	{
		DebugMessage(_T("Player does not have a nearby corpse."));
		return FALSE;
	}

	PLAYEROBJ corpse = m_aCorpseList[0];
	if (GetDistance(corpse.x, corpse.y) > 5)
		GoToEntity(TRUE, TARGET_TYPE_PLAYER, corpse.dwID, corpse.x, corpse.y);
	
	return InteractWithEntity(TARGET_TYPE_PLAYER, corpse.dwID);
}

BOOL CD2Player::GoToEntity(BOOL bRun, DWORD dwType, DWORD dwEntityID, WORD x, WORD y, BOOL bShowClientMove) const
{	
	if (dwEntityID == 0)
	{
		FormatAndDebugMessage(_T("Entity %08X (%04X, %04X) is not a valid entity or nearby position."), dwEntityID, x, y);
		return FALSE;
	}

	BYTE aPacket[16] = { 0 };
	aPacket[0] = 0x04;
	::memcpy(aPacket + 1, &dwType, 4);
	::memcpy(aPacket + 5, &dwEntityID, 4);

	// send 3 times
	if (!SendPacketToServer(aPacket, 9, 3))
	{
		DebugMessage(_T("Unable to send RunToEntity packet"));
		return FALSE;
	}

	if (bShowClientMove && IsGameReady())
	{
		DWORD dwPlayerID = GetObjectID();
		::memset(aPacket, 0, sizeof(aPacket));
		aPacket[0] = 0x10;
		aPacket[1] = 0x00;
		::memcpy(aPacket + 2, &dwPlayerID, 4);
		aPacket[6] = bRun ? 0x18 : 0x00;
		aPacket[7] = (BYTE)dwType;
		::memcpy(aPacket + 8, &dwEntityID, 4);
		::memcpy(aPacket + 12, &x, 2);
		::memcpy(aPacket + 14, &y, 2);
		SendPacketToGame(aPacket, 16, 3);
	}

	return TRUE;		
}

BOOL CD2Player::InteractWithEntity(DWORD dwType, DWORD dwEntityID) const
{
	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x13;
	::memcpy(aPacket + 1, &dwType, 4);
	::memcpy(aPacket + 5, &dwEntityID, 4);
	if (!SendPacketToServer(aPacket, 9, 2))
	{
		DebugMessage(_T("Unable to send InteractWithEntity packet"));
		return FALSE;
	}

	if (dwType == 0x01)
	{
		aPacket[0] = 0x2f;
		if (!SendPacketToServer(aPacket, 9, 1))
		{
			DebugMessage(_T("Unable to send InteractWithEntity2 packet"));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CD2Player::PrepareInteractWithNPC(DWORD dwNpcID, WORD x, WORD y) const
{
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x59;
	aPacket[1] = 0x01;
	::memcpy(aPacket + 5, &dwNpcID, 4);
	::memcpy(aPacket + 9, &x, 2);
	::memcpy(aPacket + 13, &y, 2);
	if (!SendPacketToServer(aPacket, 17))
	{
		DebugMessage(_T("Unable to send PrepareInteractWithNPC packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::TalkToNpc(const CD2Unit *pNpc, int nMaxRetryTimes)
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		DebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	m_nNpcFails = 0;
	m_nNpcMaxRetry = max(nMaxRetryTimes, 1);
	m_pNpc = pNpc;
	m_ticker.ResetTicker();
	return TRUE;
}

BOOL CD2Player::RunTo(WORD x, WORD y, BOOL bRun, BOOL bPreemptive)
{
	// We do not drive the player to run immediately, instead, we 
	// put the destination positions in to a FIFO queue and run to
	// those positions at each OnGameTimerTick call. Also, before
	// drive the player to run, we will verify each destination position
	// to see if they are valid(nearby) positions, for example, you cannot
	// run directly to a position which is 99 yards away from the player.
	if (bPreemptive)
		CleanJobs();	

	// queue this new job
	MOVEJOB job = { 0 };
	job.bRun = bRun;
	job.bShowMove = TRUE;
	job.nJobType = MJ_POSITION;
	job.x = x;
	job.y = y;
	m_aJobQueue.InsertRear(job);
	return TRUE;
}

BOOL CD2Player::LeaveNpc(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		DebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	return StopInteractWithEntity(TARGET_TYPE_UNIT, pNpc->GetObjectID());	
}

BOOL CD2Player::StopInteractWithEntity(DWORD dwType, DWORD dwEntityID) const
{
	StopInteractionUsingSpaceKey();
	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x30;
	::memcpy(&aPacket[1], &dwType, 4);
	::memcpy(&aPacket[5], &dwEntityID, 4);
	
	if (!SendPacketToServer(aPacket, 9, 2))
	{
		DebugMessage(_T("Unable to send StopInteractWithEntity packet"));
		return FALSE;
	}

	return TRUE;
}

WORD CD2Player::GetDistance(WORD x, WORD y) const
{
	if (x == 0 || y == 0)
		return 0xffff;

	WORD wPlayerX = GetPositionX();
	WORD wPlayerY = GetPositionY();

	if (wPlayerX == 0 || wPlayerY == 0)
		return 0xffff;

	return (WORD)::sqrt((double)(((int)wPlayerX - (int)x) * ((int)wPlayerX - (int)x) + ((int)wPlayerY - (int)y) * ((int)wPlayerY - (int)y)));
}

void CD2Player::OnCorpse(DWORD dwCorpseID, WORD x, WORD y)
{

}

BOOL CD2Player::OpenStaticObject(const CD2StaticObject *pStaticObject) const
{
	if (pStaticObject == NULL || !pStaticObject->IsValidObject())
	{
		DebugMessage(_T("Object data is not valid."));
		return FALSE;
	}

	// If the object is more than 5 yards away, run to it
	if (GetDistance(pStaticObject->GetPositionX(), pStaticObject->GetPositionY()) > 5)
		GoToEntity(TRUE, TARGET_TYPE_OBJ, pStaticObject->GetObjectID(), pStaticObject->GetPositionX(), pStaticObject->GetPositionY());

	// Interact with it
	return InteractWithEntity(TARGET_TYPE_OBJ, pStaticObject->GetObjectID());
}

BOOL CD2Player::LeaveStaticObject(const CD2StaticObject *pStaticObject) const
{
	if (pStaticObject == NULL || !pStaticObject->IsValidObject())
	{
		DebugMessage(_T("Object data is not valid."));
		return FALSE;
	}

	return StopInteractWithEntity(TARGET_TYPE_OBJ, pStaticObject->GetObjectID());
}

BOOL CD2Player::RunTo(const CD2Unit *pNpc, BOOL bRun, BOOL bPreemptive)
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		DebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	if (bPreemptive)
		CleanJobs();

	// queue this new job
	MOVEJOB job = { 0 };
	job.bRun = bRun;
	job.bShowMove = TRUE;
	job.nJobType = MJ_UNIT;
	job.dwEntityID = pNpc->GetObjectID();
	job.x = pNpc->GetPositionX();
	job.y = pNpc->GetPositionY();
	m_aJobQueue.InsertRear(job);

	return TRUE;
}

BOOL CD2Player::RunTo(const CD2StaticObject *pStaticObject, BOOL bRun, BOOL bPreemptive)
{
	if (pStaticObject == NULL || !pStaticObject->IsValidObject())
	{
		DebugMessage(_T("Object data is not valid."));
		return FALSE;
	}

	if (bPreemptive)
		CleanJobs();

	// queue this new job
	MOVEJOB job = { 0 };
	job.bRun = bRun;
	job.bShowMove = TRUE;
	job.nJobType = MJ_STATICOBJ;
	job.dwEntityID = pStaticObject->GetObjectID();
	job.x = pStaticObject->GetPositionX();
	job.y = pStaticObject->GetPositionY();
	m_aJobQueue.InsertRear(job);
	
	return TRUE;
}

BOOL CD2Player::EnterWP(const CD2StaticObject *pWP, BYTE iDestinationMapID) const
{
	if (pWP == NULL || !pWP->IsValidObject())
	{
		DebugMessage(_T("WP data is not valid."));
		return FALSE;
	}

	DWORD dwID = pWP->GetObjectID();
	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x49;
	::memcpy(&aPacket[1], &dwID, 4);
	aPacket[5] = iDestinationMapID;
	return SendPacketToServer(aPacket, 9);
}

BOOL CD2Player::OpenNpcStore(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		DebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}
	
	DWORD dwNpcID = pNpc->GetObjectID();	
	BYTE aPacket[13] = { 0 };
	aPacket[0] = 0x38;
	aPacket[1] = 0x01;
	::memcpy(aPacket + 5, &dwNpcID, 4);
	if (!SendPacketToServer(aPacket, 13))
	{
		DebugMessage(_T("Unable to send OpenNpcStore packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::BuyItem(DWORD dwItemID, const CD2Unit *pNpc) const
{
	if (dwItemID == 0 || pNpc == NULL || !pNpc->IsValidObject())
	{
		DebugMessage(_T("Item/NPC data are not valid."));
		return FALSE;
	}

	DWORD dwNpcID = pNpc->GetObjectID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x32;
	::memcpy(&aPacket[1], &dwNpcID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);

	if (!SendPacketToServer(aPacket, 17))
	{
		DebugMessage(_T("Unable to send BuyItem packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::RedrawClient(BOOL bFade)
{
	if (!IsGameReady())
	{
		DebugMessage(_T("Player data is not valid."));
		return FALSE;
	}

	if (m_iLastSyncMap != m_iCurMap)
	{
		DWORD dwElapsed = ::GetTickCount() - m_dwLastSyncTime;
		if (dwElapsed < PLAYER_SYNC_LIMIT)
		{
			//TCHAR szError[256] = _T("");
			//_stprintf(szError, _T("You must wait for %d seconds before redrawing client."), (PLAYER_SYNC_LIMIT - dwElapsed) / 1000);
			//server->GamePrintError(szError);
			return FALSE;
		}
	}
	
	DWORD dwPlayerID = GetObjectID();
	WORD wPlayerX = GetPositionX();
	WORD wPlayerY = GetPositionY();

	BYTE aPacket[11] = { 0 };
	aPacket[0] = 0x15;
	::memcpy(aPacket + 2, &dwPlayerID, 4);
	::memcpy(aPacket + 6, &wPlayerX, 2);
	::memcpy(aPacket + 8, &wPlayerY, 2);

	if (bFade)
		aPacket[10] = 0x01;

	if (!SendPacketToGame(aPacket, 11))
	{
		DebugMessage(_T("Unable to send RedrawClient packet"));
		return FALSE;
	}

	m_dwLastSyncTime = ::GetTickCount();
	m_iLastSyncMap = m_iCurMap;
	return TRUE;
}

void CD2Player::OnLSpell(WORD wOldSpell, WORD wNewSpell)
{

}

void CD2Player::OnRSpell(WORD wOldSpell, WORD wNewSpell)
{

}

BOOL CD2Player::RepairItem(const CD2Unit *pNpc, DWORD dwItemID) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		DebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	DWORD dwNpcID = pNpc->GetObjectID();
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x35;
	::memcpy(aPacket + 1, &dwNpcID, 4);

	if (dwItemID)
	{
		// repair a particular item
		::memcpy(aPacket + 5, &dwItemID, 4);
	}
	else
	{
		// repair all
		aPacket[16] = 0x80;
	}

	if (!SendPacketToServer(aPacket, 17, 2))
	{
		DebugMessage(_T("Unable to send RepairItem packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::Say(LPCTSTR lpszMessage) const
{
	if (lpszMessage == NULL || lpszMessage[0] == _T('\0'))
		return FALSE;

	const int LEN = _tcslen(lpszMessage);
	BYTE* aPacket = new BYTE[LEN + 6];
	memset(aPacket, 0, LEN + 6);

	aPacket[0] = 0x15;
	aPacket[1] = 0x01;
	::memcpy(aPacket + 3, lpszMessage, LEN);
	BOOL bOK = SendPacketToServer(aPacket, LEN + 6);
	if (!bOK)
		DebugMessage(_T("Unable to send Say packet"));

	delete [] aPacket;
	return bOK;
}

BOOL CD2Player::DepositGoldToStash(DWORD dwAmount)
{
	if (dwAmount == 0)
		dwAmount = m_stats.GetStats(D2PS_GOLD);

	BYTE aPacket[7] = { 0 };
	WORD g1 = 0, g2 = 0;

	// build deposit packet
	aPacket[0] = 0x4f;
	aPacket[1] = 0x14;

	g2 = WORD(dwAmount / 65536);
	g1 = WORD(dwAmount % 65536);
	
	::memcpy(&aPacket[3], &g2, 2);
	::memcpy(&aPacket[5], &g1, 2);

	if (!SendPacketToServer(aPacket, 7))
	{
		DebugMessage(_T("Unable to send DepositGoldToStash packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::ResurrectMerc(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		DebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	DWORD dwNpcID = pNpc->GetObjectID();
	BYTE aPacket[5] = { 0 };
	aPacket[0] = 0x62;
	::memcpy(aPacket + 1, &dwNpcID, 4);

	if (!SendPacketToServer(aPacket, 5))
	{
		DebugMessage(_T("Unable to send ResurrectMerc packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::AntiIdle() const
{
	DWORD dwTick = ::GetTickCount();
	if (dwTick % 2)
	{
		return OverHead(_T("Anti-idle"), FALSE);
	}
	else
	{
		// send a random sound
		BYTE i = BYTE(dwTick % 0x08) + 0x19;
		return SendSound(i);
	}
}

BOOL CD2Player::OverHead(LPCTSTR lpszMessage, BOOL bClientOnly) const
{
	if (lpszMessage == NULL || lpszMessage[0] == _T('\0'))
		return FALSE;

	const int MSGLEN = _tcslen(lpszMessage);
	BYTE* aPacket = new BYTE[MSGLEN + 12];
	::memset(aPacket, 0, MSGLEN + 12);

	BOOL bOK = FALSE;
	if (bClientOnly)
	{
		DWORD dwPlayerID = GetObjectID();
		aPacket[0] = 0x26;
		aPacket[1] = 0x05;
		::memcpy(aPacket + 4, &dwPlayerID, 4);
		WORD wRand = (rand() % 65535) + 1;
		::memcpy(aPacket + 8, &wRand, 2);
		::memcpy(aPacket + 11, lpszMessage, MSGLEN); // copy whole string including the tail-null
		bOK = SendPacketToGame(aPacket, MSGLEN + 12);
	}
	else
	{
		aPacket[0] = 0x14;
		::memcpy(aPacket + 3, lpszMessage, MSGLEN);
		bOK = SendPacketToServer(aPacket, MSGLEN + 6);
	}

	if (!bOK)
		DebugMessage(_T("Unable to send OverHead packet"));

	delete [] aPacket;
	return bOK;
}

BOOL CD2Player::SendSound(BYTE iSound) const
{
	// 0x19 - 0x20
	iSound = max(0x19, iSound);
	iSound = min(0x20, iSound);

	BYTE aPacket[3] = { 0 };
	aPacket[0] = 0x3f;
	aPacket[1] = iSound;

	if (!SendPacketToServer(aPacket, 3))
	{
		DebugMessage(_T("Unable to send SendSound packet"));
		return FALSE;
	}

	return TRUE;

}

BOOL CD2Player::SelectSpell(WORD wSpell, BOOL bLeft, BOOL bVerifySpell)
{
	if (!D2GetSpellInfo(wSpell, &m_lastD2s))
	{
		FormatAndDebugMessage(_T("%04X is not a valid spell."), wSpell);
		return FALSE;
	}

	if (wSpell != D2S_INVALID
		&& ((bLeft && wSpell == m_wLeftSpell) || (!bLeft && wSpell == m_wRightSpell)))
		return TRUE;

	if (bVerifySpell && !HasSpell(wSpell))
	{
		FormatAndDebugMessage(_T("Player does not have spell \"%s\""), m_lastD2s.szSpellName);
		return FALSE;
	}	

	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x3c;
	memcpy(aPacket + 1, &wSpell, 2);
	aPacket[4] = bLeft ? 0x80 : 0x00;
	aPacket[5] = aPacket[6] =aPacket[7] = aPacket[8] = 0xff;

	if (!SendPacketToServer(aPacket, 9, 2))
	{
		DebugMessage(_T("Unable to send SelectSpell packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::CastSpell(WORD wSpell, BOOL bLeft, WORD x, WORD y, BOOL bVerifySpell)
{
	//if (GetDistance(x, y) > 99)
	//{
	//	FormatAndDebugMessage(_T("Warning: %04X, %04X is not a nearby position."), x, y);
		//return FALSE;
	//}

	if (!SelectSpell(wSpell, bLeft, bVerifySpell))
		return FALSE;
	
	BYTE aPacket[17] = { 0 };
	aPacket[0] = bLeft ? 0x05 : 0x0c;
	memcpy(aPacket + 1, &x, 2);
	memcpy(aPacket + 3, &y, 2);	

	if (!SendPacketToServer(aPacket, 5))
	{
		DebugMessage(_T("Unable to send CastSpell packet"));
		return FALSE;
	}

	if (IsGameReady())
	{
		DWORD dwPlayerID = GetObjectID();
		::memset(aPacket, 0, sizeof(aPacket));
		aPacket[0] = 0x4d;
		::memcpy(aPacket + 2, &dwPlayerID, 4);
		::memcpy(aPacket + 6, &wSpell, 2);
		aPacket[10] = 0x01;
		::memcpy(aPacket + 11, &x, 2);
		::memcpy(aPacket + 13, &y, 2);
		SendPacketToGame(aPacket, 17);
	}

	return TRUE;	
}

BOOL CD2Player::CastSpell(WORD wSpell, BOOL bLeft, CD2StaticObject *pTarget, BOOL bVerifySpell)
{
	if (pTarget == NULL || !pTarget->IsValidObject())
	{
		DebugMessage(_T("Taget is not valid."));
		return FALSE;
	}

	return CastSpellOnEntity(wSpell, bLeft, TARGET_TYPE_OBJ, pTarget->GetObjectID(), pTarget->GetPositionX(), pTarget->GetPositionY(), bVerifySpell);
}

BOOL CD2Player::CastSpell(WORD wSpell, BOOL bLeft, const CD2Unit *pTarget, BOOL bVerifySpell)
{
	if (pTarget == NULL || !pTarget->IsValidObject())
	{
		DebugMessage(_T("Taget is not valid."));
		return FALSE;
	}

	return CastSpellOnEntity(wSpell, bLeft, TARGET_TYPE_UNIT, pTarget->GetObjectID(), pTarget->GetPositionX(), pTarget->GetPositionY(), bVerifySpell);
}

BOOL CD2Player::CastSpellOnEntity(WORD wSpell, BOOL bLeft, DWORD dwType, DWORD dwEntityID, WORD x, WORD y, BOOL bVerifySpell)
{
	//WORD wDistance = GetDistance(x, y);
	//if (wDistance > 99)
	//{
	//	DebugMessage(_T("Warning: Target position is not nearby."));
		//return FALSE;
	//}

	if (!SelectSpell(wSpell, bLeft, bVerifySpell))
		return FALSE;

	if (wSpell == m_lastD2s.wSpellID
		&& GetDistance(x, y) > 5
		&& (m_lastD2s.dwSpellInfoFlag & DSI_MELEE))
	{
		// cast a melee-only spell on an entity requires approaching the entity
		GoToEntity(TRUE, dwType, dwEntityID, x, y);
	}

	BYTE aPacket[16] = { 0 };
	aPacket[0] = bLeft ? 0x06 : 0x0d;
	::memcpy(aPacket + 1, &dwType, 4);
	::memcpy(aPacket + 5, &dwEntityID, 4);

	if (!SendPacketToServer(aPacket, 9, 2))
	{
		DebugMessage(_T("Unable to send CastSpellOnEntity packet"));
		return FALSE;
	}

	if (IsGameReady())
	{
		DWORD dwPlayerID = GetObjectID();
		::memset(aPacket, 0, sizeof(aPacket));
		aPacket[0] = 0x4c;
		::memcpy(aPacket + 2, &dwPlayerID, 4);
		::memcpy(aPacket + 6, &wSpell, 2);
		aPacket[8] = 0x01;
		aPacket[9] = (BYTE)dwType;
		::memcpy(aPacket + 10, &dwEntityID, 4);	
		SendPacketToGame(aPacket, 16);
	}

	return TRUE;	
}

WORD CD2Player::GetLeftSpell() const
{
	return m_wLeftSpell;
}

WORD CD2Player::GetRightSpell() const
{	
	return m_wRightSpell;
}

DWORD CD2Player::GetInventoryGoldLimit()
{
	return MAXGOLD(GetPlayerStats(D2PS_LEVEL));
}

DWORD CD2Player::HealMerc(BOOL bPurpleOnly) const
{
	LPCITEM p = FindPurplePotion();
	if (p == NULL && !bPurpleOnly)
		p = FindRedPotion();

	return DrinkPotion(p, TRUE) ? D2ItemCode2Dword(p->szItemCode) : 0;
}

DWORD CD2Player::HealMe(BOOL bPurpleOnly) const
{
	LPCITEM p = FindPurplePotion();
	if (p == NULL && !bPurpleOnly)
		p = FindRedPotion();

	return DrinkPotion(p, FALSE) ? D2ItemCode2Dword(p->szItemCode) : 0;
}

DWORD CD2Player::SupplyMana(BOOL bDrinkPurpleIfNoBlue) const
{
	LPCITEM p = FindBluePotion();
	if (p == NULL && bDrinkPurpleIfNoBlue)
		p = FindPurplePotion();

	return DrinkPotion(p, FALSE) ? D2ItemCode2Dword(p->szItemCode) : 0;
}

int CD2Player::DrinkAllDrunks() const
{
	int nCount = 0;
	for (BYTE row = 0; row < m_iBeltRows; row++)
	{
		for (BYTE col = 0; col < 4; col++)
		{
			int nCata = D2GetBeltItemCategory(m_aBeltItems[row][col].szItemCode);
			if (nCata == BELT_CATA_UNKNOWN || nCata == BELT_CATA_REJUV)
				continue;

			// all others are considered junks
			if (DrinkPotion(&m_aBeltItems[row][col], FALSE))
				nCount++;
		}
	}
	return nCount;
}

void CD2Player::OnTeleport(BYTE iAct, WORD x, WORD y)
{

}

BOOL CD2Player::PickItem(LPCGROUNDITEM pItem, BOOL bShowClientMove)
{
	if (pItem == NULL || pItem->dwID == 0 )
	{
		DebugMessage(_T("Item or item position are not valid."));
		return FALSE;
	}

	if (GetDistance(pItem->x, pItem->y) > 5)
		GoToEntity(TRUE, TARGET_TYPE_ITEM, pItem->dwID, pItem->x, pItem->y, bShowClientMove);

	BYTE aPacket[13] = { 0 };
	aPacket[0] = 0x16;
	aPacket[1] = 0x04;
	::memcpy(aPacket + 5, &pItem->dwID, 4);

	if (!SendPacketToServer(aPacket, 13))
	{
		DebugMessage(_T("Unable to send PickItem packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::Telekinese(LPCGROUNDITEM pItem, BOOL bVerifySpell)
{
	if (pItem == NULL || pItem->dwID == 0)
	{
		DebugMessage(_T("Item or item position are not valid."));
		return FALSE;
	}

	return CastSpellOnEntity(D2S_TELEKINESIS, FALSE, TARGET_TYPE_ITEM, pItem->dwID, pItem->x, pItem->y, bVerifySpell);
}

BOOL CD2Player::TeleportTo(WORD x, WORD y, BOOL bPreemptive)
{
	if (!HasSpell(D2S_TELEPORT))
	{
		DebugMessage(_T("Player does not have spell \"Teleport\"."));
		return FALSE;
	}

	if (bPreemptive)
		CleanJobs();

	// queue this new job
	MOVEJOB job = { 0 };
	job.bShowMove = TRUE;
	job.nJobType = MJ_TELEPORT;
	job.x = x;
	job.y = y;
	m_aJobQueue.InsertRear(job);
	return TRUE;
}

BOOL CD2Player::CastSpell(WORD wSpell, BOOL bLeft, BOOL bVerifySpell)
{
	return CastSpell(wSpell, bLeft, GetPositionX(), GetPositionY(), bVerifySpell);
}

DWORD CD2Player::GetInventoryGoldSpace()
{
	DWORD dwLimit = GetInventoryGoldLimit();
	return dwLimit - GetPlayerStats(D2PS_GOLD);
}

BOOL CD2Player::IsMoving() const
{
	if (m_thisPlayer.pPlayerLocationAddr == NULL)
		return FALSE;

	return *m_thisPlayer.pPlayerLocationAddr != 0x05 && *m_thisPlayer.pPlayerLocationAddr != 0x01;
}

UINT CD2Player::GetSpellCount() const
{
	return (UINT)m_aSkillList.GetSize() + (UINT)m_aChargeableSkillList.GetSize();
}

void CD2Player::OnSpellCharges(WORD wSpell, BYTE iChargesLeft)
{

}

BOOL CD2Player::AnalyzeMapData()
{
	if (!m_mapTicker.IsTickEnded())
		return FALSE;

	m_mapTicker.ResetTicker(MAP_CHECKTICK);
	BYTE iNewMapID = FindCurrentMapID();
	if (iNewMapID == m_iCurMap)
		return FALSE;

	BYTE iOldMapID = m_iCurMap;
	m_iCurMap = iNewMapID;

	// map changed
	OnMapChange(iOldMapID, iNewMapID);
	FormatAndDebugMessage(_T("Map Changed: %02X - %02X"), iOldMapID, iNewMapID);
	NotifyD2Message(PM_MAPCHANGE, (WPARAM)iOldMapID, (LPARAM)iNewMapID);	

	// check enter/leave town messages
	BOOL bWasTown = D2MapIsTown(iOldMapID);
	BOOL bIsTown = D2MapIsTown(iNewMapID);
	if (bWasTown && !bIsTown)
	{
		// leave town
		OnLeaveTown(iOldMapID, iNewMapID);
		FormatAndDebugMessage(_T("Left Town: %02X - %02X"), iOldMapID, iNewMapID);
		NotifyD2Message(PM_LEAVETOWN, (WPARAM)iOldMapID, (LPARAM)iNewMapID);
	}
	else if (!bWasTown && bIsTown)
	{
		// enter town
		OnEnterTown(iOldMapID, iNewMapID);
		FormatAndDebugMessage(_T("Enter Town: %02X - %02X"), iOldMapID, iNewMapID);
		NotifyD2Message(PM_ENTERTOWN, (WPARAM)iOldMapID, (LPARAM)iNewMapID);
	}	

	return TRUE;
}

void CD2Player::OnMapChange(BYTE iOldMapID, BYTE iNewMapID)
{

}

void CD2Player::OnEnterTown(BYTE iOldMapID, BYTE iNewMapID)
{

}

void CD2Player::OnLeaveTown(BYTE iOldMapID, BYTE iNewMapID)
{

}

BOOL CD2Player::IsPlayerInTown() const
{
	return D2MapIsTown(m_iCurMap);
}

BYTE CD2Player::GetCurrentMapID() const
{
	return m_iCurMap;
}

BOOL CD2Player::GoToPosition(BOOL bRun, WORD x, WORD y, BOOL bShowClientMove) const
{
	//if (GetDistance(x, y) > 99)
	//{
	//	FormatAndDebugMessage(_T("Warning: %04X, %04X is not a nearby position."), x, y);
	//	return FALSE;
	//}
	BYTE aPacket[16] = { 0 };
	aPacket[0] = 0x03;
	::memcpy(&aPacket[1], &x, 2);
	::memcpy(&aPacket[3], &y, 2);

	// send 3 times
	if (!SendPacketToServer(aPacket, 5, 3))
		return FALSE;

	if (bShowClientMove && IsGameReady())
	{
		DWORD dwPlayerID = GetObjectID();
		WORD wPlayerX = GetPositionX();
		WORD wPlayerY = GetPositionY();
		::memset(aPacket, 0, sizeof(aPacket));
		aPacket[0] = 0x0f;
		::memcpy(aPacket + 2, &dwPlayerID, 4);
		aPacket[6] = bRun ? 0x17 : 0x01;
		::memcpy(aPacket + 7, &x, 2); // New position x
		::memcpy(aPacket + 9, &y, 2); // New position y
		::memcpy(aPacket + 12, &wPlayerX, 2); // Old position X
		::memcpy(aPacket + 14, &wPlayerY, 2); // Old position Y	
		SendPacketToGame(aPacket, 16);
	}
	
	return TRUE;
}

void CD2Player::OnMoveComplete(BOOL bSuceeded, LPCMOVEJOB pLastJob)
{

}

LPCITEM CD2Player::FindBluePotion() const
{
	LPCITEM p = FindFirstBeltItem(_T("mp5"));

	if (!p)
		p = FindFirstBeltItem(_T("mp4"));

	if (!p)
		p = FindFirstBeltItem(_T("mp3"));

	if (!p)
		p = FindFirstBeltItem(_T("mp2"));

	if (!p)
		p = FindFirstBeltItem(_T("mp1"));

	return p;
}

LPCITEM CD2Player::FindRedPotion() const
{
	LPCITEM p = FindFirstBeltItem(_T("hp5"));

	if (!p)
		p = FindFirstBeltItem(_T("hp4"));

	if (!p)
		p = FindFirstBeltItem(_T("hp3"));

	if (!p)
		p = FindFirstBeltItem(_T("hp2"));

	if (!p)
		p = FindFirstBeltItem(_T("hp1"));

	return p;
}

LPCITEM CD2Player::FindPurplePotion() const
{
	LPCITEM p = FindFirstBeltItem(_T("rvl"));
	if (!p)
		p = FindFirstBeltItem(_T("rvs"));
	return p;
}

BOOL CD2Player::DrinkPotion(LPCITEM lpItem, BOOL bFeedMerc) const
{
	if (lpItem == NULL || lpItem->dwItemID == 0)
		return FALSE;

	BYTE aPacket[13] = { 0x26 };
	memcpy(aPacket + 1, &lpItem->dwItemID, 4);
	aPacket[5] = bFeedMerc ? 0x01 : 0x00;
	memset(aPacket + 6, 0x00, 7);
	return SendPacketToServer(aPacket, 13);	
}

BOOL CD2Player::IsBeltColFull(int nCol) const
{
	for (BYTE row = 0; row < m_iBeltRows; row++)
	{
		for (BYTE col = 0; col < 4; col++)
		{
			if (m_aBeltItems[row][col].dwItemID == 0)
				return FALSE;
		}
	}

	return TRUE;
}

LPCITEM CD2Player::GetBeltItem(BYTE iRow, BYTE iCol) const
{
	if (iRow < 0 || iRow >= m_iBeltRows || iCol < 0 || iCol > 3)
		return NULL;
	
	return &m_aBeltItems[iRow][iCol];
}

LPCITEM CD2Player::FindFirstBeltItem(LPCTSTR lpszItemCode) const
{
	if (lpszItemCode == NULL)
		return NULL;

	for (BYTE row = 0; row < m_iBeltRows; row++)
	{
		for (BYTE col = 0; col < 4; col++)
		{
			if (_tcsicmp(lpszItemCode, m_aBeltItems[row][col].szItemCode) == 0)
				return &m_aBeltItems[row][col];
		}
	}

	return NULL;
}

BOOL CD2Player::CanItemGotoBelt(LPCTSTR lpszItemCode) const
{
	if (m_iBeltRows < 1 || lpszItemCode == NULL)
		return FALSE;

	int nCata = D2GetBeltItemCategory(lpszItemCode);
	if (nCata == BELT_CATA_UNKNOWN)
		return FALSE;

	BYTE col = 0;
	for (col = 0; col < 4; col++)
	{
		if (!IsBeltColFull(col) && nCata == D2GetBeltItemCategory(m_aBeltItems[0][col].szItemCode))
			return TRUE;
	}

	// now look for a col whose first slot is empty
	for (col = 0; col < 4; col++)
	{
		if (m_aBeltItems[0][col].dwItemID == 0)
			return TRUE;
	}

	return FALSE;
}

BYTE CD2Player::GetBeltRows() const
{
	return m_iBeltRows;
}

BOOL CD2Player::IsBeltFull() const
{
	for (BYTE row = 0; row < m_iBeltRows; row++)
	{
		for (BYTE col = 0; col < 4; col++)
		{
			if (m_aBeltItems[row][col].dwItemID == 0)
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CD2Player::RunTo(LPCGROUNDITEM pItem, BOOL bRun, BOOL bPreemptive)
{
	if (pItem == NULL || pItem->dwID == 0)
 	{
 		DebugMessage(_T("Item or item position are not valid."));
 		return FALSE;
 	}

	if (bPreemptive)
		CleanJobs();

	// queue this new job
	MOVEJOB job = { 0 };
	job.bRun = bRun;
	job.bShowMove = TRUE;
	job.nJobType = MJ_ITEM;
	job.dwEntityID = pItem->dwID;
	job.x = pItem->x;
	job.y = pItem->y;
	m_aJobQueue.InsertRear(job);

	return TRUE;
}

BOOL CD2Player::RunTo(LPCPLAYEROBJ pPlayer, BOOL bRun, BOOL bPreemptive)
{
	if (pPlayer == NULL || pPlayer->dwID == 0)
 	{
 		DebugMessage(_T("Target player or player position are not valid."));
 		return FALSE;
 	}

	if (bPreemptive)
		CleanJobs();

	// queue this new job
	MOVEJOB job = { 0 };
	job.bRun = bRun;
	job.bShowMove = TRUE;
	job.nJobType = MJ_PLAYER;
	job.dwEntityID = pPlayer->dwID;
	job.x = pPlayer->x;
	job.y = pPlayer->y;
	m_aJobQueue.InsertRear(job);

	return TRUE;
}

BOOL CD2Player::WeaponSwitch() const
{
	BYTE aPacket[1] = { 0x60 };
	if (!SendPacketToServer(aPacket, 1))
	{
		DebugMessage(_T("Unable to send WeaponSwitch packet"));
		return FALSE;
	}

	return TRUE;
}

void CD2Player::OnWeaponSwitch()
{

}

void CD2Player::CleanJobs()
{
	m_aJobQueue.RemoveAll();
	::memset(&m_prevJob, 0, sizeof(MOVEJOB));
	m_ticker.ResetTicker(0);
	m_nTeleportFail = 0;
}

BOOL CD2Player::DoFirstJob()
{
	POSITION pos = m_aJobQueue.GetHeadPosition();
	if (pos == NULL)
		return TRUE;

	// there are jobs in queue
	m_prevJob = m_aJobQueue.GetNext(pos);
	m_aJobQueue.RemoveFront();

	BOOL bResult = FALSE;
	switch (m_prevJob.nJobType)
	{
	case MJ_POSITION:
		bResult = GoToPosition(m_prevJob.bRun, m_prevJob.x, m_prevJob.y);
		break;

	case MJ_STATICOBJ:
		bResult = GoToEntity(m_prevJob.bRun, TARGET_TYPE_OBJ, m_prevJob.dwEntityID, m_prevJob.x, m_prevJob.y);
		break;

	case MJ_UNIT:
		bResult = GoToEntity(m_prevJob.bRun, TARGET_TYPE_UNIT, m_prevJob.dwEntityID, m_prevJob.x, m_prevJob.y);
		break;

	case MJ_PLAYER:
		bResult = GoToEntity(m_prevJob.bRun, TARGET_TYPE_PLAYER, m_prevJob.dwEntityID, m_prevJob.x, m_prevJob.y);
		break;

	case MJ_ITEM:
		bResult = GoToEntity(m_prevJob.bRun, TARGET_TYPE_ITEM, m_prevJob.dwEntityID, m_prevJob.x, m_prevJob.y);
		break;

	case MJ_TELEPORT:
		m_nTeleportFail = 0;
		m_wXBeforeTele = GetPositionX();
		m_wYBeforeTele = GetPositionY();
		bResult = CastSpell(D2S_TELEPORT, FALSE, m_prevJob.x, m_prevJob.y, FALSE);
		m_bTeleported = FALSE;
		if (bResult)
			m_ticker.ResetTicker(TELEPORT_TICK);
		break;

	default:
		bResult = FALSE;
		break;
	}

	if (!bResult)
		FailJobs();

	return bResult;
}

void CD2Player::FailJobs()
{	
	MOVEJOB job = m_prevJob;
	CleanJobs();
	OnMoveComplete(FALSE, &job);
	DebugMessage(_T("Move Failed"));
	NotifyD2Message(PM_MOVECOMPLETE, (WPARAM)0, (LPARAM)&job);
}

BOOL CD2Player::BackToTown(int nMaxRetry)
{
	if (m_nTPState != TS_NONE)
		return FALSE; // already in middle of a tp process

	CleanJobs(); // Action "Back to town" has higher priority than any other jobs
	m_nTPMaxRetry = nMaxRetry;
	m_nTPState = TS_CAST_PORTAL;
	m_nTPCastfail = 0;
	m_nTPEnterFail = 0;
	m_ticker.ResetTicker(0);
	return TRUE;
}

void CD2Player::DoTPProc()
{
	if (IsPlayerInTown())
	{
		// already in town
		m_nTPState = TS_NONE;
		DebugMessage("You are in town.");
		return;
	}

	WORD wSpell = D2S_TOMEOFTOWNPORTAL;

	switch (m_nTPState)
	{
	case TS_CAST_PORTAL:

		// cast it
		wSpell = D2S_TOMEOFTOWNPORTAL;
		if (HasSpell(D2S_SCROLLOFTOWNPORTAL))
		{
			// use scroll first, if there are any
			wSpell = D2S_SCROLLOFTOWNPORTAL;
		}		
				
		::memset(&m_portal, 0, sizeof(PLAYEROBJ));
		CastSpell(wSpell, FALSE, FALSE);
		m_nTPState = TS_WAIT_PORTAL;
		m_ticker.ResetTicker(TP_CAST_TICK);		
		break;

	case TS_WAIT_PORTAL:

		// portal didn't appear
		if (m_nTPCastfail >= m_nTPMaxRetry)
		{
			// maximum retries reached and portal still did not appear,
			// assume char was already in town, or ran out of TP scrolls,
			// in either case, portal will never appear.
			server->GamePrintError("Failed to cast town portal. Out of TP scrolls?");
			m_nTPState = TS_NONE;
			DebugMessage(_T("Failed to back to town."));
			OnTownFail();
			NotifyD2Message(PM_TOWNFAIL, (WPARAM)0, (LPARAM)0);
		}
		else
		{
			// retry and prompt every seconds
			m_nTPCastfail++;
			server->GamePrintError("Trying to cast town portal...");
			m_nTPState = TS_CAST_PORTAL;
		}
		break;

	case TS_ENTER_PORTAL:
		// Walk in.
		if (GetDistance(m_portal.x, m_portal.y) > 5)
			GoToEntity(TRUE, TARGET_TYPE_OBJ, m_portal.dwID, m_portal.x, m_portal.y);
		InteractWithEntity(TARGET_TYPE_OBJ, m_portal.dwID);
		m_ticker.ResetTicker(TP_ENTER_TICK);
		m_nTPState = TS_VERIFY_INTOWN;
		break;

	case TS_VERIFY_INTOWN:
		if (m_nTPEnterFail >= m_nTPMaxRetry * 2)
		{
			server->GamePrintError("Failed to enter TP");
			m_nTPState = TS_NONE;
			DebugMessage(_T("Failed to back to town."));
			OnTownFail();
			NotifyD2Message(PM_TOWNFAIL, (WPARAM)0, (LPARAM)0);
		}
		else
		{
			m_nTPEnterFail++;
			m_nTPState = TS_ENTER_PORTAL;
			if (m_nTPEnterFail % 2)
				server->GamePrintError("Trying to enter town portal...");
		}

		break;

	default:
		break;
	}
}

void CD2Player::OnTownFail()
{

}

void CD2Player::OnInventoryFull()
{

}

BYTE CD2Player::GetPlayerClass() const
{
	return m_thisPlayer.iCharClass;
}

DWORD CD2Player::GetPlayerName(LPTSTR lpszBuffer, DWORD dwMaxChars) const
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (dwMaxChars == 0 || m_thisPlayer.szPlayerName == NULL)
		return 0;

	::memset(lpszBuffer, 0, sizeof(TCHAR) * dwMaxChars);
	_tcsncpy(lpszBuffer, m_thisPlayer.szPlayerName, dwMaxChars);
	return _tcslen(lpszBuffer);
}

BOOL CD2Player::CastSpell(WORD wSpell, BOOL bLeft, LPCMONSTERCORPSE pTarget, BOOL bVerifySpell)
{
	if (pTarget == NULL || pTarget->dwCorpseID == 0)
	{
		DebugMessage(_T("Taget is not valid."));
		return FALSE;
	}

	return CastSpellOnEntity(wSpell, bLeft, TARGET_TYPE_UNIT, pTarget->dwCorpseID, pTarget->x, pTarget->y, bVerifySpell);
}

void CD2Player::StepDone()
{
	m_nTeleportFail = 0;
	if (m_aJobQueue.IsEmpty())
	{
		// no more jobs left, notify the application that job is completed
		MOVEJOB job = m_prevJob;
		::memset(&m_prevJob, 0, sizeof(MOVEJOB));
		OnMoveComplete(TRUE, &job);
		FormatAndDebugMessage(_T("Move Completed: %04X, %04X"), job.x, job.y);
		NotifyD2Message(PM_MOVECOMPLETE, (WPARAM)1, (LPARAM)&job);
	}
	else
	{
		// There are more jobs, do next job
		DoFirstJob();
	}	
}

BOOL CD2Player::IsValidObject() const
{
	return GetObjectID() != 0;
}

DWORD CD2Player::GetObjectID() const
{
	return m_thisPlayer.dwPlayerID;
}

WORD CD2Player::GetPositionX() const
{
	return m_thisPlayer.pPlayerXAddr ? *m_thisPlayer.pPlayerXAddr : 0;
}

WORD CD2Player::GetPositionY() const
{
	return m_thisPlayer.pPlayerYAddr ? *m_thisPlayer.pPlayerYAddr : 0;
}

BOOL CD2Player::SetFingerprinter(LPCD2OBJFINGERPRINTER lpFingerprinter)
{
	return FALSE;
}

void CD2Player::LockSpawn(BOOL bLock)
{

}

DWORD CD2Player::GetPlayerStats(DWORD dwStatsID)
{
	return m_stats.GetStats(dwStatsID);
}

BYTE CD2Player::GetHPPercent()
{
	DWORD dwVal = GetPlayerStats(D2PS_HP);
	DWORD dwMax = GetPlayerStats(D2PS_MAXHP);
	if (dwVal == 0 || dwMax == 0)
		return 0;

	return min(100, max(1, (double)dwVal * 100.0 / (double)dwMax));
}

BYTE CD2Player::GetManaPercent()
{
	DWORD dwVal = GetPlayerStats(D2PS_MANA);
	DWORD dwMax = GetPlayerStats(D2PS_MAXMANA);
	if (dwVal == 0 || dwMax == 0)
		return 0;

	return min(100, max(1, (double)dwVal * 100.0 / (double)dwMax));
}

BYTE CD2Player::GetItemDurabilityPercent()
{
	DWORD dwVal = GetPlayerStats(D2PS_DURABILITY);
	DWORD dwMax = GetPlayerStats(D2PS_MAXDURABILITY);
	if (dwVal == 0 || dwMax == 0)
		return 0;

	return min(100, max(1, (double)dwVal * 100.0 / (double)dwMax));
}

BOOL CD2Player::FindPlayerStats(DWORD dwStatsID)
{
	return m_stats.HasStats(dwStatsID);
}

void CD2Player::PauseJobs(BOOL bPause)
{
	m_bPaused = bPause;
}

void CD2Player::AnalyzeItem(const ITEM &item)
{
	//////////////////////////////////////////////////////////////////////
	// Belt Items Added/Removed/Shifted
	//////////////////////////////////////////////////////////////////////
	if (item.iAction == ITEM_ACTION_TO_BELTSLOT
		|| item.iAction == ITEM_ACTION_SWITCH_BELTSLOT)
	{
		::memcpy(&m_aBeltItems[item.iAtBeltRow][item.iAtBeltColumn], &item, sizeof(ITEM));
	}

	if (item.iAction == ITEM_ACTION_FROM_BELTSLOT)
	{
		::memset(&m_aBeltItems[item.iAtBeltRow][item.iAtBeltColumn], 0, sizeof(ITEM));
	}

	if (item.iAction == ITEM_ACTION_SHIFT_BELTSLOT)
	{
		::memcpy(&m_aBeltItems[item.iAtBeltRow][item.iAtBeltColumn], &item, sizeof(ITEM));
		for (BYTE row = item.iAtBeltRow + 1; row < 4; row++)
			::memset(&m_aBeltItems[row][item.iAtBeltColumn], 0, sizeof(ITEM));
	}

	//////////////////////////////////////////////////////////////////////
	// Player Belt Equipped/Unequipped
	//////////////////////////////////////////////////////////////////////

	if (item.iAction == ITEM_ACTION_TO_EQUIP || item.iAction == ITEM_ACTION_SWITCH_EQUIP)
	{
		// is belt?
		BYTE iBeltRows = ::D2GetBeltRows(item.szItemCode);
		if (iBeltRows)
			m_iBeltRows = iBeltRows;
	}

	if (item.iAction == ITEM_ACTION_FROM_EQUIP)
	{
		// is belt?
		if (::D2GetBeltRows(item.szItemCode))
			m_iBeltRows = 1;
	}
}

void CD2Player::OnEquipEvent(BYTE iEventType, LPCITEM lpItem)
{

}

void CD2Player::AnalyzeHPMana()
{
	POSITION pos = m_aHPManaCheckList.GetHeadPosition();
	if (pos == NULL)
		return;

	DWORD dwVal = m_aHPManaCheckList.GetAt(pos);
	m_aHPManaCheckList.RemoveFront();

	const WORD HP = LOWORD(dwVal);
	const WORD MANA = HIWORD(dwVal);

	WORD wHP = m_stats.GetStats(D2PS_HP);
	WORD wMana = m_stats.GetStats(D2PS_MANA);

	if (wHP < HP)
	{
		// hp down
		OnHPDown(HP, wHP);
		FormatAndDebugMessage(_T("HP down: %u - %u"), HP, wHP);
		NotifyD2Message(PM_HPDOWN, (WPARAM)HP, (LPARAM)wHP);
	}
	else if (wHP > HP)
	{
		// hp up
		OnHPUp(HP, wHP);
		FormatAndDebugMessage(_T("HP up: %u - %u"), HP, wHP);
		NotifyD2Message(PM_HPUP, (WPARAM)HP, (LPARAM)wHP);
	}

	if (wMana < MANA)
	{
		// mana down
		OnManaDown(MANA, wMana);
		FormatAndDebugMessage(_T("Mana down: %u - %u"), MANA, wMana);
		NotifyD2Message(PM_MANADOWN, (WPARAM)MANA, (LPARAM)wMana);
	}
	else if (wMana > MANA)
	{
		// mana up
		OnManaUp(MANA, wMana);
		FormatAndDebugMessage(_T("Mana up: %u - %u"), MANA, wMana);
		NotifyD2Message(PM_MANAUP, (WPARAM)MANA, (LPARAM)wMana);
	}
}

BOOL CD2Player::IdentifyItems(const CD2Unit *pNpc) const
{
	if (pNpc == NULL || !pNpc->IsValidObject())
	{
		DebugMessage(_T("NPC data is not valid."));
		return FALSE;
	}

	DWORD dwNpcID = pNpc->GetObjectID();
	BYTE aPacket[5] = { 0 };
	aPacket[0] = 0x34;
	::memcpy(aPacket + 1, &dwNpcID, 4);

	if (!SendPacketToServer(aPacket, 5))
	{
		DebugMessage(_T("Unable to send IdentifyItems packet"));
		return FALSE;
	}

	return TRUE;
}

BOOL CD2Player::InteractWithCurrentNpc() const
{
	if (m_pNpc == NULL || !m_pNpc->IsValidObject())
		return FALSE;

	// Prepare
	PrepareInteractWithNPC(m_pNpc->GetObjectID(), m_pNpc->GetPositionX(), m_pNpc->GetPositionY());

	// If entity is more then 5 yards away, run to it
	if (GetDistance(m_pNpc->GetPositionX(), m_pNpc->GetPositionY()) > 5)
		GoToEntity(TRUE, TARGET_TYPE_UNIT, m_pNpc->GetObjectID(), m_pNpc->GetPositionX(), m_pNpc->GetPositionY());

	// talk
	return InteractWithEntity(TARGET_TYPE_UNIT, m_pNpc->GetObjectID());
}

void CD2Player::NPCEnd(BOOL bSuccessful)
{
	if (m_pNpc == NULL)
		return;

	DWORD NPCID = m_pNpc->GetObjectID();
	m_pNpc = NULL;
	m_nNpcFails = 0;
	m_nNpcMaxRetry = 0;
	m_ticker.ResetTicker();
	OnNpcEnd(bSuccessful, NPCID);
	FormatAndDebugMessage(_T("NPC ended %d %08X"), bSuccessful, NPCID);
	NotifyD2Message(PM_NPCEND, (WPARAM)bSuccessful, (LPARAM)NPCID);
}

void CD2Player::OnNpcEnd(BOOL bSuccessful, DWORD dwNpcID)
{

}

void CD2Player::OnStashOpen(BYTE iAct)
{

}

void CD2Player::OnWPOpen(BYTE iAct, BYTE iMapID)
{

}

BOOL CD2Player::CloseWP(const CD2StaticObject *pWP) const
{
	if (pWP == NULL || !pWP->IsValidObject())
	{
		DebugMessage(_T("WP data is not valid."));
		return FALSE;
	}

	StopInteractionUsingSpaceKey();
	DWORD dwID = pWP->GetObjectID();
	BYTE aPacket[9] = { 0 };
	aPacket[0] = 0x49;
	::memcpy(aPacket + 1, &dwID, 4);
	return SendPacketToServer(aPacket, 9);
}

BOOL CD2Player::CloseStash() const
{
	StopInteractionUsingSpaceKey();
	BYTE aPacket[7] = { 0 };
	aPacket[0] = 0x4f;
	aPacket[1] = 0x12;
	return SendPacketToServer(aPacket, 7);
}

void CD2Player::OnActChange(BYTE iOriginalAct, BYTE iCurrentAct)
{

}

BOOL CD2Player::SendChar(TCHAR ch) const
{
	return m_msgSender.SendChar(ch);
}

BOOL CD2Player::SendSysKey(BYTE iVKCode) const
{
	return m_msgSender.SendSysKey(iVKCode);
}

BOOL CD2Player::DblClick(long x, long y) const
{
	return m_msgSender.LDblClick(x, y);
}

BOOL CD2Player::SendString(LPCTSTR lpszString) const
{
	return m_msgSender.SendString(lpszString);
}

BOOL CD2Player::LeftClick(long x, long y) const
{
	return m_msgSender.LeftClick(x, y);
}

BOOL CD2Player::RightClick(long x, long y) const
{
	return m_msgSender.RightClick(x, y);
}

BOOL CD2Player::MouseMove(long x, long y) const
{
	return m_msgSender.MouseMove(x, y);
}	

void CD2Player::StopInteractionUsingSpaceKey(int nTimes) const
{
#ifdef USE_SPACE_KEY_TO_STOP_INTERACTION
	HWND hWnd = server->FindMyD2WndHandle();
	if (::IsWindowVisible(hWnd) && !::IsIconic(hWnd))
	{
		for (int i = 0; i < nTimes; i++)
			m_msgSender.SendSysKey(0x20);
	}	
#endif
}

void CD2Player::DebugMapData() const
{
	D2Infof("Map Count: %d, Current Map ID: %02X", m_aMapDataList.GetSize(), m_iCurMap);
	for (int i = 0; i < m_aMapDataList.GetSize(); i++)
		D2Infof("ID: %02X, Flag: %08X", m_aMapDataList[i].iMapID, m_aMapDataList[i].dwMapFlag);
}

void CD2Player::OnGameReady(BYTE iAct, WORD x, WORD y)
{

}

BYTE CD2Player::FindCurrentMapID()
{
	static MAPCOUNT aMapCounts[MAX_MAPDATA] = {0};
	int nMapCount = 0;	
	
	int nHalf = m_aMapDataList.GetSize() / 2;
	if (nHalf < 2)
		nHalf = 2;

	for (int i = 0; i < m_aMapDataList.GetSize(); i++)
	{
		const int IDX = FindDataCount(aMapCounts, nMapCount, m_aMapDataList[i].iMapID);

		if (IDX == -1)
		{
			aMapCounts[nMapCount].iMapID = m_aMapDataList[i].iMapID;
			aMapCounts[nMapCount].iCount = 1;
			nMapCount++;
		}
		else
		{
			aMapCounts[IDX].iCount++;
			if (aMapCounts[IDX].iCount >= nHalf)
				return aMapCounts[IDX].iMapID;
		}
	}	

	// intersting, no map has half the counter, forgotten tower?
	return FindDataCount(aMapCounts, nMapCount, MAP_A1_FORGOTTEN_TOWER) == -1 ? m_iCurMap : MAP_A1_FORGOTTEN_TOWER;
}

int CD2Player::FindDataCount(LPCMAPCOUNT aCounts, int nCount, BYTE iMapID)
{
	if (aCounts == NULL)
		return -1;

	for (int i = 0; i < nCount; i++)
	{
		if (aCounts[i].iMapID == iMapID)
			return i;
	}
	return -1;
}
