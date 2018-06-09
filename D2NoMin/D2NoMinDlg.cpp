// D2NoMinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2NoMin.h"
#include "D2NoMinDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TRAY_NOTYFY WM_APP + 1000
#define WM_CHKBOX	WM_APP + 1001

#include "PopupMenu.h"

BOOL operator==(const D2ITEM& lhs, const D2ITEM& rhs)
{
	return lhs.hWnd == rhs.hWnd;
}

BOOL operator>(const D2ITEM& lhs, const D2ITEM& rhs)
{
	return lhs.hWnd > rhs.hWnd;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2NoMinDlg dialog

CD2NoMinDlg::CD2NoMinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD2NoMinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CD2NoMinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bEnabled = FALSE;
}

void CD2NoMinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD2NoMinDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CD2NoMinDlg, CDialog)
	//{{AFX_MSG_MAP(CD2NoMinDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_POPUP_ABOUT, OnPopupAbout)
	ON_COMMAND(ID_POPUP_DISABLE, OnPopupDisable)
	ON_UPDATE_COMMAND_UI(ID_POPUP_DISABLE, OnUpdatePopupDisable)
	ON_COMMAND(ID_POPUP_ENABLE, OnPopupEnable)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ENABLE, OnUpdatePopupEnable)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_COMMAND(ID_SETTING, OnSetting)
	//}}AFX_MSG_MAP
	ON_MESSAGE(TRAY_NOTYFY, OnTrayNotify)
	ON_MESSAGE(WM_CHKBOX, OnChkBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2NoMinDlg message handlers

BOOL CD2NoMinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_wndList.SetCheckboxeStyle();
	m_wndList.SetColumnHeader(_T("HWND, 100; Title, 200"));
	m_tray.Create(this, TRAY_NOTYFY, APP_TITLE_DISABLED, m_hIcon, IDR_MAINFRAME);
	ListAllD2Wnds();
	//m_tray.MinimiseToTray(this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CD2NoMinDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT nMsg = (UINT)lParam;
	if (nMsg == WM_RBUTTONUP)
	{
		if (this->IsWindowVisible())
			this->SetForegroundWindow();
		else
			PopupMenu(IDR_POPUP, 0, this);
	}
	else if (nMsg == WM_LBUTTONDOWN)
	{
		if (this->IsWindowVisible())
			this->SetForegroundWindow();
		else
			OnSetting();
	}

	return (LRESULT)0;
}

void CD2NoMinDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2NoMinDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CD2NoMinDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CD2NoMinDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default	
	if (nIDEvent == ID_TMR_NOMIN)
	{
		for (int i = m_aWnds.GetSize() - 1; i >= 0; i--)
		{
			if (!::IsWindow(m_aWnds[i].hWnd))
			{
				m_aWnds.RemoveAt(i);
			}
			else
			{
				if (m_aWnds[i].bSelected && ::IsIconic(m_aWnds[i].hWnd))
					::ShowWindow(m_aWnds[i].hWnd, SW_SHOWNOACTIVATE);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CD2NoMinDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(ID_TMR_NOMIN);
}

void CD2NoMinDlg::OnPopupAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CD2NoMinDlg::OnPopupDisable() 
{
	// TODO: Add your command handler code here
	KillTimer(ID_TMR_NOMIN);
	m_bEnabled = FALSE;
	m_tray.SetTooltipText(APP_TITLE_DISABLED);
}

void CD2NoMinDlg::OnUpdatePopupDisable(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bEnabled);
}

void CD2NoMinDlg::OnPopupEnable() 
{
	// TODO: Add your command handler code here
	if (!m_bEnabled)
	{
		SetTimer(ID_TMR_NOMIN, 500, NULL);
		m_bEnabled = TRUE;
		m_tray.SetTooltipText(APP_TITLE_ENABLED);
	}
}

void CD2NoMinDlg::OnUpdatePopupEnable(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bEnabled);
}

void CD2NoMinDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	for (int i = 0; i < m_aWnds.GetSize(); i++)
		m_aWnds[i].bSelected = m_wndList.ExamItemStates(i, RC_ITEM_CHECKED);

	m_tray.MinimiseToTray(this);
	//CDialog::OnCancel();
}

void CD2NoMinDlg::ListAllD2Wnds()
{
	CArrayEx<D2ITEM, const D2ITEM&> aTemp;
	::EnumWindows(EnumWndProc, (LPARAM)&aTemp);

	aTemp.Sort();
	int i;
	for (i = 0; i < aTemp.GetSize(); i++)
	{
		int nIdx = m_aWnds.Find(aTemp[i]);
		if (nIdx != -1)
			aTemp[i].bSelected = m_aWnds[nIdx].bSelected;
	}

	m_aWnds.Copy(aTemp);
	m_wndList.DeleteAllItems();

	for (i = 0; i < m_aWnds.GetSize(); i++)
	{
		CString str;
		str.Format(_T("0x%08X"), (DWORD)m_aWnds[i].hWnd);
		m_wndList.InsertItemEx(INT_MAX, str, m_aWnds[i].szTitle);
		if (m_aWnds[i].bSelected)
			m_wndList.SetItemStates(i, RC_ITEM_CHECKED);
	}

}

BOOL CD2NoMinDlg::EnumWndProc(HWND hWnd, LPARAM lParam)
{
	CArrayEx<D2ITEM, const D2ITEM&>* pArray = (CArrayEx<D2ITEM, const D2ITEM&>*)lParam;
	if (pArray == NULL)
		return FALSE;

	D2ITEM item = {0};
	::GetClassName(hWnd, item.szTitle, 255);
	if (!_tcsicmp(item.szTitle, _T("Diablo II")))
	{
		item.hWnd = hWnd;
		::GetWindowText(hWnd, item.szTitle, 255);
		if (pArray->Find(item) == -1)
			pArray->Add(item);
	}

	return TRUE;
}

void CD2NoMinDlg::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_aWnds.GetSize(); i++)
		m_aWnds[i].bSelected = m_wndList.ExamItemStates(i, RC_ITEM_CHECKED);
	ListAllD2Wnds();
}

LRESULT CD2NoMinDlg::OnChkBox(WPARAM wParam, LPARAM lParam)
{
	if (m_aWnds.IsIndexValid(wParam))
	{
		m_aWnds[wParam].bSelected = m_wndList.ExamItemStates(wParam, RC_ITEM_CHECKED);
	}
	return 0;
}

void CD2NoMinDlg::OnSetting() 
{
	// TODO: Add your command handler code here
	m_tray.MaximiseFromTray(this);
}
