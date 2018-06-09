//////////////////////////////////////////////////////////////////////
// D2Player.h: interface of the CD2Player class.
//
// CD2Player, a big class derived from CD2Abstract, is used for manipulating
// current player's attributes and states.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __D2PLAYER_H__
#define __D2PLAYER_H__

#include "D2StaticObject.h"
#include "D2Unit.h"

///////////////////////////////////////////////////////////////////////
// CD2Player Class Definition
///////////////////////////////////////////////////////////////////////
class CD2Player : public CD2Abstract
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
	
	// Maps & Locations
	BOOL IsPlayerInTown() const; // Is player in town?
	BYTE GetCurrentMapID() const; // Retrieve current map ID
	WORD GetDistance(WORD x, WORD y) const; // Player's distance from a position
	BOOL IsMoving() const; // Is the player moving? (walking, running, etc)

	// Player Stats
	DWORD GetPlayerID() const; // Player's unique ID
	BYTE GetPlayerClass() const; // Player's character class
	DWORD GetPlayerName(LPTSTR lpszBuffer, DWORD dwMaxChars) const; // Player's character name
	WORD GetPlayerPositionX() const; // Player's current position x
	WORD GetPlayerPositionY() const; // Player's current position y
	BYTE GetPlayerLevel() const; // Player's level
	DWORD GetInventoryGold() const; // Inventory gold amount
	DWORD GetStashGold() const; // Stash gold amount
	WORD GetHP() const; // Current HP
	WORD GetMana() const; // Current mana
	BYTE GetHPPercent() const; // Current HP percentage(approximation)
	BYTE GetManaPercent() const; // Current mana percentage(approximation)
	WORD GetLeftSpell() const; // Left-hand spell
	WORD GetRightSpell() const; // Right-hand spell
	UINT GetSpellCount() const; // Spells count
	BOOL HasSpell(WORD wSpellID) const; // Whether the player has this spell
	DWORD GetCorpseCount() const; // Player's nearby corpse count
	DWORD GetHostilerCount() const; // Hostiler count
	DWORD GetInventoryGoldLimit() const; // The maximum gold amount that the player can hold in inventory
	DWORD GetInventoryGoldSpace() const; // How much more gold the player can hold in inventory
	BOOL IsBeltFull() const; // Is the player's belt full?
	BYTE GetBeltRows() const; // Row number of the player's belt(1-4)
	BOOL CanItemGotoBelt(DWORD dwType) const; // Can a particular item go directly to the player's belt?
	LPCBELTITEM GetBeltItem(BYTE iRow, BYTE iCol) const; // Belt item access
	
	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	virtual void InitAttributes(); // Clean all attributes

	// Make the player drink belt potions
	DWORD HealMe(BOOL bPurpleOnly = FALSE) const; // Drink a purple/red potion
	DWORD HealMerc(BOOL bPurpleOnly = FALSE) const; // Feed the merc a purple/red potion
	DWORD SupplyMana(BOOL bDrinkPurpleIfNoBlue = TRUE) const; // Drink a blue/purple potion for mana
	int DrinkAllDrunks(BOOL bIncludeRejuv) const; // Drink all drunky potions in belt

	// Make the player move to some position
	BOOL RunTo(WORD x, WORD y, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to a position
	BOOL RunTo(const CD2StaticObject* pStaticObject, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to a static object
	BOOL RunTo(const CD2Unit* pNpc, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to an unit (npc, monster, etc)
	BOOL RunTo(LPCGROUNDITEM pItem, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to a ground item
	BOOL RunTo(LPCPLAYEROBJ pPlayer, BOOL bRun = TRUE, BOOL bPreemptive = FALSE); // Run to an other player
	BOOL TeleportTo(WORD x, WORD y, BOOL bPreemptive = FALSE, BOOL bVerifySpell = TRUE); // teleport to a position
	
	BOOL BackToTown(int nMaxRetry = 5); // Cast a town portal and go back to town immediately
	BOOL LeaveGame() const; // Save & exit

	// Make the player cast spells
	BOOL SelectSpell(WORD wSpell, BOOL bLeft, BOOL bVerifySpell = TRUE); // Select a spell
	BOOL CastSpell(WORD wSpell, BOOL bLeft, BOOL bVerifySpell = TRUE);
	BOOL CastSpell(WORD wSpell, BOOL bLeft, WORD x, WORD y, BOOL bVerifySpell = TRUE); // Cast spell on a map position
	BOOL CastSpell(WORD wSpell, BOOL bLeft, CD2StaticObject *pTarget, BOOL bVerifySpell = TRUE); // Cast spell on a static object
	BOOL CastSpell(WORD wSpell, BOOL bLeft, const CD2Unit* pTarget, BOOL bVerifySpell = TRUE); // Cast spell on an unit (npc, monster, etc)
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
	BOOL TalkToNpc(const CD2Unit* pNpc) const; // Interact with an npc
	BOOL LeaveNpc(const CD2Unit* pNpc) const; // Stop interacting with an npc
	BOOL ResurrectMerc(const CD2Unit* pNpc) const; // Resurrect merc
	BOOL OpenNpcStore(const CD2Unit* pNpc) const; // Open an npc's store
	BOOL BuyItem(DWORD dwItemID, const CD2Unit* pNpc) const; // Buy an item from npc
	BOOL RepairItem(const CD2Unit* pNpc, DWORD dwItemID = 0) const; // Repair items
	BOOL DepositGoldToStash(DWORD dwAmount = 0) const; // Deposit inventory gold to stash
	BOOL PickupCorpse() const; // Pick up player's corpse
	BOOL RedrawClient(BOOL bFade = TRUE); // Redraw client view
	
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
	virtual void OnGoldUpdate(DWORD dwInventoryGold, DWORD dwStashGold);
	virtual void OnHostile(DWORD dwHostilerID, DWORD dwHostilerCount);
	virtual void OnUnhostile(DWORD dwUnhostilerID, DWORD dwHostilerCount);
	virtual void OnPlayerDeath(BYTE iAct, WORD x, WORD y);
	virtual void OnCorpse(DWORD dwCorpseID, WORD x, WORD y);
	virtual void OnCorpsePicked();
	virtual void OnLSpell(WORD wOldSpell, WORD wNewSpell);
	virtual void OnRSpell(WORD wOldSpell, WORD wNewSpell);
	virtual void OnTeleport(BYTE iAct, WORD x, WORD y);
	virtual void OnNpcSuccessful(DWORD dwNpcID);
	virtual void OnSpellCharges(WORD wSpell, BYTE iChargesLeft);
	virtual void OnAuraOff(BOOL bSelfAura, BYTE iAuraID);
	virtual void OnAuraOn(BOOL bSelfAura, BYTE iAuraID);
	virtual void OnMoveComplete(BOOL bSuceeded, LPCMOVEJOB pLastJob);
	virtual void OnWeaponSwitch();
	virtual void OnTownFail();
	virtual void OnInventoryFull();
	
	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	BOOL CastSpellOnEntity(WORD wSpell, BOOL bLeft, DWORD dwType, DWORD dwEntityID, WORD x, WORD y, BOOL bVerifySpell);
	BOOL StopInteractWithEntity(DWORD dwType, DWORD dwEntityID) const;
	BOOL PrepareInteractWithNPC(DWORD dwNpcID, WORD  x, WORD y) const;
	BOOL InteractWithEntity(DWORD dwType, DWORD dwEntityID) const;
	BOOL GoToEntity(BOOL bRun, DWORD dwType, DWORD dwEntityID, WORD x, WORD y, BOOL bShowClientMove = TRUE) const;
	BOOL GoToPosition(BOOL bRun, WORD x, WORD y, BOOL bShowClientMove = TRUE) const;
	LPCBELTITEM FindFirstBeltItem(DWORD dwType) const;
	BOOL IsBeltColFull(int nCol) const;
	BOOL DrinkPotion(LPCBELTITEM lpItem, BOOL bFeedMerc) const;
	LPCBELTITEM FindBluePotion() const;
	LPCBELTITEM FindPurplePotion() const;
	LPCBELTITEM FindRedPotion() const;
	void CleanJobs();
	void FailJobs();
	BOOL DoFirstJob();
	void DoTPProc();

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
	WORD m_wHP; // Current HP amount
	WORD m_wMana; // Current mana amount
	WORD m_wMaxHP; // Base HP amount
	WORD m_wMaxMana; // Base mana amount
	DWORD m_dwInventoryGold; // Inventory gold
	DWORD m_dwStashGold; // Stash gold
	WORD m_wLeftSpell; // Left-hand spell
	WORD m_wRightSpell; // Right-hand spell
	D2SPELLINFO m_lastD2s; // Last spell info
	BYTE m_iCharLevel; // Character level
	BYTE m_iAuraActivated; // Actived aura
	BYTE m_iAuraAffected; // Affected by aura
	BELTITEM m_items[4][4];	// Items in belt
	BYTE m_iBeltRows; // Current belt row number
	CTicker m_ticker; // Timer Ticker
	BOOL m_bMapBlinked; // Was previous teleport OK?
	int m_nTeleportFail; // How many times the previous teleport has failed
	int m_nTPState; // "Back to town" state
	int m_nTPMaxRetry; // Maximum retry times
	int m_nTPCastfail; // TP casting failure count
	int m_nTPEnterFail; // TP entering failure count
	PLAYEROBJ m_portal; // For "back to town" usage
	DWORD m_dwLastSync; // The last time when "RedrawClient" is called

private:
	
	///////////////////////////////////////////////////////////////////////
	// Private Members
	///////////////////////////////////////////////////////////////////////
	BOOL ExamMapData(); // Check map data
	CArrayEx<MAPDATA, const MAPDATA&> m_aMapDataList; // Map data list
	CArrayEx<MAPCOUNT, const MAPCOUNT&> m_aMapCounts; // Map area list
};

#endif // __D2PLAYER_H__