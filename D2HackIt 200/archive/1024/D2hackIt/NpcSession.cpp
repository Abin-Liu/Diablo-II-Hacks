///////////////////////////////////////////////////////////
// NpcSession.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "NpcSession.h"
#include "CommonStructs.h"
#include "Utilities\\StateTicker.h"
#include "me.h"
#include "D2HackIt.h"
#include "Server20.h"
#include "d2functions.h"
#include "GameMessage.h"
#include "ItemCheck.h"

typedef struct tagNpcStore
{
	GAMEUNIT npc;
	int nInteractType;
	BOOL bSessionStarted;
	
	// used for compare potion levels
	char szHealingPotionCode[4]; // hp1/hp2/hp3/hp4/hp5
	char szManaPotionCode[4]; // mp1/mp2/mp3/mp4/mp5
	
	// scrolls
	DWORD dwTownScrollID;
	DWORD dwIdentifyScrollID;

	// potions
	DWORD dwHealingPotionID;
	DWORD dwManaPotionID;
	DWORD dwAntidotePotionID;

	// ammo
	DWORD dwArrowID;
	DWORD dwBoltID;

	// gamble
	DWORD aAmulets[5]; // actually there will be only 1 amulet
	DWORD aRings[5]; // actually there will be only 1-2 rings
	DWORD aCirclets[5];
	DWORD aGloves[5];
	DWORD aBoots[5];
	DWORD aBelts[5];

} NPCSTORE, *LPNPCSTORE;
typedef const tagNpcStore* LPCNPCSTORE;

#define MAX_NPC_FAIL	5
enum { NPC_NONE = 0, NPC_RUNTO, NPC_INTERACT, NPC_WAIT_INTERACT, NPC_OPENSTORE, NPC_WAIT_STORE, NPC_STORE_OK };
NPCSTORE g_npcStore = { 0 };
int g_nNpcRetry = 0;
CStateTicker g_npcTicker;

struct NPC_DATA
{
	DWORD dwClassID; // class id
	int nMenuIndex; // npc menu item index for trading and gambling
};

// Vendors
static const NPC_DATA VENDORS[] = { {154, 1}, // Charis
									{147, 1}, // Gheed
									{148, 1}, // Akara
									{199, 1}, // Elzix
									{292, 1}, // lysander
									{178, 1}, // fara
									{177, 1}, // drognan
									{254, 1}, // alkor
									{252, 2}, // asheara
									{255, 1}, // ormus
									{253, 1}, // hratil
									{405, 0}, // jamella
									{257, 0}, // halbu
									{511, 1}, // larzuk
									{513, 1}, // malah
									{512, 1}, // anya
									{0, -1}};

// Gamblers
static const NPC_DATA GAMBLERS[] = { {147, 2}, // Gheed
									{199, 2}, // Elzix
									{254, 2}, // alkor
									{405, 1}, // jamella
									{512, 2}, // anya
									{514, 1}, // Nihlathak									
									{0, -1}};

// Resurrectors
static const DWORD RESURRECTORS[] = { 150, // Kashya
									198, // Greiz
									252, // asheara
									367, // tyrael
									515, // qual-khek
									0};

int GetNpcTradeMenuID(DWORD dwClassID);
int GetNpcGambleMenuID(DWORD dwClassID);
void NotifyNpcSession(BOOL bSuccessful);
BOOL PrepareInteractWithNPC(LPCGAMEUNIT lpNpcUnit);
BOOL OpenNpcStore(LPCGAMEUNIT lpNpcUnit, BOOL bGamble); // Open an npc's store
int CompareHealingPotion(LPCSTR lpszCode1, LPCSTR lpszCode2);
int CompareManaPotion(LPCSTR lpszCode1, LPCSTR lpszCode2);
BOOL BuyMiscItem(DWORD dwItemID, LPCGAMEUNIT lpNpcUnit, int nQuantity, DWORD dwDelay);
void InsertGambleItem(DWORD dwItemID, DWORD* aItems, DWORD dwArrayLen);
int BatchGamble(DWORD* aItems, DWORD dwArrayLen, DWORD dwDelay);

BOOL IsVendor(DWORD dwClassID)
{
	for (int i = 0; VENDORS[i].dwClassID; i++)
	{
		if (VENDORS[i].dwClassID == dwClassID)
			return TRUE;
	}

	return FALSE;
}

BOOL IsGambler(DWORD dwClassID)
{
	for (int i = 0; GAMBLERS[i].dwClassID; i++)
	{
		if (GAMBLERS[i].dwClassID == dwClassID)
			return TRUE;
	}

	return FALSE;
}

