//////////////////////////////////////////////////////////////////////
// D2StaticObject.h: interface of the CD2StaticObject class.
//
// This class is used for manipulating static objects in the Diablo II
// game.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __D2STATICOBJECT_H__
#define __D2STATICOBJECT_H__

#include "D2Object.h"

///////////////////////////////////////////////////////////////////////
// CD2StaticObject Class Definition
///////////////////////////////////////////////////////////////////////
class CD2StaticObject : public CD2Object
{
public:

	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CD2StaticObject();
	virtual ~CD2StaticObject();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin(const THISGAMESTRUCT* pGame);
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////

protected:

	///////////////////////////////////////////////////////////////////////
	// Overrides
	///////////////////////////////////////////////////////////////////////
	virtual void OnStaticObjectSpawn(DWORD dwObjectID, WORD x, WORD y); // The object is spawned

	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////

};

#endif // __D2STATICOBJECT_H__