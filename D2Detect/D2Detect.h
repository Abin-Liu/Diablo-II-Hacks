// D2Detect.h : main header file for the D2DETECT application
//

#if !defined(AFX_D2DETECT_H__8A8E1406_A2C6_4329_9A0F_2489075DF1E0__INCLUDED_)
#define AFX_D2DETECT_H__8A8E1406_A2C6_4329_9A0F_2489075DF1E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CD2DetectApp:
// See D2Detect.cpp for the implementation of this class
//

class CD2DetectApp : public CWinApp
{
public:
	CD2DetectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2DetectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD2DetectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2DETECT_H__8A8E1406_A2C6_4329_9A0F_2489075DF1E0__INCLUDED_)
