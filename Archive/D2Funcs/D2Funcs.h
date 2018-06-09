#ifndef __D2FUNCS_H__
#define __D2FUNCS_H__

#include <windows.h>
#include "D2Structs"

// D2Client.dll
void D2CLIENT_RevealAutomapRoom(DrlgRoom1 *room1, DWORD clipflag, AutomapLayer *layer);
MonsterTxt* D2CLIENT_GetMonsterTxt(DWORD monno);
UnitAny* D2CLIENT_GetPlayerUnit(); 
DWORD D2CLIENT_TestPvpFlag(DWORD planum1, DWORD planum2, DWORD flagmask);
DWORD D2CLIENT_GetMonsterOwner(DWORD monnum);
UnitAny * D2CLIENT_GetUnitFromId(DWORD unitid, DWORD unittype);
UnitAny * D2CLIENT_GetNextPlayer(UnitAny* pla);
DWORD D2CLIENT_GetInventoryId(UnitAny* pla, DWORD unitno, DWORD arg3);
DWORD D2CLIENT_GetUnitNoFromId(DWORD unitid);
int D2CLIENT_GetPlayerXOffest();
int D2CLIENT_GetPlayerYOffest();
void D2CLIENT_SetExitApp(DWORD flag);
void D2CLIENT_PrintGameStringAtTopLeft(wchar_t* text, int arg2);
void D2CLIENT_PrintGameStringAtBottomLeft(wchar_t* text, int arg2);
UnitAny* D2CLIENT_GetSelectedUnit();
BYTE D2CLIENT_GetDifficulty();
void* D2CLIENT_GetQuestInfo();

// D2Common.dll
void D2COMMON_InitDrlgLevel(DrlgLevel* drlglevel);
DrlgLevel* D2COMMON_GetDrlgLevel(DrlgMisc* drlgmisc, DWORD levelno);
ObjectTxt* D2COMMON_GetObjectTxt(DWORD objno);
ItemTxt* D2COMMON_GetItemTxt(DWORD itemno);
LevelTxt* D2COMMON_GetLevelTxt(DWORD levelno);
SuperuniqueTxt* D2COMMON_GetSuperuniqueTxt(DWORD monno);
TreasureClass*  D2COMMON_GetTreasureClasses(DWORD tcno, DWORD lvlno);
int D2COMMON_GetUnitStat(UnitAny *unit, DWORD statno, DWORD unk);
int D2COMMON_GetUnitState(UnitAny *unit, DWORD stateno);
DWORD D2COMMON_GetItemFlag(UnitAny *item, DWORD flagmask, DWORD lineno, char *filename);
int D2COMMON_GetQuestFlag(void *questinfo, DWORD quest, DWORD flag);
DWORD D2COMMON_GetItemFromInventory(UnitInventory* inv);
DWORD D2COMMON_GetNextItemFromInventory(DWORD no);
UnitAny* D2COMMON_GetUnitFromItem(DWORD arg1);
UnitAny* D2COMMON_GetCursorItem(UnitInventory * ptInventory);
int D2COMMON_GetItemValue(UnitAny * player, UnitAny * item, DWORD difficulty, void* questinfo, int value, DWORD flag);


// D2Gfx.dll
void D2GFX_DrawAutomapCell(CellContext *context, DWORD xpos, DWORD ypos, RECT *cliprect, DWORD bright);
void D2GFX_DrawAutomapCell2(CellContext *context, DWORD xpos, DWORD ypos, DWORD bright2, DWORD bright, BYTE *coltab);
void D2GFX_DrawLine(int x1, int y1, int x2, int y2, DWORD col, DWORD unknown);
DWORD D2GFX_GetScreenSize();
HWND D2GFX_GetHwnd();

// D2Cmp.dll
void D2CMP_InitCellFile(void *cellfile, CellFile **outptr, char *srcfile, DWORD lineno, DWORD filever, char *filename);
void D2CMP_DeleteCellFile(CellFile *cellfile);

// D2Win.dll
DWORD D2WIN_SetTextSize(DWORD size);
DWORD D2WIN_GetTextWidthFileNo(wchar_t *str, DWORD* width, DWORD* fileno);
void D2WIN_DrawText(wchar_t *str, int xpos, int ypos, DWORD col, DWORD unknown);
D2EditBox* D2WIN_CreateEditBox(DWORD style, int ypos, int xpos, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9, DWORD size, void* buf);
DWORD D2WIN_DestroyEditBox(D2EditBox* box);
wchar_t* D2WIN_GetEditBoxText(D2EditBox* box);
DWORD D2WIN_AddEditBoxChar(D2EditBox* box, BYTE keycode);
void D2WIN_SetEditBoxText(D2EditBox* box, wchar_t* txt);
void D2WIN_SetEditBoxProc(D2EditBox* box, BOOL (__stdcall *FunCallBack)(D2EditBox*,DWORD,DWORD));
void D2WIN_SelectEditBoxText();

// D2Lang.dll
wchar_t* D2LANG_GetLocaleText(WORD nLocaleTxtNo);

#endif // __D2FUNCS_H__