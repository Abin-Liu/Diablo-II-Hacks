// TestD2WndProc.h : main header file for the TESTD2WNDPROC application
//

#if !defined(AFX_TESTD2WNDPROC_H__7494A6E0_471A_436C_97B6_BA20283018B6__INCLUDED_)
#define AFX_TESTD2WNDPROC_H__7494A6E0_471A_436C_97B6_BA20283018B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestD2WndProcApp:
// See TestD2WndProc.cpp for the implementation of this class
//

class CTestD2WndProcApp : public CWinApp
{
public:
	CTestD2WndProcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestD2WndProcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestD2WndProcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTD2WNDPROC_H__7494A6E0_471A_436C_97B6_BA20283018B6__INCLUDED_)
