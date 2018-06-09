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
	{0, _T("+(x) to Strength")},
	{1, _T("+(x) to Energy")},
	{2, _T("+(x) to Dexterity")},
	{3, _T("+(x) to Vitality")},
	{7, _T("+(x) to Life")},
	{9, _T("+(x) to Mana")},
	{16, _T("+(x)%% Enhanced Defense")},
	{17, _T("+(x)%% Enhanced Damage ")},
	{19, _T("+(x) to Attack Rating")},
	{20, _T("+(x)%% Increased Chance of Blocking ")},
	{31, _T("+(x) Defense")},
	{34, _T("Damage Reduced by (x)")},
	{35, _T("Magic Damage Reduced by (x)")},
	{39, _T("Fire Resist +(x)%%")},
	{41, _T("Lightning Resist +(x)%%")},
	{43, _T("Cold Resist +(x)%%")},
	{45, _T("Poison Resist +(x)%%")},
	{60, _T("(x)%% Life Stolen Per Hit")},
	{62, _T("(x)%% Mana Stolen Per Hit")},
	{74, _T("Replenish Life +(x)")},
	{80, _T("(x)%% Better Chance of Getting Magic Items")},
	{81, _T("Knockback")},
	{86, _T("+(x) Life after each Kill")},
	{138, _T("+(x) to Mana After Each Kill")},
	{91, _T("Requirements -(x)%%")},
	{93, _T("+(x)%% Increased Attack Speed")},
	{96, _T("+(x)%% Faster Run/Walk")},
	{99, _T("+(x)%% Faster Hit Recovery")},
	{102, _T("+(x)%% Faster Block Rate")},
	{105, _T("+(x)%% Faster Cast Rate")},
	{110, _T("Poison Length Reduced by (x)%%")},
	{112, _T("Hit Causes Monster to Flee (x)%%")},
	{114, _T("(x)%% Damage Taken Goes to Mana")},
	{115, _T("Ignore Target Defense")},
	//{116, _T("-(x)%% Target Defense")},
	{117, _T("Prevent Monster Heal")},
	{118, _T("Half Freeze Duration")},
	{23, _T("+(x) to Minimum Damage")},
	{24, _T("+(x) to Maximum Damage")},

	{48, _T("Adds (x)-(y) Fire Damage")},
	{50, _T("Adds (x)-(y) Lightning Damage")},
	{54, _T("Adds (x)-(y) Cold Damage")},
	{57, _T("+(y) Poison Damage over (x) Seconds")},

	{195, _T("(z)%% Chance to Cast Level (x) (y) on Attack")},
	{198, _T("(z)%% Chance to Cast Level (x) (y) on Striking")},
	{201, _T("(z)%% Chance to Cast Level (x) (y) when Struck")},
	
	{188, _T("+(y) to (x) Skills (Skill-Tree)")},
	{83, _T("+(y) to (x) Skills (Character Class)")},
	{107, _T("+(y) to (x) (Single SKill)")},
	{204, _T("Level (x) (y) ((z) Charges)")},
	{511, _T("")}
};

BOOL GetSkillTabName(DWORD dwSkillTab, CString& sResult);
BOOL GetItemModDesc(LPCITEMMOD lpMod, CString& sResult);
BOOL IsValidItemSkill(WORD wSkillID);
int GetModVars(const CString& sMod);

#endif