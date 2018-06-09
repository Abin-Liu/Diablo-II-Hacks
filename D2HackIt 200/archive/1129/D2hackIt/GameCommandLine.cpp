//////////////////////////////////////////////////////////////////////
// GameCommandLine.cpp
// -------------------------------------------------------------------
//
// <thohell@home.se>
// <Dan_Goon@diabloworld.com>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"
#include "Server20.h"
#include "ServerDraw.h"

BOOL PRIVATE GameCommandLineHelp(char** argv, int argc);
BOOL PRIVATE GameCommandLineVersion(char** argv, int argc);
BOOL PRIVATE GameCommandLineCredits(char** argv, int argc);
BOOL PRIVATE GameCommandLineLoad(char** argv, int argc);
BOOL PRIVATE GameCommandLineUnload(char** argv, int argc);
BOOL PRIVATE GameCommandLineLoadGroup(char** argv, int argc);
BOOL PRIVATE GameCommandLineList(char** argv, int argc);
BOOL PRIVATE GameCommandLineSet(char** argv, int argc);
BOOL PRIVATE GameCommandLineSend(char** argv, int argc);
BOOL PRIVATE GameCommandLineReceive(char** argv, int argc);
BOOL PRIVATE GameCommandLineSendkey(char** argv, int argc);
BOOL PRIVATE GameCommandLineSay(char** argv, int argc);
BOOL PRIVATE GameCommandLineOverhead(char** argv, int argc);
BOOL PRIVATE GameCommandLineCrash(char** argv, int argc);
BOOL PRIVATE GameCommandLineTest(char** argv, int argc);		// Test function
BOOL PRIVATE GameCommandLineRepeat(char** argv, int argc);
BOOL PRIVATE GameCommandLineClick(char** argv, int argc);
BOOL PRIVATE GameCommandLineDelay(char** argv, int argc);
BOOL PRIVATE GameCommandLineDisconnectChat(char** argv, int argc);
BOOL PRIVATE GameCommandLineSaveAndExit(char** argv, int argc);
BOOL PRIVATE GameCommandLineBanner(char** argv, int argc);  // 2.00 (Abin)
BOOL PRIVATE GameCommandLineBackToTown(char** argv, int argc); // 2.00 (Abin)
BOOL PRIVATE GameCommandLineMyStats(char** argv, int argc); // 2.00 (Abin)
BOOL PRIVATE GameCommandLineSync(char** argv, int argc); // 2.00 (Abin)
BOOL PRIVATE GameCommandLineDebugTest(char** argv, int argc); // debug only

