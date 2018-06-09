#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include <time.h>
#include "..\\Includes\\ClientCore.cpp"	  // Include the backend of the module

#define CLOCK_DARKSHADOW	0x1C
#define CLOCK_SHADOW		0xDC
#define CLOCK_DARKDARK		0xFA
#define CLOCK_HIGHLIGHT		0xDE

enum { SCHEME_LED = 0, SCHEME_GREEN, SCHEME_RED };

struct CLOCKSCHEME
{
	BYTE iBkColor;
	BYTE iOnColor;
	BYTE iOffColor;
};

const SIZE CLOCKSIZE = { 100, 30 };
const SIZE SMALL_DIGIT = { 14, 17 };
const SIZE BIG_DIGIT = { 27, 34 };
const SIZE SMALL_COLON = { 8, 17 };
const SIZE BIG_COLON = { 12, 34 };

const CLOCKSCHEME LED = { 0x0E, 0x00, 0x1A };
const CLOCKSCHEME GREEN = { 0x00, 0x84, 0x73 };
const CLOCKSCHEME RED = { 0x00, 0x62, 0x31 };
CLOCKSCHEME g_scheme = GREEN;

char g_szTimeString[32] = "";
BOOL g_bBig = 0;
BOOL g_bColonOn = TRUE;
BOOL g_bShow = TRUE;
char g_szIniPath[MAX_PATH + 1] = "";

class CLineDraw
{
public:

	// Constructor
	CLineDraw()
	{
		m_pDC = NULL;
		::memset(&m_ptInit, 0, sizeof(POINT));
		m_x = 0;
		m_y = 0;
	}

	void Reset(CGameDC* pDC, POINT ptInit)
	{
		m_pDC = pDC;
		m_ptInit = ptInit;
		m_x = 0;
		m_y = 0;
	}

	// Set the point the top-lect corner of the rect, offset the point,
	// then move dc to that point
	void InitOffsetMoveTo(long x, long y)
	{
		m_x = m_ptInit.x + x;
		m_y = m_ptInit.y + y;
		m_pDC->MoveTo(m_x, m_y);
	}

	// Offset the point and move dc to the point
	void OffsetMoveTo(long x, long y)
	{
		m_x += x;
		m_y += y;
		m_pDC->MoveTo(m_x, m_y);
	}

	// Offset the point and draw a line to the point, then set pixel at the point
	// using the color of current pen stored in the dc
	void OffsetLineTo(long x, long y)
	{
		m_x += x;
		m_y += y;
		POINT pt = m_pDC->GetCurrentPosition();		
		m_pDC->LineTo(m_x, m_y);
		m_pDC->LineTo(pt.x, pt.y);
		m_pDC->MoveTo(m_x, m_y);
	}

private:
	CGameDC* m_pDC;
	long m_x;
	long m_y;
	POINT m_ptInit;
};

CLineDraw g_ld;

void CalculatePaneRect(LPCRECT lpScreenRect, LPRECT lpRect);
void DrawPane(CGameDC* pDC, LPCRECT lpRect);
int DrawDigit(CGameDC* pDC, char ch, const POINT& pos, BOOL bOn);
int DrawColon(CGameDC* pDC, const POINT& pos, BOOL bOn);

BOOL PRIVATE OnCommandBig(char** argv, int argc);
BOOL PRIVATE OnCommandScheme(char** argv, int argc);
BOOL PRIVATE OnCommandShow(char** argv, int argc);

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
// -------------------------------------------------------------------
// Tell us a little about this module.
//////////////////////////////////////////////////////////////////////
CLIENTINFO
(		
	1,2,							// Module version (Major, Minor)
	"Abin",						// Author
	"www.wxjindu.com/abin/",		// url (http:// is appended)
	"Clock",				// Short module description
	"abinn32@yahoo.com"				// Author eMail
)

