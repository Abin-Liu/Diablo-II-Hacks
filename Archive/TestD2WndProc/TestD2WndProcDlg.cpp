// TestD2WndProcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestD2WndProc.h"
#include "TestD2WndProcDlg.h"
#include "WndProcHook\\WndProcHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_WNDPROCHOOK	WM_APP + 120

WNDPROC m_wp = 0;


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
// CTestD2WndProcDlg dialog

CTestD2WndProcDlg::CTestD2WndProcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestD2WndProcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestD2WndProcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_wp = 0;
}

void CTestD2WndProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestD2WndProcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestD2WndProcDlg, CDialog)
	//{{AFX_MSG_MAP(CTestD2WndProcDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDPROCHOOK, OnHookProc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestD2WndProcDlg message handlers

BOOL CTestD2WndProcDlg::OnInitDialog()
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
	HINSTANCE h = AfxGetInstanceHandle();
	BOOL b = InstallHook(h);
	AddNotify(::FindWindow("diablo ii", 0), m_hWnd, WM_WNDPROCHOOK);

	char sz[2] = "";
	sz[0] = 0x3f;
	wchar_t wsz[255 + 1] = {0};
	::MultiByteToWideChar(CP_ACP, 0, sz, -1, wsz, 255);

	/*
	wsz[0] = 255;
	wsz[1] = 0;
	char sz[2] = "";
	::WideCharToMultiByte(CP_ACP, 0, wsz, -1, sz, 1, NULL, NULL);
*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestD2WndProcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestD2WndProcDlg::OnPaint() 
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
HCURSOR CTestD2WndProcDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestD2WndProcDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	HWND hWnd = ::FindWindow("diablo ii", 0);
	m_wp = (WNDPROC)::GetClassLong(hWnd, GCL_WNDPROC);
//	DWORD d = ::SetClassLong(hWnd, GCL_WNDPROC, (long)MyProc);
	//DWORD er = ::GetLastError();
//	int k = 0;
}

void CTestD2WndProcDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	UninstallHook();
}

LRESULT CTestD2WndProcDlg::OnHookProc(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CTestD2WndProcDlg::MyProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(m_wp, hWnd, nMsg, wParam, lParam);
}
