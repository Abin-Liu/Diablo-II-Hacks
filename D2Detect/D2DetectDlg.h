// D2DetectDlg.h : header file
//

#if !defined(AFX_D2DETECTDLG_H__0E6089F4_FB6E_4E3F_B4AC_F4B1C3E92DA2__INCLUDED_)
#define AFX_D2DETECTDLG_H__0E6089F4_FB6E_4E3F_B4AC_F4B1C3E92DA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CD2DetectDlg dialog

#include "ArrayEx.h"
#include "Diablo2CharStatsSearch.h"

class CD2DetectDlg : public CDialog
{
// Construction
public:
	CD2DetectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CD2DetectDlg)
	enum { IDD = IDD_D2DETECT_DIALOG };
	CListCtrl	m_wndList;
	BOOL	m_bShouldBeTrue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2DetectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DoDetect();
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CD2DetectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	HANDLE m_hProcess;
	CDiablo2CharStatsSearch m_d2Search;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2DETECTDLG_H__0E6089F4_FB6E_4E3F_B4AC_F4B1C3E92DA2__INCLUDED_)
