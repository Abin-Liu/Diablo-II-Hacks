// D2ItemParser.h : main header file for the D2ITEMPARSER application
//

#if !defined(AFX_D2ITEMPARSER_H__15B9B23F_BA8F_4C46_A114_F3B2EB4BBB59__INCLUDED_)
#define AFX_D2ITEMPARSER_H__15B9B23F_BA8F_4C46_A114_F3B2EB4BBB59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CD2ItemParserApp:
// See D2ItemParser.cpp for the implementation of this class
//

class CD2ItemParserApp : public CWinApp
{
public:
	CD2ItemParserApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2ItemParserApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CD2ItemParserApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2ITEMPARSER_H__15B9B23F_BA8F_4C46_A114_F3B2EB4BBB59__INCLUDED_)
