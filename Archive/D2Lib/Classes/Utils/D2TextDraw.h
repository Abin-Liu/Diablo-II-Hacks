//////////////////////////////////////////////////////////////////////
// D2TextDraw.h
//
// CD2TextDraw, a class for drawing text on the Diablo II game screen.
//
// Written by Abin (abinn32@yahoo.com)
//
// August 16th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __D2TEXTDRAW_H__
#define __D2TEXTDRAW_H__

#include <windows.h>

///////////////////////////////////////////////////
// Diablo 2 Fonts
///////////////////////////////////////////////////
#define D2FONT_SMALL		6	// Small font
#define D2FONT_THIN			4	// Thin font
#define D2FONT_NORMAL		0	// Normal font
#define D2FONT_SMOOTH		13	// Smooth font
#define D2FONT_BIGSMOOTH	5	// Big smooth font
#define D2FONT_CAPS			1	// Capital font
#define D2FONT_ROUGH		7	// Rough Font
#define D2FONT_BIGROUGH		2	// Big rough font
#define D2FONT_BIGBIGROUGH	3	// Big big rough font
#define D2FONT_SMOKE		11	// Smoke font

///////////////////////////////////////////////////
// Drawing Capacities
///////////////////////////////////////////////////
#define D2DRAW_MAX_TEXT		255 // Maximum text length that can be drawn in one element
#define D2DRAW_MAX_DATA		64  // Maximum elements can be drawn on the screen at a time

//////////////////////////////////////////////////////////////////////
// Drawing Element Data Definition
//////////////////////////////////////////////////////////////////////
typedef struct tagD2DrawData
{
	char szText[D2DRAW_MAX_TEXT + 1]; // Text to be drawn
	BOOL bRightAlign;				  // Right alignment
	long x;							  // Screen coordinates x
	long y;							  // Screen coordinates y
	int nFont;						  // Text font(0-13)
} D2DRAWDATA, *LPD2DRAWDATA;
typedef const tagD2DrawData* LPCD2DRAWDATA;


//////////////////////////////////////////////////////////////////////
// CD2TextDraw Class Definition
//////////////////////////////////////////////////////////////////////
class CD2TextDraw  
{
public:
		
	//////////////////////////////////////////////////////////////////
	// Constructor & Destructor
	//////////////////////////////////////////////////////////////////
	CD2TextDraw();
	virtual ~CD2TextDraw();

	//////////////////////////////////////////////////////////////////
	// Drawing Functions
	//////////////////////////////////////////////////////////////////
	BOOL DrawSingleText(LPCSTR lpszText, long x, long y, int nColor, int nFont = 0, BOOL bRightAlign = FALSE); // Draw one text
	BYTE DrawMultipleTexts(LPCD2DRAWDATA aData, BYTE iDataCount); // Draw multiple texts
	BOOL Undraw(); // Undraw, clean previously drawn texts
	static SIZE GetTextSize(LPCSTR lpszText, int nFont); // Retrieves text size

	//////////////////////////////////////////////////////////////////
	// Color/Font Validation
	//////////////////////////////////////////////////////////////////
	static int ValidateD2Font(int nFont); // Whether a font is valid

protected:

	//////////////////////////////////////////////////////////////////
	// Protected Methods
	//////////////////////////////////////////////////////////////////
	static int RemoveColorSpecs(LPWSTR lpwsz);
	static int ReplaceColorSpecs(LPWSTR lpwsz);
	static void Draw();	// Internal drawing function
	BOOL WriteMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize) const;
	BOOL PatchCall(DWORD dwAddress, LPVOID lpvFunction);
	
	//////////////////////////////////////////////////////////////////
	// Member data
	//////////////////////////////////////////////////////////////////
	BOOL m_bDrawn; // Is there any texts that were drawn by this object?
};

#endif // __D2TEXTDRAW_H__
