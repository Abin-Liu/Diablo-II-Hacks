//////////////////////////////////////////////////////////////////////
// GameOnDraw.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "D2Hackit.h"
#include <stdio.h>
#include "Server20.h"
#include "d2functions.h"
#include "D2HackItServer.h"
#include "ServerDraw.h"
#include "PathAlgorithms\\Common.h"

struct D2DRAWDATA
{
	GAMEDC dc;
	BYTE iTextColor;
	BYTE iFont;
	BYTE iPenColor;
	int nPenWidth;
	BYTE iBrushColor;
	BYTE iTransparency;
	int nBkMode;
	POINT pos;
	RECT rcScreen;
};

BOOL g_bIsDrawing = FALSE;
D2DRAWDATA g_screenDrawData = {0};

#define BIND_TO_GAMEDC(x) g_screenDrawData.dc.##x## = ##D2##x

int RemoveColorSpecs(LPWSTR lpwsz);
int ReplaceColorSpecs(LPWSTR lpwsz);
int ValidateD2Font(int nFont); // Whether a font is valid
void InitializeGameDC();
BOOL CheckDummyError(LPCSTR lpszError);
SIZE GetTextSize(LPCSTR lpszText, int nFont);
SIZE GetTextSize(LPCWSTR lpwszText, int nFont);
void D2WTextOut(LPCWSTR lpwsz, LPCRECT lpRect, BYTE iFont);

// Game DC methods
BOOL D2TextOut(LPCSTR lpszText, long x, long y);
BOOL D2DrawText(LPCSTR lpszText, LPCRECT lpRect, UINT nAlign = DT_LEFT);
POINT D2MoveTo(long x, long y);
BOOL D2LineTo(long x, long y);
BOOL D2FillRect(LPCRECT lpRect);
BOOL D2Rectangle(LPCRECT lpRect);
BOOL D2FrameRect(LPCRECT lpRect);
BYTE D2SetFont(BYTE iFont);
BYTE D2GetFont();
SIZE D2GetTextExtent(LPCSTR lpszText);
BYTE D2SetTextColor(BYTE iColor);
BYTE D2GetTextColor();
BYTE D2SetPenColor(BYTE iColor);
BYTE D2GetPenColor();
int D2SetPenWidth(int nWidth);
int D2GetPenWidth();
BYTE D2SetBrushColor(BYTE iColor);
BYTE D2GetBrushColor();
BYTE D2SetBrushTransparency(BYTE iTransparency);
BYTE D2GetBrushTransparency();
int D2SetBkMode(int nBkMode);
int D2GetBkMode();

void InitializeGameDC()
{
	// bind drawing functions
	BIND_TO_GAMEDC(TextOut);
	BIND_TO_GAMEDC(DrawText);
	BIND_TO_GAMEDC(MoveTo);
	BIND_TO_GAMEDC(LineTo);
	BIND_TO_GAMEDC(FillRect);
	BIND_TO_GAMEDC(Rectangle);
	BIND_TO_GAMEDC(FrameRect);
	BIND_TO_GAMEDC(SetFont);
	BIND_TO_GAMEDC(GetFont);
	BIND_TO_GAMEDC(SetTextColor);
	BIND_TO_GAMEDC(GetTextColor);
	BIND_TO_GAMEDC(SetBkMode);
	BIND_TO_GAMEDC(GetBkMode);
	BIND_TO_GAMEDC(GetTextExtent);
	BIND_TO_GAMEDC(SetPenColor);
	BIND_TO_GAMEDC(GetPenColor);
	BIND_TO_GAMEDC(SetPenWidth);
	BIND_TO_GAMEDC(GetPenWidth);
	BIND_TO_GAMEDC(SetBrushColor);
	BIND_TO_GAMEDC(GetBrushColor);
	BIND_TO_GAMEDC(SetBrushTransparency);
	BIND_TO_GAMEDC(GetBrushTransparency);

	// initialize data to default values
	g_screenDrawData.iTextColor = 0;
	g_screenDrawData.iFont = D2FONT_SMOOTH;
	g_screenDrawData.iPenColor = 0x20;
	g_screenDrawData.nPenWidth = 1;
	g_screenDrawData.iBrushColor = 0;
	g_screenDrawData.iTransparency = 1;
	g_screenDrawData.nBkMode = TRANSPARENT;
	::memset(&g_screenDrawData.pos, 0, sizeof(POINT));

	// retrieve screen rect
	SIZE cs = GetScreenSize();
	g_screenDrawData.rcScreen.left = 0;
	g_screenDrawData.rcScreen.top = 0;
	g_screenDrawData.rcScreen.right = cs.cx;
	g_screenDrawData.rcScreen.bottom = cs.cy;
}

