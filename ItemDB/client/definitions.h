#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

///////////////////////////////////////////////
// Originally Mousepad's Code
///////////////////////////////////////////////

///////////////////////////////////////////////
// And Ninjai D2BS Code
///////////////////////////////////////////////
#define D2NET_MAX_MSG_SIZE 512
#define MAX_MSG_SIZE 516

enum eUIModes { UI_ON = 0, UI_OFF = 1, UI_TOGGLE = 2 };

struct UnitAny;
struct UnitItem;
struct UnitMonster;
struct Act;
struct Room;
struct RoomOther;
struct Drlg;
struct PresetUnit;
struct DrlgColl;
struct DrlgDS1Pre;
struct PresetType2info;
struct Inventory;

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

struct SuperuniqueTxt {
	WORD nIndex; //+00
	WORD nLocaleTxtNo; //+02
	DWORD _1[9]; //+04
	BYTE _1a[4]; //+28
	WORD tcs[4]; //+2c
};

struct Point {
	short X;
	short Y;
};

struct D2MSG {
	HWND hwnd;				// 0x00
	DWORD message;				// 0x04
	WPARAM wParam;				// 0x08
	union {
		LPARAM lParam;
		Point pt;
	};						// 0x0C
	DWORD _1;					// 0x10
	DWORD hwnd2;				// 0x14
	DWORD _2[2];				// 0x18
};

/*struct D2HANDLER {
	DWORD _1;					// 0x00
	DWORD Message;				// 0x04
	DWORD Handler;				// 0x08
};*/
// Third and final piece to the "Handler Table" experiment.  Didn't quite work
// This struct was one of the reasons.  Mainly, I could decide what the _1 was
// and if it should go at the beginning of the struct, or at the end.

struct BnetData
{
	DWORD Id;					//0x00
	DWORD Id2;					//0x04
	DWORD Unk0;					//0x08
	DWORD Unk1;					//0x0C
	DWORD Unk2;					//0x10
	DWORD Id3;					//0x14
	WORD Unk3;					//0x18
	char GameName[22];			//0x1A
	char GameIp[16];			//0x30
	DWORD Unk4[16];				//0x40
	DWORD Id4;					//0x80
	DWORD Unk6;					//0x84
	char AccountName[48];		//0x88
	char PlayerName[24];		//0xB8
	char RealmName[8];			//0xD0
	BYTE Unk7[273];				//0xD8
	BYTE CharType;				//0x1E9 00 - 06
	BYTE CharFlags;				//0x1EA LoD, Ladder, Hc Flags + 4 Unk
	BYTE MaxDifficulty;			//0x1EB
	BYTE Unk8[31];				//0x1EC
 	BYTE Difficulty;			//0x20B Works only when creating, not joining game
	DWORD *pUnk9;				//0x20C
	DWORD Unk10[3];				//0x210
	DWORD Unk11;				//0x21C
	DWORD Unk12;				//0x220
	WORD Unk13;					//0x224
	BYTE Unk14;					//0x226
	char RealmName2[24];		//0x227
	char GamePass[24];			//0x23F
	char GameDesc[256];			//0x257
	WORD Unk15;					//0x348
	BYTE Unk16;					//0x34A
	char CurChannel[32];		//0x34B
};

enum eMessageTypes { CHAT_NORMAL = 1, CHAT_WHISPERFROM, CHAT_3, CHAT_NONAME, CHAT_OVERHEAD, CHAT_WHISPERTO, CHAT_SCROLL };

struct ChatMsg {
   BYTE Identifier;        // 0x00  // should always be 0x26
   BYTE MessageType;       // 0x01  // 01 is standard chat message

                                    // 02 prints in the format "%s whispers: %s" where the first var is Name, and the
                                    // second is Message.

                                    // 04 bypasses the name checks, etc, and prints the message with color specified in
                                    // Color.

                                    // 05 creates an overhead struct and assigns it to the unit with id (UnitId)

                                    // 06 prints the "You whispered to %s:  %s, with the first var being Name, and the second
                                    // being Message.

                                    // 07 prints Message on a recipe scroll background.  Recipe scroll pane opens but lacks
                                    // buttons.
   BYTE Unk1;              // 0x02
   BYTE Style;             // 0x03  // 00 and 01 seem to print without sender, but 00 does some sort of checking, first
                                    // anything else prints as usual.
   DWORD UnitId;           // 0x04
   BYTE Color;             // 0x08
   BYTE Unk2;              // 0x09
   char Name[0x10];        // 0x0A
   char Message[0x100];    // 0x1A
};

