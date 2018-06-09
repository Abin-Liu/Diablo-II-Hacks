// D2ClassWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2ClassWizard.h"
#include "D2ClassWizardDlg.h"

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
// CD2ClassWizardDlg dialog

CD2ClassWizardDlg::CD2ClassWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD2ClassWizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CD2ClassWizardDlg)
	m_sClassName = _T("");
	m_sBaseClass = _T("CD2Abstract");
	m_sCppFile = _T("");
	m_sHeaderFile = _T("");
	m_nMethod = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2ClassWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD2ClassWizardDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Text(pDX, IDC_CLASSNAME, m_sClassName);
	DDX_CBString(pDX, IDC_BASECLASS, m_sBaseClass);
	DDX_Text(pDX, IDC_CPPFILE, m_sCppFile);
	DDX_Text(pDX, IDC_HEADERFILE, m_sHeaderFile);
	DDX_CBIndex(pDX, IDC_METHOD, m_nMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CD2ClassWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CD2ClassWizardDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_CLASSNAME, OnChangeClassname)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2ClassWizardDlg message handlers

BOOL CD2ClassWizardDlg::OnInitDialog()
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
	m_wndList.InsertColumn(0, _T("names"), LVCFMT_LEFT, 300);
	m_wndList.InsertItem(INT_MAX, _T("OnGamePacketBeforeSent"));
	m_wndList.InsertItem(INT_MAX, _T("OnGameTimerTick"));
	m_wndList.InsertItem(INT_MAX, _T("InitAttributes"));
	m_wndList.InsertItem(INT_MAX, _T("SetD2MessageProc"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CD2ClassWizardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CD2ClassWizardDlg::OnPaint() 
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
HCURSOR CD2ClassWizardDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CD2ClassWizardDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_sClassName.IsEmpty())
	{
		MessageBox(_T("Class name cannot be empty."), _T("Invalid Input"), MB_ICONEXCLAMATION);
		GetDlgItem(IDC_CLASSNAME)->SetFocus();
		return;
	}

	if (m_sBaseClass.IsEmpty())
	{
		MessageBox(_T("Base class cannot be empty"), _T("Invalid Input"), MB_ICONEXCLAMATION);
		GetDlgItem(IDC_BASECLASS)->SetFocus();
		return;
	}

	// Check h and cpp file existance
	if (::GetFileAttributes(CString(_T(".\\")) + m_sHeaderFile) != -1)
	{
		MessageBox(CString(_T('\"')) + m_sHeaderFile + _T("\" already exists"), _T("File Creation Fail"), MB_ICONEXCLAMATION);
		return;
	}

	if (::GetFileAttributes(CString(_T(".\\")) + m_sCppFile) != -1)
	{
		MessageBox(CString(_T('\"')) + m_sCppFile + _T("\" already exists"), _T("File Creation Fail"), MB_ICONEXCLAMATION);
		return;
	}

	// create h and cpp files
	CStdioFile h;
	if (!h.Open(CString(_T(".\\")) + m_sHeaderFile, CFile::modeWrite | CFile::modeCreate))
	{
		CString sErr;
		sErr.Format(_T("Failed to open \"%s\" for write."), m_sHeaderFile);
		MessageBox(sErr,  _T("File Open Fail"), MB_ICONEXCLAMATION);
		return;
	}

	CStdioFile cpp;
	if (!cpp.Open(CString(_T(".\\")) + m_sCppFile, CFile::modeWrite | CFile::modeCreate))
	{
		CString sErr;
		sErr.Format(_T("Failed to open \"%s\" for write."), m_sCppFile);
		MessageBox(sErr,  _T("File Open Fail"), MB_ICONEXCLAMATION);
		h.Close();
		return;
	}

	// virtual functions to implement:
	BYTE aFlags[4] = { 0 };
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int n = m_wndList.GetNextSelectedItem(pos);
		aFlags[n] = 1;
	}

	// Write header file	
	CString sBaseHeader = m_sBaseClass;
	if (sBaseHeader.GetLength() > 1)
	{
		if (sBaseHeader[0] == _T('c') || sBaseHeader[0] == _T('C'))
			sBaseHeader.Delete(0);
	}
	sBaseHeader += _T(".h");
	CString sMethod = _T("public");
	if (m_nMethod == 1)
		sMethod = _T("protected");
	else if (m_nMethod == 2)
		sMethod = _T("private");
	
	CString sLine, str;	
	
	h.WriteString(_T("//////////////////////////////////////////////////////////////////////\n"));
	
	sLine.Format(_T("// %s: interface of the %s class.\n"), m_sHeaderFile, m_sClassName);
	h.WriteString(sLine);
	h.WriteString(_T("//////////////////////////////////////////////////////////////////////\n\n"));

	str.Format(_T("__%s__"), m_sHeaderFile);
	str.MakeUpper();
	str.Replace(_T('.'), _T('_'));
	sLine.Format(_T("#ifndef %s\n"), str);
	h.WriteString(sLine);

	sLine.Format(_T("#define %s\n\n"), str);
	h.WriteString(sLine);

	sLine.Format(_T("#include \"%s\"\n\n"), sBaseHeader);
	h.WriteString(sLine);

	h.WriteString(_T("///////////////////////////////////////////////////////////////////////\n"));
	sLine.Format(_T("// %s Class Definition\n"), m_sClassName);
	h.WriteString(sLine);
	h.WriteString(_T("///////////////////////////////////////////////////////////////////////\n"));

	sLine.Format(_T("class %s : %s %s\n{\npublic:\n\n"), m_sClassName, sMethod, m_sBaseClass);
	h.WriteString(sLine);

	h.WriteString(_T("\t///////////////////////////////////////////////////////////////////////\n\t// Constructor(s) & destructor\n\t///////////////////////////////////////////////////////////////////////\n"));

	sLine.Format(_T("\t%s();\n"), m_sClassName);
	h.WriteString(sLine);

	sLine.Format(_T("\tvirtual ~%s();\n\n"), m_sClassName);
	h.WriteString(sLine);

	h.WriteString(_T("\t///////////////////////////////////////////////////////////////////////\n\t// Client Entries\n\t///////////////////////////////////////////////////////////////////////\n"));
	h.WriteString(_T("\tvirtual void OnGameJoin(const THISGAMESTRUCT* pGame);\n"));
	h.WriteString(_T("\tvirtual void OnGameLeave();\n"));
	h.WriteString(_T("\tvirtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);\n"));
	
	if (aFlags[0])
		h.WriteString(_T("\tvirtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen);\n"));
	
	if (aFlags[1])
		h.WriteString(_T("\tvirtual void OnGameTimerTick();\n"));

	h.WriteString(_T("\n\t///////////////////////////////////////////////////////////////////////\n\t// Public Attributes\n\t///////////////////////////////////////////////////////////////////////\n\n"));
	h.WriteString(_T("\t///////////////////////////////////////////////////////////////////////\n\t// Public Operations\n\t///////////////////////////////////////////////////////////////////////\n"));
	
	if (aFlags[2])
		h.WriteString(_T("\tvirtual void InitAttributes(); // Clean all attributes\n"));

	if (aFlags[3])
		h.WriteString(_T("\tvirtual void SetD2MessageProc(D2MESSAGEPROC lpfn, LPVOID lpData = NULL);\n"));

	h.WriteString(_T("\nprotected:\n\n"));
	h.WriteString(_T("\t///////////////////////////////////////////////////////////////////////\n\t// Overrides\n\t///////////////////////////////////////////////////////////////////////\n\n"));
	h.WriteString(_T("\t///////////////////////////////////////////////////////////////////////\n\t// Protected Operations\n\t///////////////////////////////////////////////////////////////////////\n\n"));
	h.WriteString(_T("\t///////////////////////////////////////////////////////////////////////\n\t// Protected Member Data\n\t///////////////////////////////////////////////////////////////////////\n\n"));
	h.WriteString(_T("};\n\n"));

	sLine.Format(_T("#endif // %s"), str);
	h.WriteString(sLine);
	h.Close();

	// write cpp file

	cpp.WriteString(_T("//////////////////////////////////////////////////////////////////////\n"));

	sLine.Format(_T("// %s: implementation of the %s class.\n"), m_sCppFile, m_sClassName);
	cpp.WriteString(sLine);
	cpp.WriteString(_T("//////////////////////////////////////////////////////////////////////\n\n"));
	
	//cpp.WriteString(_T("#include \"StdAfx.h\"\n"));
	sLine.Format(_T("#include \"%s\"\n\n"), m_sHeaderFile);
	cpp.WriteString(sLine);

	sLine.Format(_T("%s::%s()\n{\n\t"), m_sClassName, m_sClassName);
	cpp.WriteString(sLine);

	if (aFlags[2])
		cpp.WriteString(_T("InitAttributes(); "));

	cpp.WriteString(_T("// Initialization\n}\n\n"));

	sLine.Format(_T("%s::~%s()\n{\n}\n\n"), m_sClassName, m_sClassName);
	cpp.WriteString(sLine);	

	sLine.Format(_T("void %s::OnGameJoin(const THISGAMESTRUCT* pGame)\n{\n\t%s::OnGameJoin(pGame); // Always call base class's method first!!!\n\n"), m_sClassName, m_sBaseClass);
	cpp.WriteString(sLine);

	if (aFlags[2])
		cpp.WriteString(_T("\tInitAttributes(); // Initialize attributes upon new game.\n\n"));

	cpp.WriteString(_T("\t// TODO: Add your implementation here.\n\t\n}\n\n"));

	sLine.Format(_T("void %s::OnGameLeave()\n{\n\t%s::OnGameLeave(); // Always call base class's method first!!!\n\n"), m_sClassName, m_sBaseClass);
	cpp.WriteString(sLine);

	if (aFlags[2])
		cpp.WriteString(_T("\tInitAttributes(); // Initialize attributes upon leave.\n\n"));

	cpp.WriteString(_T("\t// TODO: Add your implementation here.\n\t\n}\n\n"));

	sLine.Format(_T("void %s::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)\n{\n\t%s::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!\n\tif (aPacket == NULL || aLen == 0)\n\t\treturn; // Verify packet\n\n\t// TODO: Add your implementation here.\n\t\n}\n\n"), m_sClassName, m_sBaseClass);
	cpp.WriteString(sLine);
	

	if (aFlags[0])
	{
		sLine.Format(_T("void %s::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)\n{\n\t%s::OnGamePacketBeforeSent(aPacket, aLen); // Always call base class's method first!!!\n\tif (aPacket == NULL || aLen == 0)\n\t\treturn; // Verify packet\n\n\t// TODO: Add your implementation here.\n\t\n}\n\n"), m_sClassName, m_sBaseClass);
		cpp.WriteString(sLine);
	}

	if (aFlags[1])
	{
		sLine.Format(_T("void %s::OnGameTimerTick()\n{\n\t%s::OnGameTimerTick(); // Always call base class's method first!!!\n\n\t// TODO: Add your implementation here.\n\t\n}\n\n"), m_sClassName, m_sBaseClass);
		cpp.WriteString(sLine);
	}

	if (aFlags[2])
	{
		sLine.Format(_T("void %s::InitAttributes()\n{\n\t%s::InitAttributes(); // Always call base class's method first!!!\n\n\t// TODO: Add extra initialization here.\n\t\n}\n\n"), m_sClassName, m_sBaseClass);
		cpp.WriteString(sLine);
	}

	if (aFlags[3])
	{
		sLine.Format(_T("void %s::SetD2MessageProc(D2MESSAGEPROC lpfn, LPVOID lpData/* = NULL*/)\n{\n\t%s::SetD2MessageProc(lpfn, lpData); // Always call base class's method first!!!\n\n\t// TODO: Add extra implementation here.\n\t\n}\n\n"), m_sClassName, m_sBaseClass);
		cpp.WriteString(sLine);
	}

	cpp.Close();

	sLine.Format(_T("C++ source code generated successfully:\n\n%s\n%s"), m_sHeaderFile, m_sCppFile);
	MessageBox(sLine, _T("Code Generated"), MB_ICONINFORMATION);
	CDialog::EndDialog(IDOK);
}

void CD2ClassWizardDlg::OnChangeClassname() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString sClass;
	GetDlgItemText(IDC_CLASSNAME, sClass);

	if (sClass.GetLength() > 1)
	{
		if (sClass[0] == _T('c') || sClass[0] == _T('C'))
			sClass.Delete(0);
	}

	if (!sClass.IsEmpty())
	{
		SetDlgItemText(IDC_HEADERFILE, sClass + _T(".h"));
		SetDlgItemText(IDC_CPPFILE, sClass + _T(".cpp"));
	}
	else
	{
		SetDlgItemText(IDC_HEADERFILE, sClass + _T(""));
		SetDlgItemText(IDC_CPPFILE, sClass + _T(""));
	}
}

BOOL CD2ClassWizardDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_CHAR && pMsg->wParam != VK_BACK)
	{
		CString s((TCHAR)pMsg->wParam);
		s.MakeUpper();

		if ((s[0] < _T('A') || s[0] > _T('Z')) && s[0] != _T('_'))
		{
			CString str;
			GetDlgItemText(IDC_CLASSNAME, str);
			if (str.IsEmpty() || s[0] < _T('0') || s[0] > _T('9'))
				return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CD2ClassWizardDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}
