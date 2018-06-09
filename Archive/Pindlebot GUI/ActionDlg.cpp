// ActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "ActionDlg.h"
#include "CharDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionDlg dialog
CActionDlg::CActionDlg(int nType, LPPBOTACTION pAction, BYTE iCharClass, CWnd* pParent /*=NULL*/)
	: CDialog(CActionDlg::IDD, pParent)
{
	m_nType = nType;
	m_pAction = pAction;
	m_iCharClass = iCharClass;
	ASSERT(m_pAction);	

	//{{AFX_DATA_INIT(CActionDlg)
	m_nLeftRight = pAction->iLeft ? 0 : 1;
	m_iRepeat = pAction->iTimes;
	m_wDelay = pAction->wDelay;
	//}}AFX_DATA_INIT
}


void CActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionDlg)
	DDX_Control(pDX, IDC_TARGET, m_wndTarget);
	DDX_Control(pDX, IDC_SKILLS, m_wndSpells);
	DDX_Radio(pDX, IDC_LEFTRIGHT, m_nLeftRight);
	DDX_Text(pDX, IDC_REPEAT, m_iRepeat);
	DDV_MinMaxByte(pDX, m_iRepeat, 1, 255);
	DDX_Text(pDX, IDC_DELAY, m_wDelay);
	DDV_MinMaxUInt(pDX, m_wDelay, 0, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionDlg, CDialog)
	//{{AFX_MSG_MAP(CActionDlg)
	ON_CBN_SELCHANGE(IDC_SKILLS, OnSelchangeSkills)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionDlg message handlers

