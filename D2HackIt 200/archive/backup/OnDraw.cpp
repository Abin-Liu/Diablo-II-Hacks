//////////////////////////////////////////////////////////////////////
// OnDraw.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "D2Hackit.h"
#include <stdio.h>
#include "d2functions.h"
#include "Server20.h"
#include "D2HackItServer.h"
#include "D2HackIt.h"

BOOL g_bDrawBanner = TRUE;
BOOL g_bDrawMyStats = FALSE;
#define DEFAULT_FONTINDEX			8
BOOL g_bIsDrawing = FALSE;
CGameDC g_dc = {0};
BOOL g_bValid = FALSE;
DWORD g_dwOldCode = 0;

int RemoveColorSpecs(LPWSTR lpwsz);
int ReplaceColorSpecs(LPWSTR lpwsz);
BOOL WriteMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize);
BOOL ReadMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize);
int ValidateD2Font(int nFont); // Whether a font is valid
void InitializeGameDC(LPRECT lpRect);
void DrawBanner(CGameDC* pDC, LPCRECT lpScreenRect);
void DrawMyStats(CGameDC* pDC, LPCRECT lpScreenRect);

BOOL D2TextOut(LPCSTR lpszText, long x, long y, BYTE iFont = D2FONT_NORMAL);
BOOL D2DrawText(LPCSTR lpszText, LPCRECT lpRect, BYTE iFont = D2FONT_NORMAL, UINT nAlign = DT_LEFT);
POINT D2MoveTo(long x, long y);
BOOL D2LineTo(long x, long y, BYTE iColor = 0x20);
BOOL D2Frame(LPCRECT lpRect, BYTE iColor = 0x20);
BOOL D2Rectangle(LPCRECT lpRect, BYTE iColor = 0x20, BYTE iTransparency = -1);
BOOL CheckDummyError(LPCSTR lpszError);

BOOL D2InitializeScreenDraw()
{
	// save original patch code ASAP
	g_dwOldCode = 0;
	g_bValid = FALSE;
	if (!ReadMemory((LPVOID)(D2CLIENT_ScreenPrint+1), &g_dwOldCode, 4))
		return FALSE;

	// patch it
	DWORD NewScreenOffset = (DWORD)&GameOnDrawIntercept - (D2CLIENT_ScreenPrint+1+4); // offset from d2bs
	g_bValid = WriteMemory((LPBYTE)(D2CLIENT_ScreenPrint+1), &NewScreenOffset, 4);
	InitializeGameDC(NULL);
	return g_bValid;
}

BOOL D2UninitializeScreenDraw()
{	
	g_bDrawBanner = FALSE;
	g_bDrawMyStats = FALSE;
	g_bIsDrawing = FALSE;

	if (!g_bValid)
		return FALSE;
	
	// restore old code to unpatch
	g_bValid = FALSE;
	if (g_dwOldCode)
		WriteMemory((LPBYTE)(D2CLIENT_ScreenPrint+1), &g_dwOldCode, 4);	

	return TRUE;
}

void InitializeGameDC(LPRECT lpRect)
{
	g_dc.DrawText = D2DrawText;
	g_dc.Frame = D2Frame;
	g_dc.LineTo = D2LineTo;
	g_dc.MoveTo = D2MoveTo;
	g_dc.Rectangle = D2Rectangle;
	g_dc.TextOut = D2TextOut;

	if (lpRect)
	{
		SIZE cs = GetScreenSize();
		lpRect->left = 0;
		lpRect->top = 0;
		lpRect->right = cs.cx;
		lpRect->bottom = cs.cy;
	}
	
	::memset(&g_dc.ptPosition, 0, sizeof(POINT));
}

BOOL WriteMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize)
{
	DWORD dwOldProtect, dwDummy; 
	return ::VirtualProtect(lpvAddress, dwSize, PAGE_READWRITE, &dwOldProtect)
		&& ::WriteProcessMemory(::GetCurrentProcess(), lpvAddress, lpvData, dwSize, NULL)
		&& ::VirtualProtect(lpvAddress, dwSize, dwOldProtect, &dwDummy);
}