void __fastcall GameOnDrawIntercept()
{
	if (!IsGameJoined())
		return;		
	
	D2CLIENT_DrawClientScreen();

	g_bIsDrawing = TRUE;
	
	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnDraw)
		{
			// We reinitialize the game dc for every module, in case some modules
			// wanna be funny and tries to mess the dc up, for example, changes the
			// font or pen color without changing them back.
			InitializeGameDC();
			pCIS->OnDraw(&g_screenDrawData.dc, &g_screenDrawData.rcScreen);
		}
		//else
		//	GamePrintError("OnDraw error!");

		li = ClientModules.GetNextItem(li);
	}

	InitializeGameDC();
	ServerDraw(&g_screenDrawData.dc, &g_screenDrawData.rcScreen);
	g_bIsDrawing = FALSE;
}

void D2WTextOut(LPWSTR lpwsz, LPCRECT lpRect, BYTE iFont)
{
	if (lpwsz == NULL || lpRect == NULL)
		return;

	if (g_screenDrawData.nBkMode == OPAQUE)
		D2GFX_DrawRectangle(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, g_screenDrawData.iBrushColor, g_screenDrawData.iTransparency);

	DWORD dwOldFont = D2WIN_DrawTextFont(iFont);
	D2WIN_DrawText(lpwsz, lpRect->left, lpRect->bottom, min(g_screenDrawData.iTextColor, 14), 0);
	D2WIN_DrawTextFont((int)dwOldFont);	
}


BOOL D2TextOut(LPCSTR lpszText, long x, long y)
{
	if (!CheckDummyError("TextOut"))
		return FALSE;

	if (lpszText == NULL)
		return FALSE;

	const BYTE FONT = (BYTE)ValidateD2Font(g_screenDrawData.iFont);

	wchar_t wsz[256] = { 0 };
	::MultiByteToWideChar(CP_ACP, 0, lpszText, -1, wsz, 255);
	ReplaceColorSpecs(wsz);

	DWORD dwOldFont = D2WIN_DrawTextFont(FONT);
	
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.bottom = rect.top + D2WIN_GetFontHeight();

	if (g_screenDrawData.nBkMode == OPAQUE)
		rect.right = rect.left + GetTextSize(wsz, FONT).cx;
	
	D2WIN_DrawTextFont((int)dwOldFont);
	D2WTextOut(wsz, &rect, FONT);
	return TRUE;
}

BOOL D2DrawText(LPCSTR lpszText, LPCRECT lpRect, UINT nAlign)
{
	if (!CheckDummyError("DrawText"))
		return FALSE;

	if (lpszText == NULL || lpRect == NULL)
		return FALSE;
	
	RECT rcBunds = *lpRect;
	NormalizeRect(rcBunds);

	const BYTE FONT = (BYTE)ValidateD2Font(g_screenDrawData.iFont);
	wchar_t wsz[256] = { 0 };
	::MultiByteToWideChar(CP_ACP, 0, lpszText, -1, wsz, 255);
	ReplaceColorSpecs(wsz);

	const SIZE cz = GetTextSize(wsz, FONT);
	
	if (cz.cx == 0 || cz.cy == 0)
		return TRUE;

	RECT rect = *lpRect;
	const long RC_WIDTH = rcBunds.right - rcBunds.left;
	const long RC_HEIGHT = rcBunds.bottom - rcBunds.top;	

	if ((nAlign & DT_RIGHT) == DT_RIGHT)
	{
		rect.left = rcBunds.right - cz.cx;
	}

	if ((nAlign & DT_CENTER) == DT_CENTER)
	{
		rect.left = rcBunds.left + RC_WIDTH / 2 - cz.cx / 2;
	}

	if ((nAlign & DT_BOTTOM) == DT_BOTTOM)
	{
		rect.top = rcBunds.bottom - cz.cy;
	}

	if ((nAlign & DT_VCENTER) == DT_VCENTER)
	{
		rect.top = rcBunds.top + RC_HEIGHT / 2 - cz.cy / 2;
	}

	rect.right = rect.left + cz.cx;
	rect.bottom = rect.top + cz.cy;
	D2WTextOut(wsz, &rect, FONT);
	return TRUE;
}

