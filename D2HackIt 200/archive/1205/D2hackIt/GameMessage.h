//////////////////////////////////////////////////////////////////////
// GameMessage.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __GAMEMESSAGE_H__
#define __GAMEMESSAGE_H__

#include <windows.h>
#include "CommonStructs.h"

DWORD ServerPacketBeforeSent(BYTE* aPacket, DWORD aLen);
DWORD ServerPacketBeforeReceived(BYTE* aPacket, DWORD aLen);
void ServerPacketAfterReceived(const BYTE* aPacket, DWORD aLen);
void SendThisPlayerMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
void MessageReset();

#endif // __GAMEMESSAGE_H__