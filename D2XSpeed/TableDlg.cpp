// TableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2XSpeed.h"
#include "TableDlg.h"
//#include "SortListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableDlg dialog


CTableDlg::CTableDlg(WCRelation* pWcr, CWnd* pParent /*=NULL*/)
	: m_pWcr(pWcr), CDialog(CTableDlg::IDD, pParent)
	, m_strCurSpeed(_T(""))
{
	//{{AFX_DATA_INIT(CTableDlg)
	m_strCurIas = _T("");
	m_strBase = _T("");
	m_strWsm = _T("");
	m_strCurSIas = _T("");
	//}}AFX_DATA_INIT
}


void CTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableDlg)
	DDX_Control(pDX, IDC_LIST1, m_ctlTable);
	DDX_Text(pDX, IDC_CURIAS, m_strCurIas);
	DDX_Text(pDX, IDC_CURBASE, m_strBase);
	DDX_Text(pDX, IDC_CURWSM, m_strWsm);
	DDX_Text(pDX, IDC_CURSIAS, m_strCurSIas);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_CURSPEED, m_strCurSpeed);
}


BEGIN_MESSAGE_MAP(CTableDlg, CDialog)
	//{{AFX_MSG_MAP(CTableDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableDlg message handlers

BOOL CTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctlTable.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_strCurIas.Format("%d", m_pWcr->GetIas());
	m_strBase.Format("%.1f", m_pWcr->GetBase());

	if (m_pWcr->GetAttackForm() != AF_CAST)
	{
		m_strCurSIas.Format("%d", m_pWcr->GetSIas());
        m_strWsm.Format("%d", m_pWcr->GetWeapon().GetWsm());
	}
	else
	{
		m_strCurSIas = _T("N/A");
		m_strWsm = _T("N/A");
	}
	
	UpdateData(FALSE);
	ShowTable();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTableDlg::ShowTable()
{
	// calculate the total width of table
	CRect rect;
	m_ctlTable.GetWindowRect(&rect);
	int nWidth = rect.Width();

	// insert columns to table
	m_ctlTable.InsertColumn(0, "Item IAS (%)",LVCFMT_LEFT ,nWidth / 2 - 10);
	m_ctlTable.InsertColumn(1, "Speed (frames)",LVCFMT_LEFT ,nWidth / 2 - 10);

	IasUnit tableArray[MAX_SPEEDTABLE];
	int nCount = m_pWcr->GetTable(tableArray);

	CString strIas, strSpeed;
	int nSpace = 0;

	int nCurSet = -1;

	for (int i = 0; i < nCount; i++)
	{
		if ((i == nCount - 1 
			|| tableArray[i + 1].m_nIAS > m_pWcr->GetIas())
			&& nCurSet == -1)
		{
			strIas.Format("%d (current)", tableArray[i].m_nIAS);
			m_strCurSpeed.Format(_T(" %d"), tableArray[i].m_nSpeed);
			nCurSet = i;
			UpdateData(FALSE);
		}
		else
			strIas.Format("%d", tableArray[i].m_nIAS);

		// display IAS
		m_ctlTable.InsertItem(i, strIas);

		strSpeed.Format("%d", tableArray[i].m_nSpeed);

		// display speed
		m_ctlTable.SetItemText(i, 1, strSpeed);
	}
	m_ctlTable.SetHotItem(nCurSet);
}
