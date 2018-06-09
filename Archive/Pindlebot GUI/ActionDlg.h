#if !defined(AFX_ACTIONDLG_H__61C42041_AC04_46C2_BF60_180CEEEF143D__INCLUDED_)
#define AFX_ACTIONDLG_H__61C42041_AC04_46C2_BF60_180CEEEF143D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionDlg.h : header file
//
#include "PBotConfig.h"
#include "ArrayEx.h"
#include "SpellDef.h"

/////////////////////////////////////////////////////////////////////////////
// CActionDlg dialog

class CActionDlg : public CDialog
{
// Construction
public:
	CActionDlg(int nType, LPPBOTACTION pAction, BYTE iCharClass, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionDlg)
	enum { IDD = IDD_ACTIONDLG };
	CComboBox	m_wndTarget;
	CComboBox	m_wndSpells;
	int		m_nLeftRight;
	BYTE	m_iRepeat;
	UINT	m_wDelay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSkills();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void CheckStats();
	BOOL IsSpellAllowed(const D2SPELLINFO& dsi) const;
	BOOL InitSpells();
	int m_nType;
	BYTE m_iCharClass;
	CArrayEx<D2SPELLINFO, const D2SPELLINFO&> m_aSpells;
	LPPBOTACTION m_pAction;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONDLG_H__61C42041_AC04_46C2_BF60_180CEEEF143D__INCLUDED_)
