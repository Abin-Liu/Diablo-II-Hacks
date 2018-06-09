// SkillTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShopbotCfg.h"
#include "SkillTypeDlg.h"
#include "AssistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillTypeDlg dialog


CSkillTypeDlg::CSkillTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkillTypeDlg)
	m_nSkillType = 0;
	//}}AFX_DATA_INIT
	m_dwData = 0;
}


void CSkillTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillTypeDlg)
	DDX_Radio(pDX, IDC_SKILLTYPE, m_nSkillType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkillTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillTypeDlg)
	ON_BN_CLICKED(IDC_SKILLTYPE, OnSkilltype)
	ON_BN_CLICKED(IDC_RADIO2, OnSkilltype)
	ON_BN_CLICKED(IDC_RADIO3, OnSkilltype)
	ON_BN_CLICKED(IDC_RADIO4, OnSkilltype)
	ON_BN_CLICKED(IDC_RADIO5, OnSkilltype)
	ON_BN_CLICKED(IDC_RADIO6, OnSkilltype)
	ON_BN_CLICKED(IDC_RADIO7, OnSkilltype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillTypeDlg message handlers

void CSkillTypeDlg::OnSkilltype() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CAssistDlg dlg(m_nSkillType + ASSIST_AMA_SKILL);
	if (dlg.DoModal() != IDOK)
		EndDialog(IDCANCEL);
	m_dwData = dlg.GetData();
	EndDialog(IDOK);
}

DWORD CSkillTypeDlg::GetData() const
{
	return m_dwData;
}
