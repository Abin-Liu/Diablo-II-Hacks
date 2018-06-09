// D2NoMinDlg.h : header file
//

#if !defined(AFX_D2NOMINDLG_H__850D3D0A_DAEB_4766_B5C4_FD0E2748C2DB__INCLUDED_)
#define AFX_D2NOMINDLG_H__850D3D0A_DAEB_4766_B5C4_FD0E2748C2DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CD2NoMinDlg dialog

struct D2ITEM
{
	HWND hWnd;
	BOOL bSelected;
	TCHAR szTitle[256];
};

#include "SystemTray.h"
#include "ArrayEx.h"
#include "ReportCtrl.h"

class CD2NoMinDlg : public CDialog
{
// Construction
public:
	CD2NoMinDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CD2NoMinDlg)
	enum { IDD = IDD_D2NOMIN_DIALOG };
	CReportCtrl	m_wndList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2NoMinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CD2NoMinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPopupAbout();
	afx_msg void OnPopupDisable();
	afx_msg void OnUpdatePopupDisable(CCmdUI* pCmdUI);
	afx_msg void OnPopupEnable();
	afx_msg void OnUpdatePopupEnable(CCmdUI* pCmdUI);
	virtual void OnCancel();
	afx_msg void OnRefresh();
	afx_msg void OnSetting();
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	LRESULT OnChkBox(WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam);
	void ListAllD2Wnds();
	HWND m_hwnd;
	CSystemTray m_tray;
	BOOL m_bEnabled;
	CArrayEx<D2ITEM, const D2ITEM&> m_aWnds;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2NOMINDLG_H__850D3D0A_DAEB_4766_B5C4_FD0E2748C2DB__INCLUDED_)
