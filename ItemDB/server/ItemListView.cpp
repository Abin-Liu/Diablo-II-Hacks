// ItemListView.cpp : implementation file
//

#include "stdafx.h"
#include "ItemDB.h"
#include "ItemListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "InternalMsgs.h"

/////////////////////////////////////////////////////////////////////////////
// CItemListView

IMPLEMENT_DYNCREATE(CItemListView, CFormView)

CItemListView::CItemListView()
	: CFormView(CItemListView::IDD)
{
	//{{AFX_DATA_INIT(CItemListView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CItemListView::~CItemListView()
{
}

void CItemListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemListView)
	DDX_Control(pDX, IDC_SQLLIST, m_wndSQLList);
	DDX_Control(pDX, IDC_ITEMLIST, m_wndItemList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemListView, CFormView)
	//{{AFX_MSG_MAP(CItemListView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_ITEMLIST, OnDblclkItemlist)
	ON_NOTIFY(NM_DBLCLK, IDC_SQLLIST, OnDblclkSqllist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemListView message handlers

void CItemListView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (::IsWindow(m_wndItemList.m_hWnd))
		m_wndItemList.ResizeToFitParent();

	if (::IsWindow(m_wndSQLList.m_hWnd))
		m_wndSQLList.ResizeToFitParent();
}

void CItemListView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if (g_bLanguage)
	{
		m_wndItemList.SetColumnHeader("名称, 200; 代码, 50; 等级,50,2;质量,60;孔洞,60,1;无形,60,1;类别,60;位置,70");
		m_wndSQLList.SetColumnHeader("名称, 200; 代码, 50; 等级,50,2;质量,60;孔洞,60,1;无形,60,1;类别,60;位置,70;角色,70;账号,70;国度,70");
	}
	else
	{
		m_wndItemList.SetColumnHeader("Name, 200;Code, 50; iLvl,50,2;Quality,60;Sockets,60,1;Ethereal,60,1;Type,60;Location,70");
		m_wndSQLList.SetColumnHeader("Name, 200;Code, 50; iLvl,50,2;Quality,60;Sockets,60,1;Ethereal,60,1;Type,60;Location,70;Player,70;Account,70;Realm,70");
	}

	//m_wndItemList.SetBkColor(RGB(0, 0, 0));
	//m_wndSQLList.SetBkColor(RGB(0, 0, 0));
	m_wndItemList.ShowWindow(SW_SHOW);
	m_wndSQLList.ShowWindow(SW_HIDE);

	AfxGetMainWnd()->SendMessage(WM_SET_LISTVIEW, (WPARAM)&m_wndItemList, (LPARAM)&m_wndSQLList);
}

void CItemListView::OnDblclkItemlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIdx = m_wndItemList.GetFirstItem(RC_ITEM_SELECTED);
	if (nIdx != -1)
		AfxGetMainWnd()->SendMessage(WM_NAVIGATE_TO, 0, m_wndItemList.GetItemData(nIdx));

	*pResult = 0;
}

void CItemListView::OnDblclkSqllist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIdx = m_wndSQLList.GetFirstItem(RC_ITEM_SELECTED);
	if (nIdx != -1)
		AfxGetMainWnd()->SendMessage(WM_NAVIGATE_TO, 0, m_wndSQLList.GetItemData(nIdx));

	*pResult = 0;
}
