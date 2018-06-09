//////////////////////////////////////////////////////////////////
//   D2Skills.h
//
// Skill ID value definition
// CD2Skill class definition
// CD2Character class definition
// CD2Action class definition
// CD2Scheme class definition
//
// Copyright Abin (Bin Liu)
// All rights reserved.
// 9/19/2002
//
//////////////////////////////////////////////////////////////////

#ifndef __D2SKILLS_H__
#define __D2SKILLS_H__

// "DSD" stands for "Diablo II Skill Definition"

// Common Skills 
#define DSD_ATTACK				0x0000
#define DSD_THROW				0x0002

// Invalid indicator
#define DSD_INVALID				0xffff

///////////////////////////////////////////////////////////////
//    Amazon Skills
///////////////////////////////////////////////////////////////

// Javelin and Spear Skills
#define DSD_JAB					0x000a
#define DSD_IMPALE				0x0013
#define DSD_FEND				0x001e
#define DSD_POWERSTRIKE			0x000e
#define DSD_CHARGEDSTRIKE		0x0018
#define DSD_LIGHTNINGSTRIKE		0x0022
#define DSD_POISONJAVELIN		0x000f
#define DSD_LIGHTNINGBOLT		0x0014
#define DSD_PLAGUEJAVELIN		0x0019
#define DSD_LIGHTNINGFURY		0x0023

// Passive and Magic Skills
#define DSD_INNERSIGHT			0x0008
#define DSD_SLOWMISSILES		0x0011
#define DSD_DECOY				0x001c
#define DSD_VALKYRIE			0x0020
#define DSD_DODGE				0x000d
#define DSD_AVOID				0x0012
#define DSD_EVADE				0x001d
#define DSD_CRITICALSTRIKE		0x0009
#define DSD_PENETRATE			0x0017
#define DSD_PIERCE				0x0021

// Bow and Crossbow Skills
#define DSD_COLDARROW			0x000b
#define DSD_ICEARROW			0x0015
#define DSD_FREEZINGARROW		0x001f
#define DSD_MAGICARROW			0x0006
#define DSD_MULTIPLESHOT		0x000c
#define DSD_GUIDEDARROW			0x0016
#define DSD_STRAFE				0x001a
#define DSD_FIREARROW			0x0007
#define DSD_EXPLODINGARROW		0x0010
#define DSD_IMMOLATIONARROW		0x001b


///////////////////////////////////////////////////////////////
//    Assassin Skills
///////////////////////////////////////////////////////////////

// Martial Arts
#define DSD_FISTSOFFIRE			0x0103
#define DSD_CLAWSOFTHUNDER		0x010d
#define DSD_BLADESOFICE			0x0112
#define DSD_TIGERSTRIKE			0x00fe
#define DSD_COBRASTRIKE			0x0109
#define DSD_PHOENIXSTRIKE		0x0118
#define DSD_DRAGONTALON			0x00ff
#define DSD_DRAGONCLAW			0x0104
#define DSD_DRAGONTAIL			0x010e
#define DSD_DRAGONFLIGHT		0x0113

// Shadow Disciplines
#define DSD_BURSTOFSPEED		0x0102
#define DSD_FADE				0x010b
#define DSD_VENOM				0x0116
#define DSD_CLAWMASTERY			0x00fc
#define DSD_WEAPONBLOCK			0x0107
#define DSD_SHADOWWARRIOR		0x010c
#define DSD_SHADOWMASTER		0x0117
#define DSD_PSYCHICHAMMER		0x00fd
#define DSD_CLOAKOFSHADOWS		0x0108
#define DSD_MINDBLAST			0x0111

// Traps
#define DSD_SHOCKWEB			0x0100
#define DSD_CHARGEDBOLTSENTRY	0x0105
#define DSD_LIGHTNINGSENTRY		0x010f
#define DSD_DEATHSENTRY			0x0114
#define DSD_FIREBLAST			0x00fb
#define DSD_WAKEOFFIRE			0x0106
#define DSD_WAKEOFINFERNO		0x0110
#define DSD_BLADESENTINEL		0x0101
#define DSD_BLADEFURY			0x010a
#define DSD_BLADESHIELD			0x0115


///////////////////////////////////////////////////////////////
//    Barbarian Skills
///////////////////////////////////////////////////////////////

