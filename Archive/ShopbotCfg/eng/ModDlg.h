#if !defined(AFX_MODDLG_H__4938A33C_499B_4049_9887_03817CF6645A__INCLUDED_)
#define AFX_MODDLG_H__4938A33C_499B_4049_9887_03817CF6645A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModDlg.h : header file
//
#include "SBItemConfig.h"
#include "D2ModParser.h"
#include "BtnST.h"
#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CModDlg dialog

class CModDlg : public CDialog
{
// Construction
public:
	LPCITEMMOD GetMod() const;
	CModDlg(LPCITEMMOD pMod = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModDlg)
	enum { IDD = IDD_MODDLG };
	CButtonST	m_wndOK;
	CButtonST	m_wndCancel;
	CEditEx	m_wndPreview;
	CButtonST	m_wndHelp2;
	CButtonST	m_wndHelp1;
	CEditEx	m_wndVal3;
	CEditEx	m_wndVal2;
	CEditEx	m_wndVal1;
	CComboBox	m_wndMods;
	DWORD	m_dwVal1;
	DWORD	m_dwVal2;
	DWORD	m_dwVal3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHelp1();
	afx_msg void OnHelp2();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnChangeVals();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void CheckStats();
	void Preview();
	ITEMMOD m_mod;
	int m_nAssistType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODDLG_H__4938A33C_499B_4049_9887_03817CF6645A__INCLUDED_)