POINT D2MoveTo(long x, long y)
{
	if (!CheckDummyError("MoveTo"))
	{
		POINT pt = {0};
		return pt;
	}

	POINT pt = g_screenDrawData.pos;
	g_screenDrawData.pos.x = x;
	g_screenDrawData.pos.y = y;
	return pt;
}

BOOL D2LineTo(long x, long y)
{
	if (!CheckDummyError("LineTo"))
		return FALSE;

	POINT pt1 = g_screenDrawData.pos;
	POINT pt2 = { x, y };
	D2GFX_DrawLine(pt1.x, pt1.y, pt2.x, pt2.y, g_screenDrawData.iPenColor, -1);

	if (g_screenDrawData.nPenWidth > 1)
	{
		const int ANGLE = CalculateAngle(pt1, pt2) + 270;
		for (int i = 1; i < g_screenDrawData.nPenWidth; i++)
		{
			POINT p1 = CalculatePointOnTrack(pt1, i, ANGLE);
			POINT p2 = CalculatePointOnTrack(pt2, i, ANGLE);
			D2GFX_DrawLine(p1.x, p1.y, p2.x, p2.y, g_screenDrawData.iPenColor, -1);
		}
	}	

	g_screenDrawData.pos.x = x;
	g_screenDrawData.pos.y = y;
	return TRUE;
}

BOOL D2FrameRect(LPCRECT lpRect)
{
	if (!CheckDummyError("Frame"))
		return FALSE;

	if (lpRect == NULL)
		return FALSE;

	RECT rect = *lpRect;
	NormalizeRect(rect);

	for (int i = 0; i < g_screenDrawData.nPenWidth && !::IsRectEmpty(&rect); i++)
	{
		D2GFX_DrawLine(rect.left, rect.top, rect.right, rect.top, g_screenDrawData.iPenColor, -1);
		D2GFX_DrawLine(rect.right, rect.top, rect.right, rect.bottom, g_screenDrawData.iPenColor, -1);
		D2GFX_DrawLine(rect.right, rect.bottom, rect.left, rect.bottom, g_screenDrawData.iPenColor, -1);
		D2GFX_DrawLine(rect.left, rect.bottom, rect.left, rect.top, g_screenDrawData.iPenColor, -1);
		::InflateRect(&rect, -1, -1);
	}
	
	return TRUE;
}

BOOL D2FillRect(LPCRECT lpRect)
{
	if (!CheckDummyError("Rectangle"))
		return FALSE;

	if (lpRect == NULL)
		return FALSE;

	D2GFX_DrawRectangle(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, g_screenDrawData.iBrushColor, g_screenDrawData.iTransparency);
	return TRUE;
}

BOOL D2Rectangle(LPCRECT lpRect)
{
	return D2FillRect(lpRect) && D2FrameRect(lpRect);
}

BYTE D2SetFont(BYTE iFont)
{
	BYTE iOld = g_screenDrawData.iFont;
	g_screenDrawData.iFont = iFont;
	return iOld;
}

BYTE D2GetFont()
{
	return g_screenDrawData.iFont;
}

BYTE D2SetTextColor(BYTE iColor)
{
	BYTE iOld = g_screenDrawData.iTextColor;
	g_screenDrawData.iTextColor = min(iColor, 14);
	return iOld;
}

BYTE D2GetTextColor()
{
	return g_screenDrawData.iTextColor;
}

BYTE D2SetPenColor(BYTE iColor)
{
	BYTE iOld = g_screenDrawData.iPenColor;
	g_screenDrawData.iPenColor = iColor;
	return iOld;
}

BYTE D2GetPenColor()
{
	return g_screenDrawData.iPenColor;
}

int D2SetPenWidth(int nWidth)
{
	int nOld = g_screenDrawData.nPenWidth;
	g_screenDrawData.nPenWidth = nWidth;
	return nOld;
}

