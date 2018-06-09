/////////////////////////////////////////////////////////////////////
// I believe these are Mousepad's code
// Also some were from D2BS.
//////////////////////////////////////////////////////////////////////
#ifndef __D2EXPFUNCS
#define __D2EXPFUNCS

#include "definitions.h"

#define D2EXPFUNC(d1,v1,t1,t2) typedef t1 d1##_##v1##_t t2; extern d1##_##v1##_t *d1##_##v1;
#define D2EXPVAR(d1,v1,t1)     typedef t1 d1##_##v1##_t;    extern d1##_##v1##_t *p_##d1##_##v1;
#define D2EXPASM(d1,v1)        extern DWORD d1##_##v1;


/*  D2Client.dll  */

// internal functions:
D2EXPFUNC(D2CLIENT, PrintGameStringAtTopLeft, void __fastcall, (wchar_t* text, int arg2))
D2EXPFUNC(D2CLIENT, PrintGameStringAtBottomLeft, void __fastcall, (wchar_t* text, int arg2))
D2EXPFUNC(D2CLIENT, PrintMessage, void __fastcall, (ChatMsg * Message));
D2EXPFUNC(D2CLIENT, GetPlayerUnit, UnitPlayer * __fastcall, (void));
D2EXPFUNC(D2CLIENT, GetUnit, UnitAny * __fastcall, (DWORD id, DWORD type));
D2EXPFUNC(D2CLIENT, QuitGame, void __stdcall, (void));			//quits the d2 game
D2EXPFUNC(D2CLIENT, ExitGame, void __stdcall, (DWORD flags));  //exits diablo2
D2EXPFUNC(D2CLIENT, GetGameType, int __fastcall, (void)); //returns 0 for classic, 1 for exp
D2EXPFUNC(D2CLIENT, GetNameFromUnit, wchar_t * __fastcall, (UnitAny *unit)); //0x6FB297F0
D2EXPFUNC(D2CLIENT, GetItemNameString, void __fastcall, (UnitItem * i, wchar_t * name, int length)); // d2bs
D2EXPFUNC(D2COMMON, ItemGetBinFromClass, ItemBinData * __stdcall, (int itemclsid)); 								// #10600


// variables:
//D2EXPVAR(D2CLIENT, nArea, int, 0x115EF0);
D2EXPVAR(D2CLIENT, nDiff, int);
D2EXPVAR(D2CLIENT, bWeapSwitch, int);
D2EXPVAR(D2CLIENT, PlayerUnit, UnitPlayer *);
D2EXPVAR(D2CLIENT, FirstRosterPet, RosterPet *);
D2EXPVAR(D2CLIENT, FirstRosterUnit, Roster *);

/*  D2Common.dll  */

// exports:
D2EXPFUNC(D2COMMON, GetCursorItem, UnitItem * __stdcall, (Inventory * ptInventory)); 								// #10262
D2EXPFUNC(D2COMMON, GetBeltItem, UnitItem * __stdcall, (Inventory * ptInventory, int beltspot/*0-F*/));	// #10271 - mattlant 
D2EXPFUNC(D2COMMON, GetFirstInventoryItem, UnitItem * __stdcall, (Inventory * ptInventory));							// #10277
D2EXPFUNC(D2COMMON, GetNextInventoryItem, UnitItem * __stdcall, (UnitItem * ptItem)); 								// #10304
D2EXPFUNC(D2COMMON, IsItem, UnitItem * __stdcall, (UnitAny * ptUnit)); 											// #10305
D2EXPFUNC(D2COMMON, GetItemId, DWORD __stdcall, (UnitItem * ptItem));  											// #10306
D2EXPFUNC(D2COMMON, GetItemNodePage, int __stdcall, (UnitItem * ptItem));											// #10307
D2EXPFUNC(D2COMMON, GetStashGoldLimit, int __stdcall, (UnitAny * ptUnit));											// #10339
D2EXPFUNC(D2COMMON, GetUnitState, BOOL __stdcall, (UnitAny *ptUnit, DWORD stateno))									// #10487
D2EXPFUNC(D2COMMON, GetUnitModifiedStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex));	 					// #10519
D2EXPFUNC(D2COMMON, GetUnitCleanBaseStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex)); 					// #10521
D2EXPFUNC(D2COMMON, GetItemCode, char * __stdcall, (int itemid)); 												// #10600
D2EXPFUNC(D2COMMON, GetItemQuality, int __stdcall, (UnitAny * ptUnit)); 							// #10628
D2EXPFUNC(D2COMMON, GetItemPage, int __stdcall, (UnitItem * ptItem));												// #10719
D2EXPFUNC(D2COMMON, TwoHandedItemCheck, bool __stdcall, (UnitItem * ptItem));										// #10765
D2EXPFUNC(D2COMMON, GetUnitPrice, int __stdcall, (UnitPlayer * pl, UnitItem * item, int Unk /*0*/, void * somestruct, int NPCID, int buysell)); //called from #10775
D2EXPFUNC(D2COMMON, GetMaxItemQuantity, int __stdcall, (UnitItem * ptItem));										// #10795
D2EXPFUNC(D2COMMON, GetMaxItemSockets, int __stdcall, (UnitItem * ptItem));											// #10815
D2EXPFUNC(D2COMMON, PrintOverhead, Msg * __stdcall, (DWORD Unk/*always 0*/ , char * Message,  DWORD dwMsgId));				// #10892
D2EXPFUNC(D2COMMON, GetParentUnit, UnitAny * __stdcall, (UnitAny *ptUnit, DWORD *unk)); // #10535
D2EXPFUNC(D2COMMON, GetItemTxt, ItemTxt * __stdcall, (DWORD itemno))
D2EXPFUNC(D2COMMON, GetUnitPtrStats3, UnitStats * __stdcall, (UnitAny *item, DWORD unk2, DWORD unk3));
D2EXPFUNC(D2COMMON, ListUnitStats, DWORD __stdcall, (UnitStats *ptrstats, ListStats *liststats, DWORD numstats));
D2EXPFUNC(D2COMMON, TestItemMeetReq, DWORD __stdcall, (UnitAny *item, UnitAny *pla, DWORD unk1, DWORD *opReqstr, DWORD *opReqdex, DWORD *opReqlev)) // from Mousepad(TM)!

extern Roster **p_D2CLIENT_FirstRosterUnit;
extern RosterPet **p_D2CLIENT_FirstRosterPet;
extern int *p_D2CLIENT_nDiff;
extern BnetData **p_D2LAUNCH_bnData;
extern int *p_D2CLIENT_bWeapSwitch;
extern void **p_D2CLIENT_ItemPriceGameInfoUnk;

#endif