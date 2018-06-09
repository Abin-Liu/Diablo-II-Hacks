/////////////////////////////////////////////////////////////////
// Client.h
//
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <windows.h>
#include "definitions.h"

void OnGameJoin(const UnitPlayer* pPlayer);
void OnGameLeave();
void OnGameTimerTick();

#endif // __CLIENT_H__