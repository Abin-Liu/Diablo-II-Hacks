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
#include "Utilities\\Ini.h"
#include "StringParse.h"

#define INVENTORYFULL_RETRY		3
#define MAX_LOCK_WAIT			1000
#define ITEM_UNSET				0xff

#define	ITEM_ITEMTYPE_HELM				0x00
#define	ITEM_ITEMTYPE_ARMOR				0x01
#define	ITEM_ITEMTYPE_WEAPON			0x05
#define	ITEM_ITEMTYPE_BOW				0x06
#define	ITEM_ITEMTYPE_SHIELD			0x07
#define	ITEM_ITEMTYPE_EXPANSION			0x0A // heads
#define	ITEM_ITEMTYPE_OTHER				0x10

BOOL operator==(const ITEMCONFIG& lhs, const ITEMCONFIG& rhs)
{
	return ::memcmp(lhs.szItemCode, rhs.szItemCode, 3) == 0;
}

BOOL operator>(const ITEMCONFIG& lhs, const ITEMCONFIG& rhs)
{
	return ::memcmp(lhs.szItemCode, rhs.szItemCode, 3) > 0;
}

BOOL operator==(const SIMPLEITEM& lhs, const SIMPLEITEM& rhs)
{
	return lhs.dwItemID == rhs.dwItemID;
}

CItemPicker::CItemPicker()
{
	::memset(&m_tempItem, 0, sizeof(SIMPLEITEM));
	::memset(&m_tempConfig, 0, sizeof(ITEMCONFIG));
	m_lpszFullCmds = NULL;
	m_bShowClientMove = FALSE;
	m_nGoldMethod = PICKIT_PICK_AUTO;
	m_nRejuvMethod = PICKIT_PICK_NO;
	m_nFullRejuvMethod = PICKIT_PICK_AUTO;
	m_szConfigFile[0] = '\0';
	m_dwDelay = PICKIT_DELAY_DEFAULT;
	m_dwRadius = PICKIT_RADIUS_DEFAULT;
	m_bTownPick = TRUE;
	m_szLogFile[0] = '\0';
	CleanConfig();
	m_dwMinGoldAmount = 0;
	m_nInventoryFullState = 0;
	m_bPaused = TRUE;
	m_thread.Start(TickProc, (LPVOID)this, m_dwDelay, THREAD_PRIORITY_BELOW_NORMAL);
}

CItemPicker::~CItemPicker()
{
	m_thread.Stop(2500);
	if (m_lpszFullCmds != NULL)
		delete [] m_lpszFullCmds;
}

void CItemPicker::OnGameJoin()
{
	// TODO: Add your implementation here.			
	if (!m_thread.IsAlive())
		server->GamePrintError("Pickit: CreateThread failed!");

	m_nInventoryFullState = 0;
	m_bPaused = FALSE;
}

