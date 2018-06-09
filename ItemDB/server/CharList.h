#if !defined(AFX_CHARLIST_H__AD9255C4_E74E_4DE7_93B1_BBB00CCAFAC7__INCLUDED_)
#define AFX_CHARLIST_H__AD9255C4_E74E_4DE7_93B1_BBB00CCAFAC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharList.h : header file

/////////////////////////////////////////////////////////////////////////////
// CCharList window

#define HC_FLAG	0x80000000

class CCharList : public CListCtrl
{
// Construction
public:
	CCharList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharList)
	//}}AFX_VIRTUAL

// Implementation
public:

	int GetSelectedChar() const;
	int FindCharFromList(DWORD dwPlayerID) const;
	virtual ~CCharList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCharList)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARLIST_H__AD9255C4_E74E_4DE7_93B1_BBB00CCAFAC7__INCLUDED_)
