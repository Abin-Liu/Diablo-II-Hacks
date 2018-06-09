//////////////////////////////////////////////////////////////////////
// ItemPicker.h: interface of the CItemPicker class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMPICKER_H__
#define __ITEMPICKER_H__

#include <Windows.h>
#include "..\\Includes\\CommonStructs.h"
#include "Utilities\\ArrayEx.h"
#include "Utilities\\LinkedList.h"
#include "Utilities\\TickThread.h"
#include "StringParse.h"

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
	BYTE iSockets; // has socket?
	BYTE iQuality; // unique, rare, set, superior, etc
	BYTE iType; // type: weapon, armor, etc
	char szItemCode[4]; // item code
	char szDescription[129]; // description: "{elite} Unique hydra bow(Windforce) dropped."
} ITEMCONFIG, *LPITEMCONFIG;
typedef const tagItemConfig* LPCITEMCONFIG;


// The SIMPLEITEM struct, reduce amount of data we need to maintain
typedef struct tagSimpleItem
{
	DWORD dwItemID;
	WORD x;
	WORD y;
	DWORD dwGoldAmount;
	char szItemCode[4];
	int nRetry;
} SIMPLEITEM, *LPSIMPLEITEM;
typedef const tagSimpleItem* LPCSIMPLEITEM;

typedef CArrayEx<ITEMCONFIG, const ITEMCONFIG&> ConfigList; // no frequent insertion/removal, use CArrayEx for fast binary searching
typedef CLinkedList<SIMPLEITEM, const SIMPLEITEM&, NULL> ItemList; // frequent insertion/removal, must use linked list

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
	void OnGameJoin();
	void OnGameLeave();
	void OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam);
	void OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam);

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
	BOOL CanItemGotoBelt(LPCSTR lpszItemCode) const;
	DWORD GetMinGoldAmount() const;

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////

	BOOL LoadConfig(LPCSTR lpszFile);
	void CleanConfig();
	void Enable(BOOL bEnable);
	BOOL Toggle(); // Switch mode(on/off)
	DWORD SetRadius(DWORD dwRadius);
	DWORD SetDelay(DWORD dwDelay);
	BOOL SetSpecialItemMethod(int nSpecialItem, int nMethod);
	void CleanQueue(); // Should be called in "OnGameJoin"
	void ShowClientMove(BOOL bShow); // Show/hide client move when picking items
	void SetFullCmds(LPCSTR lpszFullCmds);
	LPCSTR GetLogFilePath() const;
	void SetLogFilePath(LPCSTR lpszLogFile);
	void SetMinGoldAmount(DWORD dwAmount);
	void DebugQueue();
	static BOOL WriteLog(LPCSTR lpszFilePath, LPCSTR lpszString, BOOL bAppend = TRUE);
	
protected:	

	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	static BOOL AddToQueue(const SIMPLEITEM& si, ItemList& queue);
	LPCITEMCONFIG FindItem(LPCGAMEUNIT lpUnit, BYTE iItemType, SIMPLEITEM& si, BOOL& bValuable) const;
	LPCITEMCONFIG ExamConfigItem(LPCGAMEUNIT lpUnit, const ITEMCONFIG& config, SIMPLEITEM& si, BYTE iType, BOOL& bValuable) const;
	BOOL IsNeedPickup(const SIMPLEITEM& item) const;	
	static BOOL IsValuableItem(LPCSTR lpszItemCode, BYTE iQuality);
	int GetQueueItemCount(ItemList& list, BOOL bInRadiusOnly);
	BOOL ProcessItemQueue(ItemList& queue);
	static void CALLBACK TickProc(LPVOID lpParam, LPCVBYTE lpStopFlag);
	static BOOL CALLBACK EnumSectionsProc(LPCTSTR lpSection, LPVOID lpParam);

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	int m_nInventoryFullState; // Current inventory state
	int m_nGoldMethod; // Gold
	int m_nRejuvMethod; // Small rejuvenation potions
	int m_nFullRejuvMethod; // Full rejuvenation potions

	ConfigList m_aConfig1; // The configuration queue(no duplicated item codes)
	ConfigList m_aConfig2; // The configuration queue(with duplicated item codes)
	ItemList m_aValuable; // Valueable items
	ItemList m_aJunky; // Junky items
	
	char m_szConfigFile[MAX_PATH + 1]; // Path name of the *.ini, usually "pickit.ini"
	DWORD m_dwDelay; // pickup delay, in milliseconds
	BOOL m_bPaused; // Enabled/disabled flag
	DWORD m_dwRadius; // Pickup radius
	BOOL m_bShowClientMove; // SHow client move
	LPSTR m_lpszFullCmds; // callback commands when inventory full
	BOOL m_bTownPick; // allow pickup in towns
	char m_szLogFile[MAX_PATH + 1]; // log file path
	DWORD m_dwMinGoldAmount; // minimum gold amount to pick
	CTickThread m_thread; // internal thread	
};

#endif // __ITEMPICKER_H__