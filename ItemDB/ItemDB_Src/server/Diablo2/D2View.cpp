// D2View.cpp : implementation file
//

#include "stdafx.h"
#include "D2View.h"
#include "..\\External\\memdc.h"
#include "ItemDBDef.h"
#include "..\\InternalMsgs.h"
#include "..\\ItemDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CD2View

CD2View::CD2View()
{
	m_rcInvGold.SetRectEmpty();
	m_rcStashGold.SetRectEmpty();
	m_rcGoldLimit.SetRectEmpty();
	m_pPlayer = NULL;
	::memset(&m_ptMerc, 0, sizeof(POINT));	
	m_font.CreateFont(12, 0, 0, 0, 600, 
					  FALSE, FALSE,
					  0,
					  ANSI_CHARSET,
					  OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS,
					  DEFAULT_QUALITY,
					  DEFAULT_PITCH | FF_SWISS,
					  "Arial");	
	SetPosition(0, 0);
}

CD2View::~CD2View()
{
	m_font.DeleteObject();	
}


BEGIN_MESSAGE_MAP(CD2View, CStatic)
	//{{AFX_MSG_MAP(CD2View)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2View message handlers

void CD2View::OnPaint() 
{
	CPaintDC dc1(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemDC dc(&dc1, &rcClient);
	dc.FillSolidRect(&rcClient, RGB(64, 64, 64));
	
	m_equip.DrawStorage(&dc);
	m_inv.DrawStorage(&dc);
	m_stash.DrawStorage(&dc);
	m_cube.DrawStorage(&dc);
	m_merc.DrawStorage(&dc);

	if (m_cursor.GetItemCount() && m_cursor.IsVisible())
	{
		m_cursor.DrawStorage(&dc);
		CPoint pt = m_cursor.GetBoundary().CenterPoint();
		pt.Offset(10, 10);
		m_hand.Draw(&dc, pt);
	}		

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(196, 196, 196));	
	
	CString str;
	str.Format("%d", m_pPlayer ? m_pPlayer->dwInvGold : 0);
	dc.TextOut(m_rcInvGold.left, m_rcInvGold.top, str);

	if (m_stash.IsVisible())
	{
		str.Format("%d", m_pPlayer ? m_pPlayer->dwStashGold : 0);
		dc.TextOut(m_rcStashGold.left, m_rcStashGold.top, str);
		str.Format("%d", m_pPlayer ? m_pPlayer->dwGoldLimit : 50000);
		dc.TextOut(m_rcGoldLimit.left, m_rcGoldLimit.top, str);
	}	

	//CRect rect = rcClient;
	//dc.Draw3dRect(&rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
	//rect.DeflateRect(1, 1);
	//dc.Draw3dRect(&rect, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_BTNFACE));

		
	if (m_pPlayer && !m_merc.IsVisible() && m_mercPortrait.IsValid())
	{
		m_mercPortrait.Draw(&dc, m_ptMerc);
		CFont* pOld = dc.SelectObject(&m_font);
		dc.SetTextColor(RGB(255, 255, 255));
		dc.DrawText(m_pPlayer->sMercName, &m_rcMercName, DT_CENTER | DT_SINGLELINE);
		dc.SelectObject(pOld);
	}

	// debug
	//dc.FillSolidRect(&rc, RGB(255, 255, 255));
	//dc.FillSolidRect(&m_rcStashGold, RGB(255, 255, 255));
	//dc.FillSolidRect(&m_rcGoldLimit, RGB(255, 255, 255));
}

void CD2View::OpenCube()
{
	m_merc.SetVisible(FALSE);
	m_stash.SetVisible(FALSE);
	m_cube.SetVisible(TRUE);
	m_cursor.SetVisible(TRUE);
	RedrawWindow(&m_cube.GetBoundary());
}

