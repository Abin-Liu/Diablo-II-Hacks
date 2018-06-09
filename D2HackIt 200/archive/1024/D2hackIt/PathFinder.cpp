///////////////////////////////////////////////////////////
// PathFinder.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "PathFinder.h"
#include "D2HackIt.h"

#define TP_RANGE		35		// Maximum teleport range
#define RANGE_INVALID	10000  // invalid range flag

/////////////////////////////////////////////////////////////////////
// Path Finding Result
/////////////////////////////////////////////////////////////////////
enum {   PATH_FAIL = 0,     // Failed, error occurred or no available path
		 PATH_CONTINUE,	    // Path OK, destination not reached yet
		 PATH_REACHED };    // Path OK, destination reached(Path finding completed successfully)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPathFinder::CPathFinder(CCollisionMap* pCollisionMap)
{
	m_pCollisionMap = pCollisionMap;

	// Allocates memory
	m_graph = new short*[MAP_LIMIT];

	if (!m_graph)
		return;
	
	for (int i = 0; i < MAP_LIMIT; i++)
		m_graph[i] = new short[MAP_LIMIT];

	Reset();
}

CPathFinder::~CPathFinder()
{
	// Free heap
	if (m_graph)
	{
		for (int i = 0; i < MAP_LIMIT; i++)
		{
			if (m_graph[i])
				delete [] m_graph[i];
		}
		delete [] m_graph;
	}
}

void CPathFinder::Reset()
{
	if (VerifyMemory())
	{
		for (int i = 0; i < MAP_LIMIT; i++)
		{
			for (int j = 0; j < MAP_LIMIT; j++)
				m_graph[i][j] = 0;
		}
	}

	memset(&m_ptAbsDest, 0, sizeof(POINT));
	memset(&m_ptRelDest, 0, sizeof(POINT));	
	memset(&m_rcBoundary, 0, sizeof(RECT));
}

void CPathFinder::MakeMap2()
{	
	// convert the graph into a distance table
	for (int j = m_rcBoundary.top; j < m_rcBoundary.bottom; j++)	
	{
		for (int i = m_rcBoundary.left; i < m_rcBoundary.right; i++)
		{
			if (m_graph[i][j])
				m_graph[i][j] = GetDistance(i, j, m_ptRelDest.x, m_ptRelDest.y);
			else
				m_graph[i][j] = RANGE_INVALID;
		}
	}

	if (::PtInRect(&m_rcBoundary, m_ptRelDest))
	{
		m_graph[m_ptRelDest.x][m_ptRelDest.y] = 1;
	}	
}


/////////////////////////////////////////////////////////////////////
// The "Get Best Move" Algorithm
//
// Originally developed by Niren7, Modified by Abin
/////////////////////////////////////////////////////////////////////
BOOL CPathFinder::GetBestMove(POINT& pos, int nAdjust)
{	
	if(Distance(m_ptAbsDest,pos) <= TP_RANGE)
	{
		pos = m_ptAbsDest;
		return PATH_REACHED; // we reached the destination
	}

	m_pCollisionMap->MapLocationToIndex(pos);
	if (!::PtInRect(&m_rcBoundary, pos))
		return PATH_FAIL; // fail

	Block(pos, nAdjust);

	POINT p, best;
	int value = RANGE_INVALID;

	for (p.x = pos.x - TP_RANGE; p.x <= pos.x + TP_RANGE; p.x++)
	{
		for (p.y = pos.y - TP_RANGE; p.y <= pos.y + TP_RANGE; p.y++)
		{			
			if (!::PtInRect(&m_rcBoundary, p))
				continue;
			
			if (m_graph[p.x][p.y] < value && Distance(p, pos) <= TP_RANGE)
			{
				value = m_graph[p.x][p.y];
				best = p;					
			}			
		}
	}

	if (value >= RANGE_INVALID)
		return PATH_FAIL; // no path at all	
	
	pos = best;
	Block(pos, nAdjust);	
	m_pCollisionMap->IndexToMapLocation(pos);
	return PATH_CONTINUE; // ok but not reached yet
}

int CPathFinder::Distance(const POINT &p1, const POINT &p2)
{
	return GetDistance(p1.x, p1.y, p2.x, p2.y);
}

