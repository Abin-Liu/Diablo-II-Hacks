//////////////////////////////////////////////////////////////////////
// GameMessage.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "GameMessage.h"
#include "Server20.h"
#include "D2HackIt.h"
#include "D2HackItServer.h"
#include "me.h"
#include "NpcSession.h"
#include "EnterWayPoint.h"
#include "ItemCheck.h"

#define GET_DWORD(ptr) ((ptr) ? *((DWORD*)(ptr)) : 0)
#define GET_WORD(ptr) ((ptr) ? *((WORD*)(ptr)) : 0)

// Used for tracking player spells/hp/mana only, no other usage
WORD g_wLSpell = D2S_INVALID;
WORD g_wRSpell = D2S_INVALID;
WORD g_wHP = 0;
WORD g_wMana = 0;

void SendUnitMessage(UINT nMessage, DWORD dwUnitID, DWORD dwUnitType, WPARAM wParam = 0, LPARAM lParam = 0)
{	
	const GAMEUNIT gu = { dwUnitID, dwUnitType }; 
	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnUnitMessage)
			pCIS->OnUnitMessage(nMessage, &gu, wParam, lParam);
		//else
		//	GamePrintError("OnUnitMessage error!");

		li = ClientModules.GetNextItem(li);
	}
}

void SendThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
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

void MessageReset()
{
	g_wLSpell = D2S_INVALID;
	g_wRSpell = D2S_INVALID;
	g_wHP = 0;
	g_wMana = 0;
}

DWORD ServerPacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if (aLen == 9 && aPacket[0] == 0x49)
	{		
		if (!IsWpOpened())
		{
			//GameInfof("49 blocked");
			return 0; // block invalid wp packet
		}	
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
	return aLen;
}

