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
#define MAX_RETRY_TIME			2500

BOOL operator==(const SIMPLEITEM& lhs, const SIMPLEITEM& rhs)
{
	return lhs.dwItemID == rhs.dwItemID;
}

CItemPicker::CItemPicker()
{
	InitAttributes(); // Initialization

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
	m_bDebug = FALSE;
}

CItemPicker::~CItemPicker()
{
	if (m_lpszFullCmds != NULL)
		delete [] m_lpszFullCmds;
}

void CItemPicker::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
}

void CItemPicker::OnGameLeave()
{
	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
}

void CItemPicker::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	if (aPacket == NULL || aLen == 0)
		return; // Verify packet

	// TODO: Add your implementation here.
	
	//////////////////////////////////////////////////////////
	// Item Destroyed by the Game
	//////////////////////////////////////////////////////////
	if(aPacket[0] == 0x0a && aPacket[1] == 0x04)
	{	
		// remove it from our item queue if it was there
		DWORD dwID = 0;
		::memcpy(&dwID, aPacket + 2, 4);
		int nCount = RemoveFromQueue(dwID);
		if (m_bDebug)
		{
			if (nCount)
				server->GameInfof("Item ID %08X removed from queue(%d)", dwID, nCount);
			else
				server->GameErrorf("Item ID %08X not found in the queue!", dwID);
		}
	}
	
	//////////////////////////////////////////////////////////
	// Item Dropped
	//////////////////////////////////////////////////////////
	if (IsItemDropPacket(aPacket))
	{
		ITEM item;
		if (!server->ParseItem(aPacket, aLen, &item))
		{
			return; // not an item
		}

		//if (IsItemInQueue(item.dwItemID))
		//	return; // already queued

		const ITEMCONFIG* pCfg = FindItem(item);
		if (pCfg == NULL)
		{
			return;	// Not configured
		}

		if (pCfg->szDescription[0])
		{
			// Announce it
			server->GamePrintInfo(pCfg->szDescription);
			// need to write in log?
			if (m_szLogFile[0])
				WriteLog(m_szLogFile, pCfg->szDescription, TRUE);
		}

		if (!pCfg->iPickup)
		{
			return; // item not wanted
		}
		
		// the user wanted this item, so add it into our item queues
		SIMPLEITEM si;
		si.dwItemID = item.dwItemID;
		si.x = item.wPositionX;
		si.y = item.wPositionY;
		si.dwGoldAmount = item.dwGoldAmount;
		si.nRetryCount = 0;
		strcpy(si.szItemCode, item.szItemCode);

		if (IsValuableItem(&item))
		{
			m_aValuable.Lock();
			m_aValuable.InsertRear(si); // good items
			m_aValuable.Unlock();
			//server->GameInfof("valuable added %s", si.szItemCode);
		}
		else
		{
			m_aJunky.Lock();
			m_aJunky.InsertRear(si); // junks, potions, gold, gems, normal items etc
			m_aJunky.Unlock();
			//server->GameInfof("Jnky added %s", si.szItemCode);
		}
	}
}

void CItemPicker::OnGameTimerTick()
{
	// we only check every m_dwDelay ms
	//m_dwTicker += 100;
	//if (m_dwTicker % m_dwDelay)
	//	return;	

	// Do nothing if paused or radius is zero
	if (m_dwRadius == 0 || m_bPaused || ::GetTickCount() - m_dwLastPick < m_dwDelay)
		return;		

	if (!ProcessItemQueue(m_aValuable))
		ProcessItemQueue(m_aJunky);	
}