void CItemPicker::OnGameLeave()
{
	// TODO: Add your implementation here.
	m_bPaused = TRUE;	
	CleanQueue();
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

	m_aConfig1.Lock(1000);
	m_aConfig2.Lock(1000);
	
	m_nInventoryFullState = 0;

	strncpy(m_szConfigFile, lpszFile, MAX_PATH);	
	CIni ini(lpszFile);

	// keey enlarging the buffer unless we make sure all section names
	// are loaded.
	DWORD dwBufSize = 1024;
	char *pszBuffer = new char[dwBufSize];
	DWORD dwCopied = ini.GetSectionNames(pszBuffer, dwBufSize - 1);
	while (dwCopied + 128 >= dwBufSize)
	{
		dwBufSize += 1024;
		delete [] pszBuffer;
		pszBuffer = new char[dwBufSize];
		dwCopied = ini.GetSectionNames(pszBuffer, dwBufSize - 1);
	}	
	
	// iterate through all sections
	ITEMCONFIG cfg = { 0 };
	for (char* pSection = pszBuffer; pSection[0]; pSection = &pSection[strlen(pSection) + 1])
	{	
		// now query the section and fill in data

		::memset(&cfg, 0, sizeof(ITEMCONFIG));
		char sz[256] = "";
				
		// ItemCode should be 3 chars long
		// Code="xxx" or Code=xxx
		ini.GetString(pSection, "Code", sz, 3 + 3);
		if (sz[0] == '\"')
			strncpy(cfg.szItemCode, &sz[1], 3);
		else
			strncpy(cfg.szItemCode, sz, 3);
		
		if (strlen(cfg.szItemCode) != 3)
			continue;

		// Item Description
		ini.GetString(pSection, "Description", cfg.szDescription, 128);

		// replace "%c" to "ÿc" so users do not need to put that character in their ini files
		// they can use "%c" instead.
		for (int i = 0; cfg.szDescription[i] != '\0'; i++)
		{
			if (cfg.szDescription[i] == '%' && cfg.szDescription[i + 1] == 'c')
				cfg.szDescription[i] = 'ÿ';
		}

		// identified
		cfg.iIdentified = (BYTE)ini.GetInt(pSection, "Identified", ITEM_UNSET);

		// ethereal
		cfg.iEthereal = (BYTE)ini.GetInt(pSection, "isEthereal", ITEM_UNSET);

		// socket number
		cfg.iSockets = (BYTE)ini.GetInt(pSection, "Sockets", ITEM_UNSET);

		// dont pickup item.  useful if you only want to be told of the item drop
		cfg.iPickup = (BYTE)ini.GetInt(pSection, "Pickup", 1);

		if (IsGold(cfg.szItemCode)
			|| IsRejuv(cfg.szItemCode)
			|| IsFullRejuv(cfg.szItemCode))
		{
			// these items are special, we do not load them by the normal way...
			continue;
		}

		if (!cfg.iPickup && strlen(cfg.szDescription) == 0)
			continue; // no pickup no announce

		// item level
		ini.GetString(pSection, "Level", sz, 64);

		if(stricmp(sz, "rare") == 0)
			cfg.iQuality = ITEM_QUALITY_RARE;
		else if(stricmp(sz, "unique") == 0)
			cfg.iQuality = ITEM_QUALITY_UNIQUE;
		else if(stricmp(sz, "set") == 0)
			cfg.iQuality = ITEM_QUALITY_SET;
		else if(stricmp(sz, "magic") == 0)
			cfg.iQuality = ITEM_QUALITY_MAGIC;
		else if(stricmp(sz, "craft") == 0)
			cfg.iQuality = ITEM_QUALITY_CRAFT;
		else if(stricmp(sz, "normal") == 0)
			cfg.iQuality = ITEM_QUALITY_NORMAL;
		else if(stricmp(sz, "superior") == 0)
			cfg.iQuality = ITEM_QUALITY_SUPERIOR;
		else if(stricmp(sz, "lowquality") == 0)
			cfg.iQuality = ITEM_QUALITY_INFERIOR;
		else
			cfg.iQuality = ITEM_UNSET;
	
		// item types
		ini.GetString(pSection, "Type", sz, 64);
		if(stricmp(sz, "Helm") == 0)
			cfg.iType = ITEM_ITEMTYPE_HELM;
		else if(stricmp(sz, "Armor") == 0)
			cfg.iType = ITEM_ITEMTYPE_ARMOR;
		else if(stricmp(sz, "Weapon") == 0)
			cfg.iType = ITEM_ITEMTYPE_WEAPON;
		else if(stricmp(sz, "Bow") == 0)
			cfg.iType = ITEM_ITEMTYPE_BOW;
		else if(stricmp(sz, "Shield") == 0)
			cfg.iType = ITEM_ITEMTYPE_SHIELD;
		else if(stricmp(sz, "Other") == 0)
			cfg.iType = ITEM_ITEMTYPE_OTHER;
		else 
			cfg.iType = ITEM_UNSET;

		// first check if the config already exists in m_aConfig1
		if (m_aConfig1.Find(cfg) == -1)
			m_aConfig1.Add(cfg);
		else
			m_aConfig2.Add(cfg);
	}
	
	// the large buffer is no longer needed
	delete [] pszBuffer;
	pszBuffer = NULL;	

	m_aConfig1.Sort(); // sort the config1 to turn on binary quick searching!
	m_aConfig2.Sort();

	m_aConfig1.Unlock();
	m_aConfig2.Unlock();
	return TRUE;
}

