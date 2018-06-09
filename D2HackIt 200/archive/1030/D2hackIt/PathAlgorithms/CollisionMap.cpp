// CollisionMap.cpp: implementation of the CCollisionMap class.
//
//////////////////////////////////////////////////////////////////////

#include "CollisionMap.h"
#include "..\\d2functions.h"
#include "Common.h"
#include "D2HackIt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCollisionMap::CCollisionMap()
{
	m_iCurMap = 0x00;
	::memset(&m_ptLevelOrigin, 0, sizeof(POINT));
}

CCollisionMap::~CCollisionMap()
{
}

void CCollisionMap::OnMapChanged(BYTE iNewMapID)
{
	if (iNewMapID != m_iCurMap)
	{
		m_iCurMap = iNewMapID;
		m_map.Lock(5000);
		m_map.Destroy();
		m_map.Unlock();	
		::memset(&m_ptLevelOrigin, 0, sizeof(POINT));
		m_aCollisionTypes.RemoveAll();
	}
}

void CCollisionMap::AddCollisionData(const DrlgColl* pCol)
{
	if (pCol == NULL)
		return;

	int x = pCol->nPosGameX - m_ptLevelOrigin.x;
	int y = pCol->nPosGameY - m_ptLevelOrigin.y;
	int cx = pCol->nSizeGameX;
	int cy = pCol->nSizeGameY;

	if (!m_map.IsValidIndex(x, y))
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
			m_map[i][j] = *p;	
			if (m_aCollisionTypes.Find(*p) == -1)
			{
				m_aCollisionTypes.Add(*p);
				m_aCollisionTypes.Sort();
			}

			p++;
		}
	}
}

BOOL CCollisionMap::IsValidAbsLocation(long x, long y) const
{
	if (!m_map.IsCreated())
		return FALSE;

	x -= m_ptLevelOrigin.x;
	y -= m_ptLevelOrigin.y;
	return m_map.IsValidIndex(x, y);
}

WORD CCollisionMap::GetMapData(long x, long y, BOOL bAbs) const
{
	if (!m_map.IsCreated())
		return MAP_DATA_INVALID;
	
	if (bAbs)
	{
		x -= m_ptLevelOrigin.x;
		y -= m_ptLevelOrigin.y;
	}

	if (!m_map.Lock(500))
		return MAP_DATA_INVALID;

	WORD wVal = MAP_DATA_INVALID;

	if (m_map.IsValidIndex(x, y))
		wVal = m_map[x][y];

	m_map.Unlock();
	return wVal;
}

BOOL CCollisionMap::BuildMapData()
{
	if (m_map.IsCreated())
		return TRUE;

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
	
	m_ptLevelOrigin.x = pLevel->LevelX * 5;
	m_ptLevelOrigin.y = pLevel->LevelY * 5;
	
	if (!m_map.Lock(5000))
		return FALSE;

	if (!m_map.Create(pLevel->LevelCX * 5, pLevel->LevelCY * 5, MAP_DATA_INVALID))
	{
		m_map.Unlock();
		return FALSE;
	}

	DwordArray aSkip;
	Search(pRoom2, pPlayer, aSkip);

	// Fill gaps
	FillGaps();
	FillGaps(); // Yes, we need to do it twich
	//ThickenWalls(m_ppMap, m_czLevelSize, 1);
	
	m_map.Unlock();
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
	DWORD dwStart = ::GetTickCount();
	BOOL bOK = BuildMapData();
	DWORD dwEnd = ::GetTickCount();	

	if (!bOK)
		GamePrintError("ÿc8Map data creation failed!");
	else
		GameInfof("ÿc4Map data created in ÿc2%dÿc4 milliseconds.", dwEnd - dwStart);

	return bOK;
}

POINT CCollisionMap::GetMapOrigin() const
{
	return m_ptLevelOrigin;
}

void CCollisionMap::AbsToRelative(POINT &pt) const
{
	pt.x -= m_ptLevelOrigin.x;
	pt.y -= m_ptLevelOrigin.y;
}

void CCollisionMap::RelativeToAbs(POINT &pt) const
{
	pt.x += m_ptLevelOrigin.x;
	pt.y += m_ptLevelOrigin.y;
}

BOOL CCollisionMap::DumpMap(LPCSTR lpszFilePath, const LPPOINT lpPath, DWORD dwCount) const
{
	if (lpszFilePath == NULL)
		return FALSE;

	FILE *fp = fopen(lpszFilePath, "w+");
	if(fp == NULL )
		return FALSE;	
	
	if (!m_map.IsCreated())
		return FALSE;

	if (!m_map.Lock(5000))
		return FALSE;

	char szMapName[256] = "";
	GetMapName(m_iCurMap, szMapName, 255);

	fprintf(fp, "%s (Size: %d * %d)\nKnown Collision Types: ", szMapName, m_map.GetCX(), m_map.GetCY());
	for (int i = 0; i < m_aCollisionTypes.GetSize(); i++)
	{
		fprintf(fp, "%d, ", m_aCollisionTypes[i]);
	}

	fprintf(fp, "\n\n");

	POINT* pPath = NULL;
	if (lpPath && dwCount)
	{
		pPath = new POINT[dwCount];
		::memcpy(pPath, lpPath, dwCount * sizeof(POINT));
		for (DWORD i = 0; i < dwCount; i++)
			AbsToRelative(pPath[i]);
	}	

	POINT ptPlayer = GetPosition();
	AbsToRelative(ptPlayer);

	const int CX = m_map.GetCX();
	const int CY = m_map.GetCY();

	for (int y = 0; y < CY; y++)
	{		
		for (int x = 0; x < CX; x++)
		{
			char ch = IsMarkPoint(ptPlayer, x, y, pPath, dwCount);			

			if (!ch)
				ch = (m_map[x][y] % 2) ? 'X' : ' ';

			fprintf(fp, "%C", ch); // X - unreachable
		}

		fprintf(fp, "%c", '\n');
	}

	if (pPath)
		delete [] pPath;

	m_map.Unlock();	
	fclose(fp);

	return TRUE;
}

