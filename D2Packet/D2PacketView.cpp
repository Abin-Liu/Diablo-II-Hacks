// D2PacketView.cpp : implementation of the CD2PacketView class
//

#include "stdafx.h"
#include "D2Packet.h"

#include "D2PacketDoc.h"
#include "D2PacketView.h"

#include "Clipboard.h"
#include "FilterDlg.h"
#include "ContainDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { COL_DUMMY = 0, COL_NO, COL_LEN, COL_RS, COL_PACKET };

const LPCTSTR g_lppszChar[] = { _T("Amazon"), _T("Sorceress"), _T("Necromancer"), _T("Paladin"), _T("Barbarian"), _T("Druid"), _T("Assassin") };


/////////////////////////////////////////////////////////////////////////////
// CD2PacketView

IMPLEMENT_DYNCREATE(CD2PacketView, CFormView)

BEGIN_MESSAGE_MAP(CD2PacketView, CFormView)
	//{{AFX_MSG_MAP(CD2PacketView)
	ON_WM_SIZE()
	ON_COMMAND(ID_CLEAR, OnClear)
	ON_COMMAND(ID_FILTER, OnFilter)
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, OnUpdatePause)
	ON_COMMAND(ID_POPUP_BLOCK, OnPopupBlock)
	ON_UPDATE_COMMAND_UI(ID_POPUP_BLOCK, OnUpdatePopupBlock)
	ON_COMMAND(ID_POPUP_CLIPBOARD, OnPopupClipboard)
	ON_UPDATE_COMMAND_UI(ID_POPUP_CLIPBOARD, OnUpdatePopupClipboard)
	ON_COMMAND(ID_RCV_ALLOWALL, OnRcvAllowall)
	ON_COMMAND(ID_RCV_BLOCKALL, OnRcvBlockall)
	ON_COMMAND(ID_RESUME, OnResume)
	ON_UPDATE_COMMAND_UI(ID_RESUME, OnUpdateResume)
	ON_COMMAND(ID_SNT_ALLOWALL, OnSntAllowall)
	ON_COMMAND(ID_SNT_BLOCKALL, OnSntBlockall)
	ON_COMMAND(ID_RCV_CUSTOM, OnRcvCustom)
	ON_COMMAND(ID_SNT_CUSTOM, OnSntCustom)
	ON_COMMAND(ID_DEBUG, OnDebug)
	ON_UPDATE_COMMAND_UI(ID_DEBUG, OnUpdateDebug)
	ON_COMMAND(ID_PLAYERINFO, OnPlayerinfo)
	ON_UPDATE_COMMAND_UI(ID_PLAYERINFO, OnUpdatePlayerinfo)
	ON_COMMAND(ID_SEARCH, OnSearch)
	ON_UPDATE_COMMAND_UI(ID_SEARCH, OnUpdateSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2PacketView construction/destruction

CD2PacketView::CD2PacketView()
	: CFormView(CD2PacketView::IDD)
{
	//{{AFX_DATA_INIT(CD2PacketView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_bPaused = FALSE;
	m_bInitialized = FALSE;
	m_bDebug = FALSE;
}

CD2PacketView::~CD2PacketView()
{
}

void CD2PacketView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD2PacketView)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}

BOOL CD2PacketView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CD2PacketView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	if (!IsRelayerValid())
		MessageBox(_T("The packet relayer was not properly created!"), _T("Error"), MB_ICONEXCLAMATION);
	
	if (!m_bInitialized)
	{
		m_bInitialized = TRUE;
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();
		
		m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		m_wndList.InsertColumn(COL_DUMMY, _T(""), LVCFMT_LEFT, 0);
		m_wndList.InsertColumn(COL_NO, _T("No"), LVCFMT_RIGHT, 50);
		m_wndList.InsertColumn(COL_LEN, _T("Len"), LVCFMT_RIGHT, 45);
		m_wndList.InsertColumn(COL_RS, _T("S/R"), LVCFMT_CENTER, 35);
		m_wndList.InsertColumn(COL_PACKET, _T("Packet"), LVCFMT_LEFT, 800);
	}
	else
	{
		m_wndList.DeleteAllItems();
	}
	//GetDocument()->GetFilter()->BlockAll(0);
}

/////////////////////////////////////////////////////////////////////////////
// CD2PacketView diagnostics

#ifdef _DEBUG
void CD2PacketView::AssertValid() const
{
	CFormView::AssertValid();
}

