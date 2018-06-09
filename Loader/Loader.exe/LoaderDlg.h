// LoaderDlg.h : header file
//

#if !defined(AFX_LOADERDLG_H__7E89905C_6B75_48BE_99E9_A8C2596908A8__INCLUDED_)
#define AFX_LOADERDLG_H__7E89905C_6B75_48BE_99E9_A8C2596908A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MsgWnd.h"
#include "ArrayEx.h"

/////////////////////////////////////////////////////////////////////////////
// CLoaderDlg dialog

class CLoaderDlg : public CDialog
{
// Construction
public:
	CLoaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLoaderDlg)
	enum { IDD = IDD_LOADER_DIALOG };
	CListCtrl	m_wndD2List;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLoadunload();
	//}}AFX_MSG
	LRESULT OnD2GameEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:

	BOOL CheckD2Hackit(int nIndex);
	void OnD2WndCreate(HWND hWnd);
	void OnD2WndDestroy(HWND hWnd, int nIndex);

	CMsgWnd m_msgWnd;
	CArrayEx<HWND, HWND> m_aCreats;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADERDLG_H__7E89905C_6B75_48BE_99E9_A8C2596908A8__INCLUDED_)
