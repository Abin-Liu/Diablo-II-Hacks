
#include "external\\D2hackit.h"
#include "external\\D2client.h"
#include "offsets.h"
#include "definitions.h"
#include "d2functions.h"
#include "funclib.h"


CLIENTINFO
(		
	0,0,						// Module version (Major, Minor)
	"Abin",						// Author
	"www.wxjindu.com/abin/",	// url (http:// is appended)
	"Sample plugin",			// Short module description
	"abinn32@yahoo.com"			// Author eMail
)

BOOL PRIVATE OnCommandDebug(char **argv, int argc);
MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc6 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},

	{
		"debug",
		OnCommandDebug,
		"debugÿc6 Internal usage.\n"
	},

	// Add your own commands here

	{NULL}	// No more commands
};


BOOL EXPORT OnClientStart()
{
	DefineOffsets();
	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* game)
{
	
	
	return;
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* game)
{	
	return;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}


DWORD EXPORT OnGameTimerTick(void)
{
	return 0;
}

BOOL PRIVATE OnCommandDebug(char **argv, int argc)
{
	/*
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	char sz[256] = "";
	int nCount = 0;
	for (Skill* pSkill = D2COMMON_GetStartSkill(pPlayer); pSkill && pSkill->pSkillInfo; pSkill = pSkill->pNextSkill)
	{
		nCount++;
		
		sprintf(sz, "ID %04X, Level %d", pSkill->pSkillInfo->Id, pSkill->SkillLevel);
		server->GamePrintInfo(sz);
	}

	sprintf(sz, "Count: %d", nCount);
	server->GamePrintInfo(sz);
	*/
	
	/*
	UnitAny* p = D2CLIENT_GetPlayerUnit();
	Room* pRoom = D2COMMON_GetRoomFromUnit(p);
	
	//for (Room* pRoom = D2COMMON_GetRoomFromUnit(p); pRoom; pRoom = pRoom->ptNext)
	//for (int i = 0; i < pRoom->NumRooms && pRoom->ptRoomList[i]; i++)
	//{
		char sz[256] = "";
		//sprintf(sz, "map %02X", D2COMMON_GetLevelIdFromRoom(pRoom->ptRoomList[i]));
		
		//for (UnitAny* punit = pRoom->ptRoomList[i]->ptFirstUnit; punit; punit = punit->ptRoomNext)
		//for (p = D2Client_GetNextPlayer(p); p; p = D2Client_GetNextPlayer(p))
		{
			char str[256] = "";
			char szName[128] = "";
			ConvertFromUnicode(D2CLIENT_GetNameFromUnit(punit), szName, 127);
			sprintf(str, ", unit %08X name: %s", punit->dwId, szName);
			strcat(sz, str);
			
		}
		server->GamePrintInfo(sz);

		
		//sprintf(sz, "map %02X", D2COMMON_GetLevelIdFromRoom(pRoom));
		//server->GamePrintInfo(sz);
	}
	//*/

	


	/*
	p = pRoom->ptFirstUnit;//D2COMMON_GetFirstRoomUnit(pRoom);

	if (p == NULL)
		server->GamePrintError("no room unit");

	if (p)
	{
		char szName[128] = "";
		ConvertFromUnicode(D2CLIENT_GetNameFromUnit(p), szName, 127);
		sprintf(sz, "unit %08X name: %s", p->dwId, szName);
		server->GamePrintInfo(sz);
		//p = p->ptRoomNext;
		p = pRoom->ptOtherUnit;
	}
	*/

	//*
	if (argc < 4)
		return 1;


	UnitAny* punit = D2CLIENT_GetUnit(strtoul(argv[2], 0, 16), atoi(argv[3]));
	if (punit)
	{
		char sz[256] = "";
		char szName[128] = "";
		ConvertFromUnicode(D2CLIENT_GetNameFromUnit(punit), szName, 127);
		sprintf(sz, "unit %08X name: %s, x %04X, y %04X", punit->dwId, szName, punit->wX, punit->wY);
		server->GamePrintInfo(sz);
	}
	else
	{
		server->GamePrintError("no unit");
	}
	//*/
	
	return TRUE;
}