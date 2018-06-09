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
	m_aMap = new WORD[MAP_LIMIT * MAP_LIMIT];
	if (m_aMap == NULL)
		return FALSE; // out of memory

	::memset(m_aMap, -1, sizeof(WORD) * MAP_LIMIT * MAP_LIMIT); // 2 MB
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
		const long OFFSET = j * MAP_LIMIT;
		for (int i = x; i < nLimitX; i++)
		{
			m_aMap[OFFSET + i] = *p;			

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

BOOL CCollisionMap::IsValidAbsLocation(WORD x, WORD y) const
{
	if (!m_bValid || x == 0 || y == 0)
		return FALSE;

	return x >= m_leftTop.x && x < m_rightBottom.x && y >= m_leftTop.y && y < m_rightBottom.y;
}

WORD CCollisionMap::GetMapData(long x, long y, BOOL bAbs) const
{
	if (!m_bValid)
		return MAP_DATA_INVALID;

	if (!Lock(500))
		return MAP_DATA_INVALID;

	if (bAbs)
	{
		x -= m_ptLevelLeftTop.x * 5;
		y -= m_ptLevelLeftTop.y * 5;
	}

	if (x < 0 || x >= MAP_LIMIT || y < 0 || y >= MAP_LIMIT)
	{
		Unlock();
		return MAP_DATA_INVALID;
	}

	WORD wData = m_aMap[y * MAP_LIMIT + x];
	Unlock();
	return wData;
}

BOOL CCollisionMap::GetRechableBoundary(LPRECT lpBuffer, BOOL bAbs) const
{
	if (!m_bValid || lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(RECT));
	
	POINT pt1 = m_leftTop;
	POINT pt2 = m_rightBottom;

	if (!bAbs)
	{
		AbsToRelative(pt1);
		AbsToRelative(pt2);
	}

	lpBuffer->left = pt1.x;
	lpBuffer->top = pt1.y;
	lpBuffer->right = pt2.x;
	lpBuffer->bottom = pt2.y;

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

	RelativeToAbs(m_leftTop);
	RelativeToAbs(m_rightBottom);

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
	
	Unlock();
	return m_bValid;
}

BOOL CCollisionMap::IsValid() const
{
	return m_bValid;
}

void CCollisionMap::DestroyMap()
{
	Lock();
	FreeMap();
	Unlock();
}

POINT CCollisionMap::GetMapOrigin() const
{
	return m_ptLevelLeftTop;
}

void CCollisionMap::AbsToRelative(POINT &pt) const
{
	pt.x -= m_ptLevelLeftTop.x * 5;
	pt.y -= m_ptLevelLeftTop.y * 5;
}

void CCollisionMap::RelativeToAbs(POINT &pt) const
{
	pt.x += m_ptLevelLeftTop.x * 5;
	pt.y += m_ptLevelLeftTop.y * 5;
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

	AbsToRelative(pt1);
	AbsToRelative(pt2);
	
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
		const long OFFSET = y * MAP_LIMIT;
		for (int x = pt1.x; x < pt2.x; x++)
		{
			char ch = IsMarkPoint(x, y, lpPath);
			if (ch == 0)
				ch = (m_aMap[OFFSET + x] % 2) == 0 ? ' ' : 'X';

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
	RelativeToAbs(pt);
	
	if (lpPath->ptStart.x == pt.x && lpPath->ptStart.y == pt.y)
		return 'P';

	if (lpPath->ptEnd.x == pt.x && lpPath->ptEnd.y == pt.y)
		return 'E';

	for (DWORD i = 0; i < lpPath->dwNodeCount; i++)
	{
		if (lpPath->aPathNodes[i].x == pt.x && lpPath->aPathNodes[i].y == pt.y)
			return '*';
	}

	return 0;
}
