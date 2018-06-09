// MyRelayer.cpp: implementation of the CMyRelayer class.
//
//////////////////////////////////////////////////////////////////////

#include "MyRelayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyRelayer::CMyRelayer()
{

}

CMyRelayer::~CMyRelayer()
{

}

//////////////////////////////////////////////////////////////////////
// Game Notification Handlers
//////////////////////////////////////////////////////////////////////

void CMyRelayer::OnClientStart()
{
	// TODO: Add your game notification handler code here
}

void CMyRelayer::OnClientStop()
{
	// TODO: Add your game notification handler code here
}

void CMyRelayer::OnGameJoin(LPCPLAYERINFO lpPlayerInfo)
{
	// TODO: Add your game notification handler code here
}

void CMyRelayer::OnGameLeave()
{
	// TODO: Add your game notification handler code here
}

void CMyRelayer::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	// TODO: Add your game notification handler code here
}

void CMyRelayer::OnGamePacketBeforeSent(const BYTE *aPacket, DWORD aLen)
{
	// TODO: Add your game notification handler code here
}