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
		_tcsncpy(lpszBuffer, bChinese ? _T("�κ�-Any") : _T("Any"), dwMaxChars);
		break;

	case PINDLE_BONUS_AURA_ENCHANTED:
		_tcsncpy(lpszBuffer, bChinese ? _T("�⻷��ǿ-Aura Enchanted") : _T("Aura Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_COLD_ENCHANTED:
		_tcsncpy(lpszBuffer, bChinese ? _T("������ǿ-Cold Enchanted") : _T("Cold Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_CURSED:
		_tcsncpy(lpszBuffer, bChinese ? _T("����-Cursed") : _T("Cursed"), dwMaxChars);
		break;

	case PINDLE_BONUS_EXTRA_FAST:
		_tcsncpy(lpszBuffer, bChinese ? _T("��������-Extra Fast") : _T("Extra Fast"), dwMaxChars);
		break;

	case PINDLE_BONUS_EXTRA_STRONG:
		_tcsncpy(lpszBuffer, bChinese ? _T("����ǿ׳-Extra Strong") : _T("Extra Strong"), dwMaxChars);
		break;

	case PINDLE_BONUS_LIGHTNING_ENCHANTED:
		_tcsncpy(lpszBuffer, bChinese ? _T("������ǿ-Lightning Enchanted") : _T("Lightning Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_FIRE_ENCHANTED:
		_tcsncpy(lpszBuffer, bChinese ? _T("������ǿ-Fire Enchanted") : _T("Fire Enchanted"), dwMaxChars);
		break;

	case PINDLE_BONUS_MAGIC_RESISTANT:
		_tcsncpy(lpszBuffer, bChinese ? _T("ħ���ֿ�-Magic Resistant") : _T("Magic Resistant"), dwMaxChars);
		break;

	case PINDLE_BONUS_MANA_BURN:
		_tcsncpy(lpszBuffer, bChinese ? _T("����ȼ��-Mana Burn") : _T("Mana Burn"), dwMaxChars);
		break;

	case PINDLE_BONUS_MULTISHOT:
		_tcsncpy(lpszBuffer, bChinese ? _T("�������-Multishot") : _T("Multishot"), dwMaxChars);
		break;

	case PINDLE_BONUS_SPECTRAL_HIT:
		_tcsncpy(lpszBuffer, bChinese ? _T("���칥��-Spectral Hit") : _T("Spectral Hit"), dwMaxChars);
		break;

	case PINDLE_BONUS_STONE_SKIN:
		_tcsncpy(lpszBuffer, bChinese ? _T("ʯͷƤ��-Stone Skin") : _T("Stone Skin"), dwMaxChars);
		break;

	case PINDLE_BONUS_TELEPORT:
		_tcsncpy(lpszBuffer, bChinese ? _T("���鴫��-Teleport") : _T("Teleport"), dwMaxChars);
		break;

	case PINDLE_AURA_HOLYFREEZE:
		_tcsncpy(lpszBuffer, bChinese ? _T("�����⻷-Holy Freeze Aura") : _T("Holy Freeze Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_BLESSEDAIM:
		_tcsncpy(lpszBuffer, bChinese ? _T("ף���⻷-Blessed Aim Aura") : _T("Blessed Aim Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_MIGHT:
		_tcsncpy(lpszBuffer, bChinese ? _T("�����⻷-Might Aura") : _T("Might Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_HOLYFIRE:
		_tcsncpy(lpszBuffer, bChinese ? _T("����⻷-Holy Fire Aura") : _T("Holy Fire Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_HOLYSHOCK:
		_tcsncpy(lpszBuffer, bChinese ? _T("����⻷-Holy Shock Aura") : _T("Holy Shock Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_CONVICTION:
		_tcsncpy(lpszBuffer, bChinese ? _T("���й⻷-Conviction Aura") : _T("Conviction Aura"), dwMaxChars);
		break;

	case PINDLE_AURA_FANATICISM:
		_tcsncpy(lpszBuffer, bChinese ? _T("���ȹ⻷-Fanaticism Aura") : _T("Fanaticism Aura"), dwMaxChars);
		break;

	default:
		return 0;
		break;
	}

	return _tcslen(lpszBuffer);
}