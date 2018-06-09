//////////////////////////////////////////////////////////////////////
// D2HackIt.h
// -------------------------------------------------------------------
// Main incude file for D2HackIt Exports.
//
// <thohell@home.se>
//
//
// ## This file will define only Server/Client common used exports ##
// <Dan_Goon@diabloworld.com>
//////////////////////////////////////////////////////////////////////
#ifndef __D2HACKIT_H__
#define __D2HACKIT_H__

#define D2_VERSION 110
#include <windows.h>
#include "CommonStructs.h"

//////////////////////////////////////////////////////////////////////
// Version history.
// -------------------------------------------------------------------
// (See D2Hackit.txt for Version History)
//////////////////////////////////////////////////////////////////////
#define __SERVERVERSION__		MAKELONG(2, 0)
#define __SERVERBUILD__			MAKELONG(12, 05)

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#ifdef EXPORT
#undef EXPORT
#endif
#define	EXPORT			__declspec( dllexport ) __cdecl
#define PRIVATE			__cdecl

// ClientHandler.cpp
BOOL	EXPORT HookD2WndProc(HMODULE hModule, PF_HookProc pHookProc);
BOOL	EXPORT CleanMeUp(HMODULE hModule);

// GameSendPacketToServer.cpp
DWORD	EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len);
DWORD	EXPORT BnetSendPacketToServer(LPBYTE buf, DWORD len);
DWORD	EXPORT RealmSendPacketToServer(LPBYTE buf, DWORD len);
BOOL	EXPORT GameSendMessageToChat(LPSTR msg);

// GameSendPacketToGame.cpp
DWORD	EXPORT GameSendPacketToGame(LPBYTE buf, DWORD len);
BOOL	EXPORT GameInsertPacketToGame(LPBYTE buf, DWORD len);

// IniFileHandlers.cpp
LPSTR	EXPORT GetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName);
LPSTR	EXPORT GetHackProfileStringEx(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, DWORD& cSize);
LPSTR	EXPORT GetHackProfileSectionNames( LPCSTR lpHackName, DWORD& cSize );
BOOL	EXPORT SetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPCSTR lpValue);
BOOL	EXPORT FreeHackProfileString(LPCSTR lpString);
BOOL	EXPORT GetFingerprint(LPCSTR szModule, LPCSTR szFingerprintName, FINGERPRINTSTRUCT &fps);
//*****************************************************************************************************************
// New HackProfile related functions (Dan_Goon Versions)
//
// 1. These functions can be used to get the buffer size required, when NULL value is passed in lpOutBuffer.
//		In this case, the Function will only return the required buffer size without allocating the buffer.
// 2. And then caller should allocate the buffer as amount of return value in caller's routine.
// 3. Finally call those functions with caller-allocated buffer in lpOutBuffer and buffer size in nBufferSize.
// 4. Caller should free-up those buffers which are allocated by this method, when they are no more needed.
// 5. Or you can call these functions with enough sized in-stack buffers at once.
//		In this case, the returned data in buffer may not guarantee full information you required.
// 6. The output string is terminated with zero and the return value always reflects required buffer size
//		including this zero. 
// 7. When supplied buffer size is not enough to hold all results, the function returns larger value than nBufferSize.
//	   And the output string will be truncated. Caller should check the return value for this reson.
//*****************************************************************************************************************
DWORD	EXPORT GetIniFileName( LPCSTR lpHackName, LPSTR lpOutFileName, DWORD nBufferSize );
DWORD	EXPORT GetIniString( LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPSTR lpOutBuffer, DWORD nBufferSize);
DWORD	EXPORT GetIniSectionNames( LPCSTR lpHackName, LPSTR lpOutBuffer, DWORD nBufferSize );

// GamePrintFunctions.cpp
BOOL	EXPORT GamePrintInfo(LPCSTR buf);
BOOL	EXPORT GamePrintError(LPCSTR buf);
BOOL	EXPORT GamePrintVerbose(LPCSTR buf);
BOOL	EXPORT GamePrintString(LPCSTR buf);

// GameSendPacketToServer.cpp
DWORD	EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len);

// OtherExportedFunctions.cpp
PTHISGAMESTRUCT EXPORT GetThisgameStruct(void);
PSERVERINFO EXPORT GetServerInfo(DWORD dwVersion, LPCSTR szModule);
BOOL	EXPORT GameSaveAndExit();
HWND	EXPORT FindMyD2WndHandle();

