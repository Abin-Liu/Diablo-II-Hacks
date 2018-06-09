//////////////////////////////////////////////////////////////////////
// Packet.cpp: implementation of the CSendingPacket class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "Packet.h"
#include "D2HackIt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPacket::CPacket(const BYTE* aPacket, DWORD aLen, DWORD dwDelay)
{
	m_pPacket = NULL;
	m_dwLen = 0;
	m_dwDelay = 0;
	SetPacket(aPacket, aLen, dwDelay);
}

CPacket::CPacket(const CPacket& rhs)
{
	m_pPacket = NULL;
	m_dwLen = 0;
	m_dwDelay = 0;

	SetPacket(rhs.m_pPacket, rhs.m_dwLen, rhs.m_dwDelay);
}

CPacket::~CPacket()
{
	DestroyPacket();
}

BYTE* CPacket::GetPacket()
{
	return m_pPacket;
}

const BYTE* CPacket::GetPacket() const
{
	return (const BYTE*)m_pPacket;
}

DWORD CPacket::GetLength() const
{
	return m_dwLen;
}

const CPacket& CPacket::operator=(const CPacket& rhs)
{
	if (this != &rhs)
		SetPacket(rhs.m_pPacket, rhs.m_dwLen, rhs.m_dwDelay);
	return *this;	
}

void CALLBACK FreePacketPtr(CPacket*& ptr)
{
	if (ptr)
	{
		ptr->DestroyPacket();
		delete ptr;
		ptr = NULL;
	}
}

DWORD CPacket::GetDelay() const
{
	return m_dwDelay;
}

BOOL CPacket::IsValidPacket() const
{
	return m_pPacket != NULL && m_dwLen > 0;
}

void CPacket::DestroyPacket()
{
	if (m_pPacket)
	{
		delete [] m_pPacket;
		m_pPacket = NULL;
	}
	m_dwLen = 0;
	m_dwDelay = 0;
}

void CPacket::SetPacket(const BYTE *aPacket, DWORD aLen, DWORD dwDelay)
{
	DestroyPacket();
	
	if (aPacket && aLen)
	{
		m_dwLen = aLen;
		m_pPacket = new BYTE[aLen];
		::memcpy(m_pPacket, aPacket, aLen);
		m_dwDelay = dwDelay;
	}	
}

BOOL CPacket::SendToServer() const
{
	if (!IsValidPacket())
		return FALSE;

	return GameSendPacketToServer((BYTE*)m_pPacket, m_dwLen);
}

BOOL CPacket::SendToGame() const
{
	if (!IsValidPacket())
		return FALSE;

	return GameSendPacketToGame((BYTE*)m_pPacket, m_dwLen);
}
