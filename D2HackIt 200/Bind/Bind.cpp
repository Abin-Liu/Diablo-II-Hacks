//////////////////////////////////////////////////////////////////////
// Bind.cpp
// -------------------------------------------------------------------
// The bind module used to bind keystrokes to commands.
//
// <thohell@home.se>
// Modified by Dan_Goon
// Rewritten by Abin.
//////////////////////////////////////////////////////////////////////
#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include "..\\Includes\\Utilities\\ini.h"
#include "BindEntry.h"
#include "..\\Includes\\Clientcore.cpp"

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
// -------------------------------------------------------------------
// You *need* this, don't ask why :)
//
// 0.01 2001-10-14 First version
// 0.02 2001-10-14 Added game-hook for keystrokes
// 0.03 2001-10-14 Commands commands commands...
// 0.04 2001-10-14 Fixed maphack compatibility problem
// 0.05 2001-10-14 Added Returncode to re-map existing keys
// 0.06 2001-10-14 Dont remember :)
// 1.00 2001-11-21 Small changes for D2Loader compatibility, matured
// 2.00 2001-12-09 ClckWcher - Multiple Commands Can now be binded to keys;Added "Add" Command
// 2.01 2001-12-09 ClckWcher - Speed Increase;Added Rotational Components
// 2.02 2001-12-25 ClckWcher - Add | translator
// 2.03 2002-02-13 bootyjuice - "Fixed" up so it works with D2Hackit 0.56+
//
// 3.00 2003-01-01 Dan_Goon : "Fixed" up so it works with D2Hackit 0.80+
//--------------------------------------------------------------------
// 4.00 2004-09-20 Abin - "Fixed" up so it works with D2Hackit 2.00+
// 5.00 2004-11-30 Abin - Complete rewrite for bug fixes.
//////////////////////////////////////////////////////////////////////

DWORD	ModuleVersion	= MAKELONG(5, 0);
char	ModuleAuthor[]	= "thohell + ClckWcher + bootyjuice + Dan_Goon + Abin";
char	ModuleWebsite[]	= "www.wxjindu.com/abin/d2hackit/index.htm";
char	ModuleDescription[] = "Bind module - Binds keystrokes to commands";
char	ModuleEmail[]	="abinn32@yahoo.com";

// Functions as they appear in the source...
BOOL PRIVATE OnGameCommandSet(char** argv, int argc);
BOOL PRIVATE OnGameCommandAdd(char** argv, int argc);
BOOL PRIVATE OnGameCommandRotate(char** argv, int argc);
BOOL PRIVATE OnGameCommandUnset(char** argv, int argc);
BOOL PRIVATE OnGameCommandKeycode(char** argv, int argc);
BOOL PRIVATE OnGameCommandList(char** argv, int argc);
BOOL PRIVATE OnGameCommandLoad(char** argv, int argc);
BOOL PRIVATE OnGameCommandSave(char** argv, int argc);
BOOL PRIVATE OnGameCommandClear(char** argv, int argc);

// Some global stuff
BOOL	ShowKeycode = FALSE;
BOOL	m_bSavedOnExit = FALSE;
CIni	g_ini;
char	g_szCurSection[256] = "Default";
CBindEntry g_aConfig[256];
const static char BLANKS[] = " \t\n\r";
const static char HEXES[] = "0123456789abcdefABCDEF";

