// AssistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShopbotCfg.h"
#include "AssistDlg.h"
#include "SBItemConfig.h"
#include "D2ModParser.h"
#include "SpellDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssistDlg dialog


CAssistDlg::CAssistDlg(int nType, CWnd* pParent /*=NULL*/)
	: CDialog(CAssistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssistDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwData = 0;
	m_nType = nType;
}


void CAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssistDlg)
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAssistDlg, CDialog)
	//{{AFX_MSG_MAP(CAssistDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssistDlg message handlers

BOOL CAssistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_wndOK.SetIcon(IDI_YES);
	m_wndCancel.SetIcon(IDI_NO);

	m_wndList.InsertColumn(0, _T("Options"), LVCFMT_LEFT, 300);
	m_wndList.SetHeaderImageList(IDB_BITMAP1);
	m_wndList.SetHeaderImage(0, 2);
	switch (m_nType)
	{
	case ASSIST_CLASS:
		ListClass();
		break;

	case ASSIST_TAB:
		ListTab();
		break;

	case ASSIST_AMA_SKILL:
		ListAmaSkill();
		break;

	case ASSIST_ASN_SKILL:
		ListAsnSkill();
		break;

	case ASSIST_BAR_SKILL:
		ListBarSkill();
		break;

	case ASSIST_DRU_SKILL:
		ListDruSkill();
		break;

	case ASSIST_NEC_SKILL:
		ListNecSkill();
		break;

	case ASSIST_PAL_SKILL:
		ListPalSKill();
		break;

	case ASSIST_SOR_SKILL:
		ListSorSKill();
		break;

	default:
		break;
	}

	m_wndList.SortItems(0, TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAssistDlg::ListClass()
{
	int nIdx = m_wndList.InsertItem(INT_MAX, _T("Amazon"));
	m_wndList.SetItemData(nIdx, 0);
	nIdx = m_wndList.InsertItem(INT_MAX, _T("Sorceress"));
	m_wndList.SetItemData(nIdx, 1);
	nIdx = m_wndList.InsertItem(INT_MAX, _T("Necromancer"));
	m_wndList.SetItemData(nIdx, 2);
	nIdx = m_wndList.InsertItem(INT_MAX, _T("Paladin"));
	m_wndList.SetItemData(nIdx, 3);
	nIdx = m_wndList.InsertItem(INT_MAX, _T("Barbarian"));
	m_wndList.SetItemData(nIdx, 4);
	nIdx = m_wndList.InsertItem(INT_MAX, _T("Druid"));
	m_wndList.SetItemData(nIdx, 5);
	nIdx = m_wndList.InsertItem(INT_MAX, _T("Assassin"));
	m_wndList.SetItemData(nIdx, 6);
}

void CAssistDlg::ListTab()
{
	CString sTab;
	for (int i = 0; SB_SKILLTABS[i] != -1; i++)
	{		
		if (GetSkillTabName(SB_SKILLTABS[i], sTab))
		{
			LPCTSTR lpsz = sTab;
			int nIdx = m_wndList.InsertItem(INT_MAX, &lpsz[8]);
			m_wndList.SetItemData(nIdx, SB_SKILLTABS[i]);
		}
	}
}

void CAssistDlg::ListAmaSkill()
{
	// Javelin and Spear spells
	ListSkill(D2S_JAB);
	ListSkill(D2S_IMPALE);
	ListSkill(D2S_FEND);
	ListSkill(D2S_POWERSTRIKE);
	ListSkill(D2S_CHARGEDSTRIKE);
	ListSkill(D2S_LIGHTNINGSTRIKE);
	ListSkill(D2S_POISONJAVELIN);
	ListSkill(D2S_LIGHTNINGBOLT);
	ListSkill(D2S_PLAGUEJAVELIN);
	ListSkill(D2S_LIGHTNINGFURY);

	// Passive and Magic spells
	ListSkill(D2S_INNERSIGHT);
	ListSkill(D2S_SLOWMISSILES);
	ListSkill(D2S_DECOY);
	ListSkill(D2S_VALKYRIE);
	ListSkill(D2S_DODGE);
	ListSkill(D2S_AVOID);
	ListSkill(D2S_EVADE);
	ListSkill(D2S_CRITICALSTRIKE);
	ListSkill(D2S_PENETRATE);
	ListSkill(D2S_PIERCE);

	// Bow and Crossbow spells
	ListSkill(D2S_COLDARROW);
	ListSkill(D2S_ICEARROW);
	ListSkill(D2S_FREEZINGARROW);
	ListSkill(D2S_MAGICARROW);
	ListSkill(D2S_MULTIPLESHOT);
	ListSkill(D2S_GUIDEDARROW);
	ListSkill(D2S_STRAFE);
	ListSkill(D2S_FIREARROW);
	ListSkill(D2S_EXPLODINGARROW);
	ListSkill(D2S_IMMOLATIONARROW);
}

void CAssistDlg::ListBarSkill()
{
	// Warcries
	ListSkill(D2S_HOWL);
	ListSkill(D2S_TAUNT);
	ListSkill(D2S_BATTLECRY);
	ListSkill(D2S_WARCRY);
	ListSkill(D2S_SHOUT);
	ListSkill(D2S_BATTLEORDERS);
	ListSkill(D2S_BATTLECOMMAND);
	ListSkill(D2S_FINDPOTION);
	ListSkill(D2S_FINDITEM);
	ListSkill(D2S_GRIMWARD);

	// Combat Masteries
	ListSkill(D2S_SWORDMASTERY);
	ListSkill(D2S_POLEARMMASTERY);
	ListSkill(D2S_INCREASEDSTAMINA);
	ListSkill(D2S_INCREASEDSPEED);
	ListSkill(D2S_AXEMASTERY);
	ListSkill(D2S_THROWINGMASTERY);
	ListSkill(D2S_MACEMASTERY);
	ListSkill(D2S_SPEARMASTERY);
	ListSkill(D2S_IRONSKIN);
	ListSkill(D2S_NATURALRESISTANCE);

	// Combat spells
	ListSkill(D2S_LEAP);
	ListSkill(D2S_LEAPATTACK);
	ListSkill(D2S_WHIRLWIND);
	ListSkill(D2S_BASH);
	ListSkill(D2S_STUN);
	ListSkill(D2S_CONCENTRATE);
	ListSkill(D2S_BERSERK);
	ListSkill(D2S_DOUBLESWING);
	ListSkill(D2S_DOUBLETHROW);
	ListSkill(D2S_FRENZY);
}

void CAssistDlg::ListDruSkill()
{
	// Elemental
	ListSkill(D2S_FIRESTORM);
	ListSkill(D2S_MOLTENBOULDER);
	ListSkill(D2S_FISSURE);
	ListSkill(D2S_VOLCANO);
	ListSkill(D2S_ARMAGEDDON);
	ListSkill(D2S_TWISTER);
	ListSkill(D2S_TORNADO);
	ListSkill(D2S_HURRICANE);
	ListSkill(D2S_ARCTICBLAST);
	ListSkill(D2S_CYCLONEARMOR);

	// Shape Shifting
	ListSkill(D2S_WEREWOLF);
	ListSkill(D2S_FERALRAGE);
	ListSkill(D2S_RABIES);
	ListSkill(D2S_FURY);
	ListSkill(D2S_LYCANTHROPY);
	ListSkill(D2S_FIRECLAWS);
	ListSkill(D2S_HUNGER);
	ListSkill(D2S_WEREBEAR);
	ListSkill(D2S_MAUL);
	ListSkill(D2S_SHOCKWAVE);

	// Summoning
	ListSkill(D2S_OAKSAGE);
	ListSkill(D2S_HEARTOFWOLVERINE);
	ListSkill(D2S_SPIRITOFBARBS);
	ListSkill(D2S_RAVEN);
	ListSkill(D2S_SUMMONSPIRITWOLF);
	ListSkill(D2S_SUMMONDIREWOLF);
	ListSkill(D2S_SUMMONGRIZZLY);
	ListSkill(D2S_POISONCREEPER);
	ListSkill(D2S_CARRIONVINE);
	ListSkill(D2S_SOLARCREEPER);
}

void CAssistDlg::ListNecSkill()
{
	// Summoning Spells
	ListSkill(D2S_SKELETONMASTERY);
	ListSkill(D2S_GOLEMMASTERY);
	ListSkill(D2S_SUMMONRESIST);
	ListSkill(D2S_CLAYGOLEM);
	ListSkill(D2S_BLOODGOLEM);
	ListSkill(D2S_IRONGOLEM);
	ListSkill(D2S_FIREGOLEM);
	ListSkill(D2S_RAISESKELETON);
	ListSkill(D2S_RAISESKELETALMAGE);
	ListSkill(D2S_REVIVE);

	// Poison and Bone Spells
	ListSkill(D2S_POISONDAGGER);
	ListSkill(D2S_POISONEXPLOSION);
	ListSkill(D2S_POISONNOVA);
	ListSkill(D2S_TEETH);
	ListSkill(D2S_CORPOSEEXPLOSION);
	ListSkill(D2S_BONESPEAR);
	ListSkill(D2S_BONESPIRIT);
	ListSkill(D2S_BONEARMOR);
	ListSkill(D2S_BONEWALL);
	ListSkill(D2S_BONEPRISON);

	// Curses
	ListSkill(D2S_DIMVISION);
	ListSkill(D2S_CONFUSE);
	ListSkill(D2S_ATTRACT);
	ListSkill(D2S_AMPLIFYDAMAGE);
	ListSkill(D2S_IRONMAIDEN);
	ListSkill(D2S_LIFETAP);
	ListSkill(D2S_LOWERRESIST);
	ListSkill(D2S_WEAKEN);
	ListSkill(D2S_TERROR);
	ListSkill(D2S_DECREPIFY);
}

void CAssistDlg::ListPalSKill()
{
	// Defensive Auras
	ListSkill(D2S_PRAYER);
	ListSkill(D2S_CLEANSING);
	ListSkill(D2S_MEDITATION);
	ListSkill(D2S_DEFIANCE);
	ListSkill(D2S_VIGOR);
	ListSkill(D2S_REDEMPTION);
	ListSkill(D2S_RESISTFIRE);
	ListSkill(D2S_RESISTCOLD);
	ListSkill(D2S_RESISTLIGHTNING);
	ListSkill(D2S_SALVATION);

// Offensive Auras
	ListSkill(D2S_MIGHT);
	ListSkill(D2S_BLESSEDAIM);
	ListSkill(D2S_CONCENTRATION);
	ListSkill(D2S_FANATICISM);
	ListSkill(D2S_HOLYFIRE);
	ListSkill(D2S_HOLYFREEZE);
	ListSkill(D2S_HOLYSHOCK);
	ListSkill(D2S_THORNS);
	ListSkill(D2S_SANCTUARY);
	ListSkill(D2S_CONVICTION);

// Combat spells
	ListSkill(D2S_SACRIFICE);
	ListSkill(D2S_ZEAL);
	ListSkill(D2S_VENGEANCE);
	ListSkill(D2S_CONVERSION);
	ListSkill(D2S_HOLYBOLT);
	ListSkill(D2S_BLESSEDHAMMER);
	ListSkill(D2S_FISTOFTHEHEAVENS);
	ListSkill(D2S_SMITE);
	ListSkill(D2S_CHARGE);
	ListSkill(D2S_HOLYSHIELD);
}

void CAssistDlg::ListAsnSkill()
{
	// Martial Arts
	ListSkill(D2S_FISTSOFFIRE);
	ListSkill(D2S_CLAWSOFTHUNDER);
	ListSkill(D2S_BLADESOFICE);
	ListSkill(D2S_TIGERSTRIKE);
	ListSkill(D2S_COBRASTRIKE);
	ListSkill(D2S_PHOENIXSTRIKE);
	ListSkill(D2S_DRAGONTALON);
	ListSkill(D2S_DRAGONCLAW);
	ListSkill(D2S_DRAGONTAIL);
	ListSkill(D2S_DRAGONFLIGHT);

// Shadow Disciplines
	ListSkill(D2S_BURSTOFSPEED);
	ListSkill(D2S_FADE);
	ListSkill(D2S_VENOM);
	ListSkill(D2S_CLAWMASTERY);
	ListSkill(D2S_WEAPONBLOCK);
	ListSkill(D2S_SHADOWWARRIOR);
	ListSkill(D2S_SHADOWMASTER);
	ListSkill(D2S_PSYCHICHAMMER);
	ListSkill(D2S_CLOAKOFSHADOWS);
	ListSkill(D2S_MINDBLAST);

// Traps
	ListSkill(D2S_SHOCKWEB);
	ListSkill(D2S_CHARGEDBOLTSENTRY);
	ListSkill(D2S_LIGHTNINGSENTRY);
	ListSkill(D2S_DEATHSENTRY);
	ListSkill(D2S_FIREBLAST);
	ListSkill(D2S_WAKEOFFIRE);
	ListSkill(D2S_WAKEOFINFERNO);
	ListSkill(D2S_BLADESENTINEL);
	ListSkill(D2S_BLADEFURY);
	ListSkill(D2S_BLADESHIELD);
}

void CAssistDlg::ListSorSKill()
{
	// Cold Spells
	ListSkill(D2S_FROSTNOVA);
	ListSkill(D2S_BLIZZARD);
	ListSkill(D2S_FROZENORB);
	ListSkill(D2S_ICEBOLT);
	ListSkill(D2S_ICEBLAST);
	ListSkill(D2S_GLACIALSPIKE);
	ListSkill(D2S_COLDMASTERY);
	ListSkill(D2S_FROZENARMOR);
	ListSkill(D2S_SHIVERARMOR);
	ListSkill(D2S_CHILLINGARMOR);

// Lightning Spells
	ListSkill(D2S_STATICFIELD);
	ListSkill(D2S_NOVA);
	ListSkill(D2S_THUNDERSTORM);
	ListSkill(D2S_CHARGEDBOLT);
	ListSkill(D2S_LIGHTNING);
	ListSkill(D2S_CHAINLIGHTNING);
	ListSkill(D2S_LIGHTNINGMASTERY);
	ListSkill(D2S_TELEKINESIS);
	ListSkill(D2S_TELEPORT);
	ListSkill(D2S_ENERGYSHIELD);

// Fire Spells
	ListSkill(D2S_INFERNO);
	ListSkill(D2S_BLAZE);
	ListSkill(D2S_FIREWALL);
	ListSkill(D2S_FIREBOLT);
	ListSkill(D2S_FIREBALL);
	ListSkill(D2S_METEOR);
	ListSkill(D2S_FIREMASTERY);
	ListSkill(D2S_WARMTH);
	ListSkill(D2S_ENCHANT);
	ListSkill(D2S_HYDRA);
}

void CAssistDlg::OnOK() 
{
	// TODO: Add extra validation here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	m_dwData = m_wndList.GetItemData(IDX);
	EndDialog(IDOK);
}

void CAssistDlg::ListSkill(WORD wSkillID)
{
	D2SPELLINFO dsi = {0};
	if (D2GetSpellInfo(wSkillID, &dsi))
	{
		const int IDX = m_wndList.InsertItem(INT_MAX, dsi.szSpellName);
		m_wndList.SetItemData(IDX, wSkillID);
	}
}

DWORD CAssistDlg::GetData() const
{
	return m_dwData;
}

void CAssistDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_wndOK.EnableWindow(m_wndList.GetFirstItem(RC_ITEM_SELECTED) != -1);
	*pResult = 0;
}

void CAssistDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnOK();
	*pResult = 0;
}
