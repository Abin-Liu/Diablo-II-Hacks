// ComboDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "ComboDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboDlg dialog


CComboDlg::CComboDlg(DWORD dwCombo, CWnd* pParent /*=NULL*/)
	: CDialog(CComboDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComboDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwCombo = dwCombo;
}


void CComboDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComboDlg)
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDC_LIST1, m_wndCombos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComboDlg, CDialog)
	//{{AFX_MSG_MAP(CComboDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboDlg message handlers

void CComboDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_wndOK.EnableWindow(m_wndCombos.GetItemCount(RC_ITEM_SELECTED) == 2);
	*pResult = 0;
}

BOOL CComboDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndCombos.SetColumnHeader(_T("Pindleskin 属性, 250; 凶猛程度, 80"));

	ListBonus(PINDLE_BONUS_NONE, 1);
	ListBonus(PINDLE_BONUS_LIGHTNING_ENCHANTED, 4);
	ListBonus(PINDLE_BONUS_COLD_ENCHANTED, 4);
	ListBonus(PINDLE_BONUS_EXTRA_FAST, 2);
	ListBonus(PINDLE_BONUS_EXTRA_STRONG, 3);
	ListBonus(PINDLE_BONUS_MAGIC_RESISTANT, 1);
	ListBonus(PINDLE_BONUS_STONE_SKIN, 1);
	ListBonus(PINDLE_BONUS_CURSED, 4);
	ListBonus(PINDLE_BONUS_MANA_BURN, 2);
	ListBonus(PINDLE_BONUS_MULTISHOT, 1);
	ListBonus(PINDLE_BONUS_SPECTRAL_HIT, 2);
	ListBonus(PINDLE_BONUS_TELEPORT, 1);
	ListBonus(PINDLE_AURA_MIGHT, 5);
	ListBonus(PINDLE_AURA_HOLYFIRE, 1);
	ListBonus(PINDLE_AURA_HOLYFREEZE, 2);
	ListBonus(PINDLE_AURA_HOLYSHOCK, 4);
	ListBonus(PINDLE_AURA_CONVICTION, 5);
	ListBonus(PINDLE_AURA_FANATICISM, 5);
	ListBonus(PINDLE_AURA_BLESSEDAIM, 1);

	if (m_dwCombo != 0)
	{
		int nIdx = FindBonus(LOWORD(m_dwCombo));
		if (nIdx != -1)
			m_wndCombos.SetItemStates(nIdx, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);

		nIdx = FindBonus(HIWORD(m_dwCombo));
		if (nIdx != -1)
			m_wndCombos.SetItemStates(nIdx, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CComboDlg::ListBonus(WORD wBonus, int nThreat)
{
	nThreat = max(0, nThreat);
	nThreat = min(5, nThreat);
	TCHAR szName[129] = _T("");
	::PBotGetBonusName(wBonus, szName, 128, TRUE);
	const int IDX = m_wndCombos.InsertItem(INT_MAX, szName);
	CString sThreat;
	for (int i = 0; i < nThreat; i++)
		sThreat += _T("* ");
	sThreat.TrimRight();
	m_wndCombos.SetItemText(IDX, 1, sThreat);
	m_wndCombos.SetItemData(IDX, wBonus);
	return IDX;
}

void CComboDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_dwCombo = 0;
	if (m_wndCombos.GetItemCount(RC_ITEM_SELECTED) != 2)
		return;

	const int IDX1 = m_wndCombos.GetFirstItem(RC_ITEM_SELECTED);
	const int IDX2 = m_wndCombos.GetFirstItem(RC_ITEM_SELECTED, IDX1);
	WORD w1 = (WORD)m_wndCombos.GetItemData(IDX1);
	WORD w2 = (WORD)m_wndCombos.GetItemData(IDX2);

	if ((w1 & 0x8000) && (w2 & 0x8000))
	{
		MessageBox(_T("Pindleskin不可能同时拥有2种光环，请重新选择。"), _T("无效属性"), MB_ICONEXCLAMATION);
		return;
	}

	m_dwCombo = MAKELONG(w1, w2);
	CDialog::OnOK();
}

DWORD CComboDlg::GetCombo() const
{
	return m_dwCombo;
}

int CComboDlg::FindBonus(WORD wBonus) const
{
	for (int i = 0; i < m_wndCombos.GetItemCount(); i++)
	{
		if (m_wndCombos.GetItemData(i) == wBonus)
			return i;
	}

	return -1;
}
