// D2PacketFilter.h: interface for the CD2PacketFilter class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __D2PACKETFILTER_H__
#define __D2PACKETFILTER_H__

#include <windows.h>

class CD2PacketFilter  
{
public:	
	
	CD2PacketFilter();
	virtual ~CD2PacketFilter();

	void Allow(BYTE iPacketID, BOOL bReceived);
	void Block(BYTE iPacketID, BOOL bReceived);
	void AllowAll(BOOL bReceived);
	void BlockAll(BOOL bReceived);
	BOOL IsAllowed(BYTE iPacketID, BOOL bReceived) const;

	void Reset(BOOL bAllow);
	void SetFilter(const BYTE* pFilter, DWORD dwMaxLen);
	const BYTE* GetFilter() const;

protected:
	BYTE m_filter[256];
};

#endif //
