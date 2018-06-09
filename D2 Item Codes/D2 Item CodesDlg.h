// D2 Item CodesDlg.h : header file
//

#if !defined(AFX_D2ITEMCODESDLG_H__5F548E45_94E0_4500_A089_16A9DBAB8DB3__INCLUDED_)
#define AFX_D2ITEMCODESDLG_H__5F548E45_94E0_4500_A089_16A9DBAB8DB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CD2ItemCodesDlg dialog

#include "ArrayEx.h"
#include "EditEx.h"
#include "CodeDef.h"
#include "ComboCompletion.h"

class CD2ItemCodesDlg : public CDialog
{
// Construction
public:
	CD2ItemCodesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CD2ItemCodesDlg)
	enum { IDD = IDD_D2ITEMCODES_DIALOG };
	CComboCompletion	m_wndNames;
	CComboCompletion	m_wndCodes;
	CEditEx	m_wndOutput;
	CString	m_sOutput;
	int		m_nType;
	CString	m_sCodes;
	CString	m_sNames;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2ItemCodesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CD2ItemCodesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDoit();
	afx_msg void OnType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void LoadLists();

	CArrayEx<ITEMENTRY1, const ITEMENTRY1&> m_aList1;
	CArrayEx<ITEMENTRY2, const ITEMENTRY2&> m_aList2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2ITEMCODESDLG_H__5F548E45_94E0_4500_A089_16A9DBAB8DB3__INCLUDED_)
