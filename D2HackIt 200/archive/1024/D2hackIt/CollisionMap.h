//////////////////////////////////////////////////////////////////////
// CollisionMap.h: interface for the CCollisionMap class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __COLLISIONMAP_H__
#define __COLLISIONMAP_H__

#include <windows.h>
#include "CommonStructs.h"
#include "definitions.h"
#include "Utilities\\SyncObj.h"
#include "utilities\\ArrayEx.h"

#define MAP_LIMIT	1000
#define MAP_DATA_INVALID	-1
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
	BOOL DumpMap(LPCSTR lpszFilePath, LPCPATH lpPath) const; // Dump map data into a disk file

	////////////////////////////////////////////////////////////
	// Attributes
	////////////////////////////////////////////////////////////
	POINT GetMapOrigin() const; // Map origin point(top-left)
	WORD GetMapData(WORD x, WORD y) const; // Retrieve map data at a particular location
	const WORD** GetMapData() const; // Direct access, use it carefully
	BOOL IsValid() const; // Map data verification
	BOOL IsValidLocation(WORD x, WORD y) const; // Map location verification
	BOOL GetRechableMapBoundary(LPRECT lpBuffer) const; // Reachable boundary rectangle(absolute map)
	BOOL GetRechableIndexBoundary(LPRECT lpBuffer) const; // Reachable boundary rectangle(graph index)
	
	////////////////////////////////////////////////////////////
	// Convertions
	////////////////////////////////////////////////////////////
	void MapLocationToIndex(POINT& pt) const; // Convert an absolute map location into a graph index
	void IndexToMapLocation(POINT& pt) const; // Convert a graph index into an absolute map location

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
	char IsMarkPoint(int x, int y, LPCPATH lpPath) const;

	////////////////////////////////////////////////////////////
	// Member Data
	////////////////////////////////////////////////////////////
	WORD** m_aMap; // The map
	BYTE m_iCurMap; // Current map ID
	BOOL m_bValid; // Valid flag
	POINT m_ptLevelLeftTop; // level top-left
	POINT m_leftTop;	// effective left-top
	POINT m_rightBottom; // effective right-bottom
};

#endif // __COLLISIONMAP_H__
