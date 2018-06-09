////////////////////////////////////////////////////////////////////////////
// TranslateItemMod.cpp
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////////////
#include "TranslateItemMod.h"

#define MODVAL_MULTIPLIER	1000.0

int Float2Int(int nVal, int nDiv);
int GetCoExistProperty(DWORD dwPropertyID, LPCITEMPROPERTY aList, DWORD dwCount, int nDefault);
int RoundUp(double fValue);

int Float2Int(int nVal, int nDiv)
{
	if (nDiv == 0)
		return 0;

	double f = (double)nVal / (double)nDiv * MODVAL_MULTIPLIER;
	return (int)f;
}

int GetCoExistProperty(DWORD dwPropertyID, LPCITEMPROPERTY aList, DWORD dwCount, int nDefault)
{
	if (aList == NULL)
		return nDefault;

	for (DWORD i = 0; i < dwCount; i++)
	{
		if (aList[i].wPropertyID == dwPropertyID)
			return aList[i].nValue;
	}

	return nDefault;
}

int RoundUp(double fValue)
{
	return (int)(fValue + 0.5);
}

BOOL TranslateItemMod(LPCITEMPROPERTY lpProperty, LPCITEMPROPERTY aList, DWORD dwCount, LPITEMMOD lpBuffer)
{
	if (lpProperty == NULL || lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(ITEMMOD));
	BOOL bOK = TRUE;

	// very common case
	lpBuffer->dwModID = lpProperty->wPropertyID;
	lpBuffer->dwOriginalID = lpProperty->wPropertyID;
	lpBuffer->bFloat = FALSE;
	lpBuffer->aValues[0] = (DWORD)lpProperty->nValue;
	int nTemp = 0;

	switch (lpProperty->wPropertyID)
	{

	//////////////////////////////////////////////////////////////
	// Common Cases, No Changes Required
	//////////////////////////////////////////////////////////////
		
	case 0: //+%d to Strength
	case 1: //+%d to Energy
	case 2: //+%d to Dexterity
	case 3: //+%d to Vitality
	case 16: //+%d%% Enhanced Defense		
	case 19: //+%d to Attack Rating
	case 20: //+%d%% Increased Chance of Blocking
	case 27: //Regenerate Mana %d%%
	case 28: //Heal Stamina Plus %d%%
	case 31: //+%d Defense
	case 32: //+%d Defense vs. Missile
	case 33: //+%d Defense vs. Melee
	case 34: //Damage Reduced by %d
	case 35: //Magic Damage Reduced by %d
	case 36: //Damage Reduced by %d%%
	case 37: //Magic Resist +%d%%
	case 38: //+%d%% to Maximum Magic Resist
	case 39: //Fire Resist +%d%%
	case 40: //+%d%% to Maximum Fire Resist
	case 41: //Lightning Resist +%d%%
	case 42: //+%d%% to Maximum Lightning Resist
	case 43: //Cold Resist +%d%%
	case 44: //+%d%% to Maximum Cold Resist
	case 45: //Poison Resist +%d%%
	case 46: //+%d%% to Maximum Poison Resist	
	case 49: //+%d maximum fire damage	
	case 51: //+%d maximum lightning damage	
	case 53: //+%d maximum magic damage	
	case 55: //+%d maximum cold damage
	case 58: //%d maximum poison damage
	case 60: //%d%% Life stolen per hit
	case 62: //%d%% Mana stolen per hit
	case 74: //Replenish Life +%d
	case 75: //Increase Maximum Durability %d%%
	case 76: //Increase Maximum Life %d%%
	case 77: //Increase Maximum Mana %d%%
	case 78: //Attacker Takes Damage of %d
	case 79: //%d%% Extra Gold from Monsters
	case 80: //%d%% Better Chance of Getting Magic Items
	case 81: //Knockback
	case 85: //+%d%% to Experience Gained
	case 86: //+%d Life after each Kill
	case 87: //Reduces All Vendor Prices %d%%
	case 89: //+%d to Light Radius
	case 99: //%d%% Faster Hit Recovery
	case 102: //%d%% Faster Block Rate
	case 105: //%d%% Faster Cast Rate
	case 108: //Slain Monsters Rest In Peace
	case 110: //Poison Length Reduced by %d%%
	case 111: //Adds +%d Damage
	case 113: //Hit Blinds Target +%d
	case 114: //%d%% Damage Taken Goes to Mana
	case 115: //Ignore Target Defense
	case 116: //-%d%% Target Defense
	case 117: //Prevent Monster Heal
	case 118: //Half Freeze Duration
	case 119: //%d%% Bonus to Attack Rating
	case 121: //%d%% Damage to Demons
	case 122: //%d%% Damage to Undead
	case 123: //+%d to Attack Rating against Demons
	case 124: //+%d to Attack Rating against Undead
	case 125: //Throwable
	case 127: //+%d to All Skill Levels
	case 128: //Attacker Takes Lightning Damage of %d
	case 134: //Freezes Target
	case 135: //%d%% Chance of Open Wounds
	case 136: //%d%% Chance of Crushing Blow
	case 137: //+%d Kick Damage
	case 138: //+%d to Mana After Each Kill
	case 139: //+%d Life after each Demon Kill
	case 141: //%d%% Deadly Strike
	case 142: //%d%% Fire Absorb
	case 143: //+%d Fire Absorb
	case 144: //%d%% Lightning Absorb
	case 145: //+%d Lightning Absorb
	case 146: //%d%% Magic Absorb
	case 147: //+%d Magic Absorb
	case 148: //%d%% Cold Absorb
	case 149: //+%d Cold Absorb
	case 150: //Slows Target by %d%%
	case 152: //Indestructible
	case 153: //Cannot Be Frozen
	case 154: //%d%% Slower Stamina Drain
	case 156: //Piercing Attack [%d]
	case 157: //Fires Magic Arrows
	case 158: //Fires Explosive Arrows
	case 253: //Replenishes Quantity
	case 254: //Increased Stack Size
	case 329: //+%d%% to Fire Skill Damage
	case 330: //+%d%% to Lightning Skill Damage
	case 331: //+%d%% to Cold Skill Damage
	case 332: //+%d%% to Poison Skill Damage
	case 333: //-%d%% to Enemy Fire Resistance
	case 334: //-%d%% to Enemy Lightning Resistance
	case 335: //-%d%% to Enemy Cold Resistance
	case 336: //-%d%% to Enemy Poison Resistance
	case 356: //Quest Item Difficulty: %d
		break;


	//////////////////////////////////////////////////////////////
	// Special Cases Cases
	//////////////////////////////////////////////////////////////						

	case 7: //+%d to Life
	case 9: //+%d to Mana
	case 11: //+%d to Maximum Stamina
		lpBuffer->aValues[0] /= 256;
		break;

	case 17:
	case 18: //+%d%% Enhanced Damage
		lpBuffer->dwModID = 17;
		break;

	case 21: 
	case 23: //+%d to Minimum Damage
	case 159:
		lpBuffer->dwModID = 21;
		break;

	case 22: //+%d to Maximum Damage
	case 24:
	case 160:
		lpBuffer->dwModID = 22;
		break;	

	case 48: //Adds %d to %d fire damage
		lpBuffer->aValues[1] = GetCoExistProperty(49, aList, dwCount, lpBuffer->aValues[0]);
		break;

	case 50: //Adds %d to %d lightning damage
		lpBuffer->aValues[1] = GetCoExistProperty(51, aList, dwCount, lpBuffer->aValues[0]);
		break;
		
	case 52: //Adds %d to %d magic damage
		lpBuffer->aValues[1] = GetCoExistProperty(53, aList, dwCount, lpBuffer->aValues[0]);
		break;

	case 54: //Adds %d to %d cold damage
		lpBuffer->aValues[1] = GetCoExistProperty(55, aList, dwCount, lpBuffer->aValues[0]);
		lpBuffer->aValues[2] = GetCoExistProperty(56, aList, dwCount, 25) / 25;
		break;

	case 57: //%d minimum poison damage	
		nTemp = GetCoExistProperty(59, aList, dwCount, 25);
		lpBuffer->aValues[0] = RoundUp((double)lpBuffer->aValues[0] * (double)nTemp / 256.0);
		lpBuffer->aValues[1] = RoundUp((double)GetCoExistProperty(58, aList, dwCount, lpBuffer->aValues[0]) * (double)nTemp / 256.0);
		lpBuffer->aValues[2] = nTemp / 25;
		break;

	case 56: //%d seconds cold duration
	case 59: // //%d seconds poison duration
		lpBuffer->aValues[0] /= 25;
		break;	

	case 67: //%d%% Faster Run/Walk
	case 96: 
		lpBuffer->dwModID = 67;
		break;

	case 68:
	case 93: //%d%% Increased Attack Speed
		lpBuffer->dwModID = 68;
		break;

	case 83: //+%d to %d(class) Skill Levels
		lpBuffer->aValues[1] = lpProperty->wSubProperty; 
		break;	

	case 91: //Requirements -%d%%
		lpBuffer->aValues[0] = 0 - lpBuffer->aValues[0];
		break;	

	case 107: //+%d to Skill(%d) (??? Only)
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;	

	case 112: //Hit Causes Monster to Flee %d%%
		lpBuffer->aValues[0] = lpBuffer->aValues[0] * 100 / 128; // 0x80 based percentage
		break;	

	case 120: //-%d to Monster Defense Per Hit
		lpBuffer->aValues[0] = 0 - lpBuffer->aValues[0];
		break;	

	case 126: // //+%d to %d(elemental type) Skills
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;	

	case 151: // Level %d %d Aura When Equipped
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;	

	case 155: //%d%% Reanimate As: %d
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;		

	case 188: //+%d SkillTable(%d) (??? Only)
		lpBuffer->aValues[1] = lpProperty->wSubProperty;
		break;	

	case 195: //%d%% Chance to cast Level %d %d on attack
	case 196: //%d%% Chance to cast Level %d %d When You Kill an Enemy
	case 197: //%d%% Chance to cast Level %d %d When You Die
	case 198: //%d%% Chance to cast Level %d %d on striking
	case 199: //%d%% Chance to cast Level %d %d When You Level Up
	case 201: //%d%% Chance to cast Level %d %d when struck
		lpBuffer->aValues[0] = lpProperty->nValue & 0x000000ff;
		lpBuffer->aValues[1] = lpProperty->wSubProperty & 0x000f;
		lpBuffer->aValues[2] = lpProperty->wSubProperty >> 6;
		break;

	case 204: //Level %d %d (%d/%d Charges)
		lpBuffer->aValues[0] = lpProperty->wSubProperty & 0x000f; // level
		lpBuffer->aValues[1] = lpProperty->wSubProperty >> 6; // skill id
		lpBuffer->aValues[2] = lpProperty->nValue & 0x000000ff; // remaining charges
		lpBuffer->aValues[3] = (lpProperty->nValue & 0x0000ff00) >> 8; // full charges
		break;

	case 216: //+%d to Life (Based on Character Level)
	case 217: //+%d to Mana (Based on Character Level)
	case 242: //+%d to Maximum Stamina (Based on Character Level)
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 2048);
		lpBuffer->bFloat = TRUE;
		break;

	//////////////////////////////////////////////////////////////
	// "Based on Character Level" cases, All need to be divided by
	// 8, and Life/Mana/Stamina need to be further divided by 256.
	//////////////////////////////////////////////////////////////
	case 214: //+%d to Defense (Based on Character Level)
	case 215: //+%d%% to Defense (Based on Character Level)
	case 218: //+%d to Maximum Damage (Based on Character Level)
	case 219: //+%d%% to Maximum Damage (Based on Character Level)
	case 220: //+%d to Strength (Based on Character Level)
	case 221: //+%d to Dexterity (Based on Character Level)
	case 222: //+%d to Energy (Based on Character Level)
	case 223: //+%d to Vitality (Based on Character Level)
	case 224: //+%d to Attack Rating (Based on Character Level)
	case 225: //%d%% Bonus to Attack Rating (Based on Character Level)
	case 226: //+%d to Maximum Cold Damage (Based on Character Level)
	case 227: //+%d to Maximum Fire Damage (Based on Character Level)
	case 228: //+%d to Maximum Lightning Damage (Based on Character Level)
	case 229: //+%d to Maximum Poison Damage (Based on Character Level)
	case 230: //+%d to Maximum Cold Resist (Based on Character Level)
	case 231: //+%d to Maximum Fire Resist (Based on Character Level)
	case 232: //+%d to Maximum Resist Damage (Based on Character Level)
	case 233: //+%d to Maximum Poison Resist (Based on Character Level)
	case 234: //+%d to Cold Absorb (Based on Character Level)
	case 235: //+%d to Fire Absorb (Based on Character Level)
	case 236: //+%d to Lightning Absorb (Based on Character Level)
	case 237: //+%d to Poison Absorb (Based on Character Level)
	case 238: //Attacker Takes Damage of %d (Based on Character Level)
	case 239: //%d%% Extra Gold (Based on Character Level)
	case 240: //%d%% Better Chance of Getting Magic Items (Based on Character Level)
	case 241: //Heal Stamina Plus %d%% (Based on Character Level)
	case 243: //%d%% Damage to Demons (Based on Character Level)
	case 244: //%d%% Damage to Undead (Based on Character Level)
	case 245: //%d%% Bonus to Attack Rating Against Demons (Based on Character Level)
	case 246: //%d%% Bonus to Attack Rating Against Undead (Based on Character Level)
	case 247: //%d%% Crushing Blow (Based on Character Level)
	case 248: //%d%% Open Wounds (Based on Character Level)
	case 249: //%d Kick Damage (Based on Character Level)
	case 250: //%d%% Deadly Strike (Based on Character Level)
		lpBuffer->aValues[0] = Float2Int(lpBuffer->aValues[0], 8);
		lpBuffer->bFloat = TRUE;
		break;	

	case 252: //Repairs 1 durability in %d seconds
		lpBuffer->aValues[0] = 100 / lpBuffer->aValues[0];
		lpBuffer->bFloat = TRUE;
		break;	

	default:
		bOK = FALSE;
		break;
	}

	return bOK;
}