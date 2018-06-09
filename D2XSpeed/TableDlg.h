#if !defined(AFX_TABLEDLG_H__53BCFC05_FBF4_48C9_864E_7A5D07775889__INCLUDED_)
#define AFX_TABLEDLG_H__53BCFC05_FBF4_48C9_864E_7A5D07775889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTableDlg dialog
#include "WCRelation.h"

class CTableDlg : public CDialog
{
// Construction
public:
	void ShowTable();
	CTableDlg(WCRelation* pWcr, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTableDlg)
	enum { IDD = IDD_TABLEDLG };
	CListCtrl	m_ctlTable;
	CString	m_strCurIas;
	CString	m_strBase;
	CString	m_strWsm;
	CString	m_strCurSIas;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTableDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	WCRelation* m_pWcr;
public:
	CString m_strCurSpeed;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLEDLG_H__53BCFC05_FBF4_48C9_864E_7A5D07775889__INCLUDED_)