void CItemPicker::CleanQueue()
{
	if (!m_aValuable.Lock(MAX_LOCK_WAIT))
		server->GamePrintError("Pickit: Item queue(valuable) lock failed!");

	m_aValuable.RemoveAll();
	m_aValuable.Unlock();

	if (!m_aJunky.Lock(MAX_LOCK_WAIT))
		server->GamePrintError("Pickit: Item queue(junky) lock failed!");

	m_aJunky.RemoveAll();
	m_aJunky.Unlock();
}

DWORD CItemPicker::SetRadius(DWORD dwRadius)
{
	m_dwRadius = min(PICKIT_RADIUS_MAX, dwRadius);
	return m_dwRadius;
}

DWORD CItemPicker::GetRadius() const
{
	return m_dwRadius;
}

LPCITEMCONFIG CItemPicker::FindItem(LPCGAMEUNIT lpUnit, BYTE iItemType, SIMPLEITEM& si, BOOL& bValuable)
{	
	if (!m_aConfig1.Lock(MAX_LOCK_WAIT))
	{
		server->GamePrintError("Pickit: Config lists1 lock failed. Config not searched.");
		return NULL;
	}	

	int nIndex = -1;
	LPCITEMCONFIG p = NULL;
	m_tempConfig.szItemCode[0] = 0;
	server->GetItemCode(lpUnit->dwUnitID, m_tempConfig.szItemCode, 3);

	// Search in m_config1, this will be a binary fast searching
	nIndex = m_aConfig1.Find(m_tempConfig);
	if (nIndex != -1)
		p = ExamConfigItem(lpUnit, m_aConfig1.GetAt(nIndex), si, iItemType, bValuable);

	m_aConfig1.Unlock();
	if (p)
		return p;
	
	if (!m_aConfig2.Lock(MAX_LOCK_WAIT))
	{
		server->GamePrintError("Pickit: Config list2 lock failed. Config not searched.");
		return NULL;
	}	

	// Search in m_config2, this will be a linear slow searching but number of elements
	// in m_config2 are usually very few, if there are any at all.
	for (int i = 0; p == NULL && i < m_aConfig2.GetSize(); i++)
	{
		const ITEMCONFIG& cfg = m_aConfig1.GetAt(i);
		int nCmp = ::memcmp(cfg.szItemCode, m_tempConfig.szItemCode, 3);
		if (nCmp > 0)
			break;

		if (nCmp == 0)
			p = ExamConfigItem(lpUnit, cfg, si, iItemType, bValuable);
	}		

	m_aConfig2.Unlock();

	return p;
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
	strftime(szTimeString, 32, "%m/%d %H:%M:%S", when);
	if (szTimeString[0] == '0')
		szTimeString[0] = ' ';

	const int LEN = strlen(szTimeString) + strlen(lpszString) + 10;
	char* pszString = new char[LEN];
	_stprintf(pszString, "%s | %s\n", szTimeString, lpszString);

	// replace the special color char to '%' so the log file can be read by
	// MFC CStdioFile
	for (int i = 0; pszString[i] != '\0'; i++)
	{
		if (pszString[i] == 'ÿ')
			pszString[i] = '%';
	}

	// write to file
	_ftprintf(stream, "%s", pszString);

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
	int nCount = 0;
	if (m_aConfig1.Lock(MAX_LOCK_WAIT))
		nCount += m_aConfig1.GetSize();
	m_aConfig1.Unlock();

	if (m_aConfig2.Lock(MAX_LOCK_WAIT))
		nCount += m_aConfig2.GetSize();
	m_aConfig2.Unlock();

	nCount -= 3; // minus 3 special items
	return max(0, nCount);
}

