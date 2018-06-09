//////////////////////////////////////////////////////////////////////
// ItemPicker.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMPICKER_H__
#define __ITEMPICKER_H__

#include "GetItAbstract.h"

// The SIMPLEITEM struct, reduce amount of data we need to maintain
typedef struct tagSimpleItem
{
	DWORD dwItemID;
	char szItemCode[4]; // for debug
	WORD x;
	WORD y;	
	DWORD dwMiscType;
	DWORD dwGoldAmount;
	BOOL bTeleport;
	DWORD dwRetry;
} SIMPLEITEM, *LPSIMPLEITEM;
typedef const tagSimpleItem* LPCSIMPLEITEM;

typedef CLinkedList<SIMPLEITEM, const SIMPLEITEM&, NULL> ItemList; // frequent insertion/removal, must use linked list
typedef CLinkedList<DWORD, const DWORD&, NULL> IDList; // frequent insertion/removal, must use linked list

///////////////////////////////////////////////////////////////////////
// CItemPicker Class Definition
///////////////////////////////////////////////////////////////////////
class CItemPicker : public CGetItAbstract
{
public:		
	
	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CItemPicker();
	virtual ~CItemPicker();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	void OnGameJoin();
	void OnGameLeave();
	void OnGameTimerTick();
	void OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam);
	void OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam);

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	BOOL IsEnabled() const;
	int GetQueueItemCount(BOOL bValuableOnly = FALSE, BOOL bInRadiusOnly = TRUE); // Items in queue
	BOOL CanItemGotoBelt(int nMisc) const;
	
	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	void Enable(BOOL bEnable);
	BOOL Toggle(); // Switch mode(on/off)
	void CleanQueue(); // Should be called in "OnGameJoin"
	void DebugQueue();
	
private:	
	
	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	void AddToQueue(SIMPLEITEM& si);
	static BOOL IsValuableItem(DWORD dwItemID, LPCSTR lpszItemCode);
	int GetQueueItemCount(ItemList& list, BOOL bInRadiusOnly);
	BOOL ProcessItemQueue(ItemList& queue);
	BOOL RemoveItemFromQueue(DWORD dwItemID);
	BOOL FindMatch(const SIMPLEITEM& item, BOOL& bTeleport);
	void CheckIDList();

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	int m_nInventoryFullState; // Current inventory state
	ItemList m_aValuable; // Valueable items
	ItemList m_aJunky; // Junky items
	IDList m_aIDList; // just dropped	
	BOOL m_bPaused; // Enabled/disabled flag
	DWORD m_dwLastPick; // time when the most recent pick packet was sent
};

#endif // __ITEMPICKER_H__