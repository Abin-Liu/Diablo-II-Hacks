//////////////////////////////////////////////////////////////////////
// Packet.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __PACKET_H__
#define __PACKET_H__

#include <windows.h>
#include "Utilities\\LinkedList.h"

class CPacket  
{
public:	
	
	CPacket(const BYTE* aPacket = NULL, DWORD aLen = 0, DWORD dwDelay = 0);
	CPacket(const CPacket& rhs);
	virtual ~CPacket();

	void SetPacket(const BYTE* aPacket, DWORD aLen, DWORD dwDelay);
	void DestroyPacket();
	BOOL IsValidPacket() const;
	BYTE* GetPacket();
	const BYTE* GetPacket() const;
	DWORD GetLength() const;
	DWORD GetDelay() const;

	const CPacket& operator=(const CPacket& rhs);

	BOOL SendToServer() const;
	BOOL SendToGame() const;

protected:

	BYTE* m_pPacket;
	DWORD m_dwLen;
	DWORD m_dwDelay;
};

void CALLBACK FreePacketPtr(CPacket*& ptr);
typedef CLinkedList<CPacket*, CPacket*, FreePacketPtr> PacketQueue;

#endif // __PACKET_H__
