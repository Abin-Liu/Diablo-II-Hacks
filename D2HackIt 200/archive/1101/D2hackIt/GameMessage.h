//////////////////////////////////////////////////////////////////////
// GameMessage.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __GAMEMESSAGE_H__
#define __GAMEMESSAGE_H__

#include <windows.h>
#include "CommonStructs.h"

DWORD ServerPacketBeforeReceived(BYTE* aPacket, DWORD aLen);
DWORD ServerPacketBeforeSent(BYTE* aPacket, DWORD aLen);

void QueueUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam = 0, LPARAM lParam = 0);
void QueueThisPlayerMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
void CleanMessageQueue();

#endif // __GAMEMESSAGE_H__