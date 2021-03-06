//////////////////////////////////////////////////////////////////////
// D2Monster.h: interface of the CD2Monster class.
//
// CD2Monster, derived from CD2Unit, is used for manipulating monsters'
// attributes and states.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __D2MONSTER_H__
#define __D2MONSTER_H__

#include "D2Unit.h"

// Calculate 0x80 based HP percentage
#define CALC80BASEDHPPERCENT(x) ((BYTE)((double)(x) * 100.0 / 128.0))

///////////////////////////////////////////////////////////////////////
// Monster Corpse Struct
///////////////////////////////////////////////////////////////////////
typedef struct tagMonsterCorpse
{
	DWORD dwCorpseID;	// Corpse ID
	WORD x;				// Position x
	WORD y;				// Position y
} MONSTERCORPSE, *LPMONSTERCORPSE;
typedef const tagMonsterCorpse* LPCMONSTERCORPSE;
BOOL operator==(const MONSTERCORPSE& lhs, const MONSTERCORPSE& rhs);
BOOL operator>(const MONSTERCORPSE& lhs, const MONSTERCORPSE& rhs);


///////////////////////////////////////////////////////////////////////
// CD2Monster Class Definition
///////////////////////////////////////////////////////////////////////
class CD2Monster : public CD2Unit
{
public:

	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CD2Monster();
	virtual ~CD2Monster();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin(const THISGAMESTRUCT* pGame);
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	BYTE GetHPPercent() const; // Monster's current HP percentage (0 to 100)
	BOOL IsAlive() const; // Whether the object is alive
	BOOL IsCorpse() const; // Whether the object's corpse has appeared	

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	virtual void InitAttributes(); // Clean all attributes

protected:

	///////////////////////////////////////////////////////////////////////
	// Overrides
	///////////////////////////////////////////////////////////////////////
	virtual void OnHPPercentUp(BYTE iOldPercent, BYTE iNewPercent); // Monster's HP percent increased
	virtual void OnHPPercentDown(BYTE iOldPercent, BYTE iNewPercent); // Monster's HP percent decreased
	virtual void OnAttack(WORD x, WORD y, DWORD dwVictimID); // Monster attacked some targets
	virtual void OnUnderAttack(WORD x, WORD y, DWORD dwAttackerID); // Monster is under attack
	virtual void OnDeath(); // Monster died
	virtual void OnCorpseAppeared(DWORD dwCorpseID, WORD x, WORD y); // Monster's corpse appeared
	virtual void OnRevived(BOOL bByOther, WORD x, WORD y);

	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	BOOL m_bIsAlive; // Whether the Monster is alive
	BOOL m_bIsCorpse; // Whether the Monster's corpse has appeared
	BYTE m_iHPPercent; // Monster HP remaining percent
};

#endif // __D2MONSTER_H__