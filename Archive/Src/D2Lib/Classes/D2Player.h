//////////////////////////////////////////////////////////////////////
// D2Player.h: interface of the CD2Player class.
//
// CD2Player, a big class derived from CD2Object, is used for manipulating
// current player's attributes and states.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __D2PLAYER_H__
#define __D2PLAYER_H__

#include "D2Object.h"
#include "D2StaticObject.h"
#include "D2Unit.h"
#include "D2Monster.h"
#include "Utils\\D2PlayerStats.h"
#include "Utils\\MsgSender.h"

// Comment this line out if you do not want to use the "space" key to stop
// interaction with units or static objects
#define USE_SPACE_KEY_TO_STOP_INTERACTION

///////////////////////////////////////////////////////////////////////
// CD2Player Class Definition
///////////////////////////////////////////////////////////////////////
class CD2Player : public CD2Object
{
public:		
			
	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CD2Player();
	virtual ~CD2Player();

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
	
	// Player Stats
	DWORD GetObjectID() const;
	BOOL IsValidObject() const;
	WORD GetPositionX() const;
	WORD GetPositionY() const;	
	BYTE GetPlayerClass() const; // Player's character class
	DWORD GetPlayerName(LPTSTR lpszBuffer, DWORD dwMaxChars) const; // Player's character name
	BOOL FindPlayerStats(DWORD dwStatsID);
	DWORD GetPlayerStats(DWORD dwStatsID); // Generic stats access, hp/mana,maxhp/maxmana/etc
	
	// Percentage Calculation Helpers
	BYTE GetHPPercent();			// Life percent
	BYTE GetManaPercent();			// Mana percent
	BYTE GetItemDurabilityPercent();// Item durability percent

	WORD GetLeftSpell() const; // Left-hand spell
	WORD GetRightSpell() const; // Right-hand spell
	UINT GetSpellCount() const; // Spells count
	BOOL HasSpell(WORD wSpellID) const; // Whether the player has this spell
	DWORD GetCorpseCount() const; // Player's nearby corpse count
	DWORD GetHostilerCount() const; // Hostiler count
	DWORD GetInventoryGoldLimit(); // The maximum gold amount that the player can hold in inventory
	DWORD GetInventoryGoldSpace(); // How much more gold the player can hold in inventory
	BOOL IsBeltFull() const; // Is the player's belt full?
	BYTE GetBeltRows() const; // Row number of the player's belt(1-4)
	BOOL CanItemGotoBelt(LPCTSTR lpszItemCode) const; // Can a particular item go directly to the player's belt?
	LPCITEM GetBeltItem(BYTE iRow, BYTE iCol) const; // Belt item access

	// Maps & Locations
	BOOL IsPlayerInTown() const; // Is player in town?
	BYTE GetCurrentMapID() const; // Retrieve current map ID
	WORD GetDistance(WORD x, WORD y) const; // Player's distance from a position
	BOOL IsMoving() const; // Is the player moving? (walking, running, etc)

	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	virtual void InitAttributes(); // Clean all attributes

	// Make the player drink belt potions
	DWORD HealMe(BOOL bPurpleOnly = FALSE) const; // Drink a purple/red potion
	DWORD HealMerc(BOOL bPurpleOnly = FALSE) const; // Feed the merc a purple/red potion
	DWORD SupplyMana(BOOL bDrinkPurpleIfNoBlue = TRUE) const; // Drink a blue/purple potion for mana
	int DrinkAllDrunks() const; // Drink all drunky potions in belt

