// ShopbotCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShopbotCfg.h"
#include "ShopbotCfgDlg.h"
#include "ConfigDlg.h"
#include "MapDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShopbotCfgDlg dialog

CShopbotCfgDlg::CShopbotCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShopbotCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShopbotCfgDlg)
	m_nHotkey = -1;
	m_nStopHP = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_shopbot_ini.SetPathName(AfxGetAppEx()->GetAppDir() + _T("\\Shopbot.ini"));
	m_bItemChanged = FALSE;
}

void CShopbotCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShopbotCfgDlg)
	DDX_Control(pDX, IDC_ACT, m_wndAct);
	DDX_Control(pDX, IDC_WP, m_wndWP);
	DDX_Control(pDX, IDC_NPC, m_wndNPC);
	DDX_Control(pDX, IDC_STOPHP, m_wndStopHP);
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Control(pDX, IDC_EDIT, m_wndEdit);
	DDX_Control(pDX, IDC_DEL, m_wndRemove);
	DDX_Control(pDX, IDC_ADD, m_wndAdd);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_CBIndex(pDX, IDC_HOTKEY, m_nHotkey);
	DDX_Text(pDX, IDC_STOPHP, m_nStopHP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShopbotCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CShopbotCfgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_CBN_SELCHANGE(IDC_ACT, OnSelchangeAct)
	ON_CBN_SELCHANGE(IDC_HOTKEY, OnSelchangeGlobal)
	ON_CBN_SELCHANGE(IDC_NPC, OnSelchangeGlobal)
	ON_EN_CHANGE(IDC_STOPHP, OnSelchangeGlobal)
	ON_CBN_SELCHANGE(IDC_WP, OnSelchangeGlobal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShopbotCfgDlg message handlers

BOOL CShopbotCfgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_wndList.InsertColumn(0, _T("Description"), LVCFMT_LEFT, 300);
	m_wndList.SetHeaderImageList(IDB_BITMAP1);
	m_wndList.SetHeaderImage(0, 3);
	LoadConfig();

	m_wndAdd.SetIcon(IDI_ADD);
	m_wndAdd.SetTooltipText(_T("Add a new item configuration"));
	m_wndRemove.SetIcon(IDI_REMOVE);
	m_wndRemove.SetTooltipText(_T("Remove selected items"));
	m_wndEdit.SetIcon(IDI_EDIT);
	m_wndEdit.SetTooltipText(_T("Edit selected item"));
	m_wndOK.SetIcon(IDI_YES);
	m_wndOK.SetTooltipText(_T("Save changes to shopbot.ini"));
	m_wndCancel.SetIcon(IDI_NO);
	m_wndCancel.SetTooltipText(_T("Exit"));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShopbotCfgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShopbotCfgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShopbotCfgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CShopbotCfgDlg::LoadConfig()
{
	m_aConfigs.RemoveAll();
	m_wndList.DeleteAllItems();

	// load global settings	
	SBGLOBAL sbg = {0};
	SBLoadGlobal(m_shopbot_ini.GetPathName(), &sbg);
	m_nStopHP = sbg.iStopHP;
	m_nHotkey = sbg.iHotkey - VK_F1;
	UpdateData(FALSE);
	m_wndAct.SetCurSel(sbg.iAct);
	LoadNPC();
	LoadWP();
	m_wndWP.SetCurSel(FindWP(sbg.iWP));
	m_wndNPC.SetCurSel(FindNPC(sbg.wNPC));

	// load item settings
	CStringArray aDesc;
	m_shopbot_ini.GetSectionNames(&aDesc);
	SBITEMCONFIG sbi = {0};
	for (int i = 0; i < aDesc.GetSize(); i++)
	{		
		if (SBLoadConfig(m_shopbot_ini.GetPathName(), aDesc[i], &sbi))
		{
			m_aConfigs.Add(sbi);
			m_wndList.InsertItem(INT_MAX, sbi.szDescription);
		}
	}
	
	/*

	// load global settings	
	m_nStopHP = m_shopbot_ini.GetInt(_T("Global"), _T("Stop HP"), 50);
	m_nHotkey = m_shopbot_ini.GetInt(_T("Global"), _T("Hotkey"), 11) - VK_F1;
	if (m_nHotkey < 0 || m_nHotkey > 11)
		m_nHotkey = 11;
	UpdateData(FALSE);	

	int nAct = m_shopbot_ini.GetInt(_T("Global"), _T("Act"), 4);
	if (nAct < 0 || nAct > 4)
		nAct = 4;

	m_wndAct.SetCurSel(nAct);
	LoadNPC();
	LoadWP();

	D2OBJFINGERPRINTER fp = {0};
	m_shopbot_ini.GetDataBlock(_T("Global"), _T("NPC"), &fp, sizeof(D2OBJFINGERPRINTER));
	DWORD dwWP = m_shopbot_ini.GetUInt(_T("Global"), _T("WP"), 0);
	m_wndNPC.SetCurSel(FindNPC(&fp));
	m_wndWP.SetCurSel(FindWP(dwWP));

	// load item settings
	CStringArray aDesc;
	m_shopbot_ini.GetSectionNames(&aDesc);

	for (int i = 0; i < aDesc.GetSize(); i++)
	{
		if (aDesc[i].Compare(_T("Global")) == 0)
			continue;

		SBITEMCONFIG ic = {0};
		strncpy(ic.szDescription, aDesc[i], SB_MAX_DESCRIPTION);
		strncpy(ic.szItemCode, m_shopbot_ini.GetString(aDesc[i], "Code"), SB_MAX_DESCRIPTION);
		ic.iSockets = (BYTE)m_shopbot_ini.GetUInt(aDesc[i], "Sockets", 0);
		CStringArray aKeys;
		m_shopbot_ini.GetKeyNames(aDesc[i], &aKeys);

		for (int j = 0; j < aKeys.GetSize() && ic.iModNum < SB_MAX_MOD; j++)
		{
			if (aKeys[j].CompareNoCase("Code") == 0	|| aKeys[j].CompareNoCase("Sockets") == 0)
				continue;

			ic.aMods[ic.iModNum].dwModID  = strtoul(aKeys[j], NULL, 10);

			CStringArray aModVals;
			m_shopbot_ini.GetArray(aDesc[i], aKeys[j], &aModVals, ",");
			if (aModVals.GetSize() == 0)
				continue;

			if (ic.aMods[ic.iModNum].dwModID == 107)
			{
				// single skills, up to 3
				if (aModVals.GetSize() % 2)
					aModVals.RemoveAt(aModVals.GetUpperBound());

				for (int k = 0; k + 1 < aModVals.GetSize() && ic.iModNum < SB_MAX_MOD; k += 2)
				{
					ic.aMods[ic.iModNum].dwModID = 107;
					ic.aMods[ic.iModNum].iCount = 2;
					ic.aMods[ic.iModNum].aValues[0] = strtoul(aModVals[k], NULL, 10);
					ic.aMods[ic.iModNum].aValues[1] = strtoul(aModVals[k + 1], NULL, 10);
					ic.iModNum++;
				}
			}
			else
			{
				for (int k = 0; k < aModVals.GetSize() && k < 4; k++)
				{
					ic.aMods[ic.iModNum].iCount++;			
					ic.aMods[ic.iModNum].aValues[k] = strtoul(aModVals[k], NULL, 10);
				}	
				ic.iModNum++;
			}						
		}

		m_aConfigs.Add(ic);
		m_wndList.InsertItem(INT_MAX, ic.szDescription);
	}
	*/
}

void CShopbotCfgDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CConfigDlg dlg(&m_aConfigs[IDX], &m_aConfigs);
	if (dlg.DoModal() != IDOK)
		return;

	::memcpy(&m_aConfigs[IDX], dlg.GetConfig(), sizeof(SBITEMCONFIG));
	m_wndList.SetItemText(IDX, 0, m_aConfigs[IDX].szDescription);
	m_bItemChanged = TRUE;
	m_wndOK.EnableWindow(TRUE);
}

void CShopbotCfgDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CConfigDlg dlg(NULL, &m_aConfigs);
	if (dlg.DoModal() != IDOK)
		return;

	SBITEMCONFIG ic = {0};
	::memcpy(&ic, dlg.GetConfig(), sizeof(SBITEMCONFIG));
	m_aConfigs.Add(ic);
	const int IDX = m_wndList.InsertItem(INT_MAX, dlg.GetConfig()->szDescription);
	m_wndList.SetItemData(IDX, (DWORD)&m_aConfigs.GetLast());
	m_wndList.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED | RC_ITEM_UNFOCUSED);
	m_wndList.SetItemStates(IDX, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
	m_wndList.EnsureVisible(IDX, FALSE);
	m_wndList.SetFocus();
	m_bItemChanged = TRUE;
	m_wndOK.EnableWindow(TRUE);
}

void CShopbotCfgDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	const int COUNT = m_wndList.GetItemCount();
	for (int i = COUNT - 1; i >= 0; i--)
	{
		if (m_wndList.ExamItemStates(i, RC_ITEM_SELECTED))
		{
			m_wndList.DeleteItem(i, FALSE);
			m_aConfigs.RemoveAt(i);
			m_bItemChanged = TRUE;
			m_wndOK.EnableWindow(TRUE);
		}
	}
}

void CShopbotCfgDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	m_wndEdit.EnableWindow(IDX != -1);
	m_wndRemove.EnableWindow(IDX != -1);
	*pResult = 0;
}

void CShopbotCfgDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}

void CShopbotCfgDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_nStopHP < 0 || m_nStopHP > 100)
	{
		MessageBox(_T("Please specify a value between 0 and 100"), _T("Invalid Data"), MB_ICONEXCLAMATION);
		m_wndStopHP.SetFocus();
		m_wndStopHP.SetSel(0, -1);
		return;
	}

	SaveConfig();
}

void CShopbotCfgDlg::LoadNPC()
{
	m_wndNPC.ResetContent();
	int nIndex = 0;
	switch (m_wndAct.GetCurSel())
	{
	case 0:
		AddNPC(&FP_NPC_CHARSI, _T("Charsi"));
		AddNPC(&FP_NPC_AKARA, _T("Akara"));
		break;

	case 1:
		AddNPC(&FP_NPC_FARA, _T("Fara"));
		AddNPC(&FP_NPC_GROGNAN, _T("Grognan"));
		break;

	case 2:
		AddNPC(&FP_NPC_ORMUS, _T("Ormus"));
		break;

	case 3:
		AddNPC(&FP_NPC_JAMELLA, _T("Jamella"));
		AddNPC(&FP_NPC_HALBU, _T("Halbu"));
		break;

	case 4:
		AddNPC(&FP_NPC_LARZUK, _T("Larzuk"));
		AddNPC(&FP_NPC_MALAH, _T("Malah"));
		AddNPC(&FP_NPC_TOWNANYA, _T("Anya"));
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	m_wndNPC.SetCurSel(0);
}

void CShopbotCfgDlg::LoadWP()
{
	m_wndWP.ResetContent();
	int nIndex = 0;
	switch (m_wndAct.GetCurSel())
	{
	case 0:
		AddMap(MAP_A1_COLD_PLAINS, TRUE);
		AddMap(MAP_A1_STONY_FIELD, TRUE);
		AddMap(MAP_A1_DARK_WOOD, TRUE);
		AddMap(MAP_A1_BLACK_MARSH, TRUE);
		AddMap(MAP_A1_OUTER_CLOISTER, TRUE);
		AddMap(MAP_A1_JAIL_LEVEL_1);
		AddMap(MAP_A1_INNER_CLOISTER, TRUE);
		AddMap(MAP_A1_CATACOMBS_LEVEL_2);
		break;

	case 1:
		AddMap(MAP_A2_SEWERS_LEVEL_2);
		AddMap(MAP_A2_DRY_HILLS, TRUE);
		AddMap(MAP_A2_HALLS_OF_THE_DEAD_LEVEL_2);
		AddMap(MAP_A2_FAR_OASIS, TRUE);
		AddMap(MAP_A2_LOST_CITY, TRUE);
		AddMap(MAP_A2_PALACE_CELLAR_LEVEL_1);
		AddMap(MAP_A2_ARCANE_SANCTUARY);
		AddMap(MAP_A2_CAYON_OF_THE_MAGI, TRUE);		
		break;

	case 2:
		AddMap(MAP_A3_SPIDER_FOREST, TRUE);
		AddMap(MAP_A3_GREAT_MARSH, TRUE);
		AddMap(MAP_A3_FLAYER_JUNGLE, TRUE);
		AddMap(MAP_A3_LOWER_KURAST, TRUE);
		AddMap(MAP_A3_KURAST_BAZAAR, TRUE);
		AddMap(MAP_A3_UPPER_KURAST, TRUE);
		AddMap(MAP_A3_TRAVINCAL);
		AddMap(MAP_A3_DURANCE_OF_HATE_LEVEL_2);
		break;

	case 3:
		AddMap(MAP_A4_CITY_OF_THE_DAMNED, TRUE);
		AddMap(MAP_A4_RIVER_OF_FLAME);
		break;

	case 4:
		AddMap(MAP_A5_FRIGID_HIGHLANDS, TRUE);
		AddMap(MAP_A5_ARREAT_PLATEAU, TRUE);
		AddMap(MAP_A5_CRYSTALLINE_PASSAGE);
		AddMap(MAP_A5_GLACIAL_TRAIL);
		AddMap(MAP_A5_HALLS_OF_PAIN);
		AddMap(MAP_A5_FROZEN_TUNDRA, TRUE);
		AddMap(MAP_A5_THE_ANCIENTS_WAY);
		AddMap(MAP_A5_WORLDSTONE_KEEP_LEVEL_2);
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	m_wndWP.SetCurSel(0);
}

void CShopbotCfgDlg::OnSelchangeAct() 
{
	// TODO: Add your control notification handler code here
	m_wndOK.EnableWindow(TRUE);
	LoadNPC();
	LoadWP();
}

int CShopbotCfgDlg::FindNPC(WORD wNPC) const
{
	for (int i = 0; i < m_wndNPC.GetCount(); i++)
	{
		if (wNPC == m_wndNPC.GetItemData(i))
			return i;
	}

	return 0;
}

int CShopbotCfgDlg::FindWP(DWORD dwMap) const
{
	for (int i = 0; i < m_wndWP.GetCount(); i++)
	{
		if (dwMap == m_wndWP.GetItemData(i))
			return i;
	}

	return 0;
}

void CShopbotCfgDlg::SaveConfig()
{
	m_wndOK.EnableWindow(FALSE);

	if (m_bItemChanged)
	{
		// truncate all contents
		CFile file;
		if (file.Open(m_shopbot_ini.GetPathName(), CFile::modeWrite | CFile::modeCreate))
			file.Close();
	}

	// save global setting
	SBGLOBAL sbg = {0};
	sbg.iAct = (BYTE)m_wndAct.GetCurSel();
	sbg.iHotkey = (BYTE)m_nHotkey + VK_F1;
	sbg.iStopHP = (BYTE)m_nStopHP;
	sbg.iWP = (BYTE)m_wndWP.GetItemData(m_wndWP.GetCurSel());
	sbg.wNPC = (WORD)m_wndNPC.GetItemData(m_wndNPC.GetCurSel());
	SBSaveGlobal(m_shopbot_ini.GetPathName(), &sbg);

	for (int i = 0; i < m_aConfigs.GetSize(); i++)
		SBSaveConfig(m_shopbot_ini.GetPathName(), &m_aConfigs[i]);

	/*
	// save global setting
	m_shopbot_ini.WriteInt(_T("Global"), _T("Act"), m_wndAct.GetCurSel());
	m_shopbot_ini.WriteInt(_T("Global"), _T("Stop HP"), m_nStopHP);
	m_shopbot_ini.WriteInt(_T("Global"), _T("Hotkey"), m_nHotkey + VK_F1);


	LPCD2OBJFINGERPRINTER pFp = (LPCD2OBJFINGERPRINTER)m_wndNPC.GetItemData(m_wndNPC.GetCurSel());
	if (pFp)
		m_shopbot_ini.WriteDataBlock(_T("Global"), _T("NPC"), pFp, sizeof(D2OBJFINGERPRINTER));
	
	m_shopbot_ini.WriteUInt(_T("Global"), _T("WP"), m_wndWP.GetItemData(m_wndWP.GetCurSel()));

	// save item settings
	for (int i = 0; i < m_aConfigs.GetSize(); i++)
	{
		// item codes
		m_shopbot_ini.WriteString(m_aConfigs[i].szDescription, _T("Code"), m_aConfigs[i].szItemCode);

		// sockets
		m_shopbot_ini.WriteInt(m_aConfigs[i].szDescription, _T("Sockets"), m_aConfigs[i].iSockets);

		// mods
		for (int m = 0; m < m_aConfigs[i].iModNum; m++)
		{
			CString sModID;
			sModID.Format(_T("%u"), m_aConfigs[i].aMods[m].dwModID);
			CString sVals;
			for (BYTE v = 0; v < m_aConfigs[i].aMods[m].iCount; v++)
			{
				CString str;
				str.Format(_T("%u,"), m_aConfigs[i].aMods[m].aValues[v]);
				sVals += str;
			}

			if (m_aConfigs[i].aMods[m].dwModID == 107)
			{
				m_shopbot_ini.AppendString(m_aConfigs[i].szDescription, sModID, sVals);
			}
			else
			{
				sVals.TrimRight(_T(','));
				m_shopbot_ini.WriteString(m_aConfigs[i].szDescription, sModID, sVals);
			}		
		}
	}
	*/
}

void CShopbotCfgDlg::OnSelchangeGlobal() 
{
	// TODO: Add your control notification handler code here
	m_wndOK.EnableWindow(TRUE);
}

BOOL CShopbotCfgDlg::AddNPC(LPCD2OBJFINGERPRINTER pNpc, LPCTSTR lpszNpcName)
{
	if (pNpc == NULL)
		return FALSE;

	WORD wID;
	::memcpy(&wID, pNpc->aData, 2);

	const int IDX = m_wndNPC.AddString(lpszNpcName);
	m_wndNPC.SetItemData(IDX, wID);
	return IDX >= 0;
}

BOOL CShopbotCfgDlg::AddMap(BYTE iMapID, BOOL bDangerous)
{
	TCHAR szMapName[256] = "";
	if (!::D2GetMapName(iMapID, szMapName, 255))
		return FALSE;

	if (bDangerous)
		_tcscat(szMapName, _T("(*)"));

	const int IDX = m_wndWP.AddString(szMapName);
	m_wndWP.SetItemData(IDX, iMapID);
	return IDX >= 0;
}
