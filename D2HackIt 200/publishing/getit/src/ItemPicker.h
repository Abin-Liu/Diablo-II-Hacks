//////////////////////////////////////////////////////////////////////
// ItemPicker.h: interface of the CItemPicker class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMPICKER_H__
#define __ITEMPICKER_H__

#include <Windows.h>
#include "..\\Includes\\CommonStructs.h"
#include "..\\Includes\\Utilities\\ArrayEx.h"
#include "..\\Includes\\Utilities\\LinkedList.h"
#include "..\\Includes\\Utilities\\TickThread.h"

// The ITEMCONFIG struct
typedef struct tagItemConfig
{
	char szSection[256];	// section name
	BOOL bTeleport;			// Teleport or not?
	char szCodes[513];		// codes
	char szExpression[513]; // expression
	BOOL bSyntaxError;		// Syntax error
} ITEMCONFIG, *LPITEMCONFIG;
typedef const tagItemConfig* LPCITEMCONFIG;


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

typedef CArrayEx<ITEMCONFIG, const ITEMCONFIG&> ConfigList; // no frequent insertion/removal
typedef CLinkedList<SIMPLEITEM, const SIMPLEITEM&, NULL> ItemList; // frequent insertion/removal, must use linked list
typedef CLinkedList<DWORD, const DWORD&, NULL> IDList; // frequent insertion/removal, must use linked list

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
	void OnGameTimerTick();
	void OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam);
	void OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam);

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	BOOL IsEnabled() const;
	DWORD GetRadius() const;
	LPCSTR GetConfigFile() const;
	int GetConfigCount(); // Entries loaded from *.ini
	int GetQueueItemCount(BOOL bValuableOnly = FALSE, BOOL bInRadiusOnly = TRUE); // Items in queue
	BOOL IsShowClientMove() const; // Is client move shown/hidden?
	LPCSTR GetFullCmds() const;
	BOOL IsAllowTownPick() const; // allow pick up in town?
	void SetAllowTownPick(BOOL bDisableInTown);
	BOOL CanItemGotoBelt(int nMisc) const;
	DWORD GetDelay() const;
	DWORD SetDelay(DWORD dwDelay);
	
	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////

	BOOL LoadConfig(LPCSTR lpszFile);
	void CleanConfig();
	void Enable(BOOL bEnable);
	BOOL Toggle(); // Switch mode(on/off)
	DWORD SetRadius(DWORD dwRadius);
	void CleanQueue(); // Should be called in "OnGameJoin"
	void ShowClientMove(BOOL bShow); // Show/hide client move when picking items
	void SetFullCmds(LPCSTR lpszFullCmds);
	LPCSTR GetLogFilePath() const;
	void SetLogFilePath(LPCSTR lpszLogFile);
	void DebugQueue();
	static BOOL WriteLog(LPCSTR lpszFilePath, LPCSTR lpszString, BOOL bAppend = TRUE);

	static BOOL IsAbsPath(LPCSTR lpszPath);
	static int InvokeCallbackCommands(LPCSTR lpszCommands);
	
protected:	
	
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
	static BOOL CALLBACK EnumSectionsProc(LPCTSTR lpSection, LPVOID lpParam);

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	int m_nInventoryFullState; // Current inventory state
	ConfigList m_aConfig; // The configuration queue(no duplicated item codes)
	ItemList m_aValuable; // Valueable items
	ItemList m_aJunky; // Junky items
	IDList m_aIDList; // just dropped
	
	char m_szConfigFile[MAX_PATH + 1]; // Path name of the *.ini, usually "pickit.ini"
	BOOL m_bPaused; // Enabled/disabled flag
	DWORD m_dwRadius; // Pickup radius
	DWORD m_dwDelay; // delay
	BOOL m_bShowClientMove; // SHow client move
	LPSTR m_lpszFullCmds; // callback commands when inventory full
	BOOL m_bTownPick; // allow pickup in towns
	char m_szLogFile[MAX_PATH + 1]; // log file path
	DWORD m_dwLastPick; // time when the most recent pick packet was sent
	DWORD m_dwLastAnnounce; // last announced item ID

private:

	///////////////////////////////////////////////////////////////////////
	// Private Members
	///////////////////////////////////////////////////////////////////////
	static void CALLBACK ThreadProc(LPVOID lParam, LPCVBYTE lpStopFlag);
	CTickThread m_thread;
};

#endif // __ITEMPICKER_H__