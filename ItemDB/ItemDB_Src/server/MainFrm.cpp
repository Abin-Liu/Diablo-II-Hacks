// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ItemDB.h"
#include "MainFrm.h"
#include "External\\PopupMenu.h"
#include "InternalMsgs.h"
#include "CharList.h"
#include "External\\ProgressWnd.h"
#include "External\\RemoteLib.h"
#include "RealmView.h"
#include "CharView.h"
#include "ItemDBView.h"
#include "ItemListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_CACHE		32
#define CLIENT_FILE			"ItemDB.dll"
#define DATABASE_FILE		"ItemDB.dat"
enum { SHOWTYPE_D2 = 0, SHOWTYPE_LIST, SHOWTYPE_SQL };

#define DEF_CHS	"<关键词或属性表达式>"
#define DEF_ENG	"<Keywords or mod expression>"
static const UINT INDICATORS[] = { ID_INDC_PROMPT, ID_INDC_OTHER };

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_POPUP_RESTORE, OnPopupRestore)
	ON_UPDATE_COMMAND_UI(ID_POPUP_RESTORE, OnUpdatePopupRestore)
	ON_COMMAND(ID_D2VIEW, OnD2view)
	ON_UPDATE_COMMAND_UI(ID_D2VIEW, OnUpdateD2view)
	ON_COMMAND(ID_ITEMVIEW, OnItemview)
	ON_UPDATE_COMMAND_UI(ID_ITEMVIEW, OnUpdateItemview)
	ON_COMMAND(ID_SQLVIEW, OnSqlview)
	ON_UPDATE_COMMAND_UI(ID_SQLVIEW, OnUpdateSqlview)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_COMMAND(ID_START_SEARCH, OnStartSearch)
	ON_UPDATE_COMMAND_UI(ID_START_SEARCH, OnUpdateStartSearch)
	ON_COMMAND(ID_BYKEYWORDS, OnBykeywords)
	ON_UPDATE_COMMAND_UI(ID_BYKEYWORDS, OnUpdateBykeywords)
	ON_COMMAND(ID_BYEXP, OnByexp)
	ON_UPDATE_COMMAND_UI(ID_BYEXP, OnUpdateByexp)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_INDC_PROMPT, OnUpdatePane)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_UPDATE_COMMAND_UI(ID_DELETE, OnUpdateDelete)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_LANG_ENG, OnLangEng)
	ON_UPDATE_COMMAND_UI(ID_LANG_ENG, OnUpdateLangEng)
	ON_COMMAND(ID_LANG_CHS, OnLangChs)
	ON_UPDATE_COMMAND_UI(ID_LANG_CHS, OnUpdateLangChs)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_COMMAND(ID_HELPHELP, OnHelphelp)
	ON_UPDATE_COMMAND_UI(ID_INDC_OTHER, OnUpdatePane)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAY_NOTIFY, OnTrayNotify)
	ON_MESSAGE(WM_STATUS_TEXT, OnStatusText)
	ON_MESSAGE(WM_SET_REALMVIEW, OnSetRealmView)
	ON_MESSAGE(WM_SET_CHARVIEW, OnSetCharView)
	ON_MESSAGE(WM_SET_ITEMVIEW, OnSetItemView)
	ON_MESSAGE(WM_SET_LISTVIEW, OnSetListView)
	ON_MESSAGE(WM_LIST_CHARS, OnListChars)
	ON_MESSAGE(WM_LIST_ITEMS, OnListItems)
	ON_MESSAGE(WM_NAVIGATE_TO, OnNavigateTo)
	ON_MESSAGE(WM_START_SEARCH, OnSearchIt)
	ON_MESSAGE(WM_GAME_WND, OnGameWnd)
	ON_MESSAGE(WM_ITEMDB_CLIENT_UPDATE, OnClientUpdate)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pwndRealms = NULL;
	m_pwndChars = NULL;
	m_pwndD2 = NULL;
	m_pwndItems = NULL;
	m_pwndSQL = NULL;

	m_ilChars.Create(IDB_CHARS, 32, 15, RGB(255, 255, 255));
	m_bUseExp = AfxGetApp()->GetProfileInt("", "Exp Match", 0);
	m_bIsSearching = FALSE;
	m_bMaximized = FALSE;
	m_nType = SHOWTYPE_D2;
}

