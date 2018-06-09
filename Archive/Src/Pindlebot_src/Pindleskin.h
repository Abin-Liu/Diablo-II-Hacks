//////////////////////////////////////////////////////////////////////
// Pindleskin.h: interface for the CPindleskin class.
//
// A class derived from CD2Unit, used for maintaining Pindleskin 
// states and attributes.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 16th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __PINDLESKIN_H__
#define __PINDLESKIN_H__

#include "..\\D2Lib\\D2Lib.h"
#include "PindleBonusDef.h"

///////////////////////////////////////////////////////////////////////
// Pindleskin Message Definition
///////////////////////////////////////////////////////////////////////
#define PSM_BONUSES		DM_APP + 120 // Pindleskin's bonuses just revealed

class CPindleskin : public CD2Monster
{
public:
	
	CPindleskin();
	virtual ~CPindleskin();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin(const THISGAMESTRUCT* pGame);
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);

	const WORD* GetBonuses() const;
	void InitAttributes(); // Clean all attributes

	// Blocked
	virtual BOOL SetFingerprinter(LPCD2OBJFINGERPRINTER lpFingerprinter) { return FALSE; }

protected:
	virtual void OnAffection(BOOL bPassive, BYTE iAffectionID);
	virtual void OnBonuses(const WORD* aBonuses);

	///////////////////////////////////////////////////////////////////////
	// Operations
	///////////////////////////////////////////////////////////////////////
	static WORD IsPindleskinAura(BYTE iAffectionID);

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	WORD m_aBonuses[PINDLE_MAX_BONUS + 1]; // Pindleskin bonuses + aura
};

#endif // __PINDLESKIN_H__
