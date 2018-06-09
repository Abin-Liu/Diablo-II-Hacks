// D2Items.h : main header file for the D2ITEMS application
//

#if !defined(AFX_D2ITEMS_H__31B37D64_CCC7_465F_AA1F_DCB607996CC8__INCLUDED_)
#define AFX_D2ITEMS_H__31B37D64_CCC7_465F_AA1F_DCB607996CC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CD2ItemsApp:
// See D2Items.cpp for the implementation of this class
//

class CD2ItemsApp : public CWinApp
{
public:
	CD2ItemsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2ItemsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD2ItemsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2ITEMS_H__31B37D64_CCC7_465F_AA1F_DCB607996CC8__INCLUDED_)
