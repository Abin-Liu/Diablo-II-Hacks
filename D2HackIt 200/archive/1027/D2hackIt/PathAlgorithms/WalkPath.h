/////////////////////////////////////////////////////////////////////////
//
//  WalkPath.h
//
//  Ustc_tweeg < ustc_tweeg@tom.com >
//
/////////////////////////////////////////////////////////////////////////

#ifndef __WALKPATH_H__
#define __WALKPATH_H__

#include <Windows.h>

#define __IN_DIABLOII__ // Comment this line out if not used in game

class CWalkPath  
{
public:	

	CWalkPath(WORD** Map, int MapSizeX, int MapSizeY);
	virtual ~CWalkPath();

	// return number of path points found, including the start and the end point
	// return 0 if pathing fail
	int FindWalkPath (POINT Start, POINT End, LPPOINT Path, DWORD dwMaxCount);

private:

	WORD** m_Map;
	int m_MapSizeX, m_MapSizeY;

	POINT m_Meet,m_Seperate;
	POINT m_LCurrent,m_RCurrent,m_LastLineDot;

	POINT* ContinuousPath;
	POINT* LeftWallPath;
	POINT* RightWallPath;
	int m_ContinuousCount,m_LeftWallCount,m_RightWallCount,m_Count;

	int m_LStart,m_RStart; 

	BOOL m_Direct,m_FollowWallSuccess,m_ThickenWallSuccess;

	BOOL IsBarrier(POINT pt);
	BOOL IsForbidden(POINT pt);
	
	BOOL ThickenTheWall(BOOL RightWall, POINT ThickenEnd);
	void SaveThickenWallData();
	static void CALLBACK ThickenWallProc(int X, int Y, LPARAM lpData);
	
	void FollowTheRope(POINT Dot);
	BOOL FollowTheWall(BOOL RightWall, POINT FollowEnd);
	void SaveFollowWallPath();
	static void CALLBACK WalkPathProc(int X, int Y, LPARAM lpData); 

	void StraightenIt(LPPOINT Path, DWORD dwMaxCount);
	BOOL IsDirect(POINT pt1, POINT pt2);
	static void CALLBACK IsDirectProc(int x, int y, LPARAM lpData);
	
};



#endif // __WALKPATH_H__
