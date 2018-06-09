#if !defined(AFX_CHARVIEW_H__F8F6A69F_AA13_4B64_A3A3_10F8167DAAE3__INCLUDED_)
#define AFX_CHARVIEW_H__F8F6A69F_AA13_4B64_A3A3_10F8167DAAE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "CharList.h"

class CCharView : public CFormView
{
protected:
	CCharView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCharView)

// Form Data
public:
	//{{AFX_DATA(CCharView)
	enum { IDD = IDD_CHARVIEWDLG };
	CCharList	m_wndCharList;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCharView();

	// Generated message map functions
	//{{AFX_MSG(CCharView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARVIEW_H__F8F6A69F_AA13_4B64_A3A3_10F8167DAAE3__INCLUDED_)