//////////////////////////////////////////////////////////////////////
// MODULECOMMANDSTRUCT ModuleCommands[]
// -------------------------------------------------------------------
// To add your own commands, just add them to this list like:
//
//	{
//		"name"			// Name of the command
//		pFunction		// The function that handles command
//		"usage"			// Help text for the command
//	}
// 
// ...where pFunction is the name of the function that handles the
// command. The function must be declared as...
//
// BOOL PRIVATE OnGameCommandName(char** argv, int argc)
//
// ...and return TRUE on success or FALSE to show usage of the command.
//
// The "usage" line can contain "\n" as linebreaks and should be in 
// the following format for consistency:
//
// "commandname <required parameter> [optional parameter]ÿc0 Description"
//
//
// To use your commands in the game, just type 
// .modulename <command> [arguments]
//////////////////////////////////////////////////////////////////////

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	// Add your own commands here
	{
		"big",
		OnCommandBig,
		".clock big <0/1>"
	},

	{
		"show",
		OnCommandShow,
		".clock show <0/1>"
	},

	{
		"scheme",
		OnCommandScheme,
		".clock scheme <LED/green/red>"
	},

	{NULL}	// No more commands
};

//////////////////////////////////////////////////////////////////////
// OnClientStart
// -------------------------------------------------------------------
// Runs *once* when the module loads. 
// This function can be removed.
//
// Put any initialization code here.
//
// Return FALSE to prevent module from loading.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{
	::sprintf(g_szIniPath, "%s\\D2Clock.ini", server->GetModuleDirectory());

	g_bBig = ::GetPrivateProfileInt("Default", "Big", 0, g_szIniPath);	
	int nScheme = ::GetPrivateProfileInt("Default", "Scheme", SCHEME_GREEN, g_szIniPath);
	if (nScheme == SCHEME_LED)
	{
		g_scheme = LED;
	}
	else if (nScheme == SCHEME_RED)
	{
		g_scheme = RED;
	}
	else
	{
		g_scheme = GREEN;
	}


	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStop
// -------------------------------------------------------------------
// Runs *once* before client is unloaded. 
// This function can be removed.
// 
// Put any cleanup code here.
//
// Return value should be TRUE.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStop()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnDraw - Added since v2.00 by Abin
// -------------------------------------------------------------------
// Executes when the game screen needs to be drawn.
//
// Parameters:
// 
// pDC - Pointer to a game device context structure, use member functions
//       of this structure to draw your own items, for example:
//       pDC->MoveTo(0, 0); pDC->LineTo(400, 300, 0x62);
//       will draw a red line from (0,0) to (400,300).
// lpScreenRect - Pointer to the client screen rect boundary.
//
// Check D2Hackit API document for more details.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnDraw(CGameDC* pDC, LPCRECT lpScreenRect)
{
	// TODO: Draw your own texts/graphs on the screen.
	if (!g_bShow)
		return;

	RECT rect;
	CalculatePaneRect(lpScreenRect, &rect);
	DrawPane(pDC, &rect);	
	
	POINT pt = { rect.left + 5, rect.top + 5 };
	for (int i = 0; i < 8; i++)
	{
		int nWidth = 0;
		if (g_szTimeString[i] == ':')
		{
			nWidth = DrawColon(pDC, pt, g_bColonOn);
		}
		else
		{
			nWidth = DrawDigit(pDC, g_szTimeString[i], pt, TRUE);
		}

		pt.x += nWidth;
	}
}

//////////////////////////////////////////////////////////////////////
// OnGameJoin
// -------------------------------------------------------------------
// Executes when joining a game, and if loading the module while 
// inside a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	return;
}

//////////////////////////////////////////////////////////////////////
// OnGameLeave
// -------------------------------------------------------------------
// Executes when leaving a game.
//
// Refer to the documentation for the format of THISGAMESTRUCT.
//////////////////////////////////////////////////////////////////////
VOID EXPORT OnGameLeave(THISGAMESTRUCT* thisgame)
{
	return;
}

//////////////////////////////////////////////////////////////////////
// OnGameTimerTick
// -------------------------------------------------------------------
// This gets executed apx. every 1/10th of a second when in a game.
//
// You can use this to create custom timers.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGameTimerTick(void)
{
	if ((server->GetInGameTime() % 500))
		return 0;

	tm *when;
    time_t now;
    ::time(&now);
    when = ::localtime(&now);
	::strftime(g_szTimeString, 9, "%H:%M:%S", when);
	g_bColonOn = !g_bColonOn;
	return 0;
}