struct Msg {
	BYTE Unk0;				//+00
   BYTE _1[3];                             //+01
	DWORD Unk1;				//+04
	DWORD Unk2;				//+08
	char Message[0x100];			//+0A //text msg 
};

struct Buffer
{
};

struct Quest
{
	Buffer *ptBuffer;			// 0x00
	DWORD Unk0;				// 0x04 // 60h * 2^3, 768
};

struct Waypoint
{
	BYTE flags;				// 0x00
};

struct PlrMsg
{
};

enum ePieceTokens {HD = 0, TR, LG, RA, LA, RH, LH, SH, S1, S2, S3, S4, S5, S6, S7, S8}; 

struct ObjectBinData {
   char Name[0x40];				// 0x00
   wchar_t wName[0x40];			// 0x40
   char Token[0x3];				// 0xC0
   BYTE SpawnMax;				// 0xC3
   BYTE Selectable[8];			// 0xC4
   DWORD TrapProb;				// 0xCC
   DWORD SizeX;				// 0xD0
   DWORD SizeY;				// 0xD4
   BYTE _1;					// 0xD8
   DWORD FrameCnt[8];			// 0xD9
   WORD FrameDelta[8];			// 0xF9
   BYTE CycleAnim[8];			// 0x109
   BYTE Lit[7];				// 0x111
   BYTE BlocksLight[8];			// 0x118
   BYTE Collision[8];			// 0x120
   BYTE IsAttackable;			// 0x128
   BYTE Start[8];				// 0x129
   BYTE OrderFlag[8];			// 0x131
   BYTE EnvEffect;				// 0x139
   BYTE IsDoor;				// 0x13A
   BYTE BlocksVis;				// 0x13B
   BYTE Orientation;			// 0x13C
   BYTE Trans;					// 0x13D
   BYTE PreOperate;				// 0x13E
   BYTE Mode[8];				// 0x13F
   short XOffset;				// 0x147
   short YOffset;				// 0x149
   bool Draw;					// 0x14B
   BYTE PieceTokens[0x10];		// 0x14F
   WORD _2;					// 0x15F
   BYTE TotalPieces;			// 0x161
   BYTE XSpace;				// 0x162
   BYTE YSpace;				// 0x163
   BYTE Red;					// 0x164
   BYTE Green;					// 0x165
   BYTE Blue;					// 0x166
   BYTE SubClass;				// 0x167
   long NameOffset;				// 0x168
   BYTE _3;					// 0x16C
   BYTE MonsterOK;				// 0x16D
   BYTE OperateRange;			// 0x16E
   BYTE ShrineFunction;			// 0x16F
   BYTE Act;					// 0x170 //this is a bitmask
   WORD _4;					// 0x171
   BYTE Restore;				// 0x173
   BYTE RestoreVirgins;			// 0x174
   BYTE _5[3];					// 0x175
   long Parm[8];				// 0x178
   /*BYTE Lockable;				// 0x198
   BYTE Gore;					// 0x199
   BYTE Sync;					// 0x19A
   BYTE Beta;					// 0x19B*/ // these don't seem to match as being here, for some reason.
   BYTE nTgtFX;				// 0x198
   BYTE nTgtFY;				// 0x199
   BYTE nTgtBX;				// 0x19A
   BYTE nTgtBY;				// 0x19B
   WORD Damage;				// 0x19C // temp
   WORD CollisionSubst;			// 0x19E // temp
   DWORD Left;					// 0x1A0
   DWORD Top;					// 0x1A4
   DWORD Width;				// 0x1A8
   DWORD Height;				// 0x1AC
   BYTE ClientFn;				// 0x1B0
   BYTE InitFn;				// 0x1B1
   BYTE PopulateFn;				// 0x1B2
   BYTE OperateFn;				// 0x1B3				
   // Leaving the rest undefined for now, until I get some more bin dumps.
};

struct MonsterBinData {
};

struct ItemBinData {
	char flpName[0x20];			//0x00
	char invName[0x20];			//0x20
	char Name2[0x20];
	char Name3[0x20];
	char Codes[0x18];
	BYTE none2[0x77];
	BYTE SizeX;			
	BYTE SizeY;
	BYTE none3[0xD];
	BYTE Type;
};

struct GlobalData {
};


