/////////////////////////////////////////////////////////////////////
//
//  WalkPath.cpp 
//                   
//  Ustc_tweeg < ustc_tweeg@tom.com >
//
/////////////////////////////////////////////////////////////////////

#include "WalkPath.h"
#include <stdio.h>

#define STRAIGHTEN_A 20
#define STRAIGHTEN_B 7
#define STRAIGHTEN_C 3

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

CWalkPath::CWalkPath(CCollisionMap* pCollisionMap)
{
	m_pCollisionMap = pCollisionMap;
	
	m_Map = NULL;
	m_MapSizeX = MAP_LIMIT;//MapSizeX;
	m_MapSizeY = MAP_LIMIT;//MapSizeY;

	RightWallCount = LeftWallCount = 0;
	ContinuousCount = DirectCount = Count = 0;
	m_LStart = m_RStart = 0;

	m_FollowWallSuccess = TRUE;

	ContinuousPath = new POINT[2000];
	LeftWallPath = new POINT[1000];
	RightWallPath = new POINT[1000];
	DirectPath = new POINT[50];
}

CWalkPath::~CWalkPath()
{
	delete [] ContinuousPath;
	delete [] LeftWallPath;
	delete [] RightWallPath;
	delete [] DirectPath;
}

///////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////

BOOL CWalkPath::IsBorder(POINT pt)
{
	return ( pt.x<0 || pt.y<0 || pt.x >= m_MapSizeX || pt.y >= m_MapSizeY);
}

void CWalkPath::FollowTheRope(POINT Dot)
{
	ContinuousPath[ContinuousCount] = Dot;
	ContinuousCount++;
}

