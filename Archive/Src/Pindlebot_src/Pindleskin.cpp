// Pindleskin.cpp: implementation of the CPindleskin class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Pindleskin.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPindleskin::CPindleskin()
{
	InitAttributes();

	// Fingerprinter for pindleskin
	D2OBJFINGERPRINTER fp = {{0xA0, 0xA1, 0x01, 0x10}, 4, 15, 18}; // Pindleskin
	CD2Monster::SetFingerprinter(&fp);
}

CPindleskin::~CPindleskin()
{

}

void CPindleskin::InitAttributes()
{
	CD2Monster::InitAttributes();
	::memset(m_aBonuses, PINDLE_BONUS_NONE, PINDLE_MAX_BONUS);
}

void CPindleskin::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Monster::OnGameJoin(pGame);
	InitAttributes();
}

void CPindleskin::OnGameLeave()
{
	CD2Monster::OnGameLeave();
	InitAttributes();
}

void CPindleskin::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Monster::OnGamePacketBeforeReceived(aPacket, aLen);

	if (!aPacket || !aLen)
		return;

	//////////////////////////////////////////////////////////////////////
	// Pindleskin Spawned
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0xac
		&& aLen >= 25
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		::memset(m_aBonuses, 0, sizeof(m_aBonuses));
		// Copy Pindleskin bonuses
		for (DWORD i = 0; i < PINDLE_MAX_BONUS && i < aLen - 24; i++)
		{
			WORD wBonus = aPacket[i + 20];
			if (wBonus && wBonus != PINDLE_BONUS_FIRE_ENCHANTED) // ignore file enchanted
				m_aBonuses[i] = wBonus;
		}
		OnBonuses(m_aBonuses);		
		NotifyD2Message(PSM_BONUSES, (WPARAM)0, (LPARAM)m_aBonuses);
		return;
	}
}

const WORD* CPindleskin::GetBonuses() const
{
	return m_aBonuses;
}

void CPindleskin::OnBonuses(const WORD* aBonuses)
{
	/*
	for (int i = 0; i < PINDLE_MAX_BONUS; i++)
	{
		if (m_aBonuses[i])
		{
			char sz[256] = "";
			if (PBotGetBonusName(m_aBonuses[i], sz, 255, FALSE))
				server->GamePrintInfo(sz);
		}
	}
	*/
}

void CPindleskin::OnAffection(BOOL bPassive, BYTE iAffectionID)
{
	CD2Monster::OnAffection(bPassive, iAffectionID);
	WORD wAura = IsPindleskinAura(iAffectionID); // Is boss aura?
	if (wAura)
	{	
		for (int i = 0; i < PINDLE_MAX_BONUS + 1; i++)
		{
			if (m_aBonuses[i] == PINDLE_BONUS_AURA_ENCHANTED)
			{
				m_aBonuses[i] = wAura;
				break;
			}
		}

		OnBonuses(m_aBonuses);
		NotifyD2Message(PSM_BONUSES, (WPARAM)0, (LPARAM)m_aBonuses);
	}
}

WORD CPindleskin::IsPindleskinAura(BYTE iAffectionID)
{
	WORD wResult = iAffectionID;
	switch (iAffectionID)
	{
	case STATE_CONVICTION:
	case STATE_MIGHT:
	case STATE_HOLYFIRE:
	case STATE_HOLYWIND: // holy freeze
	case STATE_BLESSEDAIM:
	case STATE_HOLYSHOCK:
	case STATE_FANATICISM:
		break;

	default:
		wResult = 0;
		break;
	}

	if (wResult)
		wResult |= 0x8000;

	return wResult;
}
