//////////////////////////////////////////////////////////////////////
// D2Monster.cpp: implementation of the CD2Monster class.
//////////////////////////////////////////////////////////////////////

#include "D2Monster.h"

BOOL operator==(const MONSTERCORPSE& lhs, const MONSTERCORPSE& rhs)
{
	return lhs.dwCorpseID == rhs.dwCorpseID;
}

BOOL operator>(const MONSTERCORPSE& lhs, const MONSTERCORPSE& rhs)
{
	return lhs.dwCorpseID > rhs.dwCorpseID;
}

CD2Monster::CD2Monster()
{
	InitAttributes(); // Initialization
}

CD2Monster::~CD2Monster()
{
}

void CD2Monster::InitAttributes()
{
	CD2Unit::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_bIsAlive = FALSE;
	m_bIsCorpse = FALSE;
	m_iHPPercent = 0;
}

void CD2Monster::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Unit::OnGameJoin(pGame); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	
}

void CD2Monster::OnGameLeave()
{
	CD2Unit::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	
}

void CD2Monster::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Unit::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	//////////////////////////////////////////////////////////////////////
	// Monster Spawned
	//////////////////////////////////////////////////////////////////////
	if (!m_bLockSpawn
		&& aPacket[0] == 0xac
		&& aLen >= 13
		&& VerifyFingerprinter(aPacket, aLen))
	{
		InitAttributes();

		::memcpy(&m_dwObjectID, aPacket + 1, 4);
		::memcpy(&m_wType, aPacket + 5, 2);
		::memcpy(&m_wPositionX, aPacket + 7, 2);
		::memcpy(&m_wPositionY, aPacket + 9, 2);
		
		m_bIsAlive = TRUE;
		m_bIsCorpse = FALSE;
		m_iHPPercent = CALC80BASEDHPPERCENT(aPacket[11]);		
		
		OnSpawn(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndDebugMessage(_T("Monster %08X spawned at %04X, %04X"), m_dwObjectID, m_wPositionX, m_wPositionY);
		NotifyD2Message(OM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Monster Being Hurt
	//////////////////////////////////////////////////////////////////////
	if (aLen == 12
		&& aPacket[0] == 0x69
		&& aPacket[5] == 0x06
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		BYTE iOldPercent = m_iHPPercent;
		m_iHPPercent = CALC80BASEDHPPERCENT(aPacket[10]);

		if (iOldPercent > m_iHPPercent)
		{
			OnHPPercentDown(iOldPercent, m_iHPPercent);
			FormatAndDebugMessage(_T("Unit HP percent down: %d%% - %d%%"), iOldPercent, m_iHPPercent);
			NotifyD2Message(MM_HPPERCENTDOWN, (WPARAM)iOldPercent, (LPARAM)m_iHPPercent);
		}
		else if (iOldPercent < m_iHPPercent)
		{
			OnHPPercentUp(iOldPercent, m_iHPPercent);
			FormatAndDebugMessage(_T("Unit HP percent up: %d%% - %d%%"), iOldPercent, m_iHPPercent);
			NotifyD2Message(MM_HPPERCENTUP, (WPARAM)iOldPercent, (LPARAM)m_iHPPercent);
		}
		return;
	}
	
	//////////////////////////////////////////////////////////////////////
	// Monster Death
	//////////////////////////////////////////////////////////////////////
	if ( aLen == 12
		&& aPacket[0] == 0x69
		&& aPacket[5] == 0x08
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		m_bIsAlive = FALSE;
		m_bIsCorpse = FALSE;
		m_iHPPercent = 0;
		m_bIsMoving = FALSE;
		OnDeath();
		DebugMessage(_T("Unit died"));
		NotifyD2Message(MM_DEATH, (WPARAM)m_dwObjectID, 0);
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Corpse Appeared
	//////////////////////////////////////////////////////////////////////
	if ( aLen == 12
		&& aPacket[0] == 0x69
		&& aPacket[5] == 0x09
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		m_bIsAlive = FALSE;
		m_bIsCorpse = TRUE;
		m_iHPPercent = 0;
		m_bIsMoving = FALSE;
		::memcpy(&m_wPositionX, aPacket + 6, 2);
		::memcpy(&m_wPositionY, aPacket + 8, 2);
		OnCorpseAppeared(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndDebugMessage(_T("Unit corpse appeared at %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_CORPSE, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Attacking
	//////////////////////////////////////////////////////////////////////
	if (aLen == 21
		&& aPacket[0] == 0x68
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		::memcpy(&m_wPositionX, aPacket + 6, 2);
		::memcpy(&m_wPositionY, aPacket + 8, 2);

		// attacking whom?
		DWORD dwVictimID = 0;
		::memcpy(&dwVictimID, aPacket + 11, 4);
		OnAttack(m_wPositionX, m_wPositionY, dwVictimID);
		FormatAndDebugMessage(_T("Unit is attacking %08X at %04X, %04X"), dwVictimID, m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_ATTACK, (WPARAM)dwVictimID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Under Attack
	//////////////////////////////////////////////////////////////////////
	if (aLen == 21
		&& aPacket[0] == 0x68
		&& !::memcmp(&m_dwObjectID, aPacket + 11, 4))
	{
		// attacked by whom?
		DWORD dwAttackerID = 0;
		::memcpy(&dwAttackerID, aPacket + 1, 4);
		OnUnderAttack(m_wPositionX, m_wPositionY, dwAttackerID);
		FormatAndDebugMessage(_T("Unit is attacked by %08X at %04X, %04X"), dwAttackerID, m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_UNDERATTACK, (WPARAM)dwAttackerID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by It's Resurrecter (Shaman, Greater Mummy, etc) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 16
		&& aPacket[0] == 0x6b
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		::memcpy(&m_wPositionX, aPacket + 12, 2);
		::memcpy(&m_wPositionY, aPacket + 14, 2);
		m_bIsAlive = TRUE;
		m_bIsCorpse = FALSE;
		m_bIsMoving = FALSE;
		m_iHPPercent = 100;
		OnRevived(TRUE, m_wPositionX, m_wPositionY);
		FormatAndDebugMessage(_T("Unit is revived at %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_REVIVED, (WPARAM)1, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by Itself (Act5 Zombies) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 17
		&& aPacket[0] == 0x4d
		&& !::memcmp(&m_dwObjectID, aPacket + 2, 4))
	{
		m_bIsAlive = TRUE;
		m_bIsCorpse = FALSE;
		m_iHPPercent = 100;
		m_bIsMoving = FALSE;
		OnRevived(FALSE, m_wPositionX, m_wPositionY);
		FormatAndDebugMessage(_T("Unit is revived at %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(MM_REVIVED, (WPARAM)0, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}	
}

BOOL CD2Monster::IsAlive() const
{
	return m_bIsAlive;
}

BOOL CD2Monster::IsCorpse() const
{
	return m_bIsCorpse;
}

BYTE CD2Monster::GetHPPercent() const
{
	return m_iHPPercent;
}

void CD2Monster::OnHPPercentUp(BYTE iOldPercent, BYTE iNewPercent)
{
}

void CD2Monster::OnHPPercentDown(BYTE iOldPercent, BYTE iNewPercent)
{
}

void CD2Monster::OnAttack(WORD x, WORD y, DWORD dwVictimID)
{
}

void CD2Monster::OnUnderAttack(WORD x, WORD y, DWORD dwAttackerID)
{
}

void CD2Monster::OnCorpseAppeared(DWORD dwCorpseID, WORD x, WORD y)
{
}

void CD2Monster::OnRevived(BOOL bByOther, WORD x, WORD y)
{
}

void CD2Monster::OnDeath()
{

}