BOOL CWalkPath::FollowTheWall(BOOL RightWall, POINT FollowEnd)
{
	if (!GetMapData())
		return FALSE;
	
	POINT NearbyA, NearbyB,TempCurrent;
	int m,n; // follow-right-wall-nearbya, follow-left-wall-nearbya
	TempCurrent.x = TempCurrent.y = 0;

	if(RightWall)
	{	
		//printf("right current %d %d\n" , m_RCurrent.x,m_RCurrent.y);
		m = m_RStart;
		for(int i=0; i<8; i++)
		{
			NearbyA.x = m_RCurrent.x + Nearby [m] [0];
			NearbyA.y = m_RCurrent.y + Nearby [m] [1];
			NearbyB.x = m_RCurrent.x + Nearby [Nearby[m][3]] [0];
			NearbyB.y = m_RCurrent.y + Nearby [Nearby[m][3]] [1];

			if( NearbyA.x == FollowEnd.x && NearbyA.y == FollowEnd.y) 
			{
				m_pCollisionMap->Unlock();
				return true;
			}

		//	printf("right nearbya %d %d\n" , NearbyA.x,NearbyA.y);			

			if( !(m_Map[NearbyA.y][NearbyA.x] % 2) && ( (m_Map[NearbyB.y][NearbyB.x] % 2) || IsBorder(NearbyB)) )
			{
				if(!TempCurrent.x && !TempCurrent.y)
				{
				TempCurrent = NearbyA;
				RightWallPath[RightWallCount] = NearbyA;
				RightWallCount++;
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
	//	printf("left current %d %d\n" , m_LCurrent.x,m_LCurrent.y);
		for(int i=0; i<8; i++)
		{
			NearbyA.x = m_LCurrent.x + Nearby [n] [0];
			NearbyA.y = m_LCurrent.y + Nearby [n] [1];
			NearbyB.x = m_LCurrent.x + Nearby [Nearby[n][2]] [0];
			NearbyB.y = m_LCurrent.y + Nearby [Nearby[n][2]] [1];

			if( NearbyA.x == FollowEnd.x && NearbyA.y == FollowEnd.y)
			{
				m_pCollisionMap->Unlock();
				return true;
			}

		//	printf("left nearbya %d %d\n" , NearbyA.x,NearbyA.y);

			if( !(m_Map[NearbyA.y][NearbyA.x] % 2) && ( (m_Map[NearbyB.y][NearbyB.x] % 2) || IsBorder(NearbyB)) )
			{
				if(!TempCurrent.x && !TempCurrent.y)
				{
				TempCurrent = NearbyA;
				LeftWallPath[LeftWallCount] = NearbyA;
				LeftWallCount++;
				m_LStart = Nearby[n][5];
				}
			}
			n = Nearby[n][3];
		}
		m_LCurrent = TempCurrent;
		TempCurrent.x = TempCurrent.y = 0;
		
	}
	m_pCollisionMap->Unlock();
	return false;	
}

void CALLBACK CWalkPath::RopeProc(int x, int y, LPARAM pWp) 
{ 
	CWalkPath* p = (CWalkPath*)pWp;

	POINT TempPoint;
	TempPoint.x = x; 
	TempPoint.y = y;


	if((p->m_Map[y][x] % 2) && !(p->m_Map[p->m_LastLineDot.y][p->m_LastLineDot.x] % 2) && p->m_FollowWallSuccess )
	{
		p->m_Seperate = p->m_LastLineDot;
		printf("sepearate x=%d y=%d\n",p->m_Seperate.x,p->m_Seperate.y);
	}
	else if(!(p->m_Map[y][x] %2 )) 
	{
		if( (p->m_Map[p->m_LastLineDot.y][p->m_LastLineDot.x] % 2) )
		{
			p->m_Meet = TempPoint;printf("meet x=%d y=%d\n",p->m_Meet.x,p->m_Meet.y);
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
			for(int j=0; j<RightWallCount; j++)
			{
				ContinuousPath[ContinuousCount] = RightWallPath[j];
				ContinuousCount++;
				
			}
			RightWallCount = LeftWallCount = 0;
			m_FollowWallSuccess = TRUE;
			return;
		}
		
		if(FollowTheWall(FALSE,m_Meet))
		{
			for(int j=0; j<LeftWallCount; j++)
			{
				ContinuousPath[ContinuousCount] = LeftWallPath[j];
				ContinuousCount++;
			}
			RightWallCount = LeftWallCount = 0;
			m_FollowWallSuccess = TRUE;
			return;
		}
	}
	RightWallCount = LeftWallCount = 0;
	m_FollowWallSuccess = FALSE;
}

void CWalkPath::StraightenIt(POINT Path[])
{
	POINT TempPathA[2000],TempPathB[1000];
	int TempCountA,TempCountB,Temp;
	int i,j,k;

	TempCountA = TempCountB = 0;

	printf("ContinuousCount %d\n", ContinuousCount);

	for( i=STRAIGHTEN_A; i<ContinuousCount; i+=STRAIGHTEN_A )
	{
		if( IsDirect(ContinuousPath[i-STRAIGHTEN_A], ContinuousPath[i]) )
		{
			Temp = DirectCount-1;
			for(j=0; j<Temp; j++) 
			{
				TempPathA[TempCountA] = DirectPath[j];
				TempCountA++;
			}
			DirectCount = 0;
		}
		else 
		{
			for(j=i-STRAIGHTEN_A; j<i; j++)
			{
				TempPathA[TempCountA] = ContinuousPath[j];
				TempCountA++;
			}
		}
	}

	for( k=i-STRAIGHTEN_A; k<ContinuousCount; k++)
	{
		TempPathA[TempCountA] = ContinuousPath[k];
		TempCountA++;
	}

	for( i=STRAIGHTEN_B; i<TempCountA; i+=STRAIGHTEN_B)
		if( IsDirect(TempPathA[i-STRAIGHTEN_B],TempPathA[i]))
		{
			TempPathB[TempCountB] = TempPathA[i-STRAIGHTEN_B];
			TempCountB++;
			DirectCount = 0; //不清会overflow
		}
		else
		{
			for(j=i-STRAIGHTEN_B; j<i; j++)
			{
				TempPathB[TempCountB] = TempPathA[j];
				TempCountB++;
			}
		}

	for( k=i-STRAIGHTEN_B; k<TempCountA; k++)
	{
		TempPathB[TempCountB] = TempPathA[k];
		TempCountB++;
	}

	for( i=STRAIGHTEN_C; i<TempCountB; i+=STRAIGHTEN_C)
		if( IsDirect(TempPathB[i-STRAIGHTEN_C],TempPathB[i]))
		{
			Path[Count] = TempPathB[i-STRAIGHTEN_C];
			Count++;
			DirectCount = 0; //不清会overflow
		}
		else
		{
			for(j=i-STRAIGHTEN_C; j<i; j++)
			{
				Path[Count] = TempPathB[j];
				Count++;
			}
		}

	for( k=i-STRAIGHTEN_C; k<TempCountB; k++)
	{
		Path[Count] = TempPathB[k];
		Count++;
	}

}

BOOL CWalkPath::IsDirect(POINT pt1, POINT pt2)
{
	if (!GetMapData())
		return FALSE;

	m_Direct = TRUE;
	LineDDA(pt1.x,pt1.y,pt2.x,pt2.y,DirectProc,(long)this);
	m_pCollisionMap->Unlock();
	if( !m_Direct) DirectCount = 0;
	return m_Direct;
}

void CALLBACK CWalkPath::DirectProc(int x, int y, LPARAM lpData) 
{ 
	CWalkPath* p = (CWalkPath*)lpData;

	p->DirectPath[p->DirectCount].x = x;
	p->DirectPath[p->DirectCount].y = y;
	p->DirectCount++;

	if (p->m_Map[y][x] % 2) p->m_Direct = FALSE;
}

///////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////

DWORD CWalkPath::FindWalkPath (POINT Start, POINT End, LPPATH lpPath)
{
	if (lpPath == NULL
		|| m_pCollisionMap == NULL
		|| !m_pCollisionMap->IsValid()
		|| !m_pCollisionMap->IsValidLocation((WORD)Start.x, (WORD)Start.y)
		|| !m_pCollisionMap->IsValidLocation((WORD)End.x, (WORD)End.y))
		return 0;

	lpPath->dwNodeCount = 0;
	lpPath->posStart.x = (WORD)Start.x;
	lpPath->posStart.y = (WORD)Start.y;
	lpPath->posEnd.x = (WORD)End.x;
	lpPath->posEnd.y = (WORD)End.y;

	m_pCollisionMap->MapLocationToIndex(Start);
	m_pCollisionMap->MapLocationToIndex(End);

	m_Start = m_LastLineDot = Start;
	m_End = End;
	
	if( IsBorder(End)|| IsBorder(Start)) return 0;

	if (!GetMapData())
		return 0;

	if( (m_Map[End.y][End.x] % 2) || (m_Map[Start.y][Start.x] % 2) )
	{
		m_pCollisionMap->Unlock();
		return 0;
	}
	
	//printf("start x=%d y=%d\n",m_Start.x,m_Start.y);
	
	LineDDA(Start.x,Start.y,End.x,End.y,RopeProc,(long)this);
	m_pCollisionMap->Unlock();
	//printf("end x=%d y=%d\n",m_End.x,m_End.y);

	if(!m_FollowWallSuccess) return 0;

	POINT aPath[PATH_MAX_NODE] = {0};

	StraightenIt(aPath);

	lpPath->dwNodeCount = Count;

	for (int i = 0; i < Count; i++)
	{
		m_pCollisionMap->IndexToMapLocation(aPath[i]);
		lpPath->aPathNodes[i].x = (WORD)aPath[i].x;
		lpPath->aPathNodes[i].y = (WORD)aPath[i].y;
	}

	return Count;
}

BOOL CWalkPath::GetMapData()
{
	m_Map = NULL;
	if (m_pCollisionMap == NULL || !m_pCollisionMap->IsValid())
		return FALSE;

	if (!m_pCollisionMap->Lock(10000))
		return FALSE;

	m_Map = m_pCollisionMap->GetMapData();
	if (m_Map)
		return TRUE; // need to unloack later

	m_pCollisionMap->Unlock();
	return FALSE;
}
