// D2PacketFilter.cpp: implementation of the CD2PacketFilter class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "D2PacketFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PACKET_ALLOW_NONE	0x00
#define PACKET_ALLOW_RCV	0x01
#define PACKET_ALLOW_SND	0x02

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2PacketFilter::CD2PacketFilter()
{
	Reset(TRUE);
}

CD2PacketFilter::~CD2PacketFilter()
{
}

void CD2PacketFilter::BlockAll(BOOL bReceived)
{
	for (int i = 0; i < 256; i++)
		m_filter[i] &= (bReceived ? ~PACKET_ALLOW_RCV : ~PACKET_ALLOW_SND);
}

void CD2PacketFilter::AllowAll(BOOL bReceived)
{
	for (int i = 0; i < 256; i++)
		m_filter[i] |= (bReceived ? PACKET_ALLOW_RCV : PACKET_ALLOW_SND);
}

void CD2PacketFilter::Block(BYTE iPacketID, BOOL bReceived)
{
	m_filter[iPacketID] &= (bReceived ? ~PACKET_ALLOW_RCV : ~PACKET_ALLOW_SND);
}

void CD2PacketFilter::Allow(BYTE iPacketID, BOOL bReceived)
{
	m_filter[iPacketID] |= (bReceived ? PACKET_ALLOW_RCV : PACKET_ALLOW_SND);
}

BOOL CD2PacketFilter::IsAllowed(BYTE iPacketID, BOOL bReceived) const
{
	return m_filter[iPacketID] & (bReceived ? PACKET_ALLOW_RCV : PACKET_ALLOW_SND);
}

const BYTE* CD2PacketFilter::GetFilter() const
{
	return m_filter;
}

void CD2PacketFilter::SetFilter(const BYTE *pFilter, DWORD dwMaxLen)
{
	if (pFilter && dwMaxLen)
		::memcpy(m_filter, pFilter, min(dwMaxLen, 256));
}

void CD2PacketFilter::Reset(BOOL bAllow)
{
	if (bAllow)
		::memset(m_filter, PACKET_ALLOW_RCV | PACKET_ALLOW_SND, 256);
	else
		::memset(m_filter, 0, 256);
}
