/****************************************************
*       D2Consts.s                                  *
* Declaration for D2X speed calculatiing constants. *
* 2001-12-17 Bin Liu                                *
* All rights reserved.                              *
*****************************************************/

#ifndef D2XCONSTS_H
#define D2XCONSTS_H

#define DATA_INVALID	-1

// Character classes
#define CC_INVALID		-1
#define CC_AMAZON		0
#define CC_ASSASSIN		1
#define CC_BARBARIAN	2
#define CC_DRUID		3
#define CC_NECROMANCER	4
#define CC_PALADIN		5
#define CC_SORCERESS	6

// weapon categories
#define WC_INVALID		-1
#define WC_1HSWING		0
#define WC_2HSWORD		1
#define WC_1HTHRUST		2
#define WC_2HTHRUST		3
#define WC_2HOTHER		4
#define WC_BOW			5
#define WC_CROSSBOW		6
#define WC_CLAW			7
#define WC_THROW		8
//#define WC_CAST			9

// weapon type filter strings
#define TF_ALL			"All"
#define TF_1HSWORD		"One-handed swords"
#define TF_2HSWORD		"Two-handed swords"
#define TF_1HMACE		"One-handed maces"
#define TF_2HMACE		"Two-handed maces"
#define TF_1HAXE		"One-handed axes"
#define TF_2HAXE		"Two-handed axes"
#define TF_WAND			"Wands"
#define TF_SCEPTER		"Scepters"
#define TF_DAGGER		"Daggers"
#define TF_JAVELIN		"Javelins"
#define TF_SPEAR		"Spears"
#define TF_POLEARM		"Polearms"
#define TF_STAFF		"Staves"
#define TF_BOW			"Bows"
#define TF_CROSSBOW		"Crossbows"
#define TF_ORB			"Orbs"
#define TF_CLAW			"Claws"
#define TF_THROW		"Throwing weapons"
//#define TF_CAST			"Casting"

// parameter ranges
#define MIN_WIAS		0
#define MAX_WIAS		999
#define MAX_EIAS		75
#define MIN_NWIAS		0
#define MAX_NWIAS		999
#define MIN_FANA		0
#define MAX_FANA		40
#define MIN_BOS			0
#define MAX_BOS			61
#define MIN_FRENZY		0
#define MAX_FRENZY		51
#define MIN_WSM			-60
#define MAX_WSM			20
#define MIN_MELEE		7
#define MIN_MELEEX		4
#define MIN_WEREWOLF	20
#define MAX_WEREWOLF	82
#define MAX_SPEEDTABLE	30
#define MAX_WEAPONLIST	350


// attack forms
#define AF_REGULAR		0
#define AF_THROW		1
#define AF_CAST			2
#define AF_ZEAL			3
#define AF_SMITE		4
#define AF_STRAFE		5
#define AF_WEREBEAR		6
#define AF_WEREWOLF		7
#define AF_FURY			8
#define AF_LIGHTNING	9

// base speed date

// AMAZON
#define BS_AMA_1HSWING		13
#define BS_AMA_2HSWORD		17
#define BS_AMA_1HTHRUST		12
#define BS_AMA_2HTHRUST		15
#define BS_AMA_2HOTHER		17
#define BS_AMA_BOW			13
#define BS_AMA_CROSSBOW		19
#define BS_AMA_THROW		15
#define BS_AMA_CAST			19
#define BS_AMA_CLAW			DATA_INVALID

// SORCERESS
#define BS_SOR_1HSWING		17
#define BS_SOR_2HSWORD		21
#define BS_SOR_1HTHRUST		16
#define BS_SOR_2HTHRUST		20
#define BS_SOR_2HOTHER		15
#define BS_SOR_BOW			16
#define BS_SOR_CROSSBOW		19
#define BS_SOR_THROW		19
#define BS_SOR_CAST			13
#define BS_SOR_CLAW			DATA_INVALID

// NECROMANCER
#define BS_NEC_1HSWING		18
#define BS_NEC_2HSWORD		22
#define BS_NEC_1HTHRUST		18
#define BS_NEC_2HTHRUST		23
#define BS_NEC_2HOTHER		19
#define BS_NEC_BOW			17
#define BS_NEC_CROSSBOW		19
#define BS_NEC_THROW		19
#define BS_NEC_CAST			15
#define BS_NEC_CLAW			DATA_INVALID

// PALADIN
#define BS_PAL_1HSWING		14
#define BS_PAL_2HSWORD		17.5
#define BS_PAL_1HTHRUST		16
#define BS_PAL_2HTHRUST		19
#define BS_PAL_2HOTHER		17
#define BS_PAL_BOW			15
#define BS_PAL_CROSSBOW		19
#define BS_PAL_THROW		15
#define BS_PAL_CAST			15
#define BS_PAL_CLAW			DATA_INVALID

// BARBARIAN
#define BS_BAR_1HSWING		15
#define BS_BAR_2HSWORD		17
#define BS_BAR_1HTHRUST		15
#define BS_BAR_2HTHRUST		18
#define BS_BAR_2HOTHER		18
#define BS_BAR_BOW			14
#define BS_BAR_CROSSBOW		19
#define BS_BAR_THROW		15
#define BS_BAR_CAST			13
#define BS_BAR_CLAW			DATA_INVALID

// DRUID
#define BS_DRU_1HSWING		20
#define BS_DRU_2HSWORD		21
#define BS_DRU_1HTHRUST		20
#define BS_DRU_2HTHRUST		23
#define BS_DRU_2HOTHER		17
#define BS_DRU_BOW			12
#define BS_DRU_CROSSBOW		19
#define BS_DRU_THROW		17
#define BS_DRU_CAST			18
#define BS_DRU_CLAW			DATA_INVALID

// ASSASSIN
#define BS_ASN_1HSWING		14
#define BS_ASN_2HSWORD		22
#define BS_ASN_1HTHRUST		14
#define BS_ASN_2HTHRUST		22
#define BS_ASN_2HOTHER		18
#define BS_ASN_BOW			15
#define BS_ASN_CROSSBOW		20
#define BS_ASN_THROW		14
#define BS_ASN_CAST			16
#define BS_ASN_CLAW			13

// special base
#define LIGHTNING_BASE		28
#define ZEAL_BASE			7
#define SMITE_BASE			11
#define WEREWOLF_BASE		28
#define WEREBEAR_BASE		23
#define STRAFE_BASE			-1
#define FURY_BASE			-1

#endif