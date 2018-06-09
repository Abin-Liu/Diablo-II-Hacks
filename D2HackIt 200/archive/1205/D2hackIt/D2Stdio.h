///////////////////////////////////////////////////////////////////////
// D2Stdio.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "CommonStructs.h"

void ServerErrorf(LPCSTR lpszFormat, ...);
void UnitError(LPCGAMEUNIT lpUnit, LPCSTR lpszDesc = NULL);
void PositionError(long x, long y, LPCSTR lpszDesc = NULL);
void PositionError(const LPPOINT lpPos, LPCSTR lpszDesc = NULL);
void SpellError(WORD wSpell);
void ItemError(DWORD dwItemID, LPCSTR lpszDesc = NULL);