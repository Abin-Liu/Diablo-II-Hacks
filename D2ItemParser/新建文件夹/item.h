// item.h
// ackmed@gotwalls.com

#pragma once
#include <windows.h>
#pragma warning( disable : 4786 )
#include <list>
#include <string>
#include <map>

// defines
#define		ITEM_MESSAGEID_DROP				0x9C // drop/pick from belt/ground
#define		ITEM_MESSAGEID_OPERATE			0x9D // pick/equip
	
#define		ITEM_ACTION_NEW_GROUND			0x00 //9c
#define		ITEM_ACTION_PICK_UP				0x01 //9c
#define		ITEM_ACTION_DROP				0x02 //9c
#define		ITEM_ACTION_OLD_GROUND			0x03 //9c
#define		ITEM_ACTION_TO_STORAGE			0x04 //9c
#define		ITEM_ACTION_FROM_STORAGE		0x05 //9d
#define		ITEM_ACTION_TO_EQUIP			0x06 //9d
#define		ITEM_ACTION_FROM_EQUIP			0x08 //9d
#define		ITEM_ACTION_TO_STORE			0x0B //9c
#define		ITEM_ACTION_FROM_STORE			0x0C //9c
#define		ITEM_ACTION_TO_BELTSLOT			0x0E //9c
#define		ITEM_ACTION_FROM_BELTSLOT		0x0F //9c
	
#define		ITEM_ITEMTYPE_HELM				0x00
#define		ITEM_ITEMTYPE_ARMOR				0x01
#define		ITEM_ITEMTYPE_WEAPON			0x05
#define		ITEM_ITEMTYPE_BOW				0x06
#define		ITEM_ITEMTYPE_SHIELD			0x07
#define		ITEM_ITEMTYPE_EXPANSION			0x0A // heads
#define		ITEM_ITEMTYPE_OTHER				0x10

#define		ITEM_LOCAIION_STORED			0x00
#define		ITEM_LOCATION_WORN				0x01
#define		ITEM_LOCATION_BELT				0x02
#define		ITEM_LOCATION_GROUND			0x03
#define		ITEM_LOCATION_TOHAND			0x04

#define		ITEM_LEVEL_INFERIOR				0x01
#define		ITEM_LEVEL_NORMAL				0x02
#define		ITEM_LEVEL_SUPERIOR				0x03
#define		ITEM_LEVEL_MAGIC				0x04
#define		ITEM_LEVEL_SET					0x05
#define		ITEM_LEVEL_RARE					0x06
#define		ITEM_LEVEL_UNIQUE				0x07
#define		ITEM_LEVEL_CRAFT				0x08

#define		ITEM_INFERIORTYPE_CRUDE			0x00
#define		ITEM_INFERIORTYPE_CRACKED		0x01
#define		ITEM_INFERIORTYPE_DAMAGED		0x02
#define		ITEM_INFERIORTYPE_LOWQUALITY	0x04

#define		ITEM_SUPERIOR_AR				0x00 // attack rating
#define		ITEM_SUPERIOR_MAXDMG			0x01 // max damage
#define		ITEM_SUPERIOR_AC				0x02
#define		ITEM_SUPERIOR_AR_MAXDMG			0x03
#define		ITEM_SUPERIOR_DUR				0x04
#define		ITEM_SUPERIOR_DUR_AR			0x05
#define		ITEM_SUPERIOR_DUR_MAXDMG		0x06
#define		ITEM_SUPERIOR_DUR_AC			0x07

#define		ITEM_GOLDSIZE_12BIT				0x00
#define		ITEM_GOLDSIZE_32BIT				0x01

#define		ITEM_UNSET						0xFF


#define	ITEM_VALUE_NPC    (-1)
#define	ITEM_VALUE_UNKOWN 0
#define	ITEM_VALUE_USEFUL 1
#define	ITEM_VALUE_TRADEABLE 2
#define	ITEM_VALUE_COSTLY 3
#define	ITEM_VALUE_INVALUABLE 4

