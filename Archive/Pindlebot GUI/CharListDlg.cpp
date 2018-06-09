// CharListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "CharListDlg.h"
#include "SettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharListDlg dialog


CCharListDlg::CCharListDlg(ConfigList* pConfigList, DWORD dwIdleLength, BOOL bUseShopbot, CWnd* pParent /*=NULL*/)
	: CDialog(CCharListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharListDlg)
	m_dwIdleLen = dwIdleLength;
	m_bUseShopbot = bUseShopbot;
	//}}AFX_DATA_INIT
	m_pConfigList = pConfigList;
	ASSERT(m_pConfigList);
}


void CCharListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharListDlg)
	DDX_Control(pDX, IDC_USESHOPBOT, m_wndUseShopbot);
	DDX_Control(pDX, IDC_SHOPBOT, m_wndShopbot);
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Control(pDX, IDC_REN, m_wndRen);
	DDX_Control(pDX, IDC_EDIT, m_wndEdit);
	DDX_Control(pDX, IDC_DEL, m_wndDel);
	DDX_Control(pDX, IDC_ADD, m_wndAdd);
	DDX_Control(pDX, IDC_LIST1, m_wndChars);
	DDX_Text(pDX, IDC_IDLELEN, m_dwIdleLen);
	DDV_MinMaxDWord(pDX, m_dwIdleLen, 0, 3600);
	DDX_Check(pDX, IDC_USESHOPBOT, m_bUseShopbot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharListDlg, CDialog)
	//{{AFX_MSG_MAP(CCharListDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_REN, OnRen)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_SHOPBOT, OnShopbot)
	ON_BN_CLICKED(IDC_USESHOPBOT, OnUseshopbot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharListDlg message handlers

BOOL CCharListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndAdd.SetIcon(IDI_ADD);
	m_wndDel.SetIcon(IDI_DELETE);
	m_wndEdit.SetIcon(IDI_EDIT);
	m_wndRen.SetIcon(IDI_RENAME);
	m_wndOK.SetIcon(IDI_YES);
	m_wndCancel.SetIcon(IDI_NO);
	m_wndShopbot.SetIcon(IDI_SHOP);

	m_wndAdd.SetTooltipText(_T("添加新角色"));
	m_wndDel.SetTooltipText(_T("删除选定的角色"));
	m_wndEdit.SetTooltipText(_T("更改角色设置"));
	m_wndRen.SetTooltipText(_T("重命名角色"));
	m_wndOK.SetTooltipText(_T("保存设置并退出"));
	m_wndCancel.SetTooltipText(_T("退出不保存设置"));
	m_wndShopbot.SetTooltipText(_T("Shopbot参数设置"));

	m_wndChars.SetColumnHeader(_T("角色, 150; 职业, 100"));
	for (int i = 0; i < m_pConfigList->GetSize(); i++)
	{
		const int IDX = m_wndChars.InsertItem(INT_MAX, (*m_pConfigList)[i].szCharName);
		m_wndChars.SetItemText(IDX, 1, CHARCLASSNAME[(*m_pConfigList)[i].iCharClass]);
	}

	m_wndShopbot.EnableWindow(m_bUseShopbot);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCharListDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CCharDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	if (FindByName(dlg.GetCharName()) != -1)
	{
		CString sErr;
		sErr.Format(_T("角色\"%s\"已存在，请使用另外的角色名。"), dlg.GetCharName());
		MessageBox(sErr, _T("无效角色名"), MB_ICONEXCLAMATION);
		return;
	}
	
	PBOTCONFIG cfg = {0};
	_tcsncpy(cfg.szCharName, dlg.GetCharName(), PB_MAX_CHARNAME);
	cfg.iCharClass = dlg.GetCharClass();

	// Prepare the config, fill in some default data
	cfg.iMalahHP = 95;
	cfg.iMalahMana = 80;
	cfg.iPotionHP = 75;
	cfg.iPotionMana = 15;
	cfg.iQuitHP = 50;
	cfg.iPetPotionHP = 80;
	cfg.bQuitOnPetDeath = TRUE;
	cfg.bResurrectPet = TRUE;
	cfg.iRepairPercent = 75;
	cfg.aAvoidCombos[cfg.iComboCount++] = MAKELONG(PINDLE_BONUS_NONE, PINDLE_AURA_FANATICISM);
	cfg.aAvoidCombos[cfg.iComboCount++] = MAKELONG(PINDLE_BONUS_NONE, PINDLE_AURA_MIGHT);
	cfg.aAvoidCombos[cfg.iComboCount++] = MAKELONG(PINDLE_BONUS_NONE, PINDLE_AURA_CONVICTION);
	cfg.aAvoidCombos[cfg.iComboCount++] = MAKELONG(PINDLE_BONUS_NONE, PINDLE_AURA_HOLYSHOCK);
	cfg.aAvoidCombos[cfg.iComboCount++] = MAKELONG(PINDLE_BONUS_LIGHTNING_ENCHANTED, PINDLE_BONUS_MULTISHOT);
	cfg.aAvoidCombos[cfg.iComboCount++] = MAKELONG(PINDLE_BONUS_LIGHTNING_ENCHANTED, PINDLE_BONUS_COLD_ENCHANTED);

	CSettingsDlg dlg2(&cfg);
	if (dlg2.DoModal() != IDOK)
		return;

	const int IDX = m_wndChars.InsertItem(INT_MAX, cfg.szCharName);
	m_wndChars.SetItemText(IDX, 1, CHARCLASSNAME[cfg.iCharClass]);
	m_wndChars.SetAllItemStates(RC_ITEM_SELECTED | RC_ITEM_FOCUSED, RC_ITEM_UNSELECTED | RC_ITEM_UNFOCUSED);
	m_wndChars.SetItemStates(IDX, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
	m_pConfigList->Add(cfg);
}

void CCharListDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	for (int i = m_pConfigList->GetSize() - 1; i >= 0; i--)
	{
		if (m_wndChars.GetItemStates(i) & RC_ITEM_SELECTED)
			m_pConfigList->RemoveAt(i);
	}

	m_wndChars.DeleteAllItems(RC_ITEM_SELECTED);
}

