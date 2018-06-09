////////////////////////////////////////////////////////////////////////////
// TranslateItemMod.cpp
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////////////
#include "TranslateItemMod.h"

int Float2Int(int nVal, int nDiv);
int FindPoisionLenFrame(LPCITEMPROPERTY aList, DWORD dwCount);

int Float2Int(int nVal, int nDiv)
{
	if (nDiv == 0)
		return 0;

	float f = (float)nVal / (float)nDiv * MODVAL_MULTIPLIER;
	return *((int*)&f);
}

int FindPoisionLenFrame(LPCITEMPROPERTY aList, DWORD dwCount)
{
	if (aList == NULL)
		return 0;

	for (DWORD i = 0; i < dwCount; i++)
	{
		if (aList[i].wPropertyID == 59)
			return aList[i].nValue;
	}

	return 0;
}

BOOL TranslateItemMod(LPCITEMPROPERTY lpProperty, LPCITEMPROPERTY aList, DWORD dwCount, LPITEMMOD lpBuffer)
{
	if (lpProperty == NULL || lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(ITEMMOD));
	BOOL bOK = TRUE;

	// very common case
	lpBuffer->dwModID = lpProperty->wPropertyID;
	lpBuffer->dwSubID = lpProperty->wPropertyID;
	lpBuffer->bFloat = FALSE;
	lpBuffer->iValueCount = 1;
	lpBuffer->aValues[0] = (DWORD)lpProperty->nValue;

	// for poison mods
	int nLenFrame = 0;
	
	switch (lpProperty->wPropertyID)
	{
	case 0: //+%d to Strength
		//::strcpy(lpBuffer->szModString, "str");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Strength");
		break;

	case 1: //+%d to Energy
		//::strcpy(lpBuffer->szModString, "eng");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Energy");
		break;

	case 2: //+%d to Dexterity
		//::strcpy(lpBuffer->szModString, "dex");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Dexterity");
		break;

	case 3: //+%d to Vitality
		//::strcpy(lpBuffer->szModString, "vit");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Vitality");
		break;

	case 7: //+%d to Life
		//::strcpy(lpBuffer->szModString, "life");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Life");
		lpBuffer->aValues[0] /= 256;
		break;

	case 9: //+%d to Mana
		//::strcpy(lpBuffer->szModString, "mana");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Mana");
		lpBuffer->aValues[0] /= 256;
		break;

	case 11: //+%d to Maximum Stamina
		//::strcpy(lpBuffer->szModString, "stamina");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Stamina");
		lpBuffer->aValues[0] /= 256;
		break;

	case 16: //+%d%% Enhanced Defense		
		//::strcpy(lpBuffer->szModString, "defp");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% Enhanced Defense");
		break;

	case 17:
	case 18: //+%d%% Enhanced Damage
		lpBuffer->dwModID = 17;
		//::strcpy(lpBuffer->szModString, "ed");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% Enhanced Damage");
		break;

	case 19: //+%d to Attack Rating
		//::strcpy(lpBuffer->szModString, "ar");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Attack Rating");
		break;

	case 20: //+%d%% Increased Chance of Blocking
		//::strcpy(lpBuffer->szModString, "block");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% Increased Chance of Blocking");
		break;

	case 21: 
	case 23: //+%d to Minimum Damage
	case 159:
		lpBuffer->dwModID = 21;
		//::strcpy(lpBuffer->szModString, "min_dmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Minimum Damage");
		break;

	case 22: //+%d to Maximum Damage
	case 24:
	case 160:
		lpBuffer->dwModID = 22;
		//::strcpy(lpBuffer->szModString, "max_dmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Damage");
		break;

	case 27: //Regenerate Mana %d%%
		//::strcpy(lpBuffer->szModString, "mana_regen");
		//::strcpy(lpBuffer->szDescFormat, "Regenerate Mana %d%%");
		break;

	case 28: //Heal Stamina Plus %d%%
		//::strcpy(lpBuffer->szModString, "stamina_regen");
		//::strcpy(lpBuffer->szDescFormat, "Heal Stamina Plus %d%%");
		break;

	case 31: //+%d Defense
		//::strcpy(lpBuffer->szModString, "def");
		//::strcpy(lpBuffer->szDescFormat, "+%d Defense");
		break;

	case 32: //+%d Defense vs. Missile
		//::strcpy(lpBuffer->szModString, "def_missle");
		//::strcpy(lpBuffer->szDescFormat, "+%d Defense vs. Missile");
		break;

	case 33: //+%d Defense vs. Melee
		//::strcpy(lpBuffer->szModString, "def_melee");
		//::strcpy(lpBuffer->szDescFormat, "+%d Defense vs. Melee");
		break;

	case 34: //Damage Reduced by %d
		//::strcpy(lpBuffer->szModString, "dr");
		//::strcpy(lpBuffer->szDescFormat, "Damage Reduced by %d");
		break;

	case 35: //Magic Damage Reduced by %d
		//::strcpy(lpBuffer->szModString, "mdr");
		//::strcpy(lpBuffer->szDescFormat, "Magic Damage Reduced by %d");
		break;

	case 36: //Damage Reduced by %d%%
		//::strcpy(lpBuffer->szModString, "dmg_res");
		//::strcpy(lpBuffer->szDescFormat, "Damage Reduced by %d%%");
		break;

	case 37: //Magic Resist +%d%%
		//::strcpy(lpBuffer->szModString, "magic_res");
		//::strcpy(lpBuffer->szDescFormat, "Magic Resist +%d%%");
		break;

	case 38: //+%d%% to Maximum Magic Resist
		//::strcpy(lpBuffer->szModString, "magic_max");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Maximum Magic Resist");
		break;

	case 39: //Fire Resist +%d%%
		//::strcpy(lpBuffer->szModString, "fire_res");
		//::strcpy(lpBuffer->szDescFormat, "Fire Resist +%d%%");
		break;

	case 40: //+%d%% to Maximum Fire Resist
		//::strcpy(lpBuffer->szModString, "fire_max");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Maximum Fire Resist");
		break;

	case 41: //Lightning Resist +%d%%
		//::strcpy(lpBuffer->szModString, "lit_res");
		//::strcpy(lpBuffer->szDescFormat, "Lightning Resist +%d%%");
		break;

	case 42: //+%d%% to Maximum Lightning Resist
		//::strcpy(lpBuffer->szModString, "lit_max");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Maximum Lightning Resist");
		break;

	case 43: //Cold Resist +%d%%
		//::strcpy(lpBuffer->szModString, "cold_res");
		//::strcpy(lpBuffer->szDescFormat, "Cold Resist +%d%%");
		break;

	case 44: //+%d%% to Maximum Cold Resist
		//::strcpy(lpBuffer->szModString, "cold_max");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Maximum Cold Resist");
		break;

	case 45: //Poison Resist +%d%%
		//::strcpy(lpBuffer->szModString, "poison_res");
		//::strcpy(lpBuffer->szDescFormat, "Poison Resist +%d%%");
		break;

	case 46: //+%d%% to Maximum Poison Resist
		//::strcpy(lpBuffer->szModString, "poison_max");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Maximum Poison Resist");
		break;

	case 48: //+%d minimum fire damage
		//::strcpy(lpBuffer->szModString, "fire_mindmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d minimum fire damage");
		break;

	case 49: //+%d maximum fire damage
		//::strcpy(lpBuffer->szModString, "fire_maxdmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d maximum fire damage");
		break;

	case 50: //+%d minimum lightning damage
		//::strcpy(lpBuffer->szModString, "lit_mindmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d minimum lightning damage");
		break;

	case 51: //+%d maximum lightning damage
		//::strcpy(lpBuffer->szModString, "lit_maxdmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d maximum lightning damage");
		break;

	case 52: //+%d minimum magic damage
		//::strcpy(lpBuffer->szModString, "magic_mindmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d minimum magic damage");
		break;

	case 53: //+%d maximum magic damage
		//::strcpy(lpBuffer->szModString, "magic_maxdmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d maximum magic damage");
		break;

	case 54: //+%d minimum cold damage
		//::strcpy(lpBuffer->szModString, "cold_mindmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d minimum cold damage");
		break;

	case 55: //+%d maximum cold damage
		//::strcpy(lpBuffer->szModString, "cold_maxdmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d maximum cold damage");
		break;

	case 56: //%d seconds cold duration
		//::strcpy(lpBuffer->szModString, "cold_len");
		//::strcpy(lpBuffer->szDescFormat, "%d seconds cold duration");
		lpBuffer->aValues[0] /= 25;
		break;

	case 57:
		//::strcpy(lpBuffer->szModString, "poison_mindmg");
		//::strcpy(lpBuffer->szDescFormat, "");
		lpBuffer->aValues[0] *= FindPoisionLenFrame(aList, dwCount);
		lpBuffer->aValues[0] /= 256;
		break;

	case 58:
		//::strcpy(lpBuffer->szModString, "poison_maxdmg");
		//::strcpy(lpBuffer->szDescFormat, "");
		lpBuffer->aValues[0] *= FindPoisionLenFrame(aList, dwCount);
		lpBuffer->aValues[0] /= 256;
		break;

	case 59: // //%d seconds poison duration
		//::strcpy(lpBuffer->szModString, "poison_len");
		//::strcpy(lpBuffer->szDescFormat, "%d seconds poison duration");
		lpBuffer->aValues[0] /= 25;
		break;

	case 60: //%d%% Life stolen per hit
		//::strcpy(lpBuffer->szModString, "life_leech");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Life stolen per hit");
		break;

	case 62: //%d%% Mana stolen per hit
		//::strcpy(lpBuffer->szModString, "mana_leech");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Mana stolen per hit");
		break;

	case 67: //%d%% Faster Run/Walk
	case 96: 
		lpBuffer->dwModID = 67;
		//::strcpy(lpBuffer->szModString, "frw");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Faster Run/Walk");
		break;

	case 68:
	case 93: //%d%% Increased Attack Speed
		lpBuffer->dwModID = 68;
		//::strcpy(lpBuffer->szModString, "ias");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Increased Attack Speed");
		break;

	case 74: //Replenish Life +%d
		//::strcpy(lpBuffer->szModString, "life_regen");
		//::strcpy(lpBuffer->szDescFormat, "Replenish Life +%d");
		break;

	case 75: //Increase Maximum Durability %d%%
		//::strcpy(lpBuffer->szModString, "dura_max");
		//::strcpy(lpBuffer->szDescFormat, "Increase Maximum Durability %d%%");
		break;

	case 76: //Increase Maximum Life %d%%
		//::strcpy(lpBuffer->szModString, "life_max");
		//::strcpy(lpBuffer->szDescFormat, "Increase Maximum Life %d%%");
		break;

	case 77: //Increase Maximum Mana %d%%
		//::strcpy(lpBuffer->szModString, "mana_max");
		//::strcpy(lpBuffer->szDescFormat, "Increase Maximum Mana %d%%");
		break;

	case 78: //Attacker Takes Damage of %d
		//::strcpy(lpBuffer->szModString, "atd");
		//::strcpy(lpBuffer->szDescFormat, "Attacker Takes Damage of %d");
		break;

	case 79: //%d%% Extra Gold from Monsters
		//::strcpy(lpBuffer->szModString, "gold");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Extra Gold from Monsters");
		break;

	case 80: //%d%% Better Chance of Getting Magic Items
		//::strcpy(lpBuffer->szModString, "mf");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Better Chance of Getting Magic Items");
		break;

	case 81: //Knockback
		//::strcpy(lpBuffer->szModString, "knockback");
		//::strcpy(lpBuffer->szDescFormat, "Knockback");
		break;

	case 83: //+%d to %d(class) Skill Levels
		//::strcpy(lpBuffer->szModString, "class_skill");
		lpBuffer->iValueCount = 2;
		lpBuffer->aValues[1] = lpProperty->wSubProperty; 
		//::strcpy(lpBuffer->szDescFormat, "+%d to %d(class) Skill Levels");
		break;

	case 85: //+%d%% to Experience Gained
		//::strcpy(lpBuffer->szModString, "exp");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Experience Gained");
		break;

	case 86: //+%d Life after each Kill
		//::strcpy(lpBuffer->szModString, "life_aek");
		//::strcpy(lpBuffer->szDescFormat, "+%d Life after each Kill");
		break;

	case 87: //Reduces All Vendor Prices %d%%
		//::strcpy(lpBuffer->szModString, "ravp");
		//::strcpy(lpBuffer->szDescFormat, "Reduces All Vendor Prices %d%%");
		break;

	case 89: //+%d to Light Radius
		//::strcpy(lpBuffer->szModString, "radius");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Light Radius");
		break;

	case 91: //Requirements -%d%%
		//::strcpy(lpBuffer->szModString, "req");
		lpBuffer->aValues[0] = 0 - lpBuffer->aValues[0];
		//::strcpy(lpBuffer->szDescFormat, "Requirements -%d%%");
		break;

	case 99: //%d%% Faster Hit Recovery
		//::strcpy(lpBuffer->szModString, "fhr");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Faster Hit Recovery");
		break;

	case 102: //%d%% Faster Block Rate
		//::strcpy(lpBuffer->szModString, "fbr");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Faster Block Rate");
		break;

	case 105: //%d%% Faster Cast Rate
		//::strcpy(lpBuffer->szModString, "fcr");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Faster Cast Rate");
		break;

	case 107: //+%d to Skill(%d) (??? Only)
		//::strcpy(lpBuffer->szModString, "skill");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Skill(%d) (<Class> Only)");
		lpBuffer->iValueCount = 2;
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;

	case 108: //Slain Monsters Rest In Peace
		//::strcpy(lpBuffer->szModString, "peace");
		//::strcpy(lpBuffer->szDescFormat, "Slain Monsters Rest In Peace");
		break;

	case 110: //Poison Length Reduced by %d%%
		//::strcpy(lpBuffer->szModString, "plr");
		//::strcpy(lpBuffer->szDescFormat, "Poison Length Reduced by %d%%");
		break;

	case 111: //Adds +%d Damage
		//::strcpy(lpBuffer->szModString, "add_dmg");
		//::strcpy(lpBuffer->szDescFormat, "Adds +%d Damage");
		break;

	case 112: //Hit Causes Monster to Flee %d%%
		//::strcpy(lpBuffer->szModString, "flee");
		//::strcpy(lpBuffer->szDescFormat, "Hit Causes Monster to Flee %d%%");
		lpBuffer->aValues[0] = lpBuffer->aValues[0] * 100 / 128;
		break;

	case 113: //Hit Blinds Target +%d
		//::strcpy(lpBuffer->szModString, "blind");
		//::strcpy(lpBuffer->szDescFormat, "Hit Blinds Target +%d");
		break;

	case 114: //%d%% Damage Taken Goes to Mana
		//::strcpy(lpBuffer->szModString, "dmg_mana");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Damage Taken Goes to Mana");
		break;

	case 115: //Ignore Target Defense
		//::strcpy(lpBuffer->szModString, "itd");
		//::strcpy(lpBuffer->szDescFormat, "Ignore Target Defense");
		break;

	case 116: //-%d%% Target Defense
		//::strcpy(lpBuffer->szModString, "tgt_def");
		//::strcpy(lpBuffer->szDescFormat, "-%d%% Target Defense");
		break;

	case 117: //Prevent Monster Heal
		//::strcpy(lpBuffer->szModString, "pmh");
		//::strcpy(lpBuffer->szDescFormat, "Prevent Monster Heal");
		break;

	case 118: //Half Freeze Duration
		//::strcpy(lpBuffer->szModString, "hfd");
		//::strcpy(lpBuffer->szDescFormat, "Half Freeze Duration");
		break;

	case 119: //%d%% Bonus to Attack Rating
		//::strcpy(lpBuffer->szModString, "bonus_ar");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Bonus to Attack Rating");
		break;

	case 120: //-%d to Monster Defense Per Hit
		//::strcpy(lpBuffer->szModString, "mdph");
		lpBuffer->aValues[0] = 0 - lpBuffer->aValues[0];
		//::strcpy(lpBuffer->szDescFormat, "-%d to Monster Defense Per Hit");
		break;

	case 121: //%d%% Damage to Demons
		//::strcpy(lpBuffer->szModString, "dmg_demon");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Damage to Demons");
		break;

	case 122: //%d%% Damage to Undead
		//::strcpy(lpBuffer->szModString, "dmg_undead");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Damage to Undead");
		break;

	case 123: //+%d to Attack Rating against Demons
		//::strcpy(lpBuffer->szModString, "ar_demon");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Attack Rating against Demons");
		break;

	case 124: //+%d to Attack Rating against Undead
		//::strcpy(lpBuffer->szModString, "ar_undead");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Attack Rating against Undead");
		break;

	case 125: //Throwable
		//::strcpy(lpBuffer->szModString, "throw");
		//::strcpy(lpBuffer->szDescFormat, "Throwable");
		break;

	case 126: // //+%d to %d(elemental type) Skills
		//::strcpy(lpBuffer->szModString, "ele_skill");
		//::strcpy(lpBuffer->szDescFormat, "+%d to %d(elemental type) Skills");
		lpBuffer->iValueCount = 2;
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;

	case 127: //+%d to All Skill Levels
		//::strcpy(lpBuffer->szModString, "all_skill");
		//::strcpy(lpBuffer->szDescFormat, "+%d to All Skill Levels");
		break;

	case 128: //Attacker Takes Lightning Damage of %d
		//::strcpy(lpBuffer->szModString, "atld");
		//::strcpy(lpBuffer->szDescFormat, "Attacker Takes Lightning Damage of %d");
		break;

	case 134: //Freezes Target
		//::strcpy(lpBuffer->szModString, "frz_target");
		//::strcpy(lpBuffer->szDescFormat, "Freezes Target");
		break;

	case 135: //%d%% Chance of Open Wounds
		//::strcpy(lpBuffer->szModString, "ow");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Chance of Open Wounds");
		break;

	case 136: //%d%% Chance of Crushing Blow
		//::strcpy(lpBuffer->szModString, "cb");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Chance of Crushing Blow");
		break;

	case 137: //+%d Kick Damage
		//::strcpy(lpBuffer->szModString, "kick_dmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d Kick Damage");
		break;

	case 138: //+%d to Mana After Each Kill
		//::strcpy(lpBuffer->szModString, "mana_aek");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Mana After Each Kill");
		break;

	case 139: //+%d Life after each Demon Kill
		//::strcpy(lpBuffer->szModString, "life_aedk");
		//::strcpy(lpBuffer->szDescFormat, "+%d Life after each Demon Kill");
		break;

	case 141: //%d%% Deadly Strike
		//::strcpy(lpBuffer->szModString, "ds");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Deadly Strike");
		break;

	case 142: //%d%% Fire Absorb
		//::strcpy(lpBuffer->szModString, "fire_absp");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Fire Absorb");
		break;

	case 143: //+%d Fire Absorb
		//::strcpy(lpBuffer->szModString, "fire_abs");
		//::strcpy(lpBuffer->szDescFormat, "+%d Fire Absorb");
		break;

	case 144: //%d%% Lightning Absorb
		//::strcpy(lpBuffer->szModString, "lit_absp");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Lightning Absorb");
		break;

	case 145: //+%d Lightning Absorb
		//::strcpy(lpBuffer->szModString, "lit_abs");
		//::strcpy(lpBuffer->szDescFormat, "+%d Lightning Absorb");
		break;

	case 146: //%d%% Magic Absorb
		//::strcpy(lpBuffer->szModString, "magic_absp");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Magic Absorb");
		break;

	case 147: //+%d Magic Absorb
		//::strcpy(lpBuffer->szModString, "magic_abs");
		//::strcpy(lpBuffer->szDescFormat, "+%d Magic Absorb");
		break;

	case 148: //%d%% Cold Absorb
		//::strcpy(lpBuffer->szModString, "cold_absp");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Cold Absorb");
		break;

	case 149: //+%d Cold Absorb
		//::strcpy(lpBuffer->szModString, "cold_abs");
		//::strcpy(lpBuffer->szDescFormat, "+%d Cold Absorb");
		break;

	case 150: //Slows Target by %d%%
		//::strcpy(lpBuffer->szModString, "slow");
		//::strcpy(lpBuffer->szDescFormat, "Slows Target by %d%%");
		break;

	case 151: // Level %d %d Aura When Equipped
		//::strcpy(lpBuffer->szModString, "aura");
		//::strcpy(lpBuffer->szDescFormat, "Level %d %d Aura When Equipped");
		lpBuffer->iValueCount = 2;
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;

	case 152: //Indestructible
		//::strcpy(lpBuffer->szModString, "Indestructible");
		//::strcpy(lpBuffer->szDescFormat, "Indestructible");
		break;

	case 153: //Cannot Be Frozen
		//::strcpy(lpBuffer->szModString, "cbf");
		//::strcpy(lpBuffer->szDescFormat, "Cannot Be Frozen");
		break;

	case 154: //%d%% Slower Stamina Drain
		//::strcpy(lpBuffer->szModString, "ssd");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Slower Stamina Drain");
		break;

	case 155: //%d%% Reanimate As: %d
		//::strcpy(lpBuffer->szModString, "reanimate");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Reanimate As: %d");
		lpBuffer->iValueCount = 2;
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;	

	case 156: //Piercing Attack [%d]
		//::strcpy(lpBuffer->szModString, "pierce");
		//::strcpy(lpBuffer->szDescFormat, "Piercing Attack [%d]");
		break;

	case 157: //Fires Magic Arrows
		//::strcpy(lpBuffer->szModString, "magic_arrow");
		//::strcpy(lpBuffer->szDescFormat, "Fires Magic Arrows");
		break;

	case 158: //Fires Explosive Arrows
		//::strcpy(lpBuffer->szModString, "explosive_arrow");
		//::strcpy(lpBuffer->szDescFormat, "Fires Explosive Arrows");
		break;

	case 188: //+%d SkillTable(%d) (??? Only)
		//::strcpy(lpBuffer->szModString, "skill_tab");
		//::strcpy(lpBuffer->szDescFormat, "+%d SkillTable(%d) (<Class> Only)");
		lpBuffer->iValueCount = 2;
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;	

	case 195: //%d%% Chance to cast Level %d %d on attack
		//::strcpy(lpBuffer->szModString, "cast_attack");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Chance to cast Level %d %d on attack");
		lpBuffer->iValueCount = 3;
		lpBuffer->aValues[0] = lpProperty->nValue & 0x000000ff;
		lpBuffer->aValues[1] = lpProperty->wSubProperty & 0x000f;
		lpBuffer->aValues[2] = lpProperty->wSubProperty >> 6;
		break;

	case 196: //%d%% Chance to cast Level %d %d When You Kill an Enemy
		//::strcpy(lpBuffer->szModString, "cast_kill");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Chance to cast Level %d %d When You Kill an Enemy");
		lpBuffer->iValueCount = 3;
		lpBuffer->aValues[0] = lpProperty->nValue & 0x000000ff;
		lpBuffer->aValues[1] = lpProperty->wSubProperty & 0x000f;
		lpBuffer->aValues[2] = lpProperty->wSubProperty >> 6;
		break;

	case 197: //%d%% Chance to cast Level %d %d When You Die
		//::strcpy(lpBuffer->szModString, "cast_die");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Chance to cast Level %d %d When You Die");
		lpBuffer->iValueCount = 3;
		lpBuffer->aValues[0] = lpProperty->nValue & 0x000000ff;
		lpBuffer->aValues[1] = lpProperty->wSubProperty & 0x000f;
		lpBuffer->aValues[2] = lpProperty->wSubProperty >> 6;
		break;

	case 198: //%d%% Chance to cast Level %d %d on striking
		//::strcpy(lpBuffer->szModString, "cast_striking");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Chance to cast Level %d %d on striking");
		lpBuffer->iValueCount = 3;
		lpBuffer->aValues[0] = lpProperty->nValue & 0x000000ff;
		lpBuffer->aValues[1] = lpProperty->wSubProperty & 0x000f;
		lpBuffer->aValues[2] = lpProperty->wSubProperty >> 6;

		break;	

	case 199: //%d%% Chance to cast Level %d %d When You Level Up
		//::strcpy(lpBuffer->szModString, "cast_level");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Chance to cast Level %d %d When You Level Up");
		lpBuffer->iValueCount = 3;
		lpBuffer->aValues[0] = lpProperty->nValue & 0x000000ff;
		lpBuffer->aValues[1] = lpProperty->wSubProperty & 0x000f;
		lpBuffer->aValues[2] = lpProperty->wSubProperty >> 6;
		break;

	case 201: //%d%% Chance to cast Level %d %d when struck
		//::strcpy(lpBuffer->szModString, "cast_struck");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Chance to cast Level %d %d when struck");
		lpBuffer->iValueCount = 3;
		lpBuffer->aValues[0] = lpProperty->nValue & 0x000000ff;
		lpBuffer->aValues[1] = lpProperty->wSubProperty & 0x000f;
		lpBuffer->aValues[2] = lpProperty->wSubProperty >> 6;
		break;

	case 204: //Level %d %d (%d/%d Charges)
		//::strcpy(lpBuffer->szModString, "skill_charge");
		//::strcpy(lpBuffer->szDescFormat, "Level %d %d (%d/%d Charges)");
		lpBuffer->iValueCount = 4;
		lpBuffer->aValues[0] = lpProperty->wSubProperty & 0x000f; // level
		lpBuffer->aValues[1] = lpProperty->wSubProperty >> 6; // skill id
		lpBuffer->aValues[2] = lpProperty->nValue & 0x000000ff; // remaining charges
		lpBuffer->aValues[3] = (lpProperty->nValue & 0x0000ff00) >> 8; // full charges
		break;

	case 214: //+%d to Defense (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_def");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Defense (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;	

	case 215: //+%d%% to Defense (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_defp");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Defense (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 216: //+%d to Life (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_life");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Life (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 2048);
		lpBuffer->bFloat = TRUE;
		break;

	case 217: //+%d to Mana (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_mana");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Mana (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 2048);
		lpBuffer->bFloat = TRUE;
		break;

	case 218: //+%d to Maximum Damage (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_dmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Damage (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;	

	case 219: //+%d%% to Maximum Damage (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_dmgp");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Maximum Damage (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 220: //+%d to Strength (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_str");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Strength (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 221: //+%d to Dexterity (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_dex");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Dexterity (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 222: //+%d to Energy (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_eng");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Energy (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;	

	case 223: //+%d to Vitality (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_vit");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Vitality (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 224: //+%d to Attack Rating (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_ar");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Attack Rating (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;	

	case 225: //%d%% Bonus to Attack Rating (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_bonusar");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Bonus to Attack Rating (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 226: //+%d to Maximum Cold Damage (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_colddmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Cold Damage (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 227: //+%d to Maximum Fire Damage (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_firedmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Fire Damage (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 228: //+%d to Maximum Lightning Damage (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_litdmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Lightning Damage (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;	

	case 229: //+%d to Maximum Poison Damage (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_poisondmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Poison Damage (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 230: //+%d to Maximum Cold Resist (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_coldres");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Cold Resist (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 231: //+%d to Maximum Fire Resist (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_fireres");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Fire Resist (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 232: //+%d to Maximum Resist Damage (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_dmgres");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Resist Damage (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;	

	case 233: //+%d to Maximum Poison Resist (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_poisonres");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Poison Resist (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 234: //+%d to Cold Absorb (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_coldabs");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Cold Absorb (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 235: //+%d to Fire Absorb (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_fireabs");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Fire Absorb (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 236: //+%d to Lightning Absorb (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_litabs");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Lightning Absorb (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 237: //+%d to Poison Absorb (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_poisonabs");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Poison Absorb (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 238: //Attacker Takes Damage of %d (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_atd");
		//::strcpy(lpBuffer->szDescFormat, "Attacker Takes Damage of %d (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 239: //%d%% Extra Gold (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_gold");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Extra Gold (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 240: //%d%% Better Chance of Getting Magic Items (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_mf");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Better Chance of Getting Magic Items (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 241: //Heal Stamina Plus %d%% (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_hsp");
		//::strcpy(lpBuffer->szDescFormat, "Heal Stamina Plus %d%% (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 242: //+%d to Maximum Stamina (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_stamina");
		//::strcpy(lpBuffer->szDescFormat, "+%d to Maximum Stamina (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 2048);
		lpBuffer->bFloat = TRUE;
		break;

	case 243: //%d%% Damage to Demons (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_dmgdemon");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Damage to Demons (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 244: //%d%% Damage to Undead (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_dmgundead");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Damage to Undead (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 245: //%d%% Bonus to Attack Rating Against Demons (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_ardemon");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Bonus to Attack Rating Against Demons (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 246: //%d%% Bonus to Attack Rating Against Undead (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_arundead");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Bonus to Attack Rating Against Undead (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 247: //%d%% Crushing Blow (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_cb");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Crushing Blow (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 248: //%d%% Open Wounds (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_ow");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Open Wounds (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 249: //%d Kick Damage (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_kickdmg");
		//::strcpy(lpBuffer->szDescFormat, "%d Kick Damage (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 250: //%d%% Deadly Strike (Based on Character Level)
		//::strcpy(lpBuffer->szModString, "bocl_ds");
		//::strcpy(lpBuffer->szDescFormat, "%d%% Deadly Strike (Based on Character Level)");
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;

	case 252: //Repairs 1 durability in %d seconds
		//::strcpy(lpBuffer->szModString, "rep_dura");
		//::strcpy(lpBuffer->szDescFormat, "Repairs 1 durability in %d seconds");
		lpBuffer->aValues[0] = 100 / lpBuffer->aValues[0];
		lpBuffer->bFloat = TRUE;
		break;

	case 253: //Replenishes Quantity
		//::strcpy(lpBuffer->szModString, "qty_regen");
		//::strcpy(lpBuffer->szDescFormat, "Replenishes Quantity");
		break;

	case 254: //Increased Stack Size
		//::strcpy(lpBuffer->szModString, "stack");
		//::strcpy(lpBuffer->szDescFormat, "Increased Stack Size");
		break;	

	case 329: //+%d%% to Fire Skill Damage
		//::strcpy(lpBuffer->szModString, "fire_skilldmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Fire Skill Damage");
		break;

	case 330: //+%d%% to Lightning Skill Damage
		//::strcpy(lpBuffer->szModString, "lit_skilldmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Lightning Skill Damage");
		break;

	case 331: //+%d%% to Cold Skill Damage
		//::strcpy(lpBuffer->szModString, "cold_skilldmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Cold Skill Damage");
		break;

	case 332: //+%d%% to Poison Skill Damage
		//::strcpy(lpBuffer->szModString, "poison_skilldmg");
		//::strcpy(lpBuffer->szDescFormat, "+%d%% to Poison Skill Damage");
		break;	

	case 333: //-%d%% to Enemy Fire Resistance
		//::strcpy(lpBuffer->szModString, "enemy_fireres");
		//::strcpy(lpBuffer->szDescFormat, "-%d%% to Enemy Fire Resistance");
		break;

	case 334: //-%d%% to Enemy Lightning Resistance
		//::strcpy(lpBuffer->szModString, "enemy_litres");
		//::strcpy(lpBuffer->szDescFormat, "-%d%% to Enemy Lightning Resistance");
		break;

	case 335: //-%d%% to Enemy Cold Resistance
		//::strcpy(lpBuffer->szModString, "enemy_coldres");
		//::strcpy(lpBuffer->szDescFormat, "-%d%% to Enemy Cold Resistance");
		break;

	case 336: //-%d%% to Enemy Poison Resistance
		//::strcpy(lpBuffer->szModString, "enemy_poisonres");
		//::strcpy(lpBuffer->szDescFormat, "-%d%% to Enemy Poison Resistance");
		break;

	case 356: //Quest Item Difficulty: %d
		//::strcpy(lpBuffer->szModString, "quest_dif");
		//::strcpy(lpBuffer->szDescFormat, "Quest Item Difficulty: %d");
		break;

	default:
		bOK = FALSE;
		break;
	}

	return bOK;
}