//////////////////////////////////////////////////////////////////////
// ItemMover.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "ItemMover.h"
#include "D2Npc.h"

enum { ID_NONE = 0,
	 ID_CHECK_STASH,
	 ID_OPEN_STASH,
	 ID_WAIT_STASH,
	 ID_DEPOSIT,
	 ID_PICK_UP, 
	 ID_WAIT_PICK,
	 ID_DROP_DOWN, 
	 ID_WAIT_DOWN,	 
	 ID_END
};

const static DWORD CAINS[5] = { NPC_A1_CAIN, NPC_A2_CAIN, NPC_A3_CAIN, NPC_A4_CAIN, NPC_A5_CAIN };
const static DWORD HEALERS[5] = { NPC_A1_AKARA, NPC_A2_FARA, NPC_A3_ORMUS, NPC_A4_JAMELLA, NPC_A5_MALAH };

BOOL operator==(const IDITEM& lhs, const IDITEM& rhs)
{
	return ::memcmp(&lhs.itemSize, &rhs.itemSize, sizeof(SIZE)) == 0;
}

BOOL operator>(const IDITEM& lhs, const IDITEM& rhs)
{
	return lhs.itemSize.cy > rhs.itemSize.cy || lhs.itemSize.cx > rhs.itemSize.cx;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemMover::CItemMover()
{
	m_bFull = FALSE;
	m_dwCurItem = 0;
	m_nStashRetry = 0;
}

CItemMover::~CItemMover()
{

}

void CItemMover::OnGameJoin()
{
	m_bFull = FALSE;
	m_dwCurItem = 0;
	m_nStashRetry = 0;
	m_aItems.RemoveAll();
	m_ticker.Clear();
}

void CItemMover::OnGameLeave()
{
}

void CItemMover::OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == PM_STASHOPEN && m_ticker.GetState() == ID_WAIT_STASH)
		m_ticker.Update(ID_DEPOSIT);
}

void CItemMover::OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
}

BOOL CItemMover::Start()
{
	if (m_pLib == NULL)
		return FALSE;

	if (m_ticker.GetState() != ID_NONE)
	{
		server->GamePrintError("GetIt is already in an \"stash\" command session!");
		return FALSE;
	}

	if (!me->IsInTown())
	{
		server->GamePrintError("Player must be in town to start an \"stash\" command session!");
		m_pLib->InvokeStashCallback();
		return FALSE;
	}
	
	int nCount = GetGoodItems();
	if (nCount == 0 && me->GetStat(STAT_GOLD) == 0)
	{
		server->GameInfof("There is no valuable items between column %d - %d.", m_pLib->GetLeftCol(), m_pLib->GetRightCol());
		m_pLib->InvokeStashCallback();
		return FALSE;
	}	
	
	me->CloseAllUIs();
	server->GamePrintInfo("GetIt is starting an \"stash\" command session.");
	m_bFull = FALSE;
	server->GameInfof("Found ÿc2%dÿc0 valuable items between column %d - %d", nCount, m_pLib->GetLeftCol(), m_pLib->GetRightCol());
	m_ticker.Clear();
	m_ticker.Update(ID_CHECK_STASH, 1000);	
	return TRUE;
}

BOOL CItemMover::EnumItemProc(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam)
{
	CItemMover* pMover = (CItemMover*)lParam;
	if (pMover == NULL || pMover->m_pLib == NULL)
		return FALSE;

	if ((int)x >= pMover->m_pLib->GetLeftCol() && (int)x <= pMover->m_pLib->GetRightCol() && pMover->IsGoodItem(dwItemID)) 
	{
		IDITEM ii;
		ii.dwItemID = dwItemID;
		ii.itemSize = server->GetItemSize2(dwItemID);
		pMover->m_aItems.Add(ii);
	}
	return TRUE;
}

int CItemMover::GetGoodItems()
{
	m_aItems.RemoveAll();
	me->EnumStorageItems(STORAGE_INVENTORY, EnumItemProc, (LPARAM)this);
	m_aItems.Sort(FALSE); // sort ascending, we move bigger items first
	return m_aItems.GetSize();
}