// HelperFunctions.cpp
BOOL	EXPORT Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len);
BOOL	EXPORT D2FindFile(LPSTR szDest, LPCSTR szFileName, int nDestSize = _MAX_PATH);
BOOL	EXPORT PatchFill(LPVOID lpDest, LPCVOID lpSource, LPVOID lpOldCode, int len); // 2.00

// GameCommandLine.cpp
BOOL	EXPORT GameCommandLine(char* buf);

// Module directory for convinience, added v2.00 by Abin
LPCSTR EXPORT GetModuleDirectory();
SIZE EXPORT GetScreenSize(); // Get the game screen size	

// Check if a particular client(*.d2h, *.dll) was loaded, added v2.00 by Abin
BOOL EXPORT IsClientLoaded(LPCSTR lpszClientName);

// Convenient string routes, added v2.00 by Abin
BOOL EXPORT GameInfof(LPCSTR lpszFormat, ...);
BOOL EXPORT GameErrorf(LPCSTR lpszFormat, ...);
BOOL EXPORT GameStringf(LPCSTR lpszFormat, ...);
BOOL EXPORT GameCommandf(LPCSTR lpszFormat, ...);

// Packet printing, added v2.00 by Abin
BOOL EXPORT GamePrintPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen);

// Game ready check, added v2.00 by Abin
BOOL EXPORT IsGameReady();
BOOL EXPORT WasD2HackitLoadedBeforeJoinGame();

// Unit enumeration
BOOL EXPORT EnumUnits(DWORD dwUnitType, fnEnumUnitProc lpfnEnumUnitProc, LPARAM lParam = 0);

BOOL EXPORT VerifyUnit(LPCGAMEUNIT lpUnit);
BOOL EXPORT FindUnitByClassID(DWORD dwUnitClassID, DWORD dwUnitType, LPGAMEUNIT lpBuffer);
BOOL EXPORT FindUnitByName(LPCSTR lpszUnitName, DWORD dwUnitType, LPGAMEUNIT lpBuffer);
POINT EXPORT GetUnitPosition(LPCGAMEUNIT lpUnit);
DWORD EXPORT GetUnitName(LPCGAMEUNIT lpUnit, LPSTR lpszBuffer, DWORD dwMaxChars);
DWORD EXPORT GetUnitOwnerID(LPCGAMEUNIT lpUnit); // extremely useful for pets/summonings
DWORD EXPORT GetUnitClassID(LPCGAMEUNIT lpUnit);
BYTE EXPORT GetUnitHPPercent(LPCGAMEUNIT lpUnit); // hp percent 0-100
DWORD EXPORT GetUnitStat(LPCGAMEUNIT lpUnit, int nStatID);
BOOL EXPORT GetUnitAffection(LPCGAMEUNIT lpUnit, int nAffectionID);
DWORD EXPORT GetAffectionName(BYTE iAffectionID, LPTSTR lpszBuffer, DWORD dwMaxChars);
BOOL EXPORT AffectionIsAura(BYTE iAffectionID);
DWORD EXPORT IsMercUnit(LPCGAMEUNIT lpUnit); // Check if an unit is a merc
int EXPORT GetMonsterType(LPCGAMEUNIT lpUnit); // normal/champion/boss/minion
DWORD EXPORT GetMonsterAttributes(LPCGAMEUNIT lpUnit); // monster attributes
DWORD EXPORT GetDistance(long x1, long y1, long x2, long y2); // distance between two positions, in yards
DWORD EXPORT GetPing(); // Get ping value (lag indicator)
BOOL EXPORT SendKey(BYTE iVKCode); // Send a key-down message to d2 window
DWORD EXPORT GetSpellName(WORD wSpell, LPSTR lpszBuffer, DWORD dwMaxChars);

// map related
DWORD EXPORT GetMapName(BYTE iMapID, LPTSTR lpszBuffer, DWORD dwMaxChars); // Name of the map
BOOL EXPORT MapIsTown(BYTE iMapID); // Is town?
BYTE EXPORT GetTownMap(BYTE iAct);
BYTE EXPORT GetActFromMap(BYTE iMapID);

// item related
SIZE EXPORT GetItemSize(LPCSTR lpszItemCode);