BOOL CActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitSpells();
	m_wndSpells.ResetContent();
	int i = 0;
	for (i = 0; i < m_aSpells.GetSize(); i++)
	{
		if (IsSpellAllowed(m_aSpells[i]))
		{
			const int IDX = m_wndSpells.AddString(m_aSpells[i].szSpellName);
			m_wndSpells.SetItemData(IDX, (DWORD)i);
		}
	}

	m_wndSpells.SetCurSel(0);
	for (i = 0; i < m_wndSpells.GetCount(); i++)
	{
		const int IDX = (int)m_wndSpells.GetItemData(i);
		if (m_aSpells.IsIndexValid(IDX) && m_aSpells[IDX].wSpellID == m_pAction->wSpellID)
		{
			m_wndSpells.SetCurSel(i);
			break;
		}
	}
	
	m_wndTarget.SetCurSel(m_pAction->iTarget);
	CheckStats();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CActionDlg::InitSpells()
{
	m_aSpells.RemoveAll();
	int nIndex = 0;

	// Common spells (melee and throwing)
	D2SPELLINFO dsi;
	::D2GetSpellInfo(D2S_ATTACK, &dsi);
	m_aSpells.Add(dsi);
	::D2GetSpellInfo(D2S_THROW, &dsi);
	m_aSpells.Add(dsi);

	switch (m_iCharClass)
	{
	case CCD_AMAZON:

		// Javelin and Spear Skills
		::D2GetSpellInfo(D2S_JAB, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_IMPALE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FEND, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_POWERSTRIKE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CHARGEDSTRIKE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LIGHTNINGSTRIKE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_POISONJAVELIN, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LIGHTNINGBOLT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_PLAGUEJAVELIN, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LIGHTNINGFURY, &dsi);
		m_aSpells.Add(dsi);

		// Passive and Magic Skills
		::D2GetSpellInfo(D2S_INNERSIGHT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SLOWMISSILES, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DECOY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_VALKYRIE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DODGE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_AVOID, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_EVADE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CRITICALSTRIKE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_PENETRATE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_PIERCE, &dsi);
		m_aSpells.Add(dsi);

		// Bow and Crossbow Skills
		::D2GetSpellInfo(D2S_COLDARROW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ICEARROW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FREEZINGARROW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_MAGICARROW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_MULTIPLESHOT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_GUIDEDARROW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_STRAFE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FIREARROW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_EXPLODINGARROW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_IMMOLATIONARROW, &dsi);
		m_aSpells.Add(dsi);
		break;

	case CCD_ASSASSIN:

		// Martial Arts
		::D2GetSpellInfo(D2S_FISTSOFFIRE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CLAWSOFTHUNDER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLADESOFICE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_TIGERSTRIKE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_COBRASTRIKE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_PHOENIXSTRIKE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DRAGONTALON, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DRAGONCLAW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DRAGONTAIL, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DRAGONFLIGHT, &dsi);
		m_aSpells.Add(dsi);

		// Shadow Disciplines
		::D2GetSpellInfo(D2S_BURSTOFSPEED, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FADE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_VENOM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CLAWMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_WEAPONBLOCK, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SHADOWWARRIOR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SHADOWMASTER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_PSYCHICHAMMER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CLOAKOFSHADOWS, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_MINDBLAST, &dsi);
		m_aSpells.Add(dsi);

		// Traps
		::D2GetSpellInfo(D2S_SHOCKWEB, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CHARGEDBOLTSENTRY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LIGHTNINGSENTRY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DEATHSENTRY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FIREBLAST, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_WAKEOFFIRE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_WAKEOFINFERNO, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLADESENTINEL, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLADEFURY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLADESHIELD, &dsi);
		m_aSpells.Add(dsi);
		break;

	case CCD_BARBARIAN:

		// Warcries
		::D2GetSpellInfo(D2S_HOWL, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_TAUNT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BATTLECRY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_WARCRY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SHOUT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BATTLEORDERS, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BATTLECOMMAND, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FINDPOTION, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FINDITEM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_GRIMWARD, &dsi);
		m_aSpells.Add(dsi);

		// Combat Masteries
		::D2GetSpellInfo(D2S_SWORDMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_POLEARMMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_INCREASEDSTAMINA, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_INCREASEDSPEED, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_AXEMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_THROWINGMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_MACEMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SPEARMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_IRONSKIN, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_NATURALRESISTANCE, &dsi);
		m_aSpells.Add(dsi);

		// Combat Skills
		::D2GetSpellInfo(D2S_LEAP, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LEAPATTACK, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_WHIRLWIND, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BASH, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_STUN, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CONCENTRATE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BERSERK, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DOUBLESWING, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DOUBLETHROW, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FRENZY, &dsi);
		m_aSpells.Add(dsi);
		break;

	case CCD_DRUID:

		// Elemental
		::D2GetSpellInfo(D2S_FIRESTORM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_MOLTENBOULDER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FISSURE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_VOLCANO, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ARMAGEDDON, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_TWISTER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_TORNADO, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HURRICANE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ARCTICBLAST, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CYCLONEARMOR, &dsi);
		m_aSpells.Add(dsi);

		// Shape Shifting
		::D2GetSpellInfo(D2S_WEREWOLF, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FERALRAGE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_RABIES, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FURY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LYCANTHROPY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FIRECLAWS, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HUNGER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_WEREBEAR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_MAUL, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SHOCKWAVE, &dsi);
		m_aSpells.Add(dsi);

		// Summoning
		::D2GetSpellInfo(D2S_OAKSAGE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HEARTOFWOLVERINE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SPIRITOFBARBS, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_RAVEN, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SUMMONSPIRITWOLF, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SUMMONDIREWOLF, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SUMMONGRIZZLY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_POISONCREEPER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CARRIONVINE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SOLARCREEPER, &dsi);
		m_aSpells.Add(dsi);

		break;

	case CCD_NECROMANCER:

		// Summoning Spells
		::D2GetSpellInfo(D2S_SKELETONMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_GOLEMMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SUMMONRESIST, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CLAYGOLEM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLOODGOLEM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_IRONGOLEM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FIREGOLEM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_RAISESKELETON, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_RAISESKELETALMAGE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_REVIVE, &dsi);
		m_aSpells.Add(dsi);

		// and Bone Spells
		::D2GetSpellInfo(D2S_POISONDAGGER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_POISONEXPLOSION, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_POISONNOVA, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_TEETH, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CORPOSEEXPLOSION, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BONESPEAR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BONESPIRIT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BONEARMOR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BONEWALL, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BONEPRISON, &dsi);
		m_aSpells.Add(dsi);

		// Curses
		::D2GetSpellInfo(D2S_DIMVISION, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CONFUSE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ATTRACT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_AMPLIFYDAMAGE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_IRONMAIDEN, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LIFETAP, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LOWERRESIST, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_WEAKEN, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_TERROR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DECREPIFY, &dsi);
		m_aSpells.Add(dsi);

		break;

	case CCD_PALADIN:

		// Defensive Auras
		::D2GetSpellInfo(D2S_PRAYER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CLEANSING, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_MEDITATION, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_DEFIANCE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_VIGOR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_REDEMPTION, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_RESISTFIRE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_RESISTCOLD, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_RESISTLIGHTNING, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SALVATION, &dsi);
		m_aSpells.Add(dsi);

		// Offensive Auras
		::D2GetSpellInfo(D2S_MIGHT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLESSEDAIM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CONCENTRATION, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FANATICISM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HOLYFIRE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HOLYFREEZE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HOLYSHOCK, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_THORNS, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SANCTUARY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CONVICTION, &dsi);
		m_aSpells.Add(dsi);

		// Combat Skills
		::D2GetSpellInfo(D2S_SACRIFICE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ZEAL, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_VENGEANCE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CONVERSION, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HOLYBOLT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLESSEDHAMMER, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FISTOFTHEHEAVENS, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SMITE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CHARGE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HOLYSHIELD, &dsi);
		m_aSpells.Add(dsi);
		break;

	case CCD_SORCERESS:

		// Cold Spells
		::D2GetSpellInfo(D2S_FROSTNOVA, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLIZZARD, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FROZENORB, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ICEBOLT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ICEBLAST, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_GLACIALSPIKE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_COLDMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FROZENARMOR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_SHIVERARMOR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CHILLINGARMOR, &dsi);
		m_aSpells.Add(dsi);

		// Spells
		::D2GetSpellInfo(D2S_STATICFIELD, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_NOVA, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_THUNDERSTORM, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CHARGEDBOLT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LIGHTNING, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_CHAINLIGHTNING, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_LIGHTNINGMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_TELEKINESIS, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_TELEPORT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ENERGYSHIELD, &dsi);
		m_aSpells.Add(dsi);

		// Spells
		::D2GetSpellInfo(D2S_INFERNO, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_BLAZE, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FIREWALL, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FIREBOLT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FIREBALL, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_METEOR, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_FIREMASTERY, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_WARMTH, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_ENCHANT, &dsi);
		m_aSpells.Add(dsi);
		::D2GetSpellInfo(D2S_HYDRA, &dsi);
		m_aSpells.Add(dsi);

		break;

	default: // error handling
		return FALSE;
		break;
	}

	return TRUE;
}