BOOL ReadMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize)
{
	DWORD dwOldProtect, dwDummy; 
	return ::VirtualProtect(lpvAddress, dwSize, PAGE_READWRITE, &dwOldProtect)
		&& ::ReadProcessMemory(::GetCurrentProcess(), lpvAddress, lpvData, dwSize, NULL)
		&& ::VirtualProtect(lpvAddress, dwSize, dwOldProtect, &dwDummy);
}

void __fastcall GameOnDrawIntercept()
{
	if (!IsGameJoined())
		return;		
	
	D2CLIENT_DrawClientScreen();

	g_bIsDrawing = TRUE;
	
	RECT rc;

	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnDraw)
		{
			InitializeGameDC(&rc);	
			pCIS->OnDraw(&g_dc, &rc);
		}
		//else
		//	GamePrintError("OnDraw error!");

		li = ClientModules.GetNextItem(li);
	}

	InitializeGameDC(&rc);			
	DrawBanner(&g_dc, &rc);
	DrawMyStats(&g_dc, &rc);
	g_bIsDrawing = FALSE;
}

BOOL D2TextOut(LPCSTR lpszText, long x, long y, BYTE iFont)
{
	if (!CheckDummyError("TextOut"))
		return FALSE;

	if (lpszText == NULL)
		return FALSE;

	iFont = (BYTE)ValidateD2Font(iFont);

	wchar_t wsz[256 + 1] = { 0 };
	::MultiByteToWideChar(CP_ACP, 0, lpszText, -1, wsz, 255);
	ReplaceColorSpecs(wsz);

	DWORD dwOldFont = D2WIN_DrawTextFont(iFont);
	long nHeight = D2WIN_GetFontHeight();
	D2WIN_DrawText(wsz, x, y + nHeight, 0, 0);
	D2WIN_DrawTextFont((int)dwOldFont);
	return TRUE;
}

BOOL D2DrawText(LPCSTR lpszText, LPCRECT lpRect, BYTE iFont, UINT nAlign)
{
	if (!CheckDummyError("DrawText"))
		return FALSE;

	if (lpszText == NULL || lpRect == NULL)
		return FALSE;

	iFont = (BYTE)ValidateD2Font(iFont);
	const SIZE TXT_SIZE = GetTextSize(lpszText, iFont);
	if (TXT_SIZE.cx == 0 || TXT_SIZE.cy == 0)
		return TRUE;

	POINT pt = { lpRect->left, lpRect->top };
	const long RC_WIDTH = abs(lpRect->right - lpRect->left);
	const long RC_HEIGHT = abs(lpRect->bottom - lpRect->top);	

	if ((nAlign & DT_RIGHT) == DT_RIGHT)
	{
		pt.x = lpRect->right - TXT_SIZE.cx;
	}

	if ((nAlign & DT_CENTER) == DT_CENTER)
	{
		pt.x = lpRect->left + RC_WIDTH / 2 - TXT_SIZE.cx / 2;
	}

	if ((nAlign & DT_BOTTOM) == DT_BOTTOM)
	{
		pt.y = lpRect->bottom - TXT_SIZE.cy;
	}

	if ((nAlign & DT_VCENTER) == DT_VCENTER)
	{
		pt.y = lpRect->top + RC_HEIGHT / 2 - TXT_SIZE.cy / 2;
	}
	
	return D2TextOut(lpszText, pt.x, pt.y, iFont);
}

POINT D2MoveTo(long x, long y)
{
	if (!CheckDummyError("MoveTo"))
	{
		POINT pt = {0};
		return pt;
	}

	POINT pt = g_dc.ptPosition;
	g_dc.ptPosition.x = x;
	g_dc.ptPosition.y = y;
	return pt;
}

BOOL D2LineTo(long x, long y, BYTE iColor)
{
	if (!CheckDummyError("LineTo"))
		return FALSE;

	D2GFX_DrawLine(g_dc.ptPosition.x, g_dc.ptPosition.y, x, y, iColor, -1);
	g_dc.ptPosition.x = x;
	g_dc.ptPosition.y = y;
	return TRUE;
}