int D2GetPenWidth()
{
	return g_screenDrawData.nPenWidth;
}

BYTE D2SetBrushColor(BYTE iColor)
{
	BYTE iOld = g_screenDrawData.iBrushColor;
	g_screenDrawData.iBrushColor = iColor;
	return iOld;
}

BYTE D2GetBrushColor()
{
	return g_screenDrawData.iBrushColor;
}

BYTE D2SetBrushTransparency(BYTE iTransparency)
{
	BYTE iOld = g_screenDrawData.iTransparency;
	g_screenDrawData.iTransparency = iTransparency;
	return iOld;
}

BYTE D2GetBrushTransparency()
{
	return g_screenDrawData.iTransparency;
}

int ValidateD2Font(int nFont)
{
	return nFont >= 0 && nFont < 14 ? nFont : D2FONT_SMOOTH;
}

int ReplaceColorSpecs(LPWSTR lpwsz)
{
	// Prevent the "ÿc" symbol to be converted into wchar_t
	if (lpwsz == NULL)
		return 0;

	const int LEN = wcslen(lpwsz);
	int nCount = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xf8f5) // Unicode 'ÿ'
		{
			lpwsz[i] = 0xff; // Ansi 'ÿ'
			nCount++;		
		}
	}

	return nCount;
}

int RemoveColorSpecs(LPWSTR lpwsz)
{
	// for checking text size, need to remove "ÿc3"'s first because
	// they shouldn't be counted in as text
	if (lpwsz == NULL)
		return 0;

	const int LEN = wcslen(lpwsz);
	wchar_t* p = new wchar_t[LEN + 1];
	::memset(p, 0, sizeof(wchar_t) * (LEN + 1));

	int nIndex = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xf8f5 || lpwsz[i] == 0xff)
			i += 2;
		else
			p[nIndex++] = lpwsz[i];
	}

	wcscpy(lpwsz, p);
	return wcslen(lpwsz);
}

SIZE GetTextSize(LPCWSTR lpwszText, int nFont)
{
	SIZE cResult = { 0 };

	if (lpwszText == NULL || lpwszText[0] == 0)
		return cResult; // invalid

	nFont = ValidateD2Font(nFont);

	wchar_t wsz[256] = {0};
	::wcsncpy(wsz, lpwszText, 255);
	RemoveColorSpecs(wsz);

	DWORD dwOldFont = D2WIN_DrawTextFont(nFont);
	cResult.cx = D2WIN_GetTextWidth(wsz);
	cResult.cy = D2WIN_GetFontHeight();
	D2WIN_DrawTextFont((int)dwOldFont);
	return cResult;
}


SIZE GetTextSize(LPCSTR lpszText, int nFont)
{
	SIZE cResult = { 0 };
	
	if (lpszText == NULL || lpszText[0] == '\0')
		return cResult; // invalid

	nFont = ValidateD2Font(nFont);

	wchar_t wsz[256] = {0};
	::MultiByteToWideChar(CP_ACP, 0, lpszText, -1, wsz, 255);
	RemoveColorSpecs(wsz);

	DWORD dwOldFont = D2WIN_DrawTextFont(nFont);
	cResult.cx = D2WIN_GetTextWidth(wsz);
	cResult.cy = D2WIN_GetFontHeight();
	D2WIN_DrawTextFont((int)dwOldFont);
	return cResult;
}

SIZE D2GetTextExtent(LPCSTR lpszText)
{
	return GetTextSize(lpszText, g_screenDrawData.iFont);
}

int D2SetBkMode(int nBkMode)
{
	int nOld = g_screenDrawData.nBkMode;
	g_screenDrawData.nBkMode = nBkMode;
	return nOld;
}

int D2GetBkMode()
{
	return g_screenDrawData.nBkMode;
}

////////////////////////////////////////////////////////////////////
// Dummy functions, Just Give Error Messages
////////////////////////////////////////////////////////////////////
BOOL CheckDummyError(LPCSTR lpszError)
{
	if (g_bIsDrawing)
		return TRUE;

	GameErrorf("ÿc4%s: ÿc8Function only allowed in \"ÿc2OnDrawÿc8\"!", lpszError);
	return FALSE;
}