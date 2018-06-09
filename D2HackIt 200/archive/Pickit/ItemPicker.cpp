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
#define ITEM_UNSET				0xff

#define	ITEM_ITEMTYPE_HELM				0x00
#define	ITEM_ITEMTYPE_ARMOR				0x01
#define	ITEM_ITEMTYPE_WEAPON			0x05
#define	ITEM_ITEMTYPE_BOW				0x06
#define	ITEM_ITEMTYPE_SHIELD			0x07
#define	ITEM_ITEMTYPE_EXPANSION			0x0A // heads
#define	ITEM_ITEMTYPE_OTHER				0x10

struct CFGDATA
{
	CItemPicker* pPicker;
	CIni* pIni;
};

enum { MISC_NO = 0, MISC_GOLD, MISC_REJUV, MISC_FULLREJUV };
int IsMiscItem(LPCSTR lpszItemCode);
int IsMiscItem(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return MISC_NO;

	if (!::memcmp(lpszItemCode, "gld", 3))
		return MISC_GOLD;

	if (!::memcmp(lpszItemCode, "rvs", 3))
		return MISC_REJUV;

	if (!::memcmp(lpszItemCode, "rvl", 3))
		return MISC_FULLREJUV;

	return MISC_NO;	
}

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
	m_lpszFullCmds = NULL;
	m_bShowClientMove = FALSE;
	m_nGoldMethod = PICKIT_PICK_AUTO;
	m_nRejuvMethod = PICKIT_PICK_NO;
	m_nFullRejuvMethod = PICKIT_PICK_AUTO;
	m_szConfigFile[0] = '\0';
	m_dwRadius = PICKIT_RADIUS_DEFAULT;
	m_bTownPick = TRUE;
	m_szLogFile[0] = '\0';
	CleanConfig();
	m_dwMinGoldAmount = 0;
	m_nInventoryFullState = 0;
	m_bPaused = TRUE;
}

CItemPicker::~CItemPicker()
{
	if (m_lpszFullCmds != NULL)
		delete [] m_lpszFullCmds;
}

void CItemPicker::OnGameJoin()
{
	// TODO: Add your implementation here.	
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

	m_nInventoryFullState = 0;
	strncpy(m_szConfigFile, lpszFile, MAX_PATH);	
	CIni ini(lpszFile);
	CFGDATA data;
	data.pIni = &ini;
	data.pPicker = this;
	ini.EnumSections(EnumSectionsProc, (LPVOID)&data);
	m_aConfig1.Sort(); // sort the config1 to turn on binary quick searching!
	m_aConfig2.Sort();

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
	m_dwRadius = dwRadius;
	return m_dwRadius;
}

DWORD CItemPicker::GetRadius() const
{
	return m_dwRadius;
}

LPCITEMCONFIG CItemPicker::FindItem(LPCGAMEUNIT lpUnit, BYTE iItemType, SIMPLEITEM& si, BOOL& bValuable) const
{	
	LPCITEMCONFIG p = NULL;
	int nIndex = -1;

	ITEMCONFIG config;
	::memcpy(config.szItemCode, si.szItemCode, 3);
	config.szItemCode[3] = 0;	
	
	// Search in m_config1, this will be a binary fast searching
	nIndex = m_aConfig1.Find(config);
	if (nIndex != -1)
		p = ExamConfigItem(lpUnit, m_aConfig1.GetAt(nIndex), si, iItemType, bValuable);

	if (p)
		return p;
	
	// Search in m_config2, this will be a linear slow searching but number of elements
	// in m_config2 are usually very few, if there are any at all.
	for (int i = 0; p == NULL && i < m_aConfig2.GetSize(); i++)
	{
		const ITEMCONFIG& cfg = m_aConfig1.GetAt(i);
		int nCmp = ::memcmp(cfg.szItemCode, config.szItemCode, 3);
		if (nCmp > 0)
			break;

		if (nCmp == 0)
			p = ExamConfigItem(lpUnit, cfg, si, iItemType, bValuable);
	}		

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
	return m_aConfig1.GetSize() + m_aConfig2.GetSize();
}

