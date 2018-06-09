//////////////////////////////////////////////////////////////////////
// ItemPicker.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "ItemPicker.h"
#include <time.h>
#include <stdio.h>
#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"
#include "..\\Includes\\Utilities\\Ini.h"
#include "D2MiscItem.h"

#define INVENTORYFULL_RETRY		3

struct CFGDATA
{
	CItemPicker* pPicker;
	CIni* pIni;
};

BOOL operator==(const SIMPLEITEM& lhs, const SIMPLEITEM& rhs)
{
	return lhs.dwItemID == rhs.dwItemID;
}

CItemPicker::CItemPicker()
{
	m_lpszFullCmds = NULL;
	m_bShowClientMove = FALSE;
	m_szConfigFile[0] = '\0';
	m_dwRadius = 15;
	m_bTownPick = TRUE;
	m_dwDelay = 300;
	m_szLogFile[0] = '\0';
	CleanConfig();
	m_nInventoryFullState = 0;
	m_bPaused = TRUE;
	m_dwLastPick = 0;
	m_dwLastAnnounce = 0;
	m_thread.Start(ThreadProc, (LPVOID)this);
}

CItemPicker::~CItemPicker()
{
	m_thread.Stop(3000);
	if (m_lpszFullCmds != NULL)
		delete [] m_lpszFullCmds;
}

void CItemPicker::OnGameJoin()
{
	// TODO: Add your implementation here.
	m_dwLastPick = 0;
	CleanQueue();
	m_nInventoryFullState = 0;
	m_bPaused = FALSE;
	m_dwLastAnnounce = 0;

	if (!m_thread.IsAlive())
		server->GamePrintError("TakeIt Error: ÿc1Thread creation failed!");
}

void CItemPicker::OnGameLeave()
{
	// TODO: Add your implementation here.
	m_bPaused = TRUE;	
}

BOOL CItemPicker::LoadConfig(LPCSTR lpszFile)
{
	if (lpszFile == NULL || *lpszFile == 0)
		return FALSE; // invalid params

	DWORD dwRes = ::GetFileAttributes(lpszFile);
	if (dwRes == -1 || dwRes == FILE_ATTRIBUTE_DIRECTORY)
		return FALSE; // file not exists

	CleanConfig();
	CleanQueue();	

	m_nInventoryFullState = 0;
	strncpy(m_szConfigFile, lpszFile, MAX_PATH);	
	CIni ini(lpszFile);
	CFGDATA data;
	data.pIni = &ini;
	data.pPicker = this;
	ini.EnumSections(EnumSectionsProc, (LPVOID)&data);
	return TRUE;
}

void CItemPicker::CleanQueue()
{
	m_aIDList.Lock();
	m_aIDList.RemoveAll();
	m_aIDList.Unlock();

	m_aValuable.Lock();
	m_aValuable.RemoveAll();
	m_aValuable.Unlock();

	m_aJunky.Lock();
	m_aJunky.RemoveAll();
	m_aJunky.Unlock();
}

DWORD CItemPicker::SetRadius(DWORD dwRadius)
{
	m_dwRadius = dwRadius;
	return m_dwRadius;
}

DWORD CItemPicker::GetRadius() const
{
	return m_dwRadius;
}

void CItemPicker::Enable(BOOL bEnable)
{
	m_bPaused = !bEnable;
	if (bEnable)
		m_nInventoryFullState = 0; // Player may have rearranged his inventory, so reset inventory state
}

BOOL CItemPicker::IsEnabled() const
{
	return !m_bPaused;
}	

BOOL CItemPicker::Toggle()
{
	Enable(!IsEnabled());
	return IsEnabled();
}

