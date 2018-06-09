// D2 Item CodesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2 Item Codes.h"
#include "D2 Item CodesDlg.h"

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
// CD2ItemCodesDlg dialog

CD2ItemCodesDlg::CD2ItemCodesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD2ItemCodesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CD2ItemCodesDlg)
	m_sOutput = _T("");
	m_nType = 0;
	m_sCodes = _T("");
	m_sNames = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2ItemCodesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD2ItemCodesDlg)
	DDX_Control(pDX, IDC_NAMES, m_wndNames);
	DDX_Control(pDX, IDC_CODES, m_wndCodes);
	DDX_Control(pDX, IDC_OUTPUT, m_wndOutput);
	DDX_Text(pDX, IDC_OUTPUT, m_sOutput);
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	DDX_CBString(pDX, IDC_CODES, m_sCodes);
	DDV_MaxChars(pDX, m_sCodes, 3);
	DDX_CBString(pDX, IDC_NAMES, m_sNames);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CD2ItemCodesDlg, CDialog)
	//{{AFX_MSG_MAP(CD2ItemCodesDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DOIT, OnDoit)
	ON_BN_CLICKED(IDC_RADIO1, OnType)
	ON_BN_CLICKED(IDC_RADIO2, OnType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2ItemCodesDlg message handlers

BOOL CD2ItemCodesDlg::OnInitDialog()
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
	LoadLists();

	m_wndNames.ShowWindow(m_nType ? SW_HIDE : SW_SHOW);
	m_wndCodes.ShowWindow(m_nType ? SW_SHOW : SW_HIDE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CD2ItemCodesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CD2ItemCodesDlg::OnPaint() 
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
HCURSOR CD2ItemCodesDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CD2ItemCodesDlg::OnDoit() 
{
	// TODO: Add your control notification handler code here	
	m_wndOutput.SetWindowText("");
	UpdateData(TRUE);

	CComboBox* pWnd = m_nType ? &m_wndCodes : &m_wndNames;
	CString m_sInput = m_nType ? m_sCodes : m_sNames;

	m_sInput.TrimLeft();
	m_sInput.TrimRight();

	if (m_sInput.IsEmpty() || (m_nType && m_sInput.GetLength() != 3))
	{
		pWnd->SetFocus();
		pWnd->SetEditSel(0, -1);
		return;
	}

	ITEMENTRY ie = { 0 };	
	int nIndex = -1;
	if (m_nType) // code to name
	{
		::strncpy(ie.szCode, m_sInput, 3);
		ITEMENTRY1 e = { &ie };
		nIndex = m_aList1.Find(e);
	}
	else // name to code
	{
		::strncpy(ie.szName, m_sInput, 63);
		ITEMENTRY2 e = { &ie };
		nIndex = m_aList2.Find(e);
	}

	if (nIndex == -1)
	{
		pWnd->SetFocus();
		pWnd->SetEditSel(0, -1);
		return;
	}

	if (m_nType)
	{
		const ITEMENTRY1& er = m_aList1.GetAt(nIndex);
		m_sOutput = er.pEntry->szName;
	}
	else
	{
		const ITEMENTRY2& er = m_aList2.GetAt(nIndex);
		m_sOutput = er.pEntry->szCode;
	}
	
	UpdateData(FALSE);
	m_wndOutput.SetFocus();
	m_wndOutput.Highlight();
}

void CD2ItemCodesDlg::LoadLists()
{	
	m_aList1.RemoveAll();
	m_aList2.RemoveAll();
	m_wndNames.ResetContent();
	m_wndCodes.ResetContent();

	for (int i = 0; TRUE; i++)
	{
		const ITEMENTRY* p = GetEntry(i);
		if (p == NULL)
			break;

		ITEMENTRY1 ie1 = { p };
		ITEMENTRY2 ie2 = { p };
		m_aList1.Add(ie1);
		m_aList2.Add(ie2);
		m_wndNames.AddString(ie1.pEntry->szName);
		m_wndCodes.AddString(ie1.pEntry->szCode);
	}

	m_aList1.Sort();
	m_aList2.Sort();
}

void CD2ItemCodesDlg::OnType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndNames.ShowWindow(m_nType ? SW_HIDE : SW_SHOW);
	m_wndCodes.ShowWindow(m_nType ? SW_SHOW : SW_HIDE);
	CComboBox* pWnd = m_nType ? &m_wndCodes : &m_wndNames;
	pWnd->SetFocus();
	pWnd->SetEditSel(0, -1);
}
