// MyRelayer.h: interface for the CMyRelayer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MYRELAYER_H__
#define __MYRELAYER_H__

#include "D2PacketRelayer.h"

class CMyRelayer : public CD2PacketRelayer  
{
public:
	CMyRelayer();
	virtual ~CMyRelayer();

protected:

	// Overrides
	virtual void OnClientStart();
	virtual void OnClientStop();
	virtual void OnGameJoin(LPCPLAYERINFO lpPlayerInfo);
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);
	virtual void OnGamePacketBeforeSent(const BYTE *aPacket, DWORD aLen);
};

#endif // __MYRELAYER_H__
