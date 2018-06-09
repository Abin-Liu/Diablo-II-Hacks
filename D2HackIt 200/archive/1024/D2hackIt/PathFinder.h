//////////////////////////////////////////////////////////////////////
// PathFinder.h
//
// Diablo II game path finding algorithms. 
//
// I first used Niren7's algorithm in his ABot, then found out it was
// not working properly and was not accurate. Then I got enlightened by
// Ninjai's D2BS code which is better than the former.
//
// Of course a lot of modifications had to be done in order to make this
// class work for this project, but nevertheless I must give enough
// credits to both Niren7 and Ninjai, their code helped me to start
// this class to say the least.
//
// Ustc_tweeg also helped me a lot on completing this algorithm.
//
// Written by Abin(abinn32@yahoo.com)
// Sep 10th, 2004
////////////////////////////////////////////////////////////////////////

#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include "CommonStructs.h"
#include "CollisionMap.h"

class CPathFinder  
{
public:	
	
	CPathFinder(CCollisionMap* pCollisionMap);
	virtual ~CPathFinder();	

	DWORD CalculatePathTo(WORD x, WORD y, LPPATH lpBuffer, int nAdjust = 5); // Calculate path

private:

	BOOL CopyMapData();
	static int GetRedundancy(LPPATH lpPath, const POINT& pos);
	void Block(POINT pos, int nRange);
	void Reset();
	static POINT MapPos2Point(MAPPOS pos);
	BOOL VerifyMemory() const;
	int Distance(const POINT &p1, const POINT &p2);
	BOOL GetBestMove(POINT& rResult, int nAdjust);
	void MakeMap2();

	short** m_graph;	// graph
	CCollisionMap* m_pCollisionMap;
	RECT m_rcBoundary; // reachable index boundary
	POINT m_ptAbsDest; // absolute destination
	POINT m_ptRelDest; // relative destination
};

#endif // __PATHFINDER_H__
