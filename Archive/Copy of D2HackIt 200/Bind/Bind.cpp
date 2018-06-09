//////////////////////////////////////////////////////////////////////
// Bind.cpp
// -------------------------------------------------------------------
// The bind module used to bind keystrokes to commands.
//
// <thohell@home.se>
// Modified by Dan_Goon
//////////////////////////////////////////////////////////////////////
#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include "..\\Includes\\ClientCore.cpp"

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
// 4.00 2004-09-20 Abin - "Fixed" up so it works with D2Hackit 2.00+
//////////////////////////////////////////////////////////////////////
DWORD	ModuleVersion	= MAKELONG(4, 0);
char	ModuleAuthor[]	= "thohell + ClckWcher + bootyjuice + Dan_Goon + Abin";
char	ModuleWebsite[]	= "www.wxjindu.com/abin/";
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
void GameKeyDownSTUB(); //0x6FACEF99
DWORD __fastcall GameKeyDown(DWORD aKeyCode);

// Some global stuff
//FINGERPRINTSTRUCT fps;
BOOL	ShowKeycode = FALSE;
char	CurrentSection[64];
BOOL	m_bSavedOnExit = FALSE;

void SaveAllKeys(BOOL bOnExit = FALSE);

typedef struct CMD_LIST		CMD_LIST, *PCMD_LIST;
typedef struct KEY_DEF		KEY_DEF, *PKEY_DEF;

// Basic Command List Structure Definition
struct CMD_LIST
{
	CMD_LIST() { pCmd = NULL; pNext = NULL; }
	~CMD_LIST() { DeleteList(); }

	void DeleteList();
	LPSTR SetCommand(LPSTR pNewCmd);
	PCMD_LIST NewList();

	LPSTR		pCmd;
	PCMD_LIST	pNext;
};

void CMD_LIST::DeleteList()
{
	if(pCmd) delete [] (LPSTR)pCmd;
	pCmd = NULL;
	if(pNext) delete pNext;
	pNext = NULL;
}

LPSTR CMD_LIST::SetCommand(LPSTR pNewCmd)
{
	if(pCmd) delete [] pCmd;
	pCmd = new char[strlen(pNewCmd) + 1];
	if(pCmd) strcpy(pCmd, pNewCmd);
	return pCmd;
}

PCMD_LIST CMD_LIST::NewList()
{
	if(pNext) delete pNext;
	pNext = new CMD_LIST;
	return pNext;
}

// Command List Derived Key Structure Definition
struct KEY_DEF : public CMD_LIST
{
	KEY_DEF() { bRotational = FALSE; nReturncode = 0; }
	~KEY_DEF();

	LPCSTR GetAllCmds(LPSTR str);
	PCMD_LIST AddTailList() const;
	void AddCommands(LPSTR* argv, int argc, BOOL bAppend = TRUE);
	LPSTR RotateCommand();

	operator PCMD_LIST() const { return (PCMD_LIST)this; }

	BOOL	bRotational;
	DWORD	nReturncode;
};


KEY_DEF::~KEY_DEF()
{
	SaveAllKeys(TRUE);
}

LPCSTR KEY_DEF::GetAllCmds(LPSTR str)
{
	LPCSTR cmd = pCmd;
	while(cmd)
	{
		strcat(str, " ; ");
		strcat(str, cmd);
		cmd = pNext ? pNext->pCmd : NULL;
	}

	return str;
}

PCMD_LIST KEY_DEF::AddTailList() const
{
	PCMD_LIST pList = (PCMD_LIST)this;
	while(pList->pCmd && pList->pNext)
	{
		pList = pList->pNext;
	}

	if(!pList->pCmd) return pList;
	return pList->NewList();
}


void KEY_DEF::AddCommands(LPSTR* argv, int argc, BOOL bAppend)
{
	PCMD_LIST pList = bAppend ? AddTailList() : (PCMD_LIST)this;
	bAppend = FALSE;

	LPSTR pSp;
	char cbf[1024];
	cbf[0] = 0;

	for(int i=0; i<argc; i++)
	{
		LPSTR pArg = argv[i];

		pSp = strchr(pArg, '`');
		if(pSp) *pSp = '|';		// Replace '`' with '|'
again:
		if(!*pArg) continue;

		pSp = strchr(pArg, ';');
		if(!pSp)
		{
			// if no command separators in this arg
			strcat(cbf, pArg);
			strcat(cbf, " ");
			continue;
		}

		// if command separator found
		if(pSp != pArg)
		{
			*pSp = 0;
			strcat(cbf, pArg);
			*pSp = ';';
		}

		if(cbf[0])
		{
			if(bAppend) pList = pList->NewList();
			pList->SetCommand(cbf);
			bAppend = TRUE;

			cbf[0] = 0;
			pArg = ++pSp;
			goto again;
		}
	}

	if(cbf[0])
	{
		if(bAppend) pList = pList->NewList();
		pList->SetCommand(cbf);
	}
}