// Warcries
#define DSD_HOWL				0x0082
#define DSD_TAUNT				0x0089
#define DSD_BATTLECRY			0x0092
#define DSD_WARCRY				0x009a
#define DSD_SHOUT				0x008a
#define DSD_BATTLEORDERS		0x0095
#define DSD_BATTLECOMMAND		0x009b
#define DSD_FINDPOTION			0x0083
#define DSD_FINDITEM			0x008e
#define DSD_GRIMWARD			0x0096

// Combat Masteries
#define DSD_SWORDMASTERY		0x007f
#define DSD_POLEARMMASTERY		0x0086
#define DSD_INCREASEDSTAMINA	0x008d
#define DSD_INCREASEDSPEED		0x0094
#define DSD_AXEMASTERY			0x0080
#define DSD_THROWINGMASTERY		0x0087
#define DSD_MACEMASTERY			0x0081
#define DSD_SPEARMASTERY		0x0088
#define DSD_IRONSKIN			0x0091
#define DSD_NATURALRESISTANCE	0x0099

// Combat Skills
#define DSD_LEAP				0x0084
#define DSD_LEAPATTACK			0x008f
#define DSD_WHIRLWIND			0x0097
#define DSD_BASH				0x007e
#define DSD_STUN				0x008b
#define DSD_CONCENTRATE			0x0090
#define DSD_BERSERK				0x0098
#define DSD_DOUBLESWING			0x0085
#define DSD_DOUBLETHROW			0x008c
#define DSD_FRENZY				0x0093


///////////////////////////////////////////////////////////////
//    Druid Skills
///////////////////////////////////////////////////////////////

// Elemental
#define DSD_FIRESTORM			0x00e1
#define DSD_MOLTENBOULDER		0x00e5
#define DSD_FISSURE				0x00ea
#define DSD_VOLCANO				0x00f4
#define DSD_ARMAGEDDON			0x00f9
#define DSD_TWISTER				0x00f0
#define DSD_TORNADO				0x00f5
#define DSD_HURRICANE			0x00fa
#define DSD_ARCTICBLAST			0x00e6
#define DSD_CYCLONEARMOR		0x00eb

// Shape Shifting
#define DSD_WEREWOLF			0x00df
#define DSD_FERALRAGE			0x00e8
#define DSD_RABIES				0x00ee
#define DSD_FURY				0x00f8
#define DSD_LYCANTHROPY			0x00e0
#define DSD_FIRECLAWS			0x00ef
#define DSD_HUNGER				0x00f2
#define DSD_WEREBEAR			0x00e4
#define DSD_MAUL				0x00e9
#define DSD_SHOCKWAVE			0x00f3

// Summoning
#define DSD_OAKSAGE				0x00e2
#define DSD_HEARTOFWOLVERINE	0x00ec
#define DSD_SPIRITOFBARBS		0x00f6
#define DSD_RAVEN				0x00dd
#define DSD_SUMMONSPIRITWOLF	0x00e3
#define DSD_SUMMONDIREWOLF		0x00ed
#define DSD_SUMMONGRIZZLY		0x00f7
#define DSD_POISONCREEPER		0x00de
#define DSD_CARRIONVINE			0x00e7
#define DSD_SOLARCREEPER		0x00f1


///////////////////////////////////////////////////////////////
//    Necromancer Skills
///////////////////////////////////////////////////////////////

// Summoning Spells
#define DSD_SKELETONMASTERY		0x0045
#define DSD_GOLEMMASTERY		0x004f
#define DSD_SUMMONRESIST		0x0059
#define DSD_CLAYGOLEM			0x004b
#define DSD_BLOODGOLEM			0x0055
#define DSD_IRONGOLEM			0x005a
#define DSD_FIREGOLEM			0x005e
#define DSD_RAISESKELETON		0x0046
#define DSD_RAISESKELETALMAGE	0x0050
#define DSD_REVIVE				0x005f

// Poison and Bone Spells
#define DSD_POISONDAGGER		0x0049
#define DSD_POISONEXPLOSION		0x0053
#define DSD_POISONNOVA			0x005c
#define DSD_TEETH				0x0043
#define DSD_CORPOSEEXPLOSION	0x004a
#define DSD_BONESPEAR			0x0054
#define DSD_BONESPIRIT			0x005d
#define DSD_BONEARMOR			0x0044
#define DSD_BONEWALL			0x004e
#define DSD_BONEPRISON			0x0058