CMainFrame::~CMainFrame()
{
	m_ilChars.DeleteImageList();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_menu.LoadMenu(g_bLanguage ? IDR_MAINFRAME_CHS : IDR_MAINFRAME_ENG))
	{
		CMenu* pMenu = GetMenu();
		if (pMenu)
		{
			pMenu->DestroyMenu();
			SetMenu(&m_menu);
			m_hMenuDefault = m_menu.m_hMenu;
		}
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
		|| !m_wndToolBar.CreateEditCtrl()
		|| !m_wndToolBar.AddArrow(ID_START_SEARCH))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.m_wndEdit.SetWindowText(g_bLanguage ? DEF_CHS : DEF_ENG);
	m_wndToolBar.m_wndChk.EnableWindow(!m_bUseExp);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(INDICATORS, 2))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(0, ID_INDC_PROMPT, SBPS_NORMAL, 600);      
	m_wndStatusBar.SetPaneInfo(1, ID_INDC_OTHER, SBPS_STRETCH ,0);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	CItemDBApp* pApp = (CItemDBApp*)AfxGetApp();
	pApp->SetPrevWnd(m_hWnd);
	SetWindowText(pApp->GetAppTitle());

	HICON hic1 = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hic2 = AfxGetApp()->LoadIcon(IDI_BLANK);
	m_tray.Create(this, WM_TRAY_NOTIFY, pApp->GetAppTitle(), hic1, IDR_MAINFRAME);
	HICON aAnim[2] = { hic1, hic2 };
	m_tray.SetIconList(aAnim, 2);	

	if (!m_db.OpenDatabase(AfxGetAppEx()->GetAppDir() + "\\" + DATABASE_FILE))
	{
		CString sErr;
		sErr.Format("Unable to open database \"%s\".", DATABASE_FILE);
		MessageBox(sErr, "Database Error", MB_ICONSTOP | MB_OK);
		return -1;
	}

	if (!m_msgWnd.Create(m_hWnd, ITEMDB_WND_SERVER, AfxGetAppEx()->GetAppDir()))
	{
		MessageBox("Failed to create the server window!", "Initialization Failure", MB_ICONSTOP);
		return -1;
	}

	OnStatusText(0, (LPARAM)(g_bLanguage ? "移动鼠标查看物品细节" : "Hover cursor to view item details"));
	OnStatusText(1, (LPARAM)"");
	
	m_monitor.Start(m_hWnd, WM_GAME_WND, 1500);

	if (AfxGetApp()->m_lpCmdLine && strstr(AfxGetApp()->m_lpCmdLine, "-minimize"))
		m_tray.MinimiseToTray(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

LRESULT CMainFrame::OnSetRealmView(WPARAM wParam, LPARAM lParam)
{
	m_pwndRealms = (CTreeCtrl*)lParam;
	if (m_pwndRealms)
	{
		m_wndToolBar.m_wndEdit.SetFont(m_pwndRealms->GetFont());
		m_wndToolBar.m_wndChk.SetFont(m_pwndRealms->GetFont());
	}

	ListRealms();
	return 0;
}

LRESULT CMainFrame::OnSetCharView(WPARAM wParam, LPARAM lParam)
{
	m_pwndChars = (CCharList*)lParam;
	if (m_pwndChars)
		m_pwndChars->SetImageList(&m_ilChars, TVSIL_NORMAL);
	return 0;
}

LRESULT CMainFrame::OnSetItemView(WPARAM wParam, LPARAM lParam)
{
	m_pwndD2 = (CD2View*)lParam;
	return 0;
}

LRESULT CMainFrame::OnSetListView(WPARAM wParam, LPARAM lParam)
{
	m_pwndItems = (CReportCtrl*)wParam;
	m_pwndSQL = (CReportCtrl*)lParam;
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class	
	//return CFrameWnd::OnCreateClient(lpcs, pContext);

	if (!m_wndSplitterV.CreateStatic(this, 1, 2))
		return FALSE;
	
	if (!m_wndSplitterH.CreateStatic(&m_wndSplitterV, 3, 1, WS_CHILD | WS_VISIBLE, m_wndSplitterV.IdFromRowCol(0, 1)))
		return FALSE;
	
	m_wndSplitterV.CreateView(0,0,RUNTIME_CLASS(CRealmView), CSize(150, 750), pContext);
	m_wndSplitterH.CreateView(0,0,RUNTIME_CLASS(CCharView), CSize(700, 70), pContext);
	m_wndSplitterH.CreateView(1,0,RUNTIME_CLASS(CItemDBView), CSize(700,700), pContext);
	m_wndSplitterH.CreateView(2,0,RUNTIME_CLASS(CItemListView), CSize(0,0), pContext);
	m_wndSplitterH.SetRowInfo(2, 0, 0);
	m_wndSplitterH.RecalcLayout();
	return TRUE;
}

LRESULT CMainFrame::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN)
	{
		OnPopupRestore();
	}
	else if (lParam == WM_RBUTTONUP)
	{
		CMenu menu;
		CMenu* popUp;
		POINT point;

		::GetCursorPos(&point);
		if (!menu.LoadMenu(g_bLanguage ? IDR_POPUP_CHS : IDR_POPUP_ENG))
			return 0;
	
		popUp = menu.GetSubMenu(0);
		popUp->SetDefaultItem(ID_POPUP_RESTORE);

		if (popUp)
		{
			HWND hWnd = this->GetSafeHwnd();
			::SetForegroundWindow(hWnd);
			PopupMenu(popUp, this, &point);
			::PostMessage(hWnd, WM_NULL, 0, 0);
		}
	}

	return 0;
}

LRESULT CMainFrame::OnStatusText(WPARAM wParam, LPARAM lParam)
{
	LPCSTR lpsz = lParam ? (LPCSTR)lParam : "";
	CString str = m_wndStatusBar.GetPaneText(wParam);
	if (str.Compare(lpsz))
		m_wndStatusBar.SetPaneText(wParam, lpsz);
	return 0;
}

void CMainFrame::OnPopupRestore() 
{
	// TODO: Add your command handler code here
	if (IsWindowVisible())
	{
		m_bMaximized = IsZoomed();
		m_tray.MinimiseToTray(this);
	}
	else
	{
		m_tray.MaximiseFromTray(this);
		if (m_bMaximized)
			ShowWindow(SW_MAXIMIZE);
	}
}

void CMainFrame::OnUpdatePopupRestore(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CString sText;
	if (IsWindowVisible())
		sText = g_bLanguage ? "隐藏(M)" : "&Minimize";
	else
		sText = g_bLanguage ? "恢复(R)" : "&Restore";

	pCmdUI->SetText(sText);
}

void CMainFrame::ListRealms()
{
	if (!VerifyViews())
		return;

	m_pwndRealms->DeleteAllItems();
	
	HTREEITEM h = NULL;
	DWORD dwID;
	CString sTemp;

	CADORecordset rs(m_db), rs1(m_db);
	if (!m_db.Query(&rs, "select * from [realm] order by rid"))
		return;

	while (!rs.IsEof())
	{
		rs.GetFieldValue("rid", dwID);
		rs.GetFieldValue("rname", sTemp);
		if (dwID == 1)
			sTemp = g_bLanguage ? "<单机国度>" : "<Single-Player Realm>";
		h = m_pwndRealms->InsertItem(sTemp, 0, 0);
		m_pwndRealms->SetItemData(h, dwID);		
		ListAccounts(h, dwID);
		rs.MoveNext();
	}
}