char CCollisionMap::IsMarkPoint(const POINT& ptPlayer, int x, int y, const LPPOINT lpPath, DWORD dwCount) const
{	
	char ch = 0;
	if (x == ptPlayer.x && y == ptPlayer.y)
		ch = 'P';

	if (lpPath == NULL || dwCount == 0)
		return ch;

	for (DWORD i = 0; i < dwCount; i++)
	{
		if (lpPath[i].x == x && lpPath[i].y == y)
		{
			if (i == 0)
				return 'S'; // start
			else if (i == dwCount - 1)
				return 'E'; // end
			else
				return '*'; // nodes
		}
	}

	return ch;
}

SIZE CCollisionMap::GetMapSize() const
{
	SIZE cz = {0};
	if (m_map.IsCreated())
	{
		cz.cx = m_map.GetCX();
		cz.cy = m_map.GetCY();
	}
	return cz;
}

SIZE CCollisionMap::CopyMapData(WORD** ppBuffer, int cx, int cy) const
{
	SIZE copied = {0};
	if (!m_map.Lock(2000))
		return copied;

	copied = m_map.ExportData(ppBuffer, cx, cy);
	m_map.Unlock();
	return copied;
}

BOOL CCollisionMap::IsGap(int x, int y) const
{
	if (m_map[x][y] % 2)
		return FALSE;

	int nSpaces = 0;
	int i = 0;

	// Horizontal check
	for (i = x - 2; i <= x + 2 && nSpaces < 3; i++)
	{
		if ( i < 0 || i >= m_map.GetCX() || (m_map[i][y] % 2))
			nSpaces = 0;
		else
			nSpaces++;
	}

	if (nSpaces < 3)
		return TRUE;

	// Vertical check
	nSpaces = 0;
	for (i = y - 2; i <= y + 2 && nSpaces < 3; i++)
	{
		if ( i < 0 || i >= m_map.GetCY() || (m_map[x][i] % 2))
			nSpaces = 0;
		else
			nSpaces++;
	}

	return nSpaces < 3;
}

void CCollisionMap::FillGaps()
{
	if (!m_map.IsCreated())
		return;

	if (!m_map.Lock(5000))
		return;

	const int CX = m_map.GetCX();
	const int CY = m_map.GetCY();

	for (int x = 0; x <CX; x++)
	{
		for (int y = 0; y < CY; y++)
		{
			if (IsGap(x, y))
				m_map[x][y] = MAP_DATA_FILLED;
		}
	}

	m_map.Unlock();
}

void CCollisionMap::MakeBlank(WordMatrix& rMatrix, POINT pos)
{
	if (!rMatrix.IsCreated())
		return;

	for (int i = pos.x - 1; i <= pos.x + 1; i++)
	{
		for (int j = pos.y - 1; j <= pos.y + 1; j++)
		{
			if (rMatrix.IsValidIndex(i, j))
				rMatrix[i][j] = MAP_DATA_CLEANED;
		}
	}
}

BOOL CCollisionMap::ThickenWalls(WordMatrix& rMatrix, int nThickenBy)
{
	if (!rMatrix.IsCreated() || nThickenBy <= 0)
		return FALSE;

	const int CX = rMatrix.GetCX();
	const int CY = rMatrix.GetCY();
	
	for (int i = 0; i < CX; i++)
	{
		for (int j = 0; j < CY; j++)
		{
			if ((rMatrix[i][j] % 2) == 0 || rMatrix[i][j] == MAP_DATA_THICKENED)
				continue;

			for (int x = i - nThickenBy; x <= i + nThickenBy; x++)
			{
				for (int y = j - nThickenBy; y <= j + nThickenBy; y++)
				{
					if (!rMatrix.IsValidIndex(x, y))
						continue;

					if ((rMatrix[x][y] % 2) == 0)
						rMatrix[x][y] = MAP_DATA_THICKENED;
				}
			}
		}
	}

	return TRUE;
}

void CCollisionMap::DestroyMap()
{
	m_map.Lock(3000);
	m_map.Destroy();
	m_map.Unlock();
	m_iCurMap = 0x00;
	m_aCollisionTypes.RemoveAll();
	::memset(&m_ptLevelOrigin, 0, sizeof(POINT));
}

BOOL CCollisionMap::CopyMapData(WordMatrix& rBuffer) const
{
	if (!m_map.Lock(2000))
		return FALSE;
	
	m_map.ExportData(rBuffer);
	m_map.Unlock();
	return rBuffer.IsCreated();
}
