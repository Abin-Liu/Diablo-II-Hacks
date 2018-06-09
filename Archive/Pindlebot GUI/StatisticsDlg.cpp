// StatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pindlebot.h"
#include "StatisticsDlg.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg dialog


CStatisticsDlg::CStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatisticsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsDlg)
	m_sCharDied = _T("");
	m_sMercDied = _T("");
	m_sPindleKilled = _T("");
	m_sRetreated = _T("");
	m_sDeadlyCombo = _T("");
	m_sInsufficientSkill = _T("");
	m_sNetEffect = _T("");
	m_sTotalTime = _T("");
	m_sTotalGames = _T("");
	m_sPotionDrunk = _T("");
	m_sPotionMerc = _T("");
	//}}AFX_DATA_INIT
}


void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsDlg)
	DDX_Text(pDX, IDC_CHARDIED, m_sCharDied);
	DDX_Text(pDX, IDC_MERCDIED, m_sMercDied);
	DDX_Text(pDX, IDC_PINDLEKILLED, m_sPindleKilled);
	DDX_Text(pDX, IDC_RETREATED, m_sRetreated);
	DDX_Text(pDX, IDC_DEADLYCOMBO, m_sDeadlyCombo);
	DDX_Text(pDX, IDC_INSUFFICIENTSKILL, m_sInsufficientSkill);
	DDX_Text(pDX, IDC_NETEFFECTIVE, m_sNetEffect);
	DDX_Text(pDX, IDC_TOTALTIME, m_sTotalTime);
	DDX_Text(pDX, IDC_TOTALGAMES, m_sTotalGames);
	DDX_Text(pDX, IDC_POTIONDRUNK, m_sPotionDrunk);
	DDX_Text(pDX, IDC_POTIONMERC, m_sPotionMerc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialog)
	//{{AFX_MSG_MAP(CStatisticsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg message handlers

CString CStatisticsDlg::GetPercent(int n, int nTotal) const
{
	if (nTotal == 0)
		return _T("");

	float f = (float)n / (float)nTotal * 100.0f;
	if (f > 100.0)
		f = 100.0;
	
	CString str;
	str.Format(_T("(%.1f%%)"), f);
	return str;
}

BOOL CStatisticsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sTitle(_T("本次运行结果统计"));
	SetWindowText(sTitle);
	
	CIni ini(AfxGetAppEx()->GetAppDir() + _T("\\pindlebot.txt"));
	// read and display data from pindlebot.txt

	int nTotalGames = ini.GetInt(_T("Log"), _T("Game"), 0);
	if (ini.GetBool(_T("Log"), _T("Running"), FALSE))
		nTotalGames -= 1;

	DWORD dwRunTime = ini.GetInt(_T("Log"), _T("Run Time"), 0);

	m_sTotalGames.Format(_T("%d"), nTotalGames);
	m_sTotalTime.Format(_T("%d小时%d分%d秒"), dwRunTime / 3600, (dwRunTime % 3600) / 60, (dwRunTime % 3600) % 60);

	float f = 0.0;

	if (nTotalGames > 0)
		f = (float)dwRunTime / (float)nTotalGames;
	
	m_sNetEffect.Format(_T("%u秒/游戏"), (long)f);

	int nData = ini.GetInt(_T("Log"), _T("Succeed"), 0);
	m_sPindleKilled.Format(_T("%d %s"), nData, GetPercent(nData, nTotalGames));

	nData = ini.GetInt(_T("Log"), _T("Merc Die"), 0);
	m_sMercDied.Format(_T("%d %s"), nData, GetPercent(nData, nTotalGames));

	nData = ini.GetInt(_T("Log"), _T("Char Die"), 0);
	m_sCharDied.Format(_T("%d %s"), nData, GetPercent(nData, nTotalGames));

	nData = ini.GetInt(_T("Log"), _T("Retreat"), 0);
	m_sRetreated.Format(_T("%d %s"), nData, GetPercent(nData, nTotalGames));

	nData = ini.GetInt(_T("Log"), _T("Avoid"), 0);
	m_sDeadlyCombo.Format(_T("%d %s"), nData, GetPercent(nData, nTotalGames));
	
	nData = ini.GetInt(_T("Log"), _T("Ins Skill"), 0);
	m_sInsufficientSkill.Format(_T("%d %s"), nData, GetPercent(nData, nTotalGames));

	nData = ini.GetInt(_T("Log"), _T("Potion Drunk"), 0);
	m_sPotionDrunk.Format(_T("%d 瓶"), nData);

	nData = ini.GetInt(_T("Log"), _T("Potion Merc"), 0);
	m_sPotionMerc.Format(_T("%d 瓶"), nData);	

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