BOOL CItemPicker::IsNeedPickup(const SIMPLEITEM& item)
{
	if (!m_bTownPick && me->IsInTown())
		return FALSE;

	// For special items check for m_bAllowxxx
	if (IsGold(item.szItemCode))
		return m_nGoldMethod == PICKIT_PICK_YES || (m_nGoldMethod == PICKIT_PICK_AUTO && item.dwGoldAmount + 5000 < me->GetInventoryGoldLimit() - me->GetStat(STAT_GOLD));

	if (IsRejuv(item.szItemCode))
		return m_nRejuvMethod == PICKIT_PICK_YES || (m_nRejuvMethod == PICKIT_PICK_AUTO && CanItemGotoBelt(item.szItemCode));

	if (IsFullRejuv(item.szItemCode))
		return m_nFullRejuvMethod == PICKIT_PICK_YES || (m_nFullRejuvMethod == PICKIT_PICK_AUTO && CanItemGotoBelt(item.szItemCode));

	return TRUE;
}

BOOL CItemPicker::RemoveFromQueue(ItemList& queue, DWORD dwItemID)
{
	if (!queue.Lock(MAX_LOCK_WAIT))
	{
		server->GamePrintError("Pickit: Item queue lock failed. Item not searched.");
		return FALSE;
	}

	BOOL bFound = FALSE;
	POSITION pos = queue.GetHeadPosition();
	while (pos)
	{
		POSITION temp = pos;
		const SIMPLEITEM& si = queue.GetNext(pos);
		if (si.dwItemID == dwItemID)
		{
			queue.RemoveAt(temp);
			bFound = TRUE;
			break;
		}
	}

	queue.Unlock();
	return bFound;
}

DWORD CItemPicker::SetDelay(DWORD dwDelay)
{
	// must not be lower than 200 ms, otherwise connection might be interrupted
	// and must be dividable by 100
	DWORD dwMod = dwDelay % 100;
	dwDelay -= dwMod;

	if (dwMod >= 50)
		dwDelay += 100;

	m_dwDelay = max(PICKIT_DELAY_MIN, dwDelay);
	m_dwDelay = min(1500, dwDelay);
	m_thread.SetInterval(m_dwDelay);

	return m_dwDelay;
}

DWORD CItemPicker::GetDelay() const
{
	return m_dwDelay;
}