BOOL D2Frame(LPCRECT lpRect, BYTE iColor)
{
	if (!CheckDummyError("Frame"))
		return FALSE;

	if (lpRect == NULL)
		return FALSE;

	D2GFX_DrawLine(lpRect->left, lpRect->top, lpRect->right, lpRect->top, iColor, -1);
	D2GFX_DrawLine(lpRect->right, lpRect->top, lpRect->right, lpRect->bottom, iColor, -1);
	D2GFX_DrawLine(lpRect->right, lpRect->bottom, lpRect->left, lpRect->bottom, iColor, -1);
	D2GFX_DrawLine(lpRect->left, lpRect->bottom, lpRect->left, lpRect->top, iColor, -1);
	return TRUE;
}

BOOL D2Rectangle(LPCRECT lpRect, BYTE iColor, BYTE iTransparency)
{
	if (!CheckDummyError("Rectangle"))
		return FALSE;

	if (lpRect == NULL)
		return FALSE;

	D2GFX_DrawRectangle(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, iColor, iTransparency);
	return TRUE;
}

int ValidateD2Font(int nFont)
{
	return nFont >= 0 && nFont < 14 ? nFont : DEFAULT_FONTINDEX;
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
		if (lpwsz[i] == 0xf8f5)
			i += 2;
		else
			p[nIndex++] = lpwsz[i];
	}

	wcscpy(lpwsz, p);
	return wcslen(lpwsz);
}

