// PacketIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2Packet.h"
#include "PacketIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketIDDlg dialog


CPacketIDDlg::CPacketIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPacketIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPacketIDDlg)
	m_sPacketID = _T("00");
	//}}AFX_DATA_INIT
}


void CPacketIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPacketIDDlg)
	DDX_Control(pDX, IDC_EDIT1, m_wndEdit);
	DDX_Text(pDX, IDC_EDIT1, m_sPacketID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPacketIDDlg, CDialog)
	//{{AFX_MSG_MAP(CPacketIDDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketIDDlg message handlers

BYTE CPacketIDDlg::GetPacketID() const
{
	return (BYTE)_tcstoul(m_sPacketID, NULL, 16);
}

void CPacketIDDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sPacketID.MakeUpper();
	m_sPacketID.TrimLeft();
	m_sPacketID.TrimRight();
	if (m_sPacketID.IsEmpty())
	{
		MessageBox(_T("Packet ID cannot be empty."));
		m_wndEdit.SetFocus();
		m_wndEdit.SetSel(0, -1);
		return;
	}

	for (int i = 0; i < m_sPacketID.GetLength(); i++)
	{
		if ((m_sPacketID[i] >= _T('0') && m_sPacketID[i] <= _T('9')) || (m_sPacketID[i] >= _T('A') && m_sPacketID[i] <= _T('F')))
		{
		}
		else
		{
			MessageBox(_T("Invalid packet ID, must be 00-FF."));
			m_wndEdit.SetFocus();
			m_wndEdit.SetSel(0, -1);
			return;
		}
	}

	EndDialog(IDOK);
}
