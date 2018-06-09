// ModDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShopbotCfg.h"
#include "ModDlg.h"
#include "D2ModParser.h"
#include "SkillTypeDlg.h"
#include "AssistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { LIST_NONE = 0, LIST_CLASS, LIST_TAB, LIST_SKILL };
/////////////////////////////////////////////////////////////////////////////
// CModDlg dialog


CModDlg::CModDlg(LPCITEMMOD pMod, CWnd* pParent /*=NULL*/)
	: CDialog(CModDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModDlg)
	m_dwVal1 = 0;
	m_dwVal2 = 0;
	m_dwVal3 = 0;
	//}}AFX_DATA_INIT
	if (pMod)
		::memcpy(&m_mod, pMod, sizeof(ITEMMOD));
	else
		::memset(&m_mod, 0, sizeof(ITEMMOD));

	m_nAssistType = LIST_NONE;
}

void CModDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModDlg)
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Control(pDX, IDC_PREVIEW, m_wndPreview);
	DDX_Control(pDX, IDC_HELP2, m_wndHelp2);
	DDX_Control(pDX, IDC_HELP1, m_wndHelp1);
	DDX_Control(pDX, IDC_VAL3, m_wndVal3);
	DDX_Control(pDX, IDC_VAL2, m_wndVal2);
	DDX_Control(pDX, IDC_VAL1, m_wndVal1);
	DDX_Control(pDX, IDC_COMBO1, m_wndMods);
	DDX_Text(pDX, IDC_VAL1, m_dwVal1);
	DDV_MinMaxDWord(pDX, m_dwVal1, 0, 65535);
	DDX_Text(pDX, IDC_VAL2, m_dwVal2);
	DDV_MinMaxDWord(pDX, m_dwVal2, 0, 65535);
	DDX_Text(pDX, IDC_VAL3, m_dwVal3);
	DDV_MinMaxDWord(pDX, m_dwVal3, 0, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModDlg, CDialog)
	//{{AFX_MSG_MAP(CModDlg)
	ON_BN_CLICKED(IDC_HELP1, OnHelp1)
	ON_BN_CLICKED(IDC_HELP2, OnHelp2)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_EN_CHANGE(IDC_VAL1, OnChangeVals)
	ON_EN_CHANGE(IDC_VAL2, OnChangeVals)
	ON_EN_CHANGE(IDC_VAL3, OnChangeVals)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModDlg message handlers

BOOL CModDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_wndHelp1.SetIcon(IDI_INFO);
	m_wndHelp1.SetTooltipText(_T("Mod Data Assistant"));
	m_wndHelp2.SetIcon(IDI_INFO);
	m_wndHelp2.SetTooltipText(_T("Mod Data Assistant"));	
	m_wndOK.SetIcon(IDI_YES);
	m_wndOK.SetTooltipText(_T("Exit and save changes"));
	m_wndCancel.SetIcon(IDI_NO);
	m_wndCancel.SetTooltipText(_T("Exit without saving any changes"));

	m_wndMods.ResetContent();
	int i = 0;
	for (i = 0; MOD_TABLE[i].dwModID < 511; i++)
	{
		DWORD dwModID = MOD_TABLE[i].dwModID;
		CString sMod = MOD_TABLE[i].szDesc;
		sMod.Replace("%%", "%");
		const int IDX = m_wndMods.AddString(sMod);
		m_wndMods.SetItemData(IDX, dwModID);
	}

	for (i = 0; i < m_wndMods.GetCount(); i++)
	{
		if (m_wndMods.GetItemData(i) == m_mod.dwModID)
		{
			m_wndMods.SetCurSel(i);
			break;
		}
	}

	m_dwVal1 = m_mod.aValues[0];
	m_dwVal2 = m_mod.aValues[1];
	m_dwVal3 = m_mod.aValues[2];
	if (m_mod.dwModID == 48 || m_mod.dwModID == 50 || m_mod.dwModID == 54)
		m_dwVal1 -= m_dwVal2;

	if (m_mod.dwModID == 91)
	{
		// requirements -x%
		m_dwVal1 = 0 - m_dwVal1;
	}

	if (m_mod.dwModID == 112)
	{
		// hit causes monster to flee x%, 0x80 based
		m_dwVal1 = (DWORD)((double)m_mod.aValues[0] * 100.0 / 128.0);
	}

	UpdateData(FALSE);

	CString sMod;
	int nIdx = m_wndMods.GetCurSel();
	m_wndMods.GetLBText(nIdx, sMod);
	m_mod.iCount = GetModVars(sMod);
	m_wndVal1.EnableWindow(sMod.Find(_T("(x)")) != -1);
	m_wndVal2.EnableWindow(m_mod.iCount > 1);
	m_wndVal3.EnableWindow(m_mod.iCount > 2);
	CheckStats();
	Preview();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModDlg::OnHelp1() 
{
	// TODO: Add your control notification handler code here
	if (m_mod.dwModID == 107)
	{
		// skill
		CSkillTypeDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			m_dwVal1 = dlg.GetData();
			UpdateData(FALSE);
			OnChangeVals();
		}
	}
	else if (m_mod.dwModID == 188)
	{
		// tab
		CAssistDlg dlg(ASSIST_TAB);
		if (dlg.DoModal() == IDOK)
		{
			m_dwVal1 = dlg.GetData();
			UpdateData(FALSE);
			OnChangeVals();
		}
	}
	else if (m_mod.dwModID == 83)
	{
		// class
		CAssistDlg dlg(ASSIST_CLASS);
		if (dlg.DoModal() == IDOK)
		{
			m_dwVal1 = dlg.GetData();
			UpdateData(FALSE);
			OnChangeVals();
		}
	}
}