void CMainFrame::ListAccounts(HTREEITEM hRealm, DWORD dwRealmID)
{
	if (!VerifyViews())
		return;	

	HTREEITEM h = NULL;
	DWORD dwID;
	CString sTemp;

	sTemp.Format("select * from [account] where realmid=%d order by aid", dwRealmID);
	CADORecordset rs(m_db);
	if (!m_db.Query(&rs, "select * from [account] where realmid=%d order by aid", dwRealmID))
		return;

	while (!rs.IsEof())
	{
		rs.GetFieldValue("aid", dwID);
		rs.GetFieldValue("aname", sTemp);
		if (dwID == 1)
			sTemp = g_bLanguage ? "<单机账号>" : "<Single-Player Account>";
		h = m_pwndRealms->InsertItem(sTemp, 1, 1, hRealm);
		m_pwndRealms->SetItemData(h, dwID);		
		rs.MoveNext();
	}
}

LRESULT CMainFrame::OnListChars(WPARAM wParam, LPARAM lParam)
{
	if (!VerifyViews())
		return 0;

	m_pwndD2->SetPlayer(NULL);
	m_pwndChars->DeleteAllItems();
	if (lParam == 0)
		return 0;	

	CADORecordset rs(m_db);
	if (!m_db.Query(&rs, "select * from [player] where accountid=%d order by pid", lParam))
		return 0;
	
	DWORD dwID;
	BOOL bHardcore;
	DWORD dwClass;
	CString str;
	while (!rs.IsEof())
	{
		rs.GetFieldValue("pid", dwID);
		rs.GetFieldValue("pname", str);
		rs.GetFieldValue("classid", dwClass);
		rs.GetFieldValue("phardcore", bHardcore);
		
		int nIdx = m_pwndChars->InsertItem(INT_MAX, str, dwClass);
		if (bHardcore)
			dwID |= HC_FLAG;
		m_pwndChars->SetItemData(nIdx, dwID);
		rs.MoveNext();
	}
	
	return 0;
}

void CMainFrame::OnD2view() 
{
	// TODO: Add your command handler code here	
	SetType(SHOWTYPE_D2);
}

void CMainFrame::OnUpdateD2view(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (VerifyViews())
		pCmdUI->SetCheck(m_nType == SHOWTYPE_D2);
}

void CMainFrame::OnItemview() 
{
	// TODO: Add your command handler code here		
	SetType(SHOWTYPE_LIST);
}

void CMainFrame::OnUpdateItemview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (VerifyViews())
		pCmdUI->SetCheck(m_nType == SHOWTYPE_LIST);
}

void CMainFrame::OnSqlview() 
{
	// TODO: Add your command handler code here
	SetType(SHOWTYPE_SQL);	
}

void CMainFrame::OnUpdateSqlview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (VerifyViews())
		pCmdUI->SetCheck(m_nType == SHOWTYPE_SQL);
}

LRESULT CMainFrame::OnListItems(WPARAM wParam, LPARAM lParam)
{
	ListItems(lParam);
	return 0;
}

CD2PlayerData* CMainFrame::FindCachedPlayer(DWORD dwPlayerID)
{
	CD2PlayerData* pPlayer = NULL;
	m_aCache.Lock();
	POSITION pos = m_aCache.GetHeadPosition();
	while (pos)
	{
		CD2PlayerData* p = m_aCache.GetNext(pos);
		if (p->dwPlayerID == dwPlayerID)
		{
			pPlayer = p;
			break;
		}
	}
	m_aCache.Unlock();
	return pPlayer;
}

CD2PlayerData* CMainFrame::CachePlayerData(DWORD dwPlayerID)
{
	if (!VerifyViews())
		return 0;

	CD2PlayerData* pPlayer = FindCachedPlayer(dwPlayerID);
	if (pPlayer)
		return pPlayer;	

	m_db.Lock();
	CADORecordset rs(m_db);
	if (!m_db.Query(&rs, "select * from [listchar] where pid=%d", dwPlayerID))
	{
		m_db.Unlock();
		return 0;
	}

	RemoveFirstCache(m_pwndD2->GetPlayerID());
	pPlayer = new CD2PlayerData;
	ASSERT(pPlayer);
	
	pPlayer->FromDatabase(m_db, &rs, m_db.GetModLib(), m_pwndD2->GetResPack());
	m_db.Unlock();

	POSITION pos = pPlayer->m_aItemList.GetHeadPosition();
	while (pos)
	{
		CD2ItemData* pItem = pPlayer->m_aItemList.GetNext(pos);
		pItem->CalcHoverSize(m_pwndD2->GetHoverWnd());
	}
	
	m_aCache.Lock();
	m_aCache.InsertRear(pPlayer);
	m_aCache.Unlock();
	return pPlayer;
}

void CMainFrame::RemoveFirstCache(DWORD dwCurPlayerID)
{
	m_aCache.Lock();
	if (m_aCache.GetItemCount() >= MAX_CACHE)
	{
		POSITION pos = m_aCache.GetHeadPosition();
		while (pos)
		{
			POSITION cur = pos;
			CD2PlayerData* p = m_aCache.GetNext(pos);
			if (dwCurPlayerID != p->dwPlayerID)
			{
				m_aCache.RemoveAt(cur);
				m_aCache.Unlock();
				return;
			}
		}
	}
	m_aCache.Unlock();
}

LRESULT CMainFrame::OnNavigateTo(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 0 || !VerifyViews())
		return 0;

	DWORD dwRealmID, dwAccountID, dwPlayerID;
	if (!m_db.GetAccountPlayerFromItem(lParam, dwRealmID, dwAccountID, dwPlayerID))
		return 0;

	CD2PlayerData* pPlayer = CachePlayerData(dwPlayerID);
	if (pPlayer == NULL)
		return 0;

	HTREEITEM h = FindTreeItem(TVI_ROOT, dwRealmID);
	if (h == NULL)
		return 0;

	m_pwndRealms->Expand(h, TVE_EXPAND);

	h = FindTreeItem(h, dwAccountID);
	if (h == NULL)
		return 0;

	m_pwndRealms->SelectItem(h);
	m_pwndRealms->EnsureVisible(h);

	int nIdx = m_pwndChars->FindCharFromList(dwPlayerID);
	if (nIdx == -1)
		return 0;
	
	SetType(SHOWTYPE_D2);
	m_pwndChars->SetItemState(nIdx, LVIS_SELECTED, LVIS_SELECTED);
	m_pwndChars->EnsureVisible(nIdx, FALSE);

	CRect rect;
	int nSto = m_pwndD2->FindItemByID(lParam, rect);
	if (nSto == -1)
		return 0;
	
	if (nSto == STO_CUBE)
	{
		m_pwndD2->OpenCube();
	}

	if (nSto == STO_MERC)
	{
		m_pwndD2->OpenMerc();
	}

	CPoint pt = rect.CenterPoint();
	POINT pt1 = pt;
	m_pwndD2->ClientToScreen(&pt1);
	::SetCursorPos(pt1.x, pt1.y);
	m_pwndD2->CheckMouseMove(pt);	
	return 0;
}