void CItemPicker::InitAttributes()
{
	// TODO: Add extra initialization here.
	m_nInventoryFullState = 0;
	m_bPaused = FALSE;
//	m_dwTicker = 0;
	m_dwLastPick = 0;
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
		ini.GetString(pSection, "Code", sz, ITEM_CODE_LEN + 3);
		if (sz[0] == '\"')
			strncpy(cfg.szItemCode, &sz[1], ITEM_CODE_LEN);
		else
			strncpy(cfg.szItemCode, sz, ITEM_CODE_LEN);
		
		if (strlen(cfg.szItemCode) != ITEM_CODE_LEN)
			continue;

		// Item Description
		ini.GetString(pSection, "Description", cfg.szDescription, ITEM_DESC_LEN);

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

		// item should have sockets, doesnt matter how many unlesss NumSockets is used
		cfg.iSocketed = (BYTE)ini.GetInt(pSection, "HasSockets", ITEM_UNSET);

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
			cfg.iQuality = ITEM_LEVEL_RARE;
		else if(stricmp(sz, "unique") == 0)
			cfg.iQuality = ITEM_LEVEL_UNIQUE;
		else if(stricmp(sz, "set") == 0)
			cfg.iQuality = ITEM_LEVEL_SET;
		else if(stricmp(sz, "magic") == 0)
			cfg.iQuality = ITEM_LEVEL_MAGIC;
		else if(stricmp(sz, "craft") == 0)
			cfg.iQuality = ITEM_LEVEL_CRAFT;
		else if(stricmp(sz, "normal") == 0)
			cfg.iQuality = ITEM_LEVEL_NORMAL;
		else if(stricmp(sz, "superior") == 0)
			cfg.iQuality = ITEM_LEVEL_SUPERIOR;
		else if(stricmp(sz, "lowquality") == 0)
			cfg.iQuality = ITEM_LEVEL_INFERIOR;
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

		// insert this entry to the config queue
		m_aConfig.Lock();
		m_aConfig.InsertRear(cfg); // add this entry
		m_aConfig.Unlock();
	}
	
	// the large buffer is no longer needed
	delete [] pszBuffer;
	pszBuffer = NULL;	

	return TRUE;
}

