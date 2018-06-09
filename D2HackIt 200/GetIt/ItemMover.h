//////////////////////////////////////////////////////////////////////
// ItemMover.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMMOVER_H__
#define __ITEMMOVER_H__

#include "GetItAbstract.h"
#include "..\\Includes\\Utilities\\StateTicker.h"

typedef struct tagIDItem
{
	DWORD dwItemID;
	SIZE itemSize;
} IDITEM, *LPIDITEM;
typedef const tagIDItem* LPCIDITEM;

BOOL operator==(const IDITEM& lhs, const IDITEM& rhs);
BOOL operator>(const IDITEM& lhs, const IDITEM& rhs);

///////////////////////////////////////////////////////////////////////
// CItemMover Class Definition
///////////////////////////////////////////////////////////////////////
class CItemMover : public CGetItAbstract  
{
public:

	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CItemMover();
	virtual ~CItemMover();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	void OnGameJoin();
	void OnGameLeave();
	void OnGameTimerTick();
	void OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam);
	void OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam);

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	BOOL Start();

private:

	BOOL IsGoodItem(DWORD dwItemID);
	int GetGoodItems();
	void RemoveItem(DWORD dwItemID);
	static void StateMessage(LPCSTR lpszMsg);
	static BOOL CALLBACK EnumItemProc(DWORD dwItemID, LPCSTR lpszItemCode, WORD x, WORD y, LPARAM lParam);

	CStateTicker m_ticker;
	BOOL m_bFull;
	DWORD m_dwCurItem;
	CArrayEx<IDITEM, const IDITEM&> m_aItems;
	int m_nStashRetry;
};

#endif // __ITEMMOVER_H__
