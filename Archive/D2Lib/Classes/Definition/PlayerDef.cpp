#include "PlayerDef.h"
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include "AffectionDef.h"

BOOL operator==(const PLAYEROBJ& lhs, const PLAYEROBJ& rhs)
{
	return lhs.dwID == rhs.dwID;
}

BOOL operator>(const PLAYEROBJ& lhs, const PLAYEROBJ& rhs)
{
	return lhs.dwID > rhs.dwID;
}

int D2GetBeltItemCategory(LPCTSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return BELT_CATA_UNKNOWN;

	if (_tcsicmp(lpszItemCode, _T("vps")) == 0
		|| _tcsicmp(lpszItemCode, _T("yps")) == 0
		|| _tcsicmp(lpszItemCode, _T("wms")) == 0)
		return BELT_CATA_JUNKPOTION;

	if (_tcsicmp(lpszItemCode, _T("rvs")) == 0
		|| _tcsicmp(lpszItemCode, _T("rvl")) == 0)
		return BELT_CATA_REJUV;

	if (_tcsicmp(lpszItemCode, _T("hp1")) == 0
		|| _tcsicmp(lpszItemCode, _T("hp2")) == 0
		|| _tcsicmp(lpszItemCode, _T("hp3")) == 0
		|| _tcsicmp(lpszItemCode, _T("hp4")) == 0
		|| _tcsicmp(lpszItemCode, _T("hp5")) == 0)
		return BELT_CATA_HEALING;

	if (_tcsicmp(lpszItemCode, _T("mp1")) == 0
		|| _tcsicmp(lpszItemCode, _T("mp2")) == 0
		|| _tcsicmp(lpszItemCode, _T("mp3")) == 0
		|| _tcsicmp(lpszItemCode, _T("mp4")) == 0
		|| _tcsicmp(lpszItemCode, _T("mp5")) == 0)
		return BELT_CATA_MANA;

	return BELT_CATA_UNKNOWN;
}

DWORD D2GetBeltItemName(LPCTSTR lpszItemCode, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	::memset(lpszBuffer, 0, sizeof(TCHAR) * dwMaxChars);
	if (dwMaxChars == 0)
		return 0;

	if (_tcsicmp(lpszItemCode, _T("vps")) == 0)
		_tcsncpy(lpszBuffer, _T("Stamina Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("yps")) == 0)
		_tcsncpy(lpszBuffer, _T("Antidote Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("wms")) == 0)
		_tcsncpy(lpszBuffer, _T("Thawing Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("rvs")) == 0)
		_tcsncpy(lpszBuffer, _T("Rejuvenate Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("rvl")) == 0)
		_tcsncpy(lpszBuffer, _T("Full Rejuvenate Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("hp1")) == 0)
		_tcsncpy(lpszBuffer, _T("Minor Healing Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("hp2")) == 0)
		_tcsncpy(lpszBuffer, _T("Light Healing Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("hp3")) == 0)
		_tcsncpy(lpszBuffer, _T("Healing Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("hp4")) == 0)
		_tcsncpy(lpszBuffer, _T("Greater Healing Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("hp5")) == 0)
		_tcsncpy(lpszBuffer, _T("Super Healing Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("mp1")) == 0)
		_tcsncpy(lpszBuffer, _T("Minor Mana Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("mp2")) == 0)
		_tcsncpy(lpszBuffer, _T("Light Mana Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("mp3")) == 0)
		_tcsncpy(lpszBuffer, _T("Mana Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("mp4")) == 0)
		_tcsncpy(lpszBuffer, _T("Greater Mana Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("mp5")) == 0)
		_tcsncpy(lpszBuffer, _T("Super Mana Potion"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("isc")) == 0)
		_tcsncpy(lpszBuffer, _T("Scroll of Identify"), dwMaxChars);
	else if (_tcsicmp(lpszItemCode, _T("tsc")) == 0)
		_tcsncpy(lpszBuffer, _T("Scroll of Town Portal"), dwMaxChars);

	return _tcslen(lpszBuffer);
}

DWORD D2GetBeltItemName(DWORD dwCodeID, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	TCHAR szItemCode[4] = _T("");
	::memcpy(szItemCode, &dwCodeID, 3);
	return D2GetBeltItemName(szItemCode, lpszBuffer, dwMaxChars);
}

/*
BYTE D2CalcWolf(BYTE iFlag)
{
	if (iFlag < 0x16)
		return 0;

	if (iFlag == 0x16)
		return 1;

	BYTE iS = 0x16;
	for (BYTE i = 2; i < 100; i++)
	{
		if (i % 2)
			iS += 2;
		else
			iS += 3;

		if (iFlag == iS)
			return i;
	}

	return 0;
}

BYTE D2CalcBear(BYTE iFlag)
{
	if (iFlag < 0x11)
		return 0;

	if (iFlag == 0x11)
		return 1;

	BYTE iS = 0x11;
	for (BYTE i = 2; i < 100; i++)
	{
		if ((i - 3) % 4)
			iS += 1;
		else
			iS += 2;

		if (iFlag == iS)
			return i;
	}

	return 0;
}

BOOL D2GetPuffFromPacket(LPPUFF lpPuff, const BYTE* aPacket, DWORD aLen)
{
	if (lpPuff == NULL || aPacket == NULL || aPacket[0] != 0xa8 || aLen < 8)
		return FALSE;

	::memset(lpPuff, 0, sizeof(PUFF));
	
	// Calculate puff level
	BYTE iLevel = 0;
	WORD wVal;
	switch (aPacket[7])
	{
	case STATE_BATTLEORDERS: //bo
		lpPuff->iPuffID = STATE_BATTLEORDERS;
		::memcpy(&wVal, aPacket + 9, 2);
		iLevel = (wVal - 0x004c) / 0x06 + 2;

		// start at 35%, +3% each level
		lpPuff->wHPPuffPercent = (iLevel - 1) * 3 + 35;
		lpPuff->wManaPuffPercent = lpPuff->wHPPuffPercent;
		break;

	case STATE_OAKSAGE: // oak
		lpPuff->iPuffID = STATE_OAKSAGE;
		::memcpy(&wVal, aPacket + 9, 2);
		iLevel = (wVal - 0x003c) / 0x0a + 1;

		// start at 30%, +5% each level
		lpPuff->wHPPuffPercent = (iLevel - 1) * 5 + 30;
		break;

	case STATE_WOLF:
		lpPuff->iPuffID = STATE_WOLF;
		iLevel = D2CalcWolf(aPacket[14]);

		// werewolf 25%, lycanthropy start at 20%, +5% each level
		// iPuffLevel is Lycanthropy level
		lpPuff->wHPPuffPercent = (iLevel - 1) * 5 + 20 + 25;
		break;

	case STATE_BEAR:
		lpPuff->iPuffID = STATE_BEAR;
		iLevel = D2CalcBear(aPacket[12]);

		// werebear 50%, lycanthropy start at 20%, +5% each level
		// iPuffLevel is Lycanthropy level
		lpPuff->wHPPuffPercent = (iLevel - 1) * 5 + 20 + 50;
		break;

	default:
		return 0;
	}

	return 1;
}*/