// game attributes
DWORD EXPORT GetD2WindowTitle(LPSTR lpszBuffer, DWORD dwMaxChars);
DWORD EXPORT GetInGameTime(); // time elapsed since joining
int EXPORT GetGameDifficulty(); // current game difficulty, normal/nm/hell
void EXPORT ScreenToMapCoords(POINT& rScreenCoords);
void EXPORT MapToScreenCoords(POINT& rMapPosition);
POINT EXPORT GetMousePosition();

LPCSTR EXPORT GetRealmName();
LPCSTR EXPORT GetGameName();
LPCSTR EXPORT GetGamePassword();
LPCSTR EXPORT GetGameIP();

BOOL EXPORT EnumPresetUnits(fnEnumPresetUnitProc lpfnEnumPresetUnitProc, LPARAM lParam = 0);
POINT EXPORT FindPresetUnit(DWORD dwUnitID, DWORD dwUnitType);
LPCSTR EXPORT GetAccountName();
BOOL EXPORT HaveSpell(WORD wSpellID);
BOOL EXPORT IsInTown();
BOOL EXPORT GetMyMercUnit(LPGAMEUNIT lpBuffer); // Get player's mercenary
DWORD EXPORT GetID(); // player ID
LPCSTR EXPORT GetName(); // Player name
DWORD EXPORT GetMode();	// Get player's current mode(walking/running/attacking etc)
WORD EXPORT GetSelectedSpell(BOOL bLeftHand); // Get the selected spell
BYTE EXPORT GetHPPercent();	// Player HP percent 0-100
BYTE EXPORT GetManaPercent();	// Player mana percent 0-100
BYTE EXPORT GetItemDurabilityPercent();	// Player item durability percent 0-100
DWORD EXPORT GetStat(int nStatID);	// General stat access, hp/maxhp/mana/maxmana/mf/ias etc
BOOL EXPORT GetAffection(int nAffectionID);
BOOL EXPORT SelectSpell(WORD wSpell, BOOL bLeftHand = FALSE);
POINT EXPORT GetPosition(); // player position
int EXPORT GetMyCorpseCount();
BOOL EXPORT PickMyCorpse();
int EXPORT GetHostilerCount(); // number of players that are hostiled to me
BYTE EXPORT GetCurrentMapID();
DWORD EXPORT GetDistanceFrom(long x, long y); // Player's distance from a position
DWORD EXPORT HealMe(BOOL bKeepBottomRow = FALSE); // Drink a purple/red potion
DWORD EXPORT HealMerc(BOOL bKeepBottomRow = FALSE); // Feed the merc a purple/red potion
DWORD EXPORT SupplyMana(BOOL bDrinkPurpleIfNoBlue = TRUE, BOOL bKeepBottomRow = FALSE); // Drink a blue/purple potion for mana
BOOL EXPORT DrinkBeltPotion(DWORD dwItemID, BOOL bFeedMerc);
BOOL EXPORT DrinkInventoryPotion(DWORD dwItemID);
BYTE EXPORT GetBeltRows(); // Row number of the player's belt(1-4)
DWORD EXPORT GetBeltItem(int nSlot); // Belt item access, slot: 0-15
BOOL EXPORT CastNoTarget(WORD wSpell, BOOL bLeftHand = FALSE);
BOOL EXPORT CastOnMap(WORD wSpell, long x, long y, BOOL bLeftHand = FALSE);
BOOL EXPORT CastOnUnit(WORD wSpell, LPCGAMEUNIT lpUnit, BOOL bLeftHand = FALSE);
BOOL EXPORT LeaveGame(); // save & exit
void EXPORT PauseJobs(BOOL bPause);
void EXPORT CleanJobs();
BOOL EXPORT BackToTown(); // Cast a town portal and go back to town immediately
BOOL EXPORT MoveTo(long x, long y, BOOL bQueueJob = TRUE); // Run to a position
BOOL EXPORT MoveToUnit(LPCGAMEUNIT lpUnit, BOOL bQueueJob = TRUE); // Run to a static object
BOOL EXPORT TeleportTo(long x, long y, BOOL bQueueJob = TRUE); // teleport to a position
BOOL EXPORT Say(LPCSTR lpszMessage); // Say a message
BOOL EXPORT OverHead(LPCSTR lpszMessage, BOOL bClientOnly = TRUE); // Display an over-head message
BOOL EXPORT SendSound(BYTE iSound); // Send a sound
BOOL EXPORT AntiIdle(); // Anti-idle
BOOL EXPORT WeaponSwitch(); // Do weapon switch
BOOL EXPORT EnterWaypoint(LPCGAMEUNIT lpWPUnit, BYTE iDestinationMapID); // take WP to another area
BOOL EXPORT Interact(LPCGAMEUNIT lpUnit);
BOOL EXPORT DepositGoldToStash(DWORD dwAmount = 0); // Deposit inventory gold to stash
BOOL EXPORT RedrawClient(BOOL bFade = TRUE); // Redraw client view
DWORD EXPORT GetStashGoldLimit(); // stash gold limit
DWORD EXPORT GetInventoryGoldLimit(); // inventory gold limit
int EXPORT GetMaxDifficulty();	// the max difficulty to which this player has access
BOOL EXPORT IsHardcore(); // is hardcore player?
BOOL EXPORT GetAlwaysRun(); // is "always run" on?
DWORD EXPORT GetCursorItem(); // returns ID of the item on cursor, if any
BOOL EXPORT DropCursorItemToGround();

