// Ticker.cpp: implementation of the CTicker class.
//
//////////////////////////////////////////////////////////////////////

#include "Ticker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTicker::CTicker(): m_dwCount(0), m_dwStartWith(0)
{}

CTicker::~CTicker()
{}

void CTicker::ResetTicker(DWORD dwNewCount)
{
	m_dwStartWith = dwNewCount;
	m_dwCount = dwNewCount;
}

void CTicker::TickIt(DWORD dwCount)
{
	if (m_dwCount > dwCount)
		m_dwCount -= dwCount;
	else
		m_dwCount = 0;
}

BOOL CTicker::IsTickEnded() const
{
	return m_dwCount == 0;
}

DWORD CTicker::GetRemainer() const
{
	return m_dwCount;
}

DWORD CTicker::GetElapsed() const
{
	return m_dwCount > m_dwStartWith ? 0 : m_dwStartWith - m_dwCount;
}
