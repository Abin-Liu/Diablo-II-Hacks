//////////////////////////////////////////////////////////////////////
// ItemPicker.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "ItemPicker.h"
#include "D2MiscItem.h"

#define INVENTORYFULL_RETRY		3

BOOL operator==(const SIMPLEITEM& lhs, const SIMPLEITEM& rhs)
{
	return lhs.dwItemID == rhs.dwItemID;
}

CItemPicker::CItemPicker()
{
	m_nInventoryFullState = 0;
	m_bPaused = TRUE;
	m_dwLastPick = 0;
}

CItemPicker::~CItemPicker()
{
}

void CItemPicker::OnGameJoin()
{
	// TODO: Add your implementation here.
	m_dwLastPick = 0;
	CleanQueue();
	m_nInventoryFullState = 0;
	m_bPaused = FALSE;
}

void CItemPicker::OnGameLeave()
{
	// TODO: Add your implementation here.
	m_bPaused = TRUE;	
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

int CItemPicker::GetQueueItemCount(BOOL bValuableOnly, BOOL bInRadiusOnly)
{
	if (bValuableOnly)
		return GetQueueItemCount(m_aValuable, bInRadiusOnly);
	else 
		return GetQueueItemCount(m_aValuable, bInRadiusOnly) + GetQueueItemCount(m_aJunky, bInRadiusOnly);
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
		if (DISTANCE > m_pLib->GetRadius())
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
		if (!me->PickGroundItem(si.dwItemID, m_pLib->GetClientMove()))
		{
			si.dwRetry = 0;
			queue.MoveToRear(cur);
			continue;
		}

		m_dwLastPick = ::GetTickCount();
		si.dwRetry++;
		if (si.dwRetry > 6 && (si.dwRetry * m_pLib->GetDelay() > 2500)) // avoid starvation
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
		if (me->GetDistanceFrom(si.x, si.y) <= m_pLib->GetRadius())
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
	if (nMessage != PM_INVENTORYFULL || GetConfigLib() == NULL)
		return;
		
	m_nInventoryFullState++; // we shall retry a few more times	
	if (m_nInventoryFullState < INVENTORYFULL_RETRY)
		return;

	// notify the client
	server->GamePrintInfo("Inventory full!");
	m_bPaused = TRUE; // disable item pick immediately if inventory is full
				
	if (m_nInventoryFullState == INVENTORYFULL_RETRY)
		m_pLib->InvokeFullCallback();
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
	if (m_pLib == NULL)
		return;

	CheckIDList();	
	
	DWORD dwMode = me->GetMode();
	if (dwMode== 7 || dwMode== 8 || (dwMode >= 10 && dwMode <= 16) || dwMode == 18)
		return; // excellent check, from d2jsp snagit

	if (m_bPaused || m_pLib->GetRadius() == 0)
		return; // disabled

	if (::GetTickCount() < m_dwLastPick + m_pLib->GetDelay())
		return; // delay not elapsed yet

	if (!m_pLib->GetTownPick() && me->IsInTown())
		return; // in town and town_pick is disabled

	if (!ProcessItemQueue(m_aValuable))
	{
		//server->GameInfof("no valuable items queued, not processing junkies...");
		ProcessItemQueue(m_aJunky);
	}
}

void CItemPicker::CheckIDList()
{	
	static GAMEUNIT unit = { 0, UNIT_TYPE_ITEM };
	static SIMPLEITEM si;	

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

		if (m_pLib->Lookup(si.dwItemID, TRUE, &si.bTeleport))
		{
			POINT pt = server->GetUnitPosition(&unit);
			si.x = (WORD)pt.x;
			si.y = (WORD)pt.y;
			si.dwRetry = 0;
			AddToQueue(si);
			if (si.dwMiscType == 0)
				AnnounceItem(si.dwItemID);
		}
	}
	m_aIDList.Unlock();
}