// Curses
#define DSD_DIMVISION			0x0047
#define DSD_CONFUSE				0x0051
#define DSD_ATTRACT				0x0056
#define DSD_AMPLIFYDAMAGE		0x0042
#define DSD_IRONMAIDEN			0x004c
#define DSD_LIFETAP				0x0052
#define DSD_LOWERRESIST			0x005b
#define DSD_WEAKEN				0x0048
#define DSD_TERROR				0x004d
#define DSD_DECREPIFY			0x0057


///////////////////////////////////////////////////////////////
//    Paladin Skills
///////////////////////////////////////////////////////////////

// Defensive Auras
#define DSD_PRAYER				0x0063
#define DSD_CLEANSING			0x006d
#define DSD_MEDITATION			0x0078
#define DSD_DEFIANCE			0x0068
#define DSD_VIGOR				0x0073
#define DSD_REDEMPTION			0x007c
#define DSD_RESISTFIRE			0x0064
#define DSD_RESISTCOLD			0x0069
#define DSD_RESISTLIGHTNING		0x006e
#define DSD_SALVATION			0x007d

// Offensive Auras
#define DSD_MIGHT				0x0062
#define DSD_BLESSEDAIM			0x006c
#define DSD_CONCENTRATION		0x0071
#define DSD_FANATICISM			0x007a
#define DSD_HOLYFIRE			0x0066
#define DSD_HOLYFREEZE			0x0072
#define DSD_HOLYSHOCK			0x0076
#define DSD_THORNS				0x0067
#define DSD_SANCTUARY			0x0077
#define DSD_CONVICTION			0x007b

// Combat Skills
#define DSD_SACRIFICE			0x0060
#define DSD_ZEAL				0x006a
#define DSD_VENGEANCE			0x006f
#define DSD_CONVERSION			0x0074
#define DSD_HOLYBOLT			0x0065
#define DSD_BLESSEDHAMMER		0x0070
#define DSD_FISTOFTHEHEAVENS	0x0079
#define DSD_SMITE				0x0061
#define DSD_CHARGE				0x006b
#define DSD_HOLYSHIELD			0x0075


///////////////////////////////////////////////////////////////
//    Sorceress Skills
///////////////////////////////////////////////////////////////

// Cold Spells
#define DSD_FROSTNOVA			0x002c
#define DSD_BLIZZARD			0x003b
#define DSD_FROZENORB			0x0040
#define DSD_ICEBOLT				0x0027
#define DSD_ICEBLAST			0x002d
#define DSD_GLACIALSPIKE		0x0037
#define DSD_COLDMASTERY			0x0041
#define DSD_FROZENARMOR			0x0028
#define DSD_SHIVERARMOR			0x0032
#define DSD_CHILLINGARMOR		0x003c

// Lightning Spells
#define DSD_STATICFIELD			0x002a
#define DSD_NOVA				0x0030
#define DSD_THUNDERSTORM		0x0039
#define DSD_CHARGEDBOLT			0x0026
#define DSD_LIGHTNING			0x0031
#define DSD_CHAINLIGHTNING		0x0035
#define DSD_LIGHTNINGMASTERY	0x003f
#define DSD_TELEKINESIS			0x002b
#define DSD_TELEPORT			0x0036
#define DSD_ENERGYSHIELD		0x003a

// Fire Spells
#define DSD_INFERNO				0x0029
#define DSD_BLAZE				0x002e
#define DSD_FIREWALL			0x0033
#define DSD_FIREBOLT			0x0024
#define DSD_FIREBALL			0x002f
#define DSD_METEOR				0x0038
#define DSD_FIREMASTERY			0x003d
#define DSD_WARMTH				0x0025
#define DSD_ENCHANT				0x0034
#define DSD_HYDRA				0x003e


//////////////////////////////////////////////////////////////
// CD2Skill Class Definition
//////////////////////////////////////////////////////////////

#include <windows.h>

#define _MAX_SKILLNAME	32 // Maximum length of a skill name
class CD2Skill
{
public:
	CD2Skill();
	CD2Skill(WORD wSkillID, WORD wCoolDown, BOOL bPassive, BOOL bRightOnly, BOOL bTargetable, BOOL bCorposeSkill, BOOL bEnableInTown, LPCSTR lpSkillName);
	~CD2Skill();

	void SetSkill(WORD wSkillID, WORD wCoolDown, BOOL bPassive, BOOL bRightOnly, BOOL bTargetable, BOOL bCorposeSkill, BOOL bEnableInTown, LPCSTR lpSkillName);
	WORD GetSkillID() const;
	WORD GetCoolDown() const;
	BOOL IsPassive() const;
	BOOL IsRightOnly() const;
	BOOL IsTargetable() const;
	BOOL IsCorposeSkill() const;
	BOOL IsEnableInTown() const;
	LPCSTR GetSkillName() const;

