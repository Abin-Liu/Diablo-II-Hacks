//////////////////////////////////////////////////////////////////////
// BotAbstract.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __BOTABSTRACT_H__
#define __BOTABSTRACT_H__

#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"

#define BOT_NAME_MAX	64
#define BOT_PATH_MAX	512

class CBotAbstract  
{
public:

	CBotAbstract();
	CBotAbstract(LPCSTR lpszName);
	virtual ~CBotAbstract();

	virtual BOOL Start() = 0;
	virtual BOOL Stop(BOOL bLeaveGame = FALSE) = 0;
	virtual BOOL Pause() = 0;
	virtual BOOL Resume() = 0;

	virtual void OnClientStart();
	virtual void OnClientStop();
	virtual void OnGameJoin(THISGAMESTRUCT* thisgame);
	virtual void OnGameLeave();
	virtual void OnDraw(CGameDC* pDC, LPCRECT lpScreenRect);
	virtual void OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam);
	virtual void OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam);


protected:

	virtual BOOL OnSearchDestination(int nFloor) = 0;
	virtual DWORD OnApproachDestination(int nFloor);
	virtual BOOL OnSearchMonster() = 0;
	virtual DWORD OnApproachMonster();

	virtual void Initialize();	
	static DWORD BotGotoDestination(const POINT& pt, BOOL bTeleport = TRUE);

	char m_szName[BOT_NAME_MAX + 1];
	BOOL m_bStarted;
	BOOL m_bPaused;
	POINT m_ptDestination;
	GAMEUNIT m_target;
	int m_nFloors;
};

#endif // __BOTABSTRACT_H__
