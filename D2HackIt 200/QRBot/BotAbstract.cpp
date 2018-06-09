//////////////////////////////////////////////////////////////////////
// BotAbstract.cpp: implementation of the CBotAbstract class.
//
//////////////////////////////////////////////////////////////////////

#include "BotAbstract.h"

CBotAbstract::CBotAbstract()
{
	Initialize();
	::memset(m_szName, 0, sizeof(m_szName));	
}

CBotAbstract::CBotAbstract(LPCSTR lpszName)
{
	Initialize();
	::memset(m_szName, 0, sizeof(m_szName));
	if (lpszName)
		::strncpy(m_szName, lpszName, BOT_NAME_MAX);
}

CBotAbstract::~CBotAbstract()
{
	
}

void CBotAbstract::Initialize()
{
	m_nFloors = 1;
	m_bStarted = FALSE;
	m_bPaused = FALSE;
	::memset(&m_ptDestination, 0, sizeof(m_ptDestination));
	::memset(&m_target, 0, sizeof(m_target));
}

void CBotAbstract::OnClientStart()
{

}

void CBotAbstract::OnClientStop()
{

}

void CBotAbstract::OnGameJoin(THISGAMESTRUCT *thisgame)
{

}

void CBotAbstract::OnGameLeave()
{

}

void CBotAbstract::OnDraw(CGameDC *pDC, LPCRECT lpScreenRect)
{

}

void CBotAbstract::OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{

}

void CBotAbstract::OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{

}

DWORD CBotAbstract::BotGotoDestination(const POINT &pt, BOOL bTeleport)
{
	POINT aPath[BOT_PATH_MAX];
	const DWORD COUNT = server->CalculateTeleportPath(pt.x, pt.y, aPath, BOT_PATH_MAX);
	for (DWORD i = 0; i < COUNT; i++)
	{
		if (bTeleport)
			me->TeleportTo(aPath[i].x, aPath[i].y, TRUE);
		else
			me->MoveTo(aPath[i].x, aPath[i].y, TRUE);
	}
	return COUNT;
}

DWORD CBotAbstract::OnApproachDestination(int nFloor)
{
	const BOOL USETELEPORT = !me->IsInTown() && me->HaveSpell(D2S_TELEPORT);
	return BotGotoDestination(m_ptDestination, USETELEPORT);
}

DWORD CBotAbstract::OnApproachMonster()
{
	return 0;
}
