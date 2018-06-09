//////////////////////////////////////////////////////////////////////
// PickitPlayer.cpp: implementation of the CPickitPlayer class.
//////////////////////////////////////////////////////////////////////

#include "PickitPlayer.h"
#include <time.h>
#include <stdio.h>

#define INVENTORYFULL_RETRY		3

CPickitPlayer::CPickitPlayer()
{
	InitAttributes(); // Initialization

	m_lpszFullCmds = NULL;
	m_bShowClientMove = TRUE;
	m_nGoldMethod = PICKIT_PICK_AUTO;
	m_nRejuvMethod = PICKIT_PICK_NO;
	m_nFullRejuvMethod = PICKIT_PICK_AUTO;
	m_szConfigFile[0] = _T('\0');
	m_dwDelay = PICKIT_DELAY_DEFAULT;
	m_dwRadius = PICKIT_RADIUS_DEFAULT;
	m_bTownPick = TRUE;
	m_szLogFile[0] = _T('\0');
	CleanConfig();
}

CPickitPlayer::~CPickitPlayer()
{
	if (m_lpszFullCmds != NULL)
		delete [] m_lpszFullCmds;
}

void CPickitPlayer::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	CD2Player::OnGameJoin(pGame); // Always call base class's method first!!!

	InitAttributes(); // Initialize attributes upon new game.

	// TODO: Add your implementation here.
}

void CPickitPlayer::OnGameLeave()
{
	CD2Player::OnGameLeave(); // Always call base class's method first!!!

	InitAttributes(); // Initialize attributes upon leave.

	// TODO: Add your implementation here.
}

void CPickitPlayer::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	CD2Player::OnGamePacketBeforeReceived(aPacket, aLen); // Always call base class's method first!!!
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
		m_bBusy = TRUE;
		RemoveFromQueue(dwID);
		m_bBusy = FALSE;
		DebugMessage(_T("Item gone."));
	}
	
	//////////////////////////////////////////////////////////
	// Item Dropped
	//////////////////////////////////////////////////////////
	if (aPacket[0] == 0x9c)
	{
		if (aPacket[1] == ITEM_ACTION_DROP
			|| aPacket[1] == ITEM_ACTION_NEW_GROUND
			|| aPacket[1] == ITEM_ACTION_OLD_GROUND)
		{
			ITEM item = { 0 };
			if (::D2ParseItem(aPacket, aLen, item))
			{
				const ITEMCONFIG* pCfg = FindItem(item);
				if (pCfg == NULL)
				{
					DebugMessage(_T("Item not wanted."));
					return;	
				}
		
				// the user wanted this item, so add it into our item queue
				m_aItemQueue.InsertRear(item);
				if (pCfg->szDescription[0] != _T('\0'))
				{
					// need to write in log?
					if (m_szLogFile[0] != _T('\0'))
						WriteLog(m_szLogFile, pCfg->szDescription, TRUE);

					OnItemDrop(&item, pCfg->szDescription);
					DebugMessage(_T("Item dropped."));
					NotifyD2Message(PHM_ITEMDROP, (WPARAM)&item, (LPARAM)pCfg->szDescription);
				}
			}
		}
	}
}

void CPickitPlayer::OnGameTimerTick()
{
	CD2Player::OnGameTimerTick(); // Always call base class's method first!!!

	// TODO: Add your implementation here.
	
	static DWORD dwTick = 0; // For far items
	dwTick++;
	dwTick %= 5;

	if (m_bBusy || m_dwRadius == 0 || m_bPaused)
		return;	

	// only send packet if delay is elapsed
	DWORD dwNow = ::GetTickCount();
	if (m_dwLastProc + m_dwDelay > dwNow)
		return; // Delay not elapsed yet, must wait

	int nCount = 0;
	if (m_curPos == NULL)
		m_curPos = m_aItemQueue.GetHeadPosition();

	while (m_curPos && nCount < 3) // no more than 3 items a time
	{
		const ITEM& item = m_aItemQueue.GetNext(m_curPos);		
		if (IsNeedPickup(item, TRUE) && DoPickup(item, dwTick))
			nCount++;
	}

	if (nCount)
		m_dwLastProc = dwNow;

	return;
	
}

void CPickitPlayer::InitAttributes()
{
	CD2Abstract::InitAttributes(); // Always call base class's method first!!!

	// TODO: Add extra initialization here.
	m_curPos = NULL;
	m_nInventoryFullState = 0;
	m_bPaused = FALSE;
	m_bBusy = FALSE;
	m_dwLastProc = 0;
	CleanQueue();
}

