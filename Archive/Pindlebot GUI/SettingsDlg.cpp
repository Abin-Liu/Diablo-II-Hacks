// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "SettingsDlg.h"
#include "ActionListDlg.h"
#include "ComboDlg.h"
#include "PindleBonusDef.h"
#include "CharDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog


CSettingsDlg::CSettingsDlg(LPPBOTCONFIG pConfig, CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	m_pConfig = pConfig;
	ASSERT(m_pConfig);

	//{{AFX_DATA_INIT(CSettingsDlg)
	m_iPetHP = pConfig->iPetPotionHP;
	m_bQuitIfPetDie = pConfig->bQuitOnPetDeath;
	m_nStay = pConfig->nLocationType;
	m_nAmmoType = pConfig->nAmmoType;
	m_iMalahHP = pConfig->iMalahHP;
	m_iMalahMana = pConfig->iMalahMana;
	m_iPotionHP = pConfig->iPotionHP;
	m_iPotionMana = pConfig->iPotionMana;
	m_iQuitHP = pConfig->iQuitHP;
	m_iQuitMana = pConfig->iQuitMana;
	m_iRepairPercent = pConfig->iRepairPercent;
	m_bResMerc = pConfig->bResurrectPet;
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDC_ONCORPSE, m_wndOnCorpse);
	DDX_Control(pDX, IDC_ONPINDLE, m_wndOnPindle);
	DDX_Control(pDX, IDC_INRED, m_wndInRed);
	DDX_Control(pDX, IDC_BEFORERED, m_wndBeforeRed);
	DDX_Control(pDX, IDC_EDITCOMBO, m_wndEditCombo);
	DDX_Control(pDX, IDC_DELCOMBO, m_wndDelCombo);
	DDX_Control(pDX, IDC_ADDCOMBO, m_wndAddCombo);
	DDX_Control(pDX, IDC_DEADLYCOMBO, m_wndCombo);
	DDX_Text(pDX, IDC_PETHP, m_iPetHP);
	DDV_MinMaxByte(pDX, m_iPetHP, 0, 100);
	DDX_Check(pDX, IDC_QUITONPETDIE, m_bQuitIfPetDie);
	DDX_Radio(pDX, IDC_STAYNORMAL, m_nStay);
	DDX_Radio(pDX, IDC_WEAPONTYPE, m_nAmmoType);
	DDX_Text(pDX, IDC_MALAHHP, m_iMalahHP);
	DDV_MinMaxByte(pDX, m_iMalahHP, 0, 100);
	DDX_Text(pDX, IDC_MALAHMANA, m_iMalahMana);
	DDV_MinMaxByte(pDX, m_iMalahMana, 0, 100);
	DDX_Text(pDX, IDC_POTIONHP, m_iPotionHP);
	DDV_MinMaxByte(pDX, m_iPotionHP, 0, 100);
	DDX_Text(pDX, IDC_POTIONMANA, m_iPotionMana);
	DDV_MinMaxByte(pDX, m_iPotionMana, 0, 100);
	DDX_Text(pDX, IDC_QUITHP, m_iQuitHP);
	DDV_MinMaxByte(pDX, m_iQuitHP, 0, 100);
	DDX_Text(pDX, IDC_QUITMANA, m_iQuitMana);
	DDV_MinMaxByte(pDX, m_iQuitMana, 0, 100);
	DDX_Text(pDX, IDC_REPAIR, m_iRepairPercent);
	DDV_MinMaxByte(pDX, m_iRepairPercent, 0, 100);
	DDX_Check(pDX, IDC_RESMERC, m_bResMerc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_BN_CLICKED(IDC_BEFORERED, OnBeforered)
	ON_BN_CLICKED(IDC_INRED, OnInred)
	ON_BN_CLICKED(IDC_ONPINDLE, OnOnpindle)
	ON_BN_CLICKED(IDC_ONCORPSE, OnOncorpse)
	ON_BN_CLICKED(IDC_ADDCOMBO, OnAddcombo)
	ON_BN_CLICKED(IDC_DELCOMBO, OnDelcombo)
	ON_BN_CLICKED(IDC_EDITCOMBO, OnEditcombo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DEADLYCOMBO, OnItemchangedDeadlycombo)
	ON_NOTIFY(NM_DBLCLK, IDC_DEADLYCOMBO, OnDblclkDeadlycombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndBeforeRed.SetBitmaps(IDB_BEFORERED, RGB(255, 0, 255));
	m_wndInRed.SetBitmaps(IDB_INRED, RGB(255, 0, 255));
	m_wndOnPindle.SetBitmaps(IDB_ONPINDLE, RGB(255, 0, 255));
	m_wndOnCorpse.SetBitmaps(IDB_ONCORPSE, RGB(255, 0, 255));

	m_wndBeforeRed.SetTooltipText(_T("进入红门之前使用的技能"));
	m_wndInRed.SetTooltipText(_T("进入红门之后使用的技能"));
	m_wndOnPindle.SetTooltipText(_T("用来杀死Pindleskin的技能"));
	m_wndOnCorpse.SetTooltipText(_T("地上出现尸体时使用的技能"));

	m_wndAddCombo.SetIcon(IDI_ADD);
	m_wndDelCombo.SetIcon(IDI_DELETE);
	m_wndEditCombo.SetIcon(IDI_EDIT);
	m_wndOK.SetIcon(IDI_YES);
	m_wndCancel.SetIcon(IDI_NO);

	m_wndAddCombo.SetTooltipText(_T("添加新组合"));
	m_wndDelCombo.SetTooltipText(_T("删除选定的组合"));
	m_wndEditCombo.SetTooltipText(_T("更改选定的组合"));
	m_wndOK.SetTooltipText(_T("完成"));
	m_wndCancel.SetTooltipText(_T("取消"));


	m_wndOnCorpse.EnableWindow(m_pConfig->iCharClass == CCD_BARBARIAN || m_pConfig->iCharClass == CCD_NECROMANCER);
	m_wndBeforeRed.EnableWindow(m_pConfig->iCharClass != CCD_BARBARIAN);

	CString str;
	str.Format(_T("参数设置 - %s （%s）"), m_pConfig->szCharName, CHARCLASSNAME[m_pConfig->iCharClass]);
	SetWindowText(str);

	m_wndCombo.SetColumnHeader(_T("属性1, 150;属性2,150"));
	for (BYTE i = 0; i < m_pConfig->iComboCount; i++)
		ListCombo(INT_MAX, m_pConfig->aAvoidCombos[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsDlg::OnBeforered() 
{
	// TODO: Add your control notification handler code here
	CActionListDlg dlg(PB_ACTION_BEFORERED, m_pConfig->aBeforeRed, &m_pConfig->iBeforeRedCount, m_pConfig->iCharClass, this);
	dlg.DoModal();
}

void CSettingsDlg::OnInred() 
{
	// TODO: Add your control notification handler code here
	CActionListDlg dlg(PB_ACTION_INRED, m_pConfig->aInRed, &m_pConfig->iInRedCount, m_pConfig->iCharClass, this);
	dlg.DoModal();
}

void CSettingsDlg::OnOnpindle() 
{
	// TODO: Add your control notification handler code here
	CActionListDlg dlg(PB_ACTION_ONPINDLE, m_pConfig->aOnPindle, &m_pConfig->iOnPindleCount, m_pConfig->iCharClass, this);
	dlg.DoModal();
}

void CSettingsDlg::OnOncorpse() 
{
	// TODO: Add your control notification handler code here
	CActionListDlg dlg(PB_ACTION_ONCORPSE, m_pConfig->aOnCorpse, &m_pConfig->iOnCorpseCount, m_pConfig->iCharClass, this);
	dlg.DoModal();
}

void CSettingsDlg::OnAddcombo() 
{
	// TODO: Add your control notification handler code here
	if (m_wndCombo.GetItemCount() >= PB_MAX_COMBO)
		return;

	CComboDlg dlg(0);
	if (dlg.DoModal() != IDOK)
		return;

	DWORD dwCombo = dlg.GetCombo();
	if (dwCombo == 0 || FindCombo(dwCombo) != -1)
		return;

	ListCombo(INT_MAX, dwCombo);
}

void CSettingsDlg::OnDelcombo() 
{
	// TODO: Add your control notification handler code here
	m_wndCombo.DeleteAllItems(RC_ITEM_SELECTED);
}

void CSettingsDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (m_pConfig->iBeforeRedCount == 0
		&& m_pConfig->iInRedCount == 0
		&& m_pConfig->iOnPindleCount == 0
		&& m_pConfig->iOnCorpseCount == 0)
	{
		CString sErr;
		sErr.Format(_T("您尚未为角色\"%s\"设置任何技能，因此这个角色现在还不能用来kp。请点击\"技能方案\"中的四个按钮进行技能设置。"), m_pConfig->szCharName);
		MessageBox(sErr, _T("无技能"), MB_ICONEXCLAMATION);
		return;
	}
	
	if (!UpdateData(TRUE))
		return;

	m_pConfig->iMalahHP = m_iMalahHP;
	m_pConfig->iMalahMana = m_iMalahMana;
	m_pConfig->iPetPotionHP = m_iPetHP;
	m_pConfig->iPotionHP = m_iPotionHP;
	m_pConfig->iPotionMana = m_iPotionMana;
	m_pConfig->iQuitHP = m_iQuitHP;
	m_pConfig->iQuitMana = m_iQuitMana;
	m_pConfig->bQuitOnPetDeath = m_bQuitIfPetDie;
	m_pConfig->bResurrectPet = m_bResMerc;
	m_pConfig->iRepairPercent = m_iRepairPercent;
	m_pConfig->nLocationType = m_nStay;
	m_pConfig->nAmmoType = m_nAmmoType;		
	m_pConfig->iComboCount = (BYTE)m_wndCombo.GetItemCount();
	::memset(m_pConfig->aAvoidCombos, 0, sizeof(m_pConfig->aAvoidCombos));
	for (BYTE i = 0; i < m_pConfig->iComboCount; i++)
		m_pConfig->aAvoidCombos[i] = m_wndCombo.GetItemData(i);
	
	EndDialog(IDOK);
}

int CSettingsDlg::FindCombo(DWORD dwCombo) const
{
	for (int i = 0; i < m_wndCombo.GetItemCount(); i++)
	{
		if (ComboMatch(dwCombo, m_wndCombo.GetItemData(i)))
			return i;
	}

	return -1;
}

int CSettingsDlg::ListCombo(int nIndex, DWORD dwCombo)
{
	if (dwCombo == 0)
		return -1;

	WORD w1 = LOWORD(dwCombo);
	WORD w2 = HIWORD(dwCombo);

	TCHAR szName[129] = _T("");
	::PBotGetBonusName(w1, szName, 128, TRUE);
	const int IDX = m_wndCombo.InsertItem(nIndex, szName);
	::PBotGetBonusName(w2, szName, 128, TRUE);
	m_wndCombo.SetItemText(IDX, 1, szName);
	m_wndCombo.SetItemData(IDX, dwCombo);
	return IDX;
}

void CSettingsDlg::OnEditcombo() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndCombo.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;
	
	CComboDlg dlg(m_wndCombo.GetItemData(IDX));
	if (dlg.DoModal() != IDOK || ComboMatch(dlg.GetCombo(), m_wndCombo.GetItemData(IDX)))
		return;

	DWORD dwCombo = dlg.GetCombo();
	if (dwCombo == 0 || FindCombo(dwCombo) != -1)
		return;

	ListCombo(IDX, dwCombo);
}

void CSettingsDlg::OnItemchangedDeadlycombo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int IDX = m_wndCombo.GetFirstItem(RC_ITEM_SELECTED);
	m_wndDelCombo.EnableWindow(IDX != -1);
	m_wndEditCombo.EnableWindow(IDX != -1);
	m_wndAddCombo.EnableWindow(m_wndCombo.GetItemCount() < PB_MAX_COMBO);
	*pResult = 0;
}

void CSettingsDlg::OnDblclkDeadlycombo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEditcombo();
	*pResult = 0;
}

BOOL CSettingsDlg::ComboMatch(DWORD dwCombo1, DWORD dwCombo2)
{
	if (dwCombo1 == dwCombo2)
		return TRUE;

	DWORD dw = MAKELONG(HIWORD(dwCombo1), LOWORD(dwCombo1));
	return dw == dwCombo2;
}
