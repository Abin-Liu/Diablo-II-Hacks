/////////////////////////////////////////////////////////////////////
//
//  WalkPath.cpp 
//                   
//  Ustc_tweeg < ustc_tweeg@tom.com >
//
/////////////////////////////////////////////////////////////////////

#include "WalkPath.h"
#include <Math.h>

//#define __SHOW_DEBUG_INFO__ // comment this line if you do not want to output debug messages

#ifndef __SHOW_DEBUG_INFO__
	#define __PRINTF DummyMsg
#else
	#ifndef __IN_DIABLOII__
		#define __PRINTF printf
	#else
		#define __PRINTF GameErrorf
	#endif
#endif

#define STRAIGHTEN_A 5
#define STRAIGHTEN_B 2
#define MAX_WALK_RANGE	20

// deta-x, deta-y, counterclockwise, clockwise, right-wall-next-point, left-wall-next-point
static const int Nearby[8][6] =	{   { 1, 0,   1, 7,   6, 2 },
									{ 1, 1,   2, 0,   0, 0 },
									{ 0, 1,   3, 1,   0, 0 },
									{-1, 1,   4, 2,   2, 6 },
									{-1, 0,   5, 3,   2, 6 },
									{-1,-1,   6, 4,   4, 4 },
									{ 0,-1,   7, 5,   4, 4 },
									{ 1,-1,   0, 6,   6, 2 } };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int DummyMsg(LPCSTR lpszFmt, ...)
{
	return 0; // silent toe screen messages
}


CWalkPath::CWalkPath(WORD** Map, int MapSizeX, int MapSizeY)
{

	m_Map = Map;

	m_MapSizeX = MapSizeX;
	m_MapSizeY = MapSizeY;

	m_RightWallCount = m_LeftWallCount = 0;
	m_Count = 0;
	m_LStart = m_RStart = 0;

	LeftWallPath = new POINT[1000];
	RightWallPath = new POINT[1000];

	m_FollowWallSuccess = TRUE;
}

CWalkPath::~CWalkPath()
{
	delete [] LeftWallPath;
	delete [] RightWallPath;

}

///////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////

BOOL CWalkPath::IsBorder(POINT pt)
{
	return ( pt.x<0 || pt.y<0 || pt.x >= m_MapSizeX || pt.y >= m_MapSizeY);
}

BOOL CWalkPath::IsBarrier(POINT pt)
{
	return (m_Map[pt.x][pt.y] % 2);
}

void CWalkPath::FollowTheRope(POINT Dot)
{
	ContinuousPath.Add(Dot);
}

BOOL CWalkPath::FollowTheWall(BOOL RightWall, POINT FollowEnd)
{
	POINT NearbyC,NearbyA, NearbyB,TempCurrent;
	int m,n; // follow-right-wall-nearbya, follow-left-wall-nearbya
	TempCurrent.x = TempCurrent.y = 0;

	if(RightWall)
	{	
		//__PRINTF("right current %d %d\n" , m_RCurrent.x,m_RCurrent.y);
		m = m_RStart;
		for(int i=0; i<8; i++)
		{
			NearbyC.x = m_RCurrent.x + Nearby [Nearby[m][2]] [0];
			NearbyC.y = m_RCurrent.y + Nearby [Nearby[m][2]] [1];
			NearbyA.x = m_RCurrent.x + Nearby [m] [0];
			NearbyA.y = m_RCurrent.y + Nearby [m] [1];
			NearbyB.x = m_RCurrent.x + Nearby [Nearby[m][3]] [0];
			NearbyB.y = m_RCurrent.y + Nearby [Nearby[m][3]] [1];

			if( NearbyA.x == FollowEnd.x && NearbyA.y == FollowEnd.y) return true;

			//__PRINTF("right nearbya %d %d\n" , NearbyA.x,NearbyA.y);

			if( !(IsBorder(NearbyA) || IsBarrier(NearbyA))
				&& ( IsBorder(NearbyB) || IsBarrier(NearbyB)) )
			{ 
				if(!TempCurrent.x && !TempCurrent.y)
				{
				TempCurrent = NearbyA;

				if(!(IsBorder(NearbyC) || IsBarrier(NearbyC)))
					RightWallPath[m_RightWallCount++] = NearbyC;
				else 
					RightWallPath[m_RightWallCount++] = NearbyA;

				m_RStart = Nearby[m][4];
				}
			}
			m = Nearby[m][2];
		}
		m_RCurrent = TempCurrent;
		TempCurrent.x = TempCurrent.y = 0;
	}
	else
	{	
		n = m_LStart;
		//__PRINTF("left current %d %d\n" , m_LCurrent.x,m_LCurrent.y);
		for(int i=0; i<8; i++)
		{
			NearbyC.x = m_LCurrent.x + Nearby [Nearby[n][3]] [0];
			NearbyC.y = m_LCurrent.y + Nearby [Nearby[n][3]] [1];
			NearbyA.x = m_LCurrent.x + Nearby [n] [0];
			NearbyA.y = m_LCurrent.y + Nearby [n] [1];
			NearbyB.x = m_LCurrent.x + Nearby [Nearby[n][2]] [0];
			NearbyB.y = m_LCurrent.y + Nearby [Nearby[n][2]] [1];

			if( NearbyA.x == FollowEnd.x && NearbyA.y == FollowEnd.y) return true;

			//__PRINTF("left nearbya %d %d\n" , NearbyA.x,NearbyA.y);
			if( !(IsBorder(NearbyA) || IsBarrier(NearbyA))
				&& ( IsBorder(NearbyB) || IsBarrier(NearbyB)))
			{
				if(!TempCurrent.x && !TempCurrent.y)
				{
				TempCurrent = NearbyA;

				if(!(IsBorder(NearbyC) || IsBarrier(NearbyC)))
					LeftWallPath[m_LeftWallCount++] = NearbyC;
				else 
					LeftWallPath[m_LeftWallCount++] = NearbyA;
				m_LStart = Nearby[n][5];
				}
			}
			n = Nearby[n][3];
		}
		m_LCurrent = TempCurrent;
		TempCurrent.x = TempCurrent.y = 0;
		
	}
	return false;
}

