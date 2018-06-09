

#include "funclib.h"

#define DllExport   __declspec(dllexport)
#define DllImport   __declspec( dllimport )




/*===================================Future funcs to add========================
Not yet Initialized kyp's slop to pick up =P
DllExport void GameSendPacketToServer(LPBYTE lpBuf, DWORD len);
DllExport void GameSendPacketToGame(LPBYTE lpBuf, DWORD len);
DllExport int GetActFromLevelId(int LevelId);
DllExport RoomOther * GetRoomOtherFromRoom(Room * ptRoom);				
DllExport int GetCellNum(int unk, TileData * ptTileData, int ActNum, Act * ptAct);		
DllExport UnitAny * GetRoomCoords(Room * ptRoom, Coords * ptCoords);				
DllExport void GetRoomsList(Room * ptRoom, Room *** RoomsList, int * RoomCount); 		
DllExport Room * GetFirstActRoom(Act * ptAct);
DllExport bool  IsInTown(Room * ptRoom);							
DllExport void GetTownLevelId(int Act); 							
DllExport void IsTownLevel(int LevelId);	
DllExport bool InventoryItem(Inventory * ptInventory, UnitItem * ptItem);
DllExport UnitItem * GetCursorItem(Inventory * ptInventory);
DllExport UnitItem * GetNextPlayerItem(UnitItem * ptItem); 
DllExport UnitItem * IsItem(UnitAny * ptUnit); 			
DllExport DWORD GetItemId(UnitItem * ptItem);  		
DllExport int GetItemNodePage(UnitItem * ptItem);	
DllExport Skill * GetStartSkill(UnitPlayer * ptPlayer);
DllExport Skill * GetLeftSkill(UnitPlayer * ptPlayer);	
DllExport Skill * GetRightSkill(UnitPlayer * ptPlayer);
DllExport Skill * GetCurrentSkill(UnitPlayer * ptPlayer);
DllExport int GetStashGoldLimit(UnitAny * ptUnit);	
DllExport bool IsUnitInRoom(Room * ptRoom, UnitAny * hUnit);		
DllExport bool IsUnitObjectDoor(UnitObject * ptUnit);				
DllExport ObjectBinData * GetObjectBinData(UnitObject * ptObject);				
DllExport ObjectBinData * GetPtData(UnitAny * ptUnit); 						
DllExport int GetBaseStat(UnitAny * ptUnit, int StatId, int StatTblIndex);	 		
DllExport int GetAddedStat(UnitAny * ptUnit, int StatId, int StatTblIndex); 		
DllExport int GetStatValue3(UnitAny * ptUnit, int StatId, int StatTblIndex); 			
DllExport int GetMaxHealth(UnitAny * ptUnit); 							
DllExport int GetMaxMana(UnitAny * ptUnit); 							
DllExport int GetMaxStamina(UnitAny * ptUnit); 						
DllExport char * GetItemCode(int itemid); 								
DllExport int GetExperienceForLevel(int PlayerClass, int TargetLevel); 
DllExport int GetItemPage(UnitItem * ptItem);							
DllExport bool TwoHandedItemCheck(UnitItem * ptItem);					
DllExport int GetNumberOfUnidItems(UnitPlayer * ptPlayer);				
DllExport int GetMaxItemQuantity(UnitItem * ptItem);					
DllExport int GetMaxItemSockets(UnitItem * ptItem);					
DllExport Msg * PrintOverhead(DWORD Unk , char * Message,  DWORD dwMsgId);		 
DllExport Skill * GetUnitSkill(UnitPlayer * ptPlayer, WORD SkillId, DWORD Flags);	
DllExport DWORD * GetSkillFlags(Skill * hSkill);				
DllExport short GetSkillType(Skill * ptSkill);									*/

DllExport bool GetGameStatus(void)
{
	return D2CLIENT_GetGameStatus();
}

DllExport void QuitGame(void)
{
	D2CLIENT_QuitGame();
}

DllExport void ExitGame(DWORD flags)
{
	D2CLIENT_ExitGame(flags);
}