void CCharListDlg::OnRen() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndChars.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CCharDlg dlg(m_pConfigList->GetAt(IDX).szCharName, m_pConfigList->GetAt(IDX).iCharClass);
	if (dlg.DoModal() != IDOK || dlg.GetCharName().CompareNoCase(m_pConfigList->GetAt(IDX).szCharName) == 0)
		return;

	if (FindByName(dlg.GetCharName()) != -1)
	{
		CString sErr;
		sErr.Format(_T("角色\"%s\"已存在，请使用另外的角色名。"), dlg.GetCharName());
		MessageBox(sErr, _T("无效角色名"), MB_ICONEXCLAMATION);
		return;
	}

	_tcsncpy(m_pConfigList->ElementAt(IDX).szCharName, dlg.GetCharName(), PB_MAX_CHARNAME);
	m_wndChars.SetItemText(IDX, 0, dlg.GetCharName());	
}

int CCharListDlg::FindByName(LPCTSTR lpszName) const
{
	for (int i = 0; i < m_pConfigList->GetSize(); i++)
	{
		if (_tcsicmp((*m_pConfigList)[i].szCharName, lpszName) == 0)
			return i;
	}

	return -1;
}

void CCharListDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndChars.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CSettingsDlg dlg(&m_pConfigList->ElementAt(IDX));
	dlg.DoModal();	
}

void CCharListDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}

void CCharListDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int IDX = m_wndChars.GetFirstItem(RC_ITEM_SELECTED);
	m_wndDel.EnableWindow(IDX != - 1);
	m_wndRen.EnableWindow(IDX != - 1);
	m_wndEdit.EnableWindow(IDX != - 1);
	*pResult = 0;
}

void CCharListDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (UpdateData(TRUE))
		EndDialog(IDOK);
}

DWORD CCharListDlg::GetIdleLength() const
{
	return m_dwIdleLen;
}

void CCharListDlg::OnShopbot() 
{
	// TODO: Add your control notification handler code here
	CString sParam;
	sParam.Format(_T("%08X"), (DWORD)m_hWnd);
	::ShellExecute(NULL, _T("open"), _T("ShopbotCfg.exe"), sParam, AfxGetAppEx()->GetAppDir(), SW_SHOWNORMAL);
}

void CCharListDlg::OnUseshopbot() 
{
	// TODO: Add your control notification handler code here
	m_wndShopbot.EnableWindow(m_wndUseShopbot.GetCheck());
}

BOOL CCharListDlg::IsUseShopbot() const
{
	return m_bUseShopbot;
}
