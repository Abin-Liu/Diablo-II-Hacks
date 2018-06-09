#include "stdafx.h"
#include "D2ModParser.h"
#include <tchar.h>
#include "SpellDef.h"

BOOL FindModDesc(DWORD dwModID, CString& sResult)
{
	sResult.Empty();
	for (int i = 0; MOD_TABLE[i].dwModID < 511; i++)
	{
		if (MOD_TABLE[i].dwModID == dwModID)
		{
			sResult = MOD_TABLE[i].szDesc;
			return TRUE;
		}
	}

	return FALSE;
}

int GetModVars(const CString& sMod)
{
	if (sMod.Find(_T("(z)")) != -1)
		return 3;

	if (sMod.Find(_T("(y)")) != -1)
		return 2;

	return 1;
}

BOOL IsValidItemSkill(WORD wSkillID)
{
	return wSkillID != D2S_INVALID
		&& wSkillID != D2S_ATTACK
		&& wSkillID != D2S_THROW
		&& wSkillID != D2S_UNSUMMON
		&& wSkillID != D2S_TOMEOFIDENTIFY
		&& wSkillID != D2S_SCROLLOFIDENTIFY
		&& wSkillID != D2S_TOMEOFTOWNPORTAL
		&& wSkillID != D2S_SCROLLOFTOWNPORTAL;
}

BOOL GetSkillTabName(DWORD dwSkillTab, CString& sResult)
{
	sResult.Empty();
	
	switch (dwSkillTab)
	{
	case SB_TAB_AMA_JAVLINEANDSPEAR:
		sResult = _T("+(x) to Javelin and Spear Skills (Amazon Only)");
		break;

	case SB_TAB_AMA_PASSIVEANDMAGIC:
		sResult = _T("+(x) to Passive and Magic Skills (Amazon Only)");
		break;

	case SB_TAB_AMA_BOWANDCROSSBOW:
		sResult = _T("+(x) to Bow and Crossbow Skills (Amazon Only)");
		break;

	case SB_TAB_ASN_MARTIALARTS:
		sResult = _T("+(x) to Martial Arts (Assassin Only)");
		break;

	case SB_TAB_ASN_SHADOWDISCIPLINES:
		sResult = _T("+(x) to Shadow Disciplines (Assassin Only)");
		break;

	case SB_TAB_ASN_TRAPS:
		sResult = _T("+(x) to Traps (Assassin Only)");
		break;

	case SB_TAB_NEC_SUMMONING:
		sResult = _T("+(x) to Summoning Spells (Necromancer Only)");
		break;

	case SB_TAB_NEC_POISONANDBONE:
		sResult = _T("+(x) to Poison and Bone Spells (Necromancer Only)");
		break;

	case SB_TAB_NEC_CURSES:
		sResult = _T("+(x) to Curses (Necromancer Only)");
		break;

	case SB_TAB_BAR_WARCRIES:
		sResult = _T("+(x) to Warcries (Barbarian Only)");
		break;

	case SB_TAB_BAR_COMBATMASTERIES:
		sResult = _T("+(x) to Combat Masteries (Barbarian Only)");
		break;

	case SB_TAB_BAR_COMBAT:
		sResult = _T("+(x) to Combat Skills (Barbarian Only)");
		break;

	case SB_TAB_PAL_DEFENSIVEAURAS:
		sResult = _T("+(x) to Defensive Auras (Paladin Only)");
		break;

	case SB_TAB_PAL_OFFENSIVEAURAS:
		sResult = _T("+(x) to Offensive Auras (Paladin Only)");
		break;

	case SB_TAB_PAL_COMBAT:
		sResult = _T("+(x) to Combat Skills (Paladin Only)");
		break;

	case SB_TAB_SOR_COLD:
		sResult = _T("+(x) to Cold Spells (Sorceress Only)");
		break;

	case SB_TAB_SOR_LIGHTNING:
		sResult = _T("+(x) to Lightning Spells (Sorceress Only)");
		break;

	case SB_TAB_SOR_FIRE:
		sResult = _T("+(x) to Fire Spells (Sorceress Only)");
		break;

	case SB_TAB_DRU_ELEMENTAL:
		sResult = _T("+(x) to Elemental (Druid Only)");
		break;

	case SB_TAB_DRU_SHAPESHIFTING:
		sResult = _T("+(x) to Shape Shifting (Druid Only)");
		break;

	case SB_TAB_DRU_SUMMONING:
		sResult = _T("+(x) to Summoning (Druid Only)");
		break;

	default:
		break;
	}

	return !sResult.IsEmpty();
}

