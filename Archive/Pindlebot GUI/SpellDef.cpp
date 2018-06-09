#include "stdafx.h"
#include "SpellDef.h"
#include <tchar.h>
#include <string.h>

BOOL D2GetSpellInfo(WORD wSpell, LPD2SPELLINFO lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;
	
	::memset(lpBuffer, 0, sizeof(D2SPELLINFO));
	lpBuffer->wSpellID = wSpell;
	
	switch (wSpell)
	{
		// Common Skills 
		case D2S_ATTACK:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Attack"), SPELL_MAX_NAME);
			break;

		case D2S_THROW:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
						
			_tcsncpy(lpBuffer->szSpellName, _T("Throw"), SPELL_MAX_NAME);
			break;

		case D2S_UNSUMMON:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Unsummon"), SPELL_MAX_NAME);
			break;

		case D2S_TOMEOFIDENTIFY:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Tome of Identify"), SPELL_MAX_NAME);
			break;

		case D2S_SCROLLOFIDENTIFY:
			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Scroll of Identify"), SPELL_MAX_NAME);
			break;

		case D2S_TOMEOFTOWNPORTAL:
						
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
						
			_tcsncpy(lpBuffer->szSpellName, _T("Tome of Town Portal"), SPELL_MAX_NAME);
			break;

		case D2S_SCROLLOFTOWNPORTAL:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
						
			_tcsncpy(lpBuffer->szSpellName, _T("Scroll of Town Portal"), SPELL_MAX_NAME);
			break;

///////////////////////////////////////////////////////////////
//    Amazon Skills
///////////////////////////////////////////////////////////////

// Javelin and Spear Skills
		case D2S_JAB:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
						
			_tcsncpy(lpBuffer->szSpellName, _T("Jab"), SPELL_MAX_NAME);
			break;

		case D2S_IMPALE:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
						
			_tcsncpy(lpBuffer->szSpellName, _T("Impale"), SPELL_MAX_NAME);
			break;

		case D2S_FEND:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;			
						
			_tcsncpy(lpBuffer->szSpellName, _T("Fend"), SPELL_MAX_NAME);
			break;

		case D2S_POWERSTRIKE:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Power Strike"), SPELL_MAX_NAME);
			break;

		case D2S_CHARGEDSTRIKE:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Charged Strike"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNINGSTRIKE:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Strike"), SPELL_MAX_NAME);
			break;

		case D2S_POISONJAVELIN:
			
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Javelin"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNINGBOLT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_PLAGUEJAVELIN:
			
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Plague Javelin"), SPELL_MAX_NAME);
			break;
			
		case D2S_LIGHTNINGFURY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Fury"), SPELL_MAX_NAME);
			break;


// Passive and Magic Skills
		case D2S_INNERSIGHT:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Inner Sight"), SPELL_MAX_NAME);
			break;

		case D2S_SLOWMISSILES:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Slow Missiles"), SPELL_MAX_NAME);
			break;

		case D2S_DECOY:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

			_tcsncpy(lpBuffer->szSpellName, _T("Decoy"), SPELL_MAX_NAME);
			break;

		case D2S_VALKYRIE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

			_tcsncpy(lpBuffer->szSpellName, _T("Valkyrie"), SPELL_MAX_NAME);
			break;

		case D2S_DODGE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dodge"), SPELL_MAX_NAME);
			break;

		case D2S_AVOID:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Avoid"), SPELL_MAX_NAME);
			break;

		case D2S_EVADE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Evade"), SPELL_MAX_NAME);
			break;

		case D2S_CRITICALSTRIKE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Critical Strike"), SPELL_MAX_NAME);
			break;

		case D2S_PENETRATE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Penetrate"), SPELL_MAX_NAME);
			break;

		case D2S_PIERCE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Pierce"), SPELL_MAX_NAME);
			break;

