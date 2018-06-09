///////////////////////////////////////////////////////////
// Me.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __ME_H__
#define __ME_H__

#include <windows.h>
#include "CommonStructs.h"

VOID Player_OnGameJoin(THISGAMESTRUCT* thisgame);
VOID Player_OnGameLeave();
BOOL VerifyItem(DWORD dwItemID);

#endif // __ME_H__