void CD2PacketView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CD2PacketDoc* CD2PacketView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CD2PacketDoc)));
	return (CD2PacketDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CD2PacketView message handlers

void CD2PacketView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (nType != SIZE_MINIMIZED && m_wndList.GetSafeHwnd())
		m_wndList.MoveWindow(0, 0, cx, cy);
}

void CD2PacketView::OnGameJoin(LPCPLAYERINFO lpPlayer)
{
	int n = m_wndList.InsertItem(INT_MAX, _T("Join"));
	m_wndList.SetItemText(n, COL_NO, _T("Join"));
	m_wndList.SetItemText(n, COL_PACKET, _T("==============================================================="));
}

void CD2PacketView::OnGameLeave()
{
	int n = m_wndList.InsertItem(INT_MAX, _T("Leave"));
	m_wndList.SetItemText(n, COL_NO, _T("Leave"));
	m_wndList.SetItemText(n, COL_PACKET, _T("==============================================================="));
}

void CD2PacketView::OnGamePacketBeforeReceived(const BYTE *aPacket, DWORD aLen)
{
	// TODO: Add your implementation here	
	//if (aPacket[0] == 0xa8 && (aPacket[7] == 32 || aPacket[7] == 149 || aPacket[7] == 139 || aPacket[7] == 140))
		PrintPacket(aPacket, aLen, TRUE);

	//if (aPacket[0] == 0xa9 && aPacket[6] == 149)
	//	PrintPacket(aPacket, aLen, TRUE);
}

void CD2PacketView::OnGamePacketBeforeSent(const BYTE *aPacket, DWORD aLen)
{
	PrintPacket(aPacket, aLen, FALSE);
}

void CD2PacketView::PrintPacket(const BYTE *aPacket, DWORD aLen, BOOL bReceived)
{
	if (aPacket == NULL || aLen == 0)
		return;

	if (m_bPaused)
		return;

	CString sNo, sPacket, sLen;
	if (!GetDocument()->IsPacketOK(aPacket, aLen, bReceived))
	{
		if (m_bDebug)
		{
			sNo.Format(_T("%5u"), GetGamePacketCount());
			sLen.Format(_T("%5d"), aLen);
			sPacket.Format(_T("%02X ***"), aPacket[0]);
		}
	}
	else
	{
		sNo.Format(_T("%5u"), GetGamePacketCount());
		sLen.Format(_T("%5d"), aLen);
		CString sByte;
		for (DWORD i = 0; i < aLen; i++)
		{
			sByte.Format(_T("%02X "), aPacket[i]);
			sPacket += sByte;
		}

		sPacket.TrimRight();
	}	
	
	while (m_wndList.GetItemCount() > 2000)
		m_wndList.DeleteItem(0);
	
	if (!sPacket.IsEmpty())
	{
		int nIdx = m_wndList.InsertItem(INT_MAX, _T(""));
		m_wndList.SetItemText(nIdx, COL_NO, sNo);
		m_wndList.SetItemText(nIdx, COL_LEN, sLen);
		m_wndList.SetItemText(nIdx, COL_RS, bReceived ? _T("R") : _T("S"));
		m_wndList.SetItemText(nIdx, COL_PACKET, sPacket);
		m_wndList.EnsureVisible(nIdx, FALSE);
	}
}

void CD2PacketView::OnClear() 
{
	// TODO: Add your command handler code here
	m_wndList.DeleteAllItems();
}

void CD2PacketView::OnFilter() 
{
	// TODO: Add your command handler code here
	CToolBar* pWnd = ((CMainFrame*)GetParent())->GetToolbar();
	ASSERT(pWnd);
	CRect rc;
	pWnd->GetItemRect(pWnd->CommandToIndex(ID_FILTER), &rc);
	pWnd->ClientToScreen(&rc);

	// load and display popup menu
	CMenu menu;
	menu.LoadMenu(IDR_POPUP);
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup);    
	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, this, &rc);
}

void CD2PacketView::OnPause() 
{
	// TODO: Add your command handler code here
	m_bPaused = TRUE;
}

void CD2PacketView::OnUpdatePause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bPaused);
}

void CD2PacketView::OnPopupBlock() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int n = m_wndList.GetNextSelectedItem(pos);
		CString s = m_wndList.GetItemText(n, COL_RS);
		CString sPacket = m_wndList.GetItemText(n, COL_PACKET);
		BYTE i = (BYTE)_tcstoul(sPacket.Left(2), NULL, 16);
		GetDocument()->GetFilter()->Block(i, s.Find(_T('R')) != -1);
		GetDocument()->SetModifiedFlag();
	}

	UpdateList();
}

