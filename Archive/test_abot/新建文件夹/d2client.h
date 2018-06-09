#pragma once
//#include "funclib.h"

#define D2CLIENT_START D2CLIENT_UnloadUIImage
// functions:
// note, try to keep this in order of offset.  Thanks
// -Ninjai
D2FUNCPTR(D2CLIENT, UnloadUIImage, void __fastcall, (CellFile * UiImage), 0x1140);
D2FUNCPTR(D2CLIENT, MouseDownHandler, void __stdcall, (D2MSG * ptMsg), 0x8C10);
D2FUNCPTR(D2CLIENT, RepeatMouseUpHandler, void __stdcall, (D2MSG * ptMsg), 0x8CC0);
D2FUNCPTR(D2CLIENT, MouseUpHandler, void __stdcall, (D2MSG * ptMsg), 0x9100);
D2FUNCPTR(D2CLIENT, GetGameStatus, BOOL __fastcall, (void), 0x9440);
D2FUNCPTR(D2CLIENT, GetGameType, int __fastcall, (void), 0xC080); //returns 0 for classic, 1 for exp
D2FUNCPTR(D2CLIENT, QuitGame, void __stdcall, (void), 0xC2F0);			//quits the d2 game
//D2FUNCPTR(D2CLIENT, PrintMessage, void __fastcall, (ptMsg * Message), 0x13890);
D2FUNCPTR(D2CLIENT, GetMouseXOffset, int __fastcall, (void), 0x15890);
D2FUNCPTR(D2CLIENT, GetMouseYOffset, int __fastcall, (void), 0x158A0);
D2FUNCPTR(D2CLIENT, GetSelectedUnit, UnitAny * __fastcall, (void), 0x15A20);
D2FUNCPTR(D2CLIENT, SetSelectedUnit, void __fastcall, (UnitAny * ptSelectedUnit), 0x15AE0);
D2FUNCPTR(D2CLIENT, ClearSelectedUnit, void __fastcall, (void), 0x15D40);
//D2FUNCPTR(D2CLIENT, ScreenToMapCoords, void __fastcall, (int * ptX, int * ptY), 0x158B0);
D2FUNCPTR(D2CLIENT, ChatPressHandler, void __stdcall, (D2MSG * ptMsg), 0x32890);
D2FUNCPTR(D2CLIENT, ToggleChatBox, void __stdcall, (void), 0x337B0);
D2FUNCPTR(D2CLIENT, KeyPressHandler, void __stdcall, (D2MSG * ptMsg), 0x35A00);
D2FUNCPTR(D2CLIENT, WeaponSwitch, void __fastcall, (void), 0x4D2B0);
D2FUNCPTR(D2CLIENT, usePotion, void __fastcall, (UnitPlayer *player, Inventory *inv, DWORD MeOrMerc /*0 or 0x8000*/, DWORD slot), 0x618A0);
D2FUNCPTR(D2CLIENT, PrintChatMessage, void __fastcall, (wchar_t * Message, DWORD color), 0x7C600);
D2FUNCPTR(D2CLIENT, PrintMessage, void __fastcall, (ChatMsg * Message), 0x7E120);
D2FUNCPTR(D2CLIENT, LoadUIImage, CellFile * __fastcall, (char * uiname), 0x80100);
D2FUNCPTR(D2CLIENT, ManipUI, void __fastcall, (int uiNum, int uiState, DWORD _1), 0x83260);
D2FUNCPTR(D2CLIENT, GetUIState, int __fastcall, (int uiNum), 0x83230);
D2FUNCPTR(D2CLIENT, ClickWaypointUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x865C0);
D2FUNCPTR(D2CLIENT, ClickWaypointPageUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x867A0);
D2FUNCPTR(D2CLIENT, UnitTestSelect, bool __fastcall, (UnitAny * ptUnit, DWORD _1, DWORD _2, bool _3), 0x886F0);
D2FUNCPTR(D2CLIENT, ClickNpcMenuUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x35DC0);
D2FUNCPTR(D2CLIENT, ClickTradeUIdown, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x4A1C0);
D2FUNCPTR(D2CLIENT, ClickTradeUIup, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x4A850);
D2FUNCPTR(D2CLIENT, ClickStashUIdown, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x4BAD0);
D2FUNCPTR(D2CLIENT, ClickStashUIup, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x4C030);
D2FUNCPTR(D2CLIENT, SetSkill, void __fastcall, (DWORD Fkey, DWORD unk) , 0x7A130);
D2FUNCPTR(D2CLIENT, DrawClientScreen, void __fastcall, (void), 0x5F480);
D2FUNCPTR(D2CLIENT, GetUnit, UnitAny * __fastcall, (DWORD id, DWORD type), 0x869F0);
D2FUNCPTR(D2CLIENT, GetPlayerUnit, UnitPlayer * __fastcall, (void), 0x883D0);

D2FUNCPTR(D2CLIENT, GetNameFromUnit, wchar_t * __fastcall, (UnitAny *unit), 0x897F0); //0x6FB297F0
D2FUNCPTR(D2CLIENT, MouseClick, int __fastcall, (int ClickType, unsigned short MouseX, unsigned short MouseY, BYTE ClickFlag), 0xA9920);
D2FUNCPTR(D2CLIENT, Move, void __fastcall, (DWORD id, UnitPlayer *player, DWORD x, DWORD y, DWORD Unk /*always 0*/), 0xAEA90);
D2FUNCPTR(D2CLIENT, GetMouseX, unsigned short __fastcall, (void), 0xB7BC0);
D2FUNCPTR(D2CLIENT, GetMouseY, unsigned short __fastcall, (void), 0xB7BD0);
D2FUNCPTR(D2CLIENT, DrawRectangle, void __fastcall, (DWORD Xpos, DWORD YPos, DWORD Width, DWORD Height, DWORD Color, DWORD TransparencyLevel), 0xBB0F0);
D2FUNCPTR(D2CLIENT, ExitGame, void __stdcall, (DWORD flags), 0xC06B3);  //exits diablo2

