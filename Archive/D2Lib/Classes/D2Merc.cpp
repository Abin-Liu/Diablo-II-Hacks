//////////////////////////////////////////////////////////////////////
// D2Merc.cpp: implementation of the CD2Merc class.
//////////////////////////////////////////////////////////////////////

#include "D2Merc.h"

CD2Merc::CD2Merc()
{
}

CD2Merc::~CD2Merc()
{
}

void CD2Merc::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Monster::OnGameJoin(pGame); // Always call base class's method first!!!

	// TODO: Add your implementation here.
}

void CD2Merc::OnGameLeave()
{
	CD2Monster::OnGameLeave(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
}

void CD2Merc::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Monster::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	///////////////////////////////////////////////////
	// Merc Created, This Is The Only Chance We Can Obtain Merc's ID
	///////////////////////////////////////////////////
	if (!m_bLockSpawn
		&& aLen > 12
		&& aPacket[0] == 0x81
		&& !::memcmp(&m_thisPlayer.dwPlayerID, aPacket + 4, 4))
	{
		InitAttributes();
				
		::memcpy(&m_dwObjectID, aPacket + 8, 4);
		
		// Merc does not have a position upon creation, so we use player's
		// position instead.
		m_wPositionX = m_thisPlayer.pPlayerXAddr ? *m_thisPlayer.pPlayerXAddr : 0;
		m_wPositionY = m_thisPlayer.pPlayerYAddr ? *m_thisPlayer.pPlayerYAddr : 0;
		m_iHPPercent = 100; // Merc's HP is always full upon creation
		
		OnSpawn(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndDebugMessage(_T("Merc %08X spawned at %04X, %04X"), m_dwObjectID, m_wPositionX, m_wPositionY);
		NotifyD2Message(OM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Spawned, Is It Our Merc?
	//////////////////////////////////////////////////////////////////////
	if (!m_bLockSpawn
		&& aPacket[0] == 0xac
		&& aLen >= 13
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
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
		NotifyD2Message(OM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionX));
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Merc Life Percentage Update
	//////////////////////////////////////////////////////////////////////
	if (aLen == 10
		&& aPacket[0] == 0x7f
		&& aPacket[1] == 0x00
		&& !::memcmp(&m_dwObjectID, aPacket + 4, 4))
	{
		BYTE iOldPercent = m_iHPPercent;
		m_iHPPercent = aPacket[2];
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
}

void CD2Merc::OnDestroy(DWORD dwObjectID)
{
	CD2Monster::OnDestroy(dwObjectID);

	// We need to keep merc's ID, once we lose it we have no way to recover
	// it in this game unless the merc dies and gets resurrected.
	m_dwObjectID = dwObjectID; // merc's ID must not be destroyed
}