BOOL EXPORT PickGroundItem(DWORD dwItemID, BOOL bShowClientMove = TRUE); // Pick up a ground item
BOOL EXPORT Transmute();
BOOL EXPORT IsUIOpened(int nUIType);

BOOL EXPORT EnumStorageItems(int nStorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam = 0);
BOOL EXPORT PickStorageItemToCursor(DWORD dwItemID);
BOOL EXPORT FindFirstStorageSpace(int nStorageType, SIZE minSize, LPPOINT lpBuffer);
BOOL EXPORT DropCursorItemToStorage(int nStorageType);
BOOL EXPORT GetItemCode(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars);

BOOL EXPORT LeftClick(long x, long y);
BOOL EXPORT RightClick(long x, long y);
BYTE EXPORT GetOpenedUI();
void EXPORT CloseAllUIs();

BOOL EXPORT ResurrectMerc(); // Resurrect merc
BOOL EXPORT IdentifyItems();
BOOL EXPORT BuyItem(DWORD dwItemID); // Buy an item from npc
BOOL EXPORT RepairItem(DWORD dwItemID = 0); // Repair items

BOOL EXPORT BuyHealingPotion(int nQuantity = 0, DWORD dwDelay = 500);
BOOL EXPORT BuyManaPotion(int nQuantity = 0, DWORD dwDelay = 500);
BOOL EXPORT BuyTownScroll(int nQuantity = 0, DWORD dwDelay = 500);
BOOL EXPORT BuyIdentifyScroll(int nQuantity = 0, DWORD dwDelay = 500);
BOOL EXPORT BuyArrows(int nQuantity, DWORD dwDelay = 500);
BOOL EXPORT BuyBolts(int nQuantity, DWORD dwDelay = 500);
BOOL EXPORT Gamble(DWORD dwItemID); // gamble an item
BOOL EXPORT SellItem(DWORD dwItemID);
BOOL EXPORT StartNpcSession(LPCGAMEUNIT lpNpcUnit, int nInteractType = NPC_NORMAL);
void EXPORT EndNpcSession();
int EXPORT DrinkAllInventoryPotions(BOOL bKeepFullRejuv = FALSE, DWORD dwDelay = 500);
BOOL EXPORT IsMercAlive();
BOOL EXPORT DropGold(DWORD dwGoldAmount);
DWORD EXPORT GetSpellCharges(WORD wSpellID);

int EXPORT GetUnidItemCount();
DWORD EXPORT GetEquipItem(int nEquipLocation);