	// Make the player move to some position
	BOOL RunTo(WORD x, WORD y, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to a position
	BOOL RunTo(const CD2StaticObject* pStaticObject, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to a static object
	BOOL RunTo(const CD2Unit* pNpc, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to an unit (npc, monster, etc)
	BOOL RunTo(LPCGROUNDITEM pItem, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to a ground item
	BOOL RunTo(LPCPLAYEROBJ pPlayer, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to an other player
	BOOL TeleportTo(WORD x, WORD y, BOOL bPreemptive = FALSE); // teleport to a position
	void PauseJobs(BOOL bPause);
	void CleanJobs();
	
	BOOL BackToTown(int nMaxRetry = 5); // Cast a town portal and go back to town immediately
	BOOL LeaveGame() const; // Save & exit

	// Make the player cast spells
	BOOL SelectSpell(WORD wSpell, BOOL bLeft, BOOL bVerifySpell = TRUE); // Select a spell
	BOOL CastSpell(WORD wSpell, BOOL bLeft, BOOL bVerifySpell = TRUE);
	BOOL CastSpell(WORD wSpell, BOOL bLeft, WORD x, WORD y, BOOL bVerifySpell = TRUE); // Cast spell on a map position
	BOOL CastSpell(WORD wSpell, BOOL bLeft, CD2StaticObject *pTarget, BOOL bVerifySpell = TRUE); // Cast spell on a static object
	BOOL CastSpell(WORD wSpell, BOOL bLeft, const CD2Unit* pTarget, BOOL bVerifySpell = TRUE); // Cast spell on an unit (npc, monster, etc)
	BOOL CastSpell(WORD wSpell, BOOL bLeft, LPCMONSTERCORPSE pTarget, BOOL bVerifySpell = TRUE); // Cast a corpse-required spell (corpse explosion, find item, etc)
	BOOL Telekinese(LPCGROUNDITEM pItem, BOOL bVerifySpell = TRUE); // Telekinese a ground item

	// Make the player speak, display overhead messages, weapon switch, or anti-idle
	BOOL Say(LPCTSTR lpszMessage) const; // Say a message
	BOOL OverHead(LPCTSTR lpszMessage, BOOL bClientOnly = TRUE) const; // Display an over-head message
	BOOL SendSound(BYTE iSound) const; // Send a sound
	BOOL AntiIdle() const; // Anti-idle
	BOOL WeaponSwitch() const; // Do weapon switch

	// Make the player interact with other game objects
	BOOL OpenStaticObject(const CD2StaticObject* pStaticObject) const; // Interact with a static object
	BOOL LeaveStaticObject(const CD2StaticObject* pStaticObject) const; // Stop interacting with a static object
	BOOL EnterWP(const CD2StaticObject* pWP, BYTE iDestinationMapID) const; // take WP to another area
	BOOL PickItem(LPCGROUNDITEM pItem, BOOL bShowClientMove = TRUE); // Pick up a ground item
	BOOL TalkToNpc(const CD2Unit* pNpc, int nMaxRetryTimes = 5); // Interact with an npc
	BOOL LeaveNpc(const CD2Unit* pNpc) const; // Stop interacting with an npc
	BOOL ResurrectMerc(const CD2Unit* pNpc) const; // Resurrect merc
	BOOL OpenNpcStore(const CD2Unit* pNpc) const; // Open an npc's store
	BOOL BuyItem(DWORD dwItemID, const CD2Unit* pNpc) const; // Buy an item from npc
	BOOL RepairItem(const CD2Unit* pNpc, DWORD dwItemID = 0) const; // Repair items
	BOOL IdentifyItems(const CD2Unit* pNpc) const;
	BOOL DepositGoldToStash(DWORD dwAmount = 0); // Deposit inventory gold to stash
	BOOL PickupCorpse() const; // Pick up player's corpse
	BOOL RedrawClient(BOOL bFade = TRUE); // Redraw client view
	BOOL CloseStash() const; // Close stash
	BOOL CloseWP(const CD2StaticObject* pWP) const; // Close WP window

	// Send messages to the D2 game window
	BOOL SendSysKey(BYTE iVKCode) const; // Send a system key
	BOOL SendChar(TCHAR ch) const; // Send a character		
	BOOL SendString(LPCTSTR lpszString) const; // Send a sequence of characters
	BOOL LeftClick(long x, long y) const; // Send a left button click 
	BOOL RightClick(long x, long y) const; // Send a right button click
	BOOL DblClick(long x, long y) const; // Send a left button double-click
	BOOL MouseMove(long x, long y) const; // Send a mouse move event

	// Debug Functions
	void DebugMapData() const;

	// Obsoleted
	BOOL SetFingerprinter(LPCD2OBJFINGERPRINTER lpFingerprinter);
	void LockSpawn(BOOL bLock);
	
protected:		
	
	///////////////////////////////////////////////////////////////////////
	// Overrides
	///////////////////////////////////////////////////////////////////////
	virtual void OnCharLevel(BYTE iLevel);
	virtual void OnMapChange(BYTE iOldMapID, BYTE iNewMapID);
	virtual void OnEnterTown(BYTE iOldMapID, BYTE iNewMapID);
	virtual void OnLeaveTown(BYTE iOldMapID, BYTE iNewMapID);
	virtual void OnHPUp(WORD wOldHP, WORD wNewHP);
	virtual void OnHPDown(WORD wOldHP, WORD wNewHP);
	virtual void OnManaUp(WORD wOldMana, WORD wNewMana);
	virtual void OnManaDown(WORD wOldMana, WORD wNewMana);
	virtual void OnHostile(DWORD dwHostilerID, DWORD dwHostilerCount);
	virtual void OnUnhostile(DWORD dwUnhostilerID, DWORD dwHostilerCount);
	virtual void OnPlayerDeath(BYTE iAct, WORD x, WORD y);
	virtual void OnCorpse(DWORD dwCorpseID, WORD x, WORD y);
	virtual void OnCorpsePicked();
	virtual void OnLSpell(WORD wOldSpell, WORD wNewSpell);
	virtual void OnRSpell(WORD wOldSpell, WORD wNewSpell);
	virtual void OnTeleport(BYTE iAct, WORD x, WORD y);
	virtual void OnNpcEnd(BOOL bSuccessful, DWORD dwNpcID);
	virtual void OnSpellCharges(WORD wSpell, BYTE iChargesLeft);
	virtual void OnMoveComplete(BOOL bSuceeded, LPCMOVEJOB pLastJob);
	virtual void OnWeaponSwitch();
	virtual void OnTownFail();
	virtual void OnInventoryFull();
	virtual void OnEquipEvent(BYTE iEventType, LPCITEM lpItem);
	virtual void OnWPOpen(BYTE iAct, BYTE iMapID);
	virtual void OnStashOpen(BYTE iAct);
	virtual void OnActChange(BYTE iOriginalAct, BYTE iCurrentAct);
	virtual void OnGameReady(BYTE iAct, WORD x, WORD y);

	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	BOOL CastSpellOnEntity(WORD wSpell, BOOL bLeft, DWORD dwType, DWORD dwEntityID, WORD x, WORD y, BOOL bVerifySpell);
	BOOL StopInteractWithEntity(DWORD dwType, DWORD dwEntityID) const;
	BOOL PrepareInteractWithNPC(DWORD dwNpcID, WORD  x, WORD y) const;
	BOOL InteractWithEntity(DWORD dwType, DWORD dwEntityID) const;
	BOOL GoToEntity(BOOL bRun, DWORD dwType, DWORD dwEntityID, WORD x, WORD y, BOOL bShowClientMove = TRUE) const;
	BOOL GoToPosition(BOOL bRun, WORD x, WORD y, BOOL bShowClientMove = TRUE) const;
	LPCITEM FindFirstBeltItem(LPCTSTR lpszItemCode) const;
	BOOL IsBeltColFull(int nCol) const;
	BOOL DrinkPotion(LPCITEM lpItem, BOOL bFeedMerc) const;
	LPCITEM FindBluePotion() const;
	LPCITEM FindPurplePotion() const;
	LPCITEM FindRedPotion() const;
	void FailJobs();
	BOOL DoFirstJob();
	void DoTPProc();
	void StepDone();
	void AnalyzeItem(const ITEM& item);
	void AnalyzeHPMana();
	void StopInteractionUsingSpaceKey(int nTimes = 2) const;

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	CArrayEx<WORD, WORD> m_aSkillList; // Player spells
	CArrayEx<WORD, WORD> m_aChargeableSkillList; // Charged spells
	CArrayEx<DWORD, DWORD> m_aHostilerList; // Hostiler IDs
	CArrayEx<PLAYEROBJ, PLAYEROBJ> m_aCorpseList; // Player corpses, up to 16 in 1.10
	CLinkedList<MOVEJOB, const MOVEJOB&, NULL> m_aJobQueue; // Motion job queue
	MOVEJOB m_prevJob; // Previous job
	BYTE m_iCurMap; // Current map ID
	WORD m_wLeftSpell; // Left-hand spell
	WORD m_wRightSpell; // Right-hand spell
	D2SPELLINFO m_lastD2s; // Last spell info
	ITEM m_aBeltItems[4][4];	// Items in belt
	BYTE m_iBeltRows; // Current belt row number
	CTicker m_ticker; // Timer Ticker
	int m_nTeleportFail; // How many times the previous teleport has failed
	int m_nTPState; // "Back to town" state
	int m_nTPMaxRetry; // Maximum retry times
	int m_nTPCastfail; // TP casting failure count
	int m_nTPEnterFail; // TP entering failure count
	PLAYEROBJ m_portal; // For "back to town" usage
	BYTE m_iLastSyncMap; // The last time when "RedrawClient" is called
	BOOL m_bTeleported; // for teleport
	DWORD m_dwLastSyncTime;	
	CLinkedList<DWORD, DWORD, NULL> m_aHPManaCheckList;	
	WORD m_wXBeforeTele;
	WORD m_wYBeforeTele;
	BOOL m_bPaused;
	BYTE m_iWatchAct;
	CMsgSender m_msgSender;

private:	
	
	///////////////////////////////////////////////////////////////////////
	// Private Members
	///////////////////////////////////////////////////////////////////////
	BOOL AnalyzeMapData(); // Check map data
	static int FindDataCount(LPCMAPCOUNT aCounts, int nCount, BYTE iMapID);
	BYTE FindCurrentMapID();
	void NPCEnd(BOOL bSuccessful);
	BOOL InteractWithCurrentNpc() const;
	CArrayEx<MAPDATA, const MAPDATA&> m_aMapDataList; // Map data list	
	CD2PlayerStats m_stats; // Player stats array, readonly
	WORD m_wHP;
	WORD m_wMana;
	const CD2Unit* m_pNpc;
	int m_nNpcMaxRetry;
	int m_nNpcFails;
	BOOL m_bGameReady;
	CTicker m_mapTicker;
};

#endif // __D2PLAYER_H__