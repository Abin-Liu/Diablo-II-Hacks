#include "stdafx.h"
#include "PBotConfig.h"

BOOL PBotLoadConfig(LPCTSTR lpszFile, LPPBOTCONFIG lpConfig)
{
	if (lpConfig == NULL)
		return FALSE;

	::memset(lpConfig, 0, sizeof(PBOTCONFIG));
	if (lpszFile == NULL || lpszFile[0] == _T('\0'))
		return FALSE;

	HANDLE hFile = ::CreateFile(lpszFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == NULL)
		return FALSE;

	BOOL bOK = FALSE;
	DWORD dwRead = 0;
	if (::ReadFile(hFile, lpConfig, sizeof(PBOTCONFIG), &dwRead, NULL))
		bOK = dwRead == sizeof(PBOTCONFIG);

	::CloseHandle(hFile);
	return bOK;
}

BOOL PBotSaveConfig(LPCTSTR lpszFile, LPCPBOTCONFIG lpConfig)
{
	if (lpConfig == NULL || lpszFile == NULL || lpszFile[0] == _T('\0'))
		return FALSE;

	HANDLE hFile = ::CreateFile(lpszFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == NULL)
		return FALSE;

	BOOL bOK = FALSE;
	DWORD dwWritten = 0;
	if (::WriteFile(hFile, lpConfig, sizeof(PBOTCONFIG), &dwWritten, NULL))
		bOK = dwWritten == sizeof(PBOTCONFIG);

	::CloseHandle(hFile);
	return bOK;
}

DWORD PBotFindCombo(const WORD* aBonuses, LPCPBOTCONFIG lpConfig)
{
	if (aBonuses == NULL || lpConfig == NULL)
		return 0;
	
	for (int i = 0; i < lpConfig->iComboCount && lpConfig->aAvoidCombos[i] != 0; i++)
	{
		WORD wFound1 = 0;
		WORD wFound2 = 0;
		WORD wBonus1 = LOWORD(lpConfig->aAvoidCombos[i]);
		WORD wBonus2 = HIWORD(lpConfig->aAvoidCombos[i]);
		if (wBonus1 == wBonus2)
			wBonus2 = 0;

		for (int j = 0; j < PINDLE_MAX_BONUS + 1; j++)
		{
			if (aBonuses[j] == 0 || aBonuses[j] == PINDLE_BONUS_FIRE_ENCHANTED || aBonuses[j] == PINDLE_BONUS_AURA_ENCHANTED)
				continue;
			
			if (wBonus1 == 0 || aBonuses[j] == wBonus1)
				wFound1 = aBonuses[j];
			else if (wBonus2 == 0 || aBonuses[j] == wBonus2)
				wFound2 = aBonuses[j];
		}

		if (wFound1 != 0 && wFound2 != 0)
			return MAKELONG(wFound1, wFound2);
	}

	return 0;
}
