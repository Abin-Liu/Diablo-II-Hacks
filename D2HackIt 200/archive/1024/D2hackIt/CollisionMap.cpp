// CollisionMap.cpp: implementation of the CCollisionMap class.
//
//////////////////////////////////////////////////////////////////////

#include "CollisionMap.h"
#include "d2functions.h"
#include "D2HackIt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCollisionMap::CCollisionMap()
{
	m_aMap = NULL;
	m_iCurMap = 0x00;
	m_bValid = FALSE;
	::memset(&m_ptLevelLeftTop, 0, sizeof(POINT));
	::memset(&m_leftTop, 0, sizeof(POINT));
	::memset(&m_rightBottom, 0, sizeof(POINT));
}

CCollisionMap::~CCollisionMap()
{
	FreeMap();
}

BOOL CCollisionMap::AllocateMap()
{	
	if (m_aMap)
		return m_bValid;

	m_bValid = FALSE;
	m_aMap = new WORD*[MAP_LIMIT];
	if (m_aMap == NULL)
		return FALSE;

	for (int i = 0; i < MAP_LIMIT; i++)
	{
		m_aMap[i] = new WORD[MAP_LIMIT];
		if (m_aMap[i] == NULL)
			return FALSE;

		for (int j = 0; j < MAP_LIMIT; j++)
			m_aMap[i][j] = MAP_DATA_INVALID;
	}

	m_bValid = TRUE;
	return TRUE;
}

void CCollisionMap::FreeMap()
{
	m_bValid = FALSE;
	::memset(&m_ptLevelLeftTop, 0, sizeof(POINT));
	::memset(&m_leftTop, 0, sizeof(POINT));
	::memset(&m_rightBottom, 0, sizeof(POINT));
	if (m_aMap)
	{
		for (int i = 0; i < MAP_LIMIT; i++)
		{
			if (m_aMap[i])
				delete [] m_aMap[i];
		}

		delete [] m_aMap;
		m_aMap = NULL;
	}
}

void CCollisionMap::OnMapChanged(BYTE iNewMapID)
{
	Lock();
	if (iNewMapID != m_iCurMap)
	{
		m_iCurMap = iNewMapID;
		FreeMap();
	}
	Unlock();
}

void CCollisionMap::AddCollisionData(const DrlgColl* pCol)
{
	if (pCol == NULL)
		return;

	int x = pCol->nPosGameX - m_ptLevelLeftTop.x * 5;
	int y = pCol->nPosGameY - m_ptLevelLeftTop.y * 5;
	int cx = pCol->nSizeGameX;
	int cy = pCol->nSizeGameY;

	if (x < 0 || x + cx > MAP_LIMIT || y < 0 || y + cy > MAP_LIMIT)
	{
		return;
	}	
	
	int nLimitX = x + cx;
	int nLimitY = y + cy;
	
	WORD* p = pCol->pMapStart;
	for (int j = y; j < nLimitY; j++)		
	{
		for (int i = x; i < nLimitX; i++)
		{
			m_aMap[i][j] = *p;			

			if ((*p % 2) == 0)
			{			
				// record reach-able area edges
				if (m_leftTop.x > i)
					m_leftTop.x = i;
				if (m_leftTop.y > j)
					m_leftTop.y = j;

				if (m_rightBottom.x < i + 1)
					m_rightBottom.x = i + 1;
				if (m_rightBottom.y < j + 1)
					m_rightBottom.y = j + 1;
			}			

			p++;
		}
	}
}

BOOL CCollisionMap::IsValidLocation(WORD x, WORD y) const
{
	Lock();

	if (!m_bValid || x == 0 || y == 0)
	{
		Unlock();
		return FALSE;
	}

	BOOL bResult = x >= m_leftTop.x && x < m_rightBottom.x && y >= m_leftTop.y && y < m_rightBottom.y;
	Unlock();

	return bResult;
}

