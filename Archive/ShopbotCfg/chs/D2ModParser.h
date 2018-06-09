#ifndef __D2MODPARSER_H__
#define __D2MODPARSER_H__

#include "item.h"

// Skill Tab
#define SB_TAB_AMA_JAVLINEANDSPEAR		2
#define SB_TAB_AMA_PASSIVEANDMAGIC		1
#define SB_TAB_AMA_BOWANDCROSSBOW		0
#define SB_TAB_ASN_MARTIALARTS			50
#define SB_TAB_ASN_SHADOWDISCIPLINES	49
#define SB_TAB_ASN_TRAPS				48
#define SB_TAB_NEC_SUMMONING			18
#define SB_TAB_NEC_POISONANDBONE		17
#define SB_TAB_NEC_CURSES				16
#define SB_TAB_BAR_WARCRIES				34
#define SB_TAB_BAR_COMBATMASTERIES		33
#define SB_TAB_BAR_COMBAT				32
#define SB_TAB_PAL_DEFENSIVEAURAS		26
#define SB_TAB_PAL_OFFENSIVEAURAS		25
#define SB_TAB_PAL_COMBAT				24
#define SB_TAB_SOR_COLD					10
#define SB_TAB_SOR_LIGHTNING			9
#define SB_TAB_SOR_FIRE					8
#define SB_TAB_DRU_ELEMENTAL			42
#define SB_TAB_DRU_SHAPESHIFTING		41
#define SB_TAB_DRU_SUMMONING			40

static const DWORD SB_SKILLTABS[] = {0, 1, 2, 8, 9, 10, 16, 17, 18, 24, 25, 26, 32, 33, 34, 40, 41, 42, 48, 49, 50, -1 };


typedef struct tagModDesc
{
	DWORD dwModID;
	TCHAR szDesc[256];
} MODDESC, *LPMODDESC;
typedef const tagModDesc* LPCMODDESC;

// Mods that are only relevant to Abin Shopbot 1.10
static const MODDESC MOD_TABLE[] = 
{
	{0, _T("+(x) ǿ׳")},
	{1, _T("+(x) �ǻ�")},
	{2, _T("+(x) ����")},
	{3, _T("+(x) ����")},
	{7, _T("+(x) ����")},
	{9, _T("+(x) ħ��")},
	{16, _T("+(x)%% ��ǿ����")},
	{17, _T("+(x)%% ��߹�����(ED)")},
	{19, _T("+(x) ���������")},
	{20, _T("+(x)%% ��߶��Ƹ�")},
	{31, _T("+(x) ����")},
	{34, _T("�����˺����� (x)")},
	{35, _T("ħ���˺����� (x)")},
	{39, _T("��ϵ���� +(x)%%")},
	{41, _T("��ϵ���� +(x)%%")},
	{43, _T("��ϵ���� +(x)%%")},
	{45, _T("��ϵ���� +(x)%%")},
	{60, _T("(x)%% ͵Ѫ(Life Leech)")},
	{62, _T("(x)%% ͵ħ��(Mana Leech)")},
	{74, _T("�����ظ� +(x)")},
	{80, _T("(x)%% ���ʻ��ħ����Ʒ(Magic Find)")},
	{81, _T("����")},
	{86, _T("+(x) Ѫ��ÿ��ɱ���Ĺ���")},
	{138, _T("+(x) ħ����ÿ��ɱ���Ĺ���")},
	{91, _T("����Ҫ�󽵵� (x)%%")},
	{93, _T("+(x)%% ��߹����ٶ�(IAS)")},
	{96, _T("+(x)%% ���ٱ���/�����ٶ�")},
	{99, _T("+(x)%% ���ٻ���ָ�")},
	{102, _T("+(x)%% ���ٸ��ٶ�")},
	{105, _T("+(x)%% ����ʩ���ٶ�")},
	{110, _T("�ж�ʱ����� (x)%%")},
	{112, _T("���к�ʹ�������� (x)%%")},
	{114, _T("(x)%% �ܵ����˺�ת��Ϊħ��")},
	{115, _T("���ӷ���(ITD)")},
	//{116, _T("-(x)%% Target Defense")},
	{117, _T("��ֹ��������")},
	{118, _T("����ʱ�����")},
	{23, _T("+(x) ��С������")},
	{24, _T("+(x) ��󹥻���")},

	{48, _T("���� (x)-(y) ��ϵ������")},
	{50, _T("���� (x)-(y) ��ϵ������")},
	{54, _T("���� (x)-(y) ��ϵ������")},
	{57, _T("+(y) ��ϵ���������� (x) ��")},

	{195, _T("(z)%% �����ͷ� (x) ���� (y) on Attack")},
	{198, _T("(z)%% �����ͷ� (x) ���� (y) on Striking")},
	{201, _T("(z)%% �����ͷ� (x) ���� (y) when Struck")},
	
	{188, _T("+(y) �� (x) ���� (������)")},
	{83, _T("+(y) �� (x) ���� (��ɫ����)")},
	{107, _T("+(y) �� (x) ���� (�����)")},
	{204, _T("(x) �� (y) (��ʹ�� (z)/(z) ��)")},
	{511, _T("")}
};

BOOL GetSkillTabName(DWORD dwSkillTab, CString& sResult);
BOOL GetItemModDesc(LPCITEMMOD lpMod, CString& sResult);
BOOL IsValidItemSkill(WORD wSkillID);
int GetModVars(const CString& sMod);

#endif