#ifndef D2_STRUCTS_H
#define D2_STRUCTS_H

#include <windows.h>

#define CELLNO_MYSHRINES 1176
#define CELLNO_WAYPOINT 307
#define CELLNO_SHRINE 310
#define NUMOF_SHRINES 23

#define ITEMFLAG_SOCKETED 0x800
#define ITEMFLAG_ETHEREAL 0x400000
#define OBJ_CHEST_LOCKED 0x80


//d2 structures etc

enum UIVar {
	UIVAR_INVENTORY = 1,
	UIVAR_STATS = 2,
	UIVAR_SKILLS = 4,
	UIVAR_QUEST = 15,
	UIVAR_STASH = 25,
	UIVAR_CUBE = 26,
	UIVAR_PET = 36,
};


enum UnitStat {
	STAT_LIFE		= 0x06,
	STAT_MAXLIFE	= 0x07,
	STAT_LEVEL		= 0x0c,
	STAT_GOLD		= 0x0e,
	STAT_MAGIC_DAMAGE_REDUCED = 0x23,
	STAT_DAMAGE_REDUCED = 0x24,
	STAT_MAGIC_RESIST = 0x25,
	STAT_FIRE_RESIST = 0x27,
	STAT_LIGHTING_RESIST = 0x29,
	STAT_COLD_RESIST = 0x2B,
	STAT_POSION_RESIST = 0x2D,
	STAT_LIFE_LEECH = 0x3c,
	STAT_MANA_LEECH	= 0x3e,
	STAT_EXTRA_GOLD = 0x4f,
	STAT_MAGIC_FIND = 0x50,
	STAT_IAS		= 0x5d,
	STAT_FRW		= 0x60,
	STAT_FHR		= 0x63,
	STAT_FBR		= 0x66,
	STAT_FCR		= 0x69,
	STAT_DAMAGE_TAKEN_GOES_TO_MANA = 0x72,
	STAT_CRUSHING_BLOW = 0x88,
	STAT_MANA_AFTER_EACH_KILL = 0x8a,
	STAT_FIRE_ABSORB = 0x8f,
	STAT_LIGHTING_ABSORB = 0x91,
	STAT_COLD_ABSORB = 0x94,
	STAT_NUMSOCKETS	= 0xc2,
	STAT_UNK1		= 0x100,
	STAT_MAGIC_ABSORB = 0,
	STAT_DEADLY_STRIKE = 0,
	STAT_PIERCING_ATTACK = 0,
	STAT_BLESSED_AIM = 0,
	STAT_REGENERATE_MANA = 0
};

enum UnitNo {
	UNITNO_PLAYER	= 0,
	UNITNO_MONSTER	= 1,
	UNITNO_OBJECT	= 2,
	UNITNO_MISSILE	= 3,
	UNITNO_ITEM		= 4,
	UNITNO_ROOMTILE	= 5
};

struct DrlgRoom2;
struct DrlgLevel;
struct DrlgMisc;
struct DrlgAct;
struct UnitAny;

struct D2Seed {
	DWORD d1,d2;
};

struct PresetUnit {
	DWORD nUnitType; //+00
	DWORD nTxtFileNo; //+04
	DWORD _1[1];
	DWORD xPos; //+0c
	DWORD yPos; //+10
	DWORD _2[2];
	PresetUnit *pNext; //+1c
};

struct RoomTile {
	DrlgRoom2 *pRoom2; //+00
	DWORD *nNum; //+04
	DWORD _1[3];
	RoomTile *pNext; //+14
};

struct DrlgRoom1 {
	DWORD xpos1; //+00
	DWORD ypos1; //+04
	DWORD xsize1; //+08
	DWORD ysize1; //+0c
	DWORD xpos2; //+10
	DWORD ypos2; //+14
	DWORD xsize2; //+18
	DWORD ysize2; //+1c
	DWORD _1[1];
	DrlgRoom1 **paRoomsNear; //+24
	DWORD nRoomsNear; //+28;
	UnitAny *pUnitFirst; //+2c
	DWORD _2[2];
	DrlgRoom2 *pRoom2; //+38
	D2Seed seed; //+3c
	DWORD _3[14];
	DrlgRoom1 *pNext; // +7c
};

struct DrlgRoom2 {
	DrlgLevel *pDrlgLevel; //+00
	DWORD xPos; //+04
	DWORD yPos; //+08
	DWORD xPos1; // +0c
	DWORD yPos1; // +10
	DWORD _1[2];
	DWORD nPresetType; //+1c
	DWORD *nPresetType2No; //+20
	DWORD _2[39];
	DrlgRoom2 **paRoomsNear; //+c0
	DWORD nRoomsNear; //+c4
	RoomTile *pRoomTiles; //+c8
	PresetUnit *pPresetUnits; //+cc
	DWORD _3[1];
	D2Seed seed; //+d4
	DWORD _3b[2];
	DrlgRoom1 *pRoom1; //+e4
	DrlgRoom2 *pNext; //+e8
};