void CMainFrame::OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	CWnd *pWnd;
	UINT nID;

	// Switch on button command id''s.
	switch (pnmtb->iItem)
	{
	case ID_START_SEARCH:
		pWnd = &m_wndToolBar;
		nID= g_bLanguage ? IDR_SEARCH_CHS : IDR_SEARCH_ENG;
		break;
		
	default:
		return;
	}

	// load and display popup menu
	CMenu menu;
	menu.LoadMenu(nID);
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup);

	CRect rc;
	pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);

	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
	rc.left, rc.bottom, this, &rc);
}

void CMainFrame::NormalizeSearchStr(CString &str)
{
	CString sDef = g_bLanguage ? DEF_CHS : DEF_ENG;
	sDef.MakeLower();
	str.MakeLower();
	str.Replace(sDef, "");
	str.TrimLeft();
	str.TrimRight();

	// we make sure there's no double spaces between 2 strings unless the user
	// wanna be funny and inserted 2^10 (1024) of them, which is not likely
	for (int i = 0; i < 10; i++)
		str.Replace("  ", " ");	
}

void CMainFrame::OnStartSearch() 
{
	// TODO: Add your command handler code here
	if (m_bIsSearching || !VerifyViews())
		return;

	m_bIsSearching = TRUE;

	CString sInput;
	m_wndToolBar.m_wndEdit.GetWindowText(sInput);

	NormalizeSearchStr(sInput);	
	
	if (sInput.IsEmpty())
	{
		if (g_bLanguage)
			MessageBox("你必须提供一个字符串以开始查询！", "无效字符串", MB_ICONEXCLAMATION);
		else
			MessageBox("You must specify a string for starting a search!.", "Invalid String", MB_ICONEXCLAMATION);
		
		m_wndToolBar.m_wndEdit.SetWindowText(g_bLanguage ? DEF_CHS : DEF_ENG);
		m_bIsSearching = FALSE;
		m_wndToolBar.m_wndEdit.SetFocus();
		m_wndToolBar.m_wndEdit.Highlight();
		return;
	}
	
	// database won't take single quotes
	CStringEx se = sInput;
	se.Replace('\'', '^');

	if (m_bUseExp)
	{
		BOOL bSyntaxError = FALSE;
		m_db.ExpSearch(se, m_pwndSQL, bSyntaxError);
		if (bSyntaxError)
		{
			CString sErr;
			sErr.Format(g_bLanguage ? "\"%s\"\n不是一个合法的属性表达式！" : "\"%s\"\nIs not a valid mod expression!", (LPCSTR)sInput);
			MessageBox(sErr, "Invalid Expression", MB_ICONEXCLAMATION);			
		}
	}
	else
	{
		if (m_wndToolBar.m_wndChk.GetCheck())
		{
			CStringEx sSeg;
			BOOL bContinue = se.FindToken(" \t\r\n");
			while (bContinue)
			{
				bContinue = se.FindNextToken(sSeg);		
				m_db.KeywordSearch(sSeg, m_pwndSQL);
			}
		}
		else
		{
			m_db.KeywordSearch(se, m_pwndSQL);
		}
	}

	m_bIsSearching = FALSE;	
	m_wndToolBar.m_wndEdit.SetFocus();
	m_wndToolBar.m_wndEdit.Highlight();	
	SetType(SHOWTYPE_SQL);
}

void CMainFrame::OnUpdateStartSearch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bIsSearching);
}

void CMainFrame::OnBykeywords() 
{
	// TODO: Add your command handler code here
	m_bUseExp = FALSE;
	m_wndToolBar.m_wndChk.EnableWindow(TRUE);
	if (m_nType == SHOWTYPE_SQL)
		OnStatusText(1, (LPARAM)(g_bLanguage ? "关键词匹配" : "Keyword Match"));
}

void CMainFrame::OnUpdateBykeywords(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!m_bUseExp);
}

void CMainFrame::OnByexp() 
{
	// TODO: Add your command handler code here
	m_bUseExp = TRUE;
	m_wndToolBar.m_wndChk.EnableWindow(FALSE);
	if (m_nType == SHOWTYPE_SQL)
		OnStatusText(1, (LPARAM)(g_bLanguage ? "表达式匹配" : "Expression Match"));
}

void CMainFrame::OnUpdateByexp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bUseExp);
}

LRESULT CMainFrame::OnSearchIt(WPARAM wParam, LPARAM lParam)
{
	OnStartSearch();
	return 0;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (nType != SIZE_MINIMIZED && ::IsWindow(m_wndStatusBar.m_hWnd))
	{
		CRect rect;
		GetClientRect(&rect);
		m_wndStatusBar.SetPaneInfo(0, ID_INDC_PROMPT, SBPS_NORMAL, rect.Width() - 200);      
		m_wndStatusBar.SetPaneInfo(1, ID_INDC_OTHER, SBPS_STRETCH ,0);
	}
}