struct PlayerData : GlobalData {
	char PlayerName[0x10];			// 0x00
 	Quest *ptQuestNormal;			// 0x10
 	Quest *ptQuestNightmare;		// 0x14
 	Quest *ptQuestHell;			// 0x18
 	Waypoint *ptWaypointNormal;		// 0x1c
	Waypoint *ptWaypointNightmare;		// 0x20
 	Waypoint *ptWaypointHell;		// 0x24
 	BYTE * Unk0;				// 0x28
	DWORD Unk1;				// 0x2C
 	DWORD Unk2;				// 0x30
 	DWORD *ptAreaUnit;			// 0x34
 	DWORD Unk3[25];				// 0x38
 	PlrMsg *ptPlrMsg;			// 0x9C
};

struct ItemData : GlobalData {
	//old -ItemBinData * ptBinData;		// 0x00
	DWORD Quality1;						// 0x00
	DWORD _1[0x2];						// 0x04
	DWORD ItemFlags;					// 0x0C  1 == owned by player, 0xFFFFFFFF == not owned
	DWORD _15[0x2];						// 0x10
	DWORD dwFlags;						// 0x18
	DWORD _13[0x3];						// 0x1C
	DWORD Quality2;						// 0x28
	DWORD _14[0x3];						// 0x2C
	WORD Prefix;						// 0x38
	WORD _1a;							// 0x3A
	WORD _1ab;							// 0x3C
	WORD Suffix;						// 0x3E
	DWORD _1ac;							// 0x40
	BYTE ItemLocation;					// 0X44 = location on body = xpos of item (only if on body) (spread)
	BYTE ItemLocation2;					// 0 = inventory, 4=stash (spread)
	BYTE Unk1;
	WORD Unk2;
	DWORD _1b[0x4];						// 
	Inventory * ptOwnerInv;				// 0x5C
	DWORD _2;							// 0x60
	UnitItem * ptNextInvenItem;			// 0x64
	BYTE _3;							// 0x68
	BYTE NodePage;						// 0x69
	WORD Pad1;					        // 0x6A
	DWORD unk3;							// 0x6C
	DWORD unk4;							// 0x70
	DWORD unk5;							// 0x74
	DWORD unk6;							// 0x78
	DWORD unk7;							// 0x7C
	DWORD unk8;							// 0x80
	UnitAny * ptOwner;					// 0x84
};

/*
struct ItemData : GlobalData {
	ItemBinData * ptBinData;			// 0x00
	DWORD _1[0x10];						// 0x04
	BYTE ItemLocation;					// 0X44
	DWORD _1b[0x5];						// 0x48
	Inventory * ptOwnerInv;				// 0x5C
	DWORD _2;							// 0x60
	UnitItem * ptNextInvenItem;			// 0x64
};
*/

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

struct TileData : GlobalData {
	/*
[20:54] <taylorjonl> 	DWORD			nWarpID;		//+00
[20:54] <taylorjonl> 	int				xSelect;		//+04
[20:54] <taylorjonl> 	int				ySelect;		//+08
[20:54] <taylorjonl> 	int				dxSelect;		//+0c
[20:54] <taylorjonl> 	int				dySelect;		//+10
[20:54] <taylorjonl> 	int				xExitWalk;		//+14
[20:54] <taylorjonl> 	int				yExitWalk;		//+18
[20:54] <taylorjonl> 	int				cxOffset;		//+1c
[20:54] <taylorjonl> 	int				cyOffset;		//+20
[20:54] <taylorjonl> 	int				nLitVersion;	//+24
[20:54] <taylorjonl> 	int				nTiles;			//+28
[20:54] <taylorjonl> 	int				chDirection;	//+2c	b/l/r
*/
};

struct ObjectData : GlobalData { //0x80 bytes in length
   ObjectBinData * ptBinData;
   DWORD _1;
   DWORD _2;
   DWORD _3;
   DWORD _4;
   DWORD _5;
   DWORD _6;
   DWORD _7;
   DWORD _8;
   DWORD _9;
   char parent[16];
};


struct Stats {
	DWORD dwStatsID;	// Stats ID
    DWORD dwValue;	// Stats Value
};

struct StatsList {
	DWORD _1[0x9];			// 0x00
	Stats * ptStats;		// 0x24
	WORD StatCount;		// 0x28
	WORD StatCount2;   		// 0x2A
	DWORD _2[2];			// 0x2C
	BYTE * _3;			// 0x34
	DWORD _4;				// 0x38
	StatsList * ptNext;		// 0x3C
};