LPSTR KEY_DEF::RotateCommand()
{
	if(bRotational)
	{
		LPSTR pC = pCmd;

		PCMD_LIST pList = (PCMD_LIST)this;
		while(pList->pNext && pList->pNext->pCmd)
		{
			pList->pCmd = pList->pNext->pCmd;
			pList = pList->pNext;
		}

		pList->pCmd = pC;
	}

	return pCmd;
}

KEY_DEF g_Keys[256];
BOOL Loaded = FALSE;

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
// Functions should use following convention...
// BOOL PRIVATE OnGameCommandName(char** argv, int argc)
// ...and return TRUE on success or FALSE to show usage of the command.
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
	DGTRACE((DBGHDR "BIND : OnClientStart...\n"));

	// Find location to intercept.
	//if(!server->GetFingerprint("D2HackIt", "GameKeyDownIntercept", fps)) return FALSE;

	// Initiate list
	char *argv[3];
	argv[0] = "";
	argv[1] = "load";
	argv[2] = "default";
	OnGameCommandLoad(argv, 3);
		
	sprintf(CurrentSection, "Default");

	//DGTRACE((DBGHDR "KeyDown Patch : Addr = %p, Size = %d\n", fps.AddressFound, fps.PatchSize));
	//PBYTE pRel = (PBYTE)((DWORD)&GameKeyDownSTUB - (fps.AddressFound + 5));
	//PBYTE* pMem = (PBYTE*)(fps.AddressFound + 1);


	// Patch code
	//server->Intercept(INST_CALL, fps.AddressFound, (DWORD)&GameKeyDownSTUB, fps.PatchSize);
	//DGTRACE((DBGHDR "(%p) == (%p) ?\n", pRel, *pMem));
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

	// Unpatch code
	//server->Intercept(INST_CALL, (DWORD)&GameKeyDownSTUB, fps.AddressFound, fps.PatchSize);

	SaveAllKeys(TRUE);

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
	
	if(g_Keys[iKeyCode].pCmd)
	{
		if(g_Keys[iKeyCode].bRotational)
		{
			server->GameCommandLine(g_Keys[iKeyCode].RotateCommand());
		}
		else
		{
			PCMD_LIST pList = g_Keys[iKeyCode];
			while(pList && pList->pCmd)
			{
				server->GameCommandLine(pList->pCmd);
				pList = pList->pNext;
			}
		}
	}

	return g_Keys[iKeyCode].nReturncode;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandSet
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandSet(char** argv, int argc)
{
	// If command line is shorter than 4 tokens, something is wrong
	if(argc < 4) return FALSE;
	
	char t[1024];	// Should be enough...
	char* x = "";
	DWORD Keycode = (strtoul(argv[2], &x, 0x10)&0xff);
	if(strlen(x))
	{
		sprintf(t, "'%s' is not a valid keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	DWORD Returncode = (strtoul(argv[3], &x, 0x10)&0xff);
	if(strlen(x))
	{
		sprintf(t, "'%s' is not a valid returncode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	g_Keys[Keycode].nReturncode = Returncode;
	g_Keys[Keycode].bRotational = FALSE;

	if(argc > 4) g_Keys[Keycode].AddCommands(&argv[4], argc - 4, FALSE);

	sprintf(t, "Binding %.2x -> %.2x ", Keycode, Returncode);
	if(g_Keys[Keycode].pCmd)
	{
		strcat(t, "Action: ");
		g_Keys[Keycode].GetAllCmds(t);
	}

	server->GamePrintVerbose(t);
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
	if(argc < 4) return FALSE;
	
	char t[1024];	// Should be enough...
	char*x = "";
	DWORD Keycode = (strtoul(argv[2], &x, 0x10)&0xff);
	if (strlen(x))
	{
		sprintf(t, "'%s' is not a valid keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}
	
	if(g_Keys[Keycode].pCmd == NULL)
	{
		sprintf(t, "'%s' is not an active keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	g_Keys[Keycode].AddCommands(&argv[3], argc - 3);

	sprintf(t, "Adding to %.2x", Keycode);
	if(g_Keys[Keycode].pCmd)
	{
		strcat(t, " Action: ");
		g_Keys[Keycode].GetAllCmds(t);
	}

	server->GamePrintVerbose(t);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandRotate
// - ClckWcher
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandRotate(char** argv, int argc)
{
	// If command line does not equal 3 tokens than something is wrong
	if(argc != 3) return FALSE;
	
	char t[128];	// Should be enough...
	char *x = "";

	DWORD Keycode = (strtoul(argv[2], &x, 0x10)&0xff);
	if(strlen(x))
	{
		sprintf(t, "'%s' is not a valid keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}
	
	if(g_Keys[Keycode].pCmd == NULL)
	{
		sprintf(t, "'%s' is not an active keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	if(g_Keys[Keycode].pNext == NULL)
	{
		sprintf(t, "'%s' you have to have more than one command to rotate!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	if(g_Keys[Keycode].bRotational)
	{
		g_Keys[Keycode].bRotational = FALSE;
		sprintf(t, "%.2X Rotate off", Keycode);
	}
	else
	{
		g_Keys[Keycode].bRotational = TRUE;
		sprintf(t, "%.2X Rotate on", Keycode);
	}

	server->GamePrintVerbose(t);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
// OnClientCommandUnset
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandUnset(char** argv, int argc)
{
	// If command line is not 3 tokens, something is wrong
	if(argc != 3) return FALSE;

	char t[128];

	// Check for valid keycode
	char* x = "";
	DWORD Keycode = (strtoul(argv[2], &x, 0x10)&0xff);
	if(strlen(x))
	{
		sprintf(t, "'%s' is not a valid keycode!", argv[2]);
		server->GamePrintError(t);
		return TRUE;
	}

	// Restore returncode
	g_Keys[Keycode].nReturncode = Keycode;

	// Remove command, if any
	g_Keys[Keycode].DeleteList();

	sprintf(t, "Keycode '%.2x' restored.", Keycode);
	server->GamePrintVerbose(t);

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
		if((g_Keys[i].nReturncode != i) || (g_Keys[i].pCmd))
		{
			char t[1024];

			sprintf(t, "%.2x -> %.2x", i, g_Keys[i].nReturncode);
			if(g_Keys[i].bRotational) strcat(t, " Rotate");

			if(g_Keys[i].pCmd)
			{
				strcat(t, " Action: ");
				g_Keys[i].GetAllCmds(t);
			}
			server->GamePrintInfo(t);
		}
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
	if((argc != 3) && (argc != 2)) return FALSE;

	char t[1024];
	char retval[256];

	if(argc == 3) strcpy(CurrentSection, argv[2]);

	sprintf(t, "Loading key-mappings from section: ÿc4[%s]", CurrentSection);
	server->GamePrintInfo(t);
	
	for(int i=0; i<256; i++)
	{
		g_Keys[i].nReturncode = i;
		g_Keys[i].bRotational = FALSE;
		
		sprintf(t, "Keycode%.2x", i);
		// Modified by Dan_Goon
		retval[0] = 0;
		server->GetIniString("bind", CurrentSection, t, retval, sizeof(retval));
		
		// If we found something, process it
		if(!retval[0]) continue;

		char *x = "";
		char Returncode[3];
		memcpy(Returncode, retval, 2);
		Returncode[2] = NULL;
		g_Keys[i].nReturncode = (strtoul(Returncode, &x, 0x10)&0xff);

		if(strlen(retval) > 3)
		{
			if(retval[2] == '0') g_Keys[i].bRotational = FALSE;
			else g_Keys[i].bRotational = TRUE;
		}

		// If there is a command-string, save it
		if(strlen(retval) > 3)
		{
			PCMD_LIST List = NULL;

			char *pSp = &retval[3];

			BOOL bEOL = FALSE;
			while(pSp < retval + strlen(retval))
			{
				while(*pSp == ' ' || *pSp == ';' || *pSp == '\t') pSp++;
				if(*pSp < ' ') break;

				char *Command = pSp++;

				while(*pSp >= ' ' && *pSp != ';') pSp++;
				if(*pSp < ' ' && *pSp != '\t') bEOL = TRUE;
				*pSp++ = 0;

				DGTRACE((DBGHDR "BIND : Load Key Command = [%s]...\n", Command));

				List = List ? List->NewList() : g_Keys[i];
				List->SetCommand(Command);

				if(bEOL) break;
			}
		}

		if((g_Keys[i].nReturncode != i) || (g_Keys[i].pCmd != NULL))
		{
			sprintf(t, "%.2X->%.2X", i, g_Keys[i].nReturncode);
			if(g_Keys[i].bRotational) strcat(t, " Rotate");

			if(g_Keys[i].pCmd)
			{
				strcat(t, " Action ");
				g_Keys[i].GetAllCmds(t);
			}

			server->GamePrintVerbose(t);
		}
	}

	return TRUE;
}


void SaveAllKeys(BOOL bOnExit)
{
	if(bOnExit)
	{
		if(m_bSavedOnExit) return;
		m_bSavedOnExit = TRUE;

		// This is for old D2HackIt safety!
		DGTRACE((DBGHDR "BIND : SaveAllKeys on Exit Process!\n"));
	}

	char t[1024];
	char sn[16];

	// Loop through struct
	for(int i=0; i<256; i++)
	{
		// Found mapped key
		sprintf(sn, "Keycode%.2X", i);
		sprintf(t, "%.2X%d", g_Keys[i].nReturncode, g_Keys[i].bRotational);

		g_Keys[i].GetAllCmds(t);

		if(g_Keys[i].pCmd)
		{
			DGTRACE((DBGHDR "BIND : Save [%s]:[%s]...\n", sn, t));
		}
		
		server->SetHackProfileString("bind", CurrentSection, sn, t);
	}

	DGTRACE((DBGHDR "BIND : All Key Saved!\n"));
}


//////////////////////////////////////////////////////////////////////
// OnClientCommandSave
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandSave(char** argv, int argc)
{
	// If command line is not 2 or 3 tokens, something is wrong
	if((argc != 3) && (argc != 2)) return FALSE;

	if(argc == 3) strcpy(CurrentSection, argv[2]);

	char t[128];	// Should be enough...
	sprintf(t, "Saving key-mappings to section: ÿc4[%s]", CurrentSection);
	server->GamePrintInfo(t);

	SaveAllKeys();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientCommandClear
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandClear(char** argv, int argc)
{
	// If command line is not 2 tokens, something is wrong
	if(argc != 2) return FALSE;
	
	for(int i=0; i<256; i++)
	{
		g_Keys[i].nReturncode = i;
		g_Keys[i].bRotational = FALSE;
		g_Keys[i].DeleteList();
	}

	server->GamePrintInfo("All key-bindings has been removed.");

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// GameKeyDownSTUB()
// --------------------------------------------------------------------------
// esi = Keycode
/////////////////////////////////////////////////////////////////////////////
/*
void __declspec(naked) GameKeyDownSTUB()
{
	__asm
	{
		nop								// Make room for original code
		nop
		nop
		nop
		nop
		nop
		pushad
		mov ecx, esi
		call GameKeyDown
		// Shuffle... This is ugly!
		push eax						// Push return code
		add esp, 4						// Move stack
		popad							// Restore registers
		sub esp, 0x24					// move stack
		pop esi							// Pop return code
		add esp, 0x20					// Move stack
		ret
	}
}

/////////////////////////////////////////////////////////////////////////////
// GameKeyDown()
// --------------------------------------------------------------------------
// Our 'clean' GameKeyDown() 
/////////////////////////////////////////////////////////////////////////////
DWORD __fastcall GameKeyDown(DWORD aKeyCode)
{
	BYTE nKey = (BYTE)aKeyCode;

	if(ShowKeycode)
	{
		ShowKeycode = FALSE;
		char t[16];
		sprintf(t, "Keycode: %.2x", nKey);
		server->GamePrintInfo(t);
		return 0;
	}
	
	if(g_Keys[nKey].pCmd)
	{
		if(g_Keys[nKey].bRotational)
		{
			server->GameCommandLine(g_Keys[nKey].RotateCommand());
		}
		else
		{
			PCMD_LIST pList = g_Keys[nKey];
			while(pList && pList->pCmd)
			{
				server->GameCommandLine(pList->pCmd);
				pList = pList->pNext;
			}
		}
	}

	return g_Keys[nKey].nReturncode;
}
*/

