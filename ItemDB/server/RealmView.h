// RealmView.h : interface of the CRealmView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALMVIEW_H__4A3CE432_EF98_4036_9B76_60C6409DEF8C__INCLUDED_)
#define AFX_REALMVIEW_H__4A3CE432_EF98_4036_9B76_60C6409DEF8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRealmView : public CTreeView
{
protected: // create from serialization only
	CRealmView();
	DECLARE_DYNCREATE(CRealmView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealmView)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRealmView();


protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRealmView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALMVIEW_H__4A3CE432_EF98_4036_9B76_60C6409DEF8C__INCLUDED_)