BOOL CPickitPlayer::LoadConfig(LPCTSTR lpszFile)
{
	if (lpszFile == NULL || *lpszFile == 0)
		return FALSE; // invalid params

	DWORD dwRes = ::GetFileAttributes(lpszFile);
	if (dwRes == -1 || dwRes == FILE_ATTRIBUTE_DIRECTORY)
		return FALSE; // file not exists

	CleanConfig();
	CleanQueue();
	
	m_bBusy = TRUE;	
	m_nInventoryFullState = 0;

	_tcsncpy(m_szConfigFile, lpszFile, MAX_PATH);	
	CIni ini(lpszFile);

	// keey enlarging the buffer unless we make sure all section names
	// are loaded.
	DWORD dwBufSize = 1024;
	TCHAR *pszBuffer = new TCHAR[dwBufSize];
	DWORD dwCopied = ini.GetSectionNames(pszBuffer, dwBufSize - 1);
	while (dwCopied + 128 >= dwBufSize)
	{
		dwBufSize += 1024;
		delete [] pszBuffer;
		pszBuffer = new TCHAR[dwBufSize];
		dwCopied = ini.GetSectionNames(pszBuffer, dwBufSize - 1);
	}	
	
	// iterate through all sections
	ITEMCONFIG cfg = { 0 };
	for (TCHAR* pSection = pszBuffer; pSection[0] != _T('\0'); pSection = &pSection[_tcslen(pSection) + 1])
	{	
		// now query the section and fill in data

		::memset(&cfg, 0, sizeof(ITEMCONFIG));
		TCHAR sz[256] = _T("");
				
		// ItemCode should be 3 chars long
		// Code="xxx" or Code=xxx
		ini.GetString(pSection, _T("Code"), sz, ITEM_CODE_LEN + 3);
		if (sz[0] == _T('\"'))
			_tcsncpy(cfg.szItemCode, &sz[1], ITEM_CODE_LEN);
		else
			_tcsncpy(cfg.szItemCode, sz, ITEM_CODE_LEN);
		
		if (_tcslen(cfg.szItemCode) != ITEM_CODE_LEN)
			continue;

		// Item Description
		ini.GetString(pSection, _T("Description"), cfg.szDescription, ITEM_DESC_LEN);

		// replace "%c" to "ÿc" so users do not need to put that character in their ini files
		// they can use "%c" instead.
		for (int i = 0; cfg.szDescription[i] != '\0'; i++)
		{
			if (cfg.szDescription[i] == '%' && cfg.szDescription[i + 1] == 'c')
				cfg.szDescription[i] = 'ÿ';
		}

		// identified
		cfg.iIdentified = (BYTE)ini.GetInt(pSection, _T("Identified"), ITEM_UNSET);

		// ethereal
		cfg.iEthereal = (BYTE)ini.GetInt(pSection, _T("isEthereal"), ITEM_UNSET);

		// item should have sockets, doesnt matter how many unlesss NumSockets is used
		cfg.iSocketed = (BYTE)ini.GetInt(pSection, _T("HasSockets"), ITEM_UNSET);

		// dont pickup item.  useful if you only want to be told of the item drop
		cfg.iPickup = (BYTE)ini.GetInt(pSection, _T("Pickup"), 1);

		if (IsGold(cfg.szItemCode)
			|| IsRejuv(cfg.szItemCode)
			|| IsFullRejuv(cfg.szItemCode))
		{
			// these items are special, we do not load them by the normal way...
			continue;
		}

		if (!cfg.iPickup && _tcslen(cfg.szDescription) == 0)
			continue; // no pickup no announce

		// item level
		ini.GetString(pSection, _T("Level"), sz, 64);

		if(_tcsicmp(sz, _T("rare")) == 0)
			cfg.iQuality = ITEM_LEVEL_RARE;
		else if(_tcsicmp(sz, _T("unique")) == 0)
			cfg.iQuality = ITEM_LEVEL_UNIQUE;
		else if(_tcsicmp(sz, _T("set")) == 0)
			cfg.iQuality = ITEM_LEVEL_SET;
		else if(_tcsicmp(sz, _T("magic")) == 0)
			cfg.iQuality = ITEM_LEVEL_MAGIC;
		else if(_tcsicmp(sz, _T("craft")) == 0)
			cfg.iQuality = ITEM_LEVEL_CRAFT;
		else if(_tcsicmp(sz, _T("normal")) == 0)
			cfg.iQuality = ITEM_LEVEL_NORMAL;
		else if(_tcsicmp(sz, _T("superior")) == 0)
			cfg.iQuality = ITEM_LEVEL_SUPERIOR;
		else if(_tcsicmp(sz, _T("lowquality")) == 0)
			cfg.iQuality = ITEM_LEVEL_INFERIOR;
		else
			cfg.iQuality = ITEM_UNSET;
	
		// item types
		ini.GetString(pSection, _T("Type"), sz, 64);
		if(_tcsicmp(sz, _T("Helm")) == 0)
			cfg.iType = ITEM_ITEMTYPE_HELM;
		else if(_tcsicmp(sz, _T("Armor")) == 0)
			cfg.iType = ITEM_ITEMTYPE_ARMOR;
		else if(_tcsicmp(sz, _T("Weapon")) == 0)
			cfg.iType = ITEM_ITEMTYPE_WEAPON;
		else if(_tcsicmp(sz, _T("Bow")) == 0)
			cfg.iType = ITEM_ITEMTYPE_BOW;
		else if(_tcsicmp(sz, _T("Shield")) == 0)
			cfg.iType = ITEM_ITEMTYPE_SHIELD;
		else if(_tcsicmp(sz, _T("Other")) == 0)
			cfg.iType = ITEM_ITEMTYPE_OTHER;
		else 
			cfg.iType = ITEM_UNSET;

		// insert this entry to the config queue
		m_aConfig.InsertRear(cfg); // add this entry
	}
	
	// the large buffer is no longer needed
	delete [] pszBuffer;
	pszBuffer = NULL;	
	m_bBusy = FALSE;

	return TRUE;
}

