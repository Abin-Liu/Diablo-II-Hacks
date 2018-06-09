#if !defined(AFX_ITEMLISTVIEW_H__D70CC893_EB4F_4151_A65C_2FDA0CCFD213__INCLUDED_)
#define AFX_ITEMLISTVIEW_H__D70CC893_EB4F_4151_A65C_2FDA0CCFD213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemListView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "External\\ReportCtrl.h"

class CItemListView : public CFormView
{
protected:
	CItemListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CItemListView)

// Form Data
public:
	//{{AFX_DATA(CItemListView)
	enum { IDD = IDD_LISTDLG };
	CReportCtrl	m_wndSQLList;
	CReportCtrl	m_wndItemList;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CItemListView();

	// Generated message map functions
	//{{AFX_MSG(CItemListView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkItemlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSqllist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMLISTVIEW_H__D70CC893_EB4F_4151_A65C_2FDA0CCFD213__INCLUDED_)
