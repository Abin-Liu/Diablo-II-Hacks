// Pindlebot.h : main header file for the PINDLEBOT application
//

#if !defined(AFX_PINDLEBOT_H__269B7459_01FA_4B48_9846_414B7B16CEF4__INCLUDED_)
#define AFX_PINDLEBOT_H__269B7459_01FA_4B48_9846_414B7B16CEF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "WinAppEx.h"

/////////////////////////////////////////////////////////////////////////////
// CPindlebotApp:
// See Pindlebot.cpp for the implementation of this class
//

class CPindlebotApp : public CWinAppEx
{
public:
	CPindlebotApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPindlebotApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CPindlebotApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINDLEBOT_H__269B7459_01FA_4B48_9846_414B7B16CEF4__INCLUDED_)
