// D2NoMin.h : main header file for the D2NOMIN application
//

#if !defined(AFX_D2NOMIN_H__9BAE7CA8_8111_405E_B4CC_10A23FA5EDCD__INCLUDED_)
#define AFX_D2NOMIN_H__9BAE7CA8_8111_405E_B4CC_10A23FA5EDCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CD2NoMinApp:
// See D2NoMin.cpp for the implementation of this class
//

class CD2NoMinApp : public CWinApp
{
public:
	CD2NoMinApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2NoMinApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD2NoMinApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2NOMIN_H__9BAE7CA8_8111_405E_B4CC_10A23FA5EDCD__INCLUDED_)
