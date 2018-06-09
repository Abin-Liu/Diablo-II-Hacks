//////////////////////////////////////////////////////////////////////
// GameMessage.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "GameMessage.h"
#include "Utilities\\LinkedList.h"
#include "Server20.h"
#include "D2HackIt.h"
#include "D2HackItServer.h"
#include "me.h"
#include "NpcSession.h"
#include "EnterWayPoint.h"

// Game Message
typedef struct tagGameMessage
{
	UINT nMessage;
	WPARAM wParam;
	LPARAM lParam;
	GAMEUNIT unit;
} GAMEMESSAGE, *LPGAMEMESSAGE;
typedef const tagGameMessage* LPCGAMEMESSAGE;

CLinkedList<GAMEMESSAGE, const GAMEMESSAGE&, NULL> g_msgQueue; // Game message queue
WORD g_wLSpell = D2S_INVALID;
WORD g_wRSpell = D2S_INVALID;

void NotifyUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam = 0, LPARAM lParam = 0);
void NotifyThisPlayerMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);

void NotifyUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{	
	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnUnitMessage)
			pCIS->OnUnitMessage(nMessage, lpUnit, wParam, lParam);
		//else
		//	GamePrintError("OnUnitMessage error!");

		li = ClientModules.GetNextItem(li);
	}
}

void NotifyThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{	
	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnThisPlayerMessage)
			pCIS->OnThisPlayerMessage(nMessage, wParam, lParam);
		//else
		//	GamePrintError("OnThisPlayerMessage error!");

		li = ClientModules.GetNextItem(li);
	}
}

void QueueUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (lpUnit == NULL || lpUnit->dwUnitID == 0)
		return;
	
	GAMEMESSAGE msg = { 0 };
	::memcpy(&msg.unit, lpUnit, sizeof(GAMEUNIT));
	msg.nMessage = nMessage;
	msg.wParam = wParam;
	msg.lParam = lParam;

	g_msgQueue.Lock();
	g_msgQueue.InsertRear(msg);
	g_msgQueue.Unlock();
}

void QueueThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	GAMEMESSAGE msg = { 0 };
	msg.nMessage = nMessage;
	msg.wParam = wParam;
	msg.lParam = lParam;

	g_msgQueue.Lock();
	g_msgQueue.InsertRear(msg);
	g_msgQueue.Unlock();
}

void CleanMessageQueue()
{
	g_wLSpell = D2S_INVALID;
	g_wRSpell = D2S_INVALID;
	g_msgQueue.Lock(2000);
	g_msgQueue.RemoveAll();
	g_msgQueue.Unlock();
}

void DispatchGameMessages(const volatile BYTE* lpStopFlag)
{
	if (NEED_ABORT(lpStopFlag))
		return;

	if (!g_msgQueue.Lock(500))
		return;

	POSITION pos = g_msgQueue.GetHeadPosition();
	while (pos && !NEED_ABORT(lpStopFlag))
	{
		const GAMEMESSAGE& msg = g_msgQueue.GetNext(pos);

		if (msg.unit.dwUnitID)
		{
			// unit message
			NotifyUnitMessage(msg.nMessage, &msg.unit, msg.wParam, msg.lParam);
		}
		else
		{
			// this player message
			NotifyThisPlayerMessage(msg.nMessage, msg.wParam, msg.lParam);
		}
	}

	g_msgQueue.RemoveAll();
	g_msgQueue.Unlock();
}

DWORD ServerPacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if (aPacket[0] == 0x49)
	{
		if (!IsWpOpened())
			return 0; // block invalid wp packet
		
		OnWpOpen(FALSE);
	}
	
	if (aLen == 9 && aPacket[0] == 0x30 && aPacket[1] == 0x01)
	{
		DWORD dwNpcID = 0;
		::memcpy(&dwNpcID, aPacket + 5, 4);
		OnNpcClosed(dwNpcID);
	}

	return aLen;
}