void SaveAllKeys(LPCSTR lpszSection);
BOOL IsHexString(LPCSTR lpszString);
BOOL IsReturnCode(LPSTR lpszString, int nLen, BYTE& iReturnCode, BOOL& bRotate);

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{
		"set",
		OnGameCommandSet,
		"set <keycode> <returncode> [command1[;command2;...;commandN]]ÿc0 Binds the key with <keycode> to return <returncode> and perform [command..commandN]."
	},
	{
		"add",
		OnGameCommandAdd,
		"add <keycode> [command1[;command2;...;commandN]]ÿc0 Binds the key with <keycode> to return <returncode> and perform [command1..commandN]."
	},
	{
		"rotate",
		OnGameCommandRotate,
		"rotate <keycode>ÿc0 Toggles the weither or not commands rotate."
	},
	{
		"unset",
		OnGameCommandUnset,
		"unset <keycode>ÿc0 Removes the binding of key with <keycode>."
	},
	{
		"keycode",
		OnGameCommandKeycode,
		"keycodeÿc0 Shows the keycode of the next key you press."
	},
	{
		"list",
		OnGameCommandList,
		"listÿc0 Lists all currently assigned keys."
	},
	{	"load",
		OnGameCommandLoad,
		"load [section]ÿc0 Loads key-bindings in [section] from bind.ini."
	},
	{	"save",
		OnGameCommandSave,
		"save [section]ÿc0 Saves current key-bindings to <section> in bind.ini."
	},
	{	"clear",
		OnGameCommandClear,
		"clearÿc0 Removes all key-bindings."
	},
	{NULL}	// No more commands
};

//////////////////////////////////////////////////////////////////////
// OnClientStart
// -------------------------------------------------------------------
// Runs *once* before client is loaded. Return FALSE to prevent from
// loading.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{
	DWORD dwVer = LOWORD(server->GetD2HackitVersion());
	if (dwVer < 2)
	{
		server->GamePrintError("D2Hackit 2.00 or higher is required for bind!");
		return FALSE;
	}

	DGTRACE((DBGHDR "BIND : OnClientStart...\n"));

	char szPath[MAX_PATH + 1] = "";
	::sprintf(szPath, "%s\\bind.ini", server->GetModuleDirectory());
	g_ini.SetPathName(szPath);

	// Initiate list
	for (int i = 0; i < 256; i++)
		g_aConfig[i].Set((BYTE)i, (BYTE)i, NULL);

	char *argv[3];
	argv[0] = "";
	argv[1] = "load";
	argv[2] = "default";
	OnGameCommandLoad(argv, 3);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// OnClientStop
