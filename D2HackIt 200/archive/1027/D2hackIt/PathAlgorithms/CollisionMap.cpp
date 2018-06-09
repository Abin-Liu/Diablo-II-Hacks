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
	m_ppMap = NULL;
	m_iCurMap = 0x00;
	m_bValid = FALSE;
	::memset(&m_ptLevelOrigin, 0, sizeof(POINT));
	::memset(&m_czLevelSize, 0, sizeof(SIZE));
}

CCollisionMap::~CCollisionMap()
{
	FreeMap();
}

BOOL CCollisionMap::AllocateMap()
{
	if (m_ppMap)
		return m_bValid;

	m_ppMap = AllocateMatrix(m_czLevelSize.cx, m_czLevelSize.cy);
	m_bValid = m_ppMap != NULL;

	if (m_ppMap)
	{
		for (int i = 0; i < m_czLevelSize.cx; i++)
		{
			for (int j = 0; j < m_czLevelSize.cy; j++)
				m_ppMap[i][j] = MAP_DATA_INVALID;
		}
	}
	return m_bValid;
}

void CCollisionMap::FreeMap()
{
	m_bValid = FALSE;	
	FreeMatrix(m_ppMap, m_czLevelSize.cx, m_czLevelSize.cy);
	::memset(&m_ptLevelOrigin, 0, sizeof(POINT));
	::memset(&m_czLevelSize, 0, sizeof(SIZE));
	m_aCollisionTypes.RemoveAll();
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

	int x = pCol->nPosGameX - m_ptLevelOrigin.x;
	int y = pCol->nPosGameY - m_ptLevelOrigin.y;
	int cx = pCol->nSizeGameX;
	int cy = pCol->nSizeGameY;

	if (x < 0 || x + cx > m_czLevelSize.cx || y < 0 || y + cy > m_czLevelSize.cy)
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
			m_ppMap[i][j] = *p;	
			if (m_aCollisionTypes.Find(*p) == -1)
			{
				m_aCollisionTypes.Add(*p);
				m_aCollisionTypes.Sort();
			}

			p++;
		}
	}
}

BOOL CCollisionMap::IsValidAbsLocation(WORD x, WORD y) const
{
	if (!m_bValid)
		return FALSE;

	return x >= m_ptLevelOrigin.x
		&& x < m_ptLevelOrigin.x + m_czLevelSize.cx
		&& y >= m_ptLevelOrigin.y
		&& y < m_ptLevelOrigin.y + m_czLevelSize.cy;
}

WORD CCollisionMap::GetMapData(long x, long y, BOOL bAbs) const
{
	if (!m_bValid)
		return MAP_DATA_INVALID;

	if (!Lock(500))
		return MAP_DATA_INVALID;

	if (bAbs)
	{
		x -= m_ptLevelOrigin.x;
		y -= m_ptLevelOrigin.y;
	}

	WORD wVal = MAP_DATA_INVALID;
	if (x >= 0 && x < m_czLevelSize.cx && y >= 0 || y < m_czLevelSize.cy)
		wVal = m_ppMap[x][y];

	Unlock();
	return wVal;
}

BOOL CCollisionMap::BuildMapData()
{
	FreeMap();

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
	m_czLevelSize.cx = pLevel->LevelCX * 5;
	m_czLevelSize.cy = pLevel->LevelCY * 5;
	
	if (!AllocateMap())
		return FALSE;	

	DwordArray aSkip;
	Search(pRoom2, pPlayer, aSkip);

	// Fill gaps
	FillGaps();
	FillGaps(); // Yes, we need to do it twich

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
	if (!Lock(5000))
	{
		GamePrintError("ÿc8Map data lock failed!");
		return FALSE;
	}

	DWORD dwStart = ::GetTickCount();
	if (m_ppMap == NULL)
		BuildMapData();
	DWORD dwEnd = ::GetTickCount();	
	Unlock();

	if (!m_bValid)
		GamePrintError("ÿc8Map data creation failed!");
	else
		GameInfof("ÿc4Map data created in ÿc2%dÿc4 milliseconds.", dwEnd - dwStart);

	return m_bValid;
}

BOOL CCollisionMap::IsValid() const
{
	return m_bValid && m_ppMap != NULL;
}

void CCollisionMap::DestroyMap()
{
	Lock();
	FreeMap();
	Unlock();
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
	
	Lock();
	if (!m_bValid || m_ppMap == NULL)
	{
		Unlock();
		return FALSE;
	}	

	char szMapName[256] = "";
	GetMapName(m_iCurMap, szMapName, 255);

	fprintf(fp, "%s (Size: %d * %d)\nKnown Collision Types: ", szMapName, m_czLevelSize.cx, m_czLevelSize.cy);
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

	for (int y = 0; y < m_czLevelSize.cy; y++)
	{		
		for (int x = 0; x < m_czLevelSize.cx; x++)
		{
			char ch = IsMarkPoint(ptPlayer, x, y, pPath, dwCount);

			//if (!ch)
			//{
			//	if (m_ppMap[x][y] == MAP_DATA_FILLED)
			//		ch = 'G';
			//}

			if (!ch)
				ch = (m_ppMap[x][y] % 2) ? 'X' : ' ';

			fprintf(fp, "%C", ch); // X - unreachable
		}

		fprintf(fp, "%c", '\n');
	}

	if (pPath)
		delete [] pPath;

	Unlock();	
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
	return m_czLevelSize;
}

SIZE CCollisionMap::CopyMapData(WORD** ppBuffer, int cx, int cy) const
{
	SIZE copied = {0};
	if (ppBuffer == NULL || !IsValid())
		return copied;

	copied.cx = min(cx, m_czLevelSize.cx);
	copied.cy = min(cy, m_czLevelSize.cy);

	if (copied.cx <= 0 || copied.cy <= 0)
	{
		::memset(&copied, 0, sizeof(SIZE));
		return copied;
	}

	if (!Lock(2000))
		return copied;

	for (int i = 0; i < copied.cx; i++)
	{
		for (int j = 0; j < copied.cy; j++)
			ppBuffer[i][j] = m_ppMap[i][j];
	}

	Unlock();
	return copied;
}

BOOL CCollisionMap::IsGap(int x, int y) const
{
	if (m_ppMap[x][y] % 2)
		return FALSE;

	int nSpaces = 0;
	int i = 0;

	// Horizontal check
	for (i = x - 2; i <= x + 2 && nSpaces < 3; i++)
	{
		if ( i < 0 || i >= m_czLevelSize.cx || (m_ppMap[i][y] % 2))
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
		if ( i < 0 || i >= m_czLevelSize.cy || (m_ppMap[x][i] % 2))
			nSpaces = 0;
		else
			nSpaces++;
	}

	return nSpaces < 3;
}

void CCollisionMap::FillGaps()
{
	if (m_ppMap == NULL)
		return;

	for (int x = 0; x < m_czLevelSize.cx; x++)
	{
		for (int y = 0; y < m_czLevelSize.cy; y++)
		{
			if (IsGap(x, y))
				m_ppMap[x][y] = MAP_DATA_FILLED;
		}
	}
}

void CCollisionMap::MakeBlank(WORD **ppMap, SIZE czMapSize, POINT pos)
{
	if (ppMap == NULL)
		return;

	for (int i = pos.x - 1; i <= pos.x + 1; i++)
	{
		for (int j = pos.y - 1; j <= pos.y + 1; j++)
		{
			if (i >= 0 && i < czMapSize.cx && j >= 0 && j < czMapSize.cy)
				ppMap[i][j] = MAP_DATA_CLEANED;
		}
	}
}
