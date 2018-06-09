#ifndef __D2STRUCTS_H__
#define __D2STRUCTS_H__

#include <windows.h>

struct UnitAny;
struct Inventory;
struct UnitItem;

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

struct PlayerData
{
	char PlayerName[0x10];			// 0x00
};

struct ItemData
{
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

struct UnitStats {};
struct ListStats
{
    WORD substatno; //+00
    WORD statno; //+02
    int value; //+04
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
		LPVOID *ptMonsterData;
		LPVOID *ptTileData;
		LPVOID * ptObjectData;
	};							// 0x14
	DWORD dwAct;               		// 0x18
	LPVOID * ptAct;					// 0x1C
	DWORD dwSeed[2];				// 0x20
	DWORD dwUnk2;					// 0x28
	union 
	{
		LPVOID * hPath;
		LPVOID *hOPath;
	};							// 0x2C
	DWORD dwUnk3[5];				// 0x30
	DWORD dwGfxFrame;				// 0x44
	DWORD FrameRemain;				// 0x48
	WORD FrameRate;           		// 0x4C
	WORD _1;                           // 0x4E
	BYTE * ptGfxUnk;				// 0x50
	LPVOID * ptGfxInfo;			// 0x54
	DWORD dwUnk5;					// 0x58
	LPVOID * ptStatsList;			// 0x5C
	Inventory * ptInventory;			// 0x60
	LPVOID * ptLight;	       		// 0x64
	DWORD dwUnk6;					// 0x68
	bool bBusy;					// 0x6C
	DWORD dwUnk7[4];				// 0x70
	LPVOID *ptGame;					// 0x80
	DWORD dwUnk8[2];				// 0x84
	WORD wX;						// 0x8C
	WORD wY;						// 0x8E
	DWORD dwUnk9[5];				// 0x90
	LPVOID * ptMsg;					// 0xA4
	LPVOID * ptInfo;					// 0xA8
	DWORD dwUnk10[6];				// 0xAC
	DWORD dwFlags;					// 0xC4
	DWORD dwFlags2;				// 0xC8
	DWORD dwUnk11[3];				// 0xCC
	LPVOID * ptPacket;				// 0xD8
	DWORD dwUnk12;    				// 0xDC
	UnitAny * ptChangedNext;			// 0xE0
	UnitAny * ptRoomNext;			// 0xE4	
	UnitAny * ptListNext;      		// 0xE8
};

struct UnitItem : UnitAny
{};

#endif // __D2STRUCTS_H__