void CalculatePaneRect(LPCRECT lpScreenRect, LPRECT lpRect)
{
	if (lpScreenRect == NULL || lpScreenRect == NULL)
		return;

	int nHeight = g_bBig ? BIG_DIGIT.cy : SMALL_DIGIT.cy;
	nHeight += 10;
	// dd:dd:dd
	int nWidth = g_bBig ? BIG_DIGIT.cx : SMALL_DIGIT.cx;
	nWidth *= 6;
	nWidth += g_bBig ? BIG_COLON.cx * 2 : SMALL_COLON.cx * 2;
	nWidth += 8;
	lpRect->left = lpScreenRect->left + 115;
	lpRect->top = lpScreenRect->bottom - nHeight - 52;
	lpRect->right = lpRect->left + nWidth;
	lpRect->bottom = lpRect->top + nHeight;
}

void DrawPane(CGameDC* pDC, LPCRECT lpRect)
{
	if (pDC == NULL || lpRect == NULL)
		return;

	// fill pane
	pDC->SetBrushColor(g_scheme.iBkColor);
	pDC->SetBrushTransparency(2);
	pDC->FillRect(lpRect);

	// draw 3D edges
	pDC->MoveTo(lpRect->left, lpRect->bottom - 1);
	pDC->SetPenColor(CLOCK_DARKSHADOW);	
	pDC->LineTo(lpRect->left, lpRect->top);
	pDC->LineTo(lpRect->right, lpRect->top);
	
	pDC->SetPenColor(CLOCK_HIGHLIGHT);
	pDC->LineTo(lpRect->right, lpRect->bottom);
	pDC->LineTo(lpRect->left, lpRect->bottom);

	pDC->MoveTo(lpRect->left + 1, lpRect->bottom - 2);
	pDC->SetPenColor(CLOCK_DARKDARK);	
	pDC->LineTo(lpRect->left + 1, lpRect->top + 1);
	pDC->LineTo(lpRect->right - 1, lpRect->top + 1);
	
	pDC->SetPenColor(CLOCK_SHADOW);
	pDC->LineTo(lpRect->right - 1, lpRect->bottom - 1);
	pDC->LineTo(lpRect->left + 1, lpRect->bottom - 1);
}

