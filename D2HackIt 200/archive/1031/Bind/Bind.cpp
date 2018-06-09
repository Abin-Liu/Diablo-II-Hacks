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
//void GameKeyDownSTUB(); //0x6FACEF99
//DWORD __fastcall GameKeyDown(DWORD aKeyCode);

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

///////////////////////////////////////////////////////////////////////////
// Stuff below are from ClientCore.cpp
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// ClientCore.cpp
// -------------------------------------------------------------------
// Default Client Dll entrypoint.
//
// <thohell@home.se>
// <Dan_Goon@diabloworld.com>
//
// 0.7	: Updated by Dan_Goon
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Global server struct holding pointers to exported functions
//////////////////////////////////////////////////////////////////////
FUNCTIONENTRYPOINTS		g_SF = { 0 };	
FUNCTIONENTRYPOINTS		*server = &g_SF;	
HMODULE					g_hModule = NULL;			// New Global
PSERVERINFO				g_pSI = NULL;				// 0.7
PLAYERENTRYPOINTS		g_player = { 0 };			// 2.00 (Abin)
PLAYERENTRYPOINTS*		me = &g_player;				// 2.00 (Abin)

BOOL PRIVATE SetupClient(HMODULE hModule, LPCSTR szModule);

LPCSTR					szLodErr = "D2Client Load Error";
LPSTR					m_szLargeBuffer = NULL;
DWORD					m_nLargeBufferSize = 0;
DWORD					m_nSectNameIndex = 0;

#ifdef	D2HACKIT_DEBUG
char	g_szDebug[512];
#endif

#ifdef _AFXDLL
static AFX_EXTENSION_MODULE D2H_CLIENT = { NULL, NULL };
#endif

//********************************************************************************************
// Important Notice on UNLOADING PROCESS!!
//
//	Client Module's DllMain(DLL_PROCESS_DETACH) can be called in two different sequences as follows! 
//
//	1. When FreeLibray() is called by D2HackIt during normal unloading process
//		(by unload command or during ServerStop() process).
//	2. Diablo II is exiting by user op (this is called prior to D2HackIt's ServerStop()!).
//		In this case, all client's thread memory is detached before OnClientStop() call!!!!
//		So, if client references any memory allocated in client's thread while OnClientStop() process,
//		it may casue memory exception, crash, hanging or remained-in-memory.
//		To prevent this situation, the unloading process is changed as follows:
//
//	New Unloading Process
//
//	1. When client's DllMain(DLL_PROCESS_DETACH) is called, calls server->ClenaMeUp()
//	2. server->CleanMeUp() handles actuall unloading process and calls client's OnGameLeave() and OnClientStop().
//	3. While unloading client by normal command or in ServerStop() process, just do as before.
//
//	So, the actual unload sequence for two different cases will be :
//
//	1. Normal Sequence (Unloading D2HackIt by second "D2Loader.exe" execution or by ".unload" command)
//
//		1) D2HackIt's DllMain(DLL_PROCESS_DETACH) : ServerStop() - this is D2HackIt's unloading process.
//		2) calls optional OnGameLeave() and OnClientStop() of client.
//		3) frees up client info structure and delete linked list.
//		4) calls FreeLibrary(hClient).
//		5) After D2HackIt is destructed, client's DllMain(DLL_PROCESS_DETACH) will be called.
//		6) server->CleanMeUp() is called : BUT THIS IS TREATED AS DUMMY CALL - DO NOTHING.
//
//	2. D2-fisrt Exiting Sequence (Exit from D2 by user op or accidental exiting)
//
//		1) Client's DllMain(DLL_PROCESS_DETACH) is called first.
//		2) server->CleanMeUp() is called.
//		3) then calls optional OnGameLeave() and OnClientStop() of client.
//		4) frees up client info structure and delete linked list.
//		5) then D2HackIt's DllMain(DLL_PROCESS_DETACH) : ServerStop() is called.
//		6) clients are already unloaded - DO NOTHING.
//
//********************************************************************************************


//////////////////////////////////////////////////////////////////////
// Dll entry/exit
//////////////////////////////////////////////////////////////////////
extern "C" BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	char szTemp[MAX_PATH], szFile[MAX_PATH];
	BOOL bResult = TRUE;

    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

#ifdef _AFXDLL
		if (!AfxInitExtensionModule(D2H_CLIENT, (HINSTANCE)hModule)) return FALSE;
