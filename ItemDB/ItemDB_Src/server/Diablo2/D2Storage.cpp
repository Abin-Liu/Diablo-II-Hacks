// D2Storage.cpp : implementation file
//

#include "stdafx.h"
#include "D2Storage.h"
#include "ItemDBDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TEMP_BUFFER	100000

#define GRID_DIM	29
#define MAX_EQUIP	13
#define MAX_MERC	6

static const SIZE WS_SIZE = { 64, 19 };
static const CRect WST1(CPoint(16, 24), WS_SIZE);
static const CRect WST2(CPoint(247, 24), WS_SIZE);

struct ESLOT
{
	int x;
	int y;
	int cx;
	int cy;
};

static const ESLOT EQUIPSLOTS[] = {
	{ 0, 0, 0, 0 },
	{ 130, 5, 2, 2 }, // head
	{ 205, 33, 1, 1 }, // amulet
	{ 133, 74, 2, 3 }, // body	
	{ 17, 48, 2, 4 }, // right hand	
	{ 247, 48, 2, 4 }, // left hand
	{ 90, 177, 1, 1 }, // right ring
	{ 205, 177, 1, 1 }, // left ring	
	{ 133, 176, 2, 1 }, // belt
	{ 247, 176, 2, 2 }, // boots
	{ 17, 176, 2, 2 }, // gloves
	{ 17, 48, 2, 4 }, // right hand 2
	{ 247, 48, 2, 4 }, // left hand 2
	{ -1, -1, 0, 0 }, // end
};

/////////////////////////////////////////////////////////////////////////////
// CD2Storage

CD2Storage::CD2Storage()
{
	m_nType = 0;
	m_cx = 0;
	m_cy = 0;
	::memset(&m_ptOrigin, 0, sizeof(POINT));
	::memset(&m_rcClose, 0, sizeof(RECT));
	m_bVisible = TRUE;
	m_bWs2 = FALSE;
}

CD2Storage::~CD2Storage()
{
}

/////////////////////////////////////////////////////////////////////////////
// CD2Storage message handlers

void CD2Storage::SetType(UINT nType, CResPack* pRes)
{
	ASSERT(pRes && pRes->GetOpenStatus());
	m_rcClose.SetRectEmpty();
	::memset(&m_ptOrigin, 0, sizeof(POINT));
	::memset(&m_rcClose, 0, sizeof(RECT));
	::memset(&m_ptCloseOrigin, 0, sizeof(POINT));

	BYTE aData[TEMP_BUFFER]; // 100k temp buffer
	DWORD dwResSize = 0;

	m_nType = nType;
	switch (m_nType)
	{
	case STO_EQUIP:
		m_cx = MAX_EQUIP;
		m_cy = 1;
		dwResSize = pRes->Extract(IDB_IMG_EQUIP, aData, TEMP_BUFFER);
		m_bkgnd.Load(aData, dwResSize);

		dwResSize = pRes->Extract(IDB_IMG_WS2, aData, TEMP_BUFFER);
		m_ws2.Load(aData, dwResSize);
		break;

	case STO_INVENTORY:
		m_ptOrigin.x = 15;
		m_ptOrigin.y = 10;
		m_cx = 10;
		m_cy = 4;
		dwResSize = pRes->Extract(IDB_IMG_INV, aData, TEMP_BUFFER);
		m_bkgnd.Load(aData, dwResSize);		
		m_rcClose = CRect(CPoint(21, 144), CSize(25, 25));
		break;

	case STO_STASH:
		m_ptOrigin.x = 73;
		m_ptOrigin.y = 83;
		m_cx = 6;
		m_cy = 8;
		dwResSize = pRes->Extract(IDB_IMG_STASH, aData, TEMP_BUFFER);
		m_bkgnd.Load(aData, dwResSize);	
		m_rcClose = CRect(CPoint(277, 390), CSize(25, 25));
		break;

	case STO_CUBE:
		m_ptOrigin.x = 116;
		m_ptOrigin.y = 140;
		m_cx = 3;
		m_cy = 4;
		dwResSize = pRes->Extract(IDB_IMG_CUBE, aData, TEMP_BUFFER);
		m_bkgnd.Load(aData, dwResSize);	
		m_rcClose = CRect(CPoint(278, 389), CSize(25, 25));
		break;

	case STO_CURSOR:
		m_ptOrigin.x = 5;
		m_ptOrigin.y = 5;
		m_cx = 1;
		m_cy = 1;		
		dwResSize = pRes->Extract(IDB_IMG_CURSOR, aData, TEMP_BUFFER);
		m_bkgnd.Load(aData, dwResSize);	
		break;

	case STO_MERC:
		m_cx = MAX_MERC;
		m_cy = 1;
		dwResSize = pRes->Extract(IDB_IMG_MERCBKGND, aData, TEMP_BUFFER);
		m_bkgnd.Load(aData, dwResSize);	
		m_rcClose = CRect(CPoint(278, 389), CSize(25, 25));
		break;

	default:
		m_cx = 0;
		m_cy = 0;
		break;
	}

	m_rcBoundary = CRect(CPoint(m_rcBoundary.left, m_rcBoundary.top), m_bkgnd.GetPictureSize());
}

const CD2ItemData* CD2Storage::GetItemAtPos(POINT pt, CRect& rect) const
{
	if (!m_bVisible)
		return NULL;

	pt.x -= m_rcBoundary.left;
	pt.y -= m_rcBoundary.top;

	for (int i = 0; i < m_items.GetSize(); i++)
	{
		if (m_items[i].bVisible && ::PtInRect(&m_items[i].rect, pt))
		{
			rect = m_items[i].rect;
			rect.OffsetRect(m_rcBoundary.left, m_rcBoundary.top);
			return m_items[i].pItem;
		}
	}

	return NULL;
}