DWORD CPathFinder::CalculatePathTo(WORD x, WORD y, LPPATH lpBuffer, int nAdjust)
{
	if (lpBuffer == NULL
		|| x == 0 || y == 0
		|| !VerifyMemory()
		|| m_pCollisionMap == NULL
		|| !m_pCollisionMap->CreateMap()
		|| !m_pCollisionMap->IsValidLocation(x, y))
		return 0;
	
	memset(lpBuffer, 0, sizeof(PATH));
	lpBuffer->posStart = GetPosition();
	if (lpBuffer->posStart.x == 0 || lpBuffer->posStart.y == 0)
		return 0;

	Reset();
	m_pCollisionMap->GetRechableIndexBoundary(&m_rcBoundary);
	CopyMapData();
	
	m_ptAbsDest.x = x;
	m_ptAbsDest.y = y;

	m_ptRelDest = m_ptAbsDest;
	m_pCollisionMap->MapLocationToIndex(m_ptRelDest);

	MakeMap2();	

	POINT pos;
	pos.x = (short)lpBuffer->posStart.x;
	pos.y = (short)lpBuffer->posStart.y;

	lpBuffer->dwNodeCount = 0;

	BOOL bOK = FALSE;
	int nRes = GetBestMove(pos, nAdjust);
	while (nRes != PATH_FAIL && lpBuffer->dwNodeCount < PATH_MAX_NODE)
	{
		// Reached?
		if (nRes == PATH_REACHED)
		{
			bOK = TRUE;
			lpBuffer->aPathNodes[lpBuffer->dwNodeCount].x = x;
			lpBuffer->aPathNodes[lpBuffer->dwNodeCount].y = y;
			lpBuffer->dwNodeCount++;
			break; // Finished
		}

		// Perform a redundancy check
		int nRedundancy = GetRedundancy(lpBuffer, pos);
		if (nRedundancy == -1)
		{
			// no redundancy
			lpBuffer->aPathNodes[lpBuffer->dwNodeCount].x = (WORD)pos.x;
			lpBuffer->aPathNodes[lpBuffer->dwNodeCount].y = (WORD)pos.y;
			lpBuffer->dwNodeCount++;
		}
		else
		{
			// redundancy found, discard all redundant steps
			lpBuffer->dwNodeCount = nRedundancy + 1;
			lpBuffer->aPathNodes[lpBuffer->dwNodeCount].x = (WORD)pos.x;
			lpBuffer->aPathNodes[lpBuffer->dwNodeCount].y = (WORD)pos.y;
		}	

		nRes = GetBestMove(pos, nAdjust);
	}	

	if (!bOK)
	{
		lpBuffer->dwNodeCount = 0;
	}
	else
	{
		lpBuffer->posEnd.x = x;
		lpBuffer->posEnd.y = y;
	}
	
	//m_pCollisionMap->DumpMap("c:\\dump.txt", lpBuffer); // path printing

	return lpBuffer->dwNodeCount;
}

BOOL CPathFinder::VerifyMemory() const
{
	if (m_graph == NULL)
		return FALSE;

	for (int i = 0; i < MAP_LIMIT; i++)
	{
		if (m_graph[i] == NULL)
			return FALSE;
	}
	return TRUE;
}

POINT CPathFinder::MapPos2Point(MAPPOS pos)
{
	POINT pt;
	pt.x = (long)pos.x;
	pt.y = (long)pos.y;
	return pt;
}

void CPathFinder::Block(POINT pos, int nRange)
{
	nRange = max(nRange, 1);

	for (int i = pos.x - nRange; i < pos.x + nRange; i++)
	{
		for (int j = pos.y - nRange; j < pos.y + nRange; j++)
		{
			if (i >= 0 && i < MAP_LIMIT && j >= 0 && j < MAP_LIMIT)
				m_graph[i][j] = RANGE_INVALID;
		}
	}
}

int CPathFinder::GetRedundancy(LPPATH lpPath, const POINT &pos)
{
	// step redundancy check
	if (lpPath == NULL || lpPath->dwNodeCount == 0)
		return -1;

	for (DWORD i = 0; i < lpPath->dwNodeCount; i++)
	{
		if (GetDistance(lpPath->aPathNodes[i].x, lpPath->aPathNodes[i].y, pos.x, pos.y) <= TP_RANGE / 2)
			return i;
	}

	return -1;
}

BOOL CPathFinder::CopyMapData()
{
	if (m_pCollisionMap == NULL || !m_pCollisionMap->IsValid())
		return FALSE;

	// Always lock the mutex when using GetMapData!
	if (!m_pCollisionMap->Lock(10000))
		return FALSE;
	
	const WORD** pMapData = m_pCollisionMap->GetMapData();
	if (pMapData == NULL)
	{
		m_pCollisionMap->Unlock();
		return FALSE;
	}

	for (int i = 0; i < MAP_LIMIT; i++)
	{
		for (int j = 0; j < MAP_LIMIT; j++)
			m_graph[i][j] = (pMapData[i][j] % 2) ? 0 : 1;
	}

	m_pCollisionMap->Unlock();
	return TRUE;
}
