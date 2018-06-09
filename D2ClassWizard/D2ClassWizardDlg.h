// D2ClassWizardDlg.h : header file
//

#if !defined(AFX_D2CLASSWIZARDDLG_H__4C0A520C_9035_4F96_987B_8FF9CE8C31EF__INCLUDED_)
#define AFX_D2CLASSWIZARDDLG_H__4C0A520C_9035_4F96_987B_8FF9CE8C31EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CD2ClassWizardDlg dialog

class CD2ClassWizardDlg : public CDialog
{
// Construction
public:
	CD2ClassWizardDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CD2ClassWizardDlg)
	enum { IDD = IDD_D2CLASSWIZARD_DIALOG };
	CListCtrl	m_wndList;
	CString	m_sClassName;
	CString	m_sBaseClass;
	CString	m_sCppFile;
	CString	m_sHeaderFile;
	int		m_nMethod;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2ClassWizardDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CD2ClassWizardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnChangeClassname();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2CLASSWIZARDDLG_H__4C0A520C_9035_4F96_987B_8FF9CE8C31EF__INCLUDED_)
