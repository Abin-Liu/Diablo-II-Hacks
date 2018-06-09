// D2TextDraw.cpp: implementation of the CD2TextDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "D2TextDraw.h"

#define WIN_DRAWTEXT				0x2785 
#define WIN_DRAWFONT				0x278F
#define CLIENT_FINISHFRAMEDISPLAY	0x6FAA97EA 

//////////////////////////////////////////////////////////////////////
// Drawing Function Definition
//////////////////////////////////////////////////////////////////////
typedef void (__fastcall *fpWinDrawText)(LPCWSTR lpwszText, int x, int y, int nColor, int nReserved);
typedef DWORD (__fastcall *fpWinDrawFont)(int nFontNo);

//////////////////////////////////////////////////////////////////////
// Drawing Data Controller Definition
//////////////////////////////////////////////////////////////////////
typedef struct tagD2DrawStruct
{
	fpWinDrawText lpfnDraw;
	fpWinDrawFont lpfnFont;
	BYTE iDataCount;
	D2DRAWDATA aData[D2DRAW_MAX_DATA];
} D2DRAWSTRUCT, *LPD2DRAWSTRUCT;
typedef const tagD2DrawStruct* LPCD2DRAWSTRUCT;

D2DRAWSTRUCT g_dataStruct = { 0 }; // The global data struct

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2TextDraw::CD2TextDraw()
{
	m_bDrawn = FALSE;

	// Obtain function address from D2Win.dll
	HMODULE hModule = ::GetModuleHandle("D2Win");
	if (hModule != NULL)
	{
		FARPROC fpFunc = ::GetProcAddress(hModule, (LPCTSTR)WIN_DRAWTEXT);
		if (fpFunc != NULL) 
			::memcpy(&g_dataStruct.lpfnDraw, &fpFunc, sizeof(FARPROC)); 

		fpFunc = NULL;
		fpFunc = ::GetProcAddress(hModule, (LPCTSTR)WIN_DRAWFONT);
		if (fpFunc != NULL) 
			::memcpy(&g_dataStruct.lpfnFont, &fpFunc, sizeof(FARPROC)); 
	}	
}

CD2TextDraw::~CD2TextDraw()
{
	if (m_bDrawn)
		Undraw(); // Undraw before exiting!
}

BOOL CD2TextDraw::DrawSingleText(LPCSTR lpszText, long x, long y, int nColor, int nFont)
{
	if (g_dataStruct.lpfnDraw == NULL || lpszText == NULL)
		return FALSE;

	D2DRAWDATA data = { 0 };
	data.nColor = nColor;
	data.nFont = nFont;
	data.x = x;
	data.y = y;
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
	if (!PatchCall(CLIENT_FINISHFRAMEDISPLAY, Draw))
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
	if (!WriteMemory((LPVOID)CLIENT_FINISHFRAMEDISPLAY, bBuffer, 5))
		return FALSE;

	m_bDrawn = FALSE;
	return TRUE;
}

void CD2TextDraw::Draw()
{
	if (g_dataStruct.lpfnDraw == NULL || g_dataStruct.iDataCount == 0)
		return;

	wchar_t wsz[D2DRAW_MAX_TEXT + 1] = { 0 };
	int nColor = 0;
	DWORD dwOldFont = -1;
	for (BYTE i = 0; i < g_dataStruct.iDataCount && i < D2DRAW_MAX_DATA; i++)
	{
		nColor = g_dataStruct.aData[i].nColor;
		if (!IsValidD2Color(nColor)) // color greater than 20 will crash the game
			nColor = 0;
		
		::MultiByteToWideChar(CP_ACP, 0, g_dataStruct.aData[i].szText, -1, wsz, D2DRAW_MAX_TEXT);
		if (g_dataStruct.lpfnFont)
			dwOldFont = g_dataStruct.lpfnFont(g_dataStruct.aData[i].nFont);

		g_dataStruct.lpfnDraw(wsz, g_dataStruct.aData[i].x, g_dataStruct.aData[i].y, nColor, 0);

		if (g_dataStruct.lpfnFont)
			g_dataStruct.lpfnFont((int)dwOldFont);
	}
}

BOOL CD2TextDraw::IsValidD2Color(int nColor)
{
	BOOL bOK = FALSE;
	switch (nColor)
	{
	case D2T_WHITE:
	case D2T_RED:
	case D2T_GREEN:
	case D2T_BLUE:
	case D2T_GOLDEN:
	case D2T_DARKDARKGRAY:
	case D2T_LIGHTGOLDEN:
	case D2T_ORANGE:
	case D2T_YELLOW:
	case D2T_DARKDARKGREEN:
	case D2T_PURPLE:
	case D2T_DARKGREEN:
	case D2T_WHITE1:	
	case D2T_GRAY:
	case D2T_DARKGRAY:
	case D2T_MIXED:
	case D2T_HIGHLIGHT:
	case D2T_RED1:
	case D2T_LIGHTYELLOW:
		bOK = TRUE;
		break;

	default:
		bOK = FALSE;
		break;
	}
	return bOK;
}
