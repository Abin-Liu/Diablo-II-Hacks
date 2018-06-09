//////////////////////////////////////////////////////////////////
//   D2Skills.cpp
//
// Implementation of CD2Skill class and CD2Character class.
//
// Copyright Abin (Bin Liu)
// All rights reserved.
// 9/19/2002
//
//////////////////////////////////////////////////////////////////

//#include "stdafx.h" // only include if used in MFC applications
#include "D2Skills.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////
// Implementation of CD2Skill class
//////////////////////////////////////////////////////////////////

CD2Skill::CD2Skill()
{
	m_wSkillID = 0x00;
	m_wCoolDown = 0;
	m_bPassive = FALSE;
	m_bRightOnly = FALSE;
	m_bTargetable = FALSE;
	m_bCorposeSkill = FALSE;
	m_bEnableInTown = FALSE;
	m_pszSkillName = new char[_MAX_SKILLNAME + 1];
	::memset(m_pszSkillName, 0, _MAX_SKILLNAME + 1);
}

CD2Skill::CD2Skill(WORD wSkillID, WORD wCoolDown, BOOL bPassive, BOOL bRightOnly, BOOL bTargetable, BOOL bCorposeSkill, BOOL bEnableInTown, LPCSTR lpSkillName)
{
	m_wSkillID = wSkillID;
	m_wCoolDown = wCoolDown;
	m_bPassive = bPassive;
	m_bRightOnly = bRightOnly;
	m_bTargetable = bTargetable;
	m_bCorposeSkill = bCorposeSkill;
	m_bEnableInTown = bEnableInTown;
	m_pszSkillName = new char[_MAX_SKILLNAME + 1];
	::memset(m_pszSkillName, 0, _MAX_SKILLNAME + 1);
	::strncpy(m_pszSkillName, lpSkillName, _MAX_SKILLNAME);
}

CD2Skill::~CD2Skill()
{
	delete [] m_pszSkillName;
}

void CD2Skill::SetSkill(WORD wSkillID, WORD wCoolDown, BOOL bPassive, BOOL bRightOnly, BOOL bTargetable, BOOL bCorposeSkill, BOOL bEnableInTown, LPCSTR lpSkillName)
{
	m_wSkillID = wSkillID;
	m_wCoolDown = wCoolDown;
	m_bPassive = bPassive;
	m_bRightOnly = bRightOnly;
	m_bTargetable = bTargetable;
	m_bCorposeSkill = bCorposeSkill;
	m_bEnableInTown = bEnableInTown;
	::memset(m_pszSkillName, 0, _MAX_SKILLNAME + 1);
	::strncpy(m_pszSkillName, lpSkillName, _MAX_SKILLNAME);
}

WORD CD2Skill::GetSkillID() const
{
	return m_wSkillID;
}

WORD CD2Skill::GetCoolDown() const
{
	return m_wCoolDown;
}

BOOL CD2Skill::IsPassive() const
{
	return m_bPassive;
}

BOOL CD2Skill::IsRightOnly() const
{
	return m_bRightOnly;
}

BOOL CD2Skill::IsTargetable() const
{
	return m_bTargetable;
}

BOOL CD2Skill::IsCorposeSkill() const
{
	return m_bCorposeSkill;
}

BOOL CD2Skill::IsEnableInTown() const
{
	return m_bEnableInTown;
}

LPCSTR CD2Skill::GetSkillName() const
{
	return m_pszSkillName; 
}

BOOL CD2Skill::operator==(const CD2Skill& rhs) const
{
	return m_wSkillID == rhs.m_wSkillID;
}

const CD2Skill& CD2Skill::operator=(const CD2Skill& rhs)
{
	m_wSkillID = rhs.m_wSkillID;
	m_wCoolDown = rhs.m_wCoolDown;
	m_bPassive = rhs.m_bPassive;
	m_bRightOnly = rhs.m_bRightOnly;
	m_bTargetable = rhs.m_bTargetable;
	m_bCorposeSkill = rhs.m_bCorposeSkill;
	m_bEnableInTown = rhs.m_bEnableInTown;
	::memset(m_pszSkillName, 0, _MAX_SKILLNAME + 1);
	::strncpy(m_pszSkillName, rhs.m_pszSkillName, _MAX_SKILLNAME);
	return *this;
}

BOOL CD2Skill::IsNameSame(LPCSTR lpString) const
{
	return (stricmp(m_pszSkillName, lpString) == 0);
}

BOOL CD2Skill::IsIDSame(WORD wSkillID) const
{
	return m_wSkillID == wSkillID;
}


