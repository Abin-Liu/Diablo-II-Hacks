// D2ClassWizard.h : main header file for the D2CLASSWIZARD application
//

#if !defined(AFX_D2CLASSWIZARD_H__98AC15F8_2CB5_411B_9591_2FAE258D5FB0__INCLUDED_)
#define AFX_D2CLASSWIZARD_H__98AC15F8_2CB5_411B_9591_2FAE258D5FB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CD2ClassWizardApp:
// See D2ClassWizard.cpp for the implementation of this class
//

class CD2ClassWizardApp : public CWinApp
{
public:
	CD2ClassWizardApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2ClassWizardApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD2ClassWizardApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2CLASSWIZARD_H__98AC15F8_2CB5_411B_9591_2FAE258D5FB0__INCLUDED_)