#endif

		DGTRACE(( CHDR "DLL_PROCESS_ATTACH\n" ));

		g_hModule = (HMODULE)hModule;
		GetModuleFileName( g_hModule, szTemp, sizeof(szTemp) );
		_splitpath(szTemp, NULL, NULL, szFile, NULL);		// Extract File Title

		bResult = FALSE;
		// Bind exported functions from server
		HMODULE hD2Hackit;
		hD2Hackit = (HMODULE)GetModuleHandle("D2HackIt");
		if(!hD2Hackit)
		{
			sprintf(szTemp, "[%s] : Failed to get D2HackIt!", szFile);
			MessageBox(NULL, szTemp, szLodErr, MB_OK);
			break;
		}

		// Fill in the server & g_pSI (server info)
		return SetupClient(hD2Hackit, szFile);

	case DLL_THREAD_DETACH:

		// Fall here first in case of accidental exit of Diablo II
		DGTRACE(( CHDR "DLL_THREAD_DETACH\n" ));
		break;

	case DLL_PROCESS_DETACH:

		// This is normal call from FreeLibrary
		DGTRACE(( CHDR "DLL_PROCESS_DETACH\n" ));

		// This call is available from V0.8
		if(!IsBadCodePtr((FARPROC)server->CleanMeUp)) server->CleanMeUp(g_hModule);

		if(m_szLargeBuffer) delete [] m_szLargeBuffer;
		m_szLargeBuffer = NULL;

#ifdef _AFXDLL
		AfxTermExtensionModule(D2H_CLIENT);
#endif
		break;
    } 

    return bResult;
}

// From V0.7 by Dan_Goon

BOOL PRIVATE SetupClient(HMODULE hModule, LPCSTR szModule)
{
	char tbf[256];

	BIND_TO_SERVER(GetServerInfo);			// This is available from V0.59

	// We need at least 0.59
	if(!server->GetServerInfo)
	{
		sprintf(tbf, "[%s] : Failed to bind server functions!\n\nPlease use D2HackIt! V0.59 or later.", szModule);
		MessageBox(NULL, tbf, szLodErr, MB_OK);
		return FALSE;
	}

	g_pSI = server->GetServerInfo(__SERVERVERSION__, szModule);

	BIND_TO_SERVER(GamePrintString);
	BIND_AND_CHECK(GamePrintInfo);
	BIND_AND_CHECK(GamePrintError);
	BIND_AND_CHECK(GamePrintVerbose);
	BIND_AND_CHECK(GamePrintString);
	BIND_AND_CHECK(GameCommandLine);
	BIND_AND_CHECK(GameSendPacketToServer);
	BIND_AND_CHECK(GameSendPacketToGame);
	BIND_AND_CHECK(GetFingerprint);
	BIND_AND_CHECK(Intercept);
	BIND_AND_CHECK(GetThisgameStruct);
//	BIND_AND_CHECK(GetHackProfileString);		// This will be obsoleted. Don't use this after 0.7!
//	BIND_AND_CHECK(GetHackProfileStringEx);		// This will be obsoleted. Don't use this after 0.7!
//	BIND_AND_CHECK(GetHackProfileSectionNames);	// This will be obsoleted. Don't use this after 0.7!
	BIND_AND_CHECK(SetHackProfileString);

	BIND_AND_CHECK(GameSendPacketToGame);
	BIND_AND_CHECK(GameInsertPacketToGame);		// 0.59
	BIND_AND_CHECK(GameSendMessageToChat);		// 0.59
	BIND_AND_CHECK(D2FindFile);
//	BIND_AND_CHECK(FreeHackProfileString);		// 0.62 This will be obsoleted. Don't use this after 0.7!
	BIND_AND_CHECK(GameSaveAndExit);			// 0.62

	BIND_AND_CHECK(GetIniFileName);				// 0.71
	BIND_AND_CHECK(GetIniString);				// 0.71
	BIND_AND_CHECK(GetIniSectionNames);			// 0.71

	BIND_AND_CHECK(FindMyD2WndHandle);			// 0.73
	BIND_AND_CHECK(HookD2WndProc);				// 0.80
	BIND_AND_CHECK(CleanMeUp);					// 0.80

	if(!g_pSI)
	{
		server->GamePrintError("D2HackIt Server Info not available!");
	}
	else
	{
		_snprintf(tbf, sizeof(tbf), "D2HackIt directory = %s", g_pSI->PluginDirectory);
		server->GamePrintVerbose(tbf);
	}

	return TRUE;
}