void CD2View::OpenMerc()
{
	m_merc.SetVisible(TRUE);
	m_stash.SetVisible(FALSE);
	m_cube.SetVisible(FALSE);
	m_cursor.SetVisible(FALSE);
	RedrawWindow(&m_cube.GetBoundary());
}

BOOL CD2View::IsCubeOpened() const
{
	return m_cube.IsVisible();
}

BOOL CD2View::IsMercOpened() const
{
	return m_merc.IsVisible();
}

CD2Storage* CD2View::GetStorage(int nStorageType)
{
	return (CD2Storage*)GetStorageConst(nStorageType);
}

const CD2ItemData* CD2View::GetItemAtPos(POINT pt, CRect& rect) const
{
	const CD2ItemData* p = NULL;

	p = m_cursor.GetItemAtPos(pt, rect);
	if (p)
		return p;

	p = m_merc.GetItemAtPos(pt, rect);
	if (p)
		return p;

	p = m_equip.GetItemAtPos(pt, rect);
	if (p)
		return p;	

	p = m_cube.GetItemAtPos(pt, rect);
	if (p)
		return p;

	p = m_inv.GetItemAtPos(pt, rect);
	if (p)
		return p;

	p = m_stash.GetItemAtPos(pt, rect);
	if (p)
		return p;

	return NULL;
}

const CD2Storage* CD2View::GetStorage(int nStorageType) const
{
	return GetStorageConst(nStorageType);
}

int CD2View::FindItemByID(DWORD dwItemID, CRect &rect) const
{
	rect.SetRectEmpty();
	
	if (m_cursor.FindItemByID(dwItemID, rect))
		return STO_CURSOR;

	if (m_merc.FindItemByID(dwItemID, rect))
		return STO_MERC;

	if (m_equip.FindItemByID(dwItemID, rect))
		return STO_EQUIP;

	if (m_cube.FindItemByID(dwItemID, rect))
		return STO_CUBE;

	if (m_inv.FindItemByID(dwItemID, rect))
		return STO_INVENTORY;

	if (m_stash.FindItemByID(dwItemID, rect))
		return STO_STASH;

	return -1;
}

CString CD2View::GetLocationName(int nStorageType)
{
	CString str;
	switch (nStorageType)
	{
	case 0: // inventory
		str = "Inventory";
		break;

	case 1: // equip
		str = "Equip";
		break;

	case 3: // cube
		str = "Cube";
		break;

	case 4: // stash
		str = "Stash";
		break;

	case 10: // cursor
		str = "Cursor";
		break;

	default:
		break;
	}

	return str;
}

const CD2Storage* CD2View::GetStorageConst(int nSTorageType) const
{
	const CD2Storage* p = NULL;
	switch (nSTorageType)
	{
	case STO_INVENTORY: // inventory
		p = &m_inv;
		break;

	case STO_EQUIP: // equip
		p = &m_equip;
		break;

	case STO_CUBE: // cube
		p = &m_cube;
		break;

	case STO_STASH: // stash
		p = &m_stash;
		break;

	case STO_MERC: // merc
		p = &m_merc;

	case STO_CURSOR: // cursor
		p = &m_cursor;
		break;

	default:
		break;
	}

	return p;
}

void CD2View::SetPlayer(CD2PlayerData *pPlayer)
{
	if (m_pPlayer == pPlayer)
		return;

	m_equip.LoadFromPlayer(pPlayer);
	m_inv.LoadFromPlayer(pPlayer);
	m_stash.LoadFromPlayer(pPlayer);
	m_cube.LoadFromPlayer(pPlayer);
	m_cursor.LoadFromPlayer(pPlayer);
	m_merc.LoadFromPlayer(pPlayer);
	
	CloseCube();
	CloseMerc();
	m_mercPortrait.FreePictureData();
	m_pPlayer = pPlayer;	
	if (m_pPlayer == NULL)
	{
		RedrawWindow();
		return;
	}
	
	BYTE aData[10000];
	DWORD dwResSize = 0;

	switch (pPlayer->dwMercClass)
	{
	case 0x010f:
		dwResSize = m_res.Extract(IDB_IMG_MERC1, aData, 10000);
		m_mercPortrait.Load(aData, dwResSize);
		break;

	case 0x0152:
		dwResSize = m_res.Extract(IDB_IMG_MERC2, aData, 10000);
		m_mercPortrait.Load(aData, dwResSize);
		break;

	case 0x0167:
		dwResSize = m_res.Extract(IDB_IMG_MERC3, aData, 10000);
		m_mercPortrait.Load(aData, dwResSize);
		break;

	case 0x0231:
		dwResSize = m_res.Extract(IDB_IMG_MERC5, aData, 10000);
		m_mercPortrait.Load(aData, dwResSize);
		break;

	default:
		break;
	}
	RedrawWindow();
}