BOOL CItemPicker::IsNeedPickup(const SIMPLEITEM& item) const
{
	if (!m_bTownPick && me->IsInTown())
		return FALSE;

	// For special items check for m_bAllowxxx
	int nMisc = IsMiscItem(item.szItemCode);	
	if (nMisc == MISC_GOLD)
		return m_nGoldMethod == PICKIT_PICK_YES || (m_nGoldMethod == PICKIT_PICK_AUTO && item.dwGoldAmount + 5000 < me->GetInventoryGoldLimit() - me->GetStat(STAT_GOLD));

	if (nMisc == MISC_REJUV)
		return m_nRejuvMethod == PICKIT_PICK_YES || (m_nRejuvMethod == PICKIT_PICK_AUTO && CanItemGotoBelt(item.szItemCode));

	if (nMisc == MISC_FULLREJUV)
		return m_nFullRejuvMethod == PICKIT_PICK_YES || (m_nFullRejuvMethod == PICKIT_PICK_AUTO && CanItemGotoBelt(item.szItemCode));

	return TRUE;
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
	m_aConfig1.RemoveAll();
	m_aConfig2.RemoveAll();
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
	queue.Lock();
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
			continue;
		
		// pick it up
		if (!me->PickGroundItem(si.dwItemID, m_bShowClientMove))
			continue;

		si.nRetry++;
		if (si.nRetry > 10)
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

void CItemPicker::OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == UM_DESTROY && lpUnit->dwUnitType == UNIT_TYPE_ITEM)
	{
		RemoveItemFromQueue(lpUnit->dwUnitID);		
		return;
	}

	if (nMessage == UM_ITEMEVENT && (wParam == ITEM_EVENT_DROP || wParam == ITEM_EVENT_NEW_GROUND || wParam == ITEM_EVENT_OLD_GROUND))
	{			
		BOOL bValuable = FALSE;
		
		SIMPLEITEM si = {0};
		si.dwItemID = lpUnit->dwUnitID;
		server->GetItemCode(lpUnit->dwUnitID, si.szItemCode, 3);
		int nMisc = IsMiscItem(si.szItemCode);
		if (nMisc)
		{
			if (nMisc == MISC_GOLD)
			{
				si.dwGoldAmount = server->GetUnitStat(lpUnit, STAT_GOLD);
				if (si.dwGoldAmount < m_dwMinGoldAmount)
					return;
			}

			POINT pt = server->GetUnitPosition(lpUnit);
			si.x = (WORD)pt.x;
			si.y = (WORD)pt.y;
			AddToQueue(si, m_aJunky);
			return;
		}
			
		const ITEMCONFIG* pCfg = FindItem(lpUnit, wParam, si, bValuable);
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
			AddToQueue(si, m_aValuable);
		else
			AddToQueue(si, m_aJunky);

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
		
	// matched, get the location
	POINT pt = server->GetUnitPosition(lpUnit);
	si.x = pt.x;
	si.y = pt.y;
	bValuable = IsValuableItem(si.szItemCode, iQuality);
	return &config;
}

BOOL CItemPicker::EnumSectionsProc(LPCTSTR lpSection, LPVOID lpParam)
{
	CFGDATA* pData = (CFGDATA*)lpParam;
	if (pData == NULL || pData->pIni == NULL || pData->pPicker == NULL)
		return FALSE;

	ITEMCONFIG cfg = {0};
	char sz[256] = "";
				
	// ItemCode should be 3 chars long
	// Code="xxx" or Code=xxx
	pData->pIni->GetString(lpSection, "Code", sz, 3 + 3);
	if (sz[0] == '\"')
		strncpy(cfg.szItemCode, &sz[1], 3);
	else
		strncpy(cfg.szItemCode, sz, 3);
		
	if (strlen(cfg.szItemCode) != 3)
		return TRUE;

	// Item Description
	pData->pIni->GetString(lpSection, "Description", cfg.szDescription, 128);

	// replace "%c" to "ÿc" so users do not need to put that character in their ini files
	// they can use "%c" instead.
	for (int i = 0; cfg.szDescription[i] != '\0'; i++)
	{
		if (cfg.szDescription[i] == '%' && cfg.szDescription[i + 1] == 'c')
			cfg.szDescription[i] = 'ÿ';
	}

	// identified
	cfg.iIdentified = (BYTE)pData->pIni->GetInt(lpSection, "Identified", ITEM_UNSET);

	// ethereal
	cfg.iEthereal = (BYTE)pData->pIni->GetInt(lpSection, "isEthereal", ITEM_UNSET);

	// socket number
	cfg.iSockets = (BYTE)pData->pIni->GetInt(lpSection, "Sockets", ITEM_UNSET);

	// dont pickup item.  useful if you only want to be told of the item drop
	cfg.iPickup = (BYTE)pData->pIni->GetInt(lpSection, "Pickup", 1);

	if (IsMiscItem(cfg.szItemCode))
		return TRUE; // these items are special, we do not load them by the normal way...

	if (!cfg.iPickup && ::strlen(cfg.szDescription) == 0)
		return TRUE; // no pickup no announce

	// item level
	pData->pIni->GetString(lpSection, "Level", sz, 64);

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
	pData->pIni->GetString(lpSection, "Type", sz, 64);
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
	if (pData->pPicker->m_aConfig1.Find(cfg) == -1)
		pData->pPicker->m_aConfig1.Add(cfg);
	else
		pData->pPicker->m_aConfig2.Add(cfg);

	return TRUE;
}

BOOL CItemPicker::AddToQueue(const SIMPLEITEM &si, ItemList &queue)
{
	queue.Lock();
	queue.InsertFront(si);
	queue.Unlock();
	return TRUE;
}

BOOL CItemPicker::RemoveItemFromQueue(DWORD dwItemID)
{
	POSITION pos = NULL;
	POSITION cur = NULL;
	BOOL bRemoved = FALSE;

	m_aValuable.Lock();
	pos = m_aValuable.GetHeadPosition();
	while (pos)
	{
		cur = pos;
		const SIMPLEITEM& si = m_aValuable.GetNext(pos);
		if (si.dwItemID == dwItemID)
		{
			bRemoved = TRUE;			
			m_aValuable.RemoveAt(cur);
			break;
		}
	}

	m_aValuable.Unlock();
	if (bRemoved)
		return TRUE;

	m_aJunky.Lock();
	pos = m_aJunky.GetHeadPosition();
	while (pos)
	{
		cur = pos;
		const SIMPLEITEM& si = m_aJunky.GetNext(pos);
		if (si.dwItemID == dwItemID)
		{
			bRemoved = TRUE;
			m_aJunky.RemoveAt(cur);
			break;
		}
	}
	m_aJunky.Unlock();
	return bRemoved;
}

void CItemPicker::OnGameTimerTick()
{
	if (!ProcessItemQueue(m_aValuable))
	{
		//server->GameInfof("no valuable items queued, not processing junkies...");
		ProcessItemQueue(m_aJunky);
	}
}
