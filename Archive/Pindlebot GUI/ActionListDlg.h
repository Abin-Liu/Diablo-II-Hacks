#if !defined(AFX_ACTIONLISTDLG_H__FB1A9F84_13E7_428B_956F_FB1D676D47DD__INCLUDED_)
#define AFX_ACTIONLISTDLG_H__FB1A9F84_13E7_428B_956F_FB1D676D47DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionListDlg.h : header file
//

#include "ReportCtrl.h"
#include "PBotConfig.h"
#include "ArrayEx.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CActionListDlg dialog

class CActionListDlg : public CDialog
{
// Construction
public:
	CActionListDlg(int nType, LPPBOTACTION pActions, BYTE* pCount, BYTE iCharClass, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionListDlg)
	enum { IDD = IDD_ACTIONLISTDLG };
	CButtonST	m_wndOK;
	CButtonST	m_wndCancel;
	CButtonST	m_wndUp;
	CButtonST	m_wndEdit;
	CButtonST	m_wndDown;
	CButtonST	m_wndDel;
	CButtonST	m_wndAdd;
	CReportCtrl	m_wndActions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnEdit();
	afx_msg void OnUp();
	afx_msg void OnDown();
	virtual void OnOK();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int ListAction(int nIndex, const PBOTACTION& action, BOOL bEdit);
	static CString GetTargetStr(int nTarget);
	int m_nType;
	LPPBOTACTION m_pActions;
	BYTE* m_pCount;
	BYTE m_iCharClass;
	CArrayEx<PBOTACTION, const PBOTACTION&> m_aActions;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONLISTDLG_H__FB1A9F84_13E7_428B_956F_FB1D676D47DD__INCLUDED_)