BOOL CActionDlg::IsSpellAllowed(const D2SPELLINFO &dsi) const
{
	if (dsi.dwSpellInfoFlag & DSI_PASSIVE)
		return FALSE;
	
	BOOL bOK = FALSE;
	switch (m_nType)
	{
	case PB_ACTION_BEFORERED:
		bOK = dsi.dwSpellInfoFlag & DSI_ENABLEINTOWN;
		break;

	case PB_ACTION_INRED:		
	case PB_ACTION_ONPINDLE:
		bOK = TRUE;
		break;

	case PB_ACTION_ONCORPSE:
		bOK = dsi.dwSpellInfoFlag & DSI_NEEDCORPSE;
		break;

	default:
		break;
	}
	return bOK;
}

void CActionDlg::OnSelchangeSkills() 
{
	// TODO: Add your control notification handler code here
	CheckStats();
}

void CActionDlg::CheckStats()
{
	const int IDX = (int)m_wndSpells.GetItemData(m_wndSpells.GetCurSel());
	if (!m_aSpells.IsIndexValid(IDX))
		return;

	if (m_aSpells[IDX].dwSpellInfoFlag & DSI_RIGHTONLY)
	{
		m_nLeftRight = 1;
		GetDlgItem(IDC_LEFTRIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RIGHT)->EnableWindow(FALSE);		
	}
	else
	{
		GetDlgItem(IDC_LEFTRIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RIGHT)->EnableWindow(TRUE);	
	}

	if (m_aSpells[IDX].dwSpellInfoFlag & DSI_TARGETABLE)
	{
		m_wndTarget.EnableWindow(TRUE);
	}	
	else
	{
		m_wndTarget.SetCurSel(0);
		m_wndTarget.EnableWindow(FALSE);
	}

	CString str;
	str.Format(_T("该技能自然冷却时间为 %d 毫秒"), m_aSpells[IDX].dwCoolDown);
	SetDlgItemText(IDC_LBL, str);
	UpdateData(FALSE);
}

void CActionDlg::OnOK() 
{
	// TODO: Add extra validation here
	const int IDX = (int)m_wndSpells.GetItemData(m_wndSpells.GetCurSel());
	if (!m_aSpells.IsIndexValid(IDX))
		return;

	if (!UpdateData(TRUE))
		return;

	m_pAction->wSpellID = m_aSpells[IDX].wSpellID;
	m_pAction->iLeft = m_nLeftRight == 0 ? 0x01 : 0x00;
	m_pAction->iTarget = (BYTE)m_wndTarget.GetCurSel();
	m_pAction->wDelay = (WORD)m_wDelay;
	m_pAction->iTimes = m_iRepeat;
	EndDialog(IDOK);
}