//
//	Initialize commands
GAMECOMMANDSTRUCT GameCommands[] =
{
	{
		"help",
		GameCommandLineHelp,
		"�c4.help�c0 List all commands available in the server dll\n"
		"�c4.<command> help�c0 Shows detailed help for specified command.\n"
		"�c4.<module> help�c0 Shows help for specified module."
	},
	{
		"load",
		GameCommandLineLoad,
		"�c4.load <module>�c0 Loads the specified module."
	},
	{
		"unload",
		GameCommandLineUnload,
		"�c4.unload <module>�c0 Unloads the specified module."
	},
	{
		"loadgroup",
		GameCommandLineLoadGroup,
		"�c4.loadgroup <group>�c0 Loads module list in group GroupLoad.<group> from d2hackit.ini."
	},
	{
		"list",
		GameCommandLineList,
		"�c4.list�c0 Lists all currently loaded modules."
	},
	{
		"version",
		GameCommandLineVersion,
		"�c4.version�c0 Displays detailed version information on the D2Hackit server dll."
	},
	{
		"credits",
		GameCommandLineCredits,
		"�c4.credits"
	},
	{
		"set",
		GameCommandLineSet,
		"�c4.set verbose <on|off>�c0 Verbose output (Can also be set in D2HackIt.ini)"
	},
	{
		"send",
		GameCommandLineSend,
		"�c4.send <packet>�c0 Sends a packet to the server. Great for testing stuff :)\n"
		"The packet is not validated so please; no spaces, only hexadecimal number and even number of chars!\n"
		"Try �c4'.send 500100000001000000'�c0 in a game you created."
	},
	{
		"receive",
		GameCommandLineReceive,
		"�c4.receive <packet>�c0 Fakes a packet from the server to the game. Great for testing stuff :)\n"
		"The packet is not validated so please; no spaces, only hexadecimal number and even number of chars!"
	},
		
	{
		"sendkey",
		GameCommandLineSendkey,
		"�c4.sendkey <keycode>�c0 Sends a keystroke to the game"
	},
	
	{
		"say",
		GameCommandLineSay,
		"�c4.say <text>�c0 Sends a string of text to the game as if your character said it."
	},
	{
		"overhead",
		GameCommandLineOverhead,
		"�c4.overhead <text>�c0 Puts a box of text over your head."
	},
	{
		"repeat",
		GameCommandLineRepeat,
		"�c4.repeat <count> <command>�c0 Repeats <command>, <count> number of times"
	},
	
	{
		"crash",
		GameCommandLineCrash,
		"�c4.crash�c0 This crashes the game in those rare cases when you *can't* exit a game!"
	},
	{
		"click",
		GameCommandLineClick,
		"�c4.click <x-coords> <y-coords>�c0 Simulates a mouse click at x,y coordinates in window window_name."
	},
	{
		"delay",
		GameCommandLineDelay,
		"�c4.delay <msecs>�c0 Delays game for msecs milliseconds."
	},
	{
		"disconnectchat",
		GameCommandLineDisconnectChat,
		"�c4.disconnectchat�c0 Disconnect from chat server."
	},
	{
		"saveandexit",
		GameCommandLineSaveAndExit,
		"�c4.saveandexit�c0 Saves game and exits."
	},
	{
		"banner",
		GameCommandLineBanner,
		"�c4.banner�c0 Shows/hides D2Hackit banner."
	},

	{
		"backtotown",
		GameCommandLineBackToTown,
		"�c4.cast a portal and back to town."
	},

	{
		"mystats",
		GameCommandLineMyStats,
		"�c4.shows player's stats."
	},

	{
		"sync",
		GameCommandLineSync,
		"�c4.redraw client screen, reposition player to the last known good position."
	},

	{
		"debug",
		GameCommandLineDebugTest,
		"�c4.debug."
	},

	{
		NULL,
	}
};