int DrawDigit(CGameDC* pDC, char ch, const POINT& pos, BOOL bOn)
{
	if (pDC == NULL)
		return 0;

	g_ld.Reset(pDC, pos);
	int i = 0, n = 0;

	// top -
	if (bOn && ch != '1' && ch != '4')
		pDC->SetPenColor(g_scheme.iOnColor);
	else
		pDC->SetPenColor(g_scheme.iOffColor);

	if (g_bBig)
	{
		n = 21;
		g_ld.InitOffsetMoveTo(1, 0);
		for (i = 0; i < 6; i++)
		{
			g_ld.OffsetLineTo(n--, 0);
			g_ld.OffsetMoveTo(-(n--), 1);
		}
	}
	else
	{
		g_ld.InitOffsetMoveTo(1, 0);
		g_ld.OffsetLineTo(9, 0);
		g_ld.InitOffsetMoveTo(2, 1);
		g_ld.OffsetLineTo(7, 0);
		g_ld.InitOffsetMoveTo(3, 2);
		g_ld.OffsetLineTo(5, 0);
	}

	// middle -
	if (bOn && ch != '0' && ch != '1' && ch != '7')
		pDC->SetPenColor(g_scheme.iOnColor);
	else
		pDC->SetPenColor(g_scheme.iOffColor);
	
	if (g_bBig)
	{
		n = 11;
		g_ld.InitOffsetMoveTo(6, 14);
		for (i = 0; i < 3; i++)
		{
			g_ld.OffsetLineTo(n++, 0);
			g_ld.OffsetMoveTo(-(n++), 1);
		}

		n = 15;
		g_ld.InitOffsetMoveTo(4, 17);
		for (i = 0; i < 3; i++)
		{
			g_ld.OffsetLineTo(n--, 0);
			g_ld.OffsetMoveTo(-(n--), 1);
		}
	}
	else
	{
		g_ld.InitOffsetMoveTo(3, 7);
		g_ld.OffsetLineTo(5, 0);
		g_ld.InitOffsetMoveTo(2, 8);
		g_ld.OffsetLineTo(7, 0);
		g_ld.InitOffsetMoveTo(3, 9);
		g_ld.OffsetLineTo(5, 0);
	}

	// bottom -
	if (bOn && ch !='1' && ch != '4' && ch != '7')
		pDC->SetPenColor(g_scheme.iOnColor);
	else
		pDC->SetPenColor(g_scheme.iOffColor);

	if (g_bBig)
	{
		n = 21;
		g_ld.InitOffsetMoveTo(1, 33);
		for (i = 0; i < 6; i++)
		{
			g_ld.OffsetLineTo(n--, 0);
			g_ld.OffsetMoveTo(-(n--), -1);
		}
	}
	else
	{
		g_ld.InitOffsetMoveTo(1, 16);
		g_ld.OffsetLineTo(9, 0);
		g_ld.InitOffsetMoveTo(2, 15);
		g_ld.OffsetLineTo(7, 0);
		g_ld.InitOffsetMoveTo(3, 14);
		g_ld.OffsetLineTo(5, 0);
	}

	// top left |
	if (bOn && ch != '1' && ch != '2' && ch != '3' && ch != '7')
		pDC->SetPenColor(g_scheme.iOnColor);
	else
		pDC->SetPenColor(g_scheme.iOffColor);

	if (g_bBig)
	{
		n = 14;
		g_ld.InitOffsetMoveTo(0, 1);
		for (i = 0; i < 4; i++)
		{
			g_ld.OffsetLineTo(0, n--);
			g_ld.OffsetMoveTo(1, -n);
		}

		n = 9;
		g_ld.InitOffsetMoveTo(4, 5);
		for (i = 0; i < 2; i++)
		{
			g_ld.OffsetLineTo(0, n--);
			g_ld.OffsetMoveTo(1, -(n--));
		}
	}
	else
	{
		g_ld.InitOffsetMoveTo(0, 1);
		g_ld.OffsetLineTo(0, 6);
		g_ld.InitOffsetMoveTo(1, 2);
		g_ld.OffsetLineTo(0, 5);
		g_ld.InitOffsetMoveTo(2, 3);
		g_ld.OffsetLineTo(0, 3);
	}

	// bottom left |
	if (bOn && ch != '1' && ch != '3' && ch != '4' && ch != '5' && ch != '7' && ch != '9')
		pDC->SetPenColor(g_scheme.iOnColor);
	else
		pDC->SetPenColor(g_scheme.iOffColor);
	
	if (g_bBig)
	{
		n = 14;
		g_ld.InitOffsetMoveTo(0, 18);
		for (i = 0; i < 4; i++)
		{
			g_ld.OffsetLineTo(0, n);
			g_ld.OffsetMoveTo(1, -(n--));
		}

		n = 9;
		g_ld.InitOffsetMoveTo(4, 19);
		for (i = 0; i < 2; i++)
		{
			g_ld.OffsetLineTo(0, n--);
			g_ld.OffsetMoveTo(1, -(n--));
		}
	}
	else
	{
		g_ld.InitOffsetMoveTo(0, 15);
		g_ld.OffsetLineTo(0, -6);
		g_ld.InitOffsetMoveTo(1, 14);
		g_ld.OffsetLineTo(0, -5);
		g_ld.InitOffsetMoveTo(2, 13);
		g_ld.OffsetLineTo(0, -3);
	}

	// top right |
	if (bOn && ch != '5' && ch != '6')
		pDC->SetPenColor(g_scheme.iOnColor);
	else
		pDC->SetPenColor(g_scheme.iOffColor);

	if (g_bBig)
	{
		n = 14;
		g_ld.InitOffsetMoveTo(23, 1);
		for (i = 0; i < 4; i++)
		{
			g_ld.OffsetLineTo(0, n--);
			g_ld.OffsetMoveTo(-1, -n);
		}

		n = 9;
		g_ld.InitOffsetMoveTo(19, 5);
		for (i = 0; i < 2; i++)
		{
			g_ld.OffsetLineTo(0, n--);
			g_ld.OffsetMoveTo(-1, -(n--));
		}
	}
	else
	{
		g_ld.InitOffsetMoveTo(11, 1);
		g_ld.OffsetLineTo(0, 6);
		g_ld.InitOffsetMoveTo(10, 2);
		g_ld.OffsetLineTo(0, 5);
		g_ld.InitOffsetMoveTo(9, 3);
		g_ld.OffsetLineTo(0, 3);
	}

	// bottom right |
	if (bOn && ch != '2')
		pDC->SetPenColor(g_scheme.iOnColor);
	else
		pDC->SetPenColor(g_scheme.iOffColor);

	if (g_bBig)
	{
		n = 14;
		g_ld.InitOffsetMoveTo(23, 18);
		for (i = 0; i < 4; i++)
		{
			g_ld.OffsetLineTo(0, n);
			g_ld.OffsetMoveTo(-1, -(n--));
		}

		n = 9;
		g_ld.InitOffsetMoveTo(19, 19);
		for (i = 0; i < 2; i++)
		{
			g_ld.OffsetLineTo(0, n--);
			g_ld.OffsetMoveTo(-1, -(n--));
		}
	}
	else
	{
		g_ld.InitOffsetMoveTo(11, 15);
		g_ld.OffsetLineTo(0, -6);
		g_ld.InitOffsetMoveTo(10, 14);
		g_ld.OffsetLineTo(0, -5);
		g_ld.InitOffsetMoveTo(9, 13);
		g_ld.OffsetLineTo(0, -3);
	}
	
	return g_bBig ? BIG_DIGIT.cx : SMALL_DIGIT.cx;
}