void CItemPicker::CleanQueue()
{
	m_aValuable.Lock();
	m_aValuable.RemoveAll();
	m_aValuable.Unlock();

	m_aJunky.Lock();
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

/*
BOOL CItemPicker::DoPickup(const SIMPLEITEM& item)
{
	if (me->GetDistanceFrom(item.x, item.y) > 5 && m_bShowClientMove)
		me->MoveTo(item.x, item.y, FALSE);

	BYTE aPacket[13] = { 0 };
	aPacket[0] = 0x16;
	aPacket[1] = 0x04;
	::memcpy(aPacket + 5, &item.dwItemID, 4);

	return server->GameSendPacketToServer(aPacket, 13);
}
*/

const ITEMCONFIG* CItemPicker::FindItem(const ITEM &item)
{
	m_aConfig.Lock();
	POSITION pos = m_aConfig.GetHeadPosition();

	while (pos)
	{
		const ITEMCONFIG& config = m_aConfig.GetNext(pos);

		if (stricmp(config.szItemCode, item.szItemCode) != 0)
			continue;

		if (config.iQuality != ITEM_UNSET && config.iQuality != item.iQuality)
		{
			DebugMessage("Quality not match.");
			continue;
		}

		if (config.iSocketed != ITEM_UNSET && (BOOL)config.iSocketed != item.iSocketNumber > 0)
		{
			DebugMessage("Socketed not match.");
			continue;
		}

		if (config.iIdentified != ITEM_UNSET && config.iIdentified != item.iIdentified)
		{
			DebugMessage("Identified not match.");
			continue;
		}

		if (config.iEthereal != ITEM_UNSET && config.iEthereal != item.iEthereal)
		{
			DebugMessage("Ethereal not match.");
			continue;
		}

		if (config.iType != ITEM_UNSET && config.iType != item.iType) 
		{
			DebugMessage("Type not match.");
			continue;
		}

		m_aConfig.Unlock();
		return &config;
	}

	m_aConfig.Unlock();
	return NULL;
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
	m_aConfig.Lock();
	int nCount = m_aConfig.GetItemCount() - 3; // minus 3 special items
	m_aConfig.Unlock();
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

int CItemPicker::RemoveFromQueue(DWORD dwItemID)
{
	SIMPLEITEM si;
	si.dwItemID = dwItemID;
	int nCount = 0;

	m_aValuable.Lock();
	nCount = m_aValuable.RemoveMatchedItems(si);
	m_aValuable.Unlock();

	if (nCount == 0)
	{
		m_aJunky.Lock();
		nCount = m_aJunky.RemoveMatchedItems(si);
		m_aJunky.Unlock();
	}

	return nCount;
}

DWORD CItemPicker::SetDelay(DWORD dwDelay)
{
	// must not be lower than 500 ms, otherwise connection might be interrupted
	// and must be dividable by 100
	DWORD dwMod = dwDelay % 100;
	dwDelay -= dwMod;

	if (dwMod >= 50)
		dwDelay += 100;

	m_dwDelay = max(PICKIT_DELAY_MIN, dwDelay);
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
	m_aConfig.Lock();

	m_aConfig.RemoveAll();

	// Always add special items
	ITEMCONFIG cfg = { 0 };
	cfg.iEthereal = ITEM_UNSET;
	cfg.iIdentified = ITEM_UNSET;
	cfg.iQuality = ITEM_UNSET;
	cfg.iPickup = 1;
	cfg.iSocketed = ITEM_UNSET;
	cfg.iType = ITEM_UNSET;	

	strncpy(cfg.szItemCode, "gld", ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Gold

	strncpy(cfg.szItemCode, "rvl", ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Full rejuv

	strncpy(cfg.szItemCode, "rvs", ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Rejuv	
	m_aConfig.Unlock();
}

void CItemPicker::OnInventoryFull()
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

BOOL CItemPicker::IsGold(LPCSTR lpszItemCode)
{
	return lpszItemCode != NULL && stricmp(lpszItemCode, "gld") == 0;
}

BOOL CItemPicker::IsRejuv(LPCSTR lpszItemCode)
{
	return lpszItemCode != NULL && stricmp(lpszItemCode, "rvs") == 0;
}

BOOL CItemPicker::IsFullRejuv(LPCSTR lpszItemCode)
{
	return lpszItemCode != NULL && stricmp(lpszItemCode, "rvl") == 0;
}

void CItemPicker::DebugItem(const ITEM &item) const
{
	if (!m_bDebug)
		return;
	
	DebugMessage("==========================");

	server->GameInfof("Item ID %08X", item.dwItemID);
	server->GameInfof("Item Code %s", item.szItemCode);
	server->GameInfof("Item Type %d", item.iType);
	server->GameInfof("Identified %d", item.iIdentified);
	server->GameInfof("Ethereal %d", item.iEthereal);
	server->GameInfof("Sockets %d", item.iSocketNumber);
	server->GameInfof("ilevel %d", item.iLevel);
	server->GameInfof("Quality %d", item.iQuality);
				
	DebugMessage("==========================");
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

void CItemPicker::DebugMessage(LPCSTR lpszMessage) const
{
	if (m_bDebug)
		server->GamePrintInfo(lpszMessage);
}

BOOL CItemPicker::IsValuableItem(LPCITEM lpItem)
{
	// Valueable items: Unique, set, runes
	if (lpItem == NULL)
		return FALSE;

	if (lpItem->iQuality >= ITEM_LEVEL_MAGIC)
		return TRUE; // unique, set, magic

	// good runes
	return (lpItem->szItemCode[0] == 'r' || lpItem->szItemCode[0] == 'R')
		&& ::isdigit(lpItem->szItemCode[1])
		&& ::isdigit(lpItem->szItemCode[2]);
}

BOOL CItemPicker::CanItemGotoBelt(LPCSTR lpszItemCode) const
{
	for (BYTE col = 0; col < 4; col++)
	{
		// check each belt column to see if this item can go directly into the belt
		LPCITEM p = me->GetBeltItem(0, col);
		if (p == NULL)
			return TRUE; // first row empty, ok

		if (me->GetAvailableBeltSlots(col) && !::memcmp(p->szItemCode, lpszItemCode, 2))
			return TRUE; // first row item is with same type and there are empty slots on this column, ok
	}

	return FALSE;
}

void CItemPicker::SetDebug(BOOL bDebug)
{
	m_bDebug = bDebug;
}

BOOL CItemPicker::IsDebug() const
{
	return m_bDebug;
}

BOOL CItemPicker::ProcessItemQueue(ItemList &queue)
{
	POSITION pos = FindFirstInRangeItem(queue);
	if (pos == NULL)
		return FALSE;

	queue.Lock();

	SIMPLEITEM& item = queue.GetAt(pos);	

	if (!IsNeedPickup(item))
	{
		// item is no longer wanted (inventory gold amount changed, belt potion states changed, etc)
		// this will only happen to junky items
		queue.RemoveAt(pos);
		queue.Unlock();
		return FALSE;
	}

	if (!me->PickGroundItem(item.dwItemID, m_bShowClientMove))
	{
		// pickup packet sent failed (this should never happen, but just in case)
		queue.RemoveAt(pos);
		queue.Unlock();
		return FALSE;
	}

	// item verified ok, and the pickup packet sent successfully
	// if this item has been retried for longer than 2.5 seconds, replace it to the rear of the queue
	item.nRetryCount++;
	if (item.nRetryCount * m_dwDelay > MAX_RETRY_TIME)
	{
		// maximum retry for this item exceeded	
		item.nRetryCount = 0;
		queue.MoveToRear(pos);
	}

	queue.Unlock();
	m_dwLastPick = ::GetTickCount();
	return TRUE;
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
		server->GameInfof("%08X, %s", si.dwItemID, si.szItemCode);
	}
	m_aValuable.Unlock();

	m_aJunky.Lock();
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
	list.Lock();
	if (!bInRadiusOnly)
	{
		list.Unlock();
		return list.GetItemCount();
	}

	int nCount = 0;
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

POSITION CItemPicker::FindFirstInRangeItem(ItemList &list)
{
	list.Lock();

	const int TOTAL = list.GetItemCount();
	int nCount = 0;
	POSITION pos = list.GetHeadPosition();
	while (pos)
	{
		POSITION temp = pos;
		SIMPLEITEM& si = list.GetNext(pos);
		if (me->GetDistanceFrom(si.x, si.y) <= m_dwRadius)
		{
			list.Unlock();
			return temp;
		}

		// if item is too far away, replace it to the rear of the queue
		nCount++;
		if (nCount >= TOTAL)
		{
			list.Unlock();
			return NULL;
		}

		si.nRetryCount = 0;
		list.MoveToRear(temp);				
	}

	list.Unlock();
	return NULL;
}

BOOL CItemPicker::IsItemDropPacket(const BYTE *aPacket)
{
	if (aPacket == NULL)
		return FALSE;

	if (aPacket[0] != 0x9c)
		return FALSE;

	return aPacket[1] == ITEM_ACTION_DROP
		|| aPacket[1] == ITEM_ACTION_NEW_GROUND
		|| aPacket[1] == ITEM_ACTION_OLD_GROUND;
}

BOOL CItemPicker::IsItemInQueue(DWORD dwID)
{
	SIMPLEITEM si;
	si.dwItemID = dwID;

	POSITION pos = NULL;
	m_aValuable.Lock();
	pos = m_aValuable.Find(si);
	m_aValuable.Unlock();

	if (pos)
		return TRUE;

	m_aJunky.Lock();
	pos = m_aJunky.Find(si);
	m_aJunky.Unlock();

	return pos != NULL;
}
