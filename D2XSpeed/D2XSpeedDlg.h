// D2XSpeedDlg.h : header file
//

#if !defined(AFX_D2XSPEEDDLG_H__40BACE33_2D0A_4717_ACFC_C69CECBE9C55__INCLUDED_)
#define AFX_D2XSPEEDDLG_H__40BACE33_2D0A_4717_ACFC_C69CECBE9C55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CD2XSpeedDlg dialog
#include "WCRelation.h"

class CD2XSpeedDlg : public CDialog
{
// Construction
public:
	void ListWeapons();
	void CheckStatus();
	CD2XSpeedDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CD2XSpeedDlg)
	enum { IDD = IDD_D2XSPEED_DIALOG };
	CComboBox	m_ctlCharClasses;
	CComboBox	m_ctlFilter;
	CComboBox	m_ctlAttackForms;
	CComboBox	m_ctlWeapon;
	CSpinButtonCtrl	m_ctlSpinFrenzy;
	CSpinButtonCtrl	m_ctlSpinIas;
	CSpinButtonCtrl	m_ctlSpinBos;
	CSpinButtonCtrl	m_ctlSpinFana;
	int		m_nBos;
	int		m_nFana;
	int		m_nIas;
	int		m_nFrenzy;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2XSpeedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ListAttackForm();
	void InitLists();
	void UpdateVals();
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CD2XSpeedDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnExit();
	afx_msg void OnAbout();
	afx_msg void OnHelp();
	afx_msg void OnShowtable();
	afx_msg void OnSelchangeAttackform();
	afx_msg void OnSelchangeClasses();
	afx_msg void OnSelchangeFilter();
	afx_msg void OnSelchangeWeapons();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	WCRelation m_wcr;
	CString m_strFilterBuffer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2XSPEEDDLG_H__40BACE33_2D0A_4717_ACFC_C69CECBE9C55__INCLUDED_)
