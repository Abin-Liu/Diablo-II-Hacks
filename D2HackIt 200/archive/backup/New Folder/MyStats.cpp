///////////////////////////////////////////////////////////
// MyStats.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "MyStats.h"
#include "D2HackIt.h"
#include <stdio.h>
#include "d2functions.h"

DWORD g_dwStatsHookID = 0;

BOOL ShowMyStats()
{
	return 1;
	/*
	if (g_dwStatsHookID == 0)
		g_dwStatsHookID = CreateScreenHook();

	if (g_dwStatsHookID == 0)
		return FALSE;

	SIZE scr = GetScreenSize();
	const POINT PT1 = { scr.cx - 210, scr.cy - 470 };
	const POINT PT2 = { scr.cx - 10, scr.cy - 110 };	

#define DATACOUNT	25
	D2DRAWDATA aData[DATACOUNT] = { 0 };

	int nIndex = 0;

	// draw rectangle
	aData[nIndex].nDrawType = D2DRAW_RECT;
	aData[nIndex].nTransparency = 1;
	aData[nIndex].x = PT1.x;
	aData[nIndex].y = PT1.y;
	aData[nIndex].x2 = PT2.x;
	aData[nIndex].y2 = PT2.y;
	nIndex++;

	// draw outter frame
	aData[nIndex].nDrawType = D2DRAW_FRAME;
	aData[nIndex].nColor = 0x1a;
	aData[nIndex].x = PT1.x;
	aData[nIndex].y = PT1.y;
	aData[nIndex].x2 = PT2.x;
	aData[nIndex].y2 = PT2.y;
	nIndex++;

	// draw top inner frame
	aData[nIndex].nDrawType = D2DRAW_FRAME;
	aData[nIndex].nColor = 0x07;
	aData[nIndex].x = PT1.x + 2;
	aData[nIndex].y = PT1.y + 2;
	aData[nIndex].x2 = PT2.x - 2;
	aData[nIndex].y2 = PT2.y - 30;
	nIndex++;

	// draw bottom inner frame
	aData[nIndex].nDrawType = D2DRAW_FRAME;
	aData[nIndex].nColor = 0x07;
	aData[nIndex].x = PT1.x + 2;
	aData[nIndex].y = PT2.y - 28;
	aData[nIndex].x2 = PT2.x - 2;
	aData[nIndex].y2 = PT2.y - 2;
	nIndex++;

	// draw stats lines
#define STATSFONT	D2FONT_THIN
	DWORD dwOldFont = D2WIN_DrawTextFont(STATSFONT);
	const int GAP = D2WIN_GetFontHeight() + 1;
	D2WIN_DrawTextFont((int)dwOldFont);
	POINT pt = PT1;
	pt.x += 10;
	pt.y += 20;
	const int CENTERX = PT1.x + (PT2.x - PT1.x) / 2;

	int num = 0;

	// file res
	num = GetStat(STAT_FIRERESIST);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Fire Resist�c0 %3d/%3d/%3d", num, num - 40, num - 100);
	nIndex++;
	pt.y += GAP;

	// cold res
	num = GetStat(STAT_COLDRESIST);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Cold Resist�c0 %3d/%3d/%3d", num, num - 40, num - 100);
	nIndex++;
	pt.y += GAP;

	// lightning res
	num = GetStat(STAT_LIGHTNINGRESIST);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Lightning Resist�c0 %3d/%3d/%3d", num, num - 40, num - 100);
	nIndex++;
	pt.y += GAP;

	// poison res
	num = GetStat(STAT_POISONRESIST);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Poison Resist�c0 %3d/%3d/%3d", num, num - 40, num - 100);
	nIndex++;
	pt.y += GAP;

	// physic res
	num = GetStat(STAT_DAMAGERESIST);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Physic Resist�c0 %3d", num);
	nIndex++;
	pt.y += GAP;

	// magic res
	num = GetStat(STAT_MAGICRESIST);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Magic Resist�c0 %3d", num);
	nIndex++;
	pt.y += GAP;
	
	// mdr
	num = GetStat(STAT_DAMAGEREDUCTION);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Damage Reduction�c0 %3d", num);
	nIndex++;
	pt.y += GAP;

	// pdr
	num = GetStat(STAT_MAGICDAMAGEREDUCTION);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Magic Damage Reduction�c0 %3d", num);
	nIndex++;
	pt.y += GAP;

	// poison length reduction
	num = GetStat(STAT_POISONLENGTHREDUCTION);
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Poison Length Reduction�c0 %3d%%", num);
	nIndex++;
	pt.y += GAP;

	// fire absorb
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Fire Absorb�c0 %3d + %3d%%", GetStat(STAT_FIREABSORB), GetStat(STAT_FIREABSORBPERCENT));
	nIndex++;
	pt.y += GAP;

	// cold absorb
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Cold Absorb�c0 %3d + %3d%%", GetStat(STAT_COLDABSORB), GetStat(STAT_COLDABSORBPERCENT));
	nIndex++;
	pt.y += GAP;

	// lightning absorb
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Lightning Absorb�c0 %3d + %3d%%", GetStat(STAT_LIGHTNINGABSORB), GetStat(STAT_LIGHTNINGABSORBPERCENT));
	nIndex++;
	pt.y += GAP;

	// crushing blow
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Crushing Blow�c0 %3d%%", GetStat(STAT_CRUSHINGBLOW));
	nIndex++;
	pt.y += GAP;

	// deadly strike
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Deadly Strike�c0 %3d%%", GetStat(STAT_DEADLYSTRIKE));
	nIndex++;
	pt.y += GAP;

	// IAS
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Increased Attack Speed�c0 %3d%%", GetStat(STAT_IAS));
	nIndex++;
	pt.y += GAP;

	// faster cast
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Faster Cast Rate�c0 %3d%%", GetStat(STAT_FASTERCAST));
	nIndex++;
	pt.y += GAP;

	// faster run
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Faster Run/Walk�c0 %3d%%", GetStat(STAT_FASTERRUNWALK));
	nIndex++;
	pt.y += GAP;

	// faster block
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Faster Block Rate�c0 %3d%%", GetStat(STAT_FASTERBLOCK));
	nIndex++;
	pt.y += GAP;

	// faster recovery
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Faster Hit Recovery�c0 %3d%%", GetStat(STAT_FASTERHITRECOVERY));
	nIndex++;
	pt.y += GAP;

	// magic find
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = pt.x;
	aData[nIndex].y = pt.y;
	sprintf(aData[nIndex].szText, "�c7Magic Find�c0 %3d%%", GetStat(STAT_MAGICFIND));
	nIndex++;
	pt.y += GAP;

	// draw "Press any key to close..."
	pt.y += 11;
	aData[nIndex].nDrawType = D2DRAW_TEXT;
	aData[nIndex].nFont = STATSFONT;
	aData[nIndex].x = PT1.x + 40;
	aData[nIndex].y = pt.y;
	strcpy(aData[nIndex].szText, "�c7Press Any Key to Close");

	return DrawScreen(g_dwStatsHookID, aData, DATACOUNT);
	*/
}

BOOL CleanMyStats()
{
	if (g_dwStatsHookID == 0)
		return TRUE;

	//ReleaseScreenHook(g_dwStatsHookID);
	//g_dwStatsHookID = 0;
	return TRUE;
}