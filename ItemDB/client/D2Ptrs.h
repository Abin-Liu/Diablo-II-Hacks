#ifndef __D2PTRS_H__
#define __D2PTRS_H__

#include <windows.h>
#include "definitions.h"

///////////////////////////////////////////////
// Originally Mousepad's & D2BS's Code
///////////////////////////////////////////////
enum {	DLLNO_D2CLIENT,
		DLLNO_D2COMMON,
		DLLNO_D2GAME,
		DLLNO_D2LAUNCH,
		DLLNO_BNCLIENT };

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|((b1)<<8))

#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2ASMPTR(d1,v1,o1)        DWORD d1##_##v1 = DLLOFFSET(d1,o1);


#define D2CLIENT_START D2CLIENT_GetItemNameString
// functions:
D2FUNCPTR(D2CLIENT, GetItemNameString, void __fastcall, (UnitItem * i, wchar_t * name, int length), 0x3D360); // d2bs
D2FUNCPTR(D2CLIENT, PrintGameStringAtTopLeft, void __fastcall, (wchar_t* text, int arg2), 0x7C600)
D2FUNCPTR(D2CLIENT, PrintGameStringAtBottomLeft, void __fastcall, (wchar_t* text, int arg2), 0x7C950)
D2FUNCPTR(D2CLIENT, PrintMessage, void __fastcall, (ChatMsg * Message), 0x7E120);
D2FUNCPTR(D2CLIENT, GetUnit, UnitAny * __fastcall, (DWORD id, DWORD type), 0x869F0);
D2FUNCPTR(D2CLIENT, GetPlayerUnit, UnitPlayer * __fastcall, (void), 0x883D0);
D2FUNCPTR(D2CLIENT, GetNameFromUnit, wchar_t * __fastcall, (UnitAny *unit), 0x897F0); //0x6FB297F0

// variables
//D2VARPTR(D2CLIENT, nArea, int, 0x115EF0);
D2VARPTR(D2CLIENT, nDiff, int, 0x10795C);
D2VARPTR(D2CLIENT, dwMsgId, DWORD, 0x119EF6);
D2VARPTR(D2CLIENT, slotsWithPotion, int, 0x115EBC);
D2VARPTR(D2CLIENT, bWeapSwitch, int, 0x11594C);
D2VARPTR(D2CLIENT, FirstRosterPet, RosterPet *, 0x10B9A0);
D2VARPTR(D2CLIENT, FirstRosterUnit, Roster *, 0x10B994);
D2VARPTR(D2CLIENT, PlayerUnit, UnitPlayer *, 0x11C200);

// asm pointers:
D2ASMPTR(D2CLIENT, CallGamePlayerInfoHandler, 0x883C4);

#define D2CLIENT_END D2CLIENT_CallGamePlayerInfoHandler

#define D2COMMON_START D2COMMON_GetCursorItem

D2FUNCPTR(D2COMMON, GetCursorItem, UnitItem * __stdcall, (Inventory * ptInventory), -0x2816); 					// #10262
D2FUNCPTR(D2COMMON, GetBeltItem, UnitItem * __stdcall, (Inventory * ptInventory, int beltspot/*0-F*/), -0x281F);	// #10271 - mattlant 
D2FUNCPTR(D2COMMON, GetFirstInventoryItem, UnitItem * __stdcall, (Inventory * ptInventory), -0x2825);				// #10277
D2FUNCPTR(D2COMMON, GetNextInventoryItem, UnitItem * __stdcall, (UnitItem * ptItem), -0x2840); 					// #10304
D2FUNCPTR(D2COMMON, IsItem, UnitItem * __stdcall, (UnitAny * ptUnit), -0x2841); 						// #10305
D2FUNCPTR(D2COMMON, GetItemId, DWORD __stdcall, (UnitItem * ptItem), -0x2842);  						// #10306
D2FUNCPTR(D2COMMON, GetItemNodePage, int __stdcall, (UnitItem * ptItem), -0x2843);						// #10307
D2FUNCPTR(D2COMMON, GetStashGoldLimit, int __stdcall, (UnitAny * ptUnit), -0x2863);						// #10339
D2FUNCPTR(D2COMMON, ItemGetBinFromClass, ItemBinData * __stdcall, (int itemclsid), -0x2968); 								// #10600

