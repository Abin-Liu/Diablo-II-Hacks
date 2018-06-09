// RealmView.cpp : implementation of the CRealmView class
//

#include "stdafx.h"
#include "ItemDB.h"

#include "ItemDBDoc.h"
#include "RealmView.h"
#include "InternalMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealmView

IMPLEMENT_DYNCREATE(CRealmView, CTreeView)

BEGIN_MESSAGE_MAP(CRealmView, CTreeView)
	//{{AFX_MSG_MAP(CRealmView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealmView construction/destruction

CRealmView::CRealmView()
{
	// TODO: add construction code here

}

CRealmView::~CRealmView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRealmView message handlers

int CRealmView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= (TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP);
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here	
	return 0;
}

void CRealmView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	DWORD dwAccountID = 0;
	HTREEITEM hSel = GetTreeCtrl().GetSelectedItem();
	if (GetTreeCtrl().GetParentItem(hSel) != NULL)
		dwAccountID = GetTreeCtrl().GetItemData(hSel);

	AfxGetMainWnd()->SendMessage(WM_LIST_CHARS, 0, dwAccountID);
	*pResult = 0;
}

void CRealmView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	AfxGetMainWnd()->SendMessage(WM_SET_REALMVIEW, 0, (LPARAM)&GetTreeCtrl());
}