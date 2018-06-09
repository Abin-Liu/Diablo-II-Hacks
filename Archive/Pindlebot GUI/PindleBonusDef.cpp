#include "stdafx.h"
#include "PindleBonusDef.h"
#include <tchar.h>
#include <string.h>
#include <stdio.h>

DWORD PBotGetBonusName(WORD wBonus, LPTSTR lpszBuffer, DWORD dwMaxChars, BOOL bChinese)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars * sizeof(TCHAR));

	switch (wBonus)
	{
	case PINDLE_BONUS_NONE:
		_tcsncpy(lpszBuffer, bChinese ? _T("任何-Any") : _T("Any"), dwMaxChars);
		break;

	case PINDLE_BONUS_AURA_ENCHANTED:
		_tcsncpy(lpszBuffer, bChinese ? _T("光环增强-Aura Enchanted") : _T("Aura Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_COLD_ENCHANTED:
		_tcsncpy(lpszBuffer, bChinese ? _T("冰冻增强-Cold Enchanted") : _T("Cold Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_CURSED:
		_tcsncpy(lpszBuffer, bChinese ? _T("诅咒-Cursed") : _T("Cursed"), dwMaxChars);
		break;

	case PINDLE_BONUS_EXTRA_FAST:
		_tcsncpy(lpszBuffer, bChinese ? _T("超级快速-Extra Fast") : _T("Extra Fast"), dwMaxChars);
		break;

	case PINDLE_BONUS_EXTRA_STRONG:
		_tcsncpy(lpszBuffer, bChinese ? _T("超级强壮-Extra Strong") : _T("Extra Strong"), dwMaxChars);
		break;

	case PINDLE_BONUS_LIGHTNING_ENCHANTED:
		_tcsncpy(lpszBuffer, bChinese ? _T("闪电增强-Lightning Enchanted") : _T("Lightning Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_FIRE_ENCHANTED:
		_tcsncpy(lpszBuffer, bChinese ? _T("火焰增强-Fire Enchanted") : _T("Fire Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_MAGIC_RESISTANT:
		_tcsncpy(lpszBuffer, bChinese ? _T("魔法抵抗-Magic Resistant") : _T("Magic Resistant"), dwMaxChars);
		break;

	case PINDLE_BONUS_MANA_BURN:
		_tcsncpy(lpszBuffer, bChinese ? _T("法力燃烧-Mana Burn") : _T("Mana Burn"), dwMaxChars);
		break;

	case PINDLE_BONUS_MULTISHOT:
		_tcsncpy(lpszBuffer, bChinese ? _T("多重射击-Multishot") : _T("Multishot"), dwMaxChars);
		break;

	case PINDLE_BONUS_SPECTRAL_HIT:
		_tcsncpy(lpszBuffer, bChinese ? _T("怪异攻击-Spectral Hit") : _T("Spectral Hit"), dwMaxChars);
		break;

	case PINDLE_BONUS_STONE_SKIN:
		_tcsncpy(lpszBuffer, bChinese ? _T("石头皮肤-Stone Skin") : _T("Stone Skin"), dwMaxChars);
		break;

	case PINDLE_BONUS_TELEPORT:
		_tcsncpy(lpszBuffer, bChinese ? _T("心灵传送-Teleport") : _T("Teleport"), dwMaxChars);
		break;

	case PINDLE_AURA_HOLYFREEZE:
		_tcsncpy(lpszBuffer, bChinese ? _T("寒冰光环-Holy Freeze Aura") : _T("Holy Freeze Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_BLESSEDAIM:
		_tcsncpy(lpszBuffer, bChinese ? _T("祝福光环-Blessed Aim Aura") : _T("Blessed Aim Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_MIGHT:
		_tcsncpy(lpszBuffer, bChinese ? _T("力量光环-Might Aura") : _T("Might Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_HOLYFIRE:
		_tcsncpy(lpszBuffer, bChinese ? _T("火焰光环-Holy Fire Aura") : _T("Holy Fire Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_HOLYSHOCK:
		_tcsncpy(lpszBuffer, bChinese ? _T("闪电光环-Holy Shock Aura") : _T("Holy Shock Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_CONVICTION:
		_tcsncpy(lpszBuffer, bChinese ? _T("审判光环-Conviction Aura") : _T("Conviction Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_FANATICISM:
		_tcsncpy(lpszBuffer, bChinese ? _T("狂热光环-Fanaticism Aura") : _T("Fanaticism Aura"), dwMaxChars);
		break;

	default:
		return 0;
		break;
	}

	return _tcslen(lpszBuffer);
}