BOOL CItemPicker::SetSpecialItemMethod(int nSpecialItem, int nMethod)
{
	if (nMethod < PICKIT_PICK_NO || nMethod > PICKIT_PICK_AUTO)
		return FALSE;

	switch (nSpecialItem)
	{
	case PICKIT_SPECIAL_GOLD:
		m_nGoldMethod = nMethod;
		break;

	case PICKIT_SPECIAL_REJUV:
		m_nRejuvMethod = nMethod;
		break;

	case PICKIT_SPECIAL_FULLREJUV:
		m_nFullRejuvMethod = nMethod;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

int CItemPicker::GetSpecialItemMethod(int nSpecialItem) const
{
	int nMethod = PICKIT_PICK_UNKNWON;
	switch (nSpecialItem)
	{
	case PICKIT_SPECIAL_GOLD:
		return m_nGoldMethod;
		break;

	case PICKIT_SPECIAL_REJUV:
		return m_nRejuvMethod;
		break;

	case PICKIT_SPECIAL_FULLREJUV:
		return m_nFullRejuvMethod;
		break;

	default:
		nMethod = PICKIT_PICK_UNKNWON;
		break;
	}

	return nMethod;
}

void CItemPicker::CleanConfig()
{
	if (!m_aConfig1.Lock(MAX_LOCK_WAIT))
		server->GamePrintError("Pickit: Config list lock failed!");

	m_aConfig1.RemoveAll();

	// Always add special items
	ITEMCONFIG cfg = { 0 };
	cfg.iEthereal = ITEM_UNSET;
	cfg.iIdentified = ITEM_UNSET;
	cfg.iQuality = ITEM_UNSET;
	cfg.iPickup = 1;
	cfg.iSockets = ITEM_UNSET;
	cfg.iType = ITEM_UNSET;	

	strncpy(cfg.szItemCode, "gld", 3);
	m_aConfig1.Add(cfg); // Gold

	strncpy(cfg.szItemCode, "rvl", 3);
	m_aConfig1.Add(cfg); // Full rejuv

	strncpy(cfg.szItemCode, "rvs", 3);
	m_aConfig1.Add(cfg); // Rejuv
	m_aConfig1.Sort();
	m_aConfig1.Unlock();

	m_aConfig2.Lock(500);
	m_aConfig2.RemoveAll();
	m_aConfig2.Unlock();
}

BOOL CItemPicker::IsGold(LPCSTR lpszItemCode)
{
	return lpszItemCode != NULL && ::memcmp(lpszItemCode, "gld", 3) == 0;
}

BOOL CItemPicker::IsRejuv(LPCSTR lpszItemCode)
{
	return lpszItemCode != NULL && ::memcmp(lpszItemCode, "rvs", 3) == 0;
}

BOOL CItemPicker::IsFullRejuv(LPCSTR lpszItemCode)
{
	return lpszItemCode != NULL && ::memcmp(lpszItemCode, "rvl", 3) == 0;
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
	if (lpszLogFile)
		strncpy(m_szLogFile, lpszLogFile, MAX_PATH);
}

LPCSTR CItemPicker::GetLogFilePath() const
{
	return m_szLogFile;
}

BOOL CItemPicker::IsValuableItem(LPCSTR lpszItemCode, BYTE iQuality)
{
	// Valueable items: Unique, set, runes
	if (lpszItemCode == NULL)
		return FALSE;

	if (iQuality >= ITEM_QUALITY_SET)
		return TRUE; // set, rare, unique, crafted

	// good runes (12+)
	if (lpszItemCode[0] == 'r' && atoi(lpszItemCode + 1) > 12)
		return TRUE;

	// small charm, Grand charm
	if (!::strcmp(lpszItemCode, "cm1") || !::strcmp(lpszItemCode, "cm3"))
		return TRUE;

	return FALSE;
}

BOOL CItemPicker::CanItemGotoBelt(LPCSTR lpszItemCode) const
{
	for (BYTE col = 0; col < 4; col++)
	{
		// check each belt column to see if this item can go directly into the belt
		DWORD dwItemID = me->GetBeltItem(col);
		if (dwItemID == 0)
			return TRUE; // first row empty, ok

		if (me->GetAvailableBeltSlots(col))
		{
			char szCode[4] = "";
			server->GetItemCode(dwItemID, szCode, 3);
			if (!::memcmp(szCode, lpszItemCode, 2))
				return TRUE; // first row item is with same type and there are empty slots on this column, ok
		}
	}

	return FALSE;
}

BOOL CItemPicker::ProcessItemQueue(ItemList &queue)
{
	if (!queue.Lock(MAX_LOCK_WAIT))
	{
		server->GamePrintError("Pickit: Item queue lock failed. Item not processed.");
		return FALSE;
	}

	BOOL bProcessed = FALSE;
	POSITION pos = queue.GetHeadPosition();
	while (pos)
	{
		POSITION cur = pos;
		SIMPLEITEM& si = queue.GetNext(pos);
		const DWORD DISTANCE = me->GetDistanceFrom(si.x, si.y);
		if (DISTANCE > m_dwRadius)
			continue; // ignore items out of range

		if (!IsNeedPickup(si))
		{
			// item is no longer wanted (inventory gold amount changed, belt potion states changed, etc)
			// usually this will only happen to junky items
			queue.RemoveAt(cur);
			continue;
		}

		// pick it up
		if (!me->PickGroundItem(si.dwItemID, m_bShowClientMove))
		{
			queue.RemoveAt(cur);
			//server->GameErrorf("Cannot pickup");
			continue;
		}

		// this item has been there for too long, replace it to the rear-end to avoid
		// starvation of other items
		si.nRetry++;
		if (si.nRetry * (int)m_dwDelay > 2500)
		{
			si.nRetry = 0;
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

	m_aValuable.Lock(MAX_LOCK_WAIT);
	server->GameInfof("Valueables: %d", m_aValuable.GetItemCount());
	pos = m_aValuable.GetHeadPosition();
	while (pos)
	{
		const SIMPLEITEM& si = m_aValuable.GetNext(pos);
		server->GameInfof("%08X, %s", si.dwItemID, si.szItemCode);
	}
	m_aValuable.Unlock();

	m_aJunky.Lock(MAX_LOCK_WAIT);
	server->GameInfof("Junks: %d", m_aJunky.GetItemCount());
	pos = m_aJunky.GetHeadPosition();
	while (pos)
	{
		const SIMPLEITEM& si = m_aJunky.GetNext(pos);
		server->GameInfof("%08X, %s", si.dwItemID, si.szItemCode);
	}
	m_aJunky.Unlock();
}

int CItemPicker::GetQueueItemCount(ItemList &list, BOOL bInRadiusOnly)
{
	if (!list.Lock(500))
	{
		server->GamePrintError("Pickit: Item queue lock failed. Item not counted.");
		list.Unlock();
		return 0;
	}

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

void CItemPicker::SetMinGoldAmount(DWORD dwAmount)
{
	m_dwMinGoldAmount = dwAmount;
}

DWORD CItemPicker::GetMinGoldAmount() const
{
	return m_dwMinGoldAmount;
}

void CItemPicker::TickProc(LPVOID lpParam, LPCVBYTE lpStopFlag)
{
	// check player modes, this is an excellent idea from d2jsp snagit script
	DWORD dwMode = me->GetMode();
	if (dwMode == 7 || dwMode == 8 || (dwMode >= 10 && dwMode <= 16) || dwMode == 18)
		return;

	CItemPicker* pPicker = (CItemPicker*)lpParam;
	if (pPicker == NULL || pPicker->m_dwRadius == 0 || pPicker->m_bPaused)
		return;

	// First process valuable items, junky items are processed only if no valuable
	// items are processed at this circle. 
	if (!pPicker->ProcessItemQueue(pPicker->m_aValuable))
		pPicker->ProcessItemQueue(pPicker->m_aJunky);
}

void CItemPicker::OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == UM_DESTROY && lpUnit->dwUnitType == UNIT_TYPE_ITEM)
	{
		BOOL bFound = RemoveFromQueue(m_aValuable, lpUnit->dwUnitID);
		if (!bFound)
			bFound = RemoveFromQueue(m_aJunky, lpUnit->dwUnitID);

		//server->GameInfof("%d removed", bFound);
		return;
	}

	if (nMessage == UM_ITEMEVENT && (wParam == ITEM_EVENT_DROP || wParam == ITEM_EVENT_NEW_GROUND || wParam == ITEM_EVENT_OLD_GROUND))
	{		
		BOOL bValuable = FALSE;
		const ITEMCONFIG* pCfg = FindItem(lpUnit, wParam, m_tempItem, bValuable);
		if (pCfg == NULL)
			return;	// Not configured

		if (pCfg->szDescription[0])
		{
			// Announce it
			server->GamePrintInfo(pCfg->szDescription);
			// need to write in log?
			if (m_szLogFile[0])
				WriteLog(m_szLogFile, pCfg->szDescription, TRUE);
		}

		if (!pCfg->iPickup)
			return; // item not wanted

		if (bValuable)
		{
			if (m_aValuable.Lock(MAX_LOCK_WAIT))
			{
				m_aValuable.InsertRear(m_tempItem); // good items			
				//server->GameInfof("valuable added %s, current queued %d", m_tempItem.szItemCode, m_aValuable.GetItemCount());
			}
			else
			{
				server->GamePrintError("Pickit: Item queue(valuable) lock failed. Item not inserted.");
			}

			m_aValuable.Unlock();
		}
		else
		{
			if (m_aJunky.Lock(MAX_LOCK_WAIT))
			{
				m_aJunky.InsertRear(m_tempItem); // junks, potions, gold, gems, normal items etc
				//server->GameInfof("Jnky added %s, current queued %d", m_tempItem.szItemCode, m_aJunky.GetItemCount());
			}	
			else
			{
				server->GamePrintError("Pickit: Item queue(junky) lock failed. Item not inserted.");
			}

			m_aJunky.Unlock();
		}

		return;
	}
}

void CItemPicker::OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == PM_INVENTORYFULL)
	{
		// notify the client
		m_nInventoryFullState++; // we shall retry a few more times	
		if (m_nInventoryFullState >= INVENTORYFULL_RETRY)
		{
			server->GamePrintInfo("Inventory full!");
			m_bPaused = TRUE; // disable item pick immediately if inventory is full
				
			if (m_nInventoryFullState == INVENTORYFULL_RETRY)
			{
				// execute callback commands
				CStringParse sp(m_lpszFullCmds, ";");
				POSITION pos = sp.GetStartPosition();
				while (pos)
				{
					char szCmd[256] = "";
					sp.GetNextString(pos, szCmd, 255);
					server->GameCommandLine(szCmd);
				}
			}
		}
	}
}

LPCITEMCONFIG CItemPicker::ExamConfigItem(LPCGAMEUNIT lpUnit, const ITEMCONFIG &config, SIMPLEITEM &si, BYTE iType, BOOL &bValuable) const
{
	if (config.iType != ITEM_UNSET && config.iType != iType) 
	{
		//server->GameErrorf("Type not match(%d : %d).", config.iType, iType);
		return NULL;
	}

	BYTE iQuality = server->GetItemQuality(lpUnit->dwUnitID);
	if (config.iQuality != ITEM_UNSET && config.iQuality != iQuality)
	{
		//server->GameErrorf("Quality not match(%d : %d).", config.iQuality, iQuality);
		return NULL;
	}

	DWORD dwFlags = server->GetItemAttributes(lpUnit->dwUnitID);
	if (config.iIdentified != ITEM_UNSET && !!config.iIdentified != !!(dwFlags & ITEM_IDENTIFIED))
	{
		//server->GameErrorf("Identified not match(%d : %d).", !!config.iIdentified, !!(dwFlags & ITEM_IDENTIFIED));
		return NULL;
	}

	if (config.iEthereal != ITEM_UNSET && !!config.iEthereal != !!(dwFlags & ITEM_ETHEREAL))
	{
		//server->GameErrorf("Ethereal not match(%d : %d).", !!config.iEthereal, !!(dwFlags & ITEM_ETHEREAL));
		return NULL;
	}

	BYTE iSockets = server->GetItemSockets(lpUnit->dwUnitID);
	if (config.iSockets != ITEM_UNSET && config.iSockets != iSockets)
	{
		//server->GameErrorf("Sockets not match(%d : %d).", config.iSockets, iSockets);
		return NULL;
	}
		
	si.dwGoldAmount = 0;
	if (IsGold(config.szItemCode))
	{
		si.dwGoldAmount = server->GetUnitStat(lpUnit, STAT_GOLD);
		if (si.dwGoldAmount < m_dwMinGoldAmount)
		{
			//server->GameErrorf("Gold amount not match(%d : %d).", m_dwMinGoldAmount, si.dwGoldAmount);
			return NULL; // gold amount
		}
	}

	// matched, get the location
	si.dwItemID = lpUnit->dwUnitID;
	POINT pt = server->GetUnitPosition(lpUnit);
	si.x = pt.x;
	si.y = pt.y;
	::strcpy(si.szItemCode, config.szItemCode);
	bValuable = IsValuableItem(si.szItemCode, iQuality);
	return &config;
}