BOOL CItemPicker::WriteLog(LPCSTR lpszFilePath, LPCSTR lpszString, BOOL bAppend)
{
	if (lpszFilePath == NULL || lpszString == NULL || lpszString[0] == 0)
		return FALSE;	

	FILE *stream = _tfopen(lpszFilePath, bAppend ? "a+" : "w");
	if (stream == NULL)
		return FALSE;

	// get time stamp
	char szTimeString[33] = "";
	struct tm *when;
    time_t now;
    ::time(&now);
    when = ::localtime(&now);
	::strftime(szTimeString, 32, "%m/%d %H:%M:%S", when);
	if (szTimeString[0] == '0')
		szTimeString[0] = ' ';

	const int LEN = ::strlen(szTimeString) + ::strlen(lpszString) + 10;
	char* pszString = new char[LEN];
	::sprintf(pszString, "%s | %s\n", szTimeString, lpszString);

	// replace the special color char to '%' so the log file can be read by
	// MFC CStdioFile
	for (int i = 0; pszString[i] != '\0'; i++)
	{
		if (pszString[i] == 'ÿ')
			pszString[i] = '%';
	}

	// write to file
	::fprintf(stream, "%s", pszString);

	delete [] pszString;
	::fclose(stream);
	return TRUE;
}

LPCSTR CItemPicker::GetConfigFile() const
{
	return m_szConfigFile;
}

int CItemPicker::GetQueueItemCount(BOOL bValuableOnly, BOOL bInRadiusOnly)
{
	if (bValuableOnly)
		return GetQueueItemCount(m_aValuable, bInRadiusOnly);
	else 
		return GetQueueItemCount(m_aValuable, bInRadiusOnly) + GetQueueItemCount(m_aJunky, bInRadiusOnly);
}

int CItemPicker::GetConfigCount()
{
	return m_aConfig.GetSize();
}

void CItemPicker::CleanConfig()
{
	m_aConfig.RemoveAll();
}

void CItemPicker::ShowClientMove(BOOL bShow)
{
	m_bShowClientMove = bShow;
}

BOOL CItemPicker::IsShowClientMove() const
{
	return m_bShowClientMove;
}

LPCSTR CItemPicker::GetFullCmds() const
{
	return m_lpszFullCmds;
}

void CItemPicker::SetFullCmds(LPCSTR lpszFullCmds)
{
	if (m_lpszFullCmds != NULL)
	{
		delete [] m_lpszFullCmds;
		m_lpszFullCmds = NULL;
	}

	if (lpszFullCmds != NULL && lpszFullCmds[0] != '\0')
		m_lpszFullCmds = strdup(lpszFullCmds);
}

void CItemPicker::SetAllowTownPick(BOOL bAllowTownPick)
{
	m_bTownPick = bAllowTownPick;
}

BOOL CItemPicker::IsAllowTownPick() const
{
	return m_bTownPick;
}

void CItemPicker::SetLogFilePath(LPCSTR lpszLogFile)
{
	::memset(m_szLogFile, 0, sizeof(m_szLogFile));
	if (lpszLogFile == NULL || *lpszLogFile == 0)
		return;

	// is abs path?
	if (IsAbsPath(lpszLogFile))
		strncpy(m_szLogFile, lpszLogFile, MAX_PATH);
	else
		sprintf(m_szLogFile, "%s\\%s", server->GetModuleDirectory(), lpszLogFile);
}

LPCSTR CItemPicker::GetLogFilePath() const
{
	return m_szLogFile;
}

BOOL CItemPicker::IsValuableItem(DWORD dwItemID, LPCSTR lpszItemCode)
{
	// Valueable items: Unique, set, runes
	if (lpszItemCode == NULL)
		return FALSE;	

	// good runes (13+)
	if (lpszItemCode[0] == 'r' && atoi(lpszItemCode + 1) >= 13)
		return TRUE;

	// small charm, Grand charm
	if (!::strcmp(lpszItemCode, "cm1") || !::strcmp(lpszItemCode, "cm3"))
		return TRUE;

	BYTE iQuality = server->GetItemQuality(dwItemID); // set, unique
	return (iQuality == ITEM_QUALITY_SET || iQuality == ITEM_QUALITY_UNIQUE);
}

BOOL CItemPicker::CanItemGotoBelt(int nMisc) const
{
	for (BYTE col = 0; col < 4; col++)
	{
		// check each belt column to see if this item can go directly into the belt
		DWORD dwItemID = me->GetBeltItem(col);
		if (dwItemID == 0)
			return TRUE; // first row empty, ok

		if (me->GetAvailableBeltSlots(col))
		{
			char szCode[3] = "";
			server->GetItemCode(dwItemID, szCode, 2);
			if (!::stricmp(szCode, "rv"))
				return TRUE; // first row item is with same type and there are empty slots on this column, ok
		}
	}

	return FALSE;
}

