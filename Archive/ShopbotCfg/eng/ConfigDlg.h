#if !defined(AFX_CONFIGDLG_H__721E4B78_7FDE_48FB_8F60_61AF94352FCE__INCLUDED_)
#define AFX_CONFIGDLG_H__721E4B78_7FDE_48FB_8F60_61AF94352FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : header file
//

#include "SBItemConfig.h"
#include "ReportCtrl.h"
#include "EditEx.h"
#include "ArrayEx.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	LPCSBITEMCONFIG GetConfig() const;
	CConfigDlg(LPCSBITEMCONFIG pConfig, const CArrayEx<SBITEMCONFIG, const SBITEMCONFIG&>* pConfigs, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CFGDLG };
	CButtonST	m_wndOK;
	CButtonST	m_wndCancel;
	CButtonST	m_wndEdit;
	CButtonST	m_wndRemove;
	CButtonST	m_wndAdd;
	CEditEx	m_wndCode;
	CEditEx	m_wndDesc;
	CReportCtrl	m_wndMods;
	CString	m_sCode;
	CString	m_sDesc;
	int		m_nSockets;
	BOOL	m_bNonMagic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	virtual void OnOK();
	afx_msg void OnItemchangedMods(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkMods(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL VerifyName() const;
	void ListMods();
	SBITEMCONFIG m_config;
	const CArrayEx<SBITEMCONFIG, const SBITEMCONFIG&>* m_pConfigs;
	BOOL m_bNew;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLG_H__721E4B78_7FDE_48FB_8F60_61AF94352FCE__INCLUDED_)
