// LoaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Loader.h"
#include "LoaderDlg.h"
#include "..\\Includes\\CommonDef.h"
#include "..\\Includes\\LoaderDLL.h"
#include "..\\Includes\\RemoteLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CLoaderDlg dialog

CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoaderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoaderDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndD2List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CLoaderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_LOADUNLOAD, OnLoadunload)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_D2WND_EVENT, OnD2GameEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoaderDlg message handlers

BOOL CLoaderDlg::OnInitDialog()
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
	if (!m_msgWnd.Create(m_hWnd, MSGWND_CLASS))
	{
		MessageBox("Failed to create the message window.", "Loader Error", MB_ICONSTOP);
		OnCancel();
		return TRUE;
	}

	if (!StartHookGameWnds(m_msgWnd.GetSafeHwnd()))
	{
		MessageBox("Failed to start the D2 game monitor.", "Loader Error", MB_ICONSTOP);
		OnCancel();
		return TRUE;
	}

	m_wndD2List.InsertColumn(0, "PID", LVCFMT_LEFT, 80);
	m_wndD2List.InsertColumn(1, "Title", LVCFMT_LEFT, 150);
	m_wndD2List.InsertColumn(2, "D2Hackit", LVCFMT_LEFT, 80);
	m_wndD2List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	SetTimer(ID_TMR_MAIN, 1000, NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoaderDlg::OnPaint() 
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
HCURSOR CLoaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLoaderDlg::OnDestroy() 
{
	StopHookGameWnds(m_msgWnd.GetSafeHwnd());
	KillTimer(ID_TMR_MAIN);
	CDialog::OnDestroy();
}

LRESULT CLoaderDlg::OnD2GameEvent(WPARAM wParam, LPARAM lParam)
{	
	m_aCreats.Lock();
	m_aCreats.Add((HWND)wParam);
	m_aCreats.Unlock();
	return 0;
}

void CLoaderDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TMR_MAIN)
	{
		int i = 0;
		HWND hWnd = NULL;

		m_aCreats.Lock();
		for (i = m_aCreats.GetSize() - 1; i >= 0; i--)
		{
			if (::IsWindow(m_aCreats[i]))
			{
				hWnd = m_aCreats[i];
				m_aCreats.RemoveAt(i);
				OnD2WndCreate(hWnd);
			}
		}
		m_aCreats.Unlock();

		for (i = m_wndD2List.GetItemCount() - 1; i >= 0; i--)
		{
			hWnd = (HWND)m_wndD2List.GetItemData(i);
			if (!::IsWindow(hWnd))
				OnD2WndDestroy(hWnd, i);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CLoaderDlg::OnD2WndCreate(HWND hWnd)
{
	char szTemp[256] = "";			
	DWORD dwProcID = 0;
	::GetWindowThreadProcessId(hWnd, &dwProcID);
	sprintf(szTemp, "0x%08X", dwProcID);
	const int IDX = m_wndD2List.InsertItem(INT_MAX, szTemp);
	m_wndD2List.SetItemData(IDX, (DWORD)hWnd);

	::GetWindowText(hWnd, szTemp, 255);
	m_wndD2List.SetItemText(IDX, 1, szTemp);
	CheckD2Hackit(IDX);
}

void CLoaderDlg::OnD2WndDestroy(HWND hWnd, int nIndex)
{
	m_wndD2List.DeleteItem(nIndex);
}

void CLoaderDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	if (nState)
	{
		for (int i = m_wndD2List.GetItemCount() - 1;  i>= 0; i--)
			CheckD2Hackit(i);
	}
}

BOOL CLoaderDlg::CheckD2Hackit(int nIndex)
{
	HWND hWnd = (HWND)m_wndD2List.GetItemData(nIndex);
	if (hWnd == NULL)
		return FALSE;

	DWORD dwProcID = 0;
	::GetWindowThreadProcessId(hWnd, &dwProcID);
	HMODULE hModule = RemoteGetModuleHandle(dwProcID, AfxGetAppEx()->GetAppDir() + "\\Modules\\D2Hackit.dll");
	if (hModule == NULL)
	{
		m_wndD2List.SetItemText(nIndex, 2, "");
		return FALSE;
	}

	char szTemp[32] = "";
	sprintf(szTemp, "0x%08X", (DWORD)hModule);
	m_wndD2List.SetItemText(nIndex, 2, szTemp);
	return TRUE;
}

void CLoaderDlg::OnLoadunload() 
{
	// TODO: Add your control notification handler code here
	
}
