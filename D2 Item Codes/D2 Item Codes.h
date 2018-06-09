// D2 Item Codes.h : main header file for the D2 ITEM CODES application
//

#if !defined(AFX_D2ITEMCODES_H__C625A5E6_C63D_49D6_AD71_20DBA83590AD__INCLUDED_)
#define AFX_D2ITEMCODES_H__C625A5E6_C63D_49D6_AD71_20DBA83590AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CD2ItemCodesApp:
// See D2 Item Codes.cpp for the implementation of this class
//

class CD2ItemCodesApp : public CWinApp
{
public:
	CD2ItemCodesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2ItemCodesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD2ItemCodesApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2ITEMCODES_H__C625A5E6_C63D_49D6_AD71_20DBA83590AD__INCLUDED_)