void CPickitPlayer::CleanQueue()
{
	m_bBusy = TRUE;
	m_aItemQueue.RemoveAll();
	m_bBusy = FALSE;
}

DWORD CPickitPlayer::SetRadius(DWORD dwRadius)
{
	m_dwRadius = min(PICKIT_RADIUS_MAX, dwRadius);
	return m_dwRadius;
}

DWORD CPickitPlayer::GetRadius() const
{
	return m_dwRadius;
}

BOOL CPickitPlayer::DoPickup(const ITEM& item, DWORD dwTicker)
{
	if (m_bBusy
		|| item.dwItemID == 0 
		|| !IsGameReady()
		|| m_dwRadius == 0
		|| m_bPaused)
	{
		return FALSE;
	}

	const DWORD Distance = GetDistance(item.wPositionX, item.wPositionY);

	// if item is less then 1/2 pickupradius away send pickup request everytime
	// if item is greater then 1/2 pickupradius away send pickup reqest every 5 function calls
	// ignore items outside pickup radius
	if (Distance <= m_dwRadius)
	{
		if (Distance <= 5 || Distance <= m_dwRadius / 2 || (dwTicker % 5) == 0)
		{
			GROUNDITEM gi = { 0 };
			gi.dwID = item.dwItemID;
			gi.x = item.wPositionX;
			gi.y = item.wPositionY;
			return PickItem(&gi, m_bShowClientMove);
		}
	}	

	return FALSE;
}

const ITEMCONFIG* CPickitPlayer::FindItem(const ITEM &item) const
{
	POSITION pos = m_aConfig.GetHeadPosition();

	while (pos)
	{
		const ITEMCONFIG& config = m_aConfig.GetNext(pos);

		if (_tcsicmp(config.szItemCode, item.szItemCode) != 0)
			continue;

		if (config.iQuality != ITEM_UNSET && config.iQuality != item.iQuality)
		{
			DebugMessage(_T("Quality not match."));
			continue;
		}

		if (config.iSocketed != ITEM_UNSET && (BOOL)config.iSocketed != item.iSocketNumber > 0)
		{
			DebugMessage(_T("Socketed not match."));
			continue;
		}

		if (config.iIdentified != ITEM_UNSET && config.iIdentified != item.iIdentified)
		{
			DebugMessage(_T("Identified not match."));
			continue;
		}

		if (config.iEthereal != ITEM_UNSET && config.iEthereal != item.iEthereal)
		{
			DebugMessage(_T("Ethereal not match."));
			continue;
		}

		if (config.iType != ITEM_UNSET && config.iType != item.iType) 
		{
			DebugMessage(_T("Type not match."));
			continue;
		}

		return &config;
	}

	return NULL;
}

void CPickitPlayer::Enable(BOOL bEnable)
{
	m_bPaused = !bEnable;
	if (bEnable)
		m_nInventoryFullState = 0; // Player may have rearranged his inventory, so reset inventory state
}

BOOL CPickitPlayer::IsEnabled() const
{
	return !m_bPaused;
}	

