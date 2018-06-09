// CharDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "CharDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharDlg dialog


CCharDlg::CCharDlg(LPCTSTR lpszCharName, int nCharClass, CWnd* pParent /*=NULL*/)
	: CDialog(CCharDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharDlg)
	m_sCharName = CString(lpszCharName);
	m_nCharClass = nCharClass;
	//}}AFX_DATA_INIT
}


void CCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharDlg)
	DDX_Control(pDX, IDC_CHARCLASS, m_wndCharClass);
	DDX_Control(pDX, IDC_CHARNAME, m_wndCharName);
	DDX_Text(pDX, IDC_CHARNAME, m_sCharName);
	DDV_MaxChars(pDX, m_sCharName, 15);
	DDX_CBIndex(pDX, IDC_CHARCLASS, m_nCharClass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharDlg, CDialog)
	//{{AFX_MSG_MAP(CCharDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharDlg message handlers

void CCharDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sCharName.TrimLeft();
	m_sCharName.TrimRight();
	if (m_sCharName.IsEmpty())
	{
		MessageBox(_T("无效角色名。"), _T("无效输入"), MB_ICONEXCLAMATION);
		m_wndCharName.SetFocus();
		return;
	}
	
	EndDialog(IDOK);
}

CString CCharDlg::GetCharName() const
{
	return m_sCharName;
}

BYTE CCharDlg::GetCharClass() const
{
	return (BYTE)m_nCharClass;
}

BOOL CCharDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndCharClass.EnableWindow(m_sCharName.IsEmpty());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