void ServerPacketAfterReceived(const BYTE* aPacket, DWORD aLen)
{
	//////////////////////////////////////////////////////////
	// Item Events
	//////////////////////////////////////////////////////////
	if ((aPacket[0] == 0x9c || aPacket[0] == 0x9d) && aPacket[1] != 0x13)
	{
		SendUnitMessage(UM_ITEMEVENT, GET_DWORD(aPacket + 4), UNIT_TYPE_ITEM, aPacket[1], aPacket[3]);
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Affection
	//////////////////////////////////////////////////////////////////////
	if ((aPacket[0] == 0xa8 || aPacket[0] == 0xaa) && aLen >= 8)
	{
		if (GET_DWORD(aPacket + 2) == GetSafePlayerID())
			SendThisPlayerMessage(PM_AFFECT, aPacket[1], aPacket[7]);
		else
			SendUnitMessage(UM_AFFECT, GET_DWORD(aPacket + 2), UNIT_TYPE_MONSTER, aPacket[1], aPacket[7]);
	}
	
	//////////////////////////////////////////////////////////////////////
	// Unit Disaffection
	//////////////////////////////////////////////////////////////////////
	if (aLen >= 7 && aPacket[0] == 0xa9)
	{
		if (GET_DWORD(aPacket + 2) == GetSafePlayerID())
			SendThisPlayerMessage(PM_DISAFFECT, aPacket[1], aPacket[6]);
		else
			SendUnitMessage(UM_DISAFFECT, GET_DWORD(aPacket + 2), UNIT_TYPE_MONSTER, aPacket[1], aPacket[6]);
	}

	//////////////////////////////////////////////////////////////////////
	// Static Object Spawned
	//////////////////////////////////////////////////////////////////////
	if (aLen > 12 && aPacket[0] == 0x51)
	{
		SendUnitMessage(UM_SPAWN, GET_DWORD(aPacket + 2), aPacket[1], 0, MAKELPARAM(GET_WORD(aPacket + 8), GET_WORD(aPacket + 10)));
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Spawned
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0xac && aLen >= 13)
	{
		SendUnitMessage(UM_SPAWN, GET_DWORD(aPacket + 1), UNIT_TYPE_MONSTER, 0, MAKELPARAM(GET_WORD(aPacket + 7), GET_WORD(aPacket + 9)));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Other Player/Corpse Spawned, My Corpse Spawned
	//////////////////////////////////////////////////////////////////////
	if (aLen == 26 && aPacket[0] == 0x59 && IsGameJoined() && !IsThisPlayerID(aPacket + 1))
	{
		if (::stricmp(LPCSTR(aPacket + 6), GetSafePlayerName()))
		{			
			// it's other player, or other player's corpse	
			SendUnitMessage(UM_SPAWN, GET_DWORD(aPacket + 1), UNIT_TYPE_PLAYER, 0, MAKELPARAM(GET_WORD(aPacket + 22), GET_WORD(aPacket + 24)));
		}
		else
		{
			// it's my corpse
			SendThisPlayerMessage(PM_CORPSE, GET_DWORD(aPacket + 1), MAKELPARAM(GET_WORD(aPacket + 22), GET_WORD(aPacket + 24)));
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Object/Unit is Destroyed by the Game (be Gone from Player's Vision) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 6 && aPacket[0] == 0x0a)
	{
		 SendUnitMessage(UM_DESTROY, GET_DWORD(aPacket + 2), aPacket[1]);
	}
	
	//////////////////////////////////////////////////////////////////////
	// Monster HP Changed
	//////////////////////////////////////////////////////////////////////
	if (aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x06)
	{
		SendUnitMessage(UM_HPPERCENT, GET_DWORD(aPacket + 1), UNIT_TYPE_MONSTER, CalcPercent(aPacket[10], 128, 1), 0);
	}

	//////////////////////////////////////////////////////////////////////
	// Hiring or summoning Life Percentage Changed
	//////////////////////////////////////////////////////////////////////
	if (aLen == 10 && aPacket[0] == 0x7f && aPacket[1] == 0x00)
	{
		SendUnitMessage(UM_HPPERCENT, GET_DWORD(aPacket + 4), UNIT_TYPE_MONSTER, aPacket[2], 1);
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Death
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x08)
	{
		SendUnitMessage(UM_DEATH, GET_DWORD(aPacket + 1), UNIT_TYPE_MONSTER);
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Corpse Appeared
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x09)
	{
		SendUnitMessage(UM_CORPSE, GET_DWORD(aPacket + 1), UNIT_TYPE_MONSTER, 0, MAKELPARAM(GET_WORD(aPacket + 6), GET_WORD(aPacket + 8)));
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

		SendUnitMessage(UM_ATTACK, unitAttacker.dwUnitID, unitAttacker.dwUnitType, (WPARAM)&unitVictim, MAKELPARAM(GET_WORD(aPacket + 6), GET_WORD(aPacket + 8)));
		SendUnitMessage(UM_UNDERATTACK, unitVictim.dwUnitID, unitVictim.dwUnitType, (WPARAM)&unitAttacker, MAKELPARAM(GET_WORD(aPacket + 6), GET_WORD(aPacket + 8)));
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by It's Resurrecter (Shaman, Greater Mummy, etc) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 16 && aPacket[0] == 0x6b)
	{
		SendUnitMessage(UM_REVIVED, GET_DWORD(aPacket + 1), UNIT_TYPE_MONSTER, (WPARAM)1, MAKELPARAM(GET_WORD(aPacket + 12), GET_WORD(aPacket + 14)));
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by Itself (Act5 Zombies) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 17 && aPacket[0] == 0x4d && aPacket[1] == 0x01)
	{
		GAMEUNIT gu = {GET_DWORD(aPacket + 2), UNIT_TYPE_MONSTER};
		if (VerifyUnit(&gu))
		{
			POINT mp = GetUnitPosition(&gu);
			SendUnitMessage(UM_REVIVED, gu.dwUnitID, gu.dwUnitType, (WPARAM)0, MAKELPARAM(mp.x, mp.y));
		}
	}
	
	//////////////////////////////////////////////////////////////////////
	// Unit Starts Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x67 && aLen == 16)
	{	
		SendUnitMessage(UM_STARTMOVE, GET_DWORD(aPacket + 1), UNIT_TYPE_MONSTER, 0, MAKELPARAM(GET_WORD(aPacket + 6), GET_WORD(aPacket + 8)));
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Ends Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x6d && aLen == 10)
	{
		SendUnitMessage(UM_ENDMOVE, GET_DWORD(aPacket + 1), UNIT_TYPE_MONSTER, 0, MAKELPARAM(GET_WORD(aPacket + 5), GET_WORD(aPacket + 7)));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Cave Notification
	//////////////////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x09)
	{
		SendUnitMessage(UM_CAVENOTIFY, GET_DWORD(aPacket + 2), aPacket[1], aPacket[6], MAKELPARAM(GET_WORD(aPacket + 7), GET_WORD(aPacket + 9)));
	}

	//////////////////////////////////////////////////////////
	// This Let Us Have a Player ID Before OnGameJoin is Called
	//////////////////////////////////////////////////////////
	if (aLen == 26 && aPacket[0] == 0x59)
	{
		SetPlayerInfoBeforeJoin(GET_DWORD(aPacket + 1), (LPCSTR)aPacket + 6);
	}

	//////////////////////////////////////////////////////////
	// Stash Opened
	//////////////////////////////////////////////////////////
	if (aLen == 2 && aPacket[0] == 0x77 && aPacket[1] == 0x10)
	{		
		SendThisPlayerMessage(PM_STASHOPEN, (WPARAM)GetCurrentAct());
	}

	//////////////////////////////////////////////////////////
	// Waypoint Opened
	//////////////////////////////////////////////////////////
	if (aLen == 21 && aPacket[0] == 0x63)
	{
		OnWpOpen(TRUE);
		SendThisPlayerMessage(PM_WPOPEN, (WPARAM)GetCurrentAct(), (LPARAM)GetCurrentMapID());
	}	

	//////////////////////////////////////////////////////////////////////
	// Player's Corpse is Picked Up 
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 6 && aPacket[0] == 0x8e && aPacket[1] == 0x00 && IsThisPlayerID(aPacket + 2))
	{
		SendThisPlayerMessage(PM_CORPSEPICKEDUP);
	}	

	//////////////////////////////////////////////////////////
	// Player Character Level Changed
	//////////////////////////////////////////////////////////
	if (aLen == 3 && aPacket[0] == 0x1d && aPacket[1] == 0x0c)
	{
		SendThisPlayerMessage(PM_LEVEL, 0, GetStat(STAT_LEVEL));
	}

	//////////////////////////////////////////////////////////
	// Player HP/Mana Updates
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x95)
	{
		WORD wOldHP = g_wHP;
		WORD wOldMana = g_wMana;
		g_wHP = *((WORD *)(aPacket + 1)) & 0x0FFF;
		g_wMana = (*((WORD *)(aPacket + 3)) & 0x0FFF) << 1;
		
		//GameInfof("hp %d-%d, mana %d-%d", wOldHP, g_wHP, wOldMana, g_wMana);

		if (wOldHP < g_wHP)
		{
			// hp up
			SendThisPlayerMessage(PM_HPUP, wOldHP, g_wHP);
		}
		else if (wOldHP > g_wHP)
		{
			// hp down
			SendThisPlayerMessage(PM_HPDOWN, wOldHP, g_wHP);
		}

		if (wOldMana < g_wMana)
		{
			//GameInfof("mana up");
			SendThisPlayerMessage(PM_MANAUP, wOldMana, g_wMana);
		}
		else if (wOldMana > g_wMana)
		{
			//GameInfof("mana down");
			SendThisPlayerMessage(PM_MANADOWN, wOldMana, g_wMana);
		}	
	}

	//////////////////////////////////////////////////////////
	// Player Death
	//////////////////////////////////////////////////////////
	if (aLen >= 6 && aPacket[0] == 0x8e && aPacket[1] == 0x01)
	{
		if (GET_DWORD(aPacket + 2) == GetSafePlayerID())
		{
			if (!IsInTown())
			{
				// i'm dead
				SendThisPlayerMessage(PM_DEATH);
			}
		}
		else
		{
			// other player dead
			SendUnitMessage(UM_DEATH, GET_DWORD(aPacket + 2), UNIT_TYPE_PLAYER);
		}
	}

	//////////////////////////////////////////////////////////
	// Party invitation/Joining
	//////////////////////////////////////////////////////////
	if (aLen == 6 && aPacket[0] == 0x8b)
	{
		if (aPacket[5] == 0x01)
		{
			// joined party			
			SendThisPlayerMessage(PM_JOINPARTY, GET_DWORD(aPacket + 1));
		}
		else if (aPacket[5] == 0x02)
		{
			// other player invited me to join his party
			SendThisPlayerMessage(PM_INVITATION, GET_DWORD(aPacket + 1));
		}		
	}

	//////////////////////////////////////////////////////////
	// Hostility/Unhostility
	//////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x8c && IsThisPlayerID(aPacket + 5))		
	{
		if (aPacket[9] == 0x08 || aPacket[9] == 0x00)
			SendThisPlayerMessage(aPacket[9] ? PM_HOSTILE : PM_UNHOSTILE, (WPARAM)GET_DWORD(aPacket + 1), (LPARAM)GetHostilerCount());
	}

	/*
	//////////////////////////////////////////////////////////
	// Other Player Quit
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x5c && aLen == 5)
	{
		if (OnHostile(FALSE, GET_DWORD(aPacket + 1)))
			SendThisPlayerMessage(PM_UNHOSTILE, (WPARAM)GET_DWORD(aPacket + 1), (LPARAM)GetHostilerCount());
	}
	*/
	
	//////////////////////////////////////////////////////////
	// Player Left/Right Spell Selected
	//////////////////////////////////////////////////////////
	if (aLen == 13 && aPacket[0] == 0x23 && aPacket[1] == 0x00 && IsThisPlayerID(aPacket + 2))
	{
		WORD wSpell = GET_WORD(aPacket + 7);
		if (aPacket[6])
		{
			// left spell
			if (wSpell != g_wLSpell)
			{
				g_wLSpell = wSpell;
				SendThisPlayerMessage(PM_SPELLSELECT, aPacket[6], wSpell);
			}
		}
		else
		{
			// right spell
			if (wSpell != g_wRSpell)
			{
				g_wRSpell = wSpell;
				SendThisPlayerMessage(PM_SPELLSELECT, aPacket[6], wSpell);
			}
		}
	}

	//////////////////////////////////////////////////////////
	// Map Blink (Teleport, WP, TP, etc)
	//////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x15 && aPacket[1] == 0x00 && IsThisPlayerID(aPacket + 2))
	{
		SendThisPlayerMessage(PM_MAPBLINK, (WPARAM)GetCurrentAct(), MAKELPARAM(GET_WORD(aPacket + 6), GET_WORD(aPacket + 8)));
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
		SendThisPlayerMessage(PM_INVENTORYFULL);
	}

	//////////////////////////////////////////////////////////////////////
	// Weapon Switch
	//////////////////////////////////////////////////////////////////////
	if (aLen == 1 && aPacket[0] == 0x97)
	{
		SendThisPlayerMessage(PM_WEAPONSWITCH);
	}	
	
	//////////////////////////////////////////////////////////
	// Vendor Item Info
	//////////////////////////////////////////////////////////
	if (aLen > 8 && aPacket[0] == 0x9c && aPacket[1] == 0x0b)
	{
		OnVendorItemPacket(aPacket, aLen);
	}	
}