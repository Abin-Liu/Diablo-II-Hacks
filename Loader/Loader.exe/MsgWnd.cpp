////////////////////////////////////////////////////////////////////
// MsgWnd.cpp
//
// Global invisible window for message relaying.
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgWnd.h"

const CMsgWnd* g_pMsgWnd = NULL;

CMsgWnd::CMsgWnd()
{
	m_hWnd = NULL;
	m_hTargetWnd = NULL;
	g_pMsgWnd = this;
}

CMsgWnd::~CMsgWnd()
{
	Destroy();
}

BOOL CMsgWnd::Create(HWND hTargetWnd, LPCTSTR lpszClassName, LPCTSTR lpszWindowName)
{
	Destroy();	
	m_hTargetWnd = hTargetWnd;

	// Register our window class
	WNDCLASS wc = { 0 };	
	wc.lpfnWndProc = RelayMessage;
	wc.lpszClassName = lpszClassName;	
	::RegisterClass(&wc);
	m_hWnd = ::CreateWindow(lpszClassName, lpszWindowName, WS_OVERLAPPED, 0, 0, 1, 1, ::GetDesktopWindow(), NULL, NULL, 0);
	return m_hWnd != NULL;
}

void CMsgWnd::Destroy()
{
	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}

HWND CMsgWnd::GetSafeHwnd() const
{
	return ::IsWindow(m_hWnd) ? m_hWnd : NULL;
}

LRESULT CMsgWnd::RelayMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg >= WM_APP)
		return g_pMsgWnd ? g_pMsgWnd->RelayMessage(uMsg, wParam, lParam) : 0;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CMsgWnd::RelayMessage(UINT nMsg, WPARAM wParam, LPARAM lParam) const
{
	if (nMsg < WM_APP || !::IsWindow(m_hTargetWnd))
		return (LRESULT)0;

	return ::SendMessage(m_hTargetWnd, nMsg, wParam, lParam);
}