void CALLBACK CWalkPath::FindPathProc(int x, int y, LPARAM pWp) 
{ 
	CWalkPath* p = (CWalkPath*)pWp;

	POINT TempPoint;
	TempPoint.x = x; 
	TempPoint.y = y;

	if( p->IsBarrier(TempPoint) && !p->IsBarrier(p->m_LastLineDot) && p->m_FollowWallSuccess )
	{
		p->m_Seperate = p->m_LastLineDot;
		__PRINTF("sepearate x=%d y=%d\n",p->m_Seperate.x,p->m_Seperate.y);
	}
	else if(!p->IsBarrier(TempPoint) ) 
	{
		if( p->IsBarrier(p->m_LastLineDot))
		{
			p->m_Meet = TempPoint;__PRINTF("meet x=%d y=%d\n",p->m_Meet.x,p->m_Meet.y);
			p->m_LCurrent = p->m_RCurrent = p->m_Seperate;
			p->SaveFollowWallPath();
		}
		if(p->m_FollowWallSuccess) p->FollowTheRope(TempPoint);
	}
	p->m_LastLineDot = TempPoint;		
	
} 

void CWalkPath::SaveFollowWallPath()
{
	for(int i=0; i<1000; i++)//1000
	{
		if(FollowTheWall(TRUE, m_Meet))
		{
			for(int j=0; j<m_RightWallCount; j++)
				ContinuousPath.Add( RightWallPath[j]);

			m_RightWallCount = m_LeftWallCount = 0;
			m_FollowWallSuccess = TRUE;

			return;
		}
		
		if(FollowTheWall(FALSE,m_Meet))
		{
			for(int j=0; j<m_LeftWallCount; j++)
				ContinuousPath.Add( LeftWallPath[j]);

			m_RightWallCount = m_LeftWallCount = 0;
			m_FollowWallSuccess = TRUE;

			return;
		}
	}

	m_RightWallCount = m_LeftWallCount = 0;
	m_FollowWallSuccess = FALSE;
}