BOOL IsResurrector(DWORD dwClassID)
{
	for (int i = 0; RESURRECTORS[i]; i++)
	{
		if (RESURRECTORS[i] == dwClassID)
			return TRUE;
	}

	return FALSE;
}

BOOL IsCurrentNpcResurrector()
{
	return IsResurrector(GetUnitClassID(&g_npcStore.npc));
}

int GetNpcTradeMenuID(DWORD dwClassID)
{
	for (int i = 0; VENDORS[i].dwClassID; i++)
	{
		if (VENDORS[i].dwClassID == dwClassID)
			return VENDORS[i].nMenuIndex;
	}

	return -1;
}

int GetNpcGambleMenuID(DWORD dwClassID)
{
	for (int i = 0; GAMBLERS[i].dwClassID; i++)
	{
		if (GAMBLERS[i].dwClassID == dwClassID)
			return GAMBLERS[i].nMenuIndex;
	}

	return -1;
}

DWORD GetCurrentNpcID()
{
	return g_npcStore.npc.dwUnitID;
}

BOOL IsNpcActive(int nInteractType)
{
	return IsUIOpened(UI_NPCMENU)
		&& g_npcStore.npc.dwUnitID
		&& g_npcStore.nInteractType == nInteractType
		&& g_npcStore.bSessionStarted;
}

BOOL IsNpcBullShitting()
{
	// is the npc bull shitting with me? (quests assigning or information chatting)
	DWORD dwResult = 0;
	if (!::ReadProcessMemory(::GetCurrentProcess(), (LPCVOID)0x6FBB9EA8, &dwResult, 4, NULL))
		return FALSE;
	return dwResult;	
}

BOOL PrepareInteractWithNPC(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return FALSE;

	MAPPOS mp = GetUnitPosition(lpUnit);
	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x59;
	aPacket[1] = 0x01;
	::memcpy(aPacket + 5, &lpUnit->dwUnitID, 4);
	::memcpy(aPacket + 9, &mp.x, 2);
	::memcpy(aPacket + 13, &mp.y, 2);
	return GameSendPacketToServer(aPacket, 17);
}

