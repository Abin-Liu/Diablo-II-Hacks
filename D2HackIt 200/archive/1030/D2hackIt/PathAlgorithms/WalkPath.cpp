/////////////////////////////////////////////////////////////////////
//
//  WalkPath.cpp 
//                   
//  Ustc_tweeg < ustc_tweeg@tom.com >
//
/////////////////////////////////////////////////////////////////////

#include "WalkPath.h"

#define __SHOW_DEBUG_INFO__ // comment this line if you do not want to output debug messages

#ifndef __IN_DIABLOII__
	#include <stdio.h>
#else
	#include "D2Hackit.h"
#endif

#ifndef __SHOW_DEBUG_INFO__
	#define __PRINTF DummyMsg
#else
	#ifndef __IN_DIABLOII__
		#define __PRINTF printf
	#else
		#define __PRINTF GameErrorf
	#endif
#endif

#define STRAIGHTEN_A 2
#define STRAIGHTEN_B 2
#define STRAIGHTEN_C 2
#define STRAIGHTEN_D 2
#define THICKEN_TAG 50000

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

CWalkPath::CWalkPath(WORD** Map, int MapSizeX, int MapSizeY)
{
	m_Map = Map;
	m_MapSizeX = MapSizeX;
	m_MapSizeY = MapSizeY;

	m_RightWallCount = m_LeftWallCount = 0;
	m_ContinuousCount = m_Count = 0;
	m_LStart = m_RStart = 0;

	m_ThickenWallSuccess = m_FollowWallSuccess = TRUE;
	
	ContinuousPath = new POINT[2000];
	LeftWallPath = new POINT[1000];
	RightWallPath = new POINT[1000];
}

CWalkPath::~CWalkPath()
{
	delete [] ContinuousPath;
	delete [] LeftWallPath;
	delete [] RightWallPath;
}

///////////////////////////////////////////////////////////////////////
// verify map data
///////////////////////////////////////////////////////////////////////

BOOL CWalkPath::IsBarrier(POINT pt)
{
	if( pt.x<0 || pt.y<0 || pt.x >= m_MapSizeX || pt.y >= m_MapSizeY) return TRUE;
	if(m_Map[pt.y][pt.x] % 2) return TRUE;
	return FALSE;
}