int DrawColon(CGameDC* pDC, const POINT& pos, BOOL bOn)
{
	if (pDC == NULL)
		return 0;

	g_ld.Reset(pDC, pos);
	pDC->SetPenColor(bOn ? g_scheme.iOnColor : g_scheme.iOffColor);
	if (g_bBig)
	{
		g_ld.InitOffsetMoveTo(3, 8);
		g_ld.OffsetLineTo(2, 0);
		g_ld.OffsetMoveTo(-2, 1);
		g_ld.OffsetLineTo(2, 0);
		g_ld.OffsetMoveTo(-2, 1);
		g_ld.OffsetLineTo(2, 0);

		g_ld.InitOffsetMoveTo(3, 24);
		g_ld.OffsetLineTo(2, 0);
		g_ld.OffsetMoveTo(-2, 1);
		g_ld.OffsetLineTo(2, 0);
		g_ld.OffsetMoveTo(-2, 1);
		g_ld.OffsetLineTo(2, 0);
	}
	else
	{
		g_ld.InitOffsetMoveTo(2, 4);
		g_ld.OffsetLineTo(1, 0);
		g_ld.OffsetMoveTo(-1, 1);
		g_ld.OffsetLineTo(1, 0);

		g_ld.InitOffsetMoveTo(2, 11);
		g_ld.OffsetLineTo(1, 0);
		g_ld.OffsetMoveTo(-1, 1);
		g_ld.OffsetLineTo(1, 0);
	}

	return g_bBig ? BIG_COLON.cx : SMALL_COLON.cx;
}

BOOL PRIVATE OnCommandBig(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: clock big <0/1>");
		return TRUE;
	}

	g_bBig = ::atoi(argv[2]);
	::WritePrivateProfileString("Default", "Big", g_bBig ? "1" : "0", g_szIniPath);
	return TRUE;
}

BOOL PRIVATE OnCommandScheme(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: clock scheme <LED/green/red>");
		return TRUE;
	}

	int nScheme = -1;
	if (!::stricmp(argv[2], "LED"))
	{
		g_scheme = LED;
		nScheme = SCHEME_LED;
	}
	else if (!::stricmp(argv[2], "green"))
	{
		g_scheme = GREEN;
		nScheme = SCHEME_GREEN;
	}
	else if (!::stricmp(argv[2], "red"))
	{
		g_scheme = RED;
		nScheme = SCHEME_RED;
	}
	else
	{
		server->GamePrintError("Syntax: clock scheme <LED/green/red>");
		return TRUE;
	}

	char szScheme[32] = "";
	::sprintf(szScheme, "%d", nScheme);
	::WritePrivateProfileString("Default", "Scheme", szScheme, g_szIniPath);
	return TRUE;
}

BOOL PRIVATE OnCommandShow(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: clock show <0/1>");
		return TRUE;
	}

	g_bShow = ::atoi(argv[2]);
	return TRUE;
}