struct DrlgLevel {
   Drlg * pDrlg;                  // 0x00
   int LevelNo;                   // 0x04
   DWORD _1[6];
   DWORD LevelX;					//1C
   DWORD LevelY;					//20
   DWORD LevelCX;					
   DWORD LevelCY;					
   RoomOther *pRoomOtherFirst;			//+30
   DWORD _3;                      // 0x34
   BYTE * ptMazeDef;              // 0x38
   DrlgDS1Pre *pRoom2FirstDS1Pre; // 0x3C
   DWORD _5[0x28];                // 0x40
   BYTE * ptLogicalRoomInfo;      // 0xE0
   DWORD _6[0x47];				// 0xE4
   DWORD _7[11];				//+200
   DrlgLevel *pNext;			//0x204
   DWORD _8[3];				//0x208
   BYTE * pnJungleDefs;           // 0x214
   // Leaving the rest undefined for now.
};
struct Drlg {
	DrlgLevel * pFirstLevel;       	// 0x00
	DWORD _1[2];			// 0x04
	Act * pAct;			// 0x0C
	int ActNum;			// 0x10
	DWORD _2[248];		//0x14
	DWORD nStaffTombLvl; //+3f4
	DWORD nBossTombLvl; //+3f8
	// Leaving the rest undefined for now.		
};

struct PresetUnit {
   DWORD nUnitType; //+00
   DWORD nTxtFileNo; //+04
   DWORD _1[1];
   DWORD xPos; //+0c position in relation to its room
   DWORD yPos; //+10 position in relation to its room
   DWORD _2[2];
   PresetUnit *pNext; //+1c
};

struct RoomTile {
	RoomOther *ptRoomOther;
	DWORD * nNum;
	DWORD _1[3];
	RoomTile *pNext;
};

struct RoomOther {
	DrlgLevel *ptDrlgLevel;       // 0x00
	DWORD xPos;                   // 0x04
	DWORD yPos;                   // 0x08
	DWORD xSize;				// 0x0C
	DWORD ySize;				// 0x10
	BYTE * _2;                    // 0x14
	DWORD _3[0x1];                // 0x18
	DWORD nPresetType;				//+1c   //1 for room1 type 2 for regular room
	PresetType2info * pPresetType2info; //+20
	DWORD _5;                     // 0x24
	BYTE * _6[0x10];              // 0x28
	DWORD _7[0x10];               // 0x68
	BYTE * _8;                    // 0xA8
	DWORD _9[0x3];                // 0xAC
	RoomOther * ptNext;           // 0xB8 //Only appears when DrlgRoom1 appears
	RoomOther * ptPrev;           // 0xBC //these 2 are sometimes rooms, sometimes not a room. Only appears when DrlgRoom1 appears
	RoomOther ** ptList;          // 0xC0 //rooms that are near you  //This all makes more sense now, when they are  close, the above is a linked list
	int nRoomList;                // 0xC4 //the number of rooms near you
	DWORD _10;                    // 0xC8 RoomTile *pRoomTiles; //+c8 Only appears when DrlgRoom1 appears
	PresetUnit *pPresetUnits;	// +cc Only appears when DrlgRoom1 appears //EXTREMELY IMPORTANT
	DWORD _12[0x4];               // 0xD0
	BYTE * _13;                   // 0xE0
	Room *pRoom;				// 0xE4 //Pointer to the room1 associated with this room2. Collision data resides in the room1 structure
	RoomOther *pNext;             // 0xE8
};

struct PresetType2info {
	DWORD roomNum;   
	DWORD _5[1];   
	DrlgDS1Pre *pDrlgDS1Pre;
};

struct DrlgDS1Pre {
	DWORD ds1roomNum;
	DWORD ds1used;
	//ds1fileinfo *pds1fileinfo;
	DWORD tmp1;
	//placed here because the actual ds1 file is not important
	DrlgDS1Pre *pNext;
	DWORD posX;
	DWORD posY;
	DWORD sizex;
	DWORD sizey;
	DWORD _1[13];
	DrlgDS1Pre *pPrevious;
};

struct Act {
	int ActNum;			                   // 0x00
	Room * ptFirstRoom;		                   // 0x04
	Drlg * ptMisc;			                   // 0x08
	DWORD _1[2];			                   // 0x0C
	BYTE * _2;			                   // 0x14
	//__fastcall int(*pfnRoomCallBack)(DWORD, DWORD);    // 0x18 act callback.
	DWORD pfnActCallBack;                              // 0x18
	DWORD _4[4];                                       // 0x1C
};

