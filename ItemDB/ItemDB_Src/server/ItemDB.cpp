// ItemDB.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ItemDB.h"

#include "MainFrm.h"
#include "ItemDBDoc.h"
#include "RealmView.h"
#include "External\\HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL g_bLanguage = FALSE;
#define APP_TITLE		"ItemDB"
#define APP_VER			"4.00"

/////////////////////////////////////////////////////////////////////////////
// CItemDBApp

BEGIN_MESSAGE_MAP(CItemDBApp, CWinApp)
	//{{AFX_MSG_MAP(CItemDBApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemDBApp construction

CItemDBApp::CItemDBApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CItemDBApp object

CItemDBApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CItemDBApp initialization

BOOL CItemDBApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	CreateSingleton("{EECEBFA0-0B1D-452D-824D-3C0AD121B061}");
	if (!IsInstanceUnique())
	{
		RestorePrevWnd();
		return FALSE;
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	//LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	SetRegistryKey(_T("Abin"));

	WriteProfileString(_T(""), _T("Version"), APP_VER); // version
	WriteProfileString(_T(""), _T("Path"), m_sAppDir); // app path
	g_bLanguage = GetProfileInt("", "Language", 0);

	if (g_bLanguage)
		m_sAppTitle = "Diablo II 物品数据库 ";
	else
		m_sAppTitle = "Diablo II Item Database v";
	m_sAppTitle += APP_VER;
	if (g_bLanguage)
		m_sAppTitle += "版";

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CItemDBDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CRealmView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	
	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);		
	m_pMainWnd->UpdateWindow();	

	return TRUE;
}

void CItemDBApp::SetLanguage(BOOL bLang)
{
	WriteProfileInt("", "Language", !!bLang);
}

CString CItemDBApp::GetAppTitle() const
{
	return m_sAppTitle;
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
	CHyperLink	m_wndUrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
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
	DDX_Control(pDX, IDC_URL, m_wndUrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CItemDBApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CItemDBApp message handlers


void CItemDBApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	::ShellExecute(NULL, _T("open"), _T("hh.exe"), g_bLanguage ? "Readme_chs.chm" : "Readme_eng.chm", m_sAppDir, SW_SHOW);
	//CWinApp::WinHelp(dwData, nCmd);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CItemDBApp* pApp = (CItemDBApp*)AfxGetApp();
	if (pApp)
		SetDlgItemText(IDC_TITLE, pApp->GetAppTitle());
	m_wndUrl.SetURL("http://www.wxjindu.com/abin/");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