WORD CCollisionMap::GetMapData(WORD x, WORD y) const
{
	Lock();

	if (!m_bValid || !IsValidLocation(x, y))
	{
		Unlock();
		return MAP_DATA_INVALID;
	}

	// convert from absolute coordinates to relative coordinates
	int nx = x, ny = y;
	nx -= (int)m_ptLevelLeftTop.x * 5;
	ny -= (int)m_ptLevelLeftTop.y * 5;

	WORD wResult = MAP_DATA_INVALID;

	if (nx >= 0 && nx < MAP_LIMIT && ny >= 0 && ny < MAP_LIMIT)
		wResult = m_aMap[nx][ny];
	Unlock();
	return wResult;
}

BOOL CCollisionMap::GetRechableMapBoundary(LPRECT lpBuffer) const
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(RECT));	

	Lock();
	if (!m_bValid)
	{
		Unlock();
		return FALSE;
	}

	lpBuffer->left = m_leftTop.x;
	lpBuffer->top = m_leftTop.y;
	lpBuffer->right = m_rightBottom.x;
	lpBuffer->bottom = m_rightBottom.y;
	Unlock();

	return lpBuffer->right > lpBuffer->left && lpBuffer->bottom > lpBuffer->top;
}

BOOL CCollisionMap::BuildMapData()
{
	UnitPlayer *pPlayer = D2CLIENT_GetPlayerUnit();
	if(!pPlayer)
		return FALSE;
	
	Room *pRoom1 = D2COMMON_GetRoomFromUnit(pPlayer);
	if(!pRoom1)
		return FALSE;	

	RoomOther *pRoom2 = pRoom1->ptRoomOther;
	if(!pRoom2) 
		return FALSE;

	DrlgLevel *pLevel = pRoom2->ptDrlgLevel;
	if(!pLevel)
		return FALSE;
	
	FreeMap();
	
	if (!AllocateMap())
	{
		return FALSE;
	}	

	m_ptLevelLeftTop.x = pLevel->LevelX;
	m_ptLevelLeftTop.y = pLevel->LevelY;

	m_leftTop.x = 1000;
	m_leftTop.y = 1000;
	
	DwordArray aSkip;
	Search(pRoom2, pPlayer, aSkip);

	//GameInfof("rel %d %d, %d %d", m_leftTop.x, m_leftTop.y, m_rightBottom.x, m_rightBottom.y);

	IndexToMapLocation(m_leftTop);
	IndexToMapLocation(m_rightBottom);

	//GameInfof("abs %04X %04X, %04X %04X", m_leftTop.x, m_leftTop.y, m_rightBottom.x, m_rightBottom.y);
	return TRUE;
}

void CCollisionMap::Search(RoomOther *ro, UnitPlayer* pPlayer, DwordArray &aSkip)
{
	if (!ro || !pPlayer)
		return;

	if(ro->ptDrlgLevel->LevelNo != m_iCurMap)
		return;

	if (aSkip.Find((DWORD)ro) != -1)
		return;

	BOOL add_room=FALSE;
	if(!ro->pRoom)
	{
		add_room=TRUE;
		D2COMMON_AddRoomData(pPlayer->ptAct, m_iCurMap, ro->xPos, ro->yPos, pPlayer);
	}

	aSkip.Add((DWORD)ro);

	if (ro->pRoom)
		AddCollisionData(ro->pRoom->pColl);
	
	RoomOther **n = ro->ptList;
	for(int i=0;i<ro->nRoomList;i++)
	{
		Search(n[i], pPlayer, aSkip);
	}
	
	if(add_room)
	{
		D2COMMON_RemoveRoomData(pPlayer->ptAct, m_iCurMap, ro->xPos, ro->yPos, pPlayer);
	}
}

BOOL CCollisionMap::CreateMap()
{
	Lock();
	if (m_aMap == NULL)
		BuildMapData();
	BOOL bOK = m_bValid;
	Unlock();
	return bOK;
}