DWORD ServerPacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	GAMEUNIT unit;
	WORD x, y;

	//////////////////////////////////////////////////////////////////////
	// Unit Affection
	//////////////////////////////////////////////////////////////////////
	if ((aPacket[0] == 0xa8 || aPacket[0] == 0xaa) && aLen >= 8)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;

		if (unit.dwUnitID == GetSafePlayerID())
			QueueThisPlayerMessage(PM_AFFECT, aPacket[1], aPacket[7]);
		else
			QueueUnitMessage(UM_AFFECT, &unit, aPacket[1], aPacket[7]);
	}
	
	//////////////////////////////////////////////////////////////////////
	// Unit Disaffection
	//////////////////////////////////////////////////////////////////////
	if (aLen >= 7 && aPacket[0] == 0xa9)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;

		if (unit.dwUnitID == GetSafePlayerID())
			QueueThisPlayerMessage(PM_DISAFFECT, aPacket[1], aPacket[6]);
		else
			QueueUnitMessage(UM_DISAFFECT, &unit, aPacket[1], aPacket[6]);
	}

	//////////////////////////////////////////////////////////////////////
	// Static Object Spawned
	//////////////////////////////////////////////////////////////////////
	if (aLen > 12 && aPacket[0] == 0x51)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = aPacket[1];
		WORD x, y;		
		
		::memcpy(&x, aPacket + 8, 2);
		::memcpy(&y, aPacket + 10, 2);
		QueueUnitMessage(UM_SPAWN, &unit, 0, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Spawned
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0xac && aLen >= 13)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 7, 2);
		::memcpy(&y, aPacket + 9, 2);
		QueueUnitMessage(UM_SPAWN, &unit, 0, MAKELPARAM(x, y));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Other Player/Corpse Spawned, My Corpse Spawned
	//////////////////////////////////////////////////////////////////////
	if (aLen == 26 && aPacket[0] == 0x59 && IsGameJoined() && !IsThisPlayerID(aPacket + 1))
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_PLAYER;

		char szPlayerName[17] = "";
		::memcpy(&szPlayerName, aPacket + 6, 17);		
		::memcpy(&x, aPacket + 22, 2);
		::memcpy(&y, aPacket + 24, 2);

		if (stricmp(szPlayerName, GetSafePlayerName()))
		{			
			// it's other player, or other player's corpse	
			QueueUnitMessage(UM_SPAWN, &unit, 0, MAKELPARAM(x, y));
		}
		else
		{
			// it's my corpse
			QueueThisPlayerMessage(PM_CORPSE, unit.dwUnitID, MAKELPARAM(x, y));
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Object/Unit is Destroyed by the Game (be Gone from Player's Vision) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 6 && aPacket[0] == 0x0a)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = aPacket[1];		
		 QueueUnitMessage(UM_DESTROY, &unit);
	}
	
	//////////////////////////////////////////////////////////////////////
	// Monster HP Changed
	//////////////////////////////////////////////////////////////////////
	if (aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x06)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		QueueUnitMessage(UM_HPPERCENT, &unit, CalcPercent(GetUnitStat(&unit, STAT_HP), GetUnitStat(&unit, STAT_MAXHP)));
	}

	//////////////////////////////////////////////////////////////////////
	// Merc Life Percentage Changed
	//////////////////////////////////////////////////////////////////////
	if (aLen == 10 && aPacket[0] == 0x7f && aPacket[1] == 0x00)
	{
		GAMEUNIT unit;
		::memcpy(&unit.dwUnitID, aPacket + 4, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		QueueUnitMessage(UM_HPPERCENT, &unit, CalcPercent(GetUnitStat(&unit, STAT_HP), GetUnitStat(&unit, STAT_MAXHP)));

	}

	//////////////////////////////////////////////////////////////////////
	// Monster Death
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x08)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		QueueUnitMessage(UM_DEATH, &unit);
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Corpse Appeared
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x09)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		QueueUnitMessage(UM_CORPSE, &unit, 0, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Attacking
	//////////////////////////////////////////////////////////////////////
	if (aLen == 21 && aPacket[0] == 0x68)
	{
		GAMEUNIT unitAttacker, unitVictim;
		::memcpy(&unitAttacker.dwUnitID, aPacket + 1, 4);
		::memcpy(&unitVictim.dwUnitID, aPacket + 11, 4);		

		unitAttacker.dwUnitType = UNIT_TYPE_MONSTER;
		unitVictim.dwUnitType = aPacket[10];

		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);

		QueueUnitMessage(UM_ATTACK, &unitAttacker, (WPARAM)&unitVictim, MAKELPARAM(x, y));
		QueueUnitMessage(UM_UNDERATTACK, &unitVictim, (WPARAM)&unitAttacker, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by It's Resurrecter (Shaman, Greater Mummy, etc) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 16 && aPacket[0] == 0x6b)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 12, 2);
		::memcpy(&y, aPacket + 14, 2);
		QueueUnitMessage(UM_REVIVED, &unit, (WPARAM)1, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by Itself (Act5 Zombies) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 17 && aPacket[0] == 0x4d && aPacket[1] == 0x01)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		MAPPOS mp = GetUnitPosition(&unit);
		QueueUnitMessage(UM_REVIVED, &unit, (WPARAM)0, MAKELPARAM(mp.x, mp.y));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Unit Starts Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x67 && aLen == 16)
	{	
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		QueueUnitMessage(UM_STARTMOVE, &unit, 0, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Ends Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x6d && aLen == 10)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 5, 2);
		::memcpy(&y, aPacket + 7, 2);
		QueueUnitMessage(UM_ENDMOVE, &unit, 0, MAKELPARAM(x, y));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Cave Notification
	//////////////////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x09)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = aPacket[1];
		::memcpy(&x, aPacket + 7, 2);
		::memcpy(&y, aPacket + 9, 2);
		QueueUnitMessage(UM_CAVENOTIFY, &unit, aPacket[6], MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////
	// This Let Us Have a Player ID Before OnGameJoin is Called
	//////////////////////////////////////////////////////////
	if (aLen == 26 && aPacket[0] == 0x59)
	{
		DWORD dwPlayerID = 0;
		::memcpy(&dwPlayerID, aPacket + 1, 4);
		SetPlayerInfoBeforeJoin(dwPlayerID, (LPCSTR)aPacket + 6);
	}

	/*
	//////////////////////////////////////////////////////////
	// Charge-able Spells 
	//////////////////////////////////////////////////////////
	if (aLen == 12 && aPacket[0] == 0x22)
	{
		CHARGEABLE ca;
		::memcpy(&ca.wSpellID, aPacket + 7, 2);
		ca.dwCharges = aPacket[9];

		g_aSkillCharges.Lock();
		int nIndex = g_aSkillCharges.Find(ca);
		if (nIndex == -1)
			g_aSkillCharges.Add(ca);
		else
			g_aSkillCharges[nIndex].dwCharges = aPacket[9];	
		g_aSkillCharges.Unlock();

		NotifyThisPlayerMessage(PM_SPELLCHARGES, (WPARAM)ca.wSpellID, (LPARAM)aPacket[9]);
	}
	*/

	//////////////////////////////////////////////////////////
	// Stash Opened
	//////////////////////////////////////////////////////////
	if (aLen == 2 && aPacket[0] == 0x77 && aPacket[1] == 0x10)
	{		
		QueueThisPlayerMessage(PM_STASHOPEN, (WPARAM)GetCurrentAct());
	}

	//////////////////////////////////////////////////////////
	// Waypoint Opened
	//////////////////////////////////////////////////////////
	if (aLen == 21 && aPacket[0] == 0x63)
	{
		OnWpOpen(TRUE);
		QueueThisPlayerMessage(PM_WPOPEN, (WPARAM)GetCurrentAct(), (LPARAM)GetCurrentMapID());
	}	

	//////////////////////////////////////////////////////////////////////
	// Player's Corpse is Picked Up 
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 6 && aPacket[0] == 0x8e && aPacket[1] == 0x00 && IsThisPlayerID(aPacket + 2))
	{
		QueueThisPlayerMessage(PM_CORPSEPICKEDUP);
	}	

	//////////////////////////////////////////////////////////
	// Player Character Level Changed
	//////////////////////////////////////////////////////////
	if (aLen == 3 && aPacket[0] == 0x1d && aPacket[1] == 0x0c)
	{
		QueueThisPlayerMessage(PM_LEVEL, 0, GetStat(STAT_LEVEL));
	}

	//////////////////////////////////////////////////////////
	// Player HP/Mana Updates
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x95)
	{
		WORD wNewHP = *((WORD *)(aPacket + 1)) & 0x0FFF;
		WORD wNewMana = (*((WORD *)(aPacket + 3)) & 0x0FFF) << 1;
		WORD wOldHP = GetStat(STAT_HP);
		WORD wOldMana = GetStat(STAT_MANA);
		
		if (wOldHP < wNewHP)
		{
			// hp up
			QueueThisPlayerMessage(PM_HPUP, wOldHP, wNewHP);
		}
		else if (wOldHP > wNewHP)
		{
			// hp down
			QueueThisPlayerMessage(PM_HPDOWN, wOldHP, wNewHP);
		}

		if (wOldMana < wNewMana)
		{
			// mana up
			QueueThisPlayerMessage(PM_MANAUP, wOldMana, wNewMana);
		}
		else if (wOldMana > wNewMana)
		{
			// mana down
			QueueThisPlayerMessage(PM_MANADOWN, wOldMana, wNewMana);
		}	
	}

	//////////////////////////////////////////////////////////
	// Player Death
	//////////////////////////////////////////////////////////
	if (aLen >= 6 && aPacket[0] == 0x8e && aPacket[1] == 0x01)
	{
		DWORD dwPlayerID;
		::memcpy(&dwPlayerID, aPacket + 2, 4);
		if (dwPlayerID == GetSafePlayerID())
		{
			if (!IsInTown())
			{
				// i'm dead
				QueueThisPlayerMessage(PM_DEATH);
			}
		}
		else
		{
			// other player dead
			GAMEUNIT gu;
			gu.dwUnitID = dwPlayerID;
			gu.dwUnitType = UNIT_TYPE_PLAYER;
			QueueUnitMessage(UM_DEATH, &gu);
		}
	}

	//////////////////////////////////////////////////////////
	// Party invitation/Joining
	//////////////////////////////////////////////////////////
	if (aLen == 6 && aPacket[0] == 0x8b)
	{
		DWORD dwPlayerID;
		::memcpy(&dwPlayerID, aPacket + 1, 4);

		if (aPacket[5] == 0x01)
		{
			// joined party			
			QueueThisPlayerMessage(PM_JOINPARTY, dwPlayerID);
		}
		else if (aPacket[5] == 0x02)
		{
			// other player invited me to join his party
			QueueThisPlayerMessage(PM_INVITATION, dwPlayerID);
		}		
	}

	//////////////////////////////////////////////////////////
	// Hostility/Unhostility
	//////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x8c && IsThisPlayerID(aPacket + 5))		
	{
		DWORD dwSrcID;
		::memcpy(&dwSrcID, aPacket + 1, 4);
		
		if (aPacket[9] == 0x08 || aPacket[9] == 0x00)
		{
			if (OnHostile(aPacket[9], dwSrcID))
				QueueThisPlayerMessage(aPacket[9] ? PM_HOSTILE : PM_UNHOSTILE, (WPARAM)dwSrcID, (LPARAM)GetHostilerCount());
		}
	}

	//////////////////////////////////////////////////////////
	// Other Player Quit
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x5c && aLen == 5)
	{
		DWORD dwQuitterID;
		::memcpy(&dwQuitterID, aPacket + 1, 4);
		if (OnHostile(FALSE, dwQuitterID))
			QueueThisPlayerMessage(PM_UNHOSTILE, (WPARAM)dwQuitterID, (LPARAM)GetHostilerCount());
	}
	
	//////////////////////////////////////////////////////////
	// Player Left/Right Spell Selected
	//////////////////////////////////////////////////////////
	if (aLen == 13 && aPacket[0] == 0x23 && aPacket[1] == 0x00 && IsThisPlayerID(aPacket + 2))
	{
		WORD wSpell;
		::memcpy(&wSpell, aPacket + 7, 2);

		if (aPacket[6])
		{
			// left spell
			if (wSpell != g_wLSpell)
			{
				g_wLSpell = wSpell;
				QueueThisPlayerMessage(PM_SPELLSELECT, aPacket[6], wSpell);
			}
		}
		else
		{
			// right spell
			if (wSpell != g_wRSpell)
			{
				g_wRSpell = wSpell;
				QueueThisPlayerMessage(PM_SPELLSELECT, aPacket[6], wSpell);
			}
		}
	}

	//////////////////////////////////////////////////////////
	// Map Blink (Teleport, WP, TP, etc)
	//////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x15 && IsThisPlayerID(aPacket + 2))
	{
		WORD x, y;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		QueueThisPlayerMessage(PM_MAPBLINK, (WPARAM)GetCurrentAct(), MAKELPARAM(x, y));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Player's Inventory is Full
	//////////////////////////////////////////////////////////////////////
	if (aLen == 8
		&& aPacket[0] == 0x2c
		&& aPacket[1] == 0x00
		&& aPacket[6] == 0x17
		&& aPacket[7] == 0x00
		 && IsThisPlayerID(aPacket + 2))
	{
		QueueThisPlayerMessage(PM_INVENTORYFULL);
	}

	//////////////////////////////////////////////////////////////////////
	// Weapon Switch
	//////////////////////////////////////////////////////////////////////
	if (aLen == 1 && aPacket[0] == 0x97)
	{
		QueueThisPlayerMessage(PM_WEAPONSWITCH);
	}	
	
	//////////////////////////////////////////////////////////
	// Vendor Item Info
	//////////////////////////////////////////////////////////
	if (aLen > 8 && aPacket[0] == 0x9c && aPacket[1] == 0x0b)
	{
		OnVendorItemPacket(aPacket, aLen);
	}
	
	return aLen;
}
