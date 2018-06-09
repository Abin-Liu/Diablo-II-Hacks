#if !defined(AFX_SKILLTYPEDLG_H__F5DD5F49_F0EA_4F5F_8E27_FF57CF38C67C__INCLUDED_)
#define AFX_SKILLTYPEDLG_H__F5DD5F49_F0EA_4F5F_8E27_FF57CF38C67C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkillTypeDlg dialog

class CSkillTypeDlg : public CDialog
{
// Construction
public:
	DWORD GetData() const;
	CSkillTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSkillTypeDlg)
	enum { IDD = IDD_SKILLTYPEDLG };
	int		m_nSkillType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkillTypeDlg)
	afx_msg void OnSkilltype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DWORD m_dwData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLTYPEDLG_H__F5DD5F49_F0EA_4F5F_8E27_FF57CF38C67C__INCLUDED_)
