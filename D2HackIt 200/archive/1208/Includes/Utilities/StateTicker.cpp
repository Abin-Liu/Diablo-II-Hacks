///////////////////////////////////////////////////////////
// StateTicker.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "StateTicker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStateTicker::CStateTicker()
{
	Clear();
}

CStateTicker::~CStateTicker()
{}

void CStateTicker::Update(DWORD dwNewState, DWORD dwWait, DWORD dwAppend)
{
	m_dwState = dwNewState;
	m_dwStartWith = dwWait + dwAppend;
	m_dwCount = dwWait + dwAppend;
}

void CStateTicker::TickIt(DWORD dwCount)
{
	if (m_bPaused)
		return; // paused

	if (m_dwCount > dwCount)
		m_dwCount -= dwCount;
	else
		m_dwCount = 0;

	if (m_dwCount == 0 && m_bWaiting)
	{
		// previous waiting failed(timed out)
		EndWait(FALSE);
	}
}

BOOL CStateTicker::IsTickEnded() const
{
	return m_dwCount == 0;
}

DWORD CStateTicker::GetRemainer() const
{
	return m_dwCount;
}

DWORD CStateTicker::GetElapsed() const
{
	return m_dwCount > m_dwStartWith ? 0 : m_dwStartWith - m_dwCount;
}

DWORD CStateTicker::GetState() const
{
	return m_dwState;
}

void CStateTicker::Clear()
{
	m_dwCount = 0;
	m_dwStartWith = 0;
	m_dwState = 0;
	m_dwOnWaitOK = 0;
	m_dwOnWaitFail = 0;
	m_dwCountOnOK = 0;
	m_dwCountOnFail = 0;
	m_bPaused = FALSE;
	m_bWaiting = FALSE;
}

void CStateTicker::StartWait(DWORD dwMaxWait, DWORD dwStateOnOK, DWORD dwStateOnFail, DWORD dwCounterOnOK, DWORD dwCounterOnFail)
{
	m_dwStartWith = dwMaxWait;
	m_dwCount = dwMaxWait;
	m_dwOnWaitOK = dwStateOnOK;
	m_dwOnWaitFail = dwStateOnFail;
	m_dwCountOnOK = dwCounterOnOK;
	m_dwCountOnFail = dwCounterOnFail;
	m_bWaiting = TRUE;
}

void CStateTicker::EndWait(BOOL bSucceeded, DWORD dwAppend)
{
	if (bSucceeded)
		Update(m_dwOnWaitOK, m_dwCountOnOK, dwAppend);
	else
		Update(m_dwOnWaitFail, m_dwCountOnFail, dwAppend);

	m_dwOnWaitOK = 0;
	m_dwOnWaitFail = 0;
	m_dwCountOnOK = 0;
	m_dwCountOnFail = 0;
	m_bWaiting = FALSE;
}

BOOL CStateTicker::IsTickerInPeriod(DWORD dwPeriod, BOOL bAllowZero) const
{
	if (dwPeriod == 0)
		return FALSE;

	if (m_dwCount == 0 && !bAllowZero)
		return FALSE;

	return (m_dwCount % dwPeriod) == 0;
}

void CStateTicker::Pause()
{
	m_bPaused = TRUE;
}

void CStateTicker::Resume()
{
	m_bPaused = FALSE;
}

BOOL CStateTicker::IsPaused() const
{
	return m_bPaused;
}

BOOL CStateTicker::IsWaiting() const
{
	return m_bWaiting;
}