DllExport LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen)
{
	MultiByteToWideChar(CP_ACP, 0, lpszAnsiString, -1, lpUnicodeString, maxlen);
	return lpUnicodeString;
}


DllExport LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen)
{
	WideCharToMultiByte(CP_ACP, 0, lpUnicodeString, -1, lpszAnsiString, maxlen, NULL, NULL);
	return lpszAnsiString;
}

DllExport void OnScreenPrint(LPSTR lpszText, LPSTR lpszName, DWORD dwColor, int Type, DWORD Id, BYTE Style)
{
	ChatMsg msg;
	msg.Identifier = 0x26;
	msg.Unk1 = 0x00;
	msg.Style = Style;
	if(Type < CHAT_NORMAL || Type > CHAT_SCROLL)
		Type = CHAT_NORMAL;
	msg.MessageType = Type;
	if(Id == 0)
	{
		UnitPlayer * player = D2CLIENT_GetPlayerUnit();
		Id = player->dwId;
	}
	msg.UnitId = Id;
	msg.Color = (BYTE)dwColor;
	if(strlen(lpszName) > 0x0F)
		lpszName[0x10] = 0;
	strcpy(msg.Name, lpszName);
	if(strlen(lpszText) > 0xFF)
		lpszName[0x100] = 0;
	strcpy(msg.Message, lpszText);
	D2CLIENT_PrintMessage(&msg);
}


DllExport void Move(UnitPlayer *player, DWORD x, DWORD y)
{
	DWORD id;
	if (*p_D2CLIENT_bAlwaysRun)
		id = 3;
	else id = 1;
	D2CLIENT_Move(id, player, x, y, 0);
}

DllExport void AddRoomData(Act * ptAct, int LevelId, int Xpos, int Ypos, UnitPlayer * ptPlayer)
{
	D2COMMON_AddRoomData(ptAct, LevelId, Xpos, Ypos, ptPlayer);
}

DllExport void RemoveRoomData(Act * ptAct, int LevelId, int Xpos, int Ypos, UnitPlayer * ptPlayer)
{
	D2COMMON_RemoveRoomData(ptAct, LevelId, Xpos, Ypos, ptPlayer);
}

DllExport int coreGetBaseStat(UnitAny *unit, int statId, int StatTblIndex)
{
	return D2COMMON_GetBaseStat(unit, statId, StatTblIndex);
}

DllExport int coreGetAddedStat(UnitAny *unit, int statId, int StatTblIndex)
{
	return D2COMMON_GetAddedStat(unit, statId, StatTblIndex);
}

DllExport int coreGetStatValue3(UnitAny *unit, int statId, int StatTblIndex)
{
	return D2COMMON_GetStatValue3(unit, statId, StatTblIndex);
}

DllExport int GetDifficulty(void) 
{
	return *p_D2CLIENT_nDiff+1;
}

DllExport int GetLevelIdFromRoom(Room * room) 
{
	return D2COMMON_GetLevelIdFromRoom(room);
}

DllExport BnetData GetBnetData() 
{
	BnetData b = **p_D2LAUNCH_bnData;
	return b;
}

DllExport int GetMaxDifficulty(void)
{
	BnetData BnData = **p_D2LAUNCH_bnData;
	if((BnData.MaxDifficulty & 0x0E)==0x0E){ return 3; }
	else if((BnData.MaxDifficulty & 0x0C)==0x0C){ return 2; }
	else if((BnData.MaxDifficulty & 0x08)==0x08){ return 1; }
	else { return 0; }
}

DllExport bool GetHardcoreFlag()
{
	BnetData BnData = **p_D2LAUNCH_bnData;
	if((BnData.CharFlags & 0x04)==0x04){ return true; }
	else { return false; }
}

DllExport wchar_t *GetNameFromUnit(UnitAny *unit)
{
	wchar_t *pszwBuf = D2CLIENT_GetNameFromUnit(unit);
	if (pszwBuf)
		return pszwBuf;
	else return L"undefined";
}

