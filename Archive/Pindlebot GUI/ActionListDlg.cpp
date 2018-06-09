// ActionListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "ActionListDlg.h"
#include "SpellDef.h"
#include "ActionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionListDlg dialog


CActionListDlg::CActionListDlg(int nType, LPPBOTACTION pActions, BYTE* pCount, BYTE iCharClass, CWnd* pParent /*=NULL*/)
	: CDialog(CActionListDlg::IDD, pParent)
{
	m_nType = nType;
	m_pActions = pActions;
	m_iCharClass = iCharClass;
	m_pCount = pCount;
	ASSERT(m_pActions && m_pCount);
	for (int i = 0; i < PB_MAX_ACTION && i < *m_pCount; i++)
		m_aActions.Add(pActions[i]);

	//{{AFX_DATA_INIT(CActionListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CActionListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionListDlg)
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Control(pDX, IDC_UP, m_wndUp);
	DDX_Control(pDX, IDC_EDIT, m_wndEdit);
	DDX_Control(pDX, IDC_DOWN, m_wndDown);
	DDX_Control(pDX, IDC_DEL, m_wndDel);
	DDX_Control(pDX, IDC_ADD, m_wndAdd);
	DDX_Control(pDX, IDC_LIST1, m_wndActions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionListDlg, CDialog)
	//{{AFX_MSG_MAP(CActionListDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionListDlg message handlers

BOOL CActionListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndAdd.SetIcon(IDI_ADD);
	m_wndDel.SetIcon(IDI_DELETE);
	m_wndEdit.SetIcon(IDI_EDIT);
	m_wndUp.SetIcon(IDI_UP);
	m_wndDown.SetIcon(IDI_DOWN);
	m_wndOK.SetIcon(IDI_YES);
	m_wndCancel.SetIcon(IDI_NO);

	m_wndAdd.SetTooltipText(_T("增添技能"));
	m_wndDel.SetTooltipText(_T("删除技能"));
	m_wndEdit.SetTooltipText(_T("编辑技能"));
	m_wndUp.SetTooltipText(_T("上移"));
	m_wndDown.SetTooltipText(_T("下移"));
	m_wndOK.SetTooltipText(_T("确定"));
	m_wndCancel.SetTooltipText(_T("取消"));


	CString str(_T("技能方案 - "));
	if (m_nType == PB_ACTION_BEFORERED)
	{
		str += _T("进红门前");
	}
	else if (m_nType == PB_ACTION_INRED)
	{
		str += _T("进红门后");
	}
	else if (m_nType == PB_ACTION_ONPINDLE)
	{
		str += _T("见到Pindleskin后");
	}
	else if (m_nType == PB_ACTION_ONCORPSE)
	{
		str += _T("地上有尸体时");
	}
	else
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	SetWindowText(str);

	m_wndActions.SetColumnHeader(_T("技能, 100; 左/右键, 60; 目标, 100; 延迟, 60; 次数, 60"));
	for (int i = 0; i < m_aActions.GetSize(); i++)
		ListAction(INT_MAX, m_aActions[i], FALSE);

	m_wndAdd.EnableWindow(m_aActions.GetSize() < PB_MAX_ACTION);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CActionListDlg::GetTargetStr(int nTarget)
{
	CString str;
	switch (nTarget)
	{
	case PB_TARGET_NONE:
		str = _T("无需瞄准");
		break;

	case PB_TARGET_PINDLE:
		str = _T("Pindleskin本身");
		break;

	case PB_TARGET_FRONT:
		str = _T("角色和Pindleskin之间");
		break;

	case PB_TARGET_BEHIND:
		str = _T("Pindleskin身后");
		break;

	default:
		break;
	}
	return str;
}

void CActionListDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int IDX = m_wndActions.GetFirstItem(RC_ITEM_SELECTED);
	m_wndAdd.EnableWindow(m_aActions.GetSize() < PB_MAX_ACTION);
	m_wndDel.EnableWindow(IDX != -1);
	m_wndEdit.EnableWindow(IDX != -1);
	m_wndUp.EnableWindow(IDX > 0);
	m_wndDown.EnableWindow(IDX != -1 && IDX != m_wndActions.GetItemCount() - 1);
	*pResult = 0;
}

void CActionListDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	if (m_aActions.GetSize() >= 255)
		return;

	PBOTACTION action = {0};
	action.wDelay = 500;
	action.iTimes = 1;
	CActionDlg dlg(m_nType, &action, m_iCharClass, this);
	if (dlg.DoModal() != IDOK)
		return;

	m_aActions.Add(action);
	ListAction(INT_MAX, action, FALSE);
}

void CActionListDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndActions.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	m_aActions.RemoveAt(IDX);
	m_wndActions.DeleteItem(IDX, TRUE);
}

void CActionListDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndActions.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CActionDlg dlg(m_nType, &m_aActions[IDX], m_iCharClass, this);
	if (dlg.DoModal() == IDOK)
		ListAction(IDX, m_aActions[IDX], TRUE);
}

void CActionListDlg::OnUp() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndActions.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX < 1)
		return;

	PBOTACTION action = m_aActions[IDX];
	m_aActions.RemoveAt(IDX);
	m_aActions.InsertAt(IDX - 1, action);
	m_wndActions.MoveUp(IDX);
}

void CActionListDlg::OnDown() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndActions.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1 || IDX == m_wndActions.GetItemCount() - 1)
		return;

	PBOTACTION action = m_aActions[IDX];
	m_aActions.RemoveAt(IDX);
	m_aActions.InsertAt(IDX + 1, action);
	m_wndActions.MoveDown(IDX);
}

void CActionListDlg::OnOK() 
{
	// TODO: Add extra validation here
	::memset(m_pActions, 0, sizeof(PBOTACTION) * PB_MAX_ACTION);
	*m_pCount = (BYTE)m_aActions.GetSize();
	::memcpy(m_pActions, m_aActions.GetData(), sizeof(PBOTACTION) * m_aActions.GetSize());
	CDialog::OnOK();
}

int CActionListDlg::ListAction(int nIndex, const PBOTACTION &action, BOOL bEdit)
{
	D2SPELLINFO dsi = {0};
	if (!::D2GetSpellInfo(action.wSpellID, &dsi))
		return -1;
	
	if (bEdit)
		m_wndActions.SetItemText(nIndex, 0, dsi.szSpellName);

	const int IDX = bEdit ? nIndex : m_wndActions.InsertItem(nIndex, dsi.szSpellName);
	m_wndActions.SetItemText(IDX, 1, action.iLeft ? _T("左") : _T("右"));
	m_wndActions.SetItemText(IDX, 2, GetTargetStr(action.iTarget));
	m_wndActions.SetItemText(IDX, 3, action.wDelay);
	m_wndActions.SetItemText(IDX, 4, action.iTimes);	
	return IDX;
}

void CActionListDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}