struct DrlgLevel {
	DrlgMisc *pDrlgMisc; //+00
	DWORD nLevelNo; //+04
	DWORD _1[10];
	DrlgRoom2 *pRoom2First; //+30
};

struct DrlgMisc {
	DrlgLevel *pLevelFirst; //+00
	DWORD _1[2];
	DrlgAct *pDrlgAct; //+0c
	DWORD _2[250];
	DWORD nStaffTombLvl; // +3f8
	DWORD nBossTombLvl; //+3fc
};

struct DrlgMisc_109 {
	DrlgLevel *pLevelFirst; //+00
	DWORD _1[2];
	DrlgAct *pDrlgAct; //+0c
	DWORD _2[249];
	DWORD nStaffTombLvl; //+3f4
	DWORD nBossTombLvl; //+3f8
};

struct DrlgAct {
	DWORD _1;
	DrlgRoom1 *pRoom1; //+04
	DrlgMisc *pDrlgMisc; //+08
};

struct DrlgAct_109 {
	DWORD _1[2];
	DrlgMisc *pDrlgMisc; //+08
};

struct AutomapCell {
	DWORD fSaved; //+00
	WORD nCellNo; //+04
	WORD xPixel; //+06
	WORD yPixel; //+08
	WORD _1[1];
	AutomapCell *pLess; //+0c
	AutomapCell *pMore; //+10
};

struct AutomapLayer {
	DWORD _1[2];
	AutomapCell *pFloors; //+08
	AutomapCell *pWalls; //+0c
	AutomapCell *pObjects; //+10
	AutomapCell *pExtras; //+14
};

struct AutomapCellNode {
	DWORD _1[4];
	AutomapCell *pAutomapCell; // +10
	DWORD _2;
	AutomapCellNode *pNext; // +18
};

struct ObjectTxt {
	char szName[0x40];	// +00
	wchar_t wszName[0x40]; // +40
	BYTE _1[4]; // +c0
	BYTE nSelectable0; //+c4
	BYTE _2[0x87];
	BYTE nOrientation; //+14c
	BYTE _2b[0x19];
	BYTE nSubClass; // +166
	BYTE _3[0x11];
	BYTE nParm0; //+178
	BYTE _4[0x39];
	BYTE nPopulateFn; //+1b2
	BYTE nOperateFn; //+1b3
	BYTE _5[8];
	DWORD nAutoMap; // +1bb
};

struct ObjectTxt_109 {
	BYTE _1[0xc4];
	BYTE nSelectable0; //+c4
	BYTE _2[0x87];
	BYTE nOrientation; //+14c
	BYTE _2b[0x2a];
	BYTE nSubClass; //+177
	BYTE _3[0x10];
	BYTE nParm0; //+188
	BYTE _4[0x39];
	BYTE nPopulateFn; //+1c2
	BYTE nOperateFn; //+1c3
	BYTE _5[8];
	DWORD nAutoMap; //+1cc
};

struct ObjgroupTxt {
	DWORD nId[8]; //+00
	BYTE nDensity[8]; //+20
	BYTE nProb[8]; //+28
};

struct LevelTxt {
	BYTE _1[0xf5];
	char cszName[40]; //+f5
	char cszName2[40]; //+11d
	char szLvlDesc[41]; //+145
	wchar_t szName[40]; //+16e
	wchar_t szName2[40]; //+16e
	BYTE nObjGrp[8]; // unk
	BYTE nObjPrb[8]; // unk
	BYTE _1b[0x79];
};

struct LevelTxt_109 {
	BYTE _1[0x18d];
	BYTE nObjGrp[8]; //+18d
	BYTE nObjPrb[8]; //+195
	BYTE _1b[0x79];
	wchar_t szName[40]; //+216
};

struct ItemTxt {
	wchar_t szName2[0x40]; //+00
	union {
		DWORD dwCode; // +80
		char szCode[4];
	};
	BYTE _2[0x70]; // +84
	WORD nLocaleTxtNo; // +f4
	BYTE _2a[0x28]; // +f6
	BYTE nType; // +11e
	BYTE _3[0x0d];
	BYTE fQuest; // +12a
};

struct ItemTxt_109 {
	char szName[0x40]; //+00
	wchar_t szName2[0x40]; //+40
	BYTE _1[0x84];
	union {
		DWORD dwCode; //+144
		char szCode[4];
	};
	BYTE _2[0x6b];
	BYTE nType; //+1b3
	BYTE _3[4];
	BYTE fQuest; //+1b8
};