// Bow and Crossbow Skills
		case D2S_COLDARROW:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cold Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_ICEARROW:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				
			
			_tcsncpy(lpBuffer->szSpellName, _T("Ice Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_FREEZINGARROW:
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Freezing Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_MAGICARROW:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Magic Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_MULTIPLESHOT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Multiple Shot"), SPELL_MAX_NAME);
			break;

		case D2S_GUIDEDARROW:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Guided Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_STRAFE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Strafe"), SPELL_MAX_NAME);
			break;

		case D2S_FIREARROW:
			
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_EXPLODINGARROW:
			
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Exploding Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_IMMOLATIONARROW:
			lpBuffer->dwCoolDown = 1000;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Immolation Arrow"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Assassin Skills
///////////////////////////////////////////////////////////////

// Martial Arts
		case D2S_FISTSOFFIRE:
			
			//lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fists of Fire"), SPELL_MAX_NAME);
			break;

		case D2S_CLAWSOFTHUNDER:
			
			//lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Claws of Thunder"), SPELL_MAX_NAME);
			break;

		case D2S_BLADESOFICE:
			
			//lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blades of Ice"), SPELL_MAX_NAME);
			break;

		case D2S_TIGERSTRIKE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Tiger Strike"), SPELL_MAX_NAME);
			break;

		case D2S_COBRASTRIKE:

			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cobra Strike"), SPELL_MAX_NAME);
			break;

		case D2S_PHOENIXSTRIKE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Phoenix Strike"), SPELL_MAX_NAME);
			break;

		case D2S_DRAGONTALON:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dragon Talon"), SPELL_MAX_NAME);
			break;

		case D2S_DRAGONCLAW:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dragon Claw"), SPELL_MAX_NAME);
			break;

		case D2S_DRAGONTAIL:
			
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dragon Tail"), SPELL_MAX_NAME);
			break;

		case D2S_DRAGONFLIGHT:
			lpBuffer->dwCoolDown = 2000;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			//lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dragon Flight"), SPELL_MAX_NAME);
			break;


// Shadow Disciplines
		case D2S_BURSTOFSPEED:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

			_tcsncpy(lpBuffer->szSpellName, _T("Burst of Speed"), SPELL_MAX_NAME);
			break;

		case D2S_FADE:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

			_tcsncpy(lpBuffer->szSpellName, _T("Fade"), SPELL_MAX_NAME);
			break;

		case D2S_VENOM:
						
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

			_tcsncpy(lpBuffer->szSpellName, _T("Venom"), SPELL_MAX_NAME);
			break;

		case D2S_CLAWMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
	
			_tcsncpy(lpBuffer->szSpellName, _T("Claw Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_WEAPONBLOCK:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Weapon Block"), SPELL_MAX_NAME);
			break;

		case D2S_SHADOWWARRIOR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

			_tcsncpy(lpBuffer->szSpellName, _T("Shadow Warrior"), SPELL_MAX_NAME);
			break;

		case D2S_SHADOWMASTER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shadow Master"), SPELL_MAX_NAME);
			break;

		case D2S_PSYCHICHAMMER:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Psychic Hammer"), SPELL_MAX_NAME);
			break;

		case D2S_CLOAKOFSHADOWS:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cloak of Shadows"), SPELL_MAX_NAME);
			break;

		case D2S_MINDBLAST:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Mind Blast"), SPELL_MAX_NAME);
			break;

// Traps
		case D2S_SHOCKWEB:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shock Web"), SPELL_MAX_NAME);
			break;

		case D2S_CHARGEDBOLTSENTRY:
			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Charged Bolt Sentry"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNINGSENTRY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Sentry"), SPELL_MAX_NAME);
			break;

		case D2S_DEATHSENTRY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Death Sentry"), SPELL_MAX_NAME);
			break;

		case D2S_FIREBLAST:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Blast"), SPELL_MAX_NAME);
			break;

		case D2S_WAKEOFFIRE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Wake of Fire"), SPELL_MAX_NAME);
			break;

		case D2S_WAKEOFINFERNO:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Wake of Inferno"), SPELL_MAX_NAME);
			break;

		case D2S_BLADESENTINEL:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blade Sentinel"), SPELL_MAX_NAME);
			break;

		case D2S_BLADEFURY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blade Fury"), SPELL_MAX_NAME);
			break;

		case D2S_BLADESHIELD:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

			_tcsncpy(lpBuffer->szSpellName, _T("Blade Shield"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Barbarian Skills
///////////////////////////////////////////////////////////////

// Warcries
		case D2S_HOWL:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Howl"), SPELL_MAX_NAME);
			break;

		case D2S_TAUNT:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Taunt"), SPELL_MAX_NAME);
			break;

		case D2S_BATTLECRY:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Battle Cry"), SPELL_MAX_NAME);
			break;

		case D2S_WARCRY:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;

			_tcsncpy(lpBuffer->szSpellName, _T("War Cry"), SPELL_MAX_NAME);
			break;

		case D2S_SHOUT:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shout"), SPELL_MAX_NAME);
			break;

		case D2S_BATTLEORDERS:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Battle Orders"), SPELL_MAX_NAME);
			break;

		case D2S_BATTLECOMMAND:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Battle Command"), SPELL_MAX_NAME);
			break;

		case D2S_FINDPOTION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Find Potion"), SPELL_MAX_NAME);
			break;

		case D2S_FINDITEM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Find Item"), SPELL_MAX_NAME);
			break;

		case D2S_GRIMWARD:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Grim Ward"), SPELL_MAX_NAME);
			break;


// Combat Masteries
		case D2S_SWORDMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Sword Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_POLEARMMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Pole Arm Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_INCREASEDSTAMINA:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Increased Stamin"), SPELL_MAX_NAME);
			break;

		case D2S_INCREASEDSPEED:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Increased Speed"), SPELL_MAX_NAME);
			break;

		case D2S_AXEMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Axe Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_THROWINGMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Throwing Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_MACEMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

			_tcsncpy(lpBuffer->szSpellName, _T("Mace Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_SPEARMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Spear Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_IRONSKIN:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Iron Skin"), SPELL_MAX_NAME);
			break;

		case D2S_NATURALRESISTANCE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Natural Resistance"), SPELL_MAX_NAME);
			break;

// Combat Skills
		case D2S_LEAP:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Leap"), SPELL_MAX_NAME);
			break;

		case D2S_LEAPATTACK:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Leap Attack"), SPELL_MAX_NAME);
			break;

		case D2S_WHIRLWIND:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Whirlwind"), SPELL_MAX_NAME);
			break;

		case D2S_BASH:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bash"), SPELL_MAX_NAME);
			break;

		case D2S_STUN:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Stun"), SPELL_MAX_NAME);
			break;

		case D2S_CONCENTRATE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Concentrate"), SPELL_MAX_NAME);
			break;

		case D2S_BERSERK:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Berserk"), SPELL_MAX_NAME);
			break;

		case D2S_DOUBLESWING:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Double Swing"), SPELL_MAX_NAME);
			break;

		case D2S_DOUBLETHROW:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Double Throw"), SPELL_MAX_NAME);
			break;

		case D2S_FRENZY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Frenzy"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Druid Skills
///////////////////////////////////////////////////////////////

// Elemental
		case D2S_FIRESTORM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Firestorm"), SPELL_MAX_NAME);
			break;

		case D2S_MOLTENBOULDER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwCoolDown = 2000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Molten Boulder"), SPELL_MAX_NAME);
			break;

		case D2S_FISSURE:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwCoolDown = 2000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fissure"), SPELL_MAX_NAME);
			break;

		case D2S_VOLCANO:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwCoolDown = 4000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Volcano"), SPELL_MAX_NAME);
			break;

		case D2S_ARMAGEDDON:
			
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwCoolDown = 6000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Armageddon"), SPELL_MAX_NAME);
			break;

		case D2S_TWISTER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Twister"), SPELL_MAX_NAME);
			break;

		case D2S_TORNADO:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Tornado"), SPELL_MAX_NAME);
			break;

		case D2S_HURRICANE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwCoolDown = 6000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Hurricane"), SPELL_MAX_NAME);
			break;

		case D2S_ARCTICBLAST:
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Arctic Blast"), SPELL_MAX_NAME);
			break;

		case D2S_CYCLONEARMOR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cyclone Armor"), SPELL_MAX_NAME);
			break;


// Shape Shifting
		case D2S_WEREWOLF:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			lpBuffer->dwCoolDown = 1000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Werewolf"), SPELL_MAX_NAME);
			break;

		case D2S_FERALRAGE:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Feral Rage"), SPELL_MAX_NAME);
			break;

		case D2S_RABIES:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Rabies"), SPELL_MAX_NAME);
			break;

		case D2S_FURY:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fury"), SPELL_MAX_NAME);
			break;

		case D2S_LYCANTHROPY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lycanthropy"), SPELL_MAX_NAME);
			break;

		case D2S_FIRECLAWS:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Claws"), SPELL_MAX_NAME);
			break;

		case D2S_HUNGER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Hunger"), SPELL_MAX_NAME);
			break;

		case D2S_WEREBEAR:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			lpBuffer->dwCoolDown = 1000;		
			
			_tcsncpy(lpBuffer->szSpellName, _T("Werebear"), SPELL_MAX_NAME);
			break;

		case D2S_MAUL:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Maul"), SPELL_MAX_NAME);
			break;

		case D2S_SHOCKWAVE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shock Wave"), SPELL_MAX_NAME);
			break;


// Summoning
		case D2S_OAKSAGE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Oak Sage"), SPELL_MAX_NAME);
			break;

		case D2S_HEARTOFWOLVERINE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Heart of Wolverine"), SPELL_MAX_NAME);
			break;

		case D2S_SPIRITOFBARBS:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Spirit of Barbs"), SPELL_MAX_NAME);
			break;

		case D2S_RAVEN:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Raven"), SPELL_MAX_NAME);
			break;

		case D2S_SUMMONSPIRITWOLF:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Summon Spirit Wolf"), SPELL_MAX_NAME);
			break;

		case D2S_SUMMONDIREWOLF:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Summon Dire Wolf"), SPELL_MAX_NAME);
			break;

		case D2S_SUMMONGRIZZLY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Summon Grizzly"), SPELL_MAX_NAME);
			break;

		case D2S_POISONCREEPER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Creeper"), SPELL_MAX_NAME);
			break;

		case D2S_CARRIONVINE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Carrion Vine"), SPELL_MAX_NAME);
			break;

		case D2S_SOLARCREEPER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Solar Creeper"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Necromaner Skills
///////////////////////////////////////////////////////////////

// Summoning Spells
		case D2S_SKELETONMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Skeleton Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_GOLEMMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Golem Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_SUMMONRESIST:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Summon Resist"), SPELL_MAX_NAME);
			break;

		case D2S_CLAYGOLEM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Clay Golem"), SPELL_MAX_NAME);
			break;

		case D2S_BLOODGOLEM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blood Golem"), SPELL_MAX_NAME);
			break;

		case D2S_IRONGOLEM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Iron Golem"), SPELL_MAX_NAME);
			break;

		case D2S_FIREGOLEM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Golem"), SPELL_MAX_NAME);
			break;

		case D2S_RAISESKELETON:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Raise Skeleton"), SPELL_MAX_NAME);
			break;

		case D2S_RAISESKELETALMAGE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Raise Skeletal Mage"), SPELL_MAX_NAME);
			break;

		case D2S_REVIVE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Revive"), SPELL_MAX_NAME);
			break;


// Poison and Bone Spells
		case D2S_POISONDAGGER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Dagger"), SPELL_MAX_NAME);
			break;

		case D2S_POISONEXPLOSION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Explosion"), SPELL_MAX_NAME);
			break;

		case D2S_POISONNOVA:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Nova"), SPELL_MAX_NAME);
			break;

		case D2S_TEETH:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Teeth"), SPELL_MAX_NAME);
			break;

		case D2S_CORPOSEEXPLOSION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Corpose Explosion"), SPELL_MAX_NAME);
			break;

		case D2S_BONESPEAR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Spear"), SPELL_MAX_NAME);
			break;

		case D2S_BONESPIRIT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Spirit"), SPELL_MAX_NAME);
			break;

		case D2S_BONEARMOR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Armor"), SPELL_MAX_NAME);
			break;

		case D2S_BONEWALL:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Wall"), SPELL_MAX_NAME);
			break;

		case D2S_BONEPRISON:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Prison"), SPELL_MAX_NAME);
			break;


// Curses
		case D2S_DIMVISION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dim Vision"), SPELL_MAX_NAME);
			break;

		case D2S_CONFUSE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Confuse"), SPELL_MAX_NAME);
			break;

		case D2S_ATTRACT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Attract"), SPELL_MAX_NAME);
			break;

		case D2S_AMPLIFYDAMAGE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Amplify Damage"), SPELL_MAX_NAME);
			break;

		case D2S_IRONMAIDEN:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Iron Maiden"), SPELL_MAX_NAME);
			break;

		case D2S_LIFETAP:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Life Tap"), SPELL_MAX_NAME);
			break;

		case D2S_LOWERRESIST:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lower Resist"), SPELL_MAX_NAME);
			break;

		case D2S_WEAKEN:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Weaken"), SPELL_MAX_NAME);
			break;

		case D2S_TERROR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Terror"), SPELL_MAX_NAME);
			break;

		case D2S_DECREPIFY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Decrepify"), SPELL_MAX_NAME);
			break;



///////////////////////////////////////////////////////////////
//    Paladin Skills
///////////////////////////////////////////////////////////////

// Defensive Auras
		case D2S_PRAYER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Prayer"), SPELL_MAX_NAME);
			break;

		case D2S_CLEANSING:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cleansing"), SPELL_MAX_NAME);
			break;

		case D2S_MEDITATION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Meditation"), SPELL_MAX_NAME);
			break;

		case D2S_DEFIANCE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Defiance"), SPELL_MAX_NAME);
			break;

		case D2S_VIGOR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Vigor"), SPELL_MAX_NAME);
			break;

		case D2S_REDEMPTION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Redemption"), SPELL_MAX_NAME);
			break;

		case D2S_RESISTFIRE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Resist Fire"), SPELL_MAX_NAME);
			break;

		case D2S_RESISTCOLD:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Resist Cold"), SPELL_MAX_NAME);
			break;

		case D2S_RESISTLIGHTNING:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Resist Lightning"), SPELL_MAX_NAME);
			break;

		case D2S_SALVATION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Salvation"), SPELL_MAX_NAME);
			break;


// Offensive Auras
		case D2S_MIGHT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Might"), SPELL_MAX_NAME);
			break;

		case D2S_BLESSEDAIM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blessed Aim"), SPELL_MAX_NAME);
			break;

		case D2S_CONCENTRATION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Concentration"), SPELL_MAX_NAME);
			break;

		case D2S_FANATICISM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fanaticism"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYFIRE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Fire"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYFREEZE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Freeze"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYSHOCK:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Shock"), SPELL_MAX_NAME);
			break;

		case D2S_THORNS:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Thorns"), SPELL_MAX_NAME);
			break;

		case D2S_SANCTUARY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Sanctuary"), SPELL_MAX_NAME);
			break;

		case D2S_CONVICTION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Conviction"), SPELL_MAX_NAME);
			break;


