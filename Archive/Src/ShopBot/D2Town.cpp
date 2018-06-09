//////////////////////////////////////////////////////////////////////
// D2Town.cpp: implementation of the CD2Town class.
//////////////////////////////////////////////////////////////////////

#include "D2Town.h"

CD2Town::CD2Town()
{
	m_aTownWPs[0].SetFingerprinter(&FP_OBJ_ACT1WP);
	m_aTownWPs[1].SetFingerprinter(&FP_OBJ_ACT2WP);
	m_aTownWPs[2].SetFingerprinter(&FP_OBJ_ACT3WP);
	m_aTownWPs[3].SetFingerprinter(&FP_OBJ_ACT4WP);
	m_aTownWPs[4].SetFingerprinter(&FP_OBJ_ACT5WP);
	m_aHealers[0].SetFingerprinter(&FP_NPC_AKARA);
	m_aHealers[1].SetFingerprinter(&FP_NPC_FARA);
	m_aHealers[2].SetFingerprinter(&FP_NPC_ORMUS);
	m_aHealers[3].SetFingerprinter(&FP_NPC_JAMELLA);
	m_aHealers[4].SetFingerprinter(&FP_NPC_MALAH);
	m_stash.SetFingerprinter(&FP_OBJ_STASH);

	m_aTownWPs[0].SetD2MessageProc(OnA1WPMessage, (LPVOID)this);
	m_stash.SetD2MessageProc(OnA1StashMessage, (LPVOID)this);
}

CD2Town::~CD2Town()
{
}

void CD2Town::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Abstract::OnGameJoin(pGame); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	::memset(&m_a1StashPos, 0, sizeof(D2POSITION));
	::memset(&m_a1WPPos, 0, sizeof(D2POSITION));
	for (int i = 0; i < 5; i++)
	{
		m_aTownWPs[i].OnGameJoin(pGame);
		m_aHealers[i].OnGameJoin(pGame);
	}
	m_stash.OnGameJoin(pGame);
}

void CD2Town::OnGameLeave()
{
	CD2Abstract::OnGameLeave(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	for (int i = 0; i < 5; i++)
	{
		m_aTownWPs[i].OnGameLeave();
		m_aHealers[i].OnGameLeave();
	}
	m_stash.OnGameLeave();
}

void CD2Town::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	for (int i = 0; i < 5; i++)
	{
		m_aTownWPs[i].OnGamePacketBeforeReceived(aPacket, aLen);
		m_aHealers[i].OnGamePacketBeforeReceived(aPacket, aLen);
	}

	m_stash.OnGamePacketBeforeReceived(aPacket, aLen);
}

void CD2Town::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeSent(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	for (int i = 0; i < 5; i++)
	{
		m_aTownWPs[i].OnGamePacketBeforeSent(aPacket, aLen);
		m_aHealers[i].OnGamePacketBeforeSent(aPacket, aLen);
	}
	m_stash.OnGamePacketBeforeSent(aPacket, aLen);
}

void CD2Town::OnGameTimerTick()
{
	CD2Abstract::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	for (int i = 0; i < 5; i++)
	{
		m_aTownWPs[i].OnGameTimerTick();
		m_aHealers[i].OnGameTimerTick();
	}	
	m_stash.OnGameTimerTick();
}


const CD2StaticObject* CD2Town::GetCurrentTownWP() const
{
	if (GetCurrentAct() > 4)
		return NULL;

	return &m_aTownWPs[GetCurrentAct()];
}

const CD2Unit* CD2Town::GetCurrentTownHealer() const
{
	if (GetCurrentAct() > 4)
		return NULL;

	return &m_aHealers[GetCurrentAct()];
}

BOOL CD2Town::IsHealer(const CD2Unit *pNpc) const
{
	if (pNpc == NULL)
		return FALSE;

	D2OBJFINGERPRINTER fp1 = {0};	
	pNpc->GetFingerprinter(&fp1);

	for (int i = 0; i < 5; i++)
	{
		D2OBJFINGERPRINTER fp2 = {0};
		m_aHealers[i].GetFingerprinter(&fp2);
		if (!::memcmp(&fp1, &fp2, sizeof(D2OBJFINGERPRINTER)))
			return TRUE;
	}

	return FALSE;
}

void CD2Town::OnA1StashMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	CD2Town* pTown = (CD2Town*)lpData;
	if (pTown == NULL)
		return;

	if (nMessage == OM_SPAWN && GetStashAct(lParam) == 0)
	{
		pTown->m_a1StashPos.x = LOWORD(lParam);
		pTown->m_a1StashPos.y = HIWORD(lParam);
	}
}

void CD2Town::OnA1WPMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LPVOID lpData)
{
	CD2Town* pTown = (CD2Town*)lpData;
	if (pTown == NULL)
		return;

	if (nMessage == OM_SPAWN)
	{
		pTown->m_a1WPPos.x = LOWORD(lParam);
		pTown->m_a1WPPos.y = HIWORD(lParam);
	}
}

BOOL CD2Town::IsAct1WPHigh() const
{
	return m_a1StashPos.x > m_a1WPPos.x;
}

BOOL CD2Town::IsAct1Visited() const
{
	return m_a1WPPos.x > 0 && m_a1StashPos.x > 0;
}

D2POSITION CD2Town::GetAct1StashPos() const
{
	return m_a1StashPos;
}

D2POSITION CD2Town::GetAct1WPPos() const
{
	return m_a1WPPos;
}

D2POSITION CD2Town::GetAct1RelativePosition(int nOffsetX, int nOffsetY) const
{
	D2POSITION pos = m_a1StashPos;
	pos.x += nOffsetX;
	pos.y += nOffsetY;
	return pos;
}

BYTE CD2Town::GetStashAct(LPARAM lParam)
{
	BYTE iAct = 0;
	switch (lParam)
	{
	case 0x13d11401: // east gate
	case 0x13d41401: // north gate
		iAct = 1; // act 2
		break;

	case 0x13c11415:
		iAct = 2; // act 3
		break;

	case 0x13b0139b:
		iAct = 3; // act 4
		break;

	case 0x13c11404:
		iAct = 4; // act 5
		break;

	default:
		break; // all other positions are act 1
	}

	return iAct;
}

BOOL CD2Town::IsPlayerAtStartPosition(const CD2Player* pPlayer, WORD wMaxOffset) const
{
	if (pPlayer == NULL || !pPlayer->IsValidObject())
		return FALSE;

	BOOL bResult = FALSE;
	switch (GetCurrentAct())
	{
	case 0:
		bResult = pPlayer->GetDistance(m_a1StashPos.x, m_a1StashPos.y) <= wMaxOffset;
		break;

	case 1:
		bResult = pPlayer->GetDistance(0x1421, 0x1453) <= wMaxOffset;
		break;

	case 2:
		bResult = pPlayer->GetDistance(0x13fe, 0x1430) <= wMaxOffset;
		break;

	case 3:
		bResult = pPlayer->GetDistance(0x13b8, 0x13b3) <= wMaxOffset;
		break;

	case 4:
		bResult = pPlayer->GetDistance(0x13ea, 0x139f) <= wMaxOffset;
		break;

	default:
		break;
	}

	return bResult;
}
