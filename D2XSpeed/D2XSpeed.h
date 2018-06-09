// D2XSpeed.h : main header file for the D2XSPEED application
//

#if !defined(AFX_D2XSPEED_H__D012A68D_8438_4AF9_8AAD_C96C888B44BB__INCLUDED_)
#define AFX_D2XSPEED_H__D012A68D_8438_4AF9_8AAD_C96C888B44BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CD2XSpeedApp:
// See D2XSpeed.cpp for the implementation of this class
//

class CD2XSpeedApp : public CWinApp
{
public:
	CD2XSpeedApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2XSpeedApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD2XSpeedApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2XSPEED_H__D012A68D_8438_4AF9_8AAD_C96C888B44BB__INCLUDED_)
