//#include "stdafx.h"
#include "PBotConfig.h"

// Load data from xxx.bot file
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

// Write data to xxx.bot file
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

// Ensure the first bonus is not greater than the second, if it is not
// the case, swap them
void _CheckSwap(WORD& w1, WORD& w2)
{
	if (w1 > w2)
	{
		WORD w = w1;
		w1 = w2;
		w2 = w;
	}
}

// Compare two combinations
DWORD ComboCompare(DWORD dwPindle, DWORD dwCfg)
{
	if (dwPindle == 0 || dwCfg == 0)
		return 0;

	WORD wPindle1 = LOWORD(dwPindle);
	WORD wPindle2 = HIWORD(dwPindle);
	WORD wCfg1 = LOWORD(dwCfg);
	WORD wCfg2 = HIWORD(dwCfg);

	_CheckSwap(wPindle1, wPindle2);
	_CheckSwap(wCfg1, wCfg2);
	
	dwPindle = MAKELONG(wPindle1, wPindle2);
	
	if (wPindle1 == 0)
	{
		// Pindleskin only has one bonus 
		if (wCfg1 == 0 && wPindle2 == wCfg2)
			return dwPindle;
	}
	else
	{
		// Pindleskin has two bonuses
		if (wCfg1 == 0) 
			wCfg1 = 0xffff; // the "Any" flag
		if (wCfg2 == 0)
			wCfg2 = 0xffff; // the "Any" flag

		DWORD dw = MAKELONG(wCfg1, wCfg2);
		if ((dwPindle & dw) == dwPindle) // bit-comparison
			return dwPindle;
	}

	return 0;
}

// Find combos
DWORD PBotFindCombo(const WORD* aBonuses, LPCPBOTCONFIG lpConfig)
{
	if (aBonuses == NULL || lpConfig == NULL)
		return 0;

	// check all possible combos in the config data
	for (int i = 0; i < lpConfig->iComboCount && lpConfig->aAvoidCombos[i] != 0; i++)
	{
		DWORD dwResult = ComboCompare(*(DWORD*)aBonuses, lpConfig->aAvoidCombos[i]);
		if (dwResult)
			return dwResult;
	}

	return 0;
}
