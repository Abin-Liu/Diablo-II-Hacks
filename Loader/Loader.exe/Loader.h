// Loader.h : main header file for the LOADER application
//

#if !defined(AFX_LOADER_H__7362DB9C_1066_4066_9141_22BB73955743__INCLUDED_)
#define AFX_LOADER_H__7362DB9C_1066_4066_9141_22BB73955743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "WinAppEx.h"

/////////////////////////////////////////////////////////////////////////////
// CLoaderApp:
// See Loader.cpp for the implementation of this class
//

class CLoaderApp : public CWinAppEx
{
public:
	CLoaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoaderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLoaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADER_H__7362DB9C_1066_4066_9141_22BB73955743__INCLUDED_)