struct Coords {
   DWORD BeginXpos;              // 0x00
   DWORD BeginYpos;              // 0x04
   DWORD Xsize;                  // 0x08
   DWORD Ysize;                  // 0x0C
   DWORD _1[2];                  // 0x10
   DWORD _2[2];                  // 0x18
};

struct Room : Coords {
	BYTE * _3;                    // 0x20
	Room ** ptRoomList;           // 0x24
	int NumRooms;                 // 0x28
	UnitAny * ptFirstUnit;        // 0x2C
	UnitAny * ptOtherUnit;        // 0x30 ?
	DrlgColl *pColl;			// +34 --> discovered by woof
	RoomOther * ptRoomOther;      // 0x38
	DWORD Seed;                   // 0x3C
	DWORD _6[0x3];                // 0x40
	BYTE * _7;                    // 0x4C
	DWORD _8[3];                  // 0x50
	int nPlayerUnits;             // 0x5C
	DWORD _9[6];                  // 0x60
	Act * ptAct;                  // 0x78
	Room * ptNext;                // 0x7C
};

struct DrlgColl {
   DWORD            nPosGameX;
   DWORD            nPosGameY;
   DWORD            nSizeGameX;
   DWORD            nSizeGameY;
   DWORD            nPosRoomX;
   DWORD            nPosRoomY;
   DWORD            nSizeRoomX; 
   DWORD            nSizeRoomY;
   WORD *pMapStart;
   WORD *pMapEnd;   
   /* 0028 end of struct Collision 40 bytes */
};

struct Path 
{
	WORD wXOffset;				// 0x00
	WORD wX;				// 0x02
	WORD wYOffset;				// 0x04
	WORD wY;				// 0x06
	DWORD targetX;				// 0x08 
	DWORD targetY;				// 0x0C
	WORD sY;				// 0x10
	WORD sX;				// 0x12
	WORD sY2;				// 0x14
	WORD sX2;				// 0x16
	WORD sY3;				// 0x18
	WORD sX3;				// 0x1A
	Room * ptRoom; 				// 0x1C
	Room * ptRoom2;            		// 0x20
	DWORD _1[3];                            // 0x24
	UnitAny * ptUnit;       		// 0x30
	DWORD flags;            		// 0x34
	DWORD _2;               		// 0x38
	DWORD PathType;         		// 0x3C
	DWORD PrevPathType;     		// 0x40
	DWORD UnitSize;         		// 0x44
	DWORD CollisionPattern; 		// 0x48
	DWORD _3[3];            		// 0x4C
	UnitAny * ptTarget;     		// 0x58
	int TargetType;         		// 0x5C
	DWORD TargetId;         		// 0x60
	DWORD _4[0xF];                          // 0x64
	// Leaving the rest undefined for now
};

struct ObjPath
{
	Room * ptRoom;				// 0x00
	DWORD _1[2];				// 0x04
	DWORD Xpos;				// 0x0C
	DWORD Ypos;				// 0x10
	DWORD _2[5];				// 0x14
	BYTE * ptUnk;				// 0x28
	DWORD _3[5];				// 0x2C
};

struct Inventory {
	DWORD Signature;  			// 0x00  always 0x01020304
	BYTE * ptGame1C;              		// 0x04 //will be 0 for client
	UnitAny * ptOwner;			// 0x08
	UnitItem * pFirstItem;			// 0x0C
	UnitItem * pLastItem;			// 0x10
	BYTE * _1;				// 0x14
	DWORD _2;				// 0x18
	DWORD LeftItemUid;			// 0x1C
	UnitItem * CursorItem;			// 0x20
	DWORD OwnerUid;				// 0x24
	int NumItems;				// 0x28
	DWORD _3[0x2];				// 0x2C
	BYTE * _4;				// 0x34
	DWORD _5;					// 0x38
};

struct SkillInfo {
	WORD Id;				// 0x00
	BYTE bUnks[0x32];		// 0x02
	DWORD dwUnk;			// 0x34
};

struct Skill {				// 0x40 bytes long
	SkillInfo * pSkillInfo;	// 0x00
	Skill * pNextSkill;		// 0x04
	DWORD dwUnk1;			// 0x08
	DWORD dwUnk2[0x7];		// 0x0C
	DWORD SkillLevel;		// 0x28
	DWORD dwUnk3[2];		// 0x2C
	DWORD Flags;			// 0x34
	DWORD dwUnk5[2];		// 0x38
};

