//////////////////////////////////////////////////////////////////////
// PickitPlayer.h: interface of the CPickitPlayer class.
//
// CPickitPlayer, derived from CD2Player, allows the player to monitor
// item drops and pick up desired items automatically. 
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// August 8th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __PICKITPLAYER_H__
#define __PICKITPLAYER_H__

#include "..\\D2Lib\\D2Lib.h"

#define PHM_ITEMDROP		DM_APP + 100 // Item dropped message

// Limits
#define PICKIT_DELAY_MIN		100 // Minimal pickup delay
#define PICKIT_DELAY_DEFAULT	200	// Default pickup delay 
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
	BYTE iIdentified; // must be identified?
	BYTE iEthereal; // must be ethereal?
	BYTE iSocketed; // must has sockets?
	BYTE iQuality; // unique, rare, set, superior, etc
	BYTE iType; // type: weapon, armor, etc
	TCHAR szItemCode[ITEM_CODE_LEN + 1]; // item code
	TCHAR szDescription[ITEM_DESC_LEN + 1]; // description: "{elite} Unique hydra bow(Windforce) dropped."
} ITEMCONFIG, *LPITEMCONFIG;
typedef const tagItemConfig* LPCITEMCONFIG;


///////////////////////////////////////////////////////////////////////
// CPickitPlayer Class Definition
///////////////////////////////////////////////////////////////////////
class CPickitPlayer : public CD2Player
{
public:		
	LPCTSTR GetLogFilePath() const;
	void SetLogFilePath(LPCTSTR lpszLogFile);
	
	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CPickitPlayer();
	virtual ~CPickitPlayer();

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
	LPCTSTR GetConfigFile() const;
	int GetConfigCount() const; // Entries loaded from *.ini
	DWORD GetDelay() const;
	int GetSpecialItemMethod(int nSpecialItem) const;
	int GetQueueItemCount(BOOL bValuableOnly = FALSE) const; // Items in queue
	BOOL IsShowClientMove() const; // Is client move shown/hidden?
	LPCTSTR GetFullCmds() const;
	BOOL IsAllowTownPick() const; // allow pick up in town?
	void SetAllowTownPick(BOOL bAllowTownPick);

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	virtual void InitAttributes(); // Clean all attributes

	BOOL LoadConfig(LPCTSTR lpszFile);
	void CleanConfig();
	void Enable(BOOL bEnable);
	BOOL Toggle(); // Switch mode(on/off)
	DWORD SetRadius(DWORD dwRadius);
	DWORD SetDelay(DWORD dwDelay);
	BOOL SetSpecialItemMethod(int nSpecialItem, int nMethod);
	void CleanQueue(); // Should be called in "OnGameJoin"
	int RemoveFromQueue(DWORD dwItemID); // Removes a particular item from the queue
	void ShowClientMove(BOOL bShow); // Show/hide client move when picking items
	void SetFullCmds(LPCTSTR lpszFullCmds);

	static BOOL WriteLog(LPCTSTR lpszFilePath, LPCTSTR lpszString, BOOL bAppend = TRUE);
	static BOOL IsGold(LPCTSTR lpszItemCode);
	static BOOL IsRejuv(LPCTSTR lpszItemCode);
	static BOOL IsFullRejuv(LPCTSTR lpszItemCode);

protected:	

	///////////////////////////////////////////////////////////////////////
	// Overrides
	///////////////////////////////////////////////////////////////////////
	virtual void OnItemDrop(LPCITEM lpItem, LPCTSTR lpszDescription);
	virtual void OnInventoryFull();
	
	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	const ITEMCONFIG* FindItem(const ITEM& item) const;
	BOOL DoPickup(const ITEM& item, DWORD dwTicker);
	BOOL IsNeedPickup(const ITEM& item, BOOL bCfgPickup);	
	void DebugItem(const ITEM& item) const;
	
	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	int m_nInventoryFullState; // Current inventory state
	int m_nGoldMethod; // Gold
	int m_nRejuvMethod; // Small rejuvenation potions
	int m_nFullRejuvMethod; // Full rejuvenation potions
	CLinkedList<ITEM, const ITEM&, NULL> m_aItemQueue; // The item queue	
	CLinkedList<ITEMCONFIG, const ITEMCONFIG&, NULL> m_aConfig; // The configuration queue
	TCHAR m_szConfigFile[MAX_PATH + 1]; // Path name of the *.ini, usually "pickit.ini"
	BOOL m_bBusy; // A flag to avoid possible access conflicts
	DWORD m_dwDelay; // pickup delay, in milliseconds
	DWORD m_dwLastProc; // Time when last pickup packet was sent
	BOOL m_bPaused; // Enabled/disabled flag
	DWORD m_dwRadius; // Pickup radius
	POSITION m_curPos; // Current item position
	BOOL m_bShowClientMove; // Show client move?
	LPTSTR m_lpszFullCmds; // Callback commands
	BOOL m_bTownPick; // Allow pickup in towns?
	TCHAR m_szLogFile[MAX_PATH + 1]; // Log file path, log will be written if not empty
};

#endif // __PICKITPLAYER_H__