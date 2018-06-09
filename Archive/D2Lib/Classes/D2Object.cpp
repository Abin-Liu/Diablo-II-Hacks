//////////////////////////////////////////////////////////////////////
// D2Object.cpp: implementation of the CD2Object class.
//////////////////////////////////////////////////////////////////////

#include "D2Object.h"

CD2Object::CD2Object()
{
	::memset(&m_fingerprinter, 0, sizeof(D2OBJFINGERPRINTER));
	m_bLockSpawn = FALSE;
	m_bLockDestroy = FALSE;
	InitAttributes(); // Initialization
}

CD2Object::~CD2Object()
{
}

void CD2Object::InitAttributes()
{
	CD2Abstract::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_dwObjectID = 0;
	m_wPositionX = 0;
	m_wPositionY = 0;
	m_wType = 0;
	m_aAffections.RemoveAll();
}

void CD2Object::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Abstract::OnGameJoin(pGame); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
	m_bLockSpawn = FALSE;
}

void CD2Object::OnGameLeave()
{
	CD2Abstract::OnGameLeave(); // Always call base class's method first!!!
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
	m_bLockSpawn = FALSE;
	if (m_dwObjectID != 0)
		NotifyD2Message(OM_DESTROY, (WPARAM)this, MAKELPARAM(m_wPositionX, m_wPositionX));	
}

void CD2Object::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Abstract::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.	
	//////////////////////////////////////////////////////////////////////
	// Object Affection
	//////////////////////////////////////////////////////////////////////
	const DWORD OBJID = GetObjectID();
	if ((aPacket[0] == 0xa8 || aPacket[0] == 0xaa)
		&& aLen >= 8
		&& !::memcmp(&OBJID, aPacket + 2, 4))
	{
		if (m_aAffections.Find(aPacket[7] == -1))
		{
			m_aAffections.Add(aPacket[7]);
			m_aAffections.Sort();			
			OnAffection(aPacket[1], aPacket[7]);
			FormatAndDebugMessage(_T("Object affection: %d, %d"), aPacket[1], aPacket[7]);
			NotifyD2Message(OM_AFFECT, (WPARAM)aPacket[1], (LPARAM)aPacket[7]);
		}	
		return;
	}	

	//////////////////////////////////////////////////////////////////////
	// Object Disaffection
	//////////////////////////////////////////////////////////////////////
	if (aLen >= 7
		&& aPacket[0] == 0xa9
		&& !::memcmp(&OBJID, aPacket + 2, 4))
	{
		const int IDX = m_aAffections.Find(aPacket[6]);
		if (IDX != -1)
		{
			m_aAffections.RemoveAt(IDX);
			OnDisaffection(aPacket[1], aPacket[6]);
			FormatAndDebugMessage(_T("Object disaffection: %d, %d"), aPacket[1], aPacket[6]);
			NotifyD2Message(OM_DISAFFECT, (WPARAM)aPacket[1], (LPARAM)aPacket[6]);
		}
		return;
	}
}

BOOL CD2Object::VerifyFingerprinter(const BYTE *aPacket, DWORD aLen) const
{
	if (!m_fingerprinter.dwLength || !aPacket)
		return FALSE;

	for (DWORD i = m_fingerprinter.dwPosStart; i + m_fingerprinter.dwLength <= aLen && i + m_fingerprinter.dwLength <= m_fingerprinter.dwPosEnd + 1; i++)
	{
		if (!::memcmp(aPacket + i, m_fingerprinter.aData, m_fingerprinter.dwLength))
			return TRUE;
	}

	return FALSE;
}

DWORD CD2Object::GetObjectID() const
{
	return m_dwObjectID;
}

WORD CD2Object::GetPositionX() const
{
	return m_wPositionX;
}

WORD CD2Object::GetPositionY() const
{
	return m_wPositionY;
}

BOOL CD2Object::IsValidObject() const
{
	return GetObjectID() != 0;
}

BOOL CD2Object::SetFingerprinter(LPCD2OBJFINGERPRINTER lpFingerprinter)
{
	if (lpFingerprinter == NULL)
	{
		::memset(&m_fingerprinter, 0, sizeof(D2OBJFINGERPRINTER));
		return FALSE;
	}

	::memcpy(&m_fingerprinter, lpFingerprinter, sizeof(D2OBJFINGERPRINTER));
	return TRUE;
}

void CD2Object::GetFingerprinter(LPD2OBJFINGERPRINTER lpBuffer) const
{
	if (lpBuffer)
		::memcpy(lpBuffer, &m_fingerprinter, sizeof(D2OBJFINGERPRINTER));
}

WORD CD2Object::GetType() const
{
	return m_wType;
}

void CD2Object::LockSpawn(BOOL bLock)
{
	m_bLockSpawn = bLock;
}

BOOL CD2Object::IsSpawnLocked() const
{
	return m_bLockSpawn;
}

void CD2Object::OnAffection(BOOL bPassive, BYTE iAffectionID)
{
}

void CD2Object::OnDisaffection(BOOL bPassive, BYTE iAffectionID)
{
}

BYTE CD2Object::GetAffection(int nIndex) const
{
	return m_aAffections.IsIndexValid(nIndex) ? m_aAffections[nIndex] : 0;
}

BOOL CD2Object::IsAffected(BYTE iAffectionID) const
{
	return m_aAffections.Find(iAffectionID) != -1;
}

void CD2Object::OnSpawn(DWORD dwObjectID, WORD x, WORD y)
{

}

void CD2Object::OnDestroy(DWORD dwObjectID)
{

}

void CD2Object::LockDestroy(BOOL bLock)
{
	m_bLockDestroy = bLock;
}

BOOL CD2Object::IsDestroyLocked() const
{
	return m_bLockDestroy;
}