void CWalkPath::StraightenThePath(LPPOINT Path, DWORD dwMaxCount)
{

	CArrayEx<POINT, const POINT> TempPathA;

	int TempCountB;
	int i,j;

	TempCountB = 0;

	__PRINTF("ContinuousCount %d\n", ContinuousPath.GetSize());

	// first straighten
	for( i=STRAIGHTEN_A; i<ContinuousPath.GetSize(); i+=STRAIGHTEN_A )
	{
		if( IsDirect(ContinuousPath[i-STRAIGHTEN_A], ContinuousPath[i]) )
		{
			TempPathA.Add(ContinuousPath[i-STRAIGHTEN_A]);
		}
		else 
		{
			for(j=i-STRAIGHTEN_A; j<i; j++)
				TempPathA.Add(ContinuousPath[j]);
		}
	}

	if( IsDirect(ContinuousPath[i-STRAIGHTEN_A], ContinuousPath.GetLast()) )
		{
			TempPathA.Add(ContinuousPath[i-STRAIGHTEN_A]);
			TempPathA.Add(ContinuousPath.GetLast());
		}
		else 
		{
			for( j=i-STRAIGHTEN_A; j<ContinuousPath.GetSize(); j++)
				TempPathA.Add(ContinuousPath[j]);
		}

	//2nd straighten
	for( i=STRAIGHTEN_B; i<TempPathA.GetSize(); i+=STRAIGHTEN_B )
	{
		if( IsDirect(TempPathA[i-STRAIGHTEN_B], TempPathA[i]) )
		{
			if(m_Count<dwMaxCount)
				Path[m_Count++] = TempPathA[i-STRAIGHTEN_B]; 
		}
		else 
		{
			for(j=i-STRAIGHTEN_B; j<i; j++)
				if(m_Count<dwMaxCount)
					Path[m_Count++] = TempPathA[j];
		}
	}

	if( IsDirect(TempPathA[i-STRAIGHTEN_B], TempPathA.GetLast()))
		{
			if(m_Count<dwMaxCount)
				Path[m_Count++] =TempPathA[i-STRAIGHTEN_B];
	/*		if(m_Count<dwMaxCount)
				Path[m_Count++] = TempPathA[TempCountA-1];*/
		}
		else 
		{
			for( j=i-STRAIGHTEN_B; j<TempPathA.GetSize(); j++)
				if(m_Count<dwMaxCount)
					Path[m_Count++] = TempPathA[j];
		}

}

BOOL CWalkPath::IsDirect(POINT pt1, POINT pt2)
{
	m_Direct = TRUE;
	LineDDA(pt1.x,pt1.y,pt2.x,pt2.y,IsDirectProc,(long)this);

	return m_Direct;
}

long CWalkPath::GetDistance(long x1, long y1, long x2, long y2)
{
	return (long) :: sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

void CALLBACK CWalkPath::IsDirectProc(int x, int y, LPARAM lpData) 
{ 
	CWalkPath* p = (CWalkPath*)lpData; 
	POINT TempPoint;

	TempPoint.x = x;
	TempPoint.y = y;

	if (p->IsBarrier(TempPoint)) p->m_Direct = FALSE;
}

///////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////

int CWalkPath::FindWalkPath (POINT Start, POINT End, LPPOINT Path, DWORD dwMaxCount)
{
	m_Start = m_LastLineDot = Start;
	m_End = End;
	
	if( IsBorder(End)|| IsBorder(Start)) return 0;
	
	__PRINTF("start x=%d y=%d\n",m_Start.x,m_Start.y);
	LineDDA(Start.x, Start.y, End.x, End.y, FindPathProc, (long)this);
	FindPathProc(End.x, End.y, (long)this);
	__PRINTF("end x=%d y=%d\n",m_End.x,m_End.y);

	if(!m_FollowWallSuccess) return 0;

	StraightenThePath(Path,dwMaxCount);
	__PRINTF("after Straighten: %d\n", m_Count);
	m_Count = FurtherStraighten(Path, m_Count);
	__PRINTF("after FurtherStraighten: %d\n", m_Count);
	return m_Count;
}

//////////////////////////////////////////////////////////////////////
// FurtherStraighten
//--------------------------------------------------------------------
// Further removal of redundant steps.
//////////////////////////////////////////////////////////////////////

DWORD CWalkPath::FurtherStraighten(LPPOINT lpPath, DWORD dwMaxCount)
{
	if (lpPath == NULL || dwMaxCount < 3)
		return dwMaxCount;

	CArrayEx<POINT, const POINT> aTemp;
	aTemp.SetSize(dwMaxCount);
	for (int t = 0; t < dwMaxCount; t++)
		aTemp[t] = lpPath[t];

	// straighten the path
	for (int nStart = 0; nStart + 2 < aTemp.GetSize(); nStart++)
	{
		for (int nEnd = aTemp.GetSize() - 1; nEnd > nStart + 1; nEnd--)
		{
			if (GetDistance(aTemp[nStart].x, aTemp[nStart].y, aTemp[nEnd].x, aTemp[nEnd].y) <= MAX_WALK_RANGE
				&& IsDirect(aTemp[nStart], aTemp[nEnd]))
			{
				aTemp.RemoveAt(nStart + 1, nEnd - nStart - 1);
				break;
			}
		}
	}

	// output
	if (aTemp.GetSize() < (int)dwMaxCount)
	{
		for (int i = 0; i < aTemp.GetSize(); i++)
			lpPath[i] = aTemp[i];
	}

	return aTemp.GetSize();
}
