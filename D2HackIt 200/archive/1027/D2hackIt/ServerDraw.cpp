//////////////////////////////////////////////////////////////////
// ServerDraw.cpp
//----------------------------------------------------------------
// Internal drawing functions used by D2Hackit itself.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////

#include "ServerDraw.h"
#include <stdio.h>
#include "D2HackIt.h"

BOOL g_bDrawBanner = TRUE;
BOOL g_bDrawMyStats = FALSE;

void DrawBanner(CGameDC* pDC, LPCRECT lpScreenRect);
void DrawMyStats(CGameDC* pDC, LPCRECT lpScreenRect);

void ServerDraw(CGameDC* pDC, LPCRECT lpScreenRect)
{
	DrawBanner(pDC, lpScreenRect);
	DrawMyStats(pDC, lpScreenRect);
}

/////////////////////////////////////////////////////
// DrawBanner
//
// Draw the text "D2Hackit! vx.xx" on the top-middle screen
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
	
	pDC->SetPenColor(0x1A);
	pDC->SetFont(D2FONT_SMOOTH);
	
	SIZE cText = pDC->GetTextExtent(szBanner);	
	RECT rect = {0};
	rect.left = lpScreenRect->right / 2 - cText.cx / 2;
	rect.right = rect.left + cText.cx;
	rect.top = 7;
	rect.bottom = rect.top + cText.cy;

	::InflateRect(&rect, 7, 5);
	pDC->Rectangle(&rect);
	::InflateRect(&rect, -2, -2);
	pDC->SetPenColor(0x07);
	pDC->FrameRect(&rect); // inner frame
	pDC->DrawText(szBanner, &rect, DT_CENTER | DT_VCENTER); // text
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

	pDC->SetFont(D2FONT_THIN);
	pDC->SetPenColor(0x1A);

	RECT rect;
	rect.left = lpScreenRect->right - 210;
	rect.right = lpScreenRect->right - 10;
	rect.top = lpScreenRect->bottom - 460;
	rect.bottom = lpScreenRect->bottom - 110;

	pDC->Rectangle(&rect);

	RECT rectBottom = rect;
	rectBottom.top = rectBottom.bottom - 27;
	rect.bottom -= 25;

	pDC->SetPenColor(0x07);
	
	::InflateRect(&rect, -2, -2);
	pDC->FrameRect(&rect);
	::InflateRect(&rectBottom, -2, -2);
	pDC->FrameRect(&rectBottom);

	const long GAP = 15;
	int num = 0;
	char szText[256] = "";

	long x = rect.left + 10;
	long y = rect.top + 8;

	// file res
	num = GetStat(STAT_FIRERESIST);
	sprintf(szText, "ÿc7Fire Resistÿc0 %3d/%3d/%3d", num, num - 40, num - 100);
	pDC->TextOut(szText, x, y);
	y += GAP;

	// cold res
	num = GetStat(STAT_COLDRESIST);
	sprintf(szText, "ÿc7Cold Resistÿc0 %3d/%3d/%3d", num, num - 40, num - 100);
	pDC->TextOut(szText, x, y);
	y += GAP;

	// lightning res
	num = GetStat(STAT_LIGHTNINGRESIST);
	sprintf(szText, "ÿc7Lightning Resistÿc0 %3d/%3d/%3d", num, num - 40, num - 100);
	pDC->TextOut(szText, x, y);
	y += GAP;

	// poison res
	num = GetStat(STAT_POISONRESIST);
	sprintf(szText, "ÿc7Poison Resistÿc0 %3d/%3d/%3d", num, num - 40, num - 100);
	pDC->TextOut(szText, x, y);
	y += GAP;

	// physic res
	sprintf(szText, "ÿc7Physic Resistÿc0 %3d", GetStat(STAT_DAMAGERESIST));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// magic res
	sprintf(szText, "ÿc7Magic Resistÿc0 %3d", GetStat(STAT_MAGICRESIST));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// mdr
	sprintf(szText, "ÿc7Damage Reductionÿc0 %3d", GetStat(STAT_DAMAGEREDUCTION));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// pdr
	sprintf(szText, "ÿc7Magic Damage Reductionÿc0 %3d", GetStat(STAT_MAGICDAMAGEREDUCTION));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// poison length reduction
	sprintf(szText, "ÿc7Poison Length Reductionÿc0 %3d%%", GetStat(STAT_POISONLENGTHREDUCTION));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// fire absorb	
	sprintf(szText, "ÿc7Fire Absorbÿc0 %3d + %3d%%", GetStat(STAT_FIREABSORB), GetStat(STAT_FIREABSORBPERCENT));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// cold absorb
	sprintf(szText, "ÿc7Cold Absorbÿc0 %3d + %3d%%", GetStat(STAT_COLDABSORB), GetStat(STAT_COLDABSORBPERCENT));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// lightning absorb
	sprintf(szText, "ÿc7Lightning Absorbÿc0 %3d + %3d%%", GetStat(STAT_LIGHTNINGABSORB), GetStat(STAT_LIGHTNINGABSORBPERCENT));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// crushing blow
	sprintf(szText, "ÿc7Crushing Blowÿc0 %3d%%", GetStat(STAT_CRUSHINGBLOW));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// deadly strike
	sprintf(szText, "ÿc7Deadly Strikeÿc0 %3d%%", GetStat(STAT_DEADLYSTRIKE));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// IAS
	sprintf(szText, "ÿc7Increased Attack Speedÿc0 %3d%%", GetStat(STAT_IAS));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// faster cast
	sprintf(szText, "ÿc7Faster Cast Rateÿc0 %3d%%", GetStat(STAT_FASTERCAST));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// faster run
	sprintf(szText, "ÿc7Faster Run/Walkÿc0 %3d%%", GetStat(STAT_FASTERRUNWALK));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// faster block
	sprintf(szText, "ÿc7Faster Block Rateÿc0 %3d%%", GetStat(STAT_FASTERBLOCK));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// faster recovery
	sprintf(szText, "ÿc7Faster Hit Recoveryÿc0 %3d%%", GetStat(STAT_FASTERHITRECOVERY));
	pDC->TextOut(szText, x, y);
	y += GAP;

	// magic find
	sprintf(szText, "ÿc7Magic Findÿc0 %3d%%", GetStat(STAT_MAGICFIND));
	pDC->TextOut(szText, x, y);
	y += GAP;	

	// draw "Press any key to close..."
	strcpy(szText, "ÿc7Press Any Key to Close");
	pDC->DrawText(szText, &rectBottom, DT_CENTER | DT_VCENTER);
}


