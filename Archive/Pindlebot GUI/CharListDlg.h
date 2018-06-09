#if !defined(AFX_CHARLISTDLG_H__49678844_2E4D_4E1B_B723_17E2AEE45309__INCLUDED_)
#define AFX_CHARLISTDLG_H__49678844_2E4D_4E1B_B723_17E2AEE45309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharListDlg.h : header file
//

#include "ReportCtrl.h"
#include "PBotConfig.h"
#include "ArrayEx.h"
#include "CharDlg.h"
#include "BtnST.h"

typedef CArrayEx<PBOTCONFIG, const PBOTCONFIG&> ConfigList;


/////////////////////////////////////////////////////////////////////////////
// CCharListDlg dialog

class CCharListDlg : public CDialog
{
// Construction
public:
	BOOL IsUseShopbot() const;
	DWORD GetIdleLength() const;
	CCharListDlg(ConfigList* pConfigList, DWORD dwIdleLength, BOOL bUseShopbot, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCharListDlg)
	enum { IDD = IDD_CHARLISTDLG };
	CButton	m_wndUseShopbot;
	CButtonST	m_wndShopbot;
	CButtonST	m_wndOK;
	CButtonST	m_wndCancel;
	CButtonST	m_wndRen;
	CButtonST	m_wndEdit;
	CButtonST	m_wndDel;
	CButtonST	m_wndAdd;
	CReportCtrl	m_wndChars;
	DWORD	m_dwIdleLen;
	BOOL	m_bUseShopbot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCharListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnRen();
	afx_msg void OnEdit();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnShopbot();
	afx_msg void OnUseshopbot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int FindByName(LPCTSTR lpszName) const;
	ConfigList* m_pConfigList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARLISTDLG_H__49678844_2E4D_4E1B_B723_17E2AEE45309__INCLUDED_)