BOOL CPickitPlayer::Toggle()
{
	Enable(!IsEnabled());
	return IsEnabled();
}

BOOL CPickitPlayer::WriteLog(LPCTSTR lpszFilePath, LPCTSTR lpszString, BOOL bAppend)
{
	if (lpszFilePath == NULL || lpszString == NULL || lpszString[0] == 0)
		return FALSE;	

	FILE *stream = _tfopen(lpszFilePath, bAppend ? _T("a+") : _T("w"));
	if (stream == NULL)
		return FALSE;

	// get time stamp
	TCHAR szTimeString[33] = _T("");
	struct tm *when;
    time_t now;
    ::time(&now);
    when = ::localtime(&now);
	_tcsftime(szTimeString, 32, _T("%m/%d %H:%M:%S"), when);
	if (szTimeString[0] == _T('0'))
		szTimeString[0] = _T(' ');

	const int LEN = _tcslen(szTimeString) + _tcslen(lpszString) + 10;
	TCHAR* pszString = new TCHAR[LEN];
	_stprintf(pszString, _T("%s | %s\n"), szTimeString, lpszString);

	// replace the special color char to '%' so the log file can be read by
	// MFC CStdioFile
	for (int i = 0; pszString[i] != _T('\0'); i++)
	{
		if (pszString[i] == _T('ÿ'))
			pszString[i] = _T('%');
	}

	// write to file
	_ftprintf(stream, _T("%s"), pszString);

	delete [] pszString;
	::fclose(stream);
	return TRUE;
}

LPCTSTR CPickitPlayer::GetConfigFile() const
{
	return m_szConfigFile;
}

int CPickitPlayer::GetQueueItemCount(BOOL bValuableOnly) const
{
	if (!bValuableOnly)
		return m_aItemQueue.GetItemCount();

	int nCount = 0;
	POSITION pos = m_aItemQueue.GetHeadPosition();
	while (pos)
	{
		const ITEM& item = m_aItemQueue.GetNext(pos);
		if (!IsGold(item.szItemCode)
			&& !IsFullRejuv(item.szItemCode)
			&& !IsRejuv(item.szItemCode))
		{
			nCount++;
		}
	}

	return nCount;
}

int CPickitPlayer::GetConfigCount() const
{
	int nCount = m_aConfig.GetItemCount() - 3; // minus 3 special items
	return max(0, nCount);
}

BOOL CPickitPlayer::IsNeedPickup(const ITEM& item, BOOL bCfgPickup)
{
	if (!m_bTownPick && IsPlayerInTown())
		return FALSE;

	// For special items check for m_bAllowxxx
	if (IsGold(item.szItemCode))
		return m_nGoldMethod == PICKIT_PICK_YES || (m_nGoldMethod == PICKIT_PICK_AUTO && item.dwGoldAmount <= GetInventoryGoldSpace());

	if (IsRejuv(item.szItemCode))
		return m_nRejuvMethod == PICKIT_PICK_YES || (m_nRejuvMethod == PICKIT_PICK_AUTO && CanItemGotoBelt(item.szItemCode));

	if (IsFullRejuv(item.szItemCode))
		return m_nFullRejuvMethod == PICKIT_PICK_YES || (m_nFullRejuvMethod == PICKIT_PICK_AUTO && CanItemGotoBelt(item.szItemCode));

	return bCfgPickup;
}

int CPickitPlayer::RemoveFromQueue(DWORD dwItemID)
{
	int nCount = 0;
	POSITION pos = m_aItemQueue.GetHeadPosition();
	while (pos)
	{
		POSITION temp = pos;
		const ITEM& item = m_aItemQueue.GetNext(pos);
		if (item.dwItemID == dwItemID)
		{
			nCount++;
			m_aItemQueue.RemoveAt(temp);
			if (m_curPos == temp)
				m_curPos = pos;
		}
	}
	return nCount;
}

DWORD CPickitPlayer::SetDelay(DWORD dwDelay)
{
	// must not be lower than 100 ms, otherwise connection might be interrupted
	m_dwDelay = max(PICKIT_DELAY_MIN, dwDelay);
	return m_dwDelay;
}

DWORD CPickitPlayer::GetDelay() const
{
	return m_dwDelay;
}

BOOL CPickitPlayer::SetSpecialItemMethod(int nSpecialItem, int nMethod)
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

int CPickitPlayer::GetSpecialItemMethod(int nSpecialItem) const
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

