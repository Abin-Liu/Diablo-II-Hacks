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

//////////////////////////////////////////////////////////////////////
// Text Color Definition
//////////////////////////////////////////////////////////////////////
#define	D2T_WHITE			0	// White
#define	D2T_RED				1	// Red
#define	D2T_GREEN			2	// Green
#define	D2T_BLUE			3	// Blue
#define	D2T_GOLDEN			4	// Golden
#define	D2T_DARKDARKGRAY	5	// Dark dark gray
#define	D2T_LIGHTGOLDEN		7	// Light golden
#define	D2T_ORANGE			8	// Orange
#define	D2T_YELLOW			9	// Yellow
#define	D2T_DARKDARKGREEN	10	// Dark dark green
#define	D2T_PURPLE			11	// Purple
#define	D2T_DARKGREEN		12	// Dark green
#define D2T_WHITE1			13	// White
#define D2T_GRAY			15	// Gray
#define D2T_DARKGRAY		16	// Dark gray
#define D2T_MIXED			17	// Mixed with yellow and green, ugly color
#define D2T_HIGHLIGHT		18	// Highlight white
#define D2T_RED1			19	// Red
#define D2T_LIGHTYELLOW		20	// Light yellow


#define D2DRAW_MAX_TEXT		255 // Maximum text length that can be drawn in one element
#define D2DRAW_MAX_DATA		64  // Maximum elements can be drawn on the screen at a time

//////////////////////////////////////////////////////////////////////
// Drawing Element Data Definition
//////////////////////////////////////////////////////////////////////
typedef struct tagD2DrawData
{
	char szText[D2DRAW_MAX_TEXT + 1]; // Text to be drawn
	long x;							  // Screen coordinates x
	long y;							  // Screen coordinates y
	int nColor;						  // Text color
	int nFont;						  // Text font
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
	BOOL DrawSingleText(LPCSTR lpszText, long x, long y, int nColor, int nFont); // Draw one text
	BYTE DrawMultipleTexts(LPCD2DRAWDATA aData, BYTE iDataCount); // Draw multiple texts
	BOOL Undraw(); // Undraw, clean previously drawn texts

	//////////////////////////////////////////////////////////////////
	// Color Validate, Color > 20 will Crash the Game
	//////////////////////////////////////////////////////////////////
	static BOOL IsValidD2Color(int nColor); // Whether a color value is valid

protected:

	//////////////////////////////////////////////////////////////////
	// Protected Methods
	//////////////////////////////////////////////////////////////////
	static void Draw();	// Internal drawing function
	BOOL WriteMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize) const;
	BOOL PatchCall(DWORD dwAddress, LPVOID lpvFunction);
	
	//////////////////////////////////////////////////////////////////
	// Member data
	//////////////////////////////////////////////////////////////////
	BOOL m_bDrawn; // Is there any texts that were drawn by this object?
};

#endif // __D2TEXTDRAW_H__