//======================================================================
//				EnumIniSectionNames (Helper Funtion)
//======================================================================
//
// This helper function illustrates how to utilize GetIniSectionNames call.
// This maybe useful for modules which handles long lines of ini file text.
// You may remove this function from your code, if you don't need this feature.
//
// Usage:
//
//	LPSTR pString = EnumIniSectionNames( "YourD2hName", ENUM_START );
//	while( pString && *pString )
//	{
//		// <<<== Do your job here with returned pString
//
//		pString = EnumIniSectionNames( "YourD2hName", ENUM_NEXT );
//	}
//	EnumIniSectionNames( "YourD2hName", ENUM_FINISH );
//
//======================================================================
LPSTR PRIVATE EnumIniSectionNames( LPCSTR lpHackName, int nEnumAction )
{
	if(!server->GetIniSectionNames) return NULL;

	switch(nEnumAction)
	{
	case ENUM_START:	// = Start Enum

		if(m_szLargeBuffer) delete [] m_szLargeBuffer;
		m_szLargeBuffer = NULL;

		// Get required buffer size
		m_nLargeBufferSize = server->GetIniSectionNames(lpHackName, NULL, 0);
		if(!m_nLargeBufferSize) break;

		m_szLargeBuffer = new char[m_nLargeBufferSize];
		if(!m_szLargeBuffer)
		{
			server->GamePrintError("Failed to allocate buffer");
			break;
		}

		server->GetIniSectionNames(lpHackName, m_szLargeBuffer, m_nLargeBufferSize);
		m_nSectNameIndex = 0;
		return m_szLargeBuffer;

	case ENUM_NEXT:		// = Enum Next

		if(!m_szLargeBuffer) break;

		m_nSectNameIndex += strlen(&m_szLargeBuffer[m_nSectNameIndex]) + 1;	// point to next z-string start

		if(m_nSectNameIndex >= m_nLargeBufferSize ||
			!m_szLargeBuffer[m_nSectNameIndex]) break;	// end of buffer reached
		
		return &m_szLargeBuffer[m_nSectNameIndex];

	case ENUM_FINISH:	// = Finish Enum

		if(m_szLargeBuffer) delete [] m_szLargeBuffer;
		m_szLargeBuffer = NULL;
		m_nSectNameIndex = 0;
		break;
	}

	return NULL;
}

//======================================================================
//				GetHackProfileString
//======================================================================
//
// This private function is for previous modules which used to call
//  server->GetHackProfileString, to keep its function-level consistency.
// Just remove your current 'server->' part.
//
// Notice : Never delete returned string pointer and do not save this pointer
//   for future use. The string context pointed by this pointer will be
//	 changed to new value whenever this function is called again.
//
//======================================================================
LPSTR PRIVATE GetHackProfileString( LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName)
{
	static char szLineBuffer[1024];		// this maybe enough for most of ini file lines

	if(!server->GetIniString ||
		!server->GetIniString(lpHackName, lpSectionName, lpKeyName, szLineBuffer, 1024))

		szLineBuffer[0] = 0;
	
	return szLineBuffer;
}


//////////////////////////////////////////////////////////////////////
// Stubfunctions for 'property get' functions.
//////////////////////////////////////////////////////////////////////
LPCSTR	EXPORT GetModuleAuthor()		{return ModuleAuthor;}
LPCSTR	EXPORT GetModuleWebsite()		{return ModuleWebsite;}
DWORD	EXPORT GetModuleVersion()		{return ModuleVersion;}
LPCSTR	EXPORT GetModuleEmail()			{return ModuleEmail;}
LPCSTR	EXPORT GetModuleDescription()	{return ModuleDescription;}

//////////////////////////////////////////////////////////////////////
// OnClientCommandLine
// -------------------------------------------------------------------
// The modules own extension of the command line interface. Any custom
// commands you add are parsed here.
//
// Return value should be TRUE, but it is not used at this 
// time.
//
// Arguments when we get here:
// argv[0]			Name of module
// argv[1]			Name of command (If supplied)
// argv[2 ... n]	The rest
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnGameCommandLine(char* argv[], int argc)
{
	DGTRACE((DBGHDR "OnGameCommandLine : (%d) [%s]..\n", argc, argc > 0 ? argv[0] : "Mola"));

	// Check if user supplied anything at all, if not assume help...
	if (argc==1) argv[argc++] = "help";


	MODULECOMMANDSTRUCT* mcs = ModuleCommands;

	while (mcs->szName)
	{
		if (!stricmp(mcs->szName, argv[1])) break;
		mcs++;
	}

	char *p, fMsg[256];
	//
	// Is this a built-in function ?
	if (mcs->szName)
	{
		// If functions returns false, show usage help
		if (!mcs->pFunc(argv, argc))
		{
			server->GamePrintInfo("Usage:");

			p = strtok(mcs->szUsage, "\n");
			while(p)
			{
				sprintf(fMsg, "ÿc4.%s %s", argv[0], p);
				server->GamePrintInfo(fMsg);

				p = strtok(NULL, "\n");
			}
		}
	}
	else
	{
		// Unknown command, show catch-all help phraze.
		sprintf(fMsg, "Unknown command ÿc4'%s %s'ÿc0 - try ÿc4'.%s help'ÿc0 to get help.",
			argv[0], argv[1], argv[0]);
		server->GamePrintError(fMsg);
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// OnClientCommandHelp
// -------------------------------------------------------------------
// Our default help function.
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc)
{
	// If command line is longer than 2, show usage for 'help'
	if (argc>2) return FALSE;

	char t[1024];
	sprintf(t, "Available commands for %s:", argv[0]);
	server->GamePrintInfo(t);

	// Loop through ModuleCommands[] and print their names
	for (int i=0; ModuleCommands[i].szName != NULL; i++)
	{
		sprintf(t, "ÿc4.%s %s", argv[0], ModuleCommands[i].szName);
		server->GamePrintInfo(t);
	}

	sprintf(t, "For help on a specific command, type ÿc4.%s <command> help", argv[0]);
	server->GamePrintInfo(t);
	return TRUE;
}