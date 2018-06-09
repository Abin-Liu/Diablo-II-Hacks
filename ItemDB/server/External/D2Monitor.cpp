// D2Monitor.cpp: implementation of the CD2Minitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D2Monitor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2Monitor::CD2Monitor()
{
	m_hTargetWnd = NULL;
	m_nMsg = 0;
}

CD2Monitor::~CD2Monitor()
{
	m_thread.Stop(2000);
}

BOOL CD2Monitor::Start(HWND hTargetWnd, UINT nMsg, DWORD dwInterval)
{
	Stop();
	m_hTargetWnd = hTargetWnd;
	m_nMsg = nMsg;
	return m_thread.Start(TickProc, (LPVOID)this, dwInterval);
}

void CD2Monitor::TickProc(LPVOID lParam, LPCVBYTE lpStopFlag)
{
	CD2Monitor* pMonitor = (CD2Monitor*)lParam;
	if (pMonitor && ::IsWindow(pMonitor->m_hTargetWnd))
		pMonitor->OnTimerTick();
}

void CD2Monitor::Stop()
{
	m_thread.Stop(INFINITE);
	m_hTargetWnd = NULL;
	m_nMsg = 0;
	m_aD2Wnds.Lock();
	m_aD2Wnds.RemoveAll();
	m_aD2Wnds.Unlock();
}

void CD2Monitor::OnTimerTick()
{
	int i = 0;
	CArrayEx<HWND, HWND> aList;
	::EnumWindows(EnumWindowsProc, (LPARAM)&aList);
	
	m_aD2Wnds.Lock();
	// first we check if any d2 game window got destroyed
	for (i = 0; i < m_aD2Wnds.GetSize(); i++)
	{
		if (aList.Find(m_aD2Wnds[i]) == -1)
			::SendMessage(m_hTargetWnd, m_nMsg, 0, (LPARAM)m_aD2Wnds[i]);
	}

	// then we check if any new d2 game is created
	for (i = 0; i < aList.GetSize(); i++)
	{
		if (m_aD2Wnds.Find(aList[i]) == -1)
			::SendMessage(m_hTargetWnd, m_nMsg, 1, (LPARAM)aList[i]);
	}

	m_aD2Wnds.Copy(aList);
	m_aD2Wnds.Unlock();
}

BOOL CD2Monitor::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	CArrayEx<HWND, HWND>* pList = (CArrayEx<HWND, HWND>*)lParam;
	if (pList == NULL)
		return FALSE;
	
	static TCHAR szClass[12] = _T("");

	::GetClassName(hWnd, szClass, 11);
	if (_tcscmp(szClass, _T("Diablo II")))
		return TRUE;

	pList->Add(hWnd);	
	return TRUE;
}