//////////////////////////////////////////////////////////////////////
// FindGameCommand
// -------------------------------------------------------------------
// Returns pointer to a COMMANDSTRUCT
//////////////////////////////////////////////////////////////////////
GAMECOMMANDSTRUCT* PRIVATE FindGameCommand(char* name)
{
	GAMECOMMANDSTRUCT* gcs = GameCommands;
	while (gcs->szName)
	{
		if (!stricmp(gcs->szName, name))
			return gcs;
		gcs++;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLine
// -------------------------------------------------------------------
// This is where we take a command line, tokenize it, and send it to
// the right destination :)
//////////////////////////////////////////////////////////////////////
BOOL EXPORT GameCommandLine(char* raw)
{
	char*     p;  
	char	buf[1024];
	char	*msg;
	char*	argv[1024];
	int		argc = 0;
	bool	keep_going = true;
	GAMECOMMANDSTRUCT*	gcs;
	 
	// Parse through the command line for tokens, string characters, and the 
	// command separator.

	// Copy the command line to a temporary buffer so we can mangle it.
	sprintf(buf, "%s", raw);

	// Initialize a few pointers to it.
	argv[0]=p=buf;

	// This is druttis' tokenizer 
	// that has been prettified by bootyjuice :)
	while ( keep_going == true )
	{
		// Added by Abin (remove leading '.''s that were accidentally added by programmers)
		// e.g., now "server->GameCommandLine(".saveandexit");" should has exactly same
		// meaning with "server->GameCommandLine("saveandexit");"
		while (*p == '.') p++;

		// Parse through any initial spaces.
		while( *p == ' ' && *p != 0 ) p++;		
		
		// Did we reach the end of the command line already?
		if( p == 0 ) return FALSE;
		
		// Make sure we're starting with argc count properly set.
		// This is important if we've encountered multiple commands via the
		// command separator.
		argc = 0;
		argv[argc++] = p;
		
		// Reset the loop flag.  This will only get set to true
		// when we find another command to execute i.e. via the
		// command separator.
		keep_going = false;
		
		//	Tokenize cmdline.
		while (*p != 0) 
		{
			// Spaces are our token separator.
			if (*p == ' ') 
			{
				// Set the first space encountered to null.
				*(p++) = 0;

				// Parse past the rest until we get to the start of the next
				// token.
				while (*p == ' ') p++;
				
				// If we're not at the end of cmdline then
				// this must be the start of the next token.
				if (*p != 0) argv[argc++] = p;
			} 
			else if (*p == '\"') 
			{
				// We've encountered a " character so this must be the
				// start of a string token.
				// Remove the " character and adjust the token accordingly.
				*(p++) = 0;
				argv[argc - 1] = p;
				
				// Parse until we find the closing " character or the end of
				// cmdline.
				while ((*p != '"') && (*p != 0)) p++;
				
				// If we're not at the end of cmdline then we must've 
				// found the closing " character.  Replace it with null and
				// parse ahead to the start of the next token.
				if (*p != 0) 
				{
					*(p++) = 0;
					while (*p == ' ') p++;
				}

				// If we're STILL not at the end of cmdline then
				// we must be at the start of the next token. Set
				// argv to point here and increment argc.
				if (*p != 0) argv[argc++] =  p;
			} 
			/*
			else if (*p == '|') 
			{
				// We've encountered the command separator.

				// Replace command separator with null character and move one
				// character forward.
				*(p++) = 0;

				// If there's another command to execute then flag it as such.
				if( *p != 0)
				{
					keep_going = true;

					// Parse ahead through whitespace.
					while (*p == ' ') p++;
				}
				else
				{
					// We've encountered the end of the command line 
					// so stop looking for additional commands.
					keep_going = false;
				}

				// Break out of the innermost loop.
				break;

			}
			*/
			else
			{
				// Parse through the token.
				p++;
			}

		}

		gcs = FindGameCommand(argv[0]);
		//
		// Is this a built-in function ?
		if (gcs) 
		{
			//
			// If functions returns false, show usage help
			if (!gcs->pFunc(argv, argc)) 
			{
//				t=new char[strlen(gcs->szUsage)+1];
				GamePrintInfo("Usage:");
				if(gcs->szUsage && gcs->szUsage[0])
				{
					strcpy(buf, gcs->szUsage);
					msg = p = buf;
					while (*p != 0) 
					{
						if (*p == '\n') 
						{
							*(p++) = 0;
							GamePrintInfo(msg);
							if (*p != 0) msg = p;
						} 
						else
						{
							p++;
						}
					}
					GamePrintInfo(msg);
				}
			}
		} 
		else 
		{	
			//
			// This is not a built in function
			// Do we have a module loaded with that name ?
			if(!ClientModules.GetItemCount())
			{
				// We have no modules...
				sprintf(buf, "�c0Module does not exist for command [%s]. Type .help to get help.", raw);
				GamePrintError(buf);	//"�c0Unknown command. Type .help to get help.");
				return FALSE;
			}

			LinkedItem *li = FindClientStructByName(argv[0]);
			if(li)
			{
				PCIS pCIS = ClientModules.GetCIS(li);

				// If module supports CLI, route the arguments to it
				if(pCIS && pCIS->OnGameCommandLine)
				{
					return pCIS->OnGameCommandLine(argv, argc);
				}
				else
				{
					//
					// If not, just let the user know
					char tbf[128];
					sprintf(tbf, "Module �c4'%s'�c0 does not support the command line interface.", argv[0]);
					GamePrintInfo(tbf);
					return FALSE;
				}
			}

			sprintf(buf, "�c0Unknown command [%s]. Type .help to get help.", raw);
			GamePrintError(buf);	//"�c0Unknown command. Type .help to get help.");
//			GamePrintError("�c0Unknown command. Type .help to get help.");
		}

	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineHelp
// -------------------------------------------------------------------
// Handles command line help
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineHelp(char** argv, int argc)
{
	if (argc>1) return FALSE;
	//
	// Header
	GamePrintInfo("Available D2HackIt commands:");
	//
	// Loop through commands and write them to screen.
	char t[128];
	for (int i=0; GameCommands[i].szName != NULL; i++)
	{
		sprintf(t, "�c4.%s",GameCommands[i].szName);
		GamePrintInfo(t);
	}
	//
	// The two extended modes of '.help'
	GamePrintInfo("�c4.<command> help");
	GamePrintInfo("�c4.<module> help");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineVersion
// -------------------------------------------------------------------
// 
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineVersion(char** argv, int argc)
{
	//
	// Show usage if we send too many arguments
	if (argc>1) return FALSE;
	//
	// Just print it :)
	char t[256];
	sprintf(t, "�c4D2HackIt! Mk2�c0 version %d.%.2d (%s@%s)",LOWORD(si->Version), HIWORD(si->Version), psi->BuildDate, psi->BuildTime);
	GamePrintInfo(t);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineCredits
// -------------------------------------------------------------------
// 
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineCredits(char** argv, int argc)
{
	//
	// Show usage if we send too many arguments
	if (argc>1) return FALSE;
	//
	// Just print it :)
	GamePrintInfo("CREDITS:");
	GamePrintInfo("These people have in one way or another made this project possible:");
	GamePrintInfo("�c4Druttis, mousepad, TechWarrior, Nyttzy99, BlackBird, hello2k1, Arsenic,");
	GamePrintInfo("�c4onlyer, icky, RazorBladeEnema, Skull, proff, ClckWtchr");
	GamePrintInfo("BIG thanks to all the beta testers, too many to fit here! :)");
	GamePrintInfo("Also, thanks to d2network for hosting D2HackIt! �c4(http://thohell.d2network.com)");
	GamePrintInfo("�c4`Soc�c0 wanted to be mentioned here, so I will mention him as well :)");
	GamePrintInfo("Latest updates done by:");
	GamePrintInfo("v0.57 - BootJuice - click, fixed '|', and added string support? \"\"");
	GamePrintInfo("v0.58 - D'Flame - Plugin recursive directory search, improved GetHackProfileString");
	GamePrintInfo("v0.59 - 0.62 - herzog_zwei");
	GamePrintInfo("From V0.70 - Dan_Goon");

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineLoad
// -------------------------------------------------------------------
// Loads a client
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineLoad(char** argv, int argc)
{
	if (argc != 2) return FALSE;
	if (!stricmp(argv[1], "help")) return FALSE;

	LoadClientModule(argv[1]);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineUnload
// -------------------------------------------------------------------
// Unloads a client
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineUnload(char** argv, int argc)
{
	if(argc != 2) return FALSE;
	if(!stricmp(argv[1], "help")) return FALSE;

	if(!ClientModules.GetItemCount())
	{
		GamePrintInfo("No modules loaded!");
		return TRUE;
	}

	UnloadClientModuleByName(argv[1]);
	return TRUE;
}

//
// Group Loading feature is modified by Dan_Goon using class CClientGroup
//

BOOL PRIVATE LoadGroupList(char* list, char* name)
{
	EnterCriticalSection(&psi->csData);
	if(!list || psi->GroupFuncDisabled)
	{
		LeaveCriticalSection(&psi->csData);
		DGTRACE((DBGHDR "Now LoadGroup Function is under processing!\n"));
		return FALSE;
	}

	// disable group function in case a module tries to be funny and use loadgroup/unloadgroup
	psi->GroupFuncDisabled = TRUE;
	LeaveCriticalSection(&psi->csData);

	DGTRACE((DBGHDR "LoadGroupList[%s]...\n", list));

	CClientGroup* loadlist = new CClientGroup;

	DGTRACE((DBGHDR "loadlist created...\n"));

	char *sp = list, *p = list;
	while(*p != 0)
	{
		if(*p == ',')
		{
			*(p++) = 0;

			loadlist->AddItem(sp);

			while(*p == ' ') p++;
			if(*p != 0) sp = p;
		}
		else p++;
	}

	if(*sp)
	{
		loadlist->AddItem(sp);
	}

	DGTRACE((DBGHDR "loadlist->LoadAll()...\n"));
	loadlist->LoadAll();

	DGTRACE((DBGHDR "loadlist->LoadAll() Finished\n"));

	if(name)
	{
		if(psi->ModuleGroupName) delete [] psi->ModuleGroupName;
		psi->ModuleGroupName = new char[strlen(name)+1];
		strcpy(psi->ModuleGroupName, name);

		if(psi->ModuleGroup) delete psi->ModuleGroup;
		psi->ModuleGroup = loadlist;
	}
	else 
	{
		delete loadlist;
	}

	EnterCriticalSection(&psi->csData);
	psi->GroupFuncDisabled = FALSE;
	LeaveCriticalSection(&psi->csData);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineLoadGroup
// -------------------------------------------------------------------
// Loads a group of clients
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineLoadGroup(char** argv, int argc)
{
	if(argc != 2) return FALSE;
	if(!stricmp(argv[1], "help")) return FALSE;

	char s[128];
	char t[512];

	_snprintf(s, sizeof(s), "Group.%s", argv[1]);

	if(!GetIniString("D2HackIt", "Misc", s, t, sizeof(t)))
	{
		_snprintf(t, sizeof(t), "Group '%s' not found", argv[1]);
		GamePrintError(t);
		return TRUE;
	}

	_snprintf(s, sizeof(s), "Loading group '%s'", argv[1]);
	GamePrintInfo(s);
	
	return LoadGroupList(t, argv[1]);
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineList
// -------------------------------------------------------------------
// Lists loaded clients
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineList(char** argv, int argc)
{
	//
	// Show usage if we send too many arguments
	if(argc>1) return FALSE;

	//
	// Are any modules loaded ?
	if(!ClientModules.GetItemCount())
	{
		GamePrintInfo("No modules loaded!");
		return TRUE;
	}
	//
	// Header
	GamePrintInfo("Loaded modules:");
	//
	// Loop through modules and print them to screen
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		PCIS pCIS = ClientModules.GetCIS(li);
		if(!pCIS) continue;

		char t[256];
		sprintf(t, "�c4%s�c0", pCIS->Name);
		if (pCIS->GetModuleVersion)
			sprintf(t, "%s version %d.%d",t, LOWORD(pCIS->GetModuleVersion()), HIWORD(pCIS->GetModuleVersion()));
		if (pCIS->GetModuleAuthor)
			sprintf(t, "%s by %s", t, pCIS->GetModuleAuthor());
		if (pCIS->GetModuleEmail)
			sprintf(t, "%s (%s)", t, pCIS->GetModuleEmail());
		if (pCIS->GetModuleWebsite)
			sprintf(t, "%s �c4http://%s", t, pCIS->GetModuleWebsite());

		GamePrintInfo(t);
		li = ClientModules.GetNextItem(li);
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineSet
// -------------------------------------------------------------------
// Set server options
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineSet(char** argv, int argc)
{
	// Too few arguments, show usage
	if (argc<3) return FALSE;
	//
	// Check for 'verbose' options
	if (!stricmp(argv[1], "verbose"))
	{
		if (!stricmp(argv[2], "on"))
		{
			psi->Verbose = TRUE;
			GamePrintInfo("Verbose operation is �c4ON");
			SetHackProfileString("D2HackIt", "Misc", "Verbose", "on");
			return TRUE;
		}
		if (!stricmp(argv[2], "off"))
		{
			psi->Verbose = FALSE;
			GamePrintInfo("Verbose operation is �c4OFF");
			SetHackProfileString("D2HackIt", "Misc", "Verbose", "off");
			return TRUE;
		}
	}
	//
	// No valid options found, show usage
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineSend
// -------------------------------------------------------------------
// QnD packet sender for testing stuff
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineSend(char** argv, int argc)
{
	//
	// Some sanity checks
	if (argc != 2) return FALSE;
	if (strlen(argv[1])%2) return FALSE;
	if (!stricmp(argv[1], "help")) return FALSE;
	//
	// Make a buffer
	int len=strlen(argv[1])/2;
	PBYTE buf=new BYTE[len];
	//
	// Build the packet
	char *x="";
	for (int i=len-1; strlen(argv[1]) > 0; i--)
	{
		buf[i]=(BYTE)strtoul(&argv[1][i*2], &x, 0x10);
		argv[1][i*2]=0;
	}
	//
	// Send the packet
	GameSendPacketToServer(buf, len);
	//
	// Done
	delete [] buf;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineReceive
// -------------------------------------------------------------------
// QnD incoming packet sender for testing stuff
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineReceive(char** argv, int argc)
{
	//
	// Some sanity checks
	if (argc != 2) return FALSE;
	if (strlen(argv[1])%2) return FALSE;
	if (!stricmp(argv[1], "help")) return FALSE;
	//
	// Make a buffer
	int len=strlen(argv[1])/2;
	PBYTE buf=new BYTE[len];
	//
	// Build the packet
	char *x="";
	for (int i=len-1; strlen(argv[1]) > 0; i--)
	{
		buf[i]=(BYTE)strtoul(&argv[1][i*2], &x, 0x10);
		argv[1][i*2]=0;
	}
	//
	// Send the packet
	GameSendPacketToGame(buf, len);
	//
	// Done
	delete [] buf;
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineSendkey
// -------------------------------------------------------------------
// Send a key-down message to the game. Reenabled by Abin.
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineSendkey(char** argv, int argc)
{
	// Sanity checks
	if (argc != 2) return FALSE;
	if (strlen(argv[1]) != 2) return FALSE;
	char* x="";
	DWORD Keycode=strtoul(argv[1], &x, 0x10);
	if (strlen(x)) return FALSE;
	SendKey((BYTE)Keycode);
	//DWORD p = psi->GameKeyDownIntercept;

	//GamePrintError(".sendkey is not working yet!");

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineSay
// -------------------------------------------------------------------
// Makes your character say something in game
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineSay(char** argv, int argc)
{
	// Sanity check
	if (argc<2) return FALSE;
		
	BYTE aPacket[1024];
	char aString[1024];
	aString[0]=0;
	sprintf(aString, "%s", argv[1]);

	for (int i=2; i!=argc; i++)
	{
		sprintf(aString, "%s %s", aString, argv[i]);
	}

	sprintf((char*)aPacket, "%c%c%c%s%c%c%c",
			0x15, 0x01, 0x00, aString, 0,0,0);
	GameSendPacketToServer(aPacket, strlen(aString)+6);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineOverhead
// -------------------------------------------------------------------
// Makes your character say something in game over his head.
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineOverhead(char** argv, int argc)
{
	// Sanity check
	if (argc<2) return FALSE;
		
	BYTE aPacket[1024];
	char aString[1024];
	aString[0]=0;
	sprintf(aString, "%s", argv[1]);

	for (int i=2; i!=argc; i++)
	{
		sprintf(aString, "%s %s", aString, argv[i]);
	}

	sprintf((char*)aPacket, "%c%c%c%s%c%c%c",
			0x14, 0x00, 0x00, aString, 0,0,0);
	GameSendPacketToServer(aPacket, strlen(aString)+6);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineRepeat
// -------------------------------------------------------------------
// Repeats <command>, <count> number of times
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineRepeat(char** argv, int argc)
{
	if (argc < 3)
		return FALSE;

	int count=atoi(argv[1]);
	
	// Build commandline
	char t[256];
	t[0]=0;
	for(int i=2;i!=argc;i++)
		sprintf(t, "%s %s", t, argv[i]);
	
	GamePrintInfo(t);

	// Send command
	for (; count; count--)
		GameCommandLine(t+1);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineCrash
// -------------------------------------------------------------------
// Crashes game, not recommended for normal use :)
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineCrash(char** argv, int argc)
{
	// Jump to nowhere should do it :)
	__asm
	{
		push 0
		ret
	}
	return TRUE;
}

	

//////////////////////////////////////////////////////////////////////
// GameCommandLineTest
// -------------------------------------------------------------------
// Test stuff goes here
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineTest(char** argv, int argc)
{
	GamePrintInfo("�c1Test does nothing!");
	return TRUE;
}

	
//////////////////////////////////////////////////////////////////////
// GameCommandLineClick
// -------------------------------------------------------------------
// Simulates a mouse click at the coordinates you specify.
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineClick(char** argv, int argc)
{
	// Sanity check.
	if (argc != 3) return FALSE;

	if(!g_hD2)
	{
		GamePrintError("Could not find Diabli II Window!");
		return TRUE;
	}

	// Convert x and y coordinates to numbers
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	int pos = ((short)y << 16) + (short)x;  

	HWND hFG = GetForegroundWindow();
	SetForegroundWindow(g_hD2);
	SendMessage(g_hD2, WM_MOUSEMOVE, NULL, pos);  
	SendMessage(g_hD2, WM_LBUTTONDOWN, MK_LBUTTON, pos);  
	SendMessage(g_hD2, WM_LBUTTONUP, MK_LBUTTON, pos);
	SetForegroundWindow(hFG);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineDelay
// -------------------------------------------------------------------
// Delay for n msecs
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineDelay(char** argv, int argc)
{
	// Sanity checks
	if (argc != 2) return FALSE;
	if (!stricmp(argv[1], "help")) return FALSE;
	DWORD delay = strtoul(argv[1], NULL, 10);
	if(0==delay || delay>30000) {
		GamePrintError("Invalid delay value");
		return TRUE;
	}

	Sleep(delay);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineDisconnectChat
// -------------------------------------------------------------------
// Disconnects from chat server
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineDisconnectChat(char** argv, int argc)
{
	// Sanity check.
	if (argc != 1)
		return FALSE;

	for(int i=0; i<5; ++i) {
		GameSendMessageToChat("/whoami");
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineSaveAndExit
// -------------------------------------------------------------------
// Saves game and exits
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineSaveAndExit(char** argv, int argc)
{
	// Sanity check.
	if (argc != 1)
		return FALSE;

	GameSaveAndExit();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineBanner
// -------------------------------------------------------------------
// Shows/hides D2Hackit banner
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineBanner(char** argv, int argc)
{
	// Sanity check.
	if (argc != 2)
		return FALSE;

	g_bDrawBanner = atoi(argv[1]);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GameCommandLineBackToTown
// -------------------------------------------------------------------
// Cast a town portal and walk in
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineBackToTown(char** argv, int argc) // 2.00 (Abin)
{
	BackToTown();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineMyStats
// -------------------------------------------------------------------
// Displays player's stats
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineMyStats(char** argv, int argc) // 2.00 (Abin)
{
	g_bDrawMyStats = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// GameCommandLineSync
// -------------------------------------------------------------------
// Reposition player to the last known good position, used for get out
// the infamous "black wall" lag.
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE GameCommandLineSync(char** argv, int argc) // 2.00 (Abin)
{
	RedrawClient(TRUE);
	return TRUE;
}


BOOL PRIVATE GameCommandLineDebugTest(char** argv, int argc) // debug only
{
	DebugTest(argv, argc);
	return TRUE;
}




