#if !defined(AFX_FILTERDLG_H__870C9257_C30D_410C_99B3_CB804A503960__INCLUDED_)
#define AFX_FILTERDLG_H__870C9257_C30D_410C_99B3_CB804A503960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterDlg.h : header file
//
#include "D2PacketFilter.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog

class CFilterDlg : public CDialog
{
// Construction
public:
	CFilterDlg(BOOL bReceived, CD2PacketFilter* pFilter, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterDlg)
	enum { IDD = IDD_FILTERDLG };
	CButton	m_wndBlockAll;
	CButton	m_wndAllowAll;
	CButton	m_wndBlock;
	CButton	m_wndAllow;
	CListCtrl	m_wndBlockedList;
	CListCtrl	m_wndAllowedList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bReceived;
	CD2PacketFilter* m_pFilter;

	// Generated message map functions
	//{{AFX_MSG(CFilterDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBlock();
	afx_msg void OnAllow();
	afx_msg void OnBlockall();
	afx_msg void OnAllowall();
	afx_msg void OnItemchangedListallowed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListblocked(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERDLG_H__870C9257_C30D_410C_99B3_CB804A503960__INCLUDED_)
