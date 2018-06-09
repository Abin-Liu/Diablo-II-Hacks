#if !defined(AFX_ComboCompletion_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_ComboCompletion_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ComboCompletion.h : header file
//
// Autocompleting combo-box (like the URL edit box in netscape)
//
// Written by Chris Maunder (cmaunder@mail.com)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then acknowledgement must be made to the author of this file 
// (in whatever form you wish).
//
// This file is provided "as is" with no expressed or implied warranty.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CComboCompletion window

class CComboCompletion : public CComboBox
{
// Construction
public:
	CComboCompletion();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboCompletion)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboCompletion();

	BOOL m_bAutoComplete;

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboCompletion)
	afx_msg void OnEditUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ComboCompletion_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_)