struct BitFields;
#define MAX_MOD 25

struct ITEM_MOD
{
	int id;
	int num;
	int value[4];
};

/*struct SINGLE_FILTER
{
	std::string exp;
	int Level;
	int code;
};
*/

#define TOKEN_TYPE_BAD -1
#define TOKEN_TYPE_UNSET 0
#define TOKEN_TYPE_OP 1
#define TOKEN_TYPE_NUM 2
#define TOKEN_TYPE_VAR 3

struct SINGLE_TOKEN
{
	int type;
	union
	{
	char var[33];
	int num;
	char op;
	};
};

class CD2Item
{
	//static std::map<std::string,DWORD> _mod_def;
	//static std::list<std::string> _filter;
	//static std::map<DWORD,SIZE> _item_size;
	
public:
	BOOL Parse9C(const BYTE* aPacket, DWORD aLen);
	const char* GetAToken(const char*,SINGLE_TOKEN&);
	static void RemoveBlanks(char *str);

	char * GetColorCode();
	BOOL IsThrowItem();

	BOOL IsPotion();
	BOOL ParseMods(BitFields &iPacket);


	int GoldAmount;

	BOOL	isSocketsFull;
	BOOL	isIdentified;
	BOOL	isEthereal;
	BOOL	isSwitchin;
	BOOL	isSwitchout;
	BOOL	isBroken;
	BOOL	fromBelt;
	BOOL	hasSockets;
	BOOL	isJustGenerated;
	BOOL	isStartitem;
	BOOL	isMiscItem;			// unverified
	BOOL	isPersonalized;
	BOOL	isGamble;
	BOOL	isRuneWord;			// unverified
	BOOL	isMagicExtra;		// unverified

	BOOL	isGold;
	BOOL	isEar;

	DWORD id;
	BYTE ItemType;
	BYTE action;
	union 
	{
		char code[5];
		DWORD code_id;
	};
	BYTE GemedNumber;
	BYTE Ilvl;
	BYTE ItemQuality;
//	BOOL isSpecialClassItem;
	
//	MAGICPREFIXSTRUCT *MagicPrefix;
//	MAGICSUFFIXSTRUCT *MagicSuffix;
	union
	{
		struct
		{
			WORD MagicPrefix;
			WORD MagicSuffix;
		};
		struct
		{
			WORD RarePrefix;
			WORD RareSuffix;
		};
		WORD UniqueId;
		WORD SetId;
		WORD RuneWordId;
	};

//	BOOL isCharmRingAmu;		// unverified
//	DWORD SellValue;

	WORD nDefense; //for armor
	
	union
	{
		struct
		{
			WORD nDur;
			WORD nMaxDur;
		};
		WORD nQuantity;
	};
	WORD nSocket; 

	WORD pos_x;
	WORD pos_y;
	BYTE storage_id;
	
	int mod_num;
	ITEM_MOD mod[MAX_MOD];
	BOOL isUselessItem;

	BOOL FromPacket(BYTE* aPacket, DWORD aLen,BOOL identify=FALSE);

	//int sta_name[5];
	//int sta_value[5];
	//int nKeyValue;
	
private:
	int OperatorPriority(char op);
protected:
	BOOL Parse9D(const BYTE* aPacket, DWORD aLen);
};

DWORD GetBitField(const BYTE* data, DWORD pos, DWORD len, DWORD max);
//DWORD GetBitsAt(BYTE* data, int max, int nPos, int nLength);

// from mousepad with some small mods
struct BitFields {
	const BYTE *data;
	DWORD pos;
	DWORD max;
	BitFields(const BYTE *d, DWORD mx) {
		data = d; 
		pos = 0; 
		max = (mx * 8);  // convert to max bits
	}
	DWORD GetField(DWORD len) {
		return GetBitField(data, (pos+=len)-len, len, max);
	}
};