struct MonsterTxt {
	BYTE _1[0x6];
	WORD nLocaleTxtNo; // +06
	WORD flag; // +08
	WORD _1a;
	union {
		DWORD flag1; // +0c
		struct {
			BYTE flag1a; // +0c
			BYTE flag1b; // +0d
			BYTE flag1c[2]; // +0e
		};
	};
	BYTE _2[0x22]; // +10
	WORD velocity; // +32
	BYTE _2a[0x52]; // +34
	WORD tcs[3][4]; //+86
	BYTE _2b[0x52]; // +9e
	wchar_t szDescriptor[0x3c]; //+f0
	BYTE _3[0x1a0];
};

struct MonsterTxt_109 {
	BYTE _1[0xf0];
	wchar_t szDescriptor[0x3c]; //+f0
	BYTE _2[0x1a0];
	DWORD tcs[3][4]; //+308
};

struct SuperuniqueTxt {
	WORD nIndex; //+00
	WORD nLocaleTxtNo; //+02
	DWORD _1[9]; //+04
	BYTE _1a[4]; //+28
	WORD tcs[4]; //+2c
};

struct SuperuniqueTxt_109 {
	BYTE _1[0xd8];
	DWORD tcs[3]; //+d8
};

struct PlayerData {
	wchar_t szName[1]; //+00
};


struct MonsterData {
	BYTE _1[0x16];
	struct {
		BYTE fUnk:1;
		BYTE fNormal:1;
		BYTE fChamp:1;
		BYTE fBoss:1;
		BYTE fMinion:1;
	}; //+16
	BYTE _1aa;
	wchar_t szName[1]; //+18
	BYTE _1a[2]; // +1a
	BYTE anEnchants[9]; // +1c
	BYTE _2a; // +25
	WORD uniqueno; // +26
	BYTE _2[50*4-14];
	struct {
		DWORD _3[1];
		WORD uniqueno; //+04
		WORD _4[1];
		DWORD anEnchants[9]; //+08
	} *pEnchants; //+cc
};

struct MonsterData_109 {
	struct {
		WORD fBoss:1;
		WORD fMinion:1;
		WORD fChamp:1;
	}; //+00
	wchar_t szName[1]; //+02
	DWORD _2[50];
	struct {
		DWORD _3[1];
		WORD uniqueno; //+04
		WORD _4[1];
		DWORD anEnchants[9]; //+08
	} *pEnchants; //+cc
};

struct ObjectData {
	ObjectTxt *pTxtFile; //+00 ??
	union {
		BYTE nShrineNo;
		struct {
			BYTE _1:7;
			BYTE fChestLocked:1;
		};
	}; //+04
};

struct ItemData {
	DWORD nQuality;
	BYTE _1[0x14]; // +04
	DWORD nFlag; // +18
	BYTE _2[0x10]; // +1c
	DWORD nItemLevel; // +2c
	BYTE _3[0x14]; // +30
	BYTE nLocation1; // +44					// location on body = xpos of item (only if on body) (spread)
	BYTE nLocation2; // +45					// 0 = inventory, 4=stash (spread)
};

struct ItemData_109 {
	DWORD nQuality;
};

struct UnitInventory {
	DWORD nFlag; // +00
	DWORD _1[2];
	DWORD nItem; // +0c
};

struct ObjectPath {
	DWORD xpos1; //+00
	DWORD ypos1; //+04
	DWORD xpos2; //+08
	DWORD ypos2; //+0c
	WORD targetx; //+10
	WORD targety; //+12
	DWORD _1[2];
	DrlgRoom1 *pRoom1; //+1c
	DWORD _2[14];
	UnitAny *pTargetUnit; //+58
};
struct ItemPath {
	void * ptRoom;				// 0x00
	DWORD _1[2];				// 0x04
	DWORD xpos;				// 0x0C
	DWORD ypos;				// 0x10
	DWORD _2[5];				// 0x14
	BYTE * ptUnk;				// 0x28
	DWORD _3[5];				// 0x2C
};
struct UnitAny {
	DWORD nUnitType; //+00
	DWORD nTxtFileNo; //+04
	DWORD _1;
	DWORD nUnitId; // +0C
	DWORD eMode; // +10
	union {
		PlayerData *pPlayerData; //+14
		MonsterData *pMonsterData; //+14
		ObjectData *pObjectData; //+14
		ItemData *pItemData; //+14
	};
	DWORD _2;
	UnitAny *pNext; //+1c
	WORD _2a0;
	WORD unkNo;
	DWORD _2a[2];
	union {
		ObjectPath *pPos;
		ItemPath *pItemPath;
	};
	//struct {
	//	DWORD xpos1; //+00
	//	DWORD ypos1; //+04
	//	DWORD xpos2; //+08
	//	DWORD ypos2; //+0c
	//	WORD targetx; //+10
	//	WORD targety; //+12
	//	DWORD _1[2];
	//	DrlgRoom1 *pRoom1; //+1c
	//	DWORD _2[14];
	//	UnitAny *pTargetUnit; //+58
	//} *pPos; // +2c
	DWORD _2b[12];
	UnitInventory *pInventory; //+60
	DWORD _4b[12];
	DWORD nOwnerType; // +94
	DWORD nOwnerId; // +98
	BYTE _4c[42];
	BYTE  fFlags0:1; //+0c6, new added
	BYTE _4d;
	DWORD _5[10];
	DWORD fFlags; //+e8
	DWORD fFlags2; //+ec
	DWORD _6[7];
};