BOOL CItemMover::IsGoodItem(DWORD dwItemID)
{
	if (m_pLib == NULL)
		return TRUE;

	DWORD dwAttr = server->GetItemAttributes(dwItemID);
	if ((dwAttr & ITEM_IDENTIFIED) == 0 || (dwAttr & ITEM_RUNEWORD))
		return TRUE;

	return m_pLib->Lookup(dwItemID, FALSE);
}

void CItemMover::StateMessage(LPCSTR lpszMsg)
{
	if (lpszMsg && *lpszMsg)
		me->OverHead(lpszMsg, TRUE);
}

void CItemMover::RemoveItem(DWORD dwItemID)
{
	for (int i = 0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i].dwItemID == dwItemID)
		{
			m_aItems.RemoveAt(i);
			return;
		}
	}
}

void CItemMover::OnGameTimerTick()
{
	if (m_pLib == NULL || m_ticker.GetState() == ID_NONE)
		return;

	m_ticker.TickIt(100);
	if (m_ticker.IsWaiting() || !m_ticker.IsTickEnded())
		return;

	static int nCount = 0;
	static GAMEUNIT stash;
	static POINT pt;

	switch (m_ticker.GetState())
	{
	case ID_CHECK_STASH:
		m_nStashRetry = 0;

		if (me->GetStat(STAT_GOLD) == 0)
		{
			m_ticker.Update(ID_PICK_UP);
			break;
		}
		
		if (!server->FindUnitByClassID(267, UNIT_TYPE_OBJECT, &stash))
		{
			m_ticker.Update(ID_PICK_UP);
			break;
		}

		pt = server->GetUnitPosition(&stash);
		if (me->GetDistanceFrom(pt.x, pt.y) > 20)
		{
			m_ticker.Update(ID_PICK_UP);
			break;
		}

		m_ticker.Update(ID_OPEN_STASH);
		break;

	case ID_OPEN_STASH:
		StateMessage("Opening stash");
		me->Interact(&stash);
		m_ticker.Update(ID_WAIT_STASH, 5000);
		break;

	case ID_WAIT_STASH:
		m_nStashRetry++;
		if (m_nStashRetry < 8)
			m_ticker.Update(ID_OPEN_STASH);
		else
			m_ticker.Update(ID_PICK_UP);
		
		break;

	case ID_DEPOSIT:
		StateMessage("Depositting gold");
		me->DepositGoldToStash(0);
		m_ticker.Update(ID_PICK_UP);
		break;
		
	case ID_PICK_UP:
		if (m_aItems.IsEmpty())
		{
			m_ticker.Update(ID_END);
			break;
		}
		
		StateMessage("Picking items");
		me->PickStorageItemToCursor(m_aItems[0].dwItemID);
		m_ticker.Update(ID_WAIT_PICK, 300);
		break;

	case ID_WAIT_PICK:
		m_dwCurItem = me->GetCursorItem();
		if (m_dwCurItem)
		{
			RemoveItem(m_dwCurItem);
			m_dwCurItem = 0;
			m_ticker.Update(ID_DROP_DOWN);
		}
		else
		{
			m_ticker.Update(ID_PICK_UP);			
		}
		break;

	case ID_DROP_DOWN:
		StateMessage("Moving items");
		if (!m_bFull)
		{
			m_bFull = !me->DropCursorItemToStorage(STORAGE_STASH);
			if (m_bFull && m_pLib->GetUseCube())
				m_bFull = !me->DropCursorItemToStorage(STORAGE_CUBE);
		}
		
		if (m_bFull)
			me->DropCursorItemToStorage(STORAGE_INVENTORY);

		m_ticker.Update(ID_WAIT_DOWN);
		break;

	case ID_WAIT_DOWN:
		if (me->GetCursorItem())
		{
			m_ticker.Update(ID_DROP_DOWN);
		}
		else
		{
			if (m_bFull)
				m_ticker.Update(ID_END);
			else
				m_ticker.Update(ID_PICK_UP);
		}

		break;

	case ID_END:
		me->CloseAllUIs();
		server->GamePrintInfo("GetIt completed the \"stash\" command session successfully.");
		m_ticker.Clear();
		StateMessage("Stash command finished");
		m_pLib->InvokeStashCallback();
		break;

	default:
		break;
	}
}
