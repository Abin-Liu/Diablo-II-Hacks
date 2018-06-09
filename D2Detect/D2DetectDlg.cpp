// D2DetectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2Detect.h"
#include "D2DetectDlg.h"
#include "ArrayEx.h"

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
// CD2DetectDlg dialog

CD2DetectDlg::CD2DetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD2DetectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CD2DetectDlg)
	m_bShouldBeTrue = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hProcess = NULL;	
}

void CD2DetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD2DetectDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Check(pDX, IDC_CHECK1, m_bShouldBeTrue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CD2DetectDlg, CDialog)
	//{{AFX_MSG_MAP(CD2DetectDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2DetectDlg message handlers

BOOL CD2DetectDlg::OnInitDialog()
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
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_wndList.InsertColumn(0, _T("Address"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(1, _T("Value"), LVCFMT_CENTER, 100);
	m_wndList.InsertColumn(2, _T("Change"), LVCFMT_CENTER, 50);

	int h = 0x0ae3101d % 8;
/*
	for (int i = 0; INGAMEADDRS[i] != 0; i++)
	{
		CString str;
		str.Format(_T("0x%08X"), INGAMEADDRS[i]);
		m_wndList.InsertItem(INT_MAX, str);
	}
	*/
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CD2DetectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CD2DetectDlg::OnPaint() 
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
HCURSOR CD2DetectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CD2DetectDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(ID_TMR_MAIN);
	::CloseHandle(m_hProcess);
}

void CD2DetectDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TMR_MAIN)
	{
		//DoDetect();
		if (!m_d2Search.IsSearching())
		{
			KillTimer(ID_TMR_MAIN);
			CString str;

			if (m_d2Search.GetFoundCount())
			{
				DWORD dwOffset = m_d2Search.GetResultAddress(FALSE) - m_d2Search.GetResultPageAddress(FALSE);
				str.Format(_T("HP: %d/%d %d%%\nMana: %d/%d %d%%\nResult Address: %08X\nPage Address: %08X\nOffset: %08X\nMod8: %d\nMod16: %d\nMod32: %d\nMod64: %d\n"),
				m_d2Search.GetCurHP(), m_d2Search.GetMaxHP(), m_d2Search.GetHPPercent(),
				m_d2Search.GetCurMana(), m_d2Search.GetMaxMana(), m_d2Search.GetManaPercent(),
				m_d2Search.GetResultAddress(FALSE),
				m_d2Search.GetResultPageAddress(FALSE),
				dwOffset,
				dwOffset % 8,
				dwOffset % 16,
				dwOffset % 32,
				dwOffset % 64);
			}
			else
			{
				str = _T("No results found!");
			}
			
			MessageBox(str);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CD2DetectDlg::DoDetect()
{
	/*
	for (int i = 0; INGAMEADDRS[i] != 0; i++)
	{
		if (!::ReadProcessMemory(m_hProcess, (LPCVOID)INGAMEADDRS[i], &dwValue, 4, &dwRead)
			|| dwRead != 4)
		{
			m_wndList.SetItemText(i, 1, _T(""));
		}
		else
		{
			str.Format(_T("%08X"), dwValue);
			m_wndList.SetItemText(i, 1, str);

			if ((m_bShouldBeTrue && dwValue == 0) || (!m_bShouldBeTrue && dwValue))
				m_wndList.SetItemText(i, 2, _T("*"));
		}
	}
	*/
}

void CD2DetectDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	KillTimer(ID_TMR_MAIN);
	UpdateData(TRUE);

	::CloseHandle(m_hProcess);
	m_hProcess = NULL;

	/*
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		m_wndList.SetItemText(i, 1, _T(""));
		m_wndList.SetItemText(i, 2, _T(""));
	}
	*/	

	m_d2Search.SetD2Wnd(::FindWindow("Diablo ii", NULL));
	m_d2Search.Search();
	SetTimer(ID_TMR_MAIN, 100, NULL);
}

void CD2DetectDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	KillTimer(ID_TMR_MAIN);
}


