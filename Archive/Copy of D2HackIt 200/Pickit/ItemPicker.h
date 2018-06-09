//////////////////////////////////////////////////////////////////////
// ItemPicker.h: interface of the CItemPicker class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMPICKER_H__
#define __ITEMPICKER_H__

#include <Windows.h>
#include "LinkedList.h"
#include "..\\Includes\\CommonStructs.h"
#include "StringParse.h"	// Added by ClassView

// Limits
#define PICKIT_DELAY_MIN		200 // Minimal pickup delay
#define PICKIT_DELAY_DEFAULT	500	// Default pickup delay 
#define PICKIT_RADIUS_MAX		50 // Maximal pickup radius
#define PICKIT_RADIUS_DEFAULT	10 // Default pickup radius

// Special items (gold, rejuvenation potions, full rejuvenation potions)
enum { PICKIT_SPECIAL_UNKNOWN = 0, PICKIT_SPECIAL_GOLD, PICKIT_SPECIAL_REJUV, PICKIT_SPECIAL_FULLREJUV};

// Pick up methods for special items (gold, rejuvenation potions, full rejuvenation potions)
enum { PICKIT_PICK_UNKNWON = 0, // Error 
	   PICKIT_PICK_NO,			// Never pick up
	   PICKIT_PICK_YES,			// Always pick up
	   PICKIT_PICK_AUTO };		// Automatic - Pick up when the player can hold more(gold)
								// or the player's belt has empty slots(potions), and disable
								// automatically otherwise.

// The ITEMCONFIG struct
typedef struct tagItemConfig
{
	BYTE iPickup; // pick up or ignore?
	BYTE iIdentified; // identified?
	BYTE iEthereal; // ethereal?
	BYTE iSocketed; // has socket?
	BYTE iQuality; // unique, rare, set, superior, etc
	BYTE iType; // type: weapon, armor, etc
	char szItemCode[ITEM_CODE_LEN + 1]; // item code
	char szDescription[ITEM_DESC_LEN + 1]; // description: "{elite} Unique hydra bow(Windforce) dropped."
} ITEMCONFIG, *LPITEMCONFIG;
typedef const tagItemConfig* LPCITEMCONFIG;


// The SIMPLEITEM struct, reduce amount of data we need to maintain
typedef struct tagSimpleItem
{
	DWORD dwItemID;
	WORD x;
	WORD y;
	DWORD dwGoldAmount;
	char szItemCode[ITEM_CODE_LEN + 1];
	int nRetryCount;
} SIMPLEITEM, *LPSIMPLEITEM;
typedef const tagSimpleItem* LPCSIMPLEITEM;

typedef CLinkedList<ITEMCONFIG, const ITEMCONFIG&, NULL> ConfigList;
typedef CLinkedList<SIMPLEITEM, const SIMPLEITEM&, NULL> ItemList;

///////////////////////////////////////////////////////////////////////
// CItemPicker Class Definition
///////////////////////////////////////////////////////////////////////
class CItemPicker
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
	virtual void OnGameJoin(const THISGAMESTRUCT* pGame);
	virtual void OnGameLeave();
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);
	virtual void OnGameTimerTick();

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	BOOL IsEnabled() const;
	DWORD GetRadius() const;
	LPCSTR GetConfigFile() const;
	int GetConfigCount(); // Entries loaded from *.ini
	DWORD GetDelay() const;
	int GetSpecialItemMethod(int nSpecialItem) const;
	int GetQueueItemCount(BOOL bValuableOnly = FALSE, BOOL bInRadiusOnly = TRUE); // Items in queue
	BOOL IsShowClientMove() const; // Is client move shown/hidden?
	LPCSTR GetFullCmds() const;
	BOOL IsAllowTownPick() const; // allow pick up in town?
	void SetAllowTownPick(BOOL bDisableInTown);
	BOOL IsDebug() const;
	void SetDebug(BOOL bDebug);
	BOOL CanItemGotoBelt(LPCSTR lpszItemCode) const;

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	virtual void InitAttributes(); // Clean all attributes

	BOOL LoadConfig(LPCSTR lpszFile);
	void CleanConfig();
	void Enable(BOOL bEnable);
	BOOL Toggle(); // Switch mode(on/off)
	DWORD SetRadius(DWORD dwRadius);
	DWORD SetDelay(DWORD dwDelay);
	BOOL SetSpecialItemMethod(int nSpecialItem, int nMethod);
	void CleanQueue(); // Should be called in "OnGameJoin"
	int RemoveFromQueue(DWORD dwItemID); // Removes a particular item from the queue
	void ShowClientMove(BOOL bShow); // Show/hide client move when picking items
	void SetFullCmds(LPCSTR lpszFullCmds);
	LPCSTR GetLogFilePath() const;
	void SetLogFilePath(LPCSTR lpszLogFile);
	void OnInventoryFull();
	void DebugQueue();

	static BOOL WriteLog(LPCSTR lpszFilePath, LPCSTR lpszString, BOOL bAppend = TRUE);
	static BOOL IsGold(LPCSTR lpszItemCode);
	static BOOL IsRejuv(LPCSTR lpszItemCode);
	static BOOL IsFullRejuv(LPCSTR lpszItemCode);

protected:
	
	BOOL IsItemInQueue(DWORD dwID);
	static BOOL IsItemDropPacket(const BYTE* aPacket);
	static BOOL IsValuableItem(LPCITEM lpItem);
	int GetQueueItemCount(ItemList& list, BOOL bInRadiusOnly);
	void DebugMessage(LPCSTR lpszMessage) const;
	void DebugItem(const ITEM& item) const;
	BOOL ProcessItemQueue(ItemList& queue);

	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	const ITEMCONFIG* FindItem(const ITEM& item);
//	BOOL DoPickup(const SIMPLEITEM& item);
	BOOL IsNeedPickup(const SIMPLEITEM& item);	
	POSITION FindFirstInRangeItem(ItemList& list);

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	int m_nInventoryFullState; // Current inventory state
	int m_nGoldMethod; // Gold
	int m_nRejuvMethod; // Small rejuvenation potions
	int m_nFullRejuvMethod; // Full rejuvenation potions

	ConfigList m_aConfig; // The configuration queue
	ItemList m_aValuable; // Valueable items
	ItemList m_aJunky; // Junky items
	
	char m_szConfigFile[MAX_PATH + 1]; // Path name of the *.ini, usually "pickit.ini"
	DWORD m_dwDelay; // pickup delay, in milliseconds
	BOOL m_bPaused; // Enabled/disabled flag
	DWORD m_dwRadius; // Pickup radius
	BOOL m_bShowClientMove;
	LPSTR m_lpszFullCmds;
	BOOL m_bTownPick;
	char m_szLogFile[MAX_PATH + 1];
	BOOL m_bDebug;	
//	DWORD m_dwTicker;
	DWORD m_dwLastPick;
};

#endif // __ITEMPICKER_H__