void CD2View::CloseCube()
{
	m_merc.SetVisible(FALSE);
	m_stash.SetVisible(TRUE);
	m_cube.SetVisible(FALSE);
	RedrawWindow(&m_cube.GetBoundary());
}

void CD2View::CloseMerc()
{
	m_merc.SetVisible(FALSE);
	m_stash.SetVisible(TRUE);
	m_cube.SetVisible(FALSE);
	m_cursor.SetVisible(TRUE);
	RedrawWindow(&m_cube.GetBoundary());
}

CResPack* CD2View::GetResPack()
{
	return &m_res;
}

BOOL CD2View::Initialize(LPCSTR lpszResourcePath)
{
	if (!m_res.OpenPackage(lpszResourcePath, FALSE, FALSE))
		return FALSE;

	// create the hover window
	m_wndHover.Create(this);

	BYTE aData[1024];
	DWORD dwResSize = m_res.Extract(IDB_IMG_HAND, aData, 100000);
	m_hand.Load(aData,dwResSize);	
	
	m_cursor.SetType(STO_CURSOR, &m_res);
	m_equip.SetType(STO_EQUIP, &m_res);
	m_inv.SetType(STO_INVENTORY, &m_res);
	m_stash.SetType(STO_STASH, &m_res);
	m_cube.SetType(STO_CUBE, &m_res);
	m_merc.SetType(STO_MERC, &m_res);

	m_cube.SetVisible(FALSE);
	m_merc.SetVisible(FALSE);

	SetPosition(0, 0);
	return TRUE;
}

void CD2View::SetPosition(int x, int y)
{
	m_stash.SetPosition(x, y);
	m_cube.SetPosition(x, y);
	m_equip.SetPosition(m_stash.GetBoundary().right, m_stash.GetBoundary().top);	
	m_inv.SetPosition(m_stash.GetBoundary().right, m_equip.GetBoundary().bottom);
	m_merc.SetPosition(x, y);

	CPoint pt = m_equip.GetBoundary().TopLeft();
	pt.Offset(-65, 140);
	m_cursor.SetPosition(pt.x, pt.y);

	m_ptMerc.x = x + 3;
	m_ptMerc.y = y + 5;
	m_rcMercName = CRect(m_ptMerc, CSize(47, 12));
	m_rcMercName.OffsetRect(0, 47);
	
	m_rcInvGold = m_inv.GetBoundary();
	m_rcInvGold.OffsetRect(107, 148);
	m_rcInvGold.right = m_rcInvGold.left + 90;
	m_rcInvGold.bottom = m_rcInvGold.top + 18;

	m_rcStashGold = m_stash.GetBoundary();
	m_rcStashGold.OffsetRect(160, 25);
	m_rcStashGold.right = m_rcStashGold.left + 150;
	m_rcStashGold.bottom = m_rcStashGold.top + 18;

	m_rcGoldLimit = m_rcStashGold;
	m_rcGoldLimit.OffsetRect(0, 27);
}

void CD2View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CStatic::OnMouseMove(nFlags, point);
	CheckMouseMove(point);
}

