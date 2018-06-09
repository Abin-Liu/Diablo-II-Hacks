#if !defined(AFX_ITEMDBVIEW_H__BBDDC686_31D6_4177_9A88_0EE62F614F07__INCLUDED_)
#define AFX_ITEMDBVIEW_H__BBDDC686_31D6_4177_9A88_0EE62F614F07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemDBView.h : header file
//
#include "Diablo2\\D2View.h"

/////////////////////////////////////////////////////////////////////////////
// CItemDBView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CItemDBView : public CFormView
{
protected:
	CItemDBView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CItemDBView)

// Form Data
public:
	//{{AFX_DATA(CItemDBView)
	enum { IDD = IDD_ITEMDBDLG };
	CD2View	m_wndD2View;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemDBView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CItemDBView();

	// Generated message map functions
	//{{AFX_MSG(CItemDBView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMDBVIEW_H__BBDDC686_31D6_4177_9A88_0EE62F614F07__INCLUDED_)
