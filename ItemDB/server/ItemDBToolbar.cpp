// ItemDBToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "ItemDB.h"
#include "ItemDBToolbar.h"
#include "InternalMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EDIT_WIDTH		280 //the width of the edit box
//#define COMBO_WIDTH		120
#define CHK_WIDTH		100

/////////////////////////////////////////////////////////////////////////////
// CItemDBToolbar

CItemDBToolbar::CItemDBToolbar()
{
}

CItemDBToolbar::~CItemDBToolbar()
{
}


BEGIN_MESSAGE_MAP(CItemDBToolbar, CToolBar)
	//{{AFX_MSG_MAP(CItemDBToolbar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CItemDBToolbar::CreateEditCtrl()
{
	if (!::IsWindow(m_hWnd))
		return FALSE;

	int	index = 0;
    while (GetItemID(index) != ID_TBEDIT)
		index++;

    //next convert that button to a seperator and get its position
    SetButtonInfo(index, ID_TBEDIT, TBBS_SEPARATOR, EDIT_WIDTH);
	CRect rect;
    GetItemRect(index, &rect);

	if (!m_wndEdit.CreateEx(WS_EX_CLIENTEDGE,
		_T("EDIT"),
		NULL,
		ES_AUTOHSCROLL | ES_NOHIDESEL | WS_CHILD | WS_VISIBLE,
		rect,
		this,
		IDC_SEARCHEDIT))
    {
        TRACE0("Failed to create editbox\n");
        return FALSE;
    }

	index = 0;
	while (GetItemID(index) != ID_TBCHK)
		index++;

    //next convert that button to a seperator and get its position
    SetButtonInfo(index, ID_TBCHK, TBBS_SEPARATOR, CHK_WIDTH);
    GetItemRect(index, &rect);
	rect.InflateRect(-16, 0, 0, 0);

	if (!m_wndChk.Create(g_bLanguage ? "Ä£ºý²éÑ¯" : "Partial Match", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE,
		rect, this, 0))
    {
        TRACE0("Failed to create chkbox\n");
        return FALSE;
    }

	return TRUE;
}

BOOL CItemDBToolbar::AddArrow(UINT nID)
{
	if (!::IsWindow(m_hWnd))
		return FALSE;

	GetToolBarCtrl().SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_DRAWDDARROWS);
	DWORD dwStyle = GetButtonStyle(CommandToIndex(nID));
    dwStyle |= TBSTYLE_DROPDOWN;
    SetButtonStyle(CommandToIndex(nID), dwStyle);
	return TRUE;	
}

BOOL CItemDBToolbar::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && GetFocus() == &m_wndEdit)
		AfxGetMainWnd()->PostMessage(WM_START_SEARCH, 0, 0);
	return CToolBar::PreTranslateMessage(pMsg);
}