	BOOL operator==(const CD2Skill& rhs) const;
	const CD2Skill& operator=(const CD2Skill& rhs);
	BOOL IsNameSame(LPCSTR lpString) const;
	BOOL IsIDSame(WORD wSkillID) const;

private:
	WORD m_wSkillID; // Skill ID value
	WORD m_wCoolDown; // Spell cool down time (built-in delay) in milliseconds
	BOOL m_bPassive; // Whether the skill is a passive skill
	BOOL m_bRightOnly; // Some spells can only be set on right side, like paladin auras
	BOOL m_bTargetable; // Some spells cannot be targeted, like Nova
	BOOL m_bCorposeSkill; // Whether the skill requires corposes, like Revive
	BOOL m_bEnableInTown; // Whether the skill is enabled in town
	char* m_pszSkillName; // Skill name to be displayed	
};


//////////////////////////////////////////////////////////////
// Character Class Definition
//////////////////////////////////////////////////////////////

// Character class ID values
#define CCD_AMAZON				0
#define CCD_ASSASSIN			1
#define CCD_BARBARIAN			2
#define CCD_DRUID				3
#define CCD_NECROMANCER			4
#define CCD_PALADIN				5
#define CCD_SORCERESS			6
#define CCD_UNKNOWN				7

// Character class string representation
const char* const CHARCLASSNAME[] =
 { "Amazon", "Assassin", "Barbarian", "Druid", "Necromancer", "Paladin", "Sorceress" }; 

//////////////////////////////////////////////////////////////
// CD2Character Class Definition
//////////////////////////////////////////////////////////////

#define _SKILLNUMBER	32 // number of available skills for each character class, unsommon excluded.
#define _CHARNAME		15 // Max length of character name
class CD2Character
{
public:
	LPCSTR GetClassName() const;
	CD2Character();
	CD2Character(LPCSTR lpName, BYTE byCharClass);
	~CD2Character();

	void SetCharClass(LPCSTR lpName, BYTE byCharClass);
	const CD2Skill* GetSkillArray() const;
	const CD2Skill* FindSkill(LPCSTR lpSkillName) const;
	const CD2Skill* FindSkill(WORD wSkillID) const;
	BYTE GetCharClass() const;
	LPCSTR GetCharName() const;

private:
	char m_szCharName[_CHARNAME + 1]; // Character Name
	char m_szClassName[12]; // Class name, Druid, Amazon, Paladin, etc
	BYTE m_byCharClass; // Character class, Amazon, Assassin, Barbarian, etc
	CD2Skill m_aSkills[_SKILLNUMBER]; // Array of skills for current character class

	void InitialSkills(); // Initialize skill array, huge work...
};


//////////////////////////////////////////////////////////////
// CD2Action Class Definition
//////////////////////////////////////////////////////////////

// Skill assign side
#define SAS_LEFT			0
#define SAS_RIGHT			1

// Casting aim target
#define CAT_NONE				0
#define CAT_PINDLESKIN			1
#define CAT_BEYONDPINDLESKIN	2
#define CAT_BEFOREPINDLESKIN	3

class CD2Action
{
public:
	void SetDelay(WORD wDelay);
	void GetTargetText(LPSTR lpsz) const;
	void GetSideText(LPSTR lpsz) const;
	BOOL IsEnabled() const;
	void Enable(BOOL bEnable);
	void ToString(LPSTR lpString) const;
	BOOL ParseString(LPCSTR lpString);
	WORD GetRepeat() const;
	WORD GetDelay() const;
	int GetTarget() const;
	int GetSide() const;
	WORD GetSkillID() const;
	const CD2Action& operator=(const CD2Action& rhs);
	void SetAction(WORD wSkillID, int nSide, int nTarget, WORD wDelay, WORD wRepeat);
	CD2Action(WORD wSkillID, int nSide, int nTarget, WORD wDelay, WORD wRepeat);
	CD2Action();

private:
	WORD m_wSkillID; // The skill ID
	int m_nSide; // Set skill on left/right side
	int m_nTarget; // Target, 0-none, 1-pindleskin, 2-behind pindleskin, 3-before pindleskin
	WORD m_wDelay; // Delay between 2 casting
	WORD m_wRepeat; // Repeat this action x times
	BOOL m_bEnabled; // Action enabled/disabled
};

#endif // __D2SKILLS_H__