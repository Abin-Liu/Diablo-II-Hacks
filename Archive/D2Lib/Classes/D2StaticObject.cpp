//////////////////////////////////////////////////////////////////////
// D2StaticObject.cpp: implementation of the CD2StaticObject class.
//////////////////////////////////////////////////////////////////////

#include "D2StaticObject.h"

CD2StaticObject::CD2StaticObject()
{
}

CD2StaticObject::~CD2StaticObject()
{
}

void CD2StaticObject::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Object::OnGameJoin(pGame); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
}

void CD2StaticObject::OnGameLeave()
{
	CD2Object::OnGameLeave(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
}

void CD2StaticObject::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Object::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	//////////////////////////////////////////////////////////////////////
	// Static Object Spawned
	//////////////////////////////////////////////////////////////////////
	if (!m_bLockSpawn
		&& aLen > 12
		&& aPacket[0] == 0x51
		&& aPacket[1] == 0x02
		&& VerifyFingerprinter(aPacket, aLen))
	{
		::memcpy(&m_dwObjectID, aPacket + 2, 4);
		::memcpy(&m_wType, aPacket + 6, 2);
		::memcpy(&m_wPositionX, aPacket + 8, 2);
		::memcpy(&m_wPositionY, aPacket + 10, 2);
		OnSpawn(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndDebugMessage(_T("Static Object %08X spawned at %04X, %04X"), m_dwObjectID, m_wPositionX, m_wPositionY);
		NotifyD2Message(OM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}
	
	//////////////////////////////////////////////////////////////////////
	// Object is Destroyed by the Game (be Gone from Player's Vision) 
	//////////////////////////////////////////////////////////////////////
	if (!m_bLockDestroy
		&& aLen == 6
		&& aPacket[0] == 0x0a
		&& aPacket[1] == 0x02
		&& !::memcmp(&m_dwObjectID, aPacket + 2, 4))
	{
		// unit is gone from player's vision
		DWORD dwObjectID = m_dwObjectID;
		InitAttributes();
		OnDestroy(dwObjectID);
		DebugMessage(_T("Object destroyed"));
		NotifyD2Message(OM_DESTROY, (WPARAM)dwObjectID, 0);
		return;
	}
}
