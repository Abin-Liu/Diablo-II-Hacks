#if !defined(AFX_STATISTICSDLG_H__FED16326_89F5_4C04_8D1B_48F921B32982__INCLUDED_)
#define AFX_STATISTICSDLG_H__FED16326_89F5_4C04_8D1B_48F921B32982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg dialog

class CStatisticsDlg : public CDialog
{
// Construction
public:
	CStatisticsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatisticsDlg)
	enum { IDD = IDD_STATISTICSDLG };
	CString	m_sCharDied;
	CString	m_sMercDied;
	CString	m_sPindleKilled;
	CString	m_sRetreated;
	CString	m_sDeadlyCombo;
	CString	m_sInsufficientSkill;
	CString	m_sNetEffect;
	CString	m_sTotalTime;
	CString	m_sTotalGames;
	CString	m_sPotionDrunk;
	CString	m_sPotionMerc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GetPercent(int n, int nTotal) const;

	// Generated message map functions
	//{{AFX_MSG(CStatisticsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSDLG_H__FED16326_89F5_4C04_8D1B_48F921B32982__INCLUDED_)