// -------------------------------------------------------------------
// Runs *once* before client is unloaded. Return value should be TRUE,
// but it is not used at this time.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStop()
{
	DGTRACE((DBGHDR "BIND : OnClientStop...\n"));

	if (m_bSavedOnExit)
		SaveAllKeys(g_szCurSection);

	DGTRACE((DBGHDR "BIND : Returning from OnClientStop()\n"));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameKeyDown
// -------------------------------------------------------------------
// Executes when a key is pressed.
//
// Return zero to prevent Diablo II from receiving this key press. Other
// modules will receive the key press regardless of the return values.
//////////////////////////////////////////////////////////////////////
BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	// Modified by Abin to make it work in D2hackit 2.00+
	if(ShowKeycode)
	{
		ShowKeycode = FALSE;
		char t[16];
		sprintf(t, "Keycode: %.2x", iKeyCode);
		server->GamePrintInfo(t);
		return 0;
	}

	if ((int)iKeyCode > 255)
		return iKeyCode;

	g_aConfig[(int)iKeyCode].InvokeCommands();
	return g_aConfig[(int)iKeyCode].GetReturnCode();
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandSet
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandSet(char** argv, int argc)
{
	// If command line is shorter than 4 tokens, something is wrong
	if(argc < 4)
	{
		server->GamePrintError("Syntax: .bind set <hex code> <hex return code> <command1;command2;...;commandx>");
		return TRUE;
	}
	
	char t[1024] = "";	// Should be enough...
	char* x = "";
	DWORD Keycode = (strtoul(argv[2], &x, 0x10)&0xff);
	if(strlen(x) || Keycode > 255)
	{
		sprintf(t, "'%s' is not a valid keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	DWORD Returncode = (strtoul(argv[3], &x, 0x10)&0xff);
	if(strlen(x) || Returncode > 255)
	{
		sprintf(t, "'%s' is not a valid returncode!", argv[3]);
		server->GamePrintError(t);
		return TRUE;
	}
	
	for (int i = 4; i < argc; i++)
	{
		::strcat(t, argv[i]);
		::strcat(t, " ");
	}

	g_aConfig[Keycode].Set(Keycode, Returncode, t);
	m_bSavedOnExit = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandAdd
// - ClckWcher
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandAdd(char** argv, int argc)
{
	// If command line is shorter than 4 tokens, something is wrong
	if(argc < 4)
	{
		server->GamePrintError("Syntax: .bind add <hex code> <command1;command2;...;commandx>");
		return TRUE;
	}
	
	char t[1024] = "";	// Should be enough...
	char*x = "";
	DWORD Keycode = (strtoul(argv[2], &x, 0x10)&0xff);
	if (strlen(x) || Keycode > 255)
	{
		sprintf(t, "'%s' is not a valid keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	for (int i = 3; i < argc; i++)
	{
		::strcat(t, argv[i]);
		::strcat(t, " ");
	}

	g_aConfig[Keycode].AddCommands(t);
	m_bSavedOnExit = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameCommandRotate
// - ClckWcher
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandRotate(char** argv, int argc)
{
	if (argc != 3)
	{
		server->GamePrintError("Syntax: .bind rotate <hex code>");
		return TRUE;
	}

	char t[1024] = "";	// Should be enough...
	char*x = "";
	DWORD Keycode = (strtoul(argv[2], &x, 0x10)&0xff);
	if (strlen(x) || Keycode > 255)
	{
		sprintf(t, "'%s' is not a valid keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	g_aConfig[Keycode].SetRotate(!g_aConfig[Keycode].IsRotate());
	m_bSavedOnExit = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandUnset
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandUnset(char** argv, int argc)
{
	// If command line is not 3 tokens, something is wrong
	if(argc != 3)
	{
		server->GamePrintError("Syntax: .bind unset <hex code>");
		return TRUE;
	}

	// Check for valid keycode
	char* x = "";
	DWORD Keycode = (strtoul(argv[2], &x, 0x10)&0xff);
	if(strlen(x) || Keycode > 255)
	{
		server->GameErrorf("'%s' is not a valid keycode!", argv[2]);
		return TRUE;
	}

	g_aConfig[Keycode].Clear();
	m_bSavedOnExit = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandKeydcode
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandKeycode(char** argv, int argc)
{
	// If command line is not 2 tokens, something is wrong
	if(argc != 2) return FALSE;

	server->GamePrintInfo("Press a key to get its keycode...");
	ShowKeycode = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandList
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandList(char** argv, int argc)
{
	// If command line is not 2 tokens, something is wrong
	if(argc != 2) return FALSE;
	
	server->GamePrintInfo("Keys currently mapped by bind:");
	// Loop through struct
	for(int i=0; i<256; i++)
	{
		// Found mapped key
		LPCSTR lpszCommands = g_aConfig[i].GetCommands();
		if (lpszCommands || g_aConfig[i].GetReturnCode() != i)
			server->GameInfof("%02X -> %02X (Rotate: %d): %s", g_aConfig[i].GetKeyCode(), g_aConfig[i].GetReturnCode(), g_aConfig[i].IsRotate(), lpszCommands ? lpszCommands : "(n/a)");
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandLoad
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandLoad(char** argv, int argc)
{
	// If command line is not 2 or 3 tokens, something is wrong
	if((argc != 3) && (argc != 2))
	{
		server->GamePrintError("Syntax: .bind load <section name>");
		return TRUE;
	}

	if (argc == 3)
		::strncpy(g_szCurSection, argv[2], 255);

	server->GameInfof("Loading key-mappings from section: ÿc4[%s]", g_szCurSection);
	
	char szKey[16] = "";
	char szValue[1024] = "";
	char szReturnCode[256] = "";

	for (int i = 0; i < 256; i++)
	{		
		BYTE iReturnCode = (BYTE)i;
		BOOL bRotate = FALSE;

		::sprintf(szKey, "Keycode%02X", i);	
		g_ini.GetString(g_szCurSection, szKey, szValue, 1023);
		const LPCSTR DELIMITOR = ::strchr(szValue, ';');
		
		if (DELIMITOR == NULL)
		{
			g_aConfig[i].Set((BYTE)i, iReturnCode, NULL);
		}
		else
		{
			const int RETLEN = min(255, (long)DELIMITOR - (long)szValue);
			::memcpy(szReturnCode, szValue, RETLEN);
			szReturnCode[RETLEN] = 0;			

			if (IsReturnCode(szReturnCode, RETLEN, iReturnCode, bRotate))
				g_aConfig[i].Set((BYTE)i, iReturnCode, DELIMITOR + 1);				
			else
				g_aConfig[i].Set((BYTE)i, iReturnCode, NULL);
		}

		g_aConfig[i].SetRotate(bRotate);
	}

	m_bSavedOnExit = FALSE;
	return TRUE;
}

void SaveAllKeys(LPCSTR lpszSection)
{
	char t[1024] = "";
	char sn[16];

	// Loop through struct
	for(int i=0; i<256; i++)
	{
		// Found mapped key
		sprintf(sn, "Keycode%02X", g_aConfig[i].GetKeyCode());
		LPCSTR lpszCommands = g_aConfig[i].GetCommands();
		if (lpszCommands)
			sprintf(t, "%02X%d;%s", g_aConfig[i].GetReturnCode(), g_aConfig[i].IsRotate(), lpszCommands);
		else
			sprintf(t, "%02X%d", g_aConfig[i].GetReturnCode(), g_aConfig[i].IsRotate());

		g_ini.WriteString(lpszSection, sn, t);
	}

	DGTRACE((DBGHDR "BIND : All Key Saved!\n"));
	m_bSavedOnExit = FALSE;
}


//////////////////////////////////////////////////////////////////////
// OnClientCommandSave
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandSave(char** argv, int argc)
{
	if (argc > 2)
	{
		g_szCurSection[0] = 0;
		for (int i = 2; i < argc; i++)
		{
			::strcat(g_szCurSection, argv[i]);
			::strcat(g_szCurSection, " ");
		}
	}
	
	server->GameInfof("Saving key-mappings to section: ÿc4[%s]", g_szCurSection);
	SaveAllKeys(g_szCurSection);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandClear
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandClear(char** argv, int argc)
{
	for(int i=0; i<256; i++)
		g_aConfig[i].Clear();

	server->GamePrintInfo("All key-bindings has been removed.");
	m_bSavedOnExit = TRUE;
	return TRUE;
}

BOOL IsHexString(LPCSTR lpszString)
{
	if (lpszString == NULL || *lpszString == 0)
		return FALSE;

	for (int i = 0; lpszString[i]; i++)
	{
		if (::strchr(HEXES, lpszString[i]) == NULL)
			return FALSE;
	}

	return TRUE;
}


BOOL IsReturnCode(LPSTR lpszString, int nLen, BYTE& iReturnCode, BOOL& bRotate)
{
	if (lpszString == NULL || *lpszString == 0)
		return FALSE;

	// trim right
	int i = 0;
	for (i = nLen - 1; i >= 0 && ::strchr(BLANKS, lpszString[i]); i++)
		lpszString[i] = 0;

	if (::strlen(lpszString) < 2)
		return FALSE;

	// verify return code
	if (!IsHexString(lpszString))
		return FALSE;

	char sz[3] = "";
	::memcpy(sz, lpszString, 2);
	iReturnCode = (BYTE)::strtoul(sz, NULL, 16);

	bRotate = (BOOL)::strtoul(lpszString + 2, NULL, 16);
	return TRUE;
}