BOOL CItemPicker::ProcessItemQueue(ItemList &queue)
{
	BOOL bProcessed = FALSE;
	queue.Lock();
	const int COUNT = queue.GetItemCount(); // prevent dead circuit
	POSITION pos = queue.GetHeadPosition();	
	for (int i = 0; pos && i < COUNT; i++)
	{
		POSITION cur = pos;
		SIMPLEITEM& si = queue.GetNext(pos);
		
		const DWORD DISTANCE = me->GetDistanceFrom(si.x, si.y);
		if (DISTANCE > m_dwRadius)
		{
			si.dwRetry = 0;
			queue.MoveToRear(cur);
			continue; // ignore items out of range
		}

		if (MiscItemNeeded(si.dwMiscType, si.dwGoldAmount) == MISC_NOT_NEED)
		{
			queue.RemoveAt(cur);
			continue;
		}
		
		// need to teleport?
		if (si.bTeleport && DISTANCE > 3 && !me->IsInTown() && me->HaveSpell(D2S_TELEPORT))
		{			
			me->CastOnMap(D2S_TELEPORT, si.x, si.y, FALSE);
			si.bTeleport = FALSE; // we only teleport once for each item
			bProcessed = TRUE;
			queue.MoveToFront(cur);
			break;
		}

		// pick it up
		if (!me->PickGroundItem(si.dwItemID, m_bShowClientMove))
		{
			si.dwRetry = 0;
			queue.MoveToRear(cur);
			continue;
		}

		m_dwLastPick = ::GetTickCount();
		si.dwRetry++;
		if (si.dwRetry > 6 && (si.dwRetry * m_dwDelay > 2500)) // avoid starvation
		{
			si.dwRetry = 0;
			queue.MoveToRear(cur);
		}

		bProcessed = TRUE;
		break;		
	}	

	queue.Unlock();
	return bProcessed;
}

void CItemPicker::DebugQueue()
{
	POSITION pos = NULL;

	m_aValuable.Lock();
	server->GameInfof("Valueables: %d", m_aValuable.GetItemCount());
	pos = m_aValuable.GetHeadPosition();
	while (pos)
	{
		const SIMPLEITEM& si = m_aValuable.GetNext(pos);
		server->GameInfof("%08X, %s, teleport %d", si.dwItemID, si.szItemCode, si.bTeleport);
	}
	m_aValuable.Unlock();

	m_aJunky.Lock();
	server->GameInfof("Junks: %d", m_aJunky.GetItemCount());
	pos = m_aJunky.GetHeadPosition();
	while (pos)
	{
		const SIMPLEITEM& si = m_aJunky.GetNext(pos);
		server->GameInfof("%08X, %s, teleport %d", si.dwItemID, si.szItemCode, si.bTeleport);
	}
	m_aJunky.Unlock();
}

int CItemPicker::GetQueueItemCount(ItemList &list, BOOL bInRadiusOnly)
{
	list.Lock();
	int nCount = 0;

	if (!bInRadiusOnly)
	{
		nCount = list.GetItemCount();
		list.Unlock();
		return nCount;
	}

	POSITION pos = list.GetHeadPosition();
	while (pos)
	{
		const SIMPLEITEM& si = list.GetNext(pos);
		if (me->GetDistanceFrom(si.x, si.y) <= m_dwRadius)
			nCount++;
	}

	list.Unlock();
	return nCount;
}

void CItemPicker::OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == UM_DESTROY && lpUnit->dwUnitType == UNIT_TYPE_ITEM)
	{
		RemoveItemFromQueue(lpUnit->dwUnitID);		
		return;
	}
	else if (nMessage == UM_ITEMEVENT && (wParam == ITEM_EVENT_DROP || wParam == ITEM_EVENT_NEW_GROUND || wParam == ITEM_EVENT_OLD_GROUND))
	{
		m_aIDList.Lock();
		m_aIDList.InsertRear(lpUnit->dwUnitID);
		m_aIDList.Unlock();
	}
}

