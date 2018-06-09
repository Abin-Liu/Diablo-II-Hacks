//////////////////////////////////////////////////////////////////////////
// BuiltinVar.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////

#include "BuiltinVar.h"
#include <string.h>
#include "D2HackIt.h"
#include "ModDef.h"

static const DWORD ALLRES_IDS[4] = { 39, 41, 43, 45 }; 
static const DWORD ALLATTR_IDS[4] = { 0, 1, 2, 3 }; 

int GetBV_type(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_quality(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_quantity(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_sockets(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_ethereal(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_identified(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_value(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_level(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_defense(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_runeword(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_allres(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_allattr(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_sizex(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_sizey(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_strreq(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_dexreq(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_levelreq(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_twohand(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_mindamage(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_maxdamage(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);

static const BUILTINVAR BVS[] = {
	{ "code", NULL, 0 },
	{ "type", GetBV_type, 0 },
	{ "quality", GetBV_quality, 0 },
	{ "quantity", GetBV_quantity, 0 },
	{ "sockets", GetBV_sockets, 0 },
	{ "ethereal", GetBV_ethereal, 0 },
	{ "identified", GetBV_identified, 0 },
	{ "value", GetBV_value, 0 },
	{ "level", GetBV_level, 0 },
	{ "defense", GetBV_defense, 0 },
	{ "runeword", GetBV_runeword, 0 },
	{ "allres", GetBV_allres, 0 },
	{ "allattr", GetBV_allattr, 0 },
	{ "sizex", GetBV_sizex, 0 },
	{ "sizey", GetBV_sizey, 0 },	
	{ "strreq", GetBV_strreq, 0 },
	{ "dexreq", GetBV_dexreq, 0 },
	{ "levelreq", GetBV_levelreq, 0 },
	{ "twohand", GetBV_twohand, 0 },
	{ "mindamage", GetBV_mindamage, 0 },
	{ "maxdamage", GetBV_maxdamage, 0 },
	{ "damage", GetBV_maxdamage, 0 },

	// item quality constants
	{ "inferior", NULL, 1 },
	{ "normal", NULL, 2 },
	{ "superior", NULL, 3 },
	{ "magical", NULL, 4 },
	{ "set", NULL, 5 },
	{ "rare", NULL, 6 },
	{ "unique", NULL, 7 },
	{ "craft", NULL, 8},

	// item type constants
	{ "shield", NULL, 2 },
	{ "armor", NULL, 3 },
	{ "boots", NULL, 15 },
	{ "gloves", NULL, 16 },
	{ "belt", NULL, 19 },
	{ "scepter", NULL, 24 },
	{ "wand", NULL, 25},
	{ "staff", NULL, 26 },
	{ "bow", NULL, 27 },
	{ "axe", NULL, 28 },
	{ "club", NULL, 29 },
	{ "sword", NULL, 30 },
	{ "hammer", NULL, 31 },
	{ "dagger", NULL, 32 },
	{ "spear", NULL, 33 },
	{ "polearm", NULL, 34 },
	{ "crossbow", NULL, 35 },
	{ "mace", NULL, 36 },
	{ "helm", NULL, 37 },
	{ "throwknife", NULL, 42 },
	{ "throwaxe", NULL, 43 },
	{ "javelin", NULL, 44},
	{ "orb", NULL, 68 },
	{ "palshield", NULL, 70 },
	{ "barhelm", NULL, 71 },
	{ "rune", NULL, 74 },
	{ "circlet", NULL, 75 },
	{ "claw", NULL, 88},
	{ "amajavelin", NULL, 87 },
	{ "amabow", NULL, 85 },
	{ "amaspear", NULL, 86 },
	{ "druhelm", NULL, 72 },
	{ "necshield", NULL, 69 },
	
	{ "", NULL, 0 }
};

BOOL operator==(const BUILTINVAR& lhs, const BUILTINVAR& rhs)
{
	return ::strcmp(lhs.szName, rhs.szName) == 0;
}

BOOL operator>(const BUILTINVAR& lhs, const BUILTINVAR& rhs)
{
	return ::strcmp(lhs.szName, rhs.szName) > 0;
}

CBuiltinVarDictionary::CBuiltinVarDictionary()
{
	m_aVars.RemoveAll();
	for (int i = 0; BVS[i].szName[0]; i++)
		m_aVars.Add(BVS[i]);
	m_aVars.Sort();
}

LPCBUILTINVAR CBuiltinVarDictionary::Lookup(LPCSTR lpszVarName) const
{
	if (lpszVarName == NULL || *lpszVarName == 0)
		return NULL;

	BUILTINVAR temp;
	::strcpy(temp.szName, lpszVarName);
	int nIndex = m_aVars.Find(temp);
	if (nIndex == -1)
		return NULL;

	const BUILTINVAR& bv = m_aVars.GetAt(nIndex);
	return &bv;
}

BOOL CBuiltinVarDictionary::Lookup(LPCSTR lpszVarName, DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount, int& rValue) const
{
	rValue = 0;
	LPCBUILTINVAR p = Lookup(lpszVarName);
	if (p == NULL)
		return FALSE;

	rValue = GetBuiltinVarValue(p, dwItemID, lpszItemCode, lpMod, dwModCount);	
	return TRUE;
}

int GetBuiltinVarValue(LPCBUILTINVAR pbv, DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	if (pbv == NULL)
		return 0;

	if (pbv->GetBuiltinValue == NULL)
		return pbv->nConstant;
	else
		return pbv->GetBuiltinValue(dwItemID, lpszItemCode, lpMod, dwModCount);
}

//////////////////////////////////////////////////////////////////////////////////
// Built-in Value Retrieval
//////////////////////////////////////////////////////////////////////////////////
int GetBV_type(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemType(dwItemID);
}

int GetBV_quality(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemQuality(dwItemID);
}

int GetBV_quantity(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	return GetUnitStat(&gu, STAT_AMMOQUANTITY);
}

int GetBV_sockets(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemSockets(dwItemID);
}

int GetBV_ethereal(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return (GetItemAttributes(dwItemID) & ITEM_ETHEREAL) != 0;
}

int GetBV_identified(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return (GetItemAttributes(dwItemID) & ITEM_IDENTIFIED) != 0;
}

int GetBV_value(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	if (::strcmp(lpszItemCode, "gld"))
	{
		return GetItemPrice(dwItemID, 513, FALSE);
	}
	else
	{
		return GetUnitStat(&gu, STAT_GOLD);
	}		
}

int GetBV_level(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return (int)GetItemLevel(dwItemID);
}

int GetBV_defense(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	return (int)GetUnitStat(&gu, 31);
}

int GetBV_runeword(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return (GetItemAttributes(dwItemID) & ITEM_RUNEWORD) != 0;
}

int GetBV_allres(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	int nAllRes = 65535;
	for (int i = 0; i < 4; i++)
	{
		LPCITEMMOD p = CModDef::CheckItemMods(lpMod, dwModCount, ALLRES_IDS[i]);
		if (p == NULL)
			return 0;

		nAllRes = min(nAllRes, p->aValues[0]);
	}
		
	return nAllRes;
}

int GetBV_allattr(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	int nAllSttr = 65535;
	for (int i = 0; i < 4; i++)
	{
		LPCITEMMOD p = CModDef::CheckItemMods(lpMod, dwModCount, ALLATTR_IDS[i]);
		if (p == NULL)
			return 0;

		nAllSttr = min(nAllSttr, p->aValues[0]);
	}
		
	return nAllSttr;
}

int GetBV_sizex(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	SIZE sz = GetItemSize2(dwItemID);
	return sz.cx;
}

int GetBV_sizey(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	SIZE sz = GetItemSize2(dwItemID);
	return sz.cy;
}

int GetBV_strreq(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemStrRequirement(dwItemID);
}

int GetBV_dexreq(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemDexRequirement(dwItemID);
}

int GetBV_levelreq(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemLevelRequirement(dwItemID);
}

int GetBV_twohand(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return IsTwoHandItem(dwItemID);
}

int GetBV_mindamage(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	DWORD dwVal;
	GetItemDamage(dwItemID, &dwVal, NULL);
	return (int)dwVal;
}

int GetBV_maxdamage(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	DWORD dwVal;
	GetItemDamage(dwItemID, NULL, &dwVal);
	return (int)dwVal;
}
