#ifndef __D2HCOMMON_H__
#define __D2HCOMMON_H__

#include <windows.h>
#include "D2Hackit.h"
#include "D2Client.h"

extern FUNCTIONENTRYPOINTS* server;
extern THISGAMESTRUCT *thisgame;
extern PSERVERINFO g_pSI;
extern char ConfigPath[_MAX_PATH];

#define PLAYERID	(thisgame->player->PlayerID)
#define PLAYERX		(thisgame->player->PlayerPositionX)
#define PLAYERY		(thisgame->player->PlayerPositionY)

enum
{
	CLASS_AMAZON = 0,
	CLASS_SORCERESS,
	CLASS_NECROMANCER,
	CLASS_PALADIN,
	CLASS_BARBARIAN,
	CLASS_DRUID,
	CLASS_ASSASSIN
};

#endif