void CItemPicker::OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (nMessage != PM_INVENTORYFULL)
		return;
		
	m_nInventoryFullState++; // we shall retry a few more times	
	if (m_nInventoryFullState < INVENTORYFULL_RETRY)
		return;

	// notify the client
	server->GamePrintInfo("Inventory full!");
	m_bPaused = TRUE; // disable item pick immediately if inventory is full
				
	if (m_nInventoryFullState != INVENTORYFULL_RETRY || m_lpszFullCmds == NULL || *m_lpszFullCmds == 0)
		return;
	
	InvokeCallbackCommands(m_lpszFullCmds);
}

BOOL CItemPicker::EnumSectionsProc(LPCTSTR lpSection, LPVOID lpParam)
{
	CFGDATA* pData = (CFGDATA*)lpParam;
	if (pData == NULL || pData->pIni == NULL || pData->pPicker == NULL)
		return FALSE;

	if (!::stricmp(lpSection, "Global"))
		return TRUE;

	ITEMCONFIG cfg = { 0 };
	strncpy(cfg.szSection, lpSection, 255);
	cfg.bTeleport = pData->pIni->GetBool(lpSection, "teleport", FALSE);
	pData->pIni->GetString(lpSection, "Code", cfg.szCodes, 255);
	if (::strcmp(cfg.szCodes, "*") == 0)
		*cfg.szCodes = 0; // ignore code

	pData->pIni->GetString(lpSection, "pick", cfg.szExpression, 512);
	if (*cfg.szExpression)
		pData->pPicker->m_aConfig.Add(cfg);
	return TRUE;
}

void CItemPicker::AddToQueue(SIMPLEITEM &si)
{
	if (IsValuableItem(si.dwItemID, si.szItemCode))
	{
		m_aValuable.Lock();
		m_aValuable.InsertRear(si);
		m_aValuable.Unlock();
	}
	else
	{
		m_aJunky.Lock();
		m_aJunky.InsertRear(si);
		m_aJunky.Unlock();
	}
}

BOOL CItemPicker::RemoveItemFromQueue(DWORD dwItemID)
{
	POSITION pos = NULL;
	BOOL bRemoved = FALSE;

	m_aIDList.Lock();
	pos = m_aIDList.Find(dwItemID);
	bRemoved = pos != NULL;
	if (bRemoved)
		m_aIDList.RemoveAt(pos);
	m_aIDList.Unlock();

	if (bRemoved)
		return TRUE;

	SIMPLEITEM si;
	si.dwItemID = dwItemID;

	m_aValuable.Lock();
	pos = m_aValuable.Find(si);
	bRemoved = pos != NULL;
	if (bRemoved)
		m_aValuable.RemoveAt(pos);
	m_aValuable.Unlock();

	if (bRemoved)
		return TRUE;

	m_aJunky.Lock();
	pos = m_aJunky.Find(si);
	bRemoved = pos != NULL;
	if (bRemoved)
		m_aJunky.RemoveAt(pos);
	m_aJunky.Unlock();	
	
	return bRemoved;
}

void CItemPicker::OnGameTimerTick()
{
	CheckIDList();	
	
	DWORD dwMode = me->GetMode();
	if (dwMode== 7 || dwMode== 8 || (dwMode >= 10 && dwMode <= 16) || dwMode == 18)
		return; // excellent check, from d2jsp snagit

	if (m_bPaused || m_dwRadius == 0)
		return; // disabled

	if (::GetTickCount() < m_dwLastPick + m_dwDelay)
		return; // delay not elapsed yet

	if (!m_bTownPick && me->IsInTown())
		return; // in town and town_pick is disabled

	if (!ProcessItemQueue(m_aValuable))
	{
		//server->GameInfof("no valuable items queued, not processing junkies...");
		ProcessItemQueue(m_aJunky);
	}
}

DWORD CItemPicker::SetDelay(DWORD dwDelay)
{
	// must not be lower than 200 ms, otherwise connection might be interrupted
	// and must be dividable by 100
	/*
	DWORD dwMod = dwDelay % 100;
	dwDelay -= dwMod;

	if (dwMod >= 50)
		dwDelay += 100;

	m_dwDelay = max(PICKIT_DELAY_MIN, dwDelay);
	*/
	m_dwDelay = min(1000, dwDelay);
	return m_dwDelay;
}

DWORD CItemPicker::GetDelay() const
{
	return m_dwDelay;
}

