#include "AffectionDef.h"
#include <tchar.h>
#include <string.h>

DWORD D2GetAffectionName(BYTE iAffectionID, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (dwMaxChars == 0)
		return 0;

	switch (iAffectionID)
	{
	case STATE_FREEZE:
		_tcsncpy(lpszBuffer, _T("Freeze"), dwMaxChars);
		break;

	case STATE_POISON:
		_tcsncpy(lpszBuffer, _T("Poison"), dwMaxChars);
		break;

	case STATE_RESISTFIRE:
		_tcsncpy(lpszBuffer, _T("Resist Fire"), dwMaxChars);
		break;

	case STATE_RESISTCOLD:
		_tcsncpy(lpszBuffer, _T("Resist Cold"), dwMaxChars);
		break;

	case STATE_RESISTLIGHT:
		_tcsncpy(lpszBuffer, _T("Resist Lightning"), dwMaxChars);
		break;

	case STATE_RESISTMAGIC:
		_tcsncpy(lpszBuffer, _T("Resist Magic"), dwMaxChars);
		break;

	case STATE_RESISTALL:
		_tcsncpy(lpszBuffer, _T("Salvation"), dwMaxChars);
		break;

	case STATE_AMPLIFYDAMAGE:
		_tcsncpy(lpszBuffer, _T("Amplify Damage"), dwMaxChars);
		break;

	case STATE_FROZENARMOR:
		_tcsncpy(lpszBuffer, _T("Frozen Armor"), dwMaxChars);
		break;

	case STATE_COLD:
		_tcsncpy(lpszBuffer, _T("Cold"), dwMaxChars);
		break;

	case STATE_INFERNO:
		_tcsncpy(lpszBuffer, _T("Inferno"), dwMaxChars);
		break;

	case STATE_BLAZE:
		_tcsncpy(lpszBuffer, _T("Blaze"), dwMaxChars);
		break;

	case STATE_BONEARMOR:
		_tcsncpy(lpszBuffer, _T("Bone Armor"), dwMaxChars);
		break;

	case STATE_CONCENTRATE:
		_tcsncpy(lpszBuffer, _T("Concentrate"), dwMaxChars);
		break;

	case STATE_ENCHANT:
		_tcsncpy(lpszBuffer, _T("Enchant"), dwMaxChars);
		break;

	case STATE_INNERSIGHT:
		_tcsncpy(lpszBuffer, _T("Inner Sight"), dwMaxChars);
		break;

	case STATE_SKILL_MOVE:
		_tcsncpy(lpszBuffer, _T("Skill Move"), dwMaxChars);
		break;

	case STATE_WEAKEN:
		_tcsncpy(lpszBuffer, _T("Weaken"), dwMaxChars);
		break;

	case STATE_CHILLINGARMOR:
		_tcsncpy(lpszBuffer, _T("Chilling Armor"), dwMaxChars);
		break;

	case STATE_STUNNED:
		_tcsncpy(lpszBuffer, _T("Stunned"), dwMaxChars);
		break;

	case STATE_SPIDERLAY:
		_tcsncpy(lpszBuffer, _T("Spider Lay"), dwMaxChars);
		break;

	case STATE_DIMVISION:
		_tcsncpy(lpszBuffer, _T("Dim Vision"), dwMaxChars);
		break;

	case STATE_SLOWED:
		_tcsncpy(lpszBuffer, _T("Slowed"), dwMaxChars);
		break;

	case STATE_FETISHAURA:
		_tcsncpy(lpszBuffer, _T("Fetish Aura"), dwMaxChars);
		break;

	case STATE_SHOUT:
		_tcsncpy(lpszBuffer, _T("Shout"), dwMaxChars);
		break;

	case STATE_TAUNT:
		_tcsncpy(lpszBuffer, _T("Taunt"), dwMaxChars);
		break;

	case STATE_CONVICTION:
		_tcsncpy(lpszBuffer, _T("Conviction"), dwMaxChars);
		break;

	case STATE_CONVICTED:
		_tcsncpy(lpszBuffer, _T("Convicted"), dwMaxChars);
		break;

	case STATE_ENERGYSHIELD:
		_tcsncpy(lpszBuffer, _T("Energy Shield"), dwMaxChars);
		break;

	case STATE_VENOMCLAWS:
		_tcsncpy(lpszBuffer, _T("Venom Claw"), dwMaxChars);
		break;

	case STATE_BATTLEORDERS:
		_tcsncpy(lpszBuffer, _T("Battle Orders"), dwMaxChars);
		break;

	case STATE_MIGHT:
		_tcsncpy(lpszBuffer, _T("Might"), dwMaxChars);
		break;

	case STATE_PRAYER:
		_tcsncpy(lpszBuffer, _T("Prayer"), dwMaxChars);
		break;

	case STATE_HOLYFIRE:
		_tcsncpy(lpszBuffer, _T("Holy Fire"), dwMaxChars);
		break;

	case STATE_THORNS:
		_tcsncpy(lpszBuffer, _T("Thorns"), dwMaxChars);
		break;

	case STATE_DEFIANCE:
		_tcsncpy(lpszBuffer, _T("Defiance"), dwMaxChars);
		break;

	case STATE_THUNDERSTORM:
		_tcsncpy(lpszBuffer, _T("Thunder Storm"), dwMaxChars);
		break;

	case STATE_LIGHTNINGBOLT:
		_tcsncpy(lpszBuffer, _T("Lightning Bolt"), dwMaxChars);
		break;

	case STATE_BLESSEDAIM:
		_tcsncpy(lpszBuffer, _T("Blessed Aim"), dwMaxChars);
		break;

	case STATE_STAMINA: // vigor
		_tcsncpy(lpszBuffer, _T("Vigor"), dwMaxChars);
		break;

	case STATE_CONCENTRATION:
		_tcsncpy(lpszBuffer, _T("Concentration"), dwMaxChars);
		break;

	case STATE_HOLYWIND: // Holy freeze
		_tcsncpy(lpszBuffer, _T("Holy Freeze"), dwMaxChars);
		break;

	case STATE_HOLYWINDCOLD:
		_tcsncpy(lpszBuffer, _T("Holy Freeze Chilled"), dwMaxChars);
		break;

	case STATE_CLEANSING:
		_tcsncpy(lpszBuffer, _T("Cleansing"), dwMaxChars);
		break;

	case STATE_HOLYSHOCK:
		_tcsncpy(lpszBuffer, _T("Holy Shock"), dwMaxChars);
		break;

	case STATE_SANCTUARY:
		_tcsncpy(lpszBuffer, _T("Sanctuary"), dwMaxChars);
		break;

	case STATE_MEDITATION:
		_tcsncpy(lpszBuffer, _T("Meditation"), dwMaxChars);
		break;

	case STATE_FANATICISM:
		_tcsncpy(lpszBuffer, _T("Fanaticism"), dwMaxChars);
		break;

	case STATE_REDEMPTION:
		_tcsncpy(lpszBuffer, _T("Redemption"), dwMaxChars);
		break;

	case STATE_BATTLECOMMAND:
		_tcsncpy(lpszBuffer, _T("Battle Command"), dwMaxChars);
		break;

	case STATE_PREVENTHEAL:
		_tcsncpy(lpszBuffer, _T("Prevent Heal"), dwMaxChars);
		break;

	case STATE_UNINTERRUPTABLE:
		_tcsncpy(lpszBuffer, _T("Uninterruptable"), dwMaxChars);
		break;

	case STATE_IRONMAIDEN:
		_tcsncpy(lpszBuffer, _T("Iron Maiden"), dwMaxChars);
		break;

	case STATE_TERROR:
		_tcsncpy(lpszBuffer, _T("Terror"), dwMaxChars);
		break;

	case STATE_ATTRACT:
		_tcsncpy(lpszBuffer, _T("Attract"), dwMaxChars);
		break;

	case STATE_LIFETAP:
		_tcsncpy(lpszBuffer, _T("Life Tap"), dwMaxChars);
		break;

	case STATE_CONFUSE:
		_tcsncpy(lpszBuffer, _T("Confuse"), dwMaxChars);
		break;

	case STATE_DECREPIFY:
		_tcsncpy(lpszBuffer, _T("Decrepify"), dwMaxChars);
		break;

	case STATE_LOWERRESIST:
		_tcsncpy(lpszBuffer, _T("Lower Resist"), dwMaxChars);
		break;

	case STATE_OPENWOUNDS:
		_tcsncpy(lpszBuffer, _T("Open Wounds"), dwMaxChars);
		break;

	case STATE_DOPPLEZON:
		_tcsncpy(lpszBuffer, _T("Dopplezon"), dwMaxChars);
		break;

	case STATE_CRITICALSTRIKE:
		_tcsncpy(lpszBuffer, _T("Critical Strike"), dwMaxChars);
		break;

	case STATE_DODGE:
		_tcsncpy(lpszBuffer, _T("Dodge"), dwMaxChars);
		break;

	case STATE_AVOID:
		_tcsncpy(lpszBuffer, _T("Avoid"), dwMaxChars);
		break;

	case STATE_PENETRATE:
		_tcsncpy(lpszBuffer, _T("Penetrate"), dwMaxChars);
		break;

	case STATE_EVADE:
		_tcsncpy(lpszBuffer, _T("Evade"), dwMaxChars);
		break;

	case STATE_PIERCE:
		_tcsncpy(lpszBuffer, _T("Pierce"), dwMaxChars);
		break;

	case STATE_WARMTH:
		_tcsncpy(lpszBuffer, _T("Warmth"), dwMaxChars);
		break;

	case STATE_FIREMASTERY:
		_tcsncpy(lpszBuffer, _T("Fire Mastery"), dwMaxChars);
		break;

	case STATE_LIGHTNINGMASTERY:
		_tcsncpy(lpszBuffer, _T("Lightning Mastery"), dwMaxChars);
		break;

	case STATE_COLDMASTERY:
		_tcsncpy(lpszBuffer, _T("Cold Mastery"), dwMaxChars);
		break;

	case STATE_SWORDMASTERY:
		_tcsncpy(lpszBuffer, _T("Shadow Mastery"), dwMaxChars);
		break;

	case STATE_AXEMASTERY:
		_tcsncpy(lpszBuffer, _T("Axe Mastery"), dwMaxChars);
		break;

	case STATE_MACEMASTERY:
		_tcsncpy(lpszBuffer, _T("Mace Mastery"), dwMaxChars);
		break;

	case STATE_POLEARMMASTERY:
		_tcsncpy(lpszBuffer, _T("Polearm Mastery"), dwMaxChars);
		break;

	case STATE_THROWINGMASTERY:
		_tcsncpy(lpszBuffer, _T("Throwing Mastery"), dwMaxChars);
		break;

	case STATE_SPEARMASTERY:
		_tcsncpy(lpszBuffer, _T("Spear Mastery"), dwMaxChars);
		break;

	case STATE_INCREASEDSTAMINA:
		_tcsncpy(lpszBuffer, _T("Increased Stamina"), dwMaxChars);
		break;

	case STATE_IRONSKIN:
		_tcsncpy(lpszBuffer, _T("Iron Skin"), dwMaxChars);
		break;

	case STATE_INCREASEDSPEED:
		_tcsncpy(lpszBuffer, _T("Increased Speed"), dwMaxChars);
		break;

	case STATE_NATURALRESISTANCE:
		_tcsncpy(lpszBuffer, _T("Natural Resistance"), dwMaxChars);
		break;

	case STATE_FINGERMAGECURSE:
		_tcsncpy(lpszBuffer, _T("Finger Mage Curse"), dwMaxChars);
		break;

	case STATE_NOMANAREGEN:
		_tcsncpy(lpszBuffer, _T("No Mana Regen"), dwMaxChars);
		break;

	case STATE_JUSTHIT:
		_tcsncpy(lpszBuffer, _T("Just Hit"), dwMaxChars);
		break;

	case STATE_SLOWMISSILES:
		_tcsncpy(lpszBuffer, _T("Slow Missles"), dwMaxChars);
		break;

	case STATE_SHIVERARMOR:
		_tcsncpy(lpszBuffer, _T("Shiver Armor"), dwMaxChars);
		break;

	case STATE_BATTLECRY:
		_tcsncpy(lpszBuffer, _T("Battle Cry"), dwMaxChars);
		break;

	case STATE_DEATH_DELAY:
		_tcsncpy(lpszBuffer, _T("Death Delay"), dwMaxChars);
		break;

	case STATE_VALKYRIE:
		_tcsncpy(lpszBuffer, _T("Valkyrie"), dwMaxChars);
		break;

	case STATE_FRENZY:
		_tcsncpy(lpszBuffer, _T("Frenzy"), dwMaxChars);
		break;

	case STATE_BERSERK:
		_tcsncpy(lpszBuffer, _T("Berserk"), dwMaxChars);
		break;

	case STATE_REVIVE:
		_tcsncpy(lpszBuffer, _T("Revive"), dwMaxChars);
		break;

	case STATE_ITEMFULLSET:
		_tcsncpy(lpszBuffer, _T("Item Full Set"), dwMaxChars);
		break;

	case STATE_SOURCEUNIT:
		_tcsncpy(lpszBuffer, _T("Source Unit"), dwMaxChars);
		break;

	case STATE_REDEEMED:
		_tcsncpy(lpszBuffer, _T("Redeemed"), dwMaxChars);
		break;

	case STATE_HEALTHPOT:
		_tcsncpy(lpszBuffer, _T("Health Pot"), dwMaxChars);
		break;

	case STATE_HOLYSHIELD:
		_tcsncpy(lpszBuffer, _T("Holy Shield"), dwMaxChars);
		break;

	case STATE_JUST_PORTALED:
		_tcsncpy(lpszBuffer, _T("Just Portaled"), dwMaxChars);
		break;

	case STATE_MONFRENZY:
		_tcsncpy(lpszBuffer, _T("MonFrenzy"), dwMaxChars);
		break;

	case STATE_CORPSE_NODRAW:
		_tcsncpy(lpszBuffer, _T("Corpse No Draw"), dwMaxChars);
		break;

	case STATE_ALIGNMENT:
		_tcsncpy(lpszBuffer, _T("Alignment"), dwMaxChars);
		break;

	case STATE_MANAPOT:
		_tcsncpy(lpszBuffer, _T("Mana Pot"), dwMaxChars);
		break;

	case STATE_SHATTER:
		_tcsncpy(lpszBuffer, _T("Shatter"), dwMaxChars);
		break;

	case STATE_SYNC_WARPED:
		_tcsncpy(lpszBuffer, _T("Sync Warped"), dwMaxChars);
		break;

	case STATE_CONVERSION_SAVE:
		_tcsncpy(lpszBuffer, _T("Conversion Save"), dwMaxChars);
		break;

	case STATE_PREGNANT:
		_tcsncpy(lpszBuffer, _T("Pregnant"), dwMaxChars); // LOL
		break;

	case STATE_RABIES:
		_tcsncpy(lpszBuffer, _T("Rabies"), dwMaxChars);
		break;

	case STATE_DEFENSE_CURSE:
		_tcsncpy(lpszBuffer, _T("Defense Curse"), dwMaxChars);
		break;

	case STATE_BLOOD_MANA:
		_tcsncpy(lpszBuffer, _T("Blood Mana"), dwMaxChars);
		break;

	case STATE_BURNING:
		_tcsncpy(lpszBuffer, _T("Burning"), dwMaxChars);
		break;

	case STATE_DRAGONFLIGHT:
		_tcsncpy(lpszBuffer, _T("Dragon Flight"), dwMaxChars);
		break;

	case STATE_MAUL:
		_tcsncpy(lpszBuffer, _T("Maul"), dwMaxChars);
		break;

	case STATE_CORPSE_NOSELECT:
		_tcsncpy(lpszBuffer, _T("Corpse No Select"), dwMaxChars);
		break;

	case STATE_SHADOWWARRIOR:
		_tcsncpy(lpszBuffer, _T("Shadow Warrior"), dwMaxChars);
		break;

	case STATE_FERALRAGE:
		_tcsncpy(lpszBuffer, _T("Feral Rage"), dwMaxChars);
		break;

	case STATE_SKILLDELAY:
		_tcsncpy(lpszBuffer, _T("Skill Delay"), dwMaxChars);
		break;

	case STATE_PROGRESSIVE_DAMAGE:
		_tcsncpy(lpszBuffer, _T("Progressive Damage"), dwMaxChars);
		break;

	case STATE_PROGRESSIVE_STEAL:
		_tcsncpy(lpszBuffer, _T("Progressive Steal"), dwMaxChars);
		break;

	case STATE_PROGRESSIVE_OTHER:
		_tcsncpy(lpszBuffer, _T("Progressive Other"), dwMaxChars);
		break;

	case STATE_PROGRESSIVE_FIRE:
		_tcsncpy(lpszBuffer, _T("Progressive Fire"), dwMaxChars);
		break;

	case STATE_PROGRESSIVE_COLD:
		_tcsncpy(lpszBuffer, _T("Progressive Cold"), dwMaxChars);
		break;

	case STATE_PROGRESSIVE_LIGHTNING:
		_tcsncpy(lpszBuffer, _T("Progressive Lightning"), dwMaxChars);
		break;

	case STATE_SHRINE_ARMOR:
		_tcsncpy(lpszBuffer, _T("Shrine Armor"), dwMaxChars);
		break;

	case STATE_SHRINE_COMBAT:
		_tcsncpy(lpszBuffer, _T("Shrine Combat"), dwMaxChars);
		break;

	case STATE_SHRINE_RESIST_LIGHTNING:
		_tcsncpy(lpszBuffer, _T("Shrine Resist Lightning"), dwMaxChars);
		break;

	case STATE_SHRINE_RESIST_FIRE:
		_tcsncpy(lpszBuffer, _T("Shrine Resist Fire"), dwMaxChars);
		break;

	case STATE_SHRINE_RESIST_POISON:
		_tcsncpy(lpszBuffer, _T("Shrine Resist Poison"), dwMaxChars);
		break;

	case STATE_SHRINE_SKILL:
		_tcsncpy(lpszBuffer, _T("Shrine Skill"), dwMaxChars);
		break;

	case STATE_SHRINE_MANA_REGEN:
		_tcsncpy(lpszBuffer, _T("Shrine Mana Regen"), dwMaxChars);
		break;

	case STATE_SHRINE_STAMINA:
		_tcsncpy(lpszBuffer, _T("Shrine Stamina"), dwMaxChars);
		break;

	case STATE_SHRINE_EXPERIENCE:
		_tcsncpy(lpszBuffer, _T("Shrine Experience"), dwMaxChars);
		break;

	case STATE_FENRIS_RAGE:
		_tcsncpy(lpszBuffer, _T("Fenris Rage"), dwMaxChars);
		break;

	case STATE_WOLF:
		_tcsncpy(lpszBuffer, _T("Wolf"), dwMaxChars);
		break;

	case STATE_BEAR:
		_tcsncpy(lpszBuffer, _T("Bear"), dwMaxChars);
		break;

	case STATE_BLOODLUST:
		_tcsncpy(lpszBuffer, _T("Blood Lust"), dwMaxChars);
		break;

	case STATE_CHANGECLASS:
		_tcsncpy(lpszBuffer, _T("Chance Class"), dwMaxChars);
		break;

	case STATE_ATTACHED:
		_tcsncpy(lpszBuffer, _T("Attached"), dwMaxChars);
		break;

	case STATE_HURRICANE:
		_tcsncpy(lpszBuffer, _T("Hurricane"), dwMaxChars);
		break;

	case STATE_ARMAGEDDON:
		_tcsncpy(lpszBuffer, _T("Armageddon"), dwMaxChars);
		break;

	case STATE_INVIS:
		_tcsncpy(lpszBuffer, _T("Invis"), dwMaxChars);
		break;

	case STATE_BARBS:
		_tcsncpy(lpszBuffer, _T("Barbs"), dwMaxChars);
		break;

	case STATE_WOLVERINE:
		_tcsncpy(lpszBuffer, _T("Wolverine"), dwMaxChars);
		break;

	case STATE_OAKSAGE:
		_tcsncpy(lpszBuffer, _T("Oak Sage"), dwMaxChars);
		break;

	case STATE_VINE_BEAST:
		_tcsncpy(lpszBuffer, _T("Battle Cry"), dwMaxChars);
		break;

	case STATE_CYCLONEARMOR:
		_tcsncpy(lpszBuffer, _T("Cyclone Armor"), dwMaxChars);
		break;

	case STATE_CLAWMASTERY:
		_tcsncpy(lpszBuffer, _T("Claw Mastery"), dwMaxChars);
		break;

	case STATE_CLOAK_OF_SHADOWS:
		_tcsncpy(lpszBuffer, _T("Cloak of Shadows"), dwMaxChars);
		break;

	case STATE_RECYCLED:
		_tcsncpy(lpszBuffer, _T("Recycled"), dwMaxChars);
		break;

	case STATE_WEAPONBLOCK:
		_tcsncpy(lpszBuffer, _T("Weapon Block"), dwMaxChars);
		break;

	case STATE_CLOAKED:
		_tcsncpy(lpszBuffer, _T("Cloaked"), dwMaxChars);
		break;

	case STATE_QUICKNESS:
		_tcsncpy(lpszBuffer, _T("Burst of Speed"), dwMaxChars);
		break;

	case STATE_BLADESHIELD:
		_tcsncpy(lpszBuffer, _T("Blade Shield"), dwMaxChars);
		break;

	case STATE_FADE:
		_tcsncpy(lpszBuffer, _T("Fade"), dwMaxChars);
		break;

	default:
		break;
	}
	
	return _tcslen(lpszBuffer);
}

BOOL D2IsAura(BYTE iAffectionID)
{
	BOOL bResult = FALSE;
	switch (iAffectionID)
	{
	case STATE_RESISTFIRE:
	case STATE_RESISTCOLD:
	case STATE_RESISTLIGHT:
	case STATE_RESISTALL: // salvation
	case STATE_CONVICTION:
	case STATE_MIGHT:
	case STATE_PRAYER:
	case STATE_HOLYFIRE:
	case STATE_HOLYWIND: // holy freeze
	case STATE_THORNS:
	case STATE_DEFIANCE:
	case STATE_BLESSEDAIM:
	case STATE_STAMINA: // vigor
	case STATE_CONCENTRATION:
	case STATE_CLEANSING:
	case STATE_HOLYSHOCK:
	case STATE_SANCTUARY:
	case STATE_MEDITATION:
	case STATE_FANATICISM:
	case STATE_REDEMPTION:
		bResult = TRUE;
		break;

	default:
		break;
	}

	return bResult;
}