void CPickitPlayer::CleanConfig()
{
	m_bBusy = TRUE;
	m_aConfig.RemoveAll();

	// Always add special items
	ITEMCONFIG cfg = { 0 };
	cfg.iEthereal = ITEM_UNSET;
	cfg.iIdentified = ITEM_UNSET;
	cfg.iQuality = ITEM_UNSET;
	cfg.iPickup = 1;
	cfg.iSocketed = ITEM_UNSET;
	cfg.iType = ITEM_UNSET;	

	_tcsncpy(cfg.szItemCode, _T("gld"), ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Gold

	_tcsncpy(cfg.szItemCode, _T("rvl"), ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Full rejuv

	_tcsncpy(cfg.szItemCode, _T("rvs"), ITEM_CODE_LEN);
	m_aConfig.InsertRear(cfg); // Rejuv
	
	m_bBusy = FALSE;
}

void CPickitPlayer::OnInventoryFull()
{
	CD2Player::OnInventoryFull();

	// notify the client
	server->GamePrintInfo("Inventory full!");

	m_nInventoryFullState++; // we shall retry a few more times	
	if (m_nInventoryFullState >= INVENTORYFULL_RETRY)
	{
		m_bPaused = TRUE; // disable item pick immediately if inventory is full
				
		if (m_nInventoryFullState == INVENTORYFULL_RETRY)
		{
			// execute callback commands
			CStringParse sp(m_lpszFullCmds, ";");
			POSITION pos = sp.GetStartPosition();
			while (pos)
			{
				TCHAR szCmd[256] = _T("");
				sp.GetNextString(pos, szCmd, 255);
				server->GameCommandLine(szCmd);
			}
		}
	}
}

BOOL CPickitPlayer::IsGold(LPCTSTR lpszItemCode)
{
	return lpszItemCode != NULL && _tcsicmp(lpszItemCode, _T("gld")) == 0;
}

BOOL CPickitPlayer::IsRejuv(LPCTSTR lpszItemCode)
{
	return lpszItemCode != NULL && _tcsicmp(lpszItemCode, _T("rvs")) == 0;
}

BOOL CPickitPlayer::IsFullRejuv(LPCTSTR lpszItemCode)
{
	return lpszItemCode != NULL && _tcsicmp(lpszItemCode, _T("rvl")) == 0;
}

void CPickitPlayer::DebugItem(const ITEM &item) const
{
	if (!IsDebug())
		return;
	
	DebugMessage(_T("=========================="));

	FormatAndDebugMessage(_T("Item ID %08X"), item.dwItemID);
	FormatAndDebugMessage(_T("Item Code %s"), item.szItemCode);
	FormatAndDebugMessage(_T("Item Type %d"), item.iType);
	FormatAndDebugMessage(_T("Identified %d"), item.iIdentified);
	FormatAndDebugMessage(_T("Ethereal %d"), item.iEthereal);
	FormatAndDebugMessage(_T("Sockets %d"), item.iSocketNumber);
	FormatAndDebugMessage(_T("ilevel %d"), item.iLevel);
	FormatAndDebugMessage(_T("Quality %d"), item.iQuality);
				
	DebugMessage(_T("=========================="));
}

void CPickitPlayer::ShowClientMove(BOOL bShow)
{
	m_bShowClientMove = bShow;
}

BOOL CPickitPlayer::IsShowClientMove() const
{
	return m_bShowClientMove;
}

LPCTSTR CPickitPlayer::GetFullCmds() const
{
	return m_lpszFullCmds;
}

void CPickitPlayer::SetFullCmds(LPCTSTR lpszFullCmds)
{
	if (m_lpszFullCmds != NULL)
	{
		delete [] m_lpszFullCmds;
		m_lpszFullCmds = NULL;
	}

	if (lpszFullCmds != NULL && lpszFullCmds[0] != _T('\0'))
		m_lpszFullCmds = _tcsdup(lpszFullCmds);
}

void CPickitPlayer::SetAllowTownPick(BOOL bAllowTownPick)
{
	m_bTownPick = bAllowTownPick;
}

BOOL CPickitPlayer::IsAllowTownPick() const
{
	return m_bTownPick;
}

void CPickitPlayer::SetLogFilePath(LPCTSTR lpszLogFile)
{
	::memset(m_szLogFile, 0, sizeof(m_szLogFile));
	if (lpszLogFile)
		_tcsncpy(m_szLogFile, lpszLogFile, MAX_PATH);
}

LPCTSTR CPickitPlayer::GetLogFilePath() const
{
	return m_szLogFile;
}

void CPickitPlayer::OnItemDrop(LPCITEM lpItem, LPCTSTR lpszDescription)
{

}
