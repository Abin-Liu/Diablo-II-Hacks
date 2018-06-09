//////////////////////////////////////////////////////////////////////
// D2Object.h: interface of the CD2Object class.
//
// This class is used for manipulating generic objects in the Diablo II
// game, such as static objects, npc's, monsters, etc. This is an ADT
// also.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __D2OBJECT_H__
#define __D2OBJECT_H__

#include "D2Abstract.h"
#include "Definition\\D2ObjFingerPrinter.h"

///////////////////////////////////////////////////////////////////////
// CD2Object Class Definition
///////////////////////////////////////////////////////////////////////
class CD2Object : public CD2Abstract
{
public:		
	
	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CD2Object();
	virtual ~CD2Object();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin(const THISGAMESTRUCT* pGame) = 0;
	virtual void OnGameLeave() = 0;
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen) = 0;

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	virtual BOOL IsValidObject() const; // TRUE if the object's ID is greater than 0, FALSE otherwise
	virtual DWORD GetObjectID() const; // Retrieve the object's ID (Unique in a game)
	virtual WORD GetPositionX() const; // Retrieve object's position x.
	virtual WORD GetPositionY() const; // Retrieve the object's position y.
	virtual WORD GetType() const; // Retrieve the object's type
	virtual void GetFingerprinter(LPD2OBJFINGERPRINTER lpBuffer) const; // Retrieve fingerprinter data
	virtual BOOL IsSpawnLocked() const; // Is the object being prevented from spawning?
	virtual BOOL IsDestroyLocked() const;
	BYTE GetAffection(int nIndex) const; // Retrieve affections by index
	BOOL IsAffected(BYTE iAffectionID) const; // Determines whether the object is affected by a particular affection
	
	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	virtual void InitAttributes(); // Clean all attributes
	virtual BOOL SetFingerprinter(LPCD2OBJFINGERPRINTER lpFingerprinter); // Set fingerprinter data
	virtual void LockSpawn(BOOL bLock); // Lock/unlock object spawning
	virtual void LockDestroy(BOOL bLock); // Lock/unlock object destroying

protected:		

	///////////////////////////////////////////////////////////////////////
	// Overrides
	///////////////////////////////////////////////////////////////////////
	virtual void OnSpawn(DWORD dwObjectID, WORD x, WORD y);
	virtual void OnDestroy(DWORD dwObjectID);
	virtual void OnAffection(BOOL bPassive, BYTE iAffectionID);
	virtual void OnDisaffection(BOOL bPassive, BYTE iAffectionID);

	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	BOOL VerifyFingerprinter(const BYTE* aPacket, DWORD aLen) const; // Verify the object's fingerprinter

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	DWORD m_dwObjectID; // Object ID, unique in a game
	WORD m_wType; // Object's type
	WORD m_wPositionX; // Object's current location X
	WORD m_wPositionY; // Object's current location X
	BOOL m_bLockSpawn; // Prevent the object from spawning
	BOOL m_bLockDestroy; // Prevent the object from being destroyed
	CArrayEx<BYTE, BYTE> m_aAffections;

private:

	///////////////////////////////////////////////////////////////////////
	// Private Member Data
	///////////////////////////////////////////////////////////////////////
	D2OBJFINGERPRINTER m_fingerprinter; // Fingerprinter
};

#endif // __D2OBJECT_H__