DllExport void ScreenToMapCoords(long * ptXCoord, long * ptYCoord)
{
	*ptXCoord += D2CLIENT_GetMouseXOffset();
	*ptYCoord += D2CLIENT_GetMouseYOffset();
	D2COMMON_AbsScreenToMapCoords(ptXCoord, ptYCoord);
}

DllExport void MapToScreenCoords(long * ptXCoord, long * ptYCoord)
{
	D2COMMON_MapToAbsScreenCoords(ptXCoord, ptYCoord);
	*ptXCoord -= D2CLIENT_GetMouseXOffset();
	*ptYCoord -= D2CLIENT_GetMouseYOffset();
}

DllExport void coreSetSkill(WORD nSkill, BYTE nHand)
{
	*p_D2CLIENT_F13Skill=(DWORD)nSkill;
	*p_D2CLIENT_F13SkillHand=(DWORD)nHand;
	
	D2CLIENT_SetSkill((DWORD)12,(DWORD)1);
}


DllExport int GetGameType(void) 
{
	return D2CLIENT_GetGameType();
}

DllExport bool GetWeaponSwitch(void) 
{
	return (bool)*p_D2CLIENT_bWeapSwitch;
}

DllExport bool GetAlwaysRun(void) 
{
	return (bool)*p_D2CLIENT_bAlwaysRun;
}

DllExport char * coreGetItemCode(int classid) 
{
	return D2COMMON_GetItemCode(classid)/*+3*/;
}

DllExport int coreGetItemQuality(UnitAny * unit)
{
    return D2COMMON_GetItemQuality(unit);
}

DllExport UnitAny * coreGetFirstRoomUnit(Room * room) 
{
	return D2COMMON_GetFirstRoomUnit(room);
}

DllExport Room * coreGetRoomFromUnit(UnitAny * unit) 
{
	return D2COMMON_GetRoomFromUnit(unit);
}

DllExport int coreGetUnitX(UnitAny * unit) 
{
	if(unit->dwType == 0 || unit->dwType == 1 || unit->dwType == 3)
		return D2COMMON_GetUnitX(unit->hPath);
	else
		return unit->hOPath->Xpos;
}

DllExport int coreGetUnitY(UnitAny * unit) 
{
	if(unit->dwType == 0 || unit->dwType == 1 || unit->dwType == 3)
		return D2COMMON_GetUnitY(unit->hPath);
	else
		return unit->hOPath->Ypos;
}

DllExport void coreHasHighlight (bool val)
{
		D2CLIENT_HasHighlight = val;
}

DllExport void coreHighlightUnitType (DWORD type)
{
		D2CLIENT_HighlightUnitType = type;
}

DllExport void coreHighlightUnitId (DWORD id)
{
		D2CLIENT_HighlightUnitId = id;
}
DllExport void coreMouseClick(int ClickType, int MouseX, int MouseY, BYTE ClickFlag)
{
		D2CLIENT_MouseClick(ClickType, (WORD)MouseX, (WORD)MouseY, ClickFlag);
}

DllExport int coreGetUIState(int flag)
{
	return D2CLIENT_GetUIState(flag);
}

DllExport bool coreGetUnitState(UnitAny * unit, int state)
{
	return D2COMMON_GetUnitState(unit, state);
}


DllExport void SetCredits(void)
{
}

DllExport int AddScreenHook()
{
	return 1;
}

DllExport void RemoveScreenHook(int id) 
{
	return;
}

DllExport bool SetScreenHook(int id, int x, int y, char * text, int color, int font, int align) 
{
	return true;
}


DllExport void UnitInteract(UnitAny * unit)
{
	D2CLIENT_Move(19, D2CLIENT_GetPlayerUnit(), unit->dwType, unit->dwId, 0);
}

DllExport UnitPlayer* GetPlayerUnit()
{
	return D2CLIENT_GetPlayerUnit();
}

DllExport bool coreHasPotion(int slot)
{
	return !!(((*p_D2CLIENT_slotsWithPotion) >> (slot*8)) & 0xFF);
}

