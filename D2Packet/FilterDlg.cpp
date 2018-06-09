// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2Packet.h"
#include "FilterDlg.h"
#include "PacketIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog


CFilterDlg::CFilterDlg(BOOL bReceived, CD2PacketFilter* pFilter, CWnd* pParent /*=NULL*/)
	: CDialog(CFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterDlg)
	//}}AFX_DATA_INIT
	m_bReceived = bReceived;
	m_pFilter = pFilter;
	ASSERT(m_pFilter);
}


void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterDlg)
	DDX_Control(pDX, IDC_BLOCKALL, m_wndBlockAll);
	DDX_Control(pDX, IDC_ALLOWALL, m_wndAllowAll);
	DDX_Control(pDX, IDC_BLOCK, m_wndBlock);
	DDX_Control(pDX, IDC_ALLOW, m_wndAllow);
	DDX_Control(pDX, IDC_LISTBLOCKED, m_wndBlockedList);
	DDX_Control(pDX, IDC_LISTALLOWED, m_wndAllowedList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterDlg)
	ON_BN_CLICKED(IDC_BLOCK, OnBlock)
	ON_BN_CLICKED(IDC_ALLOW, OnAllow)
	ON_BN_CLICKED(IDC_BLOCKALL, OnBlockall)
	ON_BN_CLICKED(IDC_ALLOWALL, OnAllowall)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTALLOWED, OnItemchangedListallowed)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTBLOCKED, OnItemchangedListblocked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg message handlers

void CFilterDlg::OnOK() 
{
	// TODO: Add extra validation here
	int i = 0;

	for (i = 0; i < m_wndAllowedList.GetItemCount(); i++)
		m_pFilter->Allow((BYTE)_tcstoul(m_wndAllowedList.GetItemText(i, 0), NULL, 16), m_bReceived);

	for (i = 0; i < m_wndBlockedList.GetItemCount(); i++)
		m_pFilter->Block((BYTE)_tcstoul(m_wndBlockedList.GetItemText(i, 0), NULL, 16), m_bReceived);

	CDialog::OnOK();
}

BOOL CFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndAllowedList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_wndAllowedList.InsertColumn(0, _T(""), LVCFMT_LEFT, 60);
	m_wndBlockedList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_wndBlockedList.InsertColumn(0, _T(""), LVCFMT_LEFT, 60);
	ASSERT(m_pFilter);

	CString s;
	for (int i = 0; i < 256; i++)
	{
		s.Format(_T("%02X"), i);
		if (m_pFilter->IsAllowed(i, m_bReceived))
			m_wndAllowedList.InsertItem(INT_MAX, s);
		else
			m_wndBlockedList.InsertItem(INT_MAX, s);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterDlg::OnBlock() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = NULL;
	while (pos = m_wndAllowedList.GetFirstSelectedItemPosition())
	{
		int n = m_wndAllowedList.GetNextSelectedItem(pos);
		m_wndBlockedList.InsertItem(INT_MAX, m_wndAllowedList.GetItemText(n, 0));
		m_wndAllowedList.DeleteItem(n);
	}
}

void CFilterDlg::OnAllow() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = NULL;
	while (pos = m_wndBlockedList.GetFirstSelectedItemPosition())
	{
		int n = m_wndBlockedList.GetNextSelectedItem(pos);
		m_wndAllowedList.InsertItem(INT_MAX, m_wndBlockedList.GetItemText(n, 0));
		m_wndBlockedList.DeleteItem(n);
	}
}

void CFilterDlg::OnBlockall() 
{
	// TODO: Add your control notification handler code here
	m_wndBlockedList.DeleteAllItems();
	m_wndAllowedList.DeleteAllItems();
	CString s;
	for (int i = 0; i < 256; i++)
	{
		s.Format(_T("%02X"), i);
		m_wndBlockedList.InsertItem(INT_MAX, s);
	}
}

void CFilterDlg::OnAllowall() 
{
	// TODO: Add your control notification handler code here
	m_wndBlockedList.DeleteAllItems();
	m_wndAllowedList.DeleteAllItems();
	CString s;
	for (int i = 0; i < 256; i++)
	{
		s.Format(_T("%02X"), i);
		m_wndAllowedList.InsertItem(INT_MAX, s);
	}	
}

void CFilterDlg::OnItemchangedListallowed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_wndBlock.EnableWindow(m_wndAllowedList.GetSelectedCount() > 0);
	*pResult = 0;
}

void CFilterDlg::OnItemchangedListblocked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_wndAllow.EnableWindow(m_wndBlockedList.GetSelectedCount() > 0);
	*pResult = 0;
}