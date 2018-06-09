// CharView.cpp : implementation file
//

#include "stdafx.h"
#include "ItemDB.h"
#include "CharView.h"
#include "InternalMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharView

IMPLEMENT_DYNCREATE(CCharView, CFormView)

CCharView::CCharView()
	: CFormView(CCharView::IDD)
{
	//{{AFX_DATA_INIT(CCharView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCharView::~CCharView()
{
}

void CCharView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharView)
	DDX_Control(pDX, IDC_LIST1, m_wndCharList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharView, CFormView)
	//{{AFX_MSG_MAP(CCharView)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharView message handlers

void CCharView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (nType != SIZE_MINIMIZED && ::IsWindow(m_wndCharList.m_hWnd))
	{
		CRect rect;
		GetClientRect(&rect);
		m_wndCharList.MoveWindow(&rect);
	}
}

void CCharView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if (::IsWindow(m_wndCharList.m_hWnd))
	{
		CRect rect;
		GetClientRect(&rect);
		m_wndCharList.MoveWindow(&rect);
	}

	AfxGetMainWnd()->SendMessage(WM_SET_CHARVIEW, 0, (LPARAM)&m_wndCharList);
}

void CCharView::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndCharList.GetSelectedChar();
	DWORD dwCharID = nCurSel == -1 ? 0 : (m_wndCharList.GetItemData(nCurSel) & ~HC_FLAG);
	AfxGetMainWnd()->SendMessage(WM_LIST_ITEMS, 0, dwCharID);
	*pResult = 0;
}