SIZE EXPORT GetTextSize(LPCSTR lpszText, int nFont)
{
	SIZE cResult = { 0 };
	
	if (!g_bValid
		|| lpszText == NULL
		|| lpszText[0] == '\0')
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

/////////////////////////////////////////////////////
// DrawBanner
//
// Draw the text "D2Hackit! vx.xx" on the top-middle screen
// Available since 2.00, by Abin
/////////////////////////////////////////////////////
void DrawBanner(CGameDC* pDC, LPCRECT lpScreenRect)
{
	if (!g_bDrawBanner)
		return;		
				
	char szBanner[256] = "";
	sprintf(szBanner,
			"ÿc4D2Hackit! ÿc2%d.%02d ÿc4Build ÿc2%02d%02d", 
			LOWORD(__SERVERVERSION__),
			HIWORD(__SERVERVERSION__), 
			LOWORD(__SERVERBUILD__),
			HIWORD(__SERVERBUILD__));	

	
	SIZE cText = GetTextSize(szBanner, D2FONT_SMOOTH);	
	RECT rect = {0};
	rect.left = lpScreenRect->right / 2 - cText.cx / 2;
	rect.right = rect.left + cText.cx;
	rect.top = 7;
	rect.bottom = rect.top + cText.cy;

	::InflateRect(&rect, 7, 5);
	pDC->Rectangle(&rect, 0, 1); // solid rectangle
	pDC->Frame(&rect, 0x1a); // outter frame
	::InflateRect(&rect, -2, -2);
	pDC->Frame(&rect, 0x07); // inner frame
	pDC->DrawText(szBanner, &rect, D2FONT_SMOOTH, DT_CENTER | DT_VCENTER); // text
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

//////////////////////////////////////////////////////////////////
// DrawMyStats
//
// Draw player's stats
//////////////////////////////////////////////////////////////////
void DrawMyStats(CGameDC* pDC, LPCRECT lpScreenRect)
{
	if (!g_bDrawMyStats)
		return;	

	RECT rect;
	rect.left = lpScreenRect->right - 210;
	rect.right = lpScreenRect->right - 10;
	rect.top = lpScreenRect->bottom - 460;
	rect.bottom = lpScreenRect->bottom - 110;

	pDC->Rectangle(&rect, 0, 1);
	pDC->Frame(&rect, 0x1a);

	RECT rectBottom = rect;
	rectBottom.top = rectBottom.bottom - 27;
	rect.bottom -= 25;

	::InflateRect(&rect, -2, -2);
	pDC->Frame(&rect, 0x07);
	::InflateRect(&rectBottom, -2, -2);
	pDC->Frame(&rectBottom, 0x07);

	const BYTE STATSFONT = D2FONT_THIN;
	const long GAP = 15;
	int num = 0;
	char szText[256] = "";

	long x = rect.left + 10;
	long y = rect.top + 8;

	// file res
	num = GetStat(STAT_FIRERESIST);
	sprintf(szText, "ÿc7Fire Resistÿc0 %3d/%3d/%3d", num, num - 40, num - 100);
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// cold res
	num = GetStat(STAT_COLDRESIST);
	sprintf(szText, "ÿc7Cold Resistÿc0 %3d/%3d/%3d", num, num - 40, num - 100);
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// lightning res
	num = GetStat(STAT_LIGHTNINGRESIST);
	sprintf(szText, "ÿc7Lightning Resistÿc0 %3d/%3d/%3d", num, num - 40, num - 100);
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// poison res
	num = GetStat(STAT_POISONRESIST);
	sprintf(szText, "ÿc7Poison Resistÿc0 %3d/%3d/%3d", num, num - 40, num - 100);
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// physic res
	sprintf(szText, "ÿc7Physic Resistÿc0 %3d", GetStat(STAT_DAMAGERESIST));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// magic res
	sprintf(szText, "ÿc7Magic Resistÿc0 %3d", GetStat(STAT_MAGICRESIST));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// mdr
	sprintf(szText, "ÿc7Damage Reductionÿc0 %3d", GetStat(STAT_DAMAGEREDUCTION));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// pdr
	sprintf(szText, "ÿc7Magic Damage Reductionÿc0 %3d", GetStat(STAT_MAGICDAMAGEREDUCTION));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// poison length reduction
	sprintf(szText, "ÿc7Poison Length Reductionÿc0 %3d%%", GetStat(STAT_POISONLENGTHREDUCTION));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// fire absorb	
	sprintf(szText, "ÿc7Fire Absorbÿc0 %3d + %3d%%", GetStat(STAT_FIREABSORB), GetStat(STAT_FIREABSORBPERCENT));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// cold absorb
	sprintf(szText, "ÿc7Cold Absorbÿc0 %3d + %3d%%", GetStat(STAT_COLDABSORB), GetStat(STAT_COLDABSORBPERCENT));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// lightning absorb
	sprintf(szText, "ÿc7Lightning Absorbÿc0 %3d + %3d%%", GetStat(STAT_LIGHTNINGABSORB), GetStat(STAT_LIGHTNINGABSORBPERCENT));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// crushing blow
	sprintf(szText, "ÿc7Crushing Blowÿc0 %3d%%", GetStat(STAT_CRUSHINGBLOW));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// deadly strike
	sprintf(szText, "ÿc7Deadly Strikeÿc0 %3d%%", GetStat(STAT_DEADLYSTRIKE));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// IAS
	sprintf(szText, "ÿc7Increased Attack Speedÿc0 %3d%%", GetStat(STAT_IAS));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// faster cast
	sprintf(szText, "ÿc7Faster Cast Rateÿc0 %3d%%", GetStat(STAT_FASTERCAST));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// faster run
	sprintf(szText, "ÿc7Faster Run/Walkÿc0 %3d%%", GetStat(STAT_FASTERRUNWALK));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// faster block
	sprintf(szText, "ÿc7Faster Block Rateÿc0 %3d%%", GetStat(STAT_FASTERBLOCK));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// faster recovery
	sprintf(szText, "ÿc7Faster Hit Recoveryÿc0 %3d%%", GetStat(STAT_FASTERHITRECOVERY));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;

	// magic find
	sprintf(szText, "ÿc7Magic Findÿc0 %3d%%", GetStat(STAT_MAGICFIND));
	pDC->TextOut(szText, x, y, STATSFONT);
	y += GAP;	

	// draw "Press any key to close..."
	strcpy(szText, "ÿc7Press Any Key to Close");
	pDC->DrawText(szText, &rectBottom, STATSFONT, DT_CENTER | DT_VCENTER);
}

