// D2XSpeedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2XSpeed.h"
#include "D2XSpeedDlg.h"
#include "TableDlg.h"
#include "D2XConsts.h"
#include "HyperLink.h"

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
	CHyperLink	m_cureteam;
	CHyperLink	m_author;
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
	DDX_Control(pDX, IDC_CURETEAM, m_cureteam);
	DDX_Control(pDX, IDC_ABIN, m_author);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2XSpeedDlg dialog

CD2XSpeedDlg::CD2XSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD2XSpeedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CD2XSpeedDlg)
	m_nBos = 0;
	m_nFana = 0;
	m_nIas = 0;
	m_nFrenzy = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2XSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD2XSpeedDlg)
	DDX_Control(pDX, IDC_CLASSES, m_ctlCharClasses);
	DDX_Control(pDX, IDC_FILTER, m_ctlFilter);
	DDX_Control(pDX, IDC_ATTACKFORM, m_ctlAttackForms);
	DDX_Control(pDX, IDC_WEAPONS, m_ctlWeapon);
	DDX_Control(pDX, IDC_SPINFRENZY, m_ctlSpinFrenzy);
	DDX_Control(pDX, IDC_SPINIAS, m_ctlSpinIas);
	DDX_Control(pDX, IDC_SPINBOS, m_ctlSpinBos);
	DDX_Control(pDX, IDC_SPINFANA, m_ctlSpinFana);
	DDX_Text(pDX, IDC_BOS, m_nBos);
	DDV_MinMaxInt(pDX, m_nBos, 0, 61);
	DDX_Text(pDX, IDC_FANA, m_nFana);
	DDV_MinMaxInt(pDX, m_nFana, 0, 40);
	DDX_Text(pDX, IDC_IAS, m_nIas);
	DDV_MinMaxInt(pDX, m_nIas, 0, 999);
	DDX_Text(pDX, IDC_FRENZY, m_nFrenzy);
	DDV_MinMaxInt(pDX, m_nFrenzy, 0, 51);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CD2XSpeedDlg, CDialog)
	//{{AFX_MSG_MAP(CD2XSpeedDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_BN_CLICKED(IDC_SHOWTABLE, OnShowtable)
	ON_CBN_SELCHANGE(IDC_ATTACKFORM, OnSelchangeAttackform)
	ON_CBN_SELCHANGE(IDC_CLASSES, OnSelchangeClasses)
	ON_CBN_SELCHANGE(IDC_FILTER, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_WEAPONS, OnSelchangeWeapons)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2XSpeedDlg message handlers

BOOL CD2XSpeedDlg::OnInitDialog()
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
	InitLists();

	m_ctlSpinIas.SetRange(MIN_WIAS, MAX_WIAS);
	m_ctlSpinBos.SetRange(MIN_BOS, MAX_BOS);
	m_ctlSpinFana.SetRange(MIN_FANA, MAX_FANA);
	m_ctlSpinFrenzy.SetRange(MIN_FRENZY, MAX_FRENZY);
	m_strFilterBuffer = "All";

	CheckStatus();	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CD2XSpeedDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CD2XSpeedDlg::OnPaint() 
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
HCURSOR CD2XSpeedDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CD2XSpeedDlg::CheckStatus()
{
	// disable controls
	GetDlgItem(IDC_LBLFANA)->EnableWindow(FALSE);
	GetDlgItem(IDC_FANA)->EnableWindow(FALSE);
	GetDlgItem(IDC_LBLBOS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BOS)->EnableWindow(FALSE);
	GetDlgItem(IDC_LBLFRENZY)->EnableWindow(FALSE);
	GetDlgItem(IDC_FRENZY)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHOWTABLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_FILTER)->EnableWindow(TRUE);
	GetDlgItem(IDC_WEAPONS)->EnableWindow(TRUE);

	if (m_wcr.GetAttackForm() == AF_CAST)
	{
		GetDlgItem(IDC_IASSOURCE)->SetWindowText("Total Fast Cast Source:");
		GetDlgItem(IDC_FILTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_WEAPONS)->EnableWindow(FALSE);
		m_nFana = 0;
		m_nBos = 0;
		m_nFrenzy = 0;
	}
	else
	{
		GetDlgItem(IDC_IASSOURCE)->SetWindowText("Total IAS Source:");
		GetDlgItem(IDC_LBLFANA)->EnableWindow();
		GetDlgItem(IDC_FANA)->EnableWindow();

		// selective enable
		if (m_wcr.Bos_able())
		{
			GetDlgItem(IDC_LBLBOS)->EnableWindow();
			GetDlgItem(IDC_BOS)->EnableWindow();
		}
		else
			m_nBos = 0;

		if (m_wcr.Frenzy_able())
		{
			GetDlgItem(IDC_LBLFRENZY)->EnableWindow();
			GetDlgItem(IDC_FRENZY)->EnableWindow();
		}
		else
			m_nFrenzy = 0;
	}

	if (m_wcr.GetBase() != DATA_INVALID)
	{
		GetDlgItem(IDC_SHOWTABLE)->EnableWindow();
	}

	UpdateData(FALSE);
}

