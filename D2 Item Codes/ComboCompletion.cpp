// ComboCompletion.cpp : implementation file
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
// Modified: 12 Sep 1998 Setting correct cursor position after 
//                       auto-complete: Petr Stejskal and Ryan Schneider
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComboCompletion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboCompletion

CComboCompletion::CComboCompletion()
{
	m_bAutoComplete = TRUE;
}

CComboCompletion::~CComboCompletion()
{
}


BEGIN_MESSAGE_MAP(CComboCompletion, CComboBox)
	//{{AFX_MSG_MAP(CComboCompletion)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboCompletion message handlers

BOOL CComboCompletion::PreTranslateMessage(MSG* pMsg)
{
	// Need to check for backspace/delete. These will modify the text in
	// the edit box, causing the auto complete to just add back the text
	// the user has just tried to delete. 

	if (pMsg->message == WM_KEYDOWN)
	{
		m_bAutoComplete = TRUE;

		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_DELETE || nVirtKey == VK_BACK)
			m_bAutoComplete = FALSE;
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

void CComboCompletion::OnEditUpdate() 
{
  // if we are not to auto update the text, get outta here
  if (!m_bAutoComplete) 
      return;

  // Get the text in the edit box
  CString str;
  GetWindowText(str);
  int nLength = str.GetLength();
  
  // Currently selected range
  DWORD dwCurSel = GetEditSel();
  WORD dStart = LOWORD(dwCurSel);
  WORD dEnd   = HIWORD(dwCurSel);

  // Search for, and select in, and string in the combo box that is prefixed
  // by the text in the edit box
  if (SelectString(-1, str) == CB_ERR)
  {
      SetWindowText(str);		// No text selected, so restore what was there before
      if (dwCurSel != CB_ERR)
        SetEditSel(dStart, dEnd);	//restore cursor postion
  }

  // Set the text selection as the additional text that we have added
  if (dEnd < nLength && dwCurSel != CB_ERR)
      SetEditSel(dStart, dEnd);
  else
      SetEditSel(nLength, -1);
}