void CModDlg::OnHelp2() 
{
	// TODO: Add your control notification handler code here
	CSkillTypeDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	m_dwVal2 = dlg.GetData();
	UpdateData(FALSE);
	OnChangeVals();
}

void CModDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	::memset(&m_mod, 0, sizeof(m_mod));
	const int IDX = m_wndMods.GetCurSel();
	if (IDX >= 0)
	{
		m_mod.dwModID = m_wndMods.GetItemData(IDX);
		m_mod.aValues[0] = 1;
		m_dwVal1 = 1;

		if (m_mod.dwModID == 91)
		{
			// requirements -x%
			m_mod.aValues[0] = -1;
		}

		m_dwVal2 = 0;
		m_dwVal3 = 0;
		CString sMod;
		m_wndMods.GetLBText(IDX, sMod);
		m_mod.iCount = GetModVars(sMod);
		UpdateData(FALSE);

		m_wndVal1.EnableWindow(sMod.Find(_T("(x)")) != -1);
		m_wndVal2.EnableWindow(m_mod.iCount > 1);
		m_wndVal3.EnableWindow(m_mod.iCount > 2);
	}

	CheckStats();
	Preview();
}

void CModDlg::Preview()
{
	CString sPreview;
	m_wndOK.EnableWindow(GetItemModDesc(&m_mod, sPreview));
	m_wndPreview.SetWindowText(sPreview);

}

void CModDlg::OnChangeVals() 
{	
	// TODO: Add your control notification handler code here
	CString sVal;
	GetDlgItemText(IDC_VAL1, sVal);
	m_dwVal1 = _tcstoul(sVal, NULL, 10);
	GetDlgItemText(IDC_VAL2, sVal);
	m_dwVal2 = _tcstoul(sVal, NULL, 10);
	GetDlgItemText(IDC_VAL3, sVal);
	m_dwVal3 = _tcstoul(sVal, NULL, 10);

	m_mod.aValues[0] = m_dwVal1;
	m_mod.aValues[1] = m_dwVal2;
	m_mod.aValues[2] = m_dwVal3;
	m_mod.aValues[3] = m_dwVal3;

	if (m_mod.dwModID == 48 || m_mod.dwModID == 50 || m_mod.dwModID == 54)
		m_mod.aValues[0] += m_mod.aValues[1];

	if (m_mod.dwModID == 91)
	{
		// requirements -x%
		m_mod.aValues[0] = 0 - m_mod.aValues[0];
	}

	if (m_mod.dwModID == 112)
	{
		// hit causes monster to flee x%, 0x80 based
		m_mod.aValues[0] = (DWORD)((double)m_dwVal1 * 128.0 / 100.0);
	}

	Preview();
}

void CModDlg::CheckStats()
{
	m_nAssistType = LIST_NONE;
	m_wndHelp1.ShowWindow(SW_HIDE);
	m_wndHelp2.ShowWindow(SW_HIDE);
	switch (m_mod.dwModID)
	{
	case 195:
	case 198:
	case 201:
	case 204:
		// list skill names
		m_nAssistType = LIST_SKILL;
		m_wndHelp2.ShowWindow(SW_SHOW);
		break;
		
	case 107:
		m_nAssistType = LIST_SKILL;
		m_wndHelp1.ShowWindow(SW_SHOW);
		break;

	case 188:
		// list skill trees
		m_nAssistType = LIST_TAB;
		m_wndHelp1.ShowWindow(SW_SHOW);
		break;

	case 83:
		// list char classes
		m_nAssistType = LIST_CLASS;
		m_wndHelp1.ShowWindow(SW_SHOW);
		break;

	default:
		break;
	}
}

void CModDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	m_wndPreview.GetWindowText(str);
	if (str.IsEmpty())
	{
		MessageBox(_T("Invalid Mod Settings"), _T("Mod Settings"), MB_ICONEXCLAMATION);
		return;
	}
	EndDialog(IDOK);
}

LPCITEMMOD CModDlg::GetMod() const
{
	return &m_mod;
}
