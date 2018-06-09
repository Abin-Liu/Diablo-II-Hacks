// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShopbotCfg.h"
#include "ConfigDlg.h"
#include "D2ModParser.h"
#include "ModDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(LPCSBITEMCONFIG pConfig, const CArrayEx<SBITEMCONFIG, const SBITEMCONFIG&>* pConfigs, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_sCode = _T("");
	m_sDesc = _T("");
	m_nSockets = 0;
	m_bNonMagic = FALSE;
	//}}AFX_DATA_INIT
	m_pConfigs = pConfigs;
	if (pConfig)
		::memcpy(&m_config, pConfig, sizeof(SBITEMCONFIG));
	else
		::memset(&m_config, 0, sizeof(SBITEMCONFIG));
	m_sCode = m_config.szItemCode;
	m_sDesc = m_config.szDescription;
	m_nSockets = m_config.iSockets;
	m_bNonMagic = m_config.bNonMagic;
	m_bNew = pConfig == NULL;
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Control(pDX, IDC_EDIT, m_wndEdit);
	DDX_Control(pDX, IDC_DEL, m_wndRemove);
	DDX_Control(pDX, IDC_ADD, m_wndAdd);
	DDX_Control(pDX, IDC_CODE, m_wndCode);
	DDX_Control(pDX, IDC_DESC, m_wndDesc);
	DDX_Control(pDX, IDC_MODS, m_wndMods);
	DDX_Text(pDX, IDC_CODE, m_sCode);
	DDV_MaxChars(pDX, m_sCode, 255);
	DDX_Text(pDX, IDC_DESC, m_sDesc);
	DDV_MaxChars(pDX, m_sDesc, 255);
	DDX_CBIndex(pDX, IDC_SOCKETS, m_nSockets);
	DDX_Check(pDX, IDC_NONMAGIC, m_bNonMagic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MODS, OnItemchangedMods)
	ON_NOTIFY(NM_DBLCLK, IDC_MODS, OnDblclkMods)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndMods.InsertColumn(0, _T("Item Mods"), LVCFMT_LEFT, 300);
	m_wndMods.SetHeaderImageList(IDB_BITMAP1);
	m_wndMods.SetHeaderImage(0, 1);
	ListMods();
	m_wndOK.EnableWindow(m_wndMods.GetItemCount());

	m_wndAdd.SetIcon(IDI_ADD);
	m_wndAdd.SetTooltipText(_T("Add a new mod"));
	m_wndRemove.SetIcon(IDI_REMOVE);
	m_wndRemove.SetTooltipText(_T("Remove selected mods"));
	m_wndEdit.SetIcon(IDI_EDIT);
	m_wndEdit.SetTooltipText(_T("Edit selected mod"));
	m_wndOK.SetIcon(IDI_YES);
	m_wndOK.SetTooltipText(_T("Exit and save changes"));
	m_wndCancel.SetIcon(IDI_NO);
	m_wndCancel.SetTooltipText(_T("Exit without saving any changes"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::ListMods()
{
	m_wndMods.DeleteAllItems();
	for (BYTE i = 0; i < m_config.iModNum; i++)
	{
		CString sMod;
		if (GetItemModDesc(&m_config.aMods[i], sMod))
		{
			const int IDX = m_wndMods.InsertItem(INT_MAX, sMod);
			m_wndMods.SetItemData(IDX, (DWORD)&m_config.aMods[i]);
		}		
	}
}

void CConfigDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndMods.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	ITEMMOD* p = (ITEMMOD*)m_wndMods.GetItemData(IDX);
	if (p == NULL)
		return;

	CModDlg dlg(p);
	if (dlg.DoModal() != IDOK)
		return;

	CString sMod;
	if (GetItemModDesc(dlg.GetMod(), sMod))
	{
		::memcpy(p, dlg.GetMod(), sizeof(ITEMMOD));
		m_wndMods.SetItemText(IDX, 0, sMod);
	}
}

void CConfigDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	if (m_config.iModNum >= SB_MAX_MOD)
		return;

	CModDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	CString sMod;
	if (GetItemModDesc(dlg.GetMod(), sMod))
	{
		::memcpy(&m_config.aMods[m_config.iModNum], dlg.GetMod(), sizeof(ITEMMOD));
		const int IDX = m_wndMods.InsertItem(INT_MAX, sMod);
		m_wndMods.SetItemData(IDX, (DWORD)&m_config.aMods[m_config.iModNum]);
		m_config.iModNum++;

		m_wndMods.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED | RC_ITEM_UNFOCUSED);
		m_wndMods.SetItemStates(IDX, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
		m_wndMods.EnsureVisible(IDX, FALSE);
		m_wndMods.SetFocus();
	}
}

void CConfigDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	if (m_wndMods.GetItemCount(RC_ITEM_SELECTED) < 1)
		return;

	m_wndMods.DeleteAllItems(RC_ITEM_SELECTED);
	ITEMMOD aTemp[SB_MAX_MOD] = {0};
	m_config.iModNum = m_wndMods.GetItemCount();
	for (int i = 0; i < m_config.iModNum; i++)
	{
		ITEMMOD* p = (ITEMMOD*)m_wndMods.GetItemData(i);
		if (p)
			::memcpy(&aTemp[i], p, sizeof(ITEMMOD));
	}

	::memcpy(m_config.aMods, aTemp, sizeof(aTemp));
	m_wndAdd.EnableWindow(m_config.iModNum < SB_MAX_MOD);
}

BOOL CConfigDlg::VerifyName() const
{
	if (m_pConfigs == NULL || !m_bNew)
		return TRUE;

	for (int i = 0; i < m_pConfigs->GetSize(); i++)
	{
		if (m_sDesc.CompareNoCase(m_pConfigs->GetAt(i).szDescription) == 0)
			return FALSE;
	}
	return TRUE;
}

void CConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sDesc.TrimLeft();
	m_sDesc.TrimRight();
	m_sCode.TrimLeft();
	m_sCode.TrimRight();
	m_sCode.MakeLower();

	if (m_wndMods.GetItemCount() == 0)
	{
		MessageBox(_T("No item mods specified!"), _T("Invalid Data"), MB_ICONEXCLAMATION);
		return;
	}

	if (m_sDesc.IsEmpty())
	{
		MessageBox(_T("Description cannot be empty!"), _T("Invalid Data"), MB_ICONEXCLAMATION);
		m_wndDesc.SetFocus();
		m_wndDesc.SetSel(0, -1);
		return;
	}

	if (m_sDesc.CompareNoCase(_T("Global")) == 0)
	{
		MessageBox(_T("\"Global\" is reserved, it cannot be used as a description!"), _T("Invalid Data"), MB_ICONEXCLAMATION);
		m_wndDesc.SetFocus();
		m_wndDesc.SetSel(0, -1);
		return;
	}

	/*
	if (m_sCode.IsEmpty())
	{
		MessageBox(_T("Item codes cannot be empty!"), _T("Invalid Data"), MB_ICONEXCLAMATION);
		m_wndCode.SetFocus();
		m_wndCode.SetSel(0, -1);
		return;
	}
	*/

	if (!VerifyName())
	{
		CString sErr;
		sErr.Format(_T("\"%s\" already exists, please use a different description."), m_sDesc);
		MessageBox(sErr, _T("Invalid Data"), MB_ICONEXCLAMATION);
		m_wndDesc.SetFocus();
		m_wndDesc.SetSel(0, -1);
		return;
	}

	m_config.iSockets = m_nSockets;
	m_config.bNonMagic = m_bNonMagic;
	_tcsncpy(m_config.szItemCode, m_sCode, SB_MAX_DESCRIPTION);
	_tcsncpy(m_config.szDescription, m_sDesc, SB_MAX_DESCRIPTION);
	
	EndDialog(IDOK);
}

LPCSBITEMCONFIG CConfigDlg::GetConfig() const
{
	return &m_config;
}

void CConfigDlg::OnItemchangedMods(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int IDX = m_wndMods.GetFirstItem(RC_ITEM_SELECTED);
	m_wndEdit.EnableWindow(IDX != -1);
	m_wndRemove.EnableWindow(IDX != -1);
	m_wndOK.EnableWindow(m_wndMods.GetItemCount());
	m_wndAdd.EnableWindow(m_config.iModNum < SB_MAX_MOD);
	*pResult = 0;
}

void CConfigDlg::OnDblclkMods(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}
