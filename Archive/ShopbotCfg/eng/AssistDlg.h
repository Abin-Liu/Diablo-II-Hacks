#if !defined(AFX_ASSISTDLG_H__D2C9DD49_37B0_498A_9DB9_6DD62D755CF1__INCLUDED_)
#define AFX_ASSISTDLG_H__D2C9DD49_37B0_498A_9DB9_6DD62D755CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssistDlg.h : header file
//

#include "ReportCtrl.h"
#include "BtnST.h"


/////////////////////////////////////////////////////////////////////////////
// CAssistDlg dialog

enum {
	ASSIST_CLASS = 0,
	ASSIST_TAB,
	ASSIST_AMA_SKILL,
	ASSIST_SOR_SKILL,
	ASSIST_NEC_SKILL,
	ASSIST_PAL_SKILL,
	ASSIST_BAR_SKILL,
	ASSIST_DRU_SKILL,
	ASSIST_ASN_SKILL
};

class CAssistDlg : public CDialog
{
// Construction
public:
	DWORD GetData() const;
	CAssistDlg(int nType, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAssistDlg)
	enum { IDD = IDD_ASSISTDLG };
	CButtonST	m_wndCancel;
	CButtonST	m_wndOK;
	CReportCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAssistDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ListSkill(WORD wSkillID);
	void ListSorSKill();
	void ListAsnSkill();
	void ListPalSKill();
	void ListNecSkill();
	void ListDruSkill();
	void ListBarSkill();
	void ListAmaSkill();
	void ListTab();
	void ListClass();
	int m_nType;
	DWORD m_dwData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASSISTDLG_H__D2C9DD49_37B0_498A_9DB9_6DD62D755CF1__INCLUDED_)
