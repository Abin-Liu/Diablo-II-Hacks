//////////////////////////////////////////////////////////////////////
// D2Mercy.cpp: implementation of the CD2Mercy class.
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "D2Mercy.h"

CD2Mercy::CD2Mercy()
{
	InitAttributes();
}

CD2Mercy::~CD2Mercy()
{
}

void CD2Mercy::InitAttributes()
{
	CD2Abstract::InitAttributes();

	// TODO: Add your implementation here.
	
}

void CD2Mercy::OnGameJoin()
{
	CD2Abstract::OnGameJoin();
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	
}

void CD2Mercy::OnGameLeave()
{
	CD2Abstract::OnGameLeave();
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	
}

void CD2Mercy::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeReceived(aPacket, aLen);

	// TODO: Add your implementation here.
	
}

void CD2Mercy::OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeSent(aPacket, aLen);

	// TODO: Add your implementation here.
	
}

void CD2Mercy::OnGameTimerTick()
{
	CD2Abstract::OnGameTimerTick();

	// TODO: Add your implementation here.
	
}