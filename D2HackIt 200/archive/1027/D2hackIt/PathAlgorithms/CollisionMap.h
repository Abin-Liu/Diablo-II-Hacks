//////////////////////////////////////////////////////////////////////
// CollisionMap.h: interface for the CCollisionMap class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __COLLISIONMAP_H__
#define __COLLISIONMAP_H__

#include <windows.h>
#include "..\\definitions.h"
#include "..\\Utilities\\SyncObj.h"
#include "..\\utilities\\ArrayEx.h"

#define MAP_DATA_INVALID	-1    // Invalid
#define MAP_DATA_FILLED		11111 // Filled gaps
#define MAP_DATA_CLEANED	11110 // Cleaned for start/end positions

typedef CArrayEx<DWORD, DWORD> DwordArray;

////////////////////////////////////////////////////////////////
// The CCollisionMap class. Synchronized access required.
////////////////////////////////////////////////////////////////
class CCollisionMap : public CSyncObj
{
public:		
	
	////////////////////////////////////////////////////////////
	// Constructor & Destructor
	////////////////////////////////////////////////////////////
	CCollisionMap();
	virtual ~CCollisionMap();

	////////////////////////////////////////////////////////////
	// Operations
	////////////////////////////////////////////////////////////
	BOOL CreateMap(); // Create the map data
	BOOL DumpMap(LPCSTR lpszFilePath, const LPPOINT lpPath, DWORD dwCount) const; // Dump map data into a disk file

	////////////////////////////////////////////////////////////
	// Attributes & Operations
	////////////////////////////////////////////////////////////
	POINT GetMapOrigin() const; // Map origin point(top-left)
	SIZE GetMapSize() const; // map size
	WORD GetMapData(long x, long y, BOOL bAbs) const; // Retrieve map data at a particular location
	BOOL IsValid() const; // Map data verification
	BOOL IsValidAbsLocation(WORD x, WORD y) const; // Map location verification
	SIZE CopyMapData(WORD** ppBuffer, int cx, int cy) const;
	
	////////////////////////////////////////////////////////////
	// Convertions
	////////////////////////////////////////////////////////////
	void AbsToRelative(POINT& pt) const; // Convert an absolute map location into a graph index
	void RelativeToAbs(POINT& pt) const; // Convert a graph index into an absolute map location
	static void MakeBlank(WORD** ppMap, SIZE czMapSize, POINT pos);

	////////////////////////////////////////////////////////////
	// Only Used by D2Hackit! Do NOT Call Them!!!
	////////////////////////////////////////////////////////////
	void OnMapChanged(BYTE iNewMapID); // Called by D2Hackit when map changes. Do not call this function manually!
	void DestroyMap(); // Called by D2Hackit. Do not call this function manually!

private:	
	
	////////////////////////////////////////////////////////////
	// Private Methods
	////////////////////////////////////////////////////////////
	BOOL AllocateMap();
	void FreeMap();
	BOOL BuildMapData();
	void Search(RoomOther *ro, UnitPlayer* pPlayer, DwordArray& aSkip);
	void AddCollisionData(const DrlgColl* pCol);	
	char IsMarkPoint(const POINT& ptPlayer, int x, int y, const LPPOINT lpPath, DWORD dwCount) const;
	BOOL IsGap(int x, int y) const;
	void FillGaps();

	////////////////////////////////////////////////////////////
	// Member Data
	////////////////////////////////////////////////////////////
	WORD** m_ppMap; // The map
	BYTE m_iCurMap; // Current map ID
	BOOL m_bValid; // Valid flag
	POINT m_ptLevelOrigin; // level top-left
	SIZE m_czLevelSize; // level size
	CArrayEx<WORD, WORD> m_aCollisionTypes;
};

#endif // __COLLISIONMAP_H__