// Combat Skills
		case D2S_SACRIFICE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Sacrifice"), SPELL_MAX_NAME);
			break;

		case D2S_ZEAL:
			
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Zeal"), SPELL_MAX_NAME);
			break;

		case D2S_VENGEANCE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Vengeance"), SPELL_MAX_NAME);
			break;

		case D2S_CONVERSION:
			
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				
			
			_tcsncpy(lpBuffer->szSpellName, _T("Conversion"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYBOLT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_BLESSEDHAMMER:
			
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blessed Hammer"), SPELL_MAX_NAME);
			break;

		case D2S_FISTOFTHEHEAVENS:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwCoolDown = 1000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fist of The Heavens"), SPELL_MAX_NAME);
			break;

		case D2S_SMITE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Smite"), SPELL_MAX_NAME);
			break;

		case D2S_CHARGE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Charge"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYSHIELD:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Shield"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Sorceress Skills
///////////////////////////////////////////////////////////////

// Cold Spells
		case D2S_FROSTNOVA:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Frost Nova"), SPELL_MAX_NAME);
			break;

		case D2S_BLIZZARD:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwCoolDown = 1800;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blizzard"), SPELL_MAX_NAME);
			break;

		case D2S_FROZENORB:
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwCoolDown = 1000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Frozen Orb"), SPELL_MAX_NAME);
			break;

		case D2S_ICEBOLT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Ice Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_ICEBLAST:
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Ice Blast"), SPELL_MAX_NAME);
			break;

		case D2S_GLACIALSPIKE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Glacial Spike"), SPELL_MAX_NAME);
			break;

		case D2S_COLDMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cold Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_FROZENARMOR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Frozen Armor"), SPELL_MAX_NAME);
			break;

		case D2S_SHIVERARMOR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shiver Armor"), SPELL_MAX_NAME);
			break;

		case D2S_CHILLINGARMOR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Chilling Armor"), SPELL_MAX_NAME);
			break;

// Lightning Spells
		case D2S_STATICFIELD:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Static Field"), SPELL_MAX_NAME);
			break;

		case D2S_NOVA:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Nova"), SPELL_MAX_NAME);
			break;

		case D2S_THUNDERSTORM:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Thunder Storm"), SPELL_MAX_NAME);
			break;

		case D2S_CHARGEDBOLT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Charged Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNING:
			
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning"), SPELL_MAX_NAME);
			break;

		case D2S_CHAINLIGHTNING:
			
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Chain Lightning"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNINGMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_TELEKINESIS:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Telekinesis"), SPELL_MAX_NAME);
			break;

		case D2S_TELEPORT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Teleport"), SPELL_MAX_NAME);
			break;

		case D2S_ENERGYSHIELD:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Energy Shield"), SPELL_MAX_NAME);
			break;


// Fire Spells
		case D2S_INFERNO:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Inferno"), SPELL_MAX_NAME);
			break;

		case D2S_BLAZE:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blaze"), SPELL_MAX_NAME);
			break;

		case D2S_FIREWALL:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwCoolDown = 1400;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Wall"), SPELL_MAX_NAME);
			break;

		case D2S_FIREBOLT:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_FIREBALL:
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Ball"), SPELL_MAX_NAME);
			break;

		case D2S_METEOR:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwCoolDown = 1200;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Meteor"), SPELL_MAX_NAME);
			break;

		case D2S_FIREMASTERY:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_WARMTH:
			
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Warmth"), SPELL_MAX_NAME);
			break;

		case D2S_ENCHANT:
			
			//lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN; // no longer in 1.10
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Enchant"), SPELL_MAX_NAME);
			break;

		case D2S_HYDRA:
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			lpBuffer->dwCoolDown = 2000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Hydra"), SPELL_MAX_NAME);
			break;


		default:
		
			lpBuffer->wSpellID = D2S_INVALID;
			return FALSE;
			break;
	}

	return TRUE;
}
