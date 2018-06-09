// MyRelayer.h: interface for the CMyRelayer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MYRELAYER_H__
#define __MYRELAYER_H__

#include "D2PacketRelayer.h"
#include "SBItemConfig.h"

// Derived from CD2PacketRelayer
class CMyRelayer : public CD2PacketRelayer  
{
public:
	BYTE CalcBear(BYTE iFlag);
	BYTE CalcWolf(BYTE iFlag);
	BYTE CalcPuffLevel(const BYTE* aPacket, DWORD aLen);
	CMyRelayer();
	virtual ~CMyRelayer();

protected:
	// Below two methods are pure virtual in base class "CD2PacketRelayer",
	// so we must implement them in this class.
	virtual void OnClientStart() {}
	virtual void OnClientStop() {}
	virtual void OnGameJoin(const struct tagPlayerInfo * p) {}
	virtual void OnGameLeave() {}
	virtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen);
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);

	WORD m_wHPAdd;
	WORD m_wManaAdd;
	WORD m_wHPMult;
	WORD m_wManaMult;

	SBITEMCONFIG m_config;
};

#endif // __MYRELAYER_H__
