#pragma once

#define D2COMMON_START D2COMMON_GetActFromLevelId

D2FUNCPTR(D2COMMON, GetActFromLevelId, int __stdcall, (int LevelId), -0x2711);							// #10001
D2FUNCPTR(D2COMMON, GetRoomOtherFromRoom, RoomOther * __fastcall, (Room * ptRoom), -0x2717);					// #10007
D2FUNCPTR(D2COMMON, GetCellNum, int __fastcall, (int unk, TileData * ptTileData, int ActNum, Act * ptAct), -0x271B);		// #10011
D2FUNCPTR(D2COMMON, GetFirstRoomUnit, UnitAny * __stdcall, (Room * ptRoom), -0x272D);						// #10029
D2FUNCPTR(D2COMMON, GetRoomCoords, void __stdcall, (Room * ptRoom, Coords * ptCoords), -0x2732);				// #10034
D2FUNCPTR(D2COMMON, GetRoomsList, void * __fastcall, (Room * ptRoom, Room *** RoomsList, int * RoomCount), -0x2733); 		// #10035
D2FUNCPTR(D2COMMON, GetFirstActRoom, Room * __stdcall, (Act * ptAct), -0x2748);							// #10056 
D2FUNCPTR(D2COMMON, GetLevelIdFromRoom, int __stdcall, (Room * hRoom), -0x2749);						// #10057
D2FUNCPTR(D2COMMON, AddRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, UnitPlayer * ptPlayer), -0x274F);	// #10063
D2FUNCPTR(D2COMMON, RemoveRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, UnitPlayer * ptPlayer), -0x2750);	// #10064
D2FUNCPTR(D2COMMON, IsInTown, bool __stdcall, (Room * ptRoom), -0x2762);							// #10082
D2FUNCPTR(D2COMMON, GetTownLevelId, int __stdcall, (int Act), -0x2765); 							// #10085
D2FUNCPTR(D2COMMON, IsTownLevel, bool __stdcall, (int LevelId), -0x2766);							// #10086
D2FUNCPTR(D2COMMON, GetMiscFromAct, Drlg * __stdcall, (Act * ptAct), -0x2768);							// #10088
D2FUNCPTR(D2COMMON, AbsScreenToMapCoords, void __stdcall, (long * ptAbsMouseX, long * ptAbsMouseY), -0x277C);			// #10108
D2FUNCPTR(D2COMMON, MapToAutomapCoords, void __stdcall, (long * ptXpos, long * ptYPos), -0x277E);				// #10110
D2FUNCPTR(D2COMMON, MapToAbsScreenCoords, void __stdcall, (long * ptXpos, long * ptYpos), -0x277F);				// #10111
D2FUNCPTR(D2COMMON, GetUnitX, int __stdcall, (Path * hPath), -0x27B2);								// #10162
D2FUNCPTR(D2COMMON, GetUnitY, int __stdcall, (Path * hPath), -0x27B3);								// #10163
D2FUNCPTR(D2COMMON, GetPathTargetY, DWORD __stdcall, (Path * hPath), -0x27B5);							// #10165
D2FUNCPTR(D2COMMON, GetRoomFromPath, Room * __stdcall, (Path * hPath), -0x27B6);     						// #10166
D2FUNCPTR(D2COMMON, GetTargetFromPath, Path * __stdcall, (Path * hPath, int * TargetType, DWORD * UnitId), -0x27BB);		// #10171
D2FUNCPTR(D2COMMON, GetTargetXFromPath, int __stdcall, (Path * hPath), -0x27BF);						// #10175
D2FUNCPTR(D2COMMON, GetTargetYFromPath, int __stdcall, (Path * hPath), -0x27C0);						// #10176
D2FUNCPTR(D2COMMON, GetTargetIdFromPath, int __stdcall, (Path * hPath), -0x27C4);						// #10180
D2FUNCPTR(D2COMMON, InventoryItem, bool __stdcall, (Inventory * ptInventory, UnitItem * ptItem), -0x2803);	 		// #10243
D2FUNCPTR(D2COMMON, GetCursorItem, UnitItem * __stdcall, (Inventory * ptInventory), -0x2816); 					// #10262
D2FUNCPTR(D2COMMON, GetFirstInventoryItem, UnitItem * __stdcall, (Inventory * ptInventory), -0x2825);				// #10277
D2FUNCPTR(D2COMMON, GetNextInventoryItem, UnitItem * __stdcall, (UnitItem * ptItem), -0x2840); 					// #10304
D2FUNCPTR(D2COMMON, IsItem, UnitItem * __stdcall, (UnitAny * ptUnit), -0x2841); 						// #10305
D2FUNCPTR(D2COMMON, GetItemId, DWORD __stdcall, (UnitItem * ptItem), -0x2842);  						// #10306
D2FUNCPTR(D2COMMON, GetItemNodePage, int __stdcall, (UnitItem * ptItem), -0x2843);						// #10307
D2FUNCPTR(D2COMMON, GetStartSkill, Skill * __stdcall, (UnitPlayer * ptPlayer), -0x2850);					// #10320
D2FUNCPTR(D2COMMON, GetLeftSkill, Skill * __stdcall, (UnitPlayer * ptPlayer), -0x2851);						// #10321
D2FUNCPTR(D2COMMON, GetRightSkill, Skill * __stdcall, (UnitPlayer * ptPlayer), -0x2852);					// #10322
D2FUNCPTR(D2COMMON, GetCurrentSkill, Skill * __stdcall, (UnitPlayer * ptPlayer), -0x2853);					// #10323
D2FUNCPTR(D2COMMON, GetStashGoldLimit, int __stdcall, (UnitAny * ptUnit), -0x2863);						// #10339
D2FUNCPTR(D2COMMON, GetRoomFromUnit, Room * __stdcall, (UnitAny * ptUnit), -0x2866); 		       				// #10342
D2FUNCPTR(D2COMMON, IsUnitInRoom, bool __stdcall, (Room * ptRoom, UnitAny * hUnit), -0x2895);					// #10389
D2FUNCPTR(D2COMMON, IsUnitObjectDoor, bool __stdcall, (UnitObject * ptUnit), -0x2898);						// #10392
D2FUNCPTR(D2COMMON, GetObjectBinData, ObjectBinData * __stdcall, (UnitObject * ptObject), -0x289A);				// #10394
D2FUNCPTR(D2COMMON, GetPtData, GlobalData * __stdcall, (UnitAny * ptUnit), -0x28B8); 						// #10424
D2FUNCPTR(D2COMMON, GetUnitState, BOOL __stdcall, (UnitAny *ptUnit, DWORD stateno), -0x28F7)					// #10487
D2FUNCPTR(D2COMMON, GetBaseStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2917);	 		// #10519
D2FUNCPTR(D2COMMON, GetAddedStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2918); 			// #10520
D2FUNCPTR(D2COMMON, GetStatValue3, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2919); 			// #10521
D2FUNCPTR(D2COMMON, GetMaxHealth, int __stdcall, (UnitAny * ptUnit), -0x2944); 							// #10564
D2FUNCPTR(D2COMMON, GetMaxMana, int __stdcall, (UnitAny * ptUnit), -0x2945); 							// #10565
D2FUNCPTR(D2COMMON, GetMaxStamina, int __stdcall, (UnitAny * ptUnit), -0x2946); 						// #10566
D2FUNCPTR(D2COMMON, GetItemCode, char * __stdcall, (int itemid), -0x2968); 								// #10600
D2FUNCPTR(D2COMMON, GetExperienceForLevel, int __stdcall, (int PlayerClass, int TargetLevel), -0x2984); 			// #10628
D2FUNCPTR(D2COMMON, GetItemQuality, int __stdcall, (UnitAny * ptUnit), -0x29C7);						// #10695
D2FUNCPTR(D2COMMON, GetItemPage, int __stdcall, (UnitItem * ptItem), -0x29DF);							// #10719
D2FUNCPTR(D2COMMON, TwoHandedItemCheck, bool __stdcall, (UnitItem * ptItem), -0x2A0D);						// #10765
D2FUNCPTR(D2COMMON, GetNumberOfUnidItems, int __stdcall, (UnitPlayer * ptPlayer), -0x2A18);					// #10776
D2FUNCPTR(D2COMMON, GetMaxItemQuantity, int __stdcall, (UnitItem * ptItem), -0x2A2B);						// #10795
D2FUNCPTR(D2COMMON, GetMaxItemSockets, int __stdcall, (UnitItem * ptItem), -0x2A3F);						// #10815
D2FUNCPTR(D2COMMON, PrintOverhead, Msg * __stdcall, (DWORD Unk/*always 0*/ , char * Message,  DWORD dwMsgId), -0x2A8C);		// #10892
D2FUNCPTR(D2COMMON, GetUnitSkill, Skill * __fastcall, (UnitPlayer * ptPlayer, WORD SkillId, DWORD Flags), -0x2AC5);		// #10949
D2FUNCPTR(D2COMMON, GetSkillFlags, DWORD * __stdcall, (Skill * hSkill), -0x2ACD);						// #10957
D2FUNCPTR(D2COMMON, GetSkillType, short __stdcall, (Skill * ptSkill), -0x2AD3);							// #10963

#define D2COMMON_END D2COMMON_GetSkillType