DWORD EXPORT QueryModuleData(LPCSTR lpszModuleName, DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam = 0);
DWORD EXPORT GetKeyName(BYTE iKeyCode, LPSTR lpszBuffer, DWORD dwMaxChars);
int EXPORT GetAvailableBeltSlots(int nColumn);
BYTE EXPORT GetCurrentAct();
DWORD EXPORT GetStorageItem(int nStorageType, int x, int y);
int EXPORT SellInventoryItems(int nLeftColumn, int nRightColumn, DWORD dwDelay = 500);
int EXPORT GambleAmulets(DWORD dwDelay = 500);
int EXPORT GambleRings(DWORD dwDelay = 500);
int EXPORT GambleCirclets(DWORD dwDelay = 500);
int EXPORT GambleGloves(DWORD dwDelay = 500);
int EXPORT GambleBoots(DWORD dwDelay = 500);
int EXPORT GambleBelts(DWORD dwDelay = 500);
BOOL EXPORT IsNpcSessionStarted(DWORD dwNpcID, int nInteractType);
BOOL EXPORT IsQuestItem(LPCSTR lpszItemCode);
BOOL EXPORT IsWeaponSwitched();
int EXPORT GetItemQuality(DWORD dwItemID);
BOOL EXPORT GetItemPrefixSuffix(DWORD dwItemID, WORD* pPrefix, WORD* pSuffix);
DWORD EXPORT GetPrefixSuffixName(WORD wPrefixOrSuffix, LPSTR lpszBuffer, DWORD dwMaxChars);
POINT EXPORT GetRandomPosition(int nRadiusMin, int nRadiusMax, int nAngleMin = 0, int nAngleMax = 360, const LPPOINT lpOrigin = NULL);
int EXPORT GetAngle(long x1, long y1, long x2, long y2);
int EXPORT GetAngleFrom(long x, long y);
BOOL EXPORT CreateCollisionMap(); // 1000 * 1000
WORD EXPORT GetCollisionData(long x, long y, BOOL bAbsolute);
BOOL EXPORT DumpCollisionMap(LPCSTR lpszFilePath, const LPPOINT lpTokens = NULL, DWORD dwTokenCount = 0);
DWORD EXPORT CalculateTeleportPath(long x, long y, LPPOINT lpBuffer, DWORD dwMaxCount);
DWORD EXPORT CalculateWalkPath(long x, long y, LPPOINT lpBuffer, DWORD dwMaxCount);
DWORD EXPORT GetItemPrice(DWORD dwItemID, DWORD dwNpcClassID, BOOL bBuyOrSell);
DWORD EXPORT GetItemAttributes(DWORD dwItemID);
BOOL EXPORT GetMapRect(LPRECT lpBuffer);
BYTE EXPORT GetItemSockets(DWORD dwItemID);
BOOL EXPORT UnitOverhead(LPCGAMEUNIT lpUnit, LPCSTR lpszText, DWORD dwDuration = 8000);
DWORD EXPORT GetItemProperties(DWORD dwItemID, LPITEMPROPERTY lpBuffer, DWORD dwMaxCount);
BOOL EXPORT PutInventoryPotionToBelt(DWORD dwItemID);
BOOL EXPORT GetHighlightUnit(LPGAMEUNIT lpBuffer);
DWORD EXPORT GetUnitMode(LPCGAMEUNIT lpUnit);
BOOL EXPORT EnumGroundItems(fnEnumItemProc lpfn, LPARAM lParam);
BOOL EXPORT ExamItemProperties(DWORD dwItemID, LPCSTR lpszExpression, LPCSTR lpszCodes = NULL, BOOL* pSyntaxError = NULL);
BOOL EXPORT EnumPlayers(fnEnumPlayerProc lpfn, LPARAM lParam);
DWORD EXPORT GetPvpFlags(DWORD dwPlayerID);
DWORD EXPORT GetClassID();
BYTE EXPORT GetItemLevel(DWORD dwItemID);
DWORD EXPORT GetItemDescription(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor = TRUE);
DWORD EXPORT GetD2HackitVersion();
DWORD EXPORT GetD2HackitBuild();

HD2BITMAP EXPORT LoadD2Bitmap(LPCSTR lpszFilePath);
BOOL EXPORT DestroyD2Bitmap(HD2BITMAP hBitmap);
DWORD EXPORT GetUnitBaseStat(LPCGAMEUNIT lpUnit, int nStatID);
DWORD EXPORT GetBaseStat(int nStatID);
DWORD EXPORT GetItemStrRequirement(DWORD dwItemID);
DWORD EXPORT GetItemDexRequirement(DWORD dwItemID);
DWORD EXPORT GetItemLevelRequirement(DWORD dwItemID);

#ifdef D2HACKIT_DEBUG
extern	char g_szDebug[512];

#define	DBGHDR			g_szDebug, "D2HACKIT : "
#define DGTRACE( _x_ )	sprintf _x_; OutputDebugString(g_szDebug);
#else
#define DGTRACE( _x_ )
#endif

#ifdef __cplusplus
}		             /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#endif // __D2HACKIT_H__

