#include "PlayerDef.h"
#include <tchar.h>
#include <string.h>
#include <stdio.h>

BOOL operator==(const PLAYEROBJ& lhs, const PLAYEROBJ& rhs)
{
	return lhs.dwID == rhs.dwID;
}

BOOL operator>(const PLAYEROBJ& lhs, const PLAYEROBJ& rhs)
{
	return lhs.dwID > rhs.dwID;
}

int D2GetBeltItemCategory(DWORD dwItemType)
{
	int nCata = BELT_CATA_UNKNOWN;
	switch (dwItemType)
	{
	case POTION_STAMINA:
	case POTION_ANTIDOTE:
	case POTION_THAWING:
		nCata = BELT_CATA_JUNKPOTION;
		break;

	case POTION_MINOR_HEALING:
	case POTION_LIGHT_HEALING:
	case POTION_HEALING:
	case POTION_GREATER_HEALING:
	case POTION_SUPER_HEALING:
		nCata = BELT_CATA_HEALING;
		break;

	case POTION_MINOR_MANA:
	case POTION_LIGHT_MANA:
	case POTION_MANA:
	case POTION_GREATER_MANA:
	case POTION_SUPER_MANA:
		nCata = BELT_CATA_MANA;
		break;
		
	case POTION_REJUVENATE:
	case POTION_FULL_REJUVENATE:
		nCata = BELT_CATA_REJUV;
		break;	

	default:
		nCata = BELT_CATA_UNKNOWN;
		break;
	}

	return nCata;
}

DWORD D2GetBeltItemName(DWORD dwItemType, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return FALSE;

	lpszBuffer[0] = _T('\0');
	::memset(lpszBuffer, 0, sizeof(TCHAR) * dwMaxChars);
	if (dwMaxChars == 0)
		return 0;

	switch (dwItemType)
	{
		// Misc Potions
	case POTION_STAMINA:
		_tcsncpy(lpszBuffer, _T("Stamina Potion"), dwMaxChars);
		break;

	case POTION_ANTIDOTE:
		_tcsncpy(lpszBuffer, _T("Antidote Potion"), dwMaxChars);
		break;

	case POTION_THAWING:
		_tcsncpy(lpszBuffer, _T("Thawing Potion"), dwMaxChars);
		break;

	// Healing Potions
	case POTION_MINOR_HEALING:
		_tcsncpy(lpszBuffer, _T("Minor Healing Potion"), dwMaxChars);
		break;

	case POTION_LIGHT_HEALING:
		_tcsncpy(lpszBuffer, _T("Light Healing Potion"), dwMaxChars);
		break;

	case POTION_HEALING:
		_tcsncpy(lpszBuffer, _T("Healing Potion"), dwMaxChars);
		break;

	case POTION_GREATER_HEALING:
		_tcsncpy(lpszBuffer, _T("Greater Healing Potion"), dwMaxChars);
		break;

	case POTION_SUPER_HEALING:
		_tcsncpy(lpszBuffer, _T("Super Healing Potion"), dwMaxChars);
		break;

	// Mana Potions
	case POTION_MINOR_MANA:
		_tcsncpy(lpszBuffer, _T("Minor Mana Potion"), dwMaxChars);
		break;

	case POTION_LIGHT_MANA:
		_tcsncpy(lpszBuffer, _T("Light Mana Potion"), dwMaxChars);
		break;

	case POTION_MANA:
		_tcsncpy(lpszBuffer, _T("Mana Potion"), dwMaxChars);
		break;

	case POTION_GREATER_MANA:
		_tcsncpy(lpszBuffer, _T("Greater Mana Potion"), dwMaxChars);
		break;

	case POTION_SUPER_MANA:
		_tcsncpy(lpszBuffer, _T("Super Mana Potion"), dwMaxChars);
		break;

	// Rejuvenate Potions
	case POTION_REJUVENATE:
		_tcsncpy(lpszBuffer, _T("Rejuvenate Potion"), dwMaxChars);
		break;

	case POTION_FULL_REJUVENATE:
		_tcsncpy(lpszBuffer, _T("Full Rejuvenate Potion"), dwMaxChars);
		break;

	// Scrolls
	case SCROLL_IDENTIFY:
		_tcsncpy(lpszBuffer, _T("Scroll of Identify"), dwMaxChars);
		break;

	case SCROLL_TOWN_PORTAL:
		_tcsncpy(lpszBuffer, _T("Scroll of Town Portal"), dwMaxChars);
		break;

	default:
		return 0;
		break;
	}

	return _tcslen(lpszBuffer);
}

DWORD D2FormatPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (aPacket == NULL || aLen == 0)
		return 0;

	TCHAR sz[32] = _T("");
	if (bReceived)
		_stprintf(sz, _T("R %d:"));
	else
		_stprintf(sz, _T("R %d:"));

	DWORD dwCopied = _tcslen(sz);
	if (dwCopied > dwMaxChars)
		return 0;

	_tcscat(lpszBuffer, sz);

	for (DWORD i = 0; i < aLen && dwCopied + 3 <= dwMaxChars; i++)
	{
		_stprintf(sz, _T(" %02X"), aPacket[i]);
		_tcscat(lpszBuffer, sz);
		dwCopied += 3;
	}

	return dwCopied;
}