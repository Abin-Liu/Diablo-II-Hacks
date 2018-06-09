// D2TextDraw.cpp: implementation of the CD2TextDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "D2TextDraw.h"
#include "SharedMem.h"
#include <stdio.h>

#define D2WIN_DRAWTEXT				0x2785 
#define D2WIN_GETTEXTWIDTH			0x2789
#define D2WIN_GETFONTHEIGHT			0x278d
#define D2WIN_DRAWFONT				0x278F
#define D2CLIENT_FINISHFRAMEDISPLAY	0x6FAA97EA
#define D2GFX_GETHWND				0x272B
#define D2GFX_SCREENSIZE			0x2715

#define DEFAULT_FONTINDEX			8
#define MAX_DRAWS					255
#define DRAW_GUID					"{C596CABA-7EEA-447A-9D9B-4A8782787E71}"

//////////////////////////////////////////////////////////////////////
// Drawing Function Definition
//////////////////////////////////////////////////////////////////////
typedef void (__fastcall *fpWinDrawText)(LPCWSTR lpwszText, int x, int y, int nColor, int nReserved);
typedef DWORD (__fastcall *fpWinDrawFont)(int nFontNo);
typedef int (__fastcall *fpWinGetTextWidth)(LPWSTR lpwszText);
typedef BYTE (__fastcall *fpWinGetFontHeight)();
typedef HWND (__stdcall *fnGfxGetHWND)();
typedef DWORD (__stdcall *fnGfxGetScreenSize)();


//////////////////////////////////////////////////////////////////////
// Drawing Data Controller Definition
//////////////////////////////////////////////////////////////////////
typedef struct tagD2DrawStruct
{
	fpWinDrawText lpfnDraw;
	fpWinDrawFont lpfnFont;
	fpWinGetTextWidth lpfnTextWidth;
	fpWinGetFontHeight lpfnFontHeight;

	BYTE iDataCount;
	D2DRAWDATA aData[D2DRAW_MAX_DATA];
} D2DRAWSTRUCT, *LPD2DRAWSTRUCT;
typedef const tagD2DrawStruct* LPCD2DRAWSTRUCT;

D2DRAWSTRUCT g_dataStruct = { 0 }; // The global data struct

CSharedMem g_mem;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2TextDraw::CD2TextDraw()
{
	m_bDrawn = FALSE;
	m_hD2Wnd = NULL;	
	m_lpfnGfxGetScreenSize = NULL;

	// Obtain function address from D2Win.dll
	HMODULE hModule = ::GetModuleHandle("D2Win");
	if (hModule != NULL)
	{
		FARPROC fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2WIN_DRAWTEXT);
		if (fpFunc != NULL) 
			::memcpy(&g_dataStruct.lpfnDraw, &fpFunc, sizeof(FARPROC)); 

		fpFunc = NULL;
		fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2WIN_DRAWFONT);
		if (fpFunc != NULL) 
			::memcpy(&g_dataStruct.lpfnFont, &fpFunc, sizeof(FARPROC)); 

		fpFunc = NULL;
		fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2WIN_GETTEXTWIDTH);
		if (fpFunc != NULL) 
			::memcpy(&g_dataStruct.lpfnTextWidth, &fpFunc, sizeof(FARPROC));

		fpFunc = NULL;
		fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2WIN_GETFONTHEIGHT);
		if (fpFunc != NULL) 
			::memcpy(&g_dataStruct.lpfnFontHeight, &fpFunc, sizeof(FARPROC));
	}
	
	hModule = GetModuleHandle("D2Gfx");
	if (hModule != NULL)
	{
		FARPROC fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2GFX_GETHWND);
		if (fpFunc != NULL) 
		{
			fnGfxGetHWND lpfn = NULL;
			::memcpy(&lpfn, &fpFunc, sizeof(FARPROC));
			if (lpfn)
				m_hD2Wnd = lpfn();
		}

		m_lpfnGfxGetScreenSize = ::GetProcAddress(hModule, (LPCTSTR)D2GFX_SCREENSIZE);
	}

	// create shared memory
	char szMemID[256] = "";
	sprintf(szMemID, "%s%08X", DRAW_GUID, (DWORD)m_hD2Wnd);
	m_bSharedMemCreated = g_mem.Create(szMemID, 4 + MAX_DRAWS * sizeof(D2DRAWDATA));
}

CD2TextDraw::~CD2TextDraw()
{
	if (m_bDrawn)
		Undraw(); // Undraw before exiting!

	if (m_bSharedMemCreated)
		g_mem.Close();
}

BOOL CD2TextDraw::DrawSingleText(LPCSTR lpszText, long x, long y, int nColor, int nFont, BOOL bRightAlign)
{
	if (g_dataStruct.lpfnDraw == NULL || lpszText == NULL)
		return FALSE;

	D2DRAWDATA data = { 0 };
	data.nFont = nFont;
	data.x = x;
	data.y = y;
	data.bRightAlign = bRightAlign;
	strncpy(data.szText, lpszText, D2DRAW_MAX_TEXT);
	return DrawMultipleTexts(&data, 1) > 0;
}

