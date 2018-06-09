#if !defined(AFX_ITEMDBTOOLBAR_H__CC6914C8_3B08_4B2A_A37B_47CC3FCB6E92__INCLUDED_)
#define AFX_ITEMDBTOOLBAR_H__CC6914C8_3B08_4B2A_A37B_47CC3FCB6E92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemDBToolbar.h : header file
//

#include "External\\EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CItemDBToolbar window

class CItemDBToolbar : public CToolBar
{
// Construction
public:
	CItemDBToolbar();
	CEditEx m_wndEdit;
	CButton m_wndChk;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemDBToolbar)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL AddArrow(UINT nID);
	BOOL CreateEditCtrl();
	virtual ~CItemDBToolbar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CItemDBToolbar)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMDBTOOLBAR_H__CC6914C8_3B08_4B2A_A37B_47CC3FCB6E92__INCLUDED_)
