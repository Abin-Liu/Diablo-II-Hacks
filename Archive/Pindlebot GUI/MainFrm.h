// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8CDB1944_6AA5_41BD_B47D_929F6FA7DFBF__INCLUDED_)
#define AFX_MAINFRM_H__8CDB1944_6AA5_41BD_B47D_929F6FA7DFBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SystemTray.h"
#include "CharListDlg.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPopupHelp();
	afx_msg void OnPopupItemlog();
	afx_msg void OnPopupSettings();
	afx_msg void OnPopupStatistics();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void SaveConfig();
	void LoadConfig();
	LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	CSystemTray m_tray;
	HICON m_hIcon;
	ConfigList m_aConfigs;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8CDB1944_6AA5_41BD_B47D_929F6FA7DFBF__INCLUDED_)
