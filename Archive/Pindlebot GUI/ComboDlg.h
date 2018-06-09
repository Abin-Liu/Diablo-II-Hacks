#if !defined(AFX_COMBODLG_H__CA1639BE_B058_42AA_A1F3_ACA2A7C37B2C__INCLUDED_)
#define AFX_COMBODLG_H__CA1639BE_B058_42AA_A1F3_ACA2A7C37B2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboDlg.h : header file
//
#include "ReportCtrl.h"
#include "PindleBonusDef.h"

/////////////////////////////////////////////////////////////////////////////
// CComboDlg dialog

class CComboDlg : public CDialog
{
// Construction
public:
	DWORD GetCombo() const;
	CComboDlg(DWORD dwCombo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComboDlg)
	enum { IDD = IDD_COMBODLG };
	CButton	m_wndOK;
	CReportCtrl	m_wndCombos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int ListBonus(WORD wBonus, int nThreat);

	// Generated message map functions
	//{{AFX_MSG(CComboDlg)
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int FindBonus(WORD wBonus) const;
	DWORD m_dwCombo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBODLG_H__CA1639BE_B058_42AA_A1F3_ACA2A7C37B2C__INCLUDED_)