BOOL GetItemModDesc(LPCITEMMOD lpMod, CString& sResult)
{
	sResult.Empty();

	if (lpMod == NULL || lpMod->iCount == 0)
		return FALSE;

	CString sFmt;	
	if (lpMod->dwModID == 188)
	{
		// +skill tab
		if (!GetSkillTabName(lpMod->aValues[0], sFmt))
			return FALSE;

		sFmt.Replace(_T("(x)"), _T("%d"));
		sResult.Format(sFmt, lpMod->aValues[1]);
		return TRUE;
	}

	if (lpMod->dwModID == 83)
	{
		// +class Skill
		switch (lpMod->aValues[0])
		{
		case 0:
			sFmt = _T("+%d to Amazon Skill Levels");
			break;

		case 1:
			sFmt = _T("+ %d to Sorceress Skill Levels");
			break;

		case 2:
			sFmt = _T("+%d to Necromancer Skill Levels");
			break;

		case 3:
			sFmt = _T("+%d to Paladin Skill Levels");
			break;

		case 4:
			sFmt = _T("+%d to Barbarian Skill Levels");
			break;

		case 5:
			sFmt = _T("+%d to Druid Skill Levels");
			break;

		case 6:
			sFmt = _T("+%d to Assassin Skill Levels");
			break;

		default:
			return FALSE;
			break;
		}

		sResult.Format(sFmt, lpMod->aValues[1]);
		return TRUE;
	}

	if (lpMod->dwModID == 204)
	{
		// Skill Charges
		//if (!IsValidItemSkill((WORD)lpMod->aValues[1]))
		//	return FALSE;

		D2SPELLINFO dsi = {0};
		if (!IsValidItemSkill((WORD)lpMod->aValues[1]) || !D2GetSpellInfo((WORD)lpMod->aValues[1], &dsi))
			return FALSE;

		sResult.Format(_T("Level %d %s (%d/%d Charges)"),
			lpMod->aValues[0],
			dsi.szSpellName,
			lpMod->aValues[2],
			lpMod->aValues[3]);
		return TRUE;
	}

	if (lpMod->dwModID == 107)
	{
		// Single skill
		//if (!IsValidItemSkill((WORD)lpMod->aValues[0]))
		//	return FALSE;

		D2SPELLINFO dsi = {0};
		if (!IsValidItemSkill((WORD)lpMod->aValues[0]) || !D2GetSpellInfo((WORD)lpMod->aValues[0], &dsi))
			return FALSE;

		char szClass[64] = "";
		D2GetClassName(dsi.nClass, szClass, 63);
		sResult.Format(_T("+%d to %s (%s Only)"), lpMod->aValues[1], dsi.szSpellName, szClass);
		return TRUE;
	}

	if (lpMod->dwModID == 57)
	{
		// +y poison damage over x seconds
		sResult.Format(_T("+%d Poison Damage Over %d Seconds"), lpMod->aValues[1], lpMod->aValues[0]);
		return TRUE;
	}

	if (!FindModDesc(lpMod->dwModID, sFmt))
		return FALSE;	

	DWORD aVals[4];
	::memcpy(aVals, lpMod->aValues, sizeof(aVals));
	if (lpMod->dwModID == 48 || lpMod->dwModID == 50 || lpMod->dwModID == 54)
	{
		// elemental damage
		aVals[0] -= aVals[1];
	}

	if (lpMod->dwModID == 91)
	{
		// requirements -x%
		aVals[0] = 0 - aVals[0];
	}

	if (lpMod->dwModID == 112)
	{
		// hit causes monster to flee x%, 0x80 based
		aVals[0] = (DWORD)((double)lpMod->aValues[0] * 100.0 / 128.0);
	}

	const int VAR = GetModVars(sFmt);

	sFmt.Replace(_T("(x)"), _T("%d"));	
	sFmt.Replace(_T("(z)"), _T("%d"));

	if (lpMod->dwModID == 195 || lpMod->dwModID == 198 || lpMod->dwModID == 201)
	{
		// z% chance casting level x y on attacking/striking etc
		sFmt.Replace(_T("(y)"), _T("%s"));
		D2SPELLINFO dsi = {0};
		if (!IsValidItemSkill((WORD)lpMod->aValues[1]) || !D2GetSpellInfo((WORD)lpMod->aValues[1], &dsi))
			return FALSE;
		sResult.Format(sFmt, lpMod->aValues[2], lpMod->aValues[0], dsi.szSpellName);
		return TRUE;
	}
	
	sFmt.Replace(_T("(y)"), _T("%d"));
	switch (VAR)
	{
	case 0:
		sResult = sFmt;
		break;

	case 1:
		sResult.Format(sFmt, aVals[0]);
		break;

	case 2:
		sResult.Format(sFmt, aVals[0], aVals[1]);
		break;

	case 3: 
	case 4:
		sResult.Format(sFmt, aVals[0], aVals[1], aVals[2]);
		break;

	default:
		return FALSE;
		break;
	}
	
	return TRUE;
}
