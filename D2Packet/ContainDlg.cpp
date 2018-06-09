// ContainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2Packet.h"
#include "ContainDlg.h"
#include "StrSplit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContainDlg dialog


CContainDlg::CContainDlg(LPCTSTR lpszContents, CWnd* pParent /*=NULL*/)
	: CDialog(CContainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContainDlg)
	m_sContents = CString(lpszContents);
	//}}AFX_DATA_INIT
}


void CContainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContainDlg)
	DDX_Control(pDX, IDC_EDIT1, m_wndEdit);
	DDX_Text(pDX, IDC_EDIT1, m_sContents);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContainDlg, CDialog)
	//{{AFX_MSG_MAP(CContainDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContainDlg message handlers

void CContainDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sContents.MakeUpper();
	m_sContents.TrimLeft();
	m_sContents.TrimRight();

	if (m_sContents.IsEmpty())
		return;

	while (m_sContents.Find(_T("  ")) != -1)
		m_sContents.Replace(_T("  "), _T(" "));

	CString str(_T("Packets that do not contain the following bytes will be removed from the list:\n\n"));
	str += m_sContents + _T("\n\nThis operation removes all unmatched packets but does not block them from being further received.\nDo you want to continue?");

	if (MessageBox(str, _T("Contents Search"), MB_YESNO) == IDYES)
		EndDialog(IDOK);
}

CString CContainDlg::GetString() const
{
	return m_sContents;
}