void CItemPicker::CheckIDList()
{	
	static GAMEUNIT unit = { 0, UNIT_TYPE_ITEM };
	static SIMPLEITEM si;	
	static char szAnnounce[128];	

	m_aIDList.Lock();
	POSITION pos = m_aIDList.GetHeadPosition();
	while (pos)
	{		
		POSITION cur = pos;
		unit.dwUnitID = m_aIDList.GetNext(pos);
		si.dwItemID = unit.dwUnitID;
		
		DWORD dwMode = server->GetUnitMode(&unit);
		if (dwMode != ITEM_MODE_ON_GROUND && dwMode != ITEM_MODE_BEING_DROPPED)
			continue; // check later

		// now get its code
		if (!server->GetItemCode(unit.dwUnitID, si.szItemCode, 3))
			continue; // check later

		// Since we've reached here, the item is known, so no matter
		// we want it or not, it must be removed from m_aIDList
		m_aIDList.RemoveAt(cur);

		si.dwMiscType = GetMiscType(si.szItemCode);
		if (IsGold(si.szItemCode))
			si.dwGoldAmount = server->GetUnitStat(&unit, STAT_GOLD);
		else
			si.dwGoldAmount = 0;

		if (FindMatch(si, si.bTeleport))
		{
			POINT pt = server->GetUnitPosition(&unit);
			si.x = (WORD)pt.x;
			si.y = (WORD)pt.y;
			si.dwRetry = 0;
			AddToQueue(si);
		}
	}
	m_aIDList.Unlock();
}

BOOL CItemPicker::FindMatch(const SIMPLEITEM& item, BOOL& bTeleport)
{
	BOOL bFound = FALSE;
	bTeleport = FALSE;

	m_aConfig.Lock();
	for (int i = 0; i < m_aConfig.GetSize(); i++)
	{
		if (m_aConfig[i].bSyntaxError)
			continue;

		//server->GameInfof("codes: %s, exp: %s", m_aConfig[i].szCodes, m_aConfig[i].szExpression);

		BOOL bResult = server->ExamItemProperties(item.dwItemID, m_aConfig[i].szExpression, m_aConfig[i].szCodes, &m_aConfig[i].bSyntaxError);
		if (m_aConfig[i].bSyntaxError)
		{
			server->GameErrorf("ÿc4Error in sectionÿc0 [%s] ÿc4section disabled.", m_aConfig[i].szSection);
			continue;
		}
		
		if (bResult)
		{
			bFound = TRUE;
			bTeleport = m_aConfig[i].bTeleport;
			break;			
		}
	}
	m_aConfig.Unlock();

	if (bFound && m_dwLastAnnounce != item.dwItemID && item.dwMiscType == 0)
	{
		// avoid multiple announcements
		m_dwLastAnnounce = item.dwItemID;

		// announce it
		char szAnnounce[128] = "";
		server->GetItemDescription(item.dwItemID, szAnnounce, 127, TRUE);
		server->GamePrintInfo(szAnnounce);

		// log it?
		if (*m_szLogFile)
		{
			WriteLog(m_szLogFile, szAnnounce + 3, TRUE);
		}
	}

	return bFound;
}

void CItemPicker::ThreadProc(LPVOID lParam, LPCVBYTE lpStopFlag)
{
	CItemPicker* pPicker = (CItemPicker*)lParam;
	if (pPicker == NULL || !server->IsGameReady())
		return;

	pPicker->OnGameTimerTick();
}

int CItemPicker::InvokeCallbackCommands(LPCSTR lpszCommands)
{
	if (lpszCommands == NULL || *lpszCommands == 0)
		return 0;

	const int LEN = ::strlen(lpszCommands);
	LPSTR psz = new char[LEN + 2];
	::strcpy(psz, lpszCommands);
	psz[LEN + 1] = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (psz[i] == ';')
			psz[i] = 0;
	}

	int nCount = 0;
	for (LPSTR p = psz; p && *p; p += ::strlen(p) + 1)
	{
		nCount++;
		server->GameCommandLine(p);
	}

	delete [] psz;
	return nCount;
}

BOOL CItemPicker::IsAbsPath(LPCSTR lpszPath)
{
	if (lpszPath == NULL || ::strlen(lpszPath) < 3)
		return FALSE;

	return ::isalpha(lpszPath[0]) && lpszPath[1] == ':' && lpszPath[2] == '\\';
}
