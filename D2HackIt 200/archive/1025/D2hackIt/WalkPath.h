/////////////////////////////////////////////////////////////////////////
//
//  WalkPath.h
//
//  Labyrinth Pathing Algorithm by Ustc_tweeg 
//
//  The algorithm is basically "follow the rope" and "follow the wall".
//  The destination should be known but not necessarily spawned on the wall.
//  Read-only map is enough. ^^
//
//  Ustc_tweeg < ustc_tweeg@tom.com >
//
/////////////////////////////////////////////////////////////////////////

#ifndef __WALKPATH_H__
#define __WALKPATH_H__

#include <Windows.h>
#include "CollisionMap.h"

class CWalkPath  
{
public:	

	CWalkPath(CCollisionMap* pCollisionMap);
	virtual ~CWalkPath();

	// return number of path points found, including the start and the end point
	// return 0 if pathing fail
	DWORD FindWalkPath (POINT Start, POINT End, LPPOINT lpBuffer, DWORD dwMaxCount);

private:
	BOOL IsBlank(long x, long y) const;

	//const WORD** m_Map;
	CCollisionMap* m_pCollisionMap;
	int m_MapSizeX, m_MapSizeY;

	POINT m_Start,m_End,m_Meet,m_Seperate;
	POINT m_LCurrent,m_RCurrent,m_LastLineDot;

	//POINT ContinuousPath[2000],LeftWallPath[1000],RightWallPath[1000],DirectPath[50];
	int ContinuousCount,LeftWallCount,RightWallCount,DirectCount,Count;

	POINT* ContinuousPath;
	POINT* LeftWallPath;
	POINT* RightWallPath;
	POINT* DirectPath;

	int m_LStart,m_RStart; 

	BOOL m_Direct,m_FollowWallSuccess;

	BOOL IsBorder(POINT pt);
	BOOL IsDirect(POINT pt1, POINT pt2);
	void FollowTheRope(POINT Dot);
	BOOL FollowTheWall(BOOL RightWall, POINT FollowEnd);
	void SaveFollowWallPath();
	void StraightenIt(LPPOINT lpBuffer, DWORD dwMaxCount);
	static void CALLBACK RopeProc(int X, int Y, LPARAM lpData); 
	static void CALLBACK DirectProc(int x, int y, LPARAM lpData);
};



#endif // __WALKPATH_H__
