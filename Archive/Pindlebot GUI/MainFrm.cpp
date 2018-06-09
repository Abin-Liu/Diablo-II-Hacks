// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Pindlebot.h"

#include "MainFrm.h"
#include "PopupMenu.h"
#include "StatisticsDlg.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_TRAY_NOTIFY	WM_APP + 120

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_POPUP_HELP, OnPopupHelp)
	ON_COMMAND(ID_POPUP_ITEMLOG, OnPopupItemlog)
	ON_COMMAND(ID_POPUP_SETTINGS, OnPopupSettings)
	ON_COMMAND(ID_POPUP_STATISTICS, OnPopupStatistics)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAY_NOTIFY, OnTrayNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_tray.Create(this, WM_TRAY_NOTIFY, CString(_T("Pindlebot - ")) + AfxGetAppEx()->GetAppDir(), m_hIcon, IDR_MAINFRAME);
	m_tray.MinimiseToTray(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_SYSMENU;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG



LRESULT CMainFrame::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN)
	{
		CWnd* pWnd = GetLastActivePopup();
		if (pWnd != this)
			pWnd->SetForegroundWindow();
		else
			OnPopupSettings();
	}
	else if (lParam == WM_RBUTTONDOWN)
	{
		CWnd* pWnd = GetLastActivePopup();
		if (pWnd != this)
		{
			pWnd->SetForegroundWindow();
		}
		else
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU1);
			CMenu* p = menu.GetSubMenu(0);
			if (p)
			{
				p->SetDefaultItem(ID_POPUP_SETTINGS);
				PopupMenu(p, this);
			}
		}		
	}
	return (LRESULT)0;
}

void CMainFrame::OnPopupHelp() 
{
	// TODO: Add your command handler code here
	AfxGetApp()->WinHelp(0, 0);
}

void CMainFrame::OnPopupItemlog() 
{
	// TODO: Add your command handler code here
	::ShellExecute(NULL, _T("open"), _T("notepad.exe"), AfxGetAppEx()->GetAppDir() + _T("\\pickit.log"), NULL, SW_SHOW);
}

void CMainFrame::OnPopupSettings() 
{
	// TODO: Add your command handler code here
	LoadConfig();

	CIni ini;
	ini.SetPathName(AfxGetAppEx()->GetAppDir() + _T("\\Pindlebot.ini"));

	CCharListDlg dlg(&m_aConfigs,
		ini.GetUInt(_T("Default"), _T("Idle Length"), 300),
		ini.GetUInt(_T("Default"), _T("Use Shopbot"), FALSE));
	if (dlg.DoModal() != IDOK)
		return;
	
	SaveConfig();	
	ini.WriteUInt(_T("Default"), _T("Idle Length"), dlg.GetIdleLength());
	ini.WriteBool(_T("Default"), _T("Use Shopbot"), dlg.IsUseShopbot());
}

void CMainFrame::OnPopupStatistics() 
{
	// TODO: Add your command handler code here
	CStatisticsDlg dlg;
	dlg.DoModal();
}

void CMainFrame::LoadConfig()
{
	m_aConfigs.RemoveAll();
	CFileFind ff;
	BOOL bOK = ff.FindFile(AfxGetAppEx()->GetAppDir() + _T("\\Pindlebot\\*.bot"));
	while (bOK)
	{
		bOK = ff.FindNextFile();
		if (ff.IsDirectory())
			continue;

		PBOTCONFIG cfg = { 0 };
		if (PBotLoadConfig(ff.GetFilePath(), &cfg))
			m_aConfigs.Add(cfg);
	}
}

void CMainFrame::SaveConfig()
{
	::CreateDirectory(AfxGetAppEx()->GetAppDir() + _T("\\Pindlebot"), NULL);
	CFileFind ff;
	BOOL bOK = ff.FindFile(AfxGetAppEx()->GetAppDir() + _T("\\Pindlebot\\*.bot"));
	while (bOK)
	{
		bOK = ff.FindNextFile();
		if (!ff.IsDirectory())
			::DeleteFile(ff.GetFilePath());		
	}

	ff.Close();
	
	for (int i = 0; i < m_aConfigs.GetSize(); i++)
	{
		CString sFile;
		sFile.Format(_T("%s\\Pindlebot\\%s.bot"), AfxGetAppEx()->GetAppDir(), m_aConfigs[i].szCharName);
		PBotSaveConfig(sFile, &m_aConfigs[i]);
	}
}
