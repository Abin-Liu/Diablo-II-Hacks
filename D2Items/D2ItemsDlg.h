// D2ItemsDlg.h : header file
//

#if !defined(AFX_D2ITEMSDLG_H__753738B0_D9AB_400A_8851_4C818EB2CCF9__INCLUDED_)
#define AFX_D2ITEMSDLG_H__753738B0_D9AB_400A_8851_4C818EB2CCF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CD2ItemsDlg dialog

class CD2ItemsDlg : public CDialog
{
// Construction
public:
	CD2ItemsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CD2ItemsDlg)
	enum { IDD = IDD_D2ITEMS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2ItemsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CD2ItemsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2ITEMSDLG_H__753738B0_D9AB_400A_8851_4C818EB2CCF9__INCLUDED_)