void CMainFrame::OnUpdatePane(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnDelete() 
{
	// TODO: Add your command handler code here
	if (!VerifyViews())
		return;
	
	CWnd* pFoc = GetFocus();
	if (pFoc == m_pwndRealms)
	{
		HTREEITEM h = m_pwndRealms->GetSelectedItem();
		if (h == NULL)
			return;
		
		DWORD dwID = m_pwndRealms->GetItemData(h);
		if (dwID <= 1)
			return;

		CString str = m_pwndRealms->GetItemText(h);
		CString sWarn;
		BOOL bDelete = FALSE;

		if (m_pwndRealms->GetParentItem(h))
		{
			// account
			if (!g_bLanguage)
				sWarn.Format("By deleting account \"%s\", all players, items that are associated to this account will be deleted too.\n\nAre you sure you want to proceed?", str);
			else
				sWarn.Format("如果删除账号\"%s\"，所有关联到这个账号的角色、物品也将被删除。\n\n你确定要继续吗？?", str);

			if (MessageBox(sWarn, "Account Deletion", MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
			{
				// delete the account
				m_db.DeleteAccount(dwID);
				bDelete = TRUE;
			}
		}
		else
		{
			// realm
			if (!g_bLanguage)
				sWarn.Format("By deleting realm \"%s\", all accounts, players, items that are associated to this realm will be deleted too.\n\nAre you sure you want to proceed?", str);
			else
				sWarn.Format("如果删除国度\"%s\"，所有关联到这个国度的账号、角色、物品也将被删除。\n\n你确定要继续吗？?", str);

			if (MessageBox(sWarn, "Realm Deletion", MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
			{
				// delete the realm
				m_db.DeleteRealm(dwID);
				bDelete = TRUE;
			}
		}
		
		if (bDelete)
			m_pwndRealms->DeleteItem(h);
	}
	else if (pFoc == m_pwndChars)
	{
		int nIdx = m_pwndChars->GetSelectedChar();
		if (nIdx < 0)
			return;

		DWORD dwPlayerID = m_pwndChars->GetItemData(nIdx);
		dwPlayerID &= ~HC_FLAG;
		if (dwPlayerID == 0)
			return;

		CString str = m_pwndChars->GetItemText(nIdx, 0);
		CString sWarn;
		if (!g_bLanguage)
			sWarn.Format("By deleting player \"%s\", all items that are associated to this player will be deleted too.\n\nAre you sure you want to proceed?", str);
		else
			sWarn.Format("如果删除角色\"%s\"，所有关联到这个角色的物品也将被删除。\n\n你确定要继续吗？?", str);
		if (MessageBox(sWarn, "Player Deletion", MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
		{
			// delete the player
			m_db.DeletePlayer(dwPlayerID);
			m_pwndChars->DeleteItem(nIdx);
		}
	}
}

void CMainFrame::OnUpdateDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bOK = FALSE;
	CWnd* pFoc = GetFocus();
	if (VerifyViews() && pFoc && (pFoc == m_pwndRealms || pFoc == m_pwndChars))
	{
		if (pFoc == m_pwndRealms)
		{
			HTREEITEM h = m_pwndRealms->GetSelectedItem();
			if (h && m_pwndRealms->GetItemData(h) > 1)
				bOK = TRUE;
		}
		else
		{
			if (m_pwndChars->GetSelectedCount())
				bOK = TRUE;
		}
	}

	pCmdUI->Enable(bOK);
}

void CMainFrame::OnExport() 
{
	// TODO: Add your command handler code here
	if (!VerifyViews())
		return;

	if (m_nType == SHOWTYPE_SQL)
	{
		CString sSQL;
		m_wndToolBar.m_wndEdit.GetWindowText(sSQL);
		sSQL.TrimLeft();
		sSQL.TrimRight();
		
		if (!sSQL.IsEmpty() && m_pwndSQL->GetItemCount())
			Export(m_pwndSQL, sSQL);
	}
	else
	{
		const CD2PlayerData* p = m_pwndD2->GetPlayer();
		if (p)
			Export(m_pwndItems, p->sName);
	}
}

void CMainFrame::OnUpdateExport(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (!VerifyViews())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if (m_nType == SHOWTYPE_SQL)
		pCmdUI->Enable(m_pwndSQL->GetItemCount());
	else
		pCmdUI->Enable(m_pwndD2->GetPlayerID());
}

BOOL CMainFrame::ExportProc(CStdioFile* pFile, CReportCtrl* pwndList, int nIndex, BOOL bHTM)
{
	if (bHTM)
		pFile->WriteString("<tr>");

	for (int i = 0; i < pwndList->GetColumnCount(); i++)
		WriteItemLine(pFile, pwndList->GetItemText(nIndex, i), bHTM, i ? RGB(0, 0, 0) : pwndList->GetItemTextColor(nIndex, i));

	if (bHTM)
		pFile->WriteString("</tr>");
	pFile->WriteString("\n");	
	return TRUE;
}

void CMainFrame::WriteItemLine(CStdioFile *pFile, const CString &str, BOOL bHtml, COLORREF color)
{
	if (bHtml)
	{
		pFile->WriteString("<td align=\"center\"");
		if (color)
		{
			CString sColor;
			sColor.Format("#%02X%02X%02X", GetRValue(color), GetGValue(color), GetBValue(color));
			pFile->WriteString(" bgcolor=#000000><font color=");
			pFile->WriteString(sColor);
		}
		pFile->WriteString(">");
	}

	pFile->WriteString(str);

	if (bHtml)
	{
		if (color)
			pFile->WriteString("</font>");
		pFile->WriteString("</td>");
	}
	else
	{
		pFile->WriteString("\t");
	}
}

BOOL CMainFrame::Export(CReportCtrl* pwndList, LPCSTR lpszTitle)
{
	if (pwndList == NULL || pwndList->GetColumnCount() == 0 || lpszTitle == NULL)
		return FALSE;

	CString sFilter = g_bLanguage ?
		"网页文件(*.htm, *.html)|*.htm;*.html|文本文件(*.txt, *.log)|*.txt;*.log|所有文件(*.*)|*.*||" :
		"HTML Files(*.htm, *.html)|*.htm;*.html|Text Files(*.txt, *.log)|*.txt;*.log|All Files(*.*)|*.*||";
	CFileDialog fd(FALSE, "htm", "Export.htm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFilter, this);
	if (fd.DoModal() != IDOK)
		return FALSE;

	CString sExt = fd.GetFileExt();
	sExt.MakeLower();
	const BOOL HTM = sExt == "htm" || sExt == "html";

	CStdioFile f;
	if (!f.Open(fd.GetPathName(), CFile::modeWrite | CFile::modeCreate))
	{
		CString sErr;
		sErr.Format("Failed to open \"%s\"!", (LPCSTR)fd.GetPathName());
		MessageBox(sErr, "File Access Failure", MB_ICONEXCLAMATION);
		return FALSE;
	}

	// file header
	if (HTM)
	{
		f.WriteString("<html>\n");
		f.WriteString("<head><title>Item DB Export</title></head>\n");
		f.WriteString("<body bgcolor=\"#000000\" text=\"#C4C4C4\">\n");
		f.WriteString("<h2>");
		f.WriteString(lpszTitle);
		f.WriteString("<h2><p></p>");
		f.WriteString("<table width=\"100%\">\n<tr>");

		for (int i = 0; i < pwndList->GetColumnCount(); i++)
		{
			f.WriteString("<td align=\"center\"><b>");
			f.WriteString(pwndList->GetHeaderText(i));
			f.WriteString("</b></td>\n");
		}

		f.WriteString("\n</tr><tr><td colspan=");
		CString sSpan;
		sSpan.Format("%d", pwndList->GetColumnCount()); 
		f.WriteString(sSpan);
		f.WriteString("><hr size=1 noshade></td></tr>\n");
	}

	CProgressWnd dlg;
	dlg.Create(this, "ItemDB Export");
	dlg.SetRange(0, pwndList->GetItemCount());
	dlg.SetPos(0);
	dlg.SetStep(1);

	// contents
	for (int i = 0; i < pwndList->GetItemCount(); i++)
		ExportProc(&f, pwndList, i, HTM);

	dlg.DestroyWindow();

	// html end
	if (HTM)
	{
		f.WriteString("</table>\n");
		f.WriteString("</body>\n");
		f.WriteString("</html>");
	}

	f.Close();
	if (IDYES == MessageBox(g_bLanguage ? "数据导出成功。\n是否要打开导出的文件？" : "Date exported successfully.\nWould you like to open the exported file?", "Export", MB_ICONINFORMATION | MB_YESNO))
	{
		if (HTM)
			::ShellExecute(m_hWnd, "open", fd.GetPathName(), NULL, NULL, SW_SHOW);
		else
			::ShellExecute(m_hWnd, "open", "notepad", fd.GetPathName(), NULL, SW_SHOW);
	}

	return TRUE;
}

void CMainFrame::OnLangEng() 
{
	// TODO: Add your command handler code here
	((CItemDBApp*)(AfxGetApp()))->SetLanguage(FALSE);
	MessageBox("程序语言已被设为英语。这一改动要到下一次运行ItemDB时才会生效。\n\nApplication language has been set to English, this change will take effect next time when you run ItemDB.", "Language Change", MB_OK | MB_ICONINFORMATION);
}

void CMainFrame::OnUpdateLangEng(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!g_bLanguage);
	pCmdUI->Enable(g_bLanguage);
}

void CMainFrame::OnLangChs() 
{
	// TODO: Add your command handler code here
	((CItemDBApp*)(AfxGetApp()))->SetLanguage(TRUE);
	MessageBox("程序语言已被设为简体中文。这一改动要到下一次运行ItemDB时才会生效。\n\nApplication language has been set to Simplified Chinese, this change will take effect next time when you run ItemDB.", "Language Change", MB_OK | MB_ICONINFORMATION);
}

void CMainFrame::OnUpdateLangChs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bLanguage);
	pCmdUI->Enable(!g_bLanguage);
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	if (nID == SC_MINIMIZE)
	{
		m_bMaximized = IsZoomed();
		m_tray.MinimiseToTray(this);
	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);		
	}
}

LRESULT CMainFrame::OnGameWnd(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		HMODULE hModule = RemoteLoadLibrary((HWND)lParam, AfxGetAppEx()->GetAppDir() + "\\" + CLIENT_FILE);
		if (hModule == NULL)
		{
			CString sErr;
			sErr.Format(g_bLanguage ? "加载ItemDB.dll到0x%08X失败！" : "Failed to load ItemDB.dll into 0x%08X!", lParam);
			MessageBox(sErr, "ItemDB.dll", MB_ICONEXCLAMATION);
		}
	}

	return 0;
}

void CMainFrame::OnDestroy() 
{	
	// TODO: Add your message handler code here	
	AfxGetApp()->WriteProfileInt("", "Exp Match", !!m_bUseExp);
	m_msgWnd.Destroy();
	m_monitor.Stop();
	CString sDll = AfxGetAppEx()->GetAppDir() + "\\" + CLIENT_FILE;
	::EnumWindows(CleanupProc, (LPARAM)(LPCSTR)sDll);
	CFrameWnd::OnDestroy();
}

BOOL CMainFrame::CleanupProc(HWND hWnd, LPARAM lParam)
{
	if (lParam == 0)
		return FALSE;

	static char szClass[12] = "";
	::GetClassName(hWnd, szClass, 11);
	if (strcmp(szClass, "Diablo II"))
		return TRUE;

	HMODULE hModule = RemoteGetModuleHandle(hWnd, (LPCSTR)lParam);
	while (hModule)
	{
		RemoteFreeLibrary(hWnd, hModule);
		hModule = RemoteGetModuleHandle(hWnd, (LPCSTR)lParam);
	}	

	return TRUE;
}

LRESULT CMainFrame::OnClientUpdate(WPARAM wParam, LPARAM lParam)
{
	if (wParam != WM_ITEMDB_IDENTIFIER)
		return 0;

	m_tray.Animate(500, 5);

	CString sPath; 
	sPath.Format("%s\\Cache\\ItemDB_%08X.tmp", AfxGetAppEx()->GetAppDir(), lParam);
	HANDLE hFile = ::CreateFile(sPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	DWORD dwDummy;
	CHARINFO player;
	if (::ReadFile(hFile, &player, sizeof(player), &dwDummy, NULL) && dwDummy == sizeof(player))
	{	
		DWORD dwFileSize = ::GetFileSize(hFile, &dwDummy);
		int nCount = ((long)dwFileSize - sizeof(CHARINFO)) / sizeof(ITEMINFO);
		OnUpdatePlayer(&player, nCount, hFile);
	}

	::CloseHandle(hFile);
	::DeleteFile(sPath);
	return 0;
}

BOOL CMainFrame::OnUpdatePlayer(LPCHARINFO lpPlayer, int nItemCount, HANDLE hFile)
{
	if (!VerifyViews() || lpPlayer == NULL || lpPlayer->szCharName[0] == 0 || hFile == NULL)
		return FALSE;
	
	DWORD dwPlayerID = GetCharID(lpPlayer);
	if (dwPlayerID == 0)
		return FALSE;

	DWORD dwPrevPlayer = m_pwndD2->GetPlayerID();
	if (m_pwndD2->IsCurrentPlayer(dwPlayerID))
	{		
		m_pwndItems->DeleteAllItems();
		m_pwndD2->SetPlayer(NULL);
	}

	CString str;
	str.Format("delete from [item] where playerid=%d", dwPlayerID);
	m_db.Lock();
	m_db.Execute(str);
	m_db.Unlock();	

	// un-cache the player, we will be reloading from database anyway
	RemoveCachedPlayer(dwPlayerID);
	
	// now flush all item data to our database
	DWORD dwRead;
	ITEMINFO item;
	CProgressWnd dlg;	
	dlg.Create(this, "Processing Item Data");

	dlg.SetRange(0, max(1, nItemCount));
	dlg.SetPos(0);
	dlg.SetStep(1);

	while (::ReadFile(hFile, &item, sizeof(item), &dwRead, NULL) && dwRead == sizeof(item))
	{
		if (item.szCode[0] == 0 || item.szCode[3] != 0 || item.szName[0] == 0)
		{
			dlg.StepIt();
			continue; // invalid item data
		}

		dlg.SetText(item.szName);		
		RemoveSocketSymbol(&item);
		RemoveSingleQuote(item.szName);
		RemoveSingleQuote(item.szCode);
		item.iType = m_db.GetTypeID(item.iType);
		m_db.CheckItemBase(&item);		

		m_db.Lock();
		m_db.AddItem(dwPlayerID, &item);
		m_db.Unlock();
	
		dlg.StepIt();
	}

	//if (dwPrevPlayer == dwPlayerID)
	//	m_pItemView->ListItems(dwPlayerID);

	return TRUE;
}

void CMainFrame::RemoveSingleQuote(LPSTR lpsz)
{
	if (lpsz == NULL)
		return;

	int i = 0;
	for (i = 0; lpsz[i]; i++)
	{
		if (lpsz[i] == '\'')
			lpsz[i] = '^';
	}
}

void CMainFrame::RemoveSocketSymbol(LPITEMINFO lpItem)
{
	if (lpItem == NULL)
		return;

	if (lpItem->iSockets)
	{
		LPSTR p = ::strchr(lpItem->szName, '(');
		if (p && p > lpItem->szName)
			*(p - 1) = 0;
	}
}

DWORD CMainFrame::GetCharID(LPCHARINFO lpChar)
{
	if (lpChar == NULL || !VerifyViews())
		return 0; // single player

	RemoveSingleQuote(lpChar->szRealm);
	RemoveSingleQuote(lpChar->szAccount);
	RemoveSingleQuote(lpChar->szCharName);

	DWORD dwRealmID = 0, dwAccountID = 0, dwCharID = 0;
	HTREEITEM hRealm = NULL, hAccount = NULL;
	int nResult = 0;

	if (lpChar->szAccount[0] == 0 || lpChar->szRealm[0] == 0)
	{
		dwRealmID = 1;
		dwAccountID = 1;
	}

	if (dwRealmID == 0)
	{
		m_db.Lock();
		nResult = m_db.GetRealmID(lpChar->szRealm, dwRealmID);
		m_db.Unlock();
		if (nResult == RECORD_ADDED)
		{
			hRealm = m_pwndRealms->InsertItem(lpChar->szRealm, 0, 0);
			m_pwndRealms->SetItemData(hRealm, dwRealmID);
		}		
	}
	
	if (hRealm == NULL)
		hRealm = FindTreeItem(TVI_ROOT, dwRealmID);

	if (dwAccountID == 0)
	{
		m_db.Lock();
		nResult = m_db.GetAccountID(lpChar->szAccount, dwRealmID, dwAccountID);
		m_db.Unlock();
		if (nResult == RECORD_ADDED)
		{
			hAccount = m_pwndRealms->InsertItem(lpChar->szAccount, 0, 0, hRealm);
			m_pwndRealms->SetItemData(hAccount, dwAccountID);
		}				
	}

	if (hAccount == NULL)
		hAccount = FindTreeItem(hRealm, dwRealmID);

	m_db.Lock();
	dwCharID = m_db.CreatePlayer(lpChar, dwAccountID);
	m_db.Unlock();

	// add to list ctrl if the current selected account is hAccount
	if (m_pwndRealms->GetSelectedItem() == hAccount)
	{
		const int ORIG = m_pwndChars->FindCharFromList(dwCharID);
		int nOrigSel = m_pwndChars->GetSelectedChar();		
		
		const int nIdx = m_pwndChars->InsertItem(INT_MAX, lpChar->szCharName, lpChar->iCharClass);
		DWORD dwData = dwCharID;
		if (lpChar->iHardcore)
			dwData |= HC_FLAG;
		m_pwndChars->SetItemData(nIdx, dwData);
		
		if (nOrigSel != -1 && nOrigSel == ORIG)
		{
			m_pwndChars->SetItemState(nIdx, LVIS_SELECTED, LVIS_SELECTED);
			m_pwndChars->EnsureVisible(nIdx, FALSE);		
		}	
		
		m_pwndChars->DeleteItem(ORIG);
		m_pwndChars->Arrange(LVA_DEFAULT);  
	}

	return dwCharID;
}

BOOL CMainFrame::RemoveCachedPlayer(DWORD dwPlayerID)
{
	BOOL bFound = FALSE;
	m_aCache.Lock();
	POSITION pos = m_aCache.GetHeadPosition();
	while (pos)
	{
		POSITION cur = pos;
		CD2PlayerData* p = m_aCache.GetNext(pos);
		if (p && p->dwPlayerID == dwPlayerID)
		{
			m_aCache.RemoveAt(cur);
			bFound = TRUE;
			break;
		}
	}
	m_aCache.Unlock();
	return bFound;
}

void CMainFrame::OnHelphelp() 
{
	// TODO: Add your command handler code here
	AfxGetApp()->WinHelp(0);
}

BOOL CMainFrame::VerifyViews() const
{
	return m_pwndRealms && m_pwndChars && m_pwndD2 && m_pwndItems && m_pwndSQL;
}

void CMainFrame::SetType(int nType)
{
	if (!VerifyViews())
		return;

	m_nType = nType;
	const CD2PlayerData* p = m_pwndD2->GetPlayer();
	if (m_nType == SHOWTYPE_D2)
	{
		m_wndSplitterH.SetRowInfo(1, 32767, 0);
		m_wndSplitterH.SetRowInfo(2, 0, 0);	
		OnStatusText(0, (LPARAM)(g_bLanguage ? "移动鼠标查看物品细节" : "Hover cursor to view item details"));
		OnStatusText(1, (LPARAM)(p ? (LPCSTR)p->sName : ""));
	}
	else
	{
		if (m_nType == SHOWTYPE_SQL)
		{
			m_pwndItems->ShowWindow(SW_HIDE);
			m_pwndSQL->ShowWindow(SW_SHOW);
			CString str;
			str.Format(g_bLanguage ? "总共找到%d笔数据。(双击一件物品切换到图形视窗)" : "%d records found. (Double-click on an item to switch to graphic view)", m_pwndSQL->GetItemCount());
			OnStatusText(0, (LPARAM)(LPCSTR)str);
			if (g_bLanguage)
				str = m_bUseExp ? "表达式匹配" : "关键词匹配";
			else
				str = m_bUseExp ? "Expression Match" : "Keyword Match";
			OnStatusText(1, (LPARAM)(LPCSTR)str);
		}
		else
		{
			m_pwndItems->ShowWindow(SW_SHOW);
			m_pwndSQL->ShowWindow(SW_HIDE);
			OnStatusText(0, (LPARAM)(g_bLanguage ? "双击一件物品切换到图形视窗" : "Double-click on an item to switch to graphic view"));
			OnStatusText(1, (LPARAM)(p ? (LPCSTR)p->sName : ""));
		}

		m_wndSplitterH.SetRowInfo(1, 0, 0);
		m_wndSplitterH.SetRowInfo(2, 32767, 0);		
	}

	m_wndSplitterH.RecalcLayout();
}

HTREEITEM CMainFrame::FindTreeItem(HTREEITEM hParent, DWORD dwItemID) const
{
	if (m_pwndRealms == NULL)
		return NULL;

	HTREEITEM h = m_pwndRealms->GetNextItem(hParent, TVGN_CHILD);
	while (h)
	{
		if (m_pwndRealms->GetItemData(h) == dwItemID)
			return h;

		h = m_pwndRealms->GetNextItem(h, TVGN_NEXT);
	}

	return NULL;
}

void CMainFrame::ListItems(DWORD dwPlayerID)
{
	if (!VerifyViews())
		return;

	if (m_pwndD2->IsCurrentPlayer(dwPlayerID))
		return;

	if (m_nType != SHOWTYPE_SQL)
		OnStatusText(1, 0);
	m_pwndItems->DeleteAllItems();	
	m_pwndD2->SetPlayer(NULL);
	if (dwPlayerID == 0)
		return;

	CD2PlayerData* pPlayer = CachePlayerData(dwPlayerID);
	ASSERT(pPlayer);
	
	m_pwndItems->LockWindowUpdate();
	POSITION pos = pPlayer->m_aItemList.GetHeadPosition();
	while (pos)
	{
		CD2ItemData* pItem = pPlayer->m_aItemList.GetNext(pos);
		ASSERT(pItem);
		
		const int IDX = m_pwndItems->InsertItem(INT_MAX, pItem->sName);
		m_pwndItems->SetItemData(IDX, pItem->dwItemID);
		m_pwndItems->SetItemTextColor(IDX, 0, pItem->GetDescColor(), FALSE);
		m_pwndItems->SetItemText(IDX, 1, pItem->sCode);
		m_pwndItems->SetItemText(IDX, 2, pItem->dwiLevel);
		m_pwndItems->SetItemText(IDX, 3, pItem->sQualityName);
		if (pItem->nSockets)
			m_pwndItems->SetItemText(IDX, 4, pItem->nSockets);
		if (pItem->bEthereal)
			m_pwndItems->SetItemText(IDX, 5, "Y");
		m_pwndItems->SetItemText(IDX, 6, pItem->sTypeName);
		m_pwndItems->SetItemText(IDX, 7, pItem->sLocation);
	}

	for (int i = 1; i < m_pwndItems->GetColumnCount(); i++)
		m_pwndItems->SetItemTextColor(-1, i, RGB(196, 196, 196), FALSE);
	m_pwndItems->SetItemBkColor(-1, -1, RGB(0, 0, 0), TRUE);
	
	m_pwndItems->UnlockWindowUpdate();
	m_pwndD2->SetPlayer(pPlayer);

	if (m_nType == SHOWTYPE_SQL)
		SetType(SHOWTYPE_D2);

	OnStatusText(1, (LPARAM)(LPCSTR)pPlayer->sName);
}
