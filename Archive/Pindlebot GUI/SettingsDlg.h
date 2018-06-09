#if !defined(AFX_SETTINGSDLG_H__43B7ED7A_D75B_47DD_AF61_0DCC32B227C2__INCLUDED_)
#define AFX_SETTINGSDLG_H__43B7ED7A_D75B_47DD_AF61_0DCC32B227C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsDlg.h : header file
//
#include "ReportCtrl.h"
#include "PBotConfig.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog

class CSettingsDlg : public CDialog
{
// Construction
public:
	CSettingsDlg(LPPBOTCONFIG pConfig, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingsDlg)
	enum { IDD = IDD_SETTINGSDLG };
	CButtonST	m_wndCancel;
	CButtonST	m_wndOK;
	CButtonST	m_wndOnCorpse;
	CButtonST	m_wndOnPindle;
	CButtonST	m_wndInRed;
	CButtonST	m_wndBeforeRed;
	CButtonST	m_wndEditCombo;
	CButtonST	m_wndDelCombo;
	CButtonST	m_wndAddCombo;
	CReportCtrl	m_wndCombo;
	BYTE	m_iPetHP;
	BOOL	m_bQuitIfPetDie;
	int		m_nStay;
	int		m_nAmmoType;
	BYTE	m_iMalahHP;
	BYTE	m_iMalahMana;
	BYTE	m_iPotionHP;
	BYTE	m_iPotionMana;
	BYTE	m_iQuitHP;
	BYTE	m_iQuitMana;
	BYTE	m_iRepairPercent;
	BOOL	m_bResMerc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBeforered();
	afx_msg void OnInred();
	afx_msg void OnOnpindle();
	afx_msg void OnOncorpse();
	afx_msg void OnAddcombo();
	afx_msg void OnDelcombo();
	virtual void OnOK();
	afx_msg void OnEditcombo();
	afx_msg void OnItemchangedDeadlycombo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkDeadlycombo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL ComboMatch(DWORD dwCombo1, DWORD dwCombo2);
	int ListCombo(int nIndex, DWORD dwCombo);
	int FindCombo(DWORD dwCombo) const;
	LPPBOTCONFIG m_pConfig;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDLG_H__43B7ED7A_D75B_47DD_AF61_0DCC32B227C2__INCLUDED_)
