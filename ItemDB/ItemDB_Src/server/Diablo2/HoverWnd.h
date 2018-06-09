#if !defined(AFX_HOVERWND_H__42003588_CAE8_46F9_84F9_5A0B35B3E690__INCLUDED_)
#define AFX_HOVERWND_H__42003588_CAE8_46F9_84F9_5A0B35B3E690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoverWnd.h : header file
//

#include "D2ItemDesc.h"
#define LINE_GAP	2


/////////////////////////////////////////////////////////////////////////////
// CHoverWnd window

class CHoverWnd : public CWnd
{
// Construction
public:
	CHoverWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoverWnd)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:

	void ShowHover(const CD2ItemData* pItem, const CRect& rect, const CRect& rcLimit);
	void HideHover();
	virtual ~CHoverWnd();

	// Generated message map functions
protected:

	void DrawText(const CString& str, int& y, CDC* pDC, int cx);
	const CD2ItemData* m_pCurItem;
	
	//{{AFX_MSG(CHoverWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOVERWND_H__42003588_CAE8_46F9_84F9_5A0B35B3E690__INCLUDED_)
