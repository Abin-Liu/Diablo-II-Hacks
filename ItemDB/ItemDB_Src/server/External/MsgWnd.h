////////////////////////////////////////////////////////////////////
// MsgWnd.h
//
// Global invisible window for message relaying.
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////

#ifndef __MSGWND_H__
#define __MSGWND_H__

#include <windows.h>

class CMsgWnd  
{
public:		

	CMsgWnd();
	virtual ~CMsgWnd();

	virtual BOOL Create(HWND hTargetWnd, LPCTSTR lpszClassName, LPCTSTR lpszWindowName = NULL);
	virtual void Destroy();
	HWND GetSafeHwnd() const;

protected:

	virtual LRESULT RelayMessage(UINT nMsg, WPARAM wParam, LPARAM lParam) const;
	static LRESULT CALLBACK RelayMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	HWND m_hWnd;
	HWND m_hTargetWnd;
};

#endif // __MSGWND_H__