DllExport void coreUsePotion(UnitPlayer *player, DWORD slot, DWORD MeOrMerc /*0 or 0x8000*/)
{
	D2CLIENT_usePotion(player, player->ptInventory, slot, MeOrMerc ? 0x8000 : 0);
}

DllExport Skill * coreGetRightSkill (UnitPlayer * player) 
{
	return D2COMMON_GetRightSkill(player);
}

DllExport Skill * coreGetLeftSkill (UnitPlayer * player) 
{
	return D2COMMON_GetLeftSkill(player);
}

DllExport Skill * coreGetUnitSkill(UnitPlayer * player, int skill)
{
	return D2COMMON_GetUnitSkill(player, skill, -1);
}

DllExport UnitAny * coreGetParent(UnitAny * unit)
{
	
	if (unit->dwType == 1) // npc's like our merc
	{
		UnitPlayer *player = D2CLIENT_GetPlayerUnit();
		RosterPet *pets = *p_D2CLIENT_FirstRosterPet;
		while(pets)
		{
			if(pets->Id == unit->dwId && pets->OwnerId == player->dwId) // unit owned by us :)
				return (UnitAny *)player;

			pets=pets->PetNext;
		} 
	}

	if (unit->dwType == 2) // objects like portals from townportal scrolls
	{
		return NULL;
		/*
		if (unit->ptObjectData->parent)
			return unit->ptObjectData->parent));
			*/
	}

	if (unit->dwType == 4) // items, need to be done when getUnit(4) gives us items that are not on the ground also !
		if(unit->ptItemData->ptOwnerInv)
			return (UnitAny *)unit->ptItemData->ptOwnerInv->ptOwner;

	return NULL;
}

