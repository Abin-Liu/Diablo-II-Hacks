#pragma once
#include <windows.h>
#include "definitions.h"

//#include "d2client.h"
//#include "d2common.h"
#define DllExport   __declspec(dllexport)
#define DllImport   __declspec( dllimport )

#define assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )
#define msg_assert(exp,msg) (void)( (exp) || (_assert(msg, __FILE__, __LINE__), 0) )




//printing functions
//void Print(LPSTR lpszText, LPSTR lpszName, DWORD dwColor = 0, int Type = CHAT_NORMAL, DWORD Id = 0, BYTE Style = 0x02);
//void Message(char * message, DWORD dwColor = 0);
//void Overhead(char * message, DWORD UId = -1);
//void WinDrawText(char * pszText, int x, int y, int nColor, int nFont = -1, int align = ALIGN_LEFT);
void DrawText(LPSTR lpszText, int x, int y, int nColor, int font);
void DrawTextFont(int nFont);

//in game functions
void usePotion(UnitPlayer *player, DWORD slot, DWORD MeOrMerc /*0 or 0x8000*/);
bool hasPotion(int slot);
void SetSkill(WORD nSkill, BYTE nHand);
void WeaponSwitch();
int GetUnitX(Path *hPath);
int GetUnitY(Path *hPath);
int GetMouseX();
int GetMouseY();
BOOL IsSwitched();

//other funcs
void DefineOffsets();

void _assert(char * str,char * file,int line);

DWORD GetDllOffset(char *dll, int offset);
DWORD GetDllOffset(int num);
void AssignOffset(void * Patch);
void PatchCall(DWORD dwAddress, LPVOID lpvFunction);
bool WriteMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize);
//bool PatchMemory(LPVOID lpvAddress, LPVOID lpvNewData, LPVOID lpvOldData, DWORD dwSize);

enum {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2LANG, DLLNO_D2WIN, DLLNO_D2NET, DLLNO_D2GAME, DLLNO_D2LAUNCH, DLLNO_FOG, DLLNO_BNCLIENT, DLLNO_STORM};

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|((b1)<<8))

#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2EXPFUNC(d1,v1,t1,t2) typedef t1 d1##_##v1##_t t2; extern d1##_##v1##_t *d1##_##v1;
#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2EXPVAR(d1,v1,t1)     typedef t1 d1##_##v1##_t;    extern d1##_##v1##_t *p_##d1##_##v1;
#define D2ASMPTR(d1,v1,o1)        DWORD d1##_##v1 = DLLOFFSET(d1,o1);
#define D2EXPASM(d1,v1)        extern DWORD d1##_##v1;

//extern int *p_D2CLIENT_nArea;
extern int *p_D2CLIENT_nDiff;
extern BnetData **p_D2LAUNCH_bnData;
extern DWORD *p_D2CLIENT_dwMsgId;
extern int *p_D2CLIENT_bAlwaysRun;
extern int *p_D2CLIENT_slotsWithPotion;
extern int *p_D2CLIENT_bWeapSwitch;