void CD2XSpeedDlg::UpdateVals() 
{
	UpdateData(TRUE);
 	m_wcr.SetFana(m_nFana);
 	m_wcr.SetBos(m_nBos);
 	m_wcr.SetFrenzy(m_nFrenzy);
 	m_wcr.SetNWIas(m_nIas);
}

void CD2XSpeedDlg::OnExit() 
{
	OnOK();	
}

void CD2XSpeedDlg::OnAbout() 
{
	CAboutDlg d1;
	d1.DoModal();
}

void CD2XSpeedDlg::OnHelp() 
{
	WinExec("notepad readme.txt", SW_SHOW);	
}

void CD2XSpeedDlg::OnShowtable() 
{
	UpdateVals();
	CTableDlg d1(&m_wcr);
	d1.DoModal();
}

void CD2XSpeedDlg::ListWeapons()
{
	char* nameArray[MAX_WEAPONLIST];
	int nCount = m_wcr.GetWeaponNameList(nameArray, m_strFilterBuffer);
	m_ctlWeapon.ResetContent();

	for (int i = 0; i < nCount; i++)
		m_ctlWeapon.AddString(nameArray[i]);

	m_ctlWeapon.SetCurSel(0);
	OnSelchangeWeapons();
}

void CD2XSpeedDlg::InitLists()
{
	char* nameArray[30];
	int nCount, i;

	// initialize character class list
	nCount = m_wcr.GetCharNameList(nameArray);
	for (i = 0; i < nCount; i++)
		m_ctlCharClasses.AddString(nameArray[i]);
	m_ctlCharClasses.SetCurSel(0);
	OnSelchangeClasses();

	// initialize weapon filter list
	nCount = m_wcr.GetWeaponFilterList(nameArray);
	for (i = 0; i < nCount; i++)
		m_ctlFilter.AddString(nameArray[i]);
	m_ctlFilter.SetCurSel(0);

	// initialize attack form list
	ListAttackForm();
	m_ctlAttackForms.SetCurSel(0);
	OnSelchangeAttackform();

	// initialize weapon list
	ListWeapons();
}

void CD2XSpeedDlg::ListAttackForm()
{
	// back up original attack form
	CString strOriginalAF;
	m_ctlAttackForms.GetWindowText(strOriginalAF);

	m_ctlAttackForms.ResetContent();

	char* nameArray[10];
	int nPos, nCount = m_wcr.GetAttackFormList(nameArray);
	for (int i = 0; i < nCount; i++)
		m_ctlAttackForms.AddString(nameArray[i]);

	nPos = m_ctlAttackForms.FindString(-1, strOriginalAF);

	if (nPos >= 0) // original attack form still avialable
		m_ctlAttackForms.SetCurSel(nPos);
	else
	{
		nPos = m_ctlAttackForms.FindString(-1, "Normal");
		m_ctlAttackForms.SetCurSel(nPos);
		m_wcr.SetAttackForm(AF_REGULAR);
	}
}

void CD2XSpeedDlg::OnSelchangeAttackform() 
{
	CString str;
	m_ctlAttackForms.GetWindowText(str);
	m_wcr.SetAttackForm(str);
	CheckStatus();
}

void CD2XSpeedDlg::OnSelchangeClasses() 
{
	CString str;
	m_ctlCharClasses.GetWindowText(str);

	if (!m_wcr.IsSameCharClass(str))
	{
		// character class changed
		m_wcr.SetCharClass(str);
		ListAttackForm();
		CheckStatus();
	}
}

void CD2XSpeedDlg::OnSelchangeFilter() 
{
	CString str;
	m_ctlFilter.GetWindowText(str);

	if (str.CompareNoCase(m_strFilterBuffer))
	{
		// filter changed
		m_strFilterBuffer = str;
		ListWeapons();
		ListAttackForm();
		CheckStatus();
	}
}

void CD2XSpeedDlg::OnSelchangeWeapons() 
{
	CString str;
	m_ctlWeapon.GetWindowText(str);

	if (!m_wcr.IsSameWeapon(str))
	{
		// weapon changed
		m_wcr.SetWeapon(str);
		ListAttackForm();
		
		m_ctlAttackForms.GetWindowText(str);
		m_wcr.SetAttackForm(str);

		CheckStatus();
	}	
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_author.SetURL(_T("mailto:abinn32@yahoo.com"));
	m_cureteam.SetURL(_T("http://www.cure-team.com/vb/"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