//////////////////////////////////////////////////////////////////
// Implementation of CD2Skill class
//////////////////////////////////////////////////////////////////

CD2Character::CD2Character()
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	::memset(m_szClassName, 0, 12);
	m_byCharClass = CCD_UNKNOWN;
}

CD2Character::CD2Character(LPCSTR lpName, BYTE byCharClass)
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	strncpy(m_szCharName, lpName, _CHARNAME);
	::memset(m_szClassName, 0, 12);
	m_byCharClass = byCharClass;
	if (m_byCharClass < 7)
		strcpy(m_szClassName, CHARCLASSNAME[m_byCharClass]);
	InitialSkills();
}

CD2Character::~CD2Character()
{
}

void CD2Character::SetCharClass(LPCSTR lpName, BYTE byCharClass)
{
	::memset(m_szCharName, 0, _CHARNAME + 1);
	strncpy(m_szCharName, lpName, _CHARNAME);
	::memset(m_szClassName, 0, 12);
	m_byCharClass = byCharClass;
	if (m_byCharClass < 7)
		strcpy(m_szClassName, CHARCLASSNAME[m_byCharClass]);
	InitialSkills();
}

const CD2Skill* CD2Character::GetSkillArray() const
{
	return m_aSkills;
}

// Initialize skill list(32 skills in total) for current character class.
// Each class have 32 skills(2 common skills and 30 class specified skills).
void CD2Character::InitialSkills()
{
	int nIndex = 0;

	// Common spells (melee and throwing)
	m_aSkills[nIndex++].SetSkill(DSD_ATTACK, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Attack");
	m_aSkills[nIndex++].SetSkill(DSD_THROW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Throw");

	switch (m_byCharClass)
	{
	case CCD_AMAZON:

		// Javelin and Spear Skills
		m_aSkills[nIndex++].SetSkill(DSD_JAB, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Jab");
		m_aSkills[nIndex++].SetSkill(DSD_IMPALE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Impale");
		m_aSkills[nIndex++].SetSkill(DSD_FEND, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Fend");
		m_aSkills[nIndex++].SetSkill(DSD_POWERSTRIKE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Power Strike");
		m_aSkills[nIndex++].SetSkill(DSD_CHARGEDSTRIKE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Charged Strike");
		m_aSkills[nIndex++].SetSkill(DSD_LIGHTNINGSTRIKE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Lightning Strike");
		m_aSkills[nIndex++].SetSkill(DSD_POISONJAVELIN, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Poison Javelin");
		m_aSkills[nIndex++].SetSkill(DSD_LIGHTNINGBOLT, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Lightning Bolt");
		m_aSkills[nIndex++].SetSkill(DSD_PLAGUEJAVELIN, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Plague Javelin");
		m_aSkills[nIndex++].SetSkill(DSD_LIGHTNINGFURY, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Lightning Fury");

		// Passive and Magic Skills
		m_aSkills[nIndex++].SetSkill(DSD_INNERSIGHT, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Inner Sight");
		m_aSkills[nIndex++].SetSkill(DSD_SLOWMISSILES, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Slow Missiles");
		m_aSkills[nIndex++].SetSkill(DSD_DECOY, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Decoy");
		m_aSkills[nIndex++].SetSkill(DSD_VALKYRIE, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Valkyrie");
		m_aSkills[nIndex++].SetSkill(DSD_DODGE, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Dodge");
		m_aSkills[nIndex++].SetSkill(DSD_AVOID, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Avoid");
		m_aSkills[nIndex++].SetSkill(DSD_EVADE, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Evade");
		m_aSkills[nIndex++].SetSkill(DSD_CRITICALSTRIKE, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Critical Strike");
		m_aSkills[nIndex++].SetSkill(DSD_PENETRATE, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Penetrate");
		m_aSkills[nIndex++].SetSkill(DSD_PIERCE, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Pierce");

		// Bow and Crossbow Skills
		m_aSkills[nIndex++].SetSkill(DSD_COLDARROW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Cold Arrow");
		m_aSkills[nIndex++].SetSkill(DSD_ICEARROW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Ice Arrow");
		m_aSkills[nIndex++].SetSkill(DSD_FREEZINGARROW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Freezing Arrow");
		m_aSkills[nIndex++].SetSkill(DSD_MAGICARROW	, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Magic Arrow");
		m_aSkills[nIndex++].SetSkill(DSD_MULTIPLESHOT, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Multiple Shot");
		m_aSkills[nIndex++].SetSkill(DSD_GUIDEDARROW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Guided Arrow");
		m_aSkills[nIndex++].SetSkill(DSD_STRAFE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Strafe");
		m_aSkills[nIndex++].SetSkill(DSD_FIREARROW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Fire Arrow");
		m_aSkills[nIndex++].SetSkill(DSD_EXPLODINGARROW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Exploding Arrow");
		m_aSkills[nIndex++].SetSkill(DSD_IMMOLATIONARROW, 1000, FALSE, FALSE, TRUE, FALSE, FALSE, "Immolation Arrow");
		break;

	case CCD_ASSASSIN:

		// Martial Arts
		m_aSkills[nIndex++].SetSkill(DSD_FISTSOFFIRE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Fists of Fire");
		m_aSkills[nIndex++].SetSkill(DSD_CLAWSOFTHUNDER, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Claws of Thunder");
		m_aSkills[nIndex++].SetSkill(DSD_BLADESOFICE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Blades of Ice");
		m_aSkills[nIndex++].SetSkill(DSD_TIGERSTRIKE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Tiger Strike");
		m_aSkills[nIndex++].SetSkill(DSD_COBRASTRIKE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Cobra Strike");
		m_aSkills[nIndex++].SetSkill(DSD_PHOENIXSTRIKE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Phoenix Strike");
		m_aSkills[nIndex++].SetSkill(DSD_DRAGONTALON, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Dragon Talon");
		m_aSkills[nIndex++].SetSkill(DSD_DRAGONCLAW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Dragon Claw");
		m_aSkills[nIndex++].SetSkill(DSD_DRAGONTAIL, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Dragon Tail");
		m_aSkills[nIndex++].SetSkill(DSD_DRAGONFLIGHT, 2000, FALSE, FALSE, TRUE, FALSE, FALSE, "Dragon Flight");

		// Shadow Disciplines
		m_aSkills[nIndex++].SetSkill(DSD_BURSTOFSPEED, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Burst of Speed");
		m_aSkills[nIndex++].SetSkill(DSD_FADE, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Fade");
		m_aSkills[nIndex++].SetSkill(DSD_VENOM, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Venom");
		m_aSkills[nIndex++].SetSkill(DSD_CLAWMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Claw Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_WEAPONBLOCK, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Weapon Block");
		m_aSkills[nIndex++].SetSkill(DSD_SHADOWWARRIOR, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Shadow Warrior");
		m_aSkills[nIndex++].SetSkill(DSD_SHADOWMASTER, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Shadow Master");
		m_aSkills[nIndex++].SetSkill(DSD_PSYCHICHAMMER, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Psychic Hammer");
		m_aSkills[nIndex++].SetSkill(DSD_CLOAKOFSHADOWS, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Cloak of Shadows");
		m_aSkills[nIndex++].SetSkill(DSD_MINDBLAST, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Mind Blast");

		// Traps
		m_aSkills[nIndex++].SetSkill(DSD_SHOCKWEB, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Shock Web");
		m_aSkills[nIndex++].SetSkill(DSD_CHARGEDBOLTSENTRY, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Charged Bolt Sentry");
		m_aSkills[nIndex++].SetSkill(DSD_LIGHTNINGSENTRY, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Lightning Sentry");
		m_aSkills[nIndex++].SetSkill(DSD_DEATHSENTRY, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Death Sentry");
		m_aSkills[nIndex++].SetSkill(DSD_FIREBLAST, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Fire Blast");
		m_aSkills[nIndex++].SetSkill(DSD_WAKEOFFIRE, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Wake of Fire");
		m_aSkills[nIndex++].SetSkill(DSD_WAKEOFINFERNO, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Wake of Inferno");
		m_aSkills[nIndex++].SetSkill(DSD_BLADESENTINEL, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Blade Sentinel");
		m_aSkills[nIndex++].SetSkill(DSD_BLADEFURY, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Blade Fury");
		m_aSkills[nIndex++].SetSkill(DSD_BLADESHIELD, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Blade Shield");
		break;

	case CCD_BARBARIAN:

		// Warcries
		m_aSkills[nIndex++].SetSkill(DSD_HOWL, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Howl");
		m_aSkills[nIndex++].SetSkill(DSD_TAUNT, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Taunt");
		m_aSkills[nIndex++].SetSkill(DSD_BATTLECRY, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Battle Cry");
		m_aSkills[nIndex++].SetSkill(DSD_WARCRY, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "War Cry");
		m_aSkills[nIndex++].SetSkill(DSD_SHOUT, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Shout");
		m_aSkills[nIndex++].SetSkill(DSD_BATTLEORDERS, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Battle Orders");
		m_aSkills[nIndex++].SetSkill(DSD_BATTLECOMMAND, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Battle Command");
		m_aSkills[nIndex++].SetSkill(DSD_FINDPOTION, 0, FALSE, TRUE, TRUE, TRUE, FALSE, "Find Potion");
		m_aSkills[nIndex++].SetSkill(DSD_FINDITEM, 0, FALSE, TRUE, TRUE, TRUE, FALSE, "Find Item");
		m_aSkills[nIndex++].SetSkill(DSD_GRIMWARD, 0, FALSE, TRUE, TRUE, TRUE, FALSE, "Grim Ward");

		// Combat Masteries
		m_aSkills[nIndex++].SetSkill(DSD_SWORDMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Sword Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_POLEARMMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Pole Arm Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_INCREASEDSTAMINA, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Increased Stamina");
		m_aSkills[nIndex++].SetSkill(DSD_INCREASEDSPEED, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Increased Speed");
		m_aSkills[nIndex++].SetSkill(DSD_AXEMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Axe Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_THROWINGMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Throwing Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_MACEMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Mace Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_SPEARMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Spear Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_IRONSKIN, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Iron Skin");
		m_aSkills[nIndex++].SetSkill(DSD_NATURALRESISTANCE, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Natural Resistance");

		// Combat Skills
		m_aSkills[nIndex++].SetSkill(DSD_LEAP, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Leap");
		m_aSkills[nIndex++].SetSkill(DSD_LEAPATTACK, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Leap Attack");
		m_aSkills[nIndex++].SetSkill(DSD_WHIRLWIND, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Whirlwind");
		m_aSkills[nIndex++].SetSkill(DSD_BASH, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Bash");
		m_aSkills[nIndex++].SetSkill(DSD_STUN, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Stun");
		m_aSkills[nIndex++].SetSkill(DSD_CONCENTRATE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Concentrate");
		m_aSkills[nIndex++].SetSkill(DSD_BERSERK, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Berserk");
		m_aSkills[nIndex++].SetSkill(DSD_DOUBLESWING, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Double Swing");
		m_aSkills[nIndex++].SetSkill(DSD_DOUBLETHROW, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Double Throw");
		m_aSkills[nIndex++].SetSkill(DSD_FRENZY, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Frenzy");
		break;

	case CCD_DRUID:

		// Elemental
		m_aSkills[nIndex++].SetSkill(DSD_FIRESTORM, 600, FALSE, FALSE, TRUE, FALSE, FALSE, "Firestorm");
		m_aSkills[nIndex++].SetSkill(DSD_MOLTENBOULDER, 2000, FALSE, FALSE, TRUE, FALSE, FALSE, "Molten Boulder");
		m_aSkills[nIndex++].SetSkill(DSD_FISSURE, 2000, FALSE, TRUE, TRUE, FALSE, FALSE, "Fissure");
		m_aSkills[nIndex++].SetSkill(DSD_VOLCANO, 4000, FALSE, TRUE, TRUE, FALSE, FALSE, "Volcano");
		m_aSkills[nIndex++].SetSkill(DSD_ARMAGEDDON, 6000, FALSE, TRUE, FALSE, FALSE, FALSE, "Armageddon");
		m_aSkills[nIndex++].SetSkill(DSD_TWISTER, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Twister");
		m_aSkills[nIndex++].SetSkill(DSD_TORNADO, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Tornado");
		m_aSkills[nIndex++].SetSkill(DSD_HURRICANE, 6000, FALSE, TRUE, FALSE, FALSE, FALSE, "Hurricane");
		m_aSkills[nIndex++].SetSkill(DSD_ARCTICBLAST, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Arctic Blast");
		m_aSkills[nIndex++].SetSkill(DSD_CYCLONEARMOR, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Cyclone Armor");

		// Shape Shifting
		m_aSkills[nIndex++].SetSkill(DSD_WEREWOLF, 1000, FALSE, TRUE, FALSE, FALSE, TRUE, "Werewolf");
		m_aSkills[nIndex++].SetSkill(DSD_FERALRAGE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Feral Rage");
		m_aSkills[nIndex++].SetSkill(DSD_RABIES, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Rabies");
		m_aSkills[nIndex++].SetSkill(DSD_FURY, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Fury");
		m_aSkills[nIndex++].SetSkill(DSD_LYCANTHROPY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Lycanthropy");
		m_aSkills[nIndex++].SetSkill(DSD_FIRECLAWS, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Fire Claws");
		m_aSkills[nIndex++].SetSkill(DSD_HUNGER, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Hunger");
		m_aSkills[nIndex++].SetSkill(DSD_WEREBEAR, 1000, FALSE, TRUE, FALSE, FALSE, TRUE, "Werebear");
		m_aSkills[nIndex++].SetSkill(DSD_MAUL, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Maul");
		m_aSkills[nIndex++].SetSkill(DSD_SHOCKWAVE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Shock Wave");

		// Summoning
		m_aSkills[nIndex++].SetSkill(DSD_OAKSAGE, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Oak Sage");
		m_aSkills[nIndex++].SetSkill(DSD_HEARTOFWOLVERINE, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Heart of Wolverine");
		m_aSkills[nIndex++].SetSkill(DSD_SPIRITOFBARBS, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Spirit of Barbs");
		m_aSkills[nIndex++].SetSkill(DSD_RAVEN, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Raven");
		m_aSkills[nIndex++].SetSkill(DSD_SUMMONSPIRITWOLF, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Summon Spirit Wolf");
		m_aSkills[nIndex++].SetSkill(DSD_SUMMONDIREWOLF, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Summon Dire Wolf");
		m_aSkills[nIndex++].SetSkill(DSD_SUMMONGRIZZLY, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Summon Grizzly");
		m_aSkills[nIndex++].SetSkill(DSD_POISONCREEPER, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Poison Creeper");
		m_aSkills[nIndex++].SetSkill(DSD_CARRIONVINE, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Carrion Vine");
		m_aSkills[nIndex++].SetSkill(DSD_SOLARCREEPER, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Solar Creeper");

		break;

	case CCD_NECROMANCER:

		// Summoning Spells
		m_aSkills[nIndex++].SetSkill(DSD_SKELETONMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Skeleton Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_GOLEMMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Golem Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_SUMMONRESIST, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Summon Resist");
		m_aSkills[nIndex++].SetSkill(DSD_CLAYGOLEM, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Clay Golem");
		m_aSkills[nIndex++].SetSkill(DSD_BLOODGOLEM, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Blood Golem");
		m_aSkills[nIndex++].SetSkill(DSD_IRONGOLEM, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Iron Golem");
		m_aSkills[nIndex++].SetSkill(DSD_FIREGOLEM, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Fire Golem");
		m_aSkills[nIndex++].SetSkill(DSD_RAISESKELETON, 0, FALSE, TRUE, TRUE, TRUE, FALSE, "Raise Skeleton");
		m_aSkills[nIndex++].SetSkill(DSD_RAISESKELETALMAGE, 0, FALSE, TRUE, TRUE, TRUE, FALSE, "Raise Skeletal Mage");
		m_aSkills[nIndex++].SetSkill(DSD_REVIVE, 0, FALSE, TRUE, TRUE, TRUE, FALSE, "Revive");

		// Poison and Bone Spells
		m_aSkills[nIndex++].SetSkill(DSD_POISONDAGGER, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Poison Dagger");
		m_aSkills[nIndex++].SetSkill(DSD_POISONEXPLOSION, 0, FALSE, TRUE, TRUE, TRUE, FALSE, "Poison Explosion");
		m_aSkills[nIndex++].SetSkill(DSD_POISONNOVA, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Poison Nova");
		m_aSkills[nIndex++].SetSkill(DSD_TEETH, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Teeth");
		m_aSkills[nIndex++].SetSkill(DSD_CORPOSEEXPLOSION, 0, FALSE, TRUE, TRUE, TRUE, FALSE, "Corpose Explosion");
		m_aSkills[nIndex++].SetSkill(DSD_BONESPEAR, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Bone Spear");
		m_aSkills[nIndex++].SetSkill(DSD_BONESPIRIT, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Bone Spirit");
		m_aSkills[nIndex++].SetSkill(DSD_BONEARMOR, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Bone Armor");
		m_aSkills[nIndex++].SetSkill(DSD_BONEWALL, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Bone Wall");
		m_aSkills[nIndex++].SetSkill(DSD_BONEPRISON, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Bone Prison");

		// Curses
		m_aSkills[nIndex++].SetSkill(DSD_DIMVISION, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Dim Vision");
		m_aSkills[nIndex++].SetSkill(DSD_CONFUSE, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Confuse");
		m_aSkills[nIndex++].SetSkill(DSD_ATTRACT, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Attract");
		m_aSkills[nIndex++].SetSkill(DSD_AMPLIFYDAMAGE, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Amplify Damage");
		m_aSkills[nIndex++].SetSkill(DSD_IRONMAIDEN, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Iron Maiden");
		m_aSkills[nIndex++].SetSkill(DSD_LIFETAP, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Life Tap");
		m_aSkills[nIndex++].SetSkill(DSD_LOWERRESIST, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Lower Resist");
		m_aSkills[nIndex++].SetSkill(DSD_WEAKEN, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Weaken");
		m_aSkills[nIndex++].SetSkill(DSD_TERROR, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Terror");
		m_aSkills[nIndex++].SetSkill(DSD_DECREPIFY, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Decrepify");

		break;

	case CCD_PALADIN:

		// Defensive Auras
		m_aSkills[nIndex++].SetSkill(DSD_PRAYER, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Prayer");
		m_aSkills[nIndex++].SetSkill(DSD_CLEANSING, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Cleansing");
		m_aSkills[nIndex++].SetSkill(DSD_MEDITATION, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Meditation");
		m_aSkills[nIndex++].SetSkill(DSD_DEFIANCE, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Defiance");
		m_aSkills[nIndex++].SetSkill(DSD_VIGOR, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Vigor");
		m_aSkills[nIndex++].SetSkill(DSD_REDEMPTION, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Redemption");
		m_aSkills[nIndex++].SetSkill(DSD_RESISTFIRE, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Resist Fire");
		m_aSkills[nIndex++].SetSkill(DSD_RESISTCOLD, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Resist Cold");
		m_aSkills[nIndex++].SetSkill(DSD_RESISTLIGHTNING, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Resist Lightning");
		m_aSkills[nIndex++].SetSkill(DSD_SALVATION, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Salvation");

		// Offensive Auras
		m_aSkills[nIndex++].SetSkill(DSD_MIGHT, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Might");
		m_aSkills[nIndex++].SetSkill(DSD_BLESSEDAIM, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Blessed Aim");
		m_aSkills[nIndex++].SetSkill(DSD_CONCENTRATION, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Concentration");
		m_aSkills[nIndex++].SetSkill(DSD_FANATICISM, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Fanaticism");
		m_aSkills[nIndex++].SetSkill(DSD_HOLYFIRE, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Holy Fire");
		m_aSkills[nIndex++].SetSkill(DSD_HOLYFREEZE, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Holy Freeze");
		m_aSkills[nIndex++].SetSkill(DSD_HOLYSHOCK, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Holy Shock");
		m_aSkills[nIndex++].SetSkill(DSD_THORNS, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Thorns");
		m_aSkills[nIndex++].SetSkill(DSD_SANCTUARY, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Sanctuary");
		m_aSkills[nIndex++].SetSkill(DSD_CONVICTION, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Conviction");

		// Combat Skills
		m_aSkills[nIndex++].SetSkill(DSD_SACRIFICE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Sacrifice");
		m_aSkills[nIndex++].SetSkill(DSD_ZEAL, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Zeal");
		m_aSkills[nIndex++].SetSkill(DSD_VENGEANCE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Vengeance");
		m_aSkills[nIndex++].SetSkill(DSD_CONVERSION, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Conversion");
		m_aSkills[nIndex++].SetSkill(DSD_HOLYBOLT, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Holy Bolt");
		m_aSkills[nIndex++].SetSkill(DSD_BLESSEDHAMMER, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Blessed Hammer");
		m_aSkills[nIndex++].SetSkill(DSD_FISTOFTHEHEAVENS, 1000, FALSE, FALSE, TRUE, FALSE, FALSE, "Fist of The Heavens");
		m_aSkills[nIndex++].SetSkill(DSD_SMITE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Smite");
		m_aSkills[nIndex++].SetSkill(DSD_CHARGE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Charge");
		m_aSkills[nIndex++].SetSkill(DSD_HOLYSHIELD, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Holy Shield");
		break;

	case CCD_SORCERESS:

		// Cold Spells
		m_aSkills[nIndex++].SetSkill(DSD_FROSTNOVA, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Frost Nova");
		m_aSkills[nIndex++].SetSkill(DSD_BLIZZARD, 1800, FALSE, TRUE, TRUE, FALSE, FALSE, "Blizzard");
		m_aSkills[nIndex++].SetSkill(DSD_FROZENORB, 1000, FALSE, FALSE, TRUE, FALSE, FALSE, "Frozen Orb");
		m_aSkills[nIndex++].SetSkill(DSD_ICEBOLT, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Ice Bolt");
		m_aSkills[nIndex++].SetSkill(DSD_ICEBLAST, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Ice Blast");
		m_aSkills[nIndex++].SetSkill(DSD_GLACIALSPIKE, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Glacial Spike");
		m_aSkills[nIndex++].SetSkill(DSD_COLDMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Cold Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_FROZENARMOR, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Frozen Armor");
		m_aSkills[nIndex++].SetSkill(DSD_SHIVERARMOR, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Shiver Armor");
		m_aSkills[nIndex++].SetSkill(DSD_CHILLINGARMOR, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Chilling Armor");

		// Lightning Spells
		m_aSkills[nIndex++].SetSkill(DSD_STATICFIELD, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Static Field");
		m_aSkills[nIndex++].SetSkill(DSD_NOVA, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Nova");
		m_aSkills[nIndex++].SetSkill(DSD_THUNDERSTORM, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Thunder Storm");
		m_aSkills[nIndex++].SetSkill(DSD_CHARGEDBOLT, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Charged Bolt");
		m_aSkills[nIndex++].SetSkill(DSD_LIGHTNING, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Lightning");
		m_aSkills[nIndex++].SetSkill(DSD_CHAINLIGHTNING, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Chain Lightning");
		m_aSkills[nIndex++].SetSkill(DSD_LIGHTNINGMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Lightning Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_TELEKINESIS, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Telekinesis");
		m_aSkills[nIndex++].SetSkill(DSD_TELEPORT, 0, FALSE, TRUE, TRUE, FALSE, FALSE, "Teleport");
		m_aSkills[nIndex++].SetSkill(DSD_ENERGYSHIELD, 0, FALSE, TRUE, FALSE, FALSE, TRUE, "Energy Shield");

		// Fire Spells
		m_aSkills[nIndex++].SetSkill(DSD_INFERNO, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Inferno");
		m_aSkills[nIndex++].SetSkill(DSD_BLAZE, 0, FALSE, TRUE, FALSE, FALSE, FALSE, "Blaze");
		m_aSkills[nIndex++].SetSkill(DSD_FIREWALL, 1400, FALSE, TRUE, TRUE, FALSE, FALSE, "Fire Wall");
		m_aSkills[nIndex++].SetSkill(DSD_FIREBOLT, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Fire Bolt");
		m_aSkills[nIndex++].SetSkill(DSD_FIREBALL, 0, FALSE, FALSE, TRUE, FALSE, FALSE, "Fire Ball");
		m_aSkills[nIndex++].SetSkill(DSD_METEOR, 1200, FALSE, TRUE, TRUE, FALSE, FALSE, "Meteor");
		m_aSkills[nIndex++].SetSkill(DSD_FIREMASTERY, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Fire Mastery");
		m_aSkills[nIndex++].SetSkill(DSD_WARMTH, 0, TRUE, FALSE, FALSE, FALSE, FALSE, "Warmth");
		m_aSkills[nIndex++].SetSkill(DSD_ENCHANT, 0, FALSE, TRUE, TRUE, FALSE, TRUE, "Enchant");
		m_aSkills[nIndex++].SetSkill(DSD_HYDRA, 2000, FALSE, TRUE, TRUE, FALSE, FALSE, "Hydra");

		break;

	default: // error handling
		//assert(FALSE); // unexpected character class
		break;
	}

	// Total skill number must be _SKILLNUMBER, otherwise something
	// went wrong.
	if (m_byCharClass < 7)
		assert(nIndex == _SKILLNUMBER);
}

const CD2Skill* CD2Character::FindSkill(WORD wSkillID) const
{
	for (int i = 0; i < _SKILLNUMBER; i++)
	{
		if (m_aSkills[i].IsIDSame(wSkillID))
			return &(m_aSkills[i]);
	}

	return NULL;
}

const CD2Skill* CD2Character::FindSkill(LPCSTR lpSkillName) const
{
	for (int i = 0; i < _SKILLNUMBER; i++)
	{
		if (m_aSkills[i].IsNameSame(lpSkillName))
			return &(m_aSkills[i]);
	}

	return NULL;
}

LPCSTR CD2Character::GetCharName() const
{
	return m_szCharName;
}

BYTE CD2Character::GetCharClass() const
{
	return m_byCharClass;
}

LPCSTR CD2Character::GetClassName() const
{
	return m_szClassName;
}


//////////////////////////////////////////////////////////////////
// Implementation of CD2Action class
//////////////////////////////////////////////////////////////////

CD2Action::CD2Action()
{
	m_wSkillID = DSD_ATTACK;
	m_nSide = SAS_LEFT;
	m_nTarget = CAT_PINDLESKIN;
	m_wDelay = 0;
	m_wRepeat = 1;
	m_bEnabled = TRUE;
}

CD2Action::CD2Action(WORD wSkillID, int nSide, int nTarget, WORD wDelay, WORD wRepeat)
{
	m_wSkillID = wSkillID;
	m_nSide = nSide;
	m_nTarget = nTarget;
	m_wDelay = wDelay;
	m_wRepeat = wRepeat;
	m_bEnabled = TRUE;
}

void CD2Action::SetAction(WORD wSkillID, int nSide, int nTarget, WORD wDelay, WORD wRepeat)
{
	m_wSkillID = wSkillID;
	m_nSide = nSide;
	m_nTarget = nTarget;
	m_wDelay = wDelay;
	m_wRepeat = wRepeat;
	m_bEnabled = TRUE;
}

const CD2Action& CD2Action::operator=(const CD2Action &rhs)
{
	m_wSkillID = rhs.m_wSkillID;
	m_nSide = rhs.m_nSide;
	m_nTarget = rhs.m_nTarget;
	m_wDelay = rhs.m_wDelay;
	m_wRepeat = rhs.m_wRepeat;
	return *this;
}

WORD CD2Action::GetSkillID() const
{
	return m_wSkillID;
}

int CD2Action::GetSide() const
{
	return m_nSide;
}

int CD2Action::GetTarget() const
{
	return m_nTarget;
}

WORD CD2Action::GetDelay() const
{
	return m_wDelay;
}

WORD CD2Action::GetRepeat() const
{
	return m_wRepeat;
}

BOOL CD2Action::ParseString(LPCSTR lpString)
{
	// information stored in a string in the following format:
	// skill_ID,side,target,delay,repeat
	LPCSTR pStart = lpString;
	LPCSTR pPos = NULL;
	char sz[32] = "";

	// Get the skill id
	pPos = strchr(pStart, ',');
	if (pPos == NULL || pPos - pStart < 1)
		return FALSE;
	::memset(sz, 0, 32);
	strncpy(sz, pStart, pPos - pStart);
	m_wSkillID = atoi(sz);
	pStart = pPos + 1;

	// Get left/right side
	pPos = strchr(pStart, ',');
	if (pPos == NULL || pPos - pStart < 1)
		return FALSE;
	::memset(sz, 0, 32);
	strncpy(sz, pStart, pPos - pStart);
	m_nSide = atoi(sz);
	pStart = pPos + 1;

	// Get target
	pPos = strchr(pStart, ',');
	if (pPos == NULL || pPos - pStart < 1)
		return FALSE;
	::memset(sz, 0, 32);
	strncpy(sz, pStart, pPos - pStart);
	m_nTarget = atoi(sz);
	pStart = pPos + 1;

	// Get cast delay
	pPos = strchr(pStart, ',');
	if (pPos == NULL || pPos - pStart < 1)
		return FALSE;
	::memset(sz, 0, 32);
	strncpy(sz, pStart, pPos - pStart);
	m_wDelay = atoi(sz);
	pStart = pPos + 1;

	// The remaining string is repeat times
	m_wRepeat = atoi(pStart);

	return TRUE; // Indicates valid action
}

void CD2Action::ToString(LPSTR lpString) const
{
	sprintf(lpString, "%d,%d,%d,%d,%d", m_wSkillID, m_nSide, m_nTarget, m_wDelay, m_wRepeat);
}

void CD2Action::Enable(BOOL bEnable)
{
	m_bEnabled = bEnable;
}

BOOL CD2Action::IsEnabled() const
{
	return m_bEnabled;
}

void CD2Action::GetSideText(LPSTR lpsz) const
{
	if (m_nSide == SAS_LEFT)
		strcpy(lpsz, "左");
	else
		strcpy(lpsz, "右");
}

void CD2Action::GetTargetText(LPSTR lpsz) const
{
	if (m_nTarget == CAT_PINDLESKIN)
	{
		strcpy(lpsz, "Pindleskin本身");
	}
	else if (m_nTarget == CAT_BEYONDPINDLESKIN)
	{
		strcpy(lpsz, "穿越Pindleskin");
	}
	else if (m_nTarget == CAT_BEFOREPINDLESKIN)
	{
		strcpy(lpsz, "角色与Pindleskin之间");
	}
	else
	{
		strcpy(lpsz, "无需目标");
	}
}

void CD2Action::SetDelay(WORD wDelay)
{
	m_wDelay = wDelay;
}
