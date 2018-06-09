// ItemDBView.cpp : implementation file
//

#include "stdafx.h"
#include "ItemDB.h"
#include "ItemDBView.h"
#include "InternalMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RESOURCE_FILE		"ItemDB.res"

/////////////////////////////////////////////////////////////////////////////
// CItemDBView

IMPLEMENT_DYNCREATE(CItemDBView, CFormView)

CItemDBView::CItemDBView()
	: CFormView(CItemDBView::IDD)
{
	//{{AFX_DATA_INIT(CItemDBView)
	//}}AFX_DATA_INIT
}

CItemDBView::~CItemDBView()
{
}

void CItemDBView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemDBView)
	DDX_Control(pDX, IDC_D2VIEW, m_wndD2View);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemDBView, CFormView)
	//{{AFX_MSG_MAP(CItemDBView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CItemDBView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_wndD2View.Initialize(AfxGetAppEx()->GetAppDir() + "\\" + RESOURCE_FILE);
	AfxGetMainWnd()->SendMessage(WM_SET_ITEMVIEW, 0, (LPARAM)&m_wndD2View);
}

void CItemDBView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, RGB(64, 64, 64));
}
