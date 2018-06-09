// ShopbotCfgDlg.h : header file
//

#if !defined(AFX_SHOPBOTCFGDLG_H__54F79FED_E3DB_4BD3_BBAD_8329AC41AE6A__INCLUDED_)
#define AFX_SHOPBOTCFGDLG_H__54F79FED_E3DB_4BD3_BBAD_8329AC41AE6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ReportCtrl.h"
#include "Ini.h"
#include "SBItemConfig.h"
#include "ArrayEx.h"
#include "BtnST.h"
#include "EditEx.h"
#include "D2ObjFingerPrinter.h"

/////////////////////////////////////////////////////////////////////////////
// CShopbotCfgDlg dialog

class CShopbotCfgDlg : public CDialog
{
// Construction
public:
	CShopbotCfgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CShopbotCfgDlg)
	enum { IDD = IDD_SHOPBOTCFG_DIALOG };
	CComboBox	m_wndAct;
	CComboBox	m_wndWP;
	CComboBox	m_wndNPC;
	CEditEx	m_wndStopHP;
	CButtonST	m_wndOK;
	CButtonST	m_wndCancel;
	CButtonST	m_wndEdit;
	CButtonST	m_wndRemove;
	CButtonST	m_wndAdd;
	CReportCtrl	m_wndList;
	int		m_nHotkey;
	int		m_nStopHP;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShopbotCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CShopbotCfgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEdit();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangeAct();
	afx_msg void OnSelchangeGlobal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL AddMap(BYTE iMapID, BOOL bDangerous = FALSE);
	BOOL AddNPC(LPCD2OBJFINGERPRINTER pNpc, LPCTSTR lpszNpcName);
	void SaveConfig();
	int FindWP(DWORD dwMap) const;
	int FindNPC(WORD wNPC) const;
	void LoadWP();
	void LoadNPC();
	void LoadConfig();
	CIni m_shopbot_ini;
	CArrayEx<SBITEMCONFIG, const SBITEMCONFIG&> m_aConfigs;
	BOOL m_bItemChanged;

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOPBOTCFGDLG_H__54F79FED_E3DB_4BD3_BBAD_8329AC41AE6A__INCLUDED_)