//Clicks Cancel in trade/stash
DllExport void coreUICancel()
{
	int wait;
	do{
		SleepEx(100, true);
		if ( D2CLIENT_GetUIState(12) || D2CLIENT_GetUIState(25) ) wait = 100;
	} while (wait < 15);
	
	if ( D2CLIENT_GetUIState(12) )
	{
		HWND d2handle=D2GFX_GetWindowHandle();
		D2MSG MouseXY;
		*p_D2CLIENT_ClickTrade=(DWORD)1;				
		MouseXY.pt.X=434;
		MouseXY.pt.Y=460;
		*p_D2CLIENT_d2MouseXa=434;
		*p_D2CLIENT_d2MouseYa=460;
		*p_D2CLIENT_d2MouseXb=434;
		*p_D2CLIENT_d2MouseYb=460;
		
		D2CLIENT_ClickTradeUIdown((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
		SleepEx(200, true);
		
		*p_D2CLIENT_ClickTrade=(DWORD)1;				
		MouseXY.pt.X=434;
		MouseXY.pt.Y=460;
		*p_D2CLIENT_d2MouseXa=434;
		*p_D2CLIENT_d2MouseYa=460;
		*p_D2CLIENT_d2MouseXb=434;
		*p_D2CLIENT_d2MouseYb=460;
		
		D2CLIENT_ClickTradeUIup((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
	}
	if ( D2CLIENT_GetUIState(25) )
	{
		HWND d2handle=D2GFX_GetWindowHandle();
		D2MSG MouseXY;
		*p_D2CLIENT_ClickTrade=(DWORD)1;				
		MouseXY.pt.X=434;
		MouseXY.pt.Y=460;
		*p_D2CLIENT_d2MouseXa=434;
		*p_D2CLIENT_d2MouseYa=460;
		*p_D2CLIENT_d2MouseXb=434;
		*p_D2CLIENT_d2MouseYb=460;
		
		D2CLIENT_ClickStashUIdown((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
		SleepEx(200, true);
		
		*p_D2CLIENT_ClickTrade=(DWORD)1;				
		MouseXY.pt.X=434;
		MouseXY.pt.Y=460;
		*p_D2CLIENT_d2MouseXa=434;
		*p_D2CLIENT_d2MouseYa=460;
		*p_D2CLIENT_d2MouseXb=434;
		*p_D2CLIENT_d2MouseYb=460;
		
		D2CLIENT_ClickStashUIup((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
	}
}

// when in trade/repair window, clicks the repair all button
DllExport void coreUIRepair()
{
		int wait;
		do{
			SleepEx(100, true);
			if ( D2CLIENT_GetUIState(12)) wait = 100;
		} while (wait < 15);
		if ( D2CLIENT_GetUIState(12) )
		{
		HWND d2handle=D2GFX_GetWindowHandle();
		D2MSG MouseXY;
			*p_D2CLIENT_ClickTrade=(DWORD)1;				
			MouseXY.pt.X=370;
			MouseXY.pt.Y=460;
			*p_D2CLIENT_d2MouseXa=370;
			*p_D2CLIENT_d2MouseYa=460;
			*p_D2CLIENT_d2MouseXb=370;
			*p_D2CLIENT_d2MouseYb=460;
			
			D2CLIENT_ClickTradeUIdown((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
			SleepEx(200, true);
			
			*p_D2CLIENT_ClickTrade=(DWORD)1;				
			MouseXY.pt.X=370;
			MouseXY.pt.Y=460;
			*p_D2CLIENT_d2MouseXa=370;
			*p_D2CLIENT_d2MouseYa=460;
			*p_D2CLIENT_d2MouseXb=370;
			*p_D2CLIENT_d2MouseYb=460;
			
			D2CLIENT_ClickTradeUIup((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
		}
}

DllExport int coreClickMap(int ClickType, bool Shift, UnitAny * unit)
{
	BYTE ClickFlag = 0x8;
	long Xpos, Ypos;
	if(Shift)
		ClickFlag |= 0x4;

	Xpos = coreGetUnitX(unit);
	Ypos = coreGetUnitY(unit);

	if(D2CLIENT_UnitTestSelect(unit, 0, 0, FALSE))
	{
		if(D2CLIENT_GetSelectedUnit())
			D2CLIENT_ClearSelectedUnit();
		D2CLIENT_SetSelectedUnit(unit);
	}
	else
	{
		return 2;
	}

	MapToScreenCoords(&Xpos, &Ypos);
	if(Xpos < 0 || Ypos < 0 || Xpos > *p_D2CLIENT_ClientScreenWidth || Ypos > *p_D2CLIENT_ClientScreenHeight)
		return 1;
	D2CLIENT_MouseClick(ClickType, (WORD)Xpos, (WORD)Ypos, ClickFlag);

	return 0;
}

DllExport int coreClickMap(int ClickType, bool Shift, int x, int y)
{
	BYTE ClickFlag = 0x8;
	long Xpos, Ypos;
	if(Shift)
		ClickFlag |= 0x4;

	Xpos = x;
	Ypos = y;

	if(D2CLIENT_GetSelectedUnit())
		D2CLIENT_ClearSelectedUnit();

	MapToScreenCoords(&Xpos, &Ypos);
	if(Xpos < 0 || Ypos < 0 || Xpos > *p_D2CLIENT_ClientScreenWidth || Ypos > *p_D2CLIENT_ClientScreenHeight)
		return 1;
	D2CLIENT_MouseClick(ClickType, (WORD)Xpos, (WORD)Ypos, ClickFlag);

	return 0;
}

int GetWpIdbyOldWpId(int oldId)
{
	for(int i = 0; i < 50; i++)
		if(WpId_Tbl[i].oldId == oldId)
			return i;
	return 0;
}

// unit.interact(); compatible to d2jsp at taking Waypoints (spread)
// npc.interact( menuentry ); not compatible with d2jsp npc.interact(2); clicks second menu entry
DllExport void coreUnitInteract(UnitAny * unit, int a, int b)
{
	D2CLIENT_Move(19, D2CLIENT_GetPlayerUnit(), unit->dwType, unit->dwId, 0);

	// interact with waypoints
	if (unit->dwType == 2 && a)
	{
		int wait;
		do{
			SleepEx(100, true);
			if ( D2CLIENT_GetUIState(20)) wait = 100;
			D2CLIENT_Move(19, D2CLIENT_GetPlayerUnit(), unit->dwType, unit->dwId, 0);
		} while (wait < 25);

		HWND d2handle=D2GFX_GetWindowHandle();
		D2MSG MouseXY;
		int	tempId=GetWpIdbyOldWpId(a);
		int actid=WpId_Tbl[tempId].Act;
		int buttonid=WpId_Tbl[tempId].Wp;
		if (b)
		{
			actid=a;
			buttonid=b;
		}
		if (actid > 0 && actid < 6 && buttonid > 0 && buttonid < 10 && D2CLIENT_GetUIState(20) )
		{
			MouseXY.pt.X=49+actid*62;
			MouseXY.pt.Y=76;

			SleepEx(800, true);
			D2CLIENT_ClickWaypointPageUI((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);

			MouseXY.pt.X=111;
			MouseXY.pt.Y=94+buttonid*36;
			*p_D2CLIENT_ClickWpId=buttonid-1;

			SleepEx(800, true);
			D2CLIENT_ClickWaypointUI((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
		}
	}
	if (unit->dwType == 1 && a)
	{
		int wait;
		do{
			SleepEx(100, true);
			if ( D2CLIENT_GetUIState(8)) wait = 100;
			D2CLIENT_Move(19, D2CLIENT_GetPlayerUnit(), unit->dwType, unit->dwId, 0);
		} while (wait < 25);
		if ( *p_D2CLIENT_NpcMenu != 0x00000000 && D2CLIENT_GetUIState(8) )
		{
		SleepEx(200, true);
		HWND d2handle=D2GFX_GetWindowHandle();
		D2MSG MouseXY;
		d2_NpcMenu *NpcMenu;

		NpcMenu=&(**p_D2CLIENT_NpcMenu);

		MouseXY.pt.X=(WORD)NpcMenu->X +32;
		MouseXY.pt.Y=(WORD)NpcMenu->Y +15 +15*(WORD)a;
		NpcMenu->clicked=1;
		NpcMenu->MenuId1=a;
		NpcMenu->MenuId2=a;

		D2CLIENT_ClickNpcMenuUI((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
		}
	}
}

DllExport bool coreClickWPPage(int page)
{
		HWND d2handle=D2GFX_GetWindowHandle();
		D2MSG MouseXY;
		if (page > 0 && page < 6&& D2CLIENT_GetUIState(20) )
		{
			MouseXY.pt.X=49+page*62;
			MouseXY.pt.Y=76;
			D2CLIENT_ClickWaypointPageUI((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
			return true;
		}
		return false;
}

DllExport bool coreClickWPButton(int button)
{
		HWND d2handle=D2GFX_GetWindowHandle();
		D2MSG MouseXY;
		if (button > 0 && button < 10 && D2CLIENT_GetUIState(20) )
		{
			MouseXY.pt.X=111;
			MouseXY.pt.Y=94+button*36;
			*p_D2CLIENT_ClickWpId=button-1;
			D2CLIENT_ClickWaypointUI((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
			return true;
		}
		return false;
}

DllExport bool coreClickUIScreen(int MenuItem)
{
	if (*p_D2CLIENT_NpcMenu != 0x00000000 && D2CLIENT_GetUIState(8))
	{
		HWND d2handle=D2GFX_GetWindowHandle();
		D2MSG MouseXY;
		d2_NpcMenu *NpcMenu;

		NpcMenu=&(**p_D2CLIENT_NpcMenu);

		MouseXY.pt.X=(WORD)NpcMenu->X +32;
		MouseXY.pt.Y=(WORD)NpcMenu->Y +15 +15*(WORD)MenuItem;
		NpcMenu->clicked=1;
		NpcMenu->MenuId1=MenuItem;
		NpcMenu->MenuId2=MenuItem;

		D2CLIENT_ClickNpcMenuUI((DWORD)d2handle,(DWORD)0x6FFEA1C4,(DWORD)&MouseXY);
		return true;
	}
	return false;
}


// My say command
DllExport void corePrintToBnet(char * message)
{
	wchar_t * lastmessage = p_D2CLIENT_wLastChatMessage;
	HWND d2handle = D2GFX_GetWindowHandle();
	if(*p_D2CLIENT_MessageTimer <= 0)
	{
		D2CLIENT_ManipUI(5, UI_ON, 0);
		*p_D2CLIENT_MessageTimer = 1;
	}
	ConvertToUnicode(message, lastmessage, strlen(message)+1);
	SendMessage(d2handle, WM_CHAR, 0x0D, 0x1C0001);
	D2CLIENT_ManipUI(5, UI_OFF, 0);
	*p_D2CLIENT_MessageTimer = 0;
}

DllExport UnitAny * coreGetSelectedUnit(void)
{
	return D2CLIENT_GetSelectedUnit();
}



///////////////////////
//Screen Hook stuff
///////////////////////
DllExport void corePrintImage(::ImageStruct img, int x, int y, int layer)
{
	D2GFX_PrintImage(&img, x, y, layer, 5, 0);
}

DllExport void coreDrawRectangle(int x, int y, int x2, int y2, int color, int transparency)
{
	D2GFX_DrawRectangle(x, y, x2, y2, color, transparency);
}

DllExport DWORD coreDrawTextFont(int font)
{
	return D2WIN_DrawTextFont(font);
}

DllExport BYTE coreGetTextWidth(wchar_t * text)
{
    return D2WIN_GetTextWidth(text);
}

DllExport int coreGetFontHeight()
{
    return D2WIN_GetFontHeight();
}

DllExport WORD coreGetMouseX()
{
	return D2CLIENT_GetMouseX();
}
DllExport WORD coreGetMouseY()
{
	return D2CLIENT_GetMouseY();
}

DllExport UnitAny * coreGetFirstInventoryItem(Inventory * unitownerinventory)
{
	return D2COMMON_GetFirstInventoryItem(unitownerinventory);
}

DllExport UnitAny * coreGetNextInventoryItem(UnitItem * item)
{
	return D2COMMON_GetNextInventoryItem(item);
}

DllExport int coreGetLevelIdFromRoom(Room * room)
{
	return D2COMMON_GetLevelIdFromRoom(room);
}

DllExport void coreSendKey(DWORD keycode, DWORD keycode2)
{

	D2MSG ptMsg;
	ptMsg.message = 0x100;
	ptMsg._1 = 0;
	ptMsg._2[0] = 0;
	ptMsg._2[1] = 0;
	ptMsg.hwnd = D2GFX_GetWindowHandle();
	ptMsg.hwnd2 = keycode2;
	ptMsg.lParam = keycode2;
	ptMsg.wParam = keycode;
	D2CLIENT_KeyPressHandler(&ptMsg);
}

DllExport HWND coreGetWindowHandle()
{
	return D2GFX_GetWindowHandle();
}

DllExport DWORD coreGetScreenXOffset()
{
	return *p_D2CLIENT_ScreenXOffset;
}

DllExport DWORD coreGetScreenYOffset()
{
	return *p_D2CLIENT_ScreenYOffset;
}

DllExport DWORD coreGetClientScreenWidth()
{
	return *p_D2CLIENT_ClientScreenWidth;
}

DllExport DWORD coreGetClientScreenHeight()
{
	return *p_D2CLIENT_ClientScreenHeight;
}

DllExport bool coreIsPaused()
{
	return *p_D2CLIENT_IsPaused;
}

DllExport RosterPet * coreGetFirstRosterPet()
{
	return *p_D2CLIENT_FirstRosterPet;
}


//DllExport Roster * coreGetFirstRosterUnit()
//{
//	return *p_D2CLIENT_FirstRosterUnit;
//}


DllExport UnitPlayer * coreGetPlayerUnit()
{
	return *p_D2CLIENT_PlayerUnit;
}

DllExport UnitItem * coreGetCursorItem(Inventory * ptInventory)
{
	return D2COMMON_GetCursorItem(ptInventory);
}













