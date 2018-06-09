#include "AuraDef.h"
#include <tchar.h>
#include <string.h>

DWORD D2GetAuraName(BYTE iAuraID, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (dwMaxChars == 0)
		return 0;

	switch (iAuraID)
	{
	case AURA_HOLYFREEZE:
		_tcsncpy(lpszBuffer, _T("Holy Freeze"), dwMaxChars);
		break;

	case AURA_BLESSEDAIM:
		_tcsncpy(lpszBuffer, _T("Blessed Aim"), dwMaxChars);
		break;

	case AURA_MIGHT:
		_tcsncpy(lpszBuffer, _T("Might"), dwMaxChars);
		break;

	case AURA_HOLYFIRE:
		_tcsncpy(lpszBuffer, _T("Holy Fire"), dwMaxChars);
		break;

	case AURA_HOLYSHOCK:
		_tcsncpy(lpszBuffer, _T("Holy Shock"), dwMaxChars);
		break;

	case AURA_CONVICTION:
		_tcsncpy(lpszBuffer, _T("Conviction"), dwMaxChars);
		break;

	case AURA_FANATICISM:
		_tcsncpy(lpszBuffer, _T("Fanaticism"), dwMaxChars);
		break;

	case AURA_SALVATION:
		_tcsncpy(lpszBuffer, _T("Salvation"), dwMaxChars);
		break;

	case AURA_REDEMPTION:
		_tcsncpy(lpszBuffer, _T("Redemption"), dwMaxChars);
		break;

	case AURA_MEDITATION:
		_tcsncpy(lpszBuffer, _T("Meditation"), dwMaxChars);
		break;

	case AURA_VIGOR:
		_tcsncpy(lpszBuffer, _T("Vigor"), dwMaxChars);
		break;

	case AURA_RESISTLIGHTNING:
		_tcsncpy(lpszBuffer, _T("Resist Lightning"), dwMaxChars);
		break;

	case AURA_CLEANSING:
		_tcsncpy(lpszBuffer, _T("Cleansing"), dwMaxChars);
		break;

	case AURA_RESISTCOLD:
		_tcsncpy(lpszBuffer, _T("Resist Cold"), dwMaxChars);
		break;

	case AURA_DEFIANCE:
		_tcsncpy(lpszBuffer, _T("Defiance"), dwMaxChars);
		break;

	case AURA_RESISTFIRE:
		_tcsncpy(lpszBuffer, _T("Resist Fire"), dwMaxChars);
		break;

	case AURA_PRAYER:
		_tcsncpy(lpszBuffer, _T("Prayer"), dwMaxChars);
		break;

	case AURA_SANCTUARY:
		_tcsncpy(lpszBuffer, _T("Sanctuary"), dwMaxChars);
		break;

	case AURA_CONCENTRATION:
		_tcsncpy(lpszBuffer, _T("Concentration"), dwMaxChars);
		break;

	case AURA_THORNS:
		_tcsncpy(lpszBuffer, _T("Thorns"), dwMaxChars);
		break;

	default:
		break;
	}
	
	return _tcslen(lpszBuffer);
}