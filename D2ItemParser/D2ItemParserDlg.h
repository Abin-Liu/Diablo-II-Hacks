// D2ItemParserDlg.h : header file
//

#if !defined(AFX_D2ITEMPARSERDLG_H__66E5EA1C_6816_4904_8D15_AB59084622BD__INCLUDED_)
#define AFX_D2ITEMPARSERDLG_H__66E5EA1C_6816_4904_8D15_AB59084622BD__INCLUDED_
#include "MyRelayer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CD2ItemParserDlg dialog

class CD2ItemParserDlg : public CDialog
{
// Construction
public:
	CD2ItemParserDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CD2ItemParserDlg)
	enum { IDD = IDD_D2ITEMPARSER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2ItemParserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CMyRelayer m_relayer;

	// Generated message map functions
	//{{AFX_MSG(CD2ItemParserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2ITEMPARSERDLG_H__66E5EA1C_6816_4904_8D15_AB59084622BD__INCLUDED_)
