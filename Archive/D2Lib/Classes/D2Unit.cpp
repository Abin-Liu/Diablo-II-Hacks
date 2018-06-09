//////////////////////////////////////////////////////////////////////
// D2Unit.cpp: implementation of the CD2Unit class.
//////////////////////////////////////////////////////////////////////

#include "D2Unit.h"

CD2Unit::CD2Unit()
{
	InitAttributes(); // Initialization
}

CD2Unit::~CD2Unit()
{
}

void CD2Unit::InitAttributes()
{
	CD2Object::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_bIsMoving = FALSE;	
}

void CD2Unit::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Object::OnGameJoin(pGame); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	
}

void CD2Unit::OnGameLeave()
{
	CD2Object::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	
}

void CD2Unit::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Object::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.

	//////////////////////////////////////////////////////////////////////
	// Unit Spawned
	//////////////////////////////////////////////////////////////////////
	if (!m_bLockSpawn
		&& aPacket[0] == 0xac
		&& aLen >= 13
		&& VerifyFingerprinter(aPacket, aLen))
	{
		// unit spawned
		InitAttributes();
		::memcpy(&m_dwObjectID, aPacket + 1, 4);
		::memcpy(&m_wType, aPacket + 5, 2);
		::memcpy(&m_wPositionX, aPacket + 7, 2);
		::memcpy(&m_wPositionY, aPacket + 9, 2);
		
		OnSpawn(m_dwObjectID, m_wPositionX, m_wPositionY);
		FormatAndDebugMessage(_T("Unit ID %08X spawned at %04X, %04X"), m_dwObjectID, m_wPositionX, m_wPositionY);
		NotifyD2Message(OM_SPAWN, (WPARAM)m_dwObjectID, MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Uint is Destroyed by the Game (be Gone from Player's Vision) 
	//////////////////////////////////////////////////////////////////////
	if (!m_bLockDestroy
		&& aLen == 6
		&& aPacket[0] == 0x0a
		&& aPacket[1] == 0x01
		&& !::memcmp(&m_dwObjectID, aPacket + 2, 4))
	{
		// unit is gone from player's vision
		DWORD dwObjectID = m_dwObjectID;
		InitAttributes();
		OnDestroy(dwObjectID);
		DebugMessage(_T("Unit destroyed"));
		NotifyD2Message(OM_DESTROY, (WPARAM)dwObjectID, 0);
		return;
	}
	
	//////////////////////////////////////////////////////////////////////
	// Unit Starts Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x67 // start moving
		&& aLen == 16
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{		
		WORD x = 0;
		WORD y = 0;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		m_bIsMoving = TRUE;
		OnStartMove(m_wPositionX, m_wPositionY, x, y);
		FormatAndDebugMessage(_T("Unit is moving towards %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(UM_STARTMOVE, MAKEWPARAM(m_wPositionX, m_wPositionY), MAKELPARAM(x, y));
		return;
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Ends Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x6d
		&& aLen == 10
		&& !::memcmp(&m_dwObjectID, aPacket + 1, 4))
	{
		// object just reached this location
		WORD x = m_wPositionX;
		WORD y = m_wPositionY;		
		::memcpy(&m_wPositionX, aPacket + 5, 2);
		::memcpy(&m_wPositionY, aPacket + 7, 2);
		m_bIsMoving = FALSE;
		OnEndMove(x, y, m_wPositionX, m_wPositionY);
		FormatAndDebugMessage(_T("Unit has moved to %04X, %04X"), m_wPositionX, m_wPositionY);
		NotifyD2Message(UM_ENDMOVE, MAKEWPARAM(x, y), MAKELPARAM(m_wPositionX, m_wPositionY));
		return;
	}	
}

void CD2Unit::OnStartMove(WORD wFromX, WORD wFromY, WORD wToX, WORD wToY)
{
}

void CD2Unit::OnEndMove(WORD wFromX, WORD wFromY, WORD wToX, WORD wToY)
{
}

BOOL CD2Unit::IsMoving() const
{
	return m_bIsMoving;
}