BOOL CD2Storage::IsPtInStorage(POINT pt) const
{
	return IsVisible() && m_rcBoundary.PtInRect(pt);
}

void CD2Storage::DrawStorage(CDC *pDC)
{
	if (!m_bVisible || pDC == NULL)
		return;

	if (m_bkgnd.IsValid())
		m_bkgnd.Draw(pDC, m_rcBoundary.TopLeft());

	if (m_nType == STO_EQUIP)
	{
		if (m_bWs2)
		{
			CPoint pt = m_rcWs1.TopLeft();
			pt.Offset(-8, -3);
			m_ws2.Draw(pDC, pt);

			pt = m_rcWs2.TopLeft();
			pt.Offset(-8, -3);
			m_ws2.Draw(pDC, pt);
		}
	}

	static CRect rect;
	for (int i = 0; i < m_items.GetSize(); i++)
	{
		if (m_items[i].bVisible)
		{
			rect = m_items[i].rect;
			rect.OffsetRect(m_rcBoundary.left, m_rcBoundary.top);
			m_items[i].pItem->DrawItem(pDC, rect);
		}		
	}
}	

void CD2Storage::SetPosition(int x, int y)
{
	m_rcBoundary = CRect(CPoint(x, y), m_bkgnd.GetPictureSize());
	m_rcClose.OffsetRect(x, y);
	if (m_nType == STO_EQUIP)
	{
		m_rcWs1 = WST1;
		m_rcWs2 = WST2;
		m_rcWs1.OffsetRect(x, y);
		m_rcWs2.OffsetRect(x, y);
	}
}

CD2Storage::operator LPCRECT () const
{
	return (LPCRECT)m_rcBoundary;
}

void CD2Storage::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}

BOOL CD2Storage::IsVisible() const
{
	return m_bVisible;
}

const CRect& CD2Storage::GetBoundary() const
{
	return m_rcBoundary;
}

int CD2Storage::GetType() const
{
	return m_nType;
}

BOOL CD2Storage::FindItemByID(DWORD dwItemID, CRect &rect) const
{
	for (int i = 0; i < m_items.GetSize(); i++)
	{
		if (m_items[i].pItem->dwItemID == dwItemID)
		{
			rect = m_items[i].rect;
			rect.OffsetRect(m_rcBoundary.left, m_rcBoundary.top);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CD2Storage::IsCloseButton(POINT pt) const
{
	return m_bVisible && m_rcClose.PtInRect(pt);
}

int CD2Storage::IsPtOnWeaponSwitch(POINT pt) const
{
	if (m_nType != STO_EQUIP)
		return 0;

	return m_rcWs1.PtInRect(pt) || m_rcWs2.PtInRect(pt);
}

BOOL CD2Storage::LoadFromPlayer(CD2PlayerData *pPlayer)
{
	m_items.RemoveAll();

	if (pPlayer == NULL)
		return FALSE;

	m_bWs2 = m_nType == STO_EQUIP ? pPlayer->nActiveSwitch : FALSE;
	POSITION pos = pPlayer->m_aItemList.GetHeadPosition();
	while (pos)
	{
		CD2ItemData* p = pPlayer->m_aItemList.GetNext(pos);
		if (p && (int)p->dwStorageID == m_nType && p->x >= 0 && p->x < m_cx && p->y >= 0 && p->y < m_cy)
		{
			ITEMPOS ip;
			ip.bVisible = TRUE;
			if (m_nType == STO_EQUIP && (p->x == 11 || p->x == 12))
				ip.bVisible = FALSE;
			ip.pItem = p;
			CalcItemRect(p, ip.rect);

			if (m_nType == STO_CURSOR)
			{
				CRect rect = ip.rect;
				rect.OffsetRect((m_rcBoundary.Width() - rect.Width()) / 2, (m_rcBoundary.Height() - rect.Height()) / 2);
				ip.rect = rect;
			}

			m_items.Add(ip);
		}
	}

	return TRUE;
}

BOOL CD2Storage::CalcItemRect(const CD2ItemData *pItem, RECT &rect) const
{
	if (pItem == NULL || m_cx == 0 || m_cy == 0)
		return FALSE;

	if (m_nType == STO_EQUIP || m_nType == STO_MERC)
	{
		// equip
		if (pItem->x >= MAX_EQUIP)
			return FALSE;

		rect.left = EQUIPSLOTS[pItem->x].x;
		rect.right = rect.left + EQUIPSLOTS[pItem->x].cx * GRID_DIM;
		rect.top = EQUIPSLOTS[pItem->x].y;
		rect.bottom = rect.top + EQUIPSLOTS[pItem->x].cy * GRID_DIM;
	}
	else
	{
		rect.left = m_ptOrigin.x + pItem->x * GRID_DIM;
		rect.right = rect.left + pItem->cx * GRID_DIM;
		rect.top = m_ptOrigin.y + pItem->y * GRID_DIM;
		rect.bottom = rect.top + pItem->cy * GRID_DIM;
	}

	return TRUE;	
}

int CD2Storage::GetItemCount() const
{
	return m_items.GetSize();
}

void CD2Storage::WeaponSwitch()
{
	if (m_nType != STO_EQUIP)
		return;

	m_bWs2 = !m_bWs2;

	for (int i = 0; i < m_items.GetSize(); i++)
	{
		if (m_items[i].pItem->x == 4 || m_items[i].pItem->x == 5 || m_items[i].pItem->x == 11 || m_items[i].pItem->x == 12)
			m_items[i].bVisible = !m_items[i].bVisible;
	}
}