BOOL CCollisionMap::IsValid() const
{
	Lock();
	BOOL bValid = m_bValid;
	Unlock();
	return bValid;
}

void CCollisionMap::DestroyMap()
{
	Lock();
	FreeMap();
	Unlock();
}

const WORD** CCollisionMap::GetMapData() const
{
	return IsValid() ? (const WORD**)m_aMap : NULL;
}

POINT CCollisionMap::GetMapOrigin() const
{
	Lock();
	POINT pt = m_ptLevelLeftTop;
	Unlock();
	return pt;
}

void CCollisionMap::MapLocationToIndex(POINT &pt) const
{
	POINT ptLevelOrigin = GetMapOrigin();
	pt.x -= ptLevelOrigin.x * 5;
	pt.y -= ptLevelOrigin.y * 5;
}

void CCollisionMap::IndexToMapLocation(POINT &pt) const
{
	POINT ptLevelOrigin = GetMapOrigin();
	pt.x += ptLevelOrigin.x * 5;
	pt.y += ptLevelOrigin.y * 5;
}

BOOL CCollisionMap::GetRechableIndexBoundary(LPRECT lpBuffer) const
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(RECT));

	RECT rect = { 0 };
	if (!GetRechableMapBoundary(&rect))
		return FALSE;

	POINT pt;

	pt.x = rect.left;
	pt.y = rect.top;
	MapLocationToIndex(pt);
	lpBuffer->left = pt.x;
	lpBuffer->top = pt.y;

	pt.x = rect.right;
	pt.y = rect.bottom;
	MapLocationToIndex(pt);
	lpBuffer->right = pt.x;
	lpBuffer->bottom = pt.y;

	return lpBuffer->right > lpBuffer->left && lpBuffer->bottom > lpBuffer->top;	
}

BOOL CCollisionMap::DumpMap(LPCSTR lpszFilePath, LPCPATH lpPath) const
{
	if (lpszFilePath == NULL)
		return FALSE;

	FILE *fp = fopen(lpszFilePath, "w+");
	if(fp == NULL )
		return FALSE;	
	
	Lock();
	if (!m_bValid || m_aMap == NULL)
	{
		Unlock();
		return FALSE;
	}
	
	POINT pt1 = m_leftTop;
	POINT pt2 = m_rightBottom;

	MapLocationToIndex(pt1);
	MapLocationToIndex(pt2);
	
	// make the picture border
	if (pt1.x)
		pt1.x--;
	if (pt1.y)
		pt1.y--;

	if (pt2.x < MAP_LIMIT - 1)
		pt2.x++;
	if (pt2.y < MAP_LIMIT - 1)
		pt2.y++;
	
	for (int y = pt1.y; y < pt2.y; y++)
	{			
		for (int x = pt1.x; x < pt2.x; x++)
		{
			char ch = IsMarkPoint(x, y, lpPath);
			if (ch == 0)
				ch = (m_aMap[x][y] % 2) == 0 ? ' ' : 'X';

			fprintf(fp, "%C", ch); // X - unreachable
		}

		fprintf(fp, "%c", '\n');
	}

	Unlock();	
	fclose(fp);

	return TRUE;
}

char CCollisionMap::IsMarkPoint(int x, int y, LPCPATH lpPath) const
{
	if (lpPath == NULL)
		return 0;

	POINT pt;
	pt.x = x;
	pt.y = y;
	IndexToMapLocation(pt);
	
	if (lpPath->posStart.x == pt.x && lpPath->posStart.y == pt.y)
		return 'P';

	if (lpPath->posEnd.x == pt.x && lpPath->posEnd.y == pt.y)
		return 'E';

	for (DWORD i = 0; i < lpPath->dwNodeCount; i++)
	{
		if (lpPath->aPathNodes[i].x == pt.x && lpPath->aPathNodes[i].y == pt.y)
			return '*';
	}

	return 0;
}