void CD2View::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CStatic::OnRButtonDown(nFlags, point);

	// is merc?
	if (!m_merc.IsVisible())
	{
		CRect rcMerc(m_ptMerc, m_mercPortrait.GetPictureSize());
		if (rcMerc.PtInRect(point))
			OpenMerc();
	}

	if (m_cube.IsVisible())
		return;

	CRect rect;
	const CD2ItemData* p = GetItemAtPos(point, rect);
	if (p == NULL || p->sCode != "box")
		return;

	OpenCube();
}

void CD2View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CStatic::OnLButtonDown(nFlags, point);
	if (m_equip.IsPtOnWeaponSwitch(point))
	{
		m_equip.WeaponSwitch();
		RedrawWindow(&m_equip.GetBoundary());
	}
	else if (m_cube.IsCloseButton(point))
	{
		CloseCube();
	}
	else if (m_merc.IsCloseButton(point))
	{
		CloseMerc();
	}
}

void CD2View::NotifyStatus(LPCSTR lpsz)
{
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd && ::IsWindow(pWnd->m_hWnd))
		pWnd->SendMessage(WM_STATUS_TEXT, 0, (LPARAM)lpsz);
}

BOOL CD2View::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	//return CStatic::OnEraseBkgnd(pDC);
}

CWnd* CD2View::GetHoverWnd()
{
	return &m_wndHover;
}

void CD2View::OnEscape()
{
	if (IsCubeOpened() || IsMercOpened())
	{
		CloseCube();
		CloseMerc();
	}
}

void CD2View::CheckMouseMove(POINT point)
{
	// merc portrait	
	BOOL bNotified = FALSE;
	if (m_pPlayer && !m_merc.IsVisible() && m_mercPortrait.IsValid())
	{
		CRect rect(m_ptMerc, m_mercPortrait.GetPictureSize());
		if (rect.PtInRect(point))
		{
			NotifyStatus(g_bLanguage ? "点击右键打开雇佣兵物品栏" : "Right-click to open merc inventory");
			bNotified = TRUE;
		}
	}

	if (m_equip.IsPtOnWeaponSwitch(point))
	{
		NotifyStatus(g_bLanguage ? "点击左键查看副手装备" : "Left-click to view secondary weapon switch items");
		bNotified = TRUE;
	}

	CRect rect;
	const CD2ItemData* p = GetItemAtPos(point, rect);
	if (p)
	{
		CRect rcLimit;
		GetClientRect(&rcLimit);
		m_wndHover.ShowHover(p, rect, rcLimit);
		
		if (p->sCode == "box")
		{
			NotifyStatus(g_bLanguage ? "点击右键打开赫拉迪克方块" : "Right-click to open the Horadic Cube");
			bNotified = TRUE;
		}
		else
		{
			NotifyStatus(p->sName);
			bNotified = TRUE;
		}
	}
	else
	{
		m_wndHover.HideHover();
	}

	if (!bNotified)
		NotifyStatus(g_bLanguage ? "移动鼠标查看物品属性" : "Hover cursor to view item attributes");
}

void CD2View::OnMove(int x, int y) 
{
	CStatic::OnMove(x, y);	
	SetPosition(0, 0);
}

void CD2View::GetMinRect(CRect &rect) const
{
	rect = m_stash.GetBoundary();
	rect.right = m_inv.GetBoundary().right;
}

const CD2PlayerData* CD2View::GetPlayer() const
{
	return m_pPlayer;
}

BOOL CD2View::IsCurrentPlayer(DWORD dwPlayerID) const
{
	if (m_pPlayer == NULL)
		return dwPlayerID == 0;

	return m_pPlayer->dwPlayerID == dwPlayerID;
}

DWORD CD2View::GetPlayerID() const
{
	return m_pPlayer ? m_pPlayer->dwPlayerID : 0;
}

BOOL CD2View::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->lParam & 0x40000000) == 0)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_SPACE)
			OnEscape();
	}
	return CStatic::PreTranslateMessage(pMsg);
}
