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
	{0, _T("+(x) 强壮")},
	{1, _T("+(x) 智慧")},
	{2, _T("+(x) 敏捷")},
	{3, _T("+(x) 活力")},
	{7, _T("+(x) 生命")},
	{9, _T("+(x) 魔法")},
	{16, _T("+(x)%% 增强防御")},
	{17, _T("+(x)%% 提高攻击力(ED)")},
	{19, _T("+(x) 提高命中率")},
	{20, _T("+(x)%% 提高盾牌格挡")},
	{31, _T("+(x) 防御")},
	{34, _T("物理伤害减少 (x)")},
	{35, _T("魔法伤害减少 (x)")},
	{39, _T("火系抗性 +(x)%%")},
	{41, _T("电系抗性 +(x)%%")},
	{43, _T("冰系抗性 +(x)%%")},
	{45, _T("毒系抗性 +(x)%%")},
	{60, _T("(x)%% 偷血(Life Leech)")},
	{62, _T("(x)%% 偷魔法(Mana Leech)")},
	{74, _T("生命回复 +(x)")},
	{80, _T("(x)%% 几率获得魔法物品(Magic Find)")},
	{81, _T("震退")},
	{86, _T("+(x) 血从每个杀死的怪物")},
	{138, _T("+(x) 魔法从每个杀死的怪物")},
	{91, _T("属性要求降低 (x)%%")},
	{93, _T("+(x)%% 提高攻击速度(IAS)")},
	{96, _T("+(x)%% 快速奔跑/行走速度")},
	{99, _T("+(x)%% 快速击打恢复")},
	{102, _T("+(x)%% 快速格挡速度")},
	{105, _T("+(x)%% 快速施法速度")},
	{110, _T("中毒时间减少 (x)%%")},
	{112, _T("击中后使怪物逃逸 (x)%%")},
	{114, _T("(x)%% 受到的伤害转变为魔法")},
	{115, _T("无视防御(ITD)")},
	//{116, _T("-(x)%% Target Defense")},
	{117, _T("防止怪物自疗")},
	{118, _T("冰冻时间减半")},
	{23, _T("+(x) 最小攻击力")},
	{24, _T("+(x) 最大攻击力")},

	{48, _T("增加 (x)-(y) 火系攻击力")},
	{50, _T("增加 (x)-(y) 电系攻击力")},
	{54, _T("增加 (x)-(y) 冰系攻击力")},
	{57, _T("+(y) 毒系攻击力持续 (x) 秒")},

	{195, _T("(z)%% 几率释放 (x) 级的 (y) on Attack")},
	{198, _T("(z)%% 几率释放 (x) 级的 (y) on Striking")},
	{201, _T("(z)%% 几率释放 (x) 级的 (y) when Struck")},
	
	{188, _T("+(y) 级 (x) 技能 (技能树)")},
	{83, _T("+(y) 级 (x) 技能 (角色技能)")},
	{107, _T("+(y) 级 (x) 技能 (单项技能)")},
	{204, _T("(x) 级 (y) (可使用 (z)/(z) 次)")},
	{511, _T("")}
};

BOOL GetSkillTabName(DWORD dwSkillTab, CString& sResult);
BOOL GetItemModDesc(LPCITEMMOD lpMod, CString& sResult);
BOOL IsValidItemSkill(WORD wSkillID);
int GetModVars(const CString& sMod);

#endif