struct Info
{
	BYTE * ptGame_1C;              // 0x00
	Skill * ptFirstSkill;          // 0x04
	Skill * ptlSkill;             // 0x08
	Skill * ptrSkill;             // 0x0C
};

struct Light {
   DWORD _1[0x3];                // 0x00
   int nType;                    // 0x0C
   DWORD _2[0x7];                // 0x10
   DWORD fStaticValid;           // 0x2C
   int * pnStaticMap;            // 0x30

};

struct Game
{
};

struct Packet {
	long nTotal;				// 0x00
	long nUsed;				// 0x04
};

struct Roster {
	char Name[0x10];              // 0x00
	DWORD PlayerId;               // 0x10
	DWORD _1[0x2];                // 0x14
	DWORD ClassId;                // 0x1C
	WORD _2;                      // 0x20
	WORD _3;                      // 0x22
	DWORD LevelId;                // 0x24
	DWORD Xpos;                   // 0x28
	DWORD Ypos;                   // 0x2C
	DWORD _4;                     // 0x24
	BYTE * _5;                    // 0x34
	DWORD _6[0xB];                // 0x38
	WORD _7;                      // 0x64
	char Name2[0x10];             // 0x66
	WORD _8;                      // 0x76
	DWORD _9[0x2];                // 0x78
	Roster * ptNext;              // 0x80
};

// added needed DWORDS (spread)
struct RosterPet {
	DWORD ClassId;				//0x00
	DWORD PetType;				//0x04
	DWORD Id;					//0x08
	DWORD OwnerId;				//0x0C	Owners PlayerUnitId
	DWORD	_2;					//0x10
	DWORD	_3;					//0x14
	DWORD	_4;					//0x18
	DWORD	_5;					//0x1C
	DWORD	_6;					//0x20
	DWORD	_7;					//0x24
	DWORD	_8;					//0x28
	DWORD	_9;					//0x2C
	RosterPet *PetNext;			//0x30
};

//  Unit_All struct (basic)
struct UnitAny
{
	DWORD dwType;					// 0x00
	DWORD dwClassId;				// 0x04
	DWORD dwUnk1;					// 0x08
	DWORD dwId;					// 0x0C
	DWORD dwMode;					// 0x10
	union
	{
		PlayerData *ptPlayerData;
		ItemData *ptItemData;
		MonsterData *ptMonsterData;
		TileData *ptTileData;
		ObjectData * ptObjectData;
	};							// 0x14
	DWORD dwAct;               		// 0x18
	Act * ptAct;					// 0x1C
	DWORD dwSeed[2];				// 0x20
	DWORD dwUnk2;					// 0x28
	union 
	{
		Path * hPath;
		ObjPath *hOPath;
	};							// 0x2C
	DWORD dwUnk3[5];				// 0x30
	DWORD dwGfxFrame;				// 0x44
	DWORD FrameRemain;				// 0x48
	WORD FrameRate;           		// 0x4C
	WORD _1;                           // 0x4E
	BYTE * ptGfxUnk;				// 0x50
	BYTE * ptGfxInfo;			// 0x54
	DWORD dwUnk5;					// 0x58
	StatsList * ptStatsList;			// 0x5C
	Inventory * ptInventory;			// 0x60
	Light * ptLight;	       		// 0x64
	DWORD dwUnk6;					// 0x68
	bool bBusy;					// 0x6C
	DWORD dwUnk7[4];				// 0x70
	Game *ptGame;					// 0x80
	DWORD dwUnk8[2];				// 0x84
	WORD wX;						// 0x8C
	WORD wY;						// 0x8E
	DWORD dwUnk9[5];				// 0x90
	Msg * ptMsg;					// 0xA4
	Info * ptInfo;					// 0xA8
	DWORD dwUnk10[6];				// 0xAC
	DWORD dwFlags;					// 0xC4
	DWORD dwFlags2;				// 0xC8
	DWORD dwUnk11[3];				// 0xCC
	Packet * ptPacket;				// 0xD8
	DWORD dwUnk12;    				// 0xDC
	UnitAny * ptChangedNext;			// 0xE0
	UnitAny * ptRoomNext;			// 0xE4	
	UnitAny * ptListNext;      		// 0xE8
};

struct UnitPlayer : UnitAny
{
};

struct UnitItem : UnitAny
{
};

struct UnitMonster : UnitAny
{
};

struct UnitObject : UnitAny
{
};

struct UnitStats {};
struct ListStats
{
    WORD substatno; //+00
    WORD statno; //+02
    int value; //+04
};

#endif // __DEFINITIONS_H__