//D2FUNCPTR(D2CLIENT, RecvCommand07, void __fastcall, (BYTE *cmdbuf), 0x6FAB1C80)
//D2FUNCPTR(D2CLIENT, RecvCommand08, void __fastcall, (BYTE *cmdbuf), 0x6FAB1CF0)
//D2FUNCPTR(D2CLIENT, RevealAutomapRoom, void __fastcall, (Room *room, DWORD clipflag, AutomapLayer *layer), 0x6FAC6E70)

// variables
//D2VARPTR(D2CLIENT, nArea, int, 0x115EF0);
D2VARPTR(D2CLIENT, nDiff, int, 0x10795C);
D2VARPTR(D2CLIENT, dwMsgId, DWORD, 0x119EF6);
D2VARPTR(D2CLIENT, bAlwaysRun, int, 0x1079B8);
D2VARPTR(D2CLIENT, slotsWithPotion, int, 0x115EBC);
D2VARPTR(D2CLIENT, bWeapSwitch, int, 0x11594C);
D2VARPTR(D2CLIENT, wLastChatMessage, wchar_t, 0x111A58);
D2VARPTR(D2CLIENT, MessageTimer, DWORD, 0x112D08);
D2VARPTR(D2CLIENT, ClickWpId, DWORD, 0x11A9A5);
D2VARPTR(D2CLIENT, NpcMenu, d2_NpcMenu *, 0x113644);
D2VARPTR(D2CLIENT, ClickTrade, DWORD, 0x115BA8);
D2VARPTR(D2CLIENT, d2MouseXa, DWORD, 0x121AE4);
D2VARPTR(D2CLIENT, d2MouseYa, DWORD, 0x121AE8);
D2VARPTR(D2CLIENT, d2MouseXb, DWORD, 0x11388C);
D2VARPTR(D2CLIENT, d2MouseYb, DWORD, 0x113894);
D2VARPTR(D2CLIENT, F13Skill, DWORD, 0x119D20);
D2VARPTR(D2CLIENT, F13SkillHand, DWORD, 0x119888);
D2VARPTR(D2CLIENT, HasHighlight, bool, 0x10B9E8);
D2VARPTR(D2CLIENT, HighlightUnitType, int, 0x10B9CC);
D2VARPTR(D2CLIENT, HighlightUnitId, int, 0x10B9D0);
D2VARPTR(D2CLIENT, ghRoom, Room *, 0x10B9F8);
D2VARPTR(D2CLIENT, ScreenYOffset, DWORD, 0x10B9B8);
D2VARPTR(D2CLIENT, ScreenXOffset, DWORD, 0x10B9BC);
D2VARPTR(D2CLIENT, ClientScreenWidth, int, 0xFA704);
D2VARPTR(D2CLIENT, ClientScreenHeight, int, 0xFA700);
/*D2VARPTR(D2CLIENT, FirstHandlerTable, D2HANDLER, 0xD5818);
#define NUM_FIRSTHANDLERS 10
D2VARPTR(D2CLIENT, SecondHandlerTable, D2HANDLER, 0xD87F0);
D2VARPTR(D2CLIENT, ThirdHandlerTable, D2HANDLER, 0x*/
// Was experimenting with this, but they don't seem to align too well.
// Also, there are too many of them, some with duplicate message ids.
// I'll have to look into it a little later.
D2VARPTR(D2CLIENT, IsPaused, bool, 0x11C23C);
D2VARPTR(D2CLIENT, FirstRosterPet, RosterPet *, 0x10B9A0);
D2VARPTR(D2CLIENT, FirstRosterUnit, Roster *, 0x10B994);
D2VARPTR(D2CLIENT, PlayerUnit, UnitPlayer *, 0x11C200);

// asm pointers:
D2ASMPTR(D2CLIENT, CallGameReceivePacketHandler, 0x15123);
D2ASMPTR(D2CLIENT, ScreenPrint, 0x81FF7);	// This is a call opcode, so patch at +1 with &MyFunc - (ScreenPrint+1+4);
									// Add 1 for opcode length, then 4 for opcode payload.  This will return
									// the right offset relative to the next instruction in line.
D2ASMPTR(D2CLIENT, CallGamePlayerInfoHandler, 0x883C4);
D2ASMPTR(D2CLIENT, CallStartGame, 0xD490);  
D2ASMPTR(D2CLIENT, CallMouseDownHandler, 0xD5820);   
D2ASMPTR(D2CLIENT, CallRepeatMouseUpHandler, 0xD582C);
D2ASMPTR(D2CLIENT, CallMouseUpHandler, 0xD588C);
D2ASMPTR(D2CLIENT, UIClickHandler, 0xD582C);
D2ASMPTR(D2CLIENT, CallKeyHandler, 0xD8C2C);
D2ASMPTR(D2CLIENT, CallChatHandler, 0xD82F8);

#define D2CLIENT_END D2CLIENT_CallChatHandler