void CD2PacketView::OnUpdatePopupBlock(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndList.GetSelectedCount() > 0);
}

void CD2PacketView::OnPopupClipboard() 
{
	// TODO: Add your command handler code here
	CString strAll;
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int n = m_wndList.GetNextSelectedItem(pos);
		CString str;
		str.Format(_T("%s | %s | %s\r\n"), m_wndList.GetItemText(n, COL_LEN), m_wndList.GetItemText(n, COL_RS), m_wndList.GetItemText(n, COL_PACKET));
		strAll += str;		
	}

	CClipboard::SetText(strAll);
}

void CD2PacketView::OnUpdatePopupClipboard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndList.GetSelectedCount() > 0);
}

void CD2PacketView::OnRcvAllowall() 
{
	// TODO: Add your command handler code here
	GetDocument()->GetFilter()->AllowAll(TRUE);
	GetDocument()->SetModifiedFlag();
}

void CD2PacketView::OnRcvBlockall() 
{
	// TODO: Add your command handler code here
	GetDocument()->GetFilter()->BlockAll(TRUE);
	GetDocument()->SetModifiedFlag();
	UpdateList();
}

void CD2PacketView::OnResume() 
{
	// TODO: Add your command handler code here
	m_bPaused = FALSE;
}

void CD2PacketView::OnUpdateResume(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bPaused);
}

void CD2PacketView::OnSntAllowall() 
{
	// TODO: Add your command handler code here
	GetDocument()->GetFilter()->AllowAll(FALSE);
	GetDocument()->SetModifiedFlag();
}

void CD2PacketView::OnSntBlockall() 
{
	// TODO: Add your command handler code here
	GetDocument()->GetFilter()->BlockAll(FALSE);
	GetDocument()->SetModifiedFlag();
	UpdateList();
}

void CD2PacketView::OnRcvCustom() 
{
	// TODO: Add your command handler code here
	CFilterDlg dlg(TRUE, GetDocument()->GetFilter());
	if (dlg.DoModal() == IDOK)
	{
		GetDocument()->SetModifiedFlag();
		UpdateList();
	}
}

void CD2PacketView::OnSntCustom() 
{
	// TODO: Add your command handler code here
	CFilterDlg dlg(FALSE, GetDocument()->GetFilter());
	if (dlg.DoModal() == IDOK)
	{
		GetDocument()->SetModifiedFlag();
		UpdateList();
	}
}

void CD2PacketView::OnDebug() 
{
	// TODO: Add your command handler code here
	m_bDebug = !m_bDebug;
	if (m_bDebug)
		MessageBox("Debug mode enabled.");
	else
		MessageBox("Debug mode disabled.");
}

void CD2PacketView::OnUpdateDebug(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bDebug);
}

void CD2PacketView::OnPlayerinfo() 
{
	// TODO: Add your command handler code here
	LPCPLAYERINFO p = GetPlayer();
	if (p->dwPlayerID)
	{
		CString str;
		str.Format(_T("Player information:\n\nPlayer ID: 0x%08X\nClass: %s\nName: %s"),
			p->dwPlayerID, g_lppszChar[p->iCharClass], p->szPlayerName);
		MessageBox(str);
	}	
}

void CD2PacketView::OnUpdatePlayerinfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetPlayer()->dwPlayerID);
}

void CD2PacketView::OnSearch() 
{
	// TODO: Add your command handler code here
	CContainDlg dlg(m_sSearch);
	if (dlg.DoModal() != IDOK)
		return;

	m_sSearch = dlg.GetString();
	CString str;
	for (int i = m_wndList.GetItemCount() - 1; i >= 0; i--)
	{
		str = m_wndList.GetItemText(i, COL_PACKET);
		if (str.Find(m_sSearch) == -1)
			m_wndList.DeleteItem(i);
	}
}

void CD2PacketView::OnUpdateSearch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_wndList.GetItemCount() > 0);
}

void CD2PacketView::UpdateList()
{
	m_wndList.LockWindowUpdate();
	for (int i = m_wndList.GetItemCount() - 1; i >= 0; i--)
	{
		BOOL bReceived = m_wndList.GetItemText(i, COL_RS).Find(_T('R')) != -1;
		BYTE n = (BYTE)_tcstoul(m_wndList.GetItemText(i, COL_PACKET).Left(2), NULL, 16);
		if (!GetDocument()->GetFilter()->IsAllowed(n, bReceived))
			m_wndList.DeleteItem(i);
	}
	m_wndList.UnlockWindowUpdate();
}
