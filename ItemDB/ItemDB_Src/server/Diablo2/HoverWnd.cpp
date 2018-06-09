// HoverWnd.cpp : implementation file
//

#include "stdafx.h"
#include "HoverWnd.h"
#include "D2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverWnd

CHoverWnd::CHoverWnd()
{
	m_pCurItem = NULL;
}

CHoverWnd::~CHoverWnd()
{
}


BEGIN_MESSAGE_MAP(CHoverWnd, CWnd)
	//{{AFX_MSG_MAP(CHoverWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHoverWnd message handlers

BOOL CHoverWnd::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!CWnd::Create(NULL, _T(""), WS_CHILD, CRect(0, 0, 200, 200), pParentWnd, 0, NULL))
		return FALSE;

	//SetFont(GetParent()->GetFont());
	return TRUE;
}

void CHoverWnd::HideHover()
{
	m_pCurItem = NULL;
	if (::IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);
}

void CHoverWnd::ShowHover(const CD2ItemData *pItem, const CRect &rect, const CRect &rcLimit)
{
	if (pItem == NULL || pItem == m_pCurItem || !::IsWindow(m_hWnd))
		return;

	m_pCurItem = pItem;
	CRect rc = rect;
	rc.left = rect.left + (rect.Width() - m_pCurItem->czHover.cx) / 2;
	rc.bottom = rect.top - 5;
	rc.right = rc.left + m_pCurItem->czHover.cx;
	rc.top = rc.bottom - m_pCurItem->czHover.cy;
	rc.InflateRect(2, 2);

	if (rc.right > rcLimit.right)
		rc.OffsetRect(rcLimit.right - rc.right, 0);	

	if (rc.left < rcLimit.left)
		rc.OffsetRect(rcLimit.left - rc.left, 0);

		if (rc.bottom > rcLimit.bottom)
		rc.OffsetRect(0, rcLimit.bottom - rc.bottom);

	if (rc.top < rcLimit.top)
		rc.OffsetRect(0, rcLimit.top - rc.top);	
	

	MoveWindow(&rc, TRUE);	
	ShowWindow(SW_SHOW);
	RedrawWindow();
}

void CHoverWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(&rcClient, RGB(0, 0, 0));
	//dc.Draw3dRect(&rcClient, RGB(148, 128, 100), RGB(148, 128, 100));

	if (m_pCurItem == NULL)
		return;

	dc.SetBkMode(TRANSPARENT);
	const cx = rcClient.Width();
	int y = 0;
	int i = 0;

	dc.SetTextColor(m_pCurItem->GetDescColor());
	for (i = 0; i < m_pCurItem->m_aTitle.GetSize(); i++)
		DrawText(m_pCurItem->m_aTitle[i], y, &dc, cx);

	//if (m_pCurItem->bRuneword)
	//	dc.SetTextColor(RGB(80, 80, 80));
	//DrawText(m_pCurItem->sBaseName, y, &dc, cx);

	if (!m_pCurItem->bIdentified)
	{
		dc.SetTextColor(RGB(176, 68, 52));
		DrawText(g_bLanguage ? "Î´¼ø¶¨µÄ" : "Unidentified", y, &dc, cx);
		return;
	}
	
	dc.SetTextColor(RGB(196, 196, 196));
	for (i = 0; i < m_pCurItem->m_aHead.GetSize(); i++)
		DrawText(m_pCurItem->m_aHead[i], y, &dc, cx);

	dc.SetTextColor(RGB(80, 80, 172));
	for (i = 0; i < m_pCurItem->m_aMods.GetSize(); i++)
		DrawText(m_pCurItem->m_aMods[i], y, &dc, cx);

	for (i = 0; i < m_pCurItem->m_aFoot.GetSize(); i++)
		DrawText(m_pCurItem->m_aFoot[i], y, &dc, cx);
}

void CHoverWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CWnd::OnMouseMove(nFlags, point);
	CD2View* pWnd = (CD2View*)GetParent();
	if (pWnd == NULL)
		return;

	ClientToScreen(&point);
	pWnd->ScreenToClient(&point);
	pWnd->CheckMouseMove(point);
}

void CHoverWnd::DrawText(const CString &str, int &y, CDC *pDC, int cx)
{
	if (str.IsEmpty() || pDC == NULL)
		return;

	CSize cz = pDC->GetTextExtent(str);
	int x = (cx - cz.cx) / 2;
	pDC->TextOut(x, y, str);
	y += cz.cy + LINE_GAP;
}

BOOL CHoverWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_KEYDOWN)
		ShowWindow(SW_HIDE);
	return CWnd::PreTranslateMessage(pMsg);
}
