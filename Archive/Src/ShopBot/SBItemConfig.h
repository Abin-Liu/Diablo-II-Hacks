#ifndef __SBITEMCONFIG_H__
#define __SBITEMCONFIG_H__

#include "..\\D2Lib\\Classes\\Utils\\item.h"

#define SB_MAX_MOD			6
#define SB_MAX_DESCRIPTION	255

typedef struct tagSBGlobal
{
	BYTE iAct;
	BYTE iWP;
	WORD wNPC;
	BYTE iHotkey;
	BYTE iStopHP;
	BOOL bAutoStart;
} SBGLOBAL, *LPSBGLOBAL;
typedef const tagSBGlobal* LPCSBGLOBAL;

typedef struct tagSBItemConfig
{
	char szDescription[SB_MAX_DESCRIPTION + 1];
	char szItemCode[SB_MAX_DESCRIPTION + 1];
	BYTE iModNum;
	ITEMMOD aMods[SB_MAX_MOD];
	BYTE iSockets;
	BOOL bNonMagic;
} SBITEMCONFIG, *LPSBITEMCONFIG;
typedef const tagSBItemConfig* LPCSBITEMCONFIG;

// Shopbot npc ID
#define CHARIS	0x009a
#define AKARA	0x0094
#define FARA	0x00b2
#define GROGNAN	0x00b1
#define ORMUS	0x00ff
#define JAMELLA	0x0195
#define HALBU	0x0101
#define LARZUK	0x01ff
#define MALAH	0x0201
#define ANYA	0x0200

BOOL SBLoadGlobal(LPCSTR lpszFile, LPSBGLOBAL lpBuffer);
BOOL SBSaveGlobal(LPCSTR lpszFile, LPCSBGLOBAL lpGlobal);
BOOL SBLoadConfig(LPCSTR lpszFile, LPCSTR lpszSection, LPSBITEMCONFIG lpBuffer);
BOOL SBSaveConfig(LPCSTR lpszFile, LPCSBITEMCONFIG lpConfig);
BOOL SBVerifyItemMods(LPCITEM lpItem, LPCSBITEMCONFIG lpConfig);
DWORD SBGetNpcName(WORD wNpcID, LPSTR lpszBuffer, DWORD dwMaxChars);

#endif // __SBITEMCONFIG_H__