// From mousepad
D2FUNCPTR(D2COMMON, GetUnitPtrStats3, UnitStats * __stdcall, (UnitAny *item, DWORD unk2, DWORD unk3), -0x28f4);
D2FUNCPTR(D2COMMON, ListUnitStats, DWORD __stdcall, (UnitStats *ptrstats, ListStats *liststats, DWORD numstats), -0x2beb);

D2FUNCPTR(D2COMMON, GetUnitState, BOOL __stdcall, (UnitAny *ptUnit, DWORD stateno), -0x28F7)					// #10487
D2FUNCPTR(D2COMMON, GetUnitModifiedStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2917);	 		// #10519
D2FUNCPTR(D2COMMON, GetUnitCleanBaseStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2919); 			// #10521
D2FUNCPTR(D2COMMON, GetParentUnit, UnitAny * __stdcall, (UnitAny *ptUnit, DWORD *unk), -0x2927); // #10535
D2FUNCPTR(D2COMMON, GetItemCode, char * __stdcall, (int itemid), -0x2968); 								// #10600

D2FUNCPTR(D2COMMON, GetItemTxt, ItemTxt * __stdcall, (DWORD itemno), -0x2968)
D2FUNCPTR(D2COMMON, TestItemMeetReq, DWORD __stdcall, (UnitAny *item, UnitAny *pla, DWORD unk1, DWORD *opReqstr, DWORD *opReqdex, DWORD *opReqlev), -0x2A04) // from Mousepad(TM)!
D2FUNCPTR(D2COMMON, GetItemQuality, int __stdcall, (UnitAny * ptUnit), -0x29C7);						// #10695
D2FUNCPTR(D2COMMON, GetItemPage, int __stdcall, (UnitItem * ptItem), -0x29DF);							// #10719
D2FUNCPTR(D2COMMON, TwoHandedItemCheck, bool __stdcall, (UnitItem * ptItem), -0x2A0D);						// #10765
D2FUNCPTR(D2COMMON, GetUnitPrice, int __stdcall, (UnitPlayer * pl, UnitItem * item, int Unk /*0*/, void * somestruct, int NPCID, int buysell), 0x5CDE0); //called from #10775
D2FUNCPTR(D2COMMON, GetNumberOfUnidItems, int __stdcall, (UnitPlayer * ptPlayer), -0x2A18);					// #10776
D2FUNCPTR(D2COMMON, GetMaxItemQuantity, int __stdcall, (UnitItem * ptItem), -0x2A2B);						// #10795
D2FUNCPTR(D2COMMON, GetMaxItemSockets, int __stdcall, (UnitItem * ptItem), -0x2A3F);						// #10815
D2FUNCPTR(D2COMMON, PrintOverhead, Msg * __stdcall, (DWORD Unk/*always 0*/ , char * Message,  DWORD dwMsgId), -0x2A8C);		// #10892

#define D2COMMON_END D2COMMON_PrintOverhead

#define D2OTHER_START D2GAME_IsSwitched
// functions
D2FUNCPTR(D2GAME, IsSwitched, BYTE __fastcall ,(PlrMsg *ptPlrMsg), 0x45B0);
D2VARPTR(D2CLIENT, ItemPriceGameInfoUnk, void *, 0x115D13); // d2bs
D2VARPTR(D2LAUNCH, bnData, BnetData*, 0x2CD44);

#define D2OTHER_END p_D2LAUNCH_bnData

#endif // __D2PTRS_H__