//////////////////////////////////////////////////////////////////
// ServerDraw.h
//----------------------------------------------------------------
// Internal drawing functions used by D2Hackit itself.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////

#ifndef __SERVERDRAW_H__
#define __SERVERDRAW_H__

#include "CommonStructs.h"
#include <windows.h>

extern BOOL g_bDrawBanner; // draw banner text
extern BOOL g_bDrawMyStats; // draw my stats

void ServerDraw(CGameDC* pDC, LPCRECT lpScreenRect);
void ServerDrawOnGameJoin();
void ServerDrawOnGameLeave();

#endif // __SERVERDRAW_H__