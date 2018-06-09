// TestD2WndProcDlg.h : header file
//

#if !defined(AFX_TESTD2WNDPROCDLG_H__51E7BD4C_C4EE_493E_931C_3F268704B8E6__INCLUDED_)
#define AFX_TESTD2WNDPROCDLG_H__51E7BD4C_C4EE_493E_931C_3F268704B8E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestD2WndProcDlg dialog

class CTestD2WndProcDlg : public CDialog
{
// Construction
public:
	CTestD2WndProcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestD2WndProcDlg)
	enum { IDD = IDD_TESTD2WNDPROC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestD2WndProcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static LRESULT CALLBACK MyProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnHookProc(WPARAM wParam, LPARAM lParam);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestD2WndProcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTD2WNDPROCDLG_H__51E7BD4C_C4EE_493E_931C_3F268704B8E6__INCLUDED_)