BOOL CWalkPath::IsForbidden(POINT pt)
{	
	if(IsBarrier(pt)) return TRUE;
	if(m_Map[pt.y][pt.x]==THICKEN_TAG)	return TRUE;
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////
// thicken the wall
////////////////////////////////////////////////////////////////////////

void CALLBACK CWalkPath::ThickenWallProc(int x, int y, LPARAM pWp) 
{ 
	CWalkPath* p = (CWalkPath*)pWp;

	POINT TempPoint;
	TempPoint.x = x; 
	TempPoint.y = y;

	if( p->IsBarrier(TempPoint) && !p->IsBarrier(p->m_LastLineDot) && p->m_ThickenWallSuccess )
	{
		p->m_Seperate = p->m_LastLineDot;
		p->m_Map[p->m_Seperate.y][p->m_Seperate.x] = THICKEN_TAG;
	}
	else if( !p->IsBarrier(TempPoint) && p->IsBarrier(p->m_LastLineDot) ) 
	{
			p->m_Meet = TempPoint;
			p->m_Map[p->m_Meet.y][p->m_Meet.x] = THICKEN_TAG;
			p->m_LCurrent = p->m_RCurrent = p->m_Seperate;
			p->SaveThickenWallData();
	}

	p->m_LastLineDot = TempPoint;	
	
} 

void CWalkPath::SaveThickenWallData()
{
	for(int i=0; i<1000; i++)//1000
	{
		if(ThickenTheWall(TRUE, m_Meet))
		{
			m_ThickenWallSuccess = TRUE;
			return;
		}
		
		if(ThickenTheWall(FALSE,m_Meet))
		{
			m_ThickenWallSuccess = TRUE;
			return;
		}
	}
	m_ThickenWallSuccess = FALSE;
}

BOOL CWalkPath::ThickenTheWall(BOOL RightWall, POINT ThickenEnd)
{
	POINT NearbyA, NearbyB,TempCurrent;
	int m,n; // thicken-right-wall-nearbya, thicken-left-wall-nearbya
	TempCurrent.x = TempCurrent.y = 0;

	if(RightWall)
	{	
		m = m_RStart;
		for(int i=0; i<8; i++)
		{
			NearbyA.x = m_RCurrent.x + Nearby [m] [0];
			NearbyA.y = m_RCurrent.y + Nearby [m] [1];
			NearbyB.x = m_RCurrent.x + Nearby [Nearby[m][3]] [0];
			NearbyB.y = m_RCurrent.y + Nearby [Nearby[m][3]] [1];

			if( NearbyA.x == ThickenEnd.x && NearbyA.y == ThickenEnd.y) return true;

			if( !IsBarrier(NearbyA) && IsBarrier(NearbyB) )
			{ 
				if(!TempCurrent.x && !TempCurrent.y)
				{
				TempCurrent = NearbyA;
				m_Map[NearbyA.y][NearbyA.x] = THICKEN_TAG;
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
		for(int i=0; i<8; i++)
		{
			NearbyA.x = m_LCurrent.x + Nearby [n] [0];
			NearbyA.y = m_LCurrent.y + Nearby [n] [1];
			NearbyB.x = m_LCurrent.x + Nearby [Nearby[n][2]] [0];
			NearbyB.y = m_LCurrent.y + Nearby [Nearby[n][2]] [1];

			if( NearbyA.x == ThickenEnd.x && NearbyA.y == ThickenEnd.y) return true;

			if( !IsBarrier(NearbyA) && IsBarrier(NearbyB) )
			{
				if(!TempCurrent.x && !TempCurrent.y)
				{
				TempCurrent = NearbyA;
				m_Map[NearbyA.y][NearbyA.x] = THICKEN_TAG;
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

///////////////////////////////////////////////////////////////////////
// follow the rope and follow the wall
///////////////////////////////////////////////////////////////////////

void CALLBACK CWalkPath::WalkPathProc(int x, int y, LPARAM pWp) 
{ 
	CWalkPath* p = (CWalkPath*)pWp;

	POINT TempPoint;
	TempPoint.x = x; 
	TempPoint.y = y;

	if( p->IsForbidden(TempPoint) && !p->IsForbidden(p->m_LastLineDot) && p->m_FollowWallSuccess )
	{
		p->m_Seperate = p->m_LastLineDot;
		__PRINTF("sepearate x=%d y=%d\n",p->m_Seperate.x,p->m_Seperate.y);
	}
	else if(!p->IsForbidden(TempPoint) ) 
	{
		if( p->IsForbidden(p->m_LastLineDot))
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
			{
				ContinuousPath[m_ContinuousCount] = RightWallPath[j];
				m_ContinuousCount++;
				
			}
			m_RightWallCount = m_LeftWallCount = 0;
			m_FollowWallSuccess = TRUE;
			return;
		}
		
		if(FollowTheWall(FALSE,m_Meet))
		{
			for(int j=0; j<m_LeftWallCount; j++)
			{
				ContinuousPath[m_ContinuousCount] = LeftWallPath[j];
				m_ContinuousCount++;
			}
			m_RightWallCount = m_LeftWallCount = 0;
			m_FollowWallSuccess = TRUE;
			return;
		}
	}
	m_RightWallCount = m_LeftWallCount = 0;
	m_FollowWallSuccess = FALSE;
}

void CWalkPath::FollowTheRope(POINT Dot)
{
	ContinuousPath[m_ContinuousCount] = Dot;
	m_ContinuousCount++;
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

			if( !IsForbidden(NearbyA) && IsForbidden(NearbyB) )
			{ 
				if(!TempCurrent.x && !TempCurrent.y)
				{
				TempCurrent = NearbyA;

				if(!IsForbidden(NearbyC))
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
			if( !IsForbidden(NearbyA) && IsForbidden(NearbyB))
			{
				if(!TempCurrent.x && !TempCurrent.y)
				{
				TempCurrent = NearbyA;

				if(!IsForbidden(NearbyC))
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

//////////////////////////////////////////////////////////////////////////////
// straighten the rope
/////////////////////////////////////////////////////////////////////////////

void CWalkPath::StraightenIt(LPPOINT Path, DWORD dwMaxCount)
{
	POINT* TempPathA;
	POINT* TempPathB;
	POINT* TempPathC;

	TempPathA = new POINT[2000];
	TempPathB = new POINT[1000];
	TempPathC = new POINT[500];

	int TempCountA,TempCountB,TempCountC;
	int i,j;

	TempCountA = TempCountB = TempCountC = 0;

	__PRINTF("ContinuousCount %d\n", m_ContinuousCount);

	// first straighten
	for( i=STRAIGHTEN_A; i<m_ContinuousCount; i+=STRAIGHTEN_A )
	{
		if( IsDirect(ContinuousPath[i-STRAIGHTEN_A], ContinuousPath[i]) )
		{
			TempPathA[TempCountA++] = ContinuousPath[i-STRAIGHTEN_A];
		}
		else 
		{
			for(j=i-STRAIGHTEN_A; j<i; j++)
				TempPathA[TempCountA++] = ContinuousPath[j];
		}
	}

	if( IsDirect(ContinuousPath[i-STRAIGHTEN_A], ContinuousPath[m_ContinuousCount-1]) )
		{
			TempPathA[TempCountA++] = ContinuousPath[i-STRAIGHTEN_A];
			TempPathA[TempCountA++] = ContinuousPath[m_ContinuousCount-1];
		}
		else 
		{
			for( j=i-STRAIGHTEN_A; j<m_ContinuousCount; j++)
				TempPathA[TempCountA++] = ContinuousPath[j];
		}

		
	//second straighten
	for( i=STRAIGHTEN_B; i<TempCountA; i+=STRAIGHTEN_B )
	{
		if( IsDirect(TempPathA[i-STRAIGHTEN_B], TempPathA[i]) )
		{
			TempPathB[TempCountB++] = TempPathA[i-STRAIGHTEN_B]; 
		}
		else 
		{
			for(j=i-STRAIGHTEN_B; j<i; j++)
				TempPathB[TempCountB++] = TempPathA[j];
		}
	}

	if( IsDirect(TempPathA[i-STRAIGHTEN_B], TempPathA[TempCountA-1]) )
		{
			TempPathB[TempCountB++] = TempPathA[i-STRAIGHTEN_B];
			TempPathB[TempCountB++] = TempPathA[TempCountA-1];
		}
		else 
		{
			for( j=i-STRAIGHTEN_B; j<TempCountA; j++)
				TempPathB[TempCountB++] = TempPathA[j];
		}

	//third straighten
	for( i=STRAIGHTEN_C; i<TempCountB; i+=STRAIGHTEN_C )
	{
		if( IsDirect(TempPathB[i-STRAIGHTEN_C], TempPathB[i]) )
		{

			TempPathC[TempCountC++] = TempPathB[i-STRAIGHTEN_C];
		}
		else 
		{
			for(j=i-STRAIGHTEN_C; j<i; j++)
				TempPathC[TempCountC++] = TempPathB[j];
		}
	}

	if( IsDirect(TempPathB[i-STRAIGHTEN_C], TempPathB[TempCountB-1]) )
		{
			TempPathC[TempCountC++] = TempPathB[i-STRAIGHTEN_C];
			TempPathC[TempCountC++] = TempPathB[TempCountB-1];
		}
		else 
		{
			for( j=i-STRAIGHTEN_C; j<TempCountB; j++)
				TempPathC[TempCountC++] = TempPathB[j];
		}



	//fourth straighten
	for( i=STRAIGHTEN_D; i<TempCountC; i+=STRAIGHTEN_D )
	{
		if( IsDirect(TempPathC[i-STRAIGHTEN_D], TempPathC[i]) )
		{
			if(m_Count<dwMaxCount)
				Path[m_Count++] = TempPathC[i-STRAIGHTEN_D]; 
		}
		else 
		{
			for(j=i-STRAIGHTEN_D; j<i; j++)
				if(m_Count<dwMaxCount)
					Path[m_Count++] = TempPathC[j];
		}
	}

	if( IsDirect(TempPathC[i-STRAIGHTEN_D], TempPathC[TempCountC-1]) )
		{
			if(m_Count<dwMaxCount)
				Path[m_Count++] =TempPathC[i-STRAIGHTEN_D];
		/*	if(m_Count<dwMaxCount)
				Path[m_Count++] = TempPathC[TempCountC-1];*/
		}
		else 
		{
			for( j=i-STRAIGHTEN_D; j<TempCountC; j++)
				if(m_Count<dwMaxCount)
					Path[m_Count++] = TempPathC[j];
		}

	delete [] TempPathA;
	delete [] TempPathB;
	delete [] TempPathC;
	

}

BOOL CWalkPath::IsDirect(POINT pt1, POINT pt2)
{
	m_Direct = TRUE;
	LineDDA(pt1.x,pt1.y,pt2.x,pt2.y,IsDirectProc,(long)this);

	return m_Direct;
}

void CALLBACK CWalkPath::IsDirectProc(int x, int y, LPARAM lpData) 
{ 
	CWalkPath* p = (CWalkPath*)lpData; 
	POINT TempPoint;

	TempPoint.x = x;
	TempPoint.y = y;

	if (p->IsForbidden(TempPoint)) p->m_Direct = FALSE;
}

///////////////////////////////////////////////////////////////////////
// public function
///////////////////////////////////////////////////////////////////////

int CWalkPath::FindWalkPath (POINT Start, POINT End, LPPOINT Path, DWORD dwMaxCount)
{
	/*
	FILE*fp=fopen("c:\\a.txt","w+");
	for(int i=0; i<m_MapSizeY; i++) 
	{
		for(int j=0; j<m_MapSizeX; j++)
		{
			//for(int k=0; k<m_Count; k++) // Map[i][j] is read-only. This is for display convenience.
			//	if( Path[k].x==j && Path[k].y==i) m_Map[i][j]=-6; // -6+48='*'
				
			if(!m_Map[i][j]) fprintf(fp,"  ");
			else if(m_Map[i][j]==50000) fprintf(fp,"@ ");
			else fprintf(fp,"%c ", m_Map[i][j]+48);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	*/
	
	//if( IsBarrier(End) || IsBarrier(Start) ) return 0; 

	m_LastLineDot = Start;
	LineDDA(Start.x,Start.y,End.x,End.y,ThickenWallProc,(long)this);
	if(!m_ThickenWallSuccess) return 0;

	m_LastLineDot = Start;
	__PRINTF("start x=%d y=%d\n",Start.x,Start.y);
	LineDDA(Start.x,Start.y,End.x,End.y,WalkPathProc,(long)this);
	__PRINTF("end x=%d y=%d\n",End.x,End.y);
	if(!m_FollowWallSuccess) return 0;

	ContinuousPath[m_ContinuousCount++] = End;

	//for(int i=0;i<m_ContinuousCount;i++) Path[m_Count++]=ContinuousPath[i];

	StraightenIt(Path,dwMaxCount);

	return m_Count;
}

//////////////////////////////////////////////////////////////////////////////////////
//   other functions...
//////////////////////////////////////////////////////////////////////////////////////

int DummyMsg(LPCSTR lpszFmt, ...)
{
	return 0; // silent toe screen messages
}



