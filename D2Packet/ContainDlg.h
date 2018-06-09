#if !defined(AFX_CONTAINDLG_H__01AF0ED5_68CF_4417_96D6_9C8E27079BBA__INCLUDED_)
#define AFX_CONTAINDLG_H__01AF0ED5_68CF_4417_96D6_9C8E27079BBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContainDlg dialog
class CContainDlg : public CDialog
{
// Construction
public:
	CString GetString() const;
	CContainDlg(LPCTSTR lpszContents, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContainDlg)
	enum { IDD = IDD_CONTAINDLG };
	CEdit	m_wndEdit;
	CString	m_sContents;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContainDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTAINDLG_H__01AF0ED5_68CF_4417_96D6_9C8E27079BBA__INCLUDED_)