BYTE CD2TextDraw::DrawMultipleTexts(LPCD2DRAWDATA aData, BYTE iDataCount)
{
	if (g_dataStruct.lpfnDraw == NULL || aData == NULL || iDataCount == 0)
		return 0;

	// copy the drawing data list
	iDataCount = min(iDataCount, D2DRAW_MAX_DATA);
	g_dataStruct.iDataCount = iDataCount;
	::memcpy(g_dataStruct.aData, aData, sizeof(D2DRAWDATA) * iDataCount);	
	if (!PatchCall(D2CLIENT_FINISHFRAMEDISPLAY, Draw))
		return 0;

	m_bDrawn = TRUE;
	return iDataCount;
}

BOOL CD2TextDraw::PatchCall(DWORD dwAddress, LPVOID lpvFunction)
{
	LONG lOffset; 
	BYTE bBuffer[5]; 
	LPBYTE lpbAddress; 

	lOffset = (LONG) lpvFunction - ((LONG) dwAddress + 5); 
	lpbAddress = (LPBYTE) dwAddress; 
	bBuffer[0] = 0xE8; 
	CopyMemory(&bBuffer[1], &lOffset, sizeof(LONG)); 
	return WriteMemory(lpbAddress, bBuffer, 5); 
}

BOOL CD2TextDraw::WriteMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize) const
{
	DWORD dwOldProtect, dwDummy; 
	return ::VirtualProtect(lpvAddress, dwSize, PAGE_READWRITE, &dwOldProtect)
		&& ::WriteProcessMemory(::GetCurrentProcess(), lpvAddress, lpvData, dwSize, NULL)
		&& ::VirtualProtect(lpvAddress, dwSize, dwOldProtect, &dwDummy);
}

BOOL CD2TextDraw::Undraw()
{	
	// clean texts
	if (g_dataStruct.lpfnDraw == NULL)
		return FALSE;

	BYTE bBuffer[5]; 
	FillMemory(bBuffer, 5, 0x90); 
	if (!WriteMemory((LPVOID)D2CLIENT_FINISHFRAMEDISPLAY, bBuffer, 5))
		return FALSE;

	m_bDrawn = FALSE;
	return TRUE;
}

void CD2TextDraw::Draw()
{
	if (g_dataStruct.lpfnDraw == NULL || g_dataStruct.iDataCount == 0)
		return;

	wchar_t wsz[D2DRAW_MAX_TEXT + 1] = { 0 };	
	DWORD dwOldFont = -1;
	for (BYTE i = 0; i < g_dataStruct.iDataCount && i < D2DRAW_MAX_DATA; i++)
	{
		int nFont = ValidateD2Font(g_dataStruct.aData[i].nFont);
		long x = g_dataStruct.aData[i].x;
		if (g_dataStruct.aData[i].bRightAlign)
			x -= GetTextSize(g_dataStruct.aData[i].szText, g_dataStruct.aData[i].nFont).cx;

		::MultiByteToWideChar(CP_ACP, 0, g_dataStruct.aData[i].szText, -1, wsz, D2DRAW_MAX_TEXT);
		ReplaceColorSpecs(wsz);

		if (g_dataStruct.lpfnFont)
			dwOldFont = g_dataStruct.lpfnFont(nFont);

		g_dataStruct.lpfnDraw(wsz, x, g_dataStruct.aData[i].y, 0, 0);

		if (g_dataStruct.lpfnFont)
			g_dataStruct.lpfnFont((int)dwOldFont);
	}
}

int CD2TextDraw::ValidateD2Font(int nFont)
{
	return nFont >= 0 && nFont < 14 ? nFont : DEFAULT_FONTINDEX;
}

SIZE CD2TextDraw::GetTextSize(LPCSTR lpszText, int nFont)
{
	SIZE cResult = { 0 };
	
	if (g_dataStruct.lpfnTextWidth == NULL
		|| g_dataStruct.lpfnFontHeight == NULL
		|| lpszText == NULL
		|| lpszText[0] == '\0')
		return cResult; // invalid

	nFont = ValidateD2Font(nFont);

	wchar_t wsz[D2DRAW_MAX_TEXT + 1] = {0};
	::MultiByteToWideChar(CP_ACP, 0, lpszText, -1, wsz, D2DRAW_MAX_TEXT);
	RemoveColorSpecs(wsz);

	DWORD dwOldFont = g_dataStruct.lpfnFont(nFont);
	cResult.cx = g_dataStruct.lpfnTextWidth(wsz);
	cResult.cy = g_dataStruct.lpfnFontHeight();
	g_dataStruct.lpfnFont((int)dwOldFont);
	return cResult;
}

int CD2TextDraw::ReplaceColorSpecs(LPWSTR lpwsz)
{
	if (lpwsz == NULL)
		return 0;

	const int LEN = wcslen(lpwsz);
	int nCount = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xf8f5)
		{
			lpwsz[i] = 0xff;
			nCount++;		
		}
	}

	return nCount;
}

int CD2TextDraw::RemoveColorSpecs(LPWSTR lpwsz)
{
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

HWND CD2TextDraw::GetD2WndHandle() const
{
	return m_hD2Wnd;
}

SIZE CD2TextDraw::GetScreenSize() const
{
	SIZE cz = { 640, 480 };
	if (m_lpfnGfxGetScreenSize == NULL)
		return cz;

	fnGfxGetScreenSize lpfn = NULL;
	::memcpy(&lpfn, &m_lpfnGfxGetScreenSize, sizeof(FARPROC));
	DWORD dw = lpfn ? lpfn() : 0;
	if (dw == 2)
	{
		cz.cx = 800;
		cz.cy = 600;
	}

	return cz;
}
