///////////////////////////////////////////////////////////
// D2Stdio.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "D2Hackit.h"

#define D2ERROR_HEADER	"ÿc1::ÿc4D2Hackit Errorÿc1::ÿc0 "
#define D2ERROR_HEADER_LEN	31 // str len of D2ERROR_HEADER

BOOL EXPORT GameErrorf(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return FALSE;

	char szString[512] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString, 511, lpszFormat, args);
	va_end(args);
	return GamePrintError(szString);
}

BOOL EXPORT GameInfof(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return FALSE;

	TCHAR szString[512] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString, 511, lpszFormat, args);
	va_end(args);
	return GamePrintInfo(szString);
}

BOOL EXPORT GameStringf(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return FALSE;

	TCHAR szString[512] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString, 511, lpszFormat, args);
	va_end(args);
	return GamePrintString(szString);
}

BOOL EXPORT GameCommandf(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return FALSE;

	TCHAR szString[512] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString, 511, lpszFormat, args);
	va_end(args);
	return GameCommandLine(szString);
}

void ServerErrorf(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return;

	char szString[D2ERROR_HEADER_LEN + 512] = D2ERROR_HEADER;
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString + D2ERROR_HEADER_LEN, 511, lpszFormat, args);
	va_end(args);
	GamePrintString(szString);
}

void UnitError(LPCGAMEUNIT lpUnit, LPCSTR lpszDesc)
{
	char szDesc[64] = "";
	if (lpszDesc && *lpszDesc)
		::_snprintf(szDesc, 63, "(%s)", lpszDesc);

	if (lpUnit)
		ServerErrorf("Invalid unit%s: ID=0x%08X, type=%d.", szDesc, lpUnit->dwUnitID, lpUnit->dwUnitType);
	else
		ServerErrorf("Null unit%s.", szDesc);
}

void PositionError(long x, long y, LPCSTR lpszDesc)
{
	char szDesc[64] = "";
	if (lpszDesc && *lpszDesc)
		::_snprintf(szDesc, 63, "(%s)", lpszDesc);

	ServerErrorf("Invalid position%s: 0x%04X, 0x%04X.", szDesc, x, y);
}

void PositionError(const LPPOINT lpPos, LPCSTR lpszDesc)
{
	char szDesc[64] = "";
	if (lpszDesc && *lpszDesc)
		::_snprintf(szDesc, 63, "(%s)", lpszDesc);

	if (lpPos)
		ServerErrorf("Invalid position%s: 0x%04X, 0x%04X.", szDesc, lpPos->x, lpPos->y);
	else
		ServerErrorf("Null position%s.", szDesc);
}

void SpellError(WORD wSpell)
{
	ServerErrorf("Player does not have spell 0x%04X", wSpell);
}

void ItemError(DWORD dwItemID, LPCSTR lpszDesc)
{
	char szDesc[64] = "";
	if (lpszDesc && *lpszDesc)
		::_snprintf(szDesc, 63, "(%s)", lpszDesc);

	ServerErrorf("Invalid Item ID%s: 0x%08X.", szDesc, dwItemID);
}