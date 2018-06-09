// ItemDB.h : main header file for the ITEMDB application
//

#if !defined(AFX_ITEMDB_H__918DA7E0_C2EB_485E_82B9_284FD2967CE4__INCLUDED_)
#define AFX_ITEMDB_H__918DA7E0_C2EB_485E_82B9_284FD2967CE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "External\\WinAppEx.h"

/////////////////////////////////////////////////////////////////////////////
// CItemDBApp:
// See ItemDB.cpp for the implementation of this class
//

class CItemDBApp : public CWinAppEx
{
public:
	CItemDBApp();

	CString GetAppTitle() const;
	void SetLanguage(BOOL bLang);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemDBApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CItemDBApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_sAppTitle;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMDB_H__918DA7E0_C2EB_485E_82B9_284FD2967CE4__INCLUDED_)
