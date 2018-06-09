///////////////////////////////////////////////////////////
// ItemCheck.h
//
// For misc item attribute checks.
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __ITEMCHECK_H__
#define __ITEMCHECK_H__

#include <windows.h>

enum { POTION_UNKNOWN = 0, POTION_RED, POTION_BLUE, POTION_PURPLE, POTION_JUNK };

int D2IsBelt(LPCSTR lpszItemCode); // if it's a belt, get the row count(2-4)
int D2IsPotion(LPCSTR lpszItemCode);
BOOL D2IsRune(LPCSTR lpszItemCode);
BOOL D2IsCirclets(LPCSTR lpszItemCode);
BOOL D2IsGloves(LPCSTR lpszItemCode);
BOOL D2IsBoots(LPCSTR lpszItemCode);
BOOL D2IsThrowItem(LPCSTR lpszItemCode);
BOOL D2IsBow(LPCSTR lpszItemCode);
BOOL D2IsCrossBow(LPCSTR lpszItemCode);
BOOL D2IsQuestItem(LPCSTR lpszItemCode);
DWORD D2ParseItemIDFromPacket(const BYTE* aPacket, DWORD aLen);
BOOL D2ParseItemCodeFromPacket(const BYTE* aPacket, DWORD aLen, LPSTR lpszBuffer);

#endif // __ITEMCHECK_H__