struct UnitAny_109 {
	DWORD nUnitType; //+00
	DWORD nTxtFileNo; //+04
	DWORD nUnitId; //+08
	DWORD eMode; //+0c
	DWORD _1[10];
	struct {
		DWORD xpos1; //+00
		DWORD ypos1; //+04
		DWORD xpos2; //+08
		DWORD ypos2; //+0c
		WORD targetx; //+10
		WORD targety; //+12
		DWORD _1[2];
		DrlgRoom1 *pRoom1; //+1c
		DWORD _2[14];
		UnitAny *pTargetUnit; //+58
	} *pPos; //+38
	DWORD _2[13];
	union {
		PlayerData *pPlayerData; //+70
		MonsterData *pMonsterData; //+70
		ObjectData *pObjectData; //+70
		ItemData *pItemData; //+70
	};
	DWORD _4a[4];
	UnitInventory *pInventory; //+84
	DWORD _4b[12];
	DWORD nOwnerType; //+b8
	DWORD nOwnerId; //+bc
	DWORD _5[10];
	DWORD fFlags; //+e8
	DWORD fFlags2; //+ec
	DWORD _6[7];
	UnitAny *pNext; //+10c
};

struct GfxCell {
	DWORD flags; //+00 ??
	DWORD width; //+04
	DWORD height; //+08
	DWORD xoffs; //+0c
	DWORD yoffs; //+10
	DWORD _2[1];
	DWORD lpParent; //+18
	DWORD length; //+1c
	BYTE cols[1]; //+20
};

struct CellFile {
	DWORD dwVersion; //+00
	struct {
		WORD dwFlags;
		BYTE mylastcol;
		BYTE mytabno:1;
	}; //+04
	DWORD eFormat; //+08
	DWORD termination; //+0c
	DWORD numdirs; //+10
	DWORD numcells; //+14
	GfxCell *cells[1]; //+18
};

struct CellContext {
	GfxCell *hCell; //+00
	CellFile *pCellFile; //+04
	DWORD nCellNo; //+08
	DWORD direction; //+0c ??
	DWORD _1[14];
};

struct TreasureClassItem {
	DWORD _1[2];
	DWORD thingno; //+08
	struct {
		DWORD _3:2;
		DWORD tcflag:1;
	}; //+0c
	DWORD _2[3];
};

struct TreasureClass {
	DWORD _1a;
	DWORD numitems; //+04
	DWORD _1[8];
	TreasureClassItem *items; //+28
};

struct TreasureClass_109 {
	DWORD numitems; //+00
	DWORD _1[8];
	TreasureClassItem *items; //+24
};

struct D2EditBox2 {
	DWORD _1;
	DWORD _2[6];
	void (__fastcall *func)(D2EditBox2*); // +1c
	DWORD _3[0x0F];
	wchar_t* text;		// +5c
};
struct D2EditBox {
	DWORD _1;
	DWORD _2[6];
	void (__fastcall *func)(D2EditBox*); // +1c
	DWORD _3[0x0D];
	DWORD	dwSelectStart; // +54
	DWORD	dwSelectEnd; // +58
	wchar_t text[0x100];		// +5c
	DWORD	dwCursorPos;		// 0x25C
};

struct GameStructInfo
{
	DWORD _1[6];
	WORD _1a;
	char szGameName[0x18];
	char szGameServerIp[0x56];
	char szAccountName[0x30];
	char szCharName[0x18];
	char szRealmName[0x18];
	BYTE _2[0x157];
	char szGamePassword[0x18];
};

struct D2MSG {					// 0x20 bytes long
	HWND hwnd;				// 0x00
	DWORD message;				// 0x04
	WPARAM wParam;				// 0x08
	union {
		DWORD lParam;
		struct {
		WORD xpos;
		WORD ypos;
		};
	};
	DWORD _1;					// 0x10
	HWND hwnd2;				// 0x14
	DWORD _2[2];				// 0x18
};

#endif // D2_STRUCTS_H