BOOL OpenNpcStore(LPCGAMEUNIT lpNpcUnit, BOOL bGamble) // Open an npc's store
{
	if (!VerifyUnit(lpNpcUnit))
		return FALSE;

	// verify npc capability
	DWORD dwClassID = GetUnitClassID(&g_npcStore.npc);
	int nMenuID = -1;
	if (bGamble)
		nMenuID = GetNpcGambleMenuID(dwClassID);
	else
		nMenuID = GetNpcTradeMenuID(dwClassID);
	if (nMenuID == -1)
		return FALSE;

	nMenuID++;

	// D2BS code for clicking npc menus
	D2MSG MouseXY = {0};
	d2_NpcMenu *NpcMenu = &(**p_D2CLIENT_NpcMenu);
	if (!NpcMenu)
		return FALSE;
	
	MouseXY.pt.X=(WORD)NpcMenu->X +32;
	MouseXY.pt.Y=(WORD)NpcMenu->Y +15 +15*(WORD)nMenuID;
	NpcMenu->clicked=1;
	NpcMenu->MenuId1=nMenuID;
	NpcMenu->MenuId2=nMenuID;

	// Avoid mouse position change when d2 is minimized
	POINT ptOrig = {0};
	::GetCursorPos(&ptOrig);
	D2CLIENT_ClickNpcMenuUI((DWORD)FindMyD2WndHandle(),(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
	::SetCursorPos(ptOrig.x, ptOrig.y);

	return TRUE;
		/*
	BYTE aPacket[13] = { 0 };
	aPacket[0] = 0x38;
	aPacket[1] = bGamble ? 0x02 : 0x01;
	::memcpy(aPacket + 5, &lpNpcUnit->dwUnitID, 4);
	return GameSendPacketToServer(aPacket, 13);
	*/
}


void CleanNpcSessionStates()
{
	g_nNpcRetry = 0;
	g_npcTicker.Clear();
	::memset(&g_npcStore, 0, sizeof(NPCSTORE));
}

void NotifyNpcSession(BOOL bSuccessful)
{
	g_npcTicker.Clear();
	DWORD NPCID = g_npcStore.npc.dwUnitID;	
	g_nNpcRetry = 0;
	g_npcStore.bSessionStarted = bSuccessful;
	if (!bSuccessful)
		::memset(&g_npcStore, 0, sizeof(NPCSTORE));
	QueueThisPlayerMessage(PM_NPCSESSION, (WPARAM)bSuccessful, (LPARAM)NPCID);
}

BOOL ProcessNpcSession(const volatile BYTE* lpStopFlag)
{	
	if (g_npcTicker.GetState() == NPC_NONE)
		return FALSE;

	g_npcTicker.TickIt(100);
	if (!g_npcTicker.IsTickEnded())
		return TRUE;
	
	if (g_npcStore.npc.dwUnitID == 0)
	{
		NotifyNpcSession(FALSE);
		return FALSE;
	}

	switch (g_npcTicker.GetState())
	{
	case NPC_RUNTO:
		if (!MoveToUnit(&g_npcStore.npc))
		{
			NotifyNpcSession(FALSE);
		}
		else
		{
			g_npcTicker.Update(NPC_INTERACT, 1000);
		}
		break;

	case NPC_INTERACT:
		if (!PrepareInteractWithNPC(&g_npcStore.npc) || !Interact(&g_npcStore.npc))
		{
			NotifyNpcSession(FALSE);
		}
		else
		{
			g_npcTicker.Update(NPC_WAIT_INTERACT, 2000);
		}
		break;

	case NPC_WAIT_INTERACT:
		if (IsUIOpened(UI_NPCMENU))
		{
			g_nNpcRetry = 0;
			if (g_npcStore.nInteractType)
			{
				g_npcTicker.Update(NPC_OPENSTORE, 500);
			}
			else
			{
				NotifyNpcSession(TRUE);
			}			
		}
		else
		{			
			g_nNpcRetry++;
			//GameInfof("npc fails: %d", g_nNpcFail);
			if (g_nNpcRetry < MAX_NPC_FAIL)
			{
				//if (IsNpcBullShitting())
				//	SendKey(VK_ESCAPE); // close the npc bull-shitting screen
				CloseAllUIs();
				g_npcTicker.Update(NPC_INTERACT, 500);
			}
			else
			{
				NotifyNpcSession(FALSE);
			}
		}
		break;

	case NPC_OPENSTORE:
		if (!IsUIOpened(UI_NPCMENU) || !OpenNpcStore(&g_npcStore.npc, g_npcStore.nInteractType == NPC_GAMBLE))
		{
			NotifyNpcSession(FALSE);
		}
		else
		{
			g_npcTicker.Update(NPC_WAIT_STORE, 5000);
		}
		break;

	case NPC_WAIT_STORE:
		if (!IsUIOpened(UI_NPCMENU) || g_nNpcRetry)
		{
			NotifyNpcSession(FALSE);
		}
		else
		{
			g_nNpcRetry++;
			g_npcTicker.Update(NPC_OPENSTORE, 0);
		}
		break;

	case NPC_STORE_OK:
		NotifyNpcSession(TRUE);
		break;

	default:
		break;
	}

	return TRUE;
}

void OnVendorItemPacket(const BYTE* aPacket, DWORD aLen)
{
	if (g_npcStore.npc.dwUnitID)
	{
		DWORD dwItemID = D2ParseItemIDFromPacket(aPacket, aLen);
		char szItemCode[4] = "";
		D2ParseItemCodeFromPacket(aPacket, aLen, szItemCode);
		AnalyzeVendorItem(dwItemID, szItemCode);

		if (g_npcTicker.GetState() == NPC_WAIT_STORE || g_npcTicker.GetState() == NPC_STORE_OK)
		{
			g_npcTicker.Update(NPC_STORE_OK, 1000);
		}
	}
}

void AnalyzeVendorItem(DWORD dwItemID, LPCSTR lpszItemCode)
{
	if (dwItemID == 0 || lpszItemCode == NULL)
		return; // invalid item	

	if (g_npcStore.nInteractType == NPC_TRADE)
	{
		// healing potion "hp1/2/3/4/5"
		if (lpszItemCode[0] == 'h' && lpszItemCode[1] == 'p')
		{
			if (lpszItemCode[2] > g_npcStore.szHealingPotionCode[2])
			{
				g_npcStore.dwHealingPotionID = dwItemID;
				strcpy(g_npcStore.szHealingPotionCode, lpszItemCode);
			}
		}	

		// mana potion "mp1/2/3/4/5"
		else if (lpszItemCode[0] == 'm' && lpszItemCode[1] == 'p')
		{		
			if (lpszItemCode[2] > g_npcStore.szManaPotionCode[2])
			{
				g_npcStore.dwManaPotionID = dwItemID;
				strcpy(g_npcStore.szManaPotionCode, lpszItemCode);
			}
		}

		// antidote potion
		else if (!stricmp(lpszItemCode, "yps"))
		{
			g_npcStore.dwAntidotePotionID = dwItemID;
		}

		// town scroll
		else if (!stricmp(lpszItemCode, "tsc"))
		{
			g_npcStore.dwTownScrollID = dwItemID;
		}

		// identify scroll
		else if (!stricmp(lpszItemCode, "isc"))
		{
			g_npcStore.dwIdentifyScrollID = dwItemID;
		}

		// arrow
		else if (!stricmp(lpszItemCode, "aqv"))
		{
			g_npcStore.dwArrowID = dwItemID;
		}

		// bolt
		else if (!stricmp(lpszItemCode, "cqv"))
		{
			g_npcStore.dwBoltID = dwItemID;
		}
	}
	else if (g_npcStore.nInteractType == NPC_GAMBLE)
	{
		// amulet
		if (!stricmp(lpszItemCode, "amu"))
		{
			InsertGambleItem(dwItemID, g_npcStore.aAmulets, 5);
		}

		// ring
		else if (!stricmp(lpszItemCode, "rin"))
		{
			InsertGambleItem(dwItemID, g_npcStore.aRings, 5);
		}

		// circlets
		else if (D2IsCirclets(lpszItemCode))
		{
			InsertGambleItem(dwItemID, g_npcStore.aCirclets, 5);
		}

		// gloves
		else if (D2IsGloves(lpszItemCode))
		{
			InsertGambleItem(dwItemID, g_npcStore.aGloves, 5);
		}

		// blets (4-row belts only)
		else if (D2IsBelt(lpszItemCode) == 4)
		{
			InsertGambleItem(dwItemID, g_npcStore.aBelts, 5);
		}

		// boots
		else if (D2IsBoots(lpszItemCode))
		{
			InsertGambleItem(dwItemID, g_npcStore.aBoots, 5);
		}
	}
}

BOOL BuyMiscItem(DWORD dwItemID, LPCGAMEUNIT lpNpcUnit, int nQuantity, DWORD dwDelay)
{
	if (!dwItemID || !lpNpcUnit || !VerifyUnit(lpNpcUnit))
		return FALSE;

	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x32;
	::memcpy(&aPacket[1], &lpNpcUnit->dwUnitID, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	
	if (nQuantity <= 0)
	{
		aPacket[12] = 0x80;
		return GameSendPacketToServer(aPacket, 17);
	}
	
	for (int i = 0; i < nQuantity && i < 40; i++)
	{
		GameSendPacketToServer2(aPacket, 17, dwDelay);
	}

	return TRUE;
}

BOOL EXPORT BuyHealingPotion(int nQuantity, DWORD dwDelay)
{
	if (!g_npcStore.dwHealingPotionID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No healing potion found!");
		return FALSE;	
	}	

	return BuyMiscItem(g_npcStore.dwHealingPotionID, &g_npcStore.npc, nQuantity, dwDelay);
}

BOOL EXPORT BuyManaPotion(int nQuantity, DWORD dwDelay)
{
	if (!g_npcStore.dwManaPotionID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No mana potion found!");
		return FALSE;	
	}

	return BuyMiscItem(g_npcStore.dwManaPotionID, &g_npcStore.npc, nQuantity, dwDelay);
}

BOOL EXPORT BuyTownScroll(int nQuantity, DWORD dwDelay)
{
	if (!g_npcStore.dwTownScrollID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No town scroll found!");
		return FALSE;	
	}

	return BuyMiscItem(g_npcStore.dwTownScrollID, &g_npcStore.npc, nQuantity, dwDelay);
}

BOOL EXPORT BuyIdentifyScroll(int nQuantity, DWORD dwDelay)
{
	if (!g_npcStore.dwIdentifyScrollID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No identify scroll found!");
		return FALSE;	
	}

	return BuyMiscItem(g_npcStore.dwIdentifyScrollID, &g_npcStore.npc, nQuantity, dwDelay);
}

BOOL EXPORT BuyArrows(int nQuantity, DWORD dwDelay)
{
	if (!g_npcStore.dwArrowID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No arrows found!");
		return FALSE;	
	}
	
	if (nQuantity < 1)
		return FALSE;	

	return BuyMiscItem(g_npcStore.dwArrowID, &g_npcStore.npc, nQuantity, dwDelay);
}

BOOL EXPORT BuyBolts(int nQuantity, DWORD dwDelay)
{
	if (!g_npcStore.dwBoltID || !IsNpcActive(NPC_TRADE))
	{
		GameErrorf("No bolts found!");
		return FALSE;	
	}
	
	if (nQuantity < 1)
		return FALSE;

	return BuyMiscItem(g_npcStore.dwBoltID, &g_npcStore.npc, nQuantity, dwDelay);
}

BOOL EXPORT StartNpcSession(LPCGAMEUNIT lpNpcUnit, int nInteractType)
{
	UnitAny* pUnit = (UnitAny*)VerifyUnit(lpNpcUnit);
	if (pUnit == NULL)
	{
		//GamePrintError("Invalid unit");
		return FALSE;
	}
	
	if (nInteractType == NPC_TRADE && GetNpcTradeMenuID(pUnit->dwClassId) == -1)
	{
		//GamePrintError("Not a vendor");
		return FALSE; // not a vendor
	}

	if (nInteractType == NPC_GAMBLE && GetNpcGambleMenuID(pUnit->dwClassId) == -1)
	{
		//GamePrintError("Not a Gambler");
		return FALSE; // not a gambler
	}

	CleanOutgoingPacketQueue();
	g_nNpcRetry = 0;
	::memset(&g_npcStore, 0, sizeof(NPCSTORE));
	::memcpy(&g_npcStore.npc, lpNpcUnit, sizeof(GAMEUNIT));
	g_npcStore.nInteractType = nInteractType;
	CleanJobs();
	CloseAllUIs();
	g_npcTicker.Update(NPC_RUNTO, 500);
	return TRUE;
}

void EXPORT EndNpcSession()
{
	g_npcTicker.Clear();
	::memset(&g_npcStore, 0, sizeof(NPCSTORE));
	CloseAllUIs();
}

void InsertGambleItem(DWORD dwItemID, DWORD* aItems, DWORD dwArrayLen)
{
	if (dwItemID == 0 || aItems == NULL || dwArrayLen == 0)
		return;

	for (DWORD i = 0; i < dwArrayLen; i++)
	{
		if (aItems[i] == 0)
		{
			aItems[i] = dwItemID;
			return;
		}
	}
}

int BatchGamble(DWORD* aItems, DWORD dwArrayLen, DWORD dwDelay)
{
	if (aItems == NULL || dwArrayLen == 0)
		return 0;

	int nCount = 0;
	for (DWORD i = 0; i < dwArrayLen; i++)
	{
		if (aItems[i])
		{
			BYTE aPacket[17] = { 0 };
			aPacket[0] = 0x32;
			::memcpy(&aPacket[1], &g_npcStore.npc.dwUnitID, 4);
			::memcpy(&aPacket[5], &aItems[i], 4);
			aPacket[9] = 0x02;
			GameSendPacketToServer2(aPacket, 17, dwDelay);
			nCount++;
		}

		aItems[i] = 0;
	}

	return nCount;
}

int EXPORT GambleAmulets(DWORD dwDelay)
{
	return BatchGamble(g_npcStore.aAmulets, 5, dwDelay);
}

int EXPORT GambleRings(DWORD dwDelay)
{
	return BatchGamble(g_npcStore.aRings, 5, dwDelay);
}

int EXPORT GambleCirclets(DWORD dwDelay)
{
	return BatchGamble(g_npcStore.aCirclets, 5, dwDelay);
}

int EXPORT GambleGloves(DWORD dwDelay)
{
	return BatchGamble(g_npcStore.aGloves, 5, dwDelay);
}

int EXPORT GambleBoots(DWORD dwDelay)
{
	return BatchGamble(g_npcStore.aBoots, 5, dwDelay);
}

int EXPORT GambleBelts(DWORD dwDelay)
{
	return BatchGamble(g_npcStore.aBelts, 5, dwDelay);
}

BOOL EXPORT IsNpcSessionStarted(DWORD dwNpcID, int nInteractType)
{
	if (!dwNpcID || g_npcStore.npc.dwUnitID != dwNpcID || !g_npcStore.bSessionStarted)
		return FALSE;

	if (!nInteractType)
		return TRUE;

	return g_npcStore.nInteractType == nInteractType;
}

void OnNpcClosed(DWORD dwNpcID)
{
	CleanOutgoingPacketQueue();
	g_npcStore.bSessionStarted = FALSE;
}
