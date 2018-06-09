//////////////////////////////////////////////////////////////////////
// TickThread.cpp: implementation of the CThread class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "TickThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTickThread::CTickThread()
{
	m_dwInterval = 100;
	m_lpfnProc = NULL;
	m_lpParam = NULL;
	m_dwThreadID = 0;
	m_hThreadHandle = NULL;
	m_iIsAlive = 0x00;
	m_iStopFlas = 0x01;
}

CTickThread::~CTickThread()
{
	Stop(2500);
}

CTickThread::operator HANDLE() const
{
	return m_hThreadHandle;
}

DWORD CTickThread::InternalProc(LPVOID lpParam)
{
	CTickThread* ptc = (CTickThread*)lpParam;
	if (ptc == NULL || ptc->m_hThreadHandle == NULL || ptc->m_lpfnProc == NULL)
		return -1;

	ptc->m_iIsAlive = 0x01; // mark as alive

	while (ptc->m_iStopFlas == 0x00)
	{
		ptc->m_lpfnProc(ptc->m_lpParam, &ptc->m_iStopFlas);
		const DWORD DELAY = max(100, ptc->m_dwInterval);
		for (DWORD i = 0; i < DELAY; i += 100)
		{
			if (ptc->m_iStopFlas)
				break;

			Sleep(100);
		}		
	}

	ptc->m_iIsAlive = 0x00; // mark as dead
	return 0;
}

BOOL CTickThread::IsAlive() const
{
	return m_iIsAlive || m_hThreadHandle || m_dwThreadID;
}

BOOL CTickThread::Start(TICKPROC lpfnProc, LPVOID lpParam, DWORD dwInterval, int nPriority)
{
	if (lpfnProc == NULL || IsAlive())
		return FALSE;

	m_lpfnProc = lpfnProc;
	m_lpParam = lpParam;
	m_dwInterval = dwInterval;	

	m_iStopFlas = 0x00;
	m_hThreadHandle = ::CreateThread(NULL, 0, InternalProc, (LPVOID)this, 0, &m_dwThreadID);
	if (m_hThreadHandle == NULL)
		return FALSE;

	::SetThreadPriority(m_hThreadHandle, nPriority);
	return TRUE;
}

BOOL CTickThread::Stop(DWORD dwTimeout)
{
	if (!IsAlive())
		return TRUE;

	// try to stop the thread the normal way
	m_iStopFlas = 0x01;
	BOOL bOK = FALSE;
	for (DWORD i = 0; i < dwTimeout / 100; i += 100)
	{
		if (m_iIsAlive == 0x00)
		{
			bOK = TRUE;
			break;
		}
		Sleep(100);
	}
	
	if (!bOK)
	{
		// timed out, ends it the rude way
		bOK = ::TerminateThread(m_hThreadHandle, -1);
	}

	if (bOK)
	{
		::CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
		m_dwThreadID = 0;
	}	

	return bOK;
}

DWORD CTickThread::Suspend() const
{
	return ::SuspendThread(m_hThreadHandle);
}

DWORD CTickThread::Resume() const
{
	return ::ResumeThread(m_hThreadHandle);
}

DWORD CTickThread::GetThreadID() const
{
	return m_dwThreadID;
}

LPVOID CTickThread::GetParam() const
{
	return m_lpParam;
}

DWORD CTickThread::GetInterval() const
{
	return m_dwInterval;
}

void CTickThread::SetParam(LPVOID lpParam)
{
	m_lpParam = lpParam;
}

void CTickThread::SetInterval(DWORD dwInterval)
{
	m_dwInterval = dwInterval;
}

BOOL CTickThread::SetPriority(int nPriority) const
{
	return ::SetThreadPriority(m_hThreadHandle, nPriority);
}

int CTickThread::GetPriority() const
{
	return ::GetThreadPriority(m_hThreadHandle);
}

HANDLE CTickThread::GetThreadHandle() const
{
